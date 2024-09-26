#include <iostream>
#include <memory>
#include <string>
#include <fstream>      // std::ifstream
#include <filesystem>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "mmskn.grpc.pb.h"
#include "mmskn.pb.h"

#include "grpc/grpc_helper.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;
using grpc::ServerWriteReactor;
using grpc::CallbackServerContext;
using grpc::ServerUnaryReactor;

using mmskn::v1::PictureResponse;
using mmskn::v1::PictureRequest;
using mmskn::v1::MmsknService;

namespace fs = std::filesystem;

struct DataBuffer 
{
	std::mutex tv_mutex;
	Picture_out tv_data;

	std::mutex ir_mutex;
	Picture_out ir_data;
};


class GrpcHelper::Private
{
public:
	int port{ 0 };
	bool usetls{ false };
	std::unique_ptr<Server> server;
	std::shared_ptr<std::thread> workerThread;

	// Для отправки
	DataBuffer buffer;
};

// Логика и данные, лежащие в основе поведения сервера
class ConnectionServiceServiceImpl final : public MmsknService::CallbackService
{
public:
	explicit ConnectionServiceServiceImpl(DataBuffer& buffer)	:
		buf(buffer)
	{
	}

	ServerUnaryReactor* GetPictureTV(CallbackServerContext* context, 
		const PictureRequest* request,
		PictureResponse* reply) override {

		std::lock_guard<std::mutex> lock(buf.tv_mutex);

		if (buf.tv_data.pix.empty()) {
			auto* reactor = context->DefaultReactor();
			reactor->Finish(grpc::Status::CANCELLED);
			return reactor;
		}

		reply->set_pix(std::string(buf.tv_data.pix.begin(), buf.tv_data.pix.end()));
		reply->set_height(buf.tv_data.height);
		reply->set_width(buf.tv_data.width);
		reply->set_size_pix(buf.tv_data.size_pix);

		auto* reactor = context->DefaultReactor();
		reactor->Finish(grpc::Status::OK);
		return reactor;
	}

	ServerUnaryReactor* GetPictureIR(CallbackServerContext* context, 
		const PictureRequest* request,
		PictureResponse* reply) override {

		std::lock_guard<std::mutex> lock(buf.ir_mutex);

		if (buf.ir_data.pix.empty()) {
			auto* reactor = context->DefaultReactor();
			reactor->Finish(grpc::Status::CANCELLED);
			return reactor;
		}

		reply->set_pix(std::string(buf.ir_data.pix.begin(), buf.ir_data.pix.end()));
		reply->set_height(buf.ir_data.height);
		reply->set_width(buf.ir_data.width);
		reply->set_size_pix(buf.ir_data.size_pix);

		auto* reactor = context->DefaultReactor();
		reactor->Finish(grpc::Status::OK);
		return reactor;
	}

protected:
	DataBuffer& buf;

};

std::string readAll(std::string const& fileName)
{
	if (!fileName.empty() && fs::exists(fileName)) {
		std::ifstream ifs(fileName, std::ifstream::in | std::ifstream::binary);
		if (ifs.is_open()) {
			std::string content{ std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>() };
			return content;
		}
	}

	return std::string();
}

GrpcHelper::GrpcHelper() :
	m_data(new GrpcHelper::Private)
{
}

GrpcHelper::~GrpcHelper()
{
	shutdownServer();
	delete m_data;
}

void GrpcHelper::startServer(int port)
{
	m_data->port = port;

	m_data->workerThread = std::make_shared<std::thread>([&]() {
		std::string server_address = absl::StrFormat("0.0.0.0:%d", port);

		ConnectionServiceServiceImpl service(m_data->buffer);

		grpc::EnableDefaultHealthCheckService(true);
		grpc::reflection::InitProtoReflectionServerBuilderPlugin();

		std::shared_ptr<grpc::ServerCredentials> creds;

		if (m_data->usetls) {
			// Использование безопасного соединения
			auto options = grpc::SslServerCredentialsOptions(GRPC_SSL_REQUEST_AND_REQUIRE_CLIENT_CERTIFICATE_AND_VERIFY);

			std::string server_key_file = "credentials/localhost.key";
			std::string client_cert_file = "credentials/localhost.crt";
			std::string server_cert_file = "credentials/root.crt";

			options.pem_root_certs = readAll(client_cert_file);
			//std::cout << "\npem_root_certs = \n" << options.pem_root_certs << std::endl;

			auto key = grpc::SslServerCredentialsOptions::PemKeyCertPair();
			key.private_key = readAll(server_key_file);
			//std::cout << "\nprivate_key = \n" << key.private_key << std::endl;
			key.cert_chain = readAll(server_cert_file);
			//std::cout << "\ncert_chain = \n" << key.cert_chain << std::endl;
			options.pem_key_cert_pairs.push_back(key);

			creds = grpc::SslServerCredentials(options);
		}
		else {
			creds = grpc::InsecureServerCredentials();
		}

		ServerBuilder builder;
		// builder.SetMaxReceiveMessageSize(std::numeric_limits<int>::max());

		// Listen on the given address without any authentication mechanism.
		builder.AddListeningPort(server_address, creds);
		// Register "service" as the instance through which we'll communicate with
		// clients. In this case it corresponds to an *synchronous* service.
		builder.RegisterService(&service);

		// Finally assemble the server.
		m_data->server = (builder.BuildAndStart());
		std::cout << "\nGRPC Server listening on " << server_address
			<< " useTLS=" << (m_data->usetls ? "YES" : "NO")
			<< std::endl;

		// Сервер должен быть либо выключен, либо какой-то другой поток должен вызвать 
		// Shutdown, чтобы эта функция когда-либо вернулась
		m_data->server->Wait();
	});
	
}

void GrpcHelper::shutdownServer()
{
	if (m_data->workerThread) {
		std::cout << "Shutdown GRPC Server" << std::endl;

		if (m_data->server) {
			m_data->server->Shutdown();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		m_data->workerThread->join();

		if (m_data->server) {
			m_data->server.reset();
		}

		m_data->workerThread.reset();
	}
}

bool GrpcHelper::isServerWorking() const
{
	return !!m_data->workerThread;
}

void GrpcHelper::set_tv_data(Picture_out& data)
{
	std::lock_guard<std::mutex> lock(m_data->buffer.tv_mutex);
	m_data->buffer.tv_data = data;
}

void GrpcHelper::set_ir_data(Picture_out& data)
{
	std::lock_guard<std::mutex> lock(m_data->buffer.ir_mutex);
	m_data->buffer.ir_data = data;
}

