// Copyright 2024 DVG
// contact@dvg.spb.ru
//

#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <fstream>      // std::ifstream
#include <filesystem>
#include <future>

#include <grpcpp/grpcpp.h>

#include "mmskn.grpc.pb.h"
#include "mmskn.pb.h"

#include "grpc/grpc_helper.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using mmskn::v1::PictureResponse;
using mmskn::v1::PictureRequest;
using mmskn::v1::MmsknService;

namespace fs = std::filesystem;


class MmsknServiceClient {
public:
    explicit MmsknServiceClient(std::shared_ptr<Channel> channel) :
        stub_(MmsknService::NewStub(channel)) {
    }

    bool GetPictureTV(PictureRecieve* pictureRecieve, std::string& message) {

        // Данные, которые мы отправляем на сервер
        PictureRequest request;
        // Задать параметры запроса если они есть

        // Контейнер для данных, которые мы ожидаем от сервера
        PictureResponse response;

        // Контекст для клиента. Может быть использован для передачи дополнительной информации 
        // на сервер и/или настройки определенного поведения RPC
        ClientContext context;

        std::mutex mu;
        std::condition_variable cv;
        bool done = false;
        Status status;
        stub_->async()->GetPictureTV(&context, &request, &response, [&](Status s) {
            status = std::move(s);
            std::lock_guard<std::mutex> lock(mu);
            done = true;
            cv.notify_one();
            });
        std::unique_lock<std::mutex> lock(mu);
        while (!done) {
            cv.wait(lock);
        }

        // Ошибка RPC при выполнении запроса
        if (!status.ok()) {
            message = StatusErrorString(status);
            return false;
        }

        // Параметры ответа от сервера
        if (pictureRecieve) {
            pictureRecieve->pix = std::vector<uint8_t>(response.pix().begin(), response.pix().end());
            pictureRecieve->height = response.height();
            pictureRecieve->width = response.width();
            pictureRecieve->size_pix = response.size_pix();
            return true;
        }

        return false;
    }

    bool GetPictureIR(PictureRecieve* pictureRecieve, std::string& message) {

        // Данные, которые мы отправляем на сервер
        PictureRequest request;
        // Задать параметры запроса если они есть

        // Контейнер для данных, которые мы ожидаем от сервера
        PictureResponse response;

        // Контекст для клиента. Может быть использован для передачи дополнительной информации 
        // на сервер и/или настройки определенного поведения RPC
        ClientContext context;

        std::mutex mu;
        std::condition_variable cv;
        bool done = false;
        Status status;
        stub_->async()->GetPictureIR(&context, &request, &response, [&](Status s) {
            status = std::move(s);
            std::lock_guard<std::mutex> lock(mu);
            done = true;
            cv.notify_one();
            });
        std::unique_lock<std::mutex> lock(mu);
        while (!done) {
            cv.wait(lock);
        }

        // Ошибка RPC при выполнении запроса
        if (!status.ok()) {
            message = StatusErrorString(status);
            return false;
        }

        // Параметры ответа от сервера
        if (pictureRecieve) {
            pictureRecieve->pix = std::vector<uint8_t>(response.pix().begin(), response.pix().end());
            pictureRecieve->height = response.height();
            pictureRecieve->width = response.width();
            pictureRecieve->size_pix = response.size_pix();
            return true;
        }

        return false;
    }


private:
    std::string StatusErrorString(const Status& status) const
    {
        std::string result = std::string("Запрос на сервер завершился с ошибкой: ") +
            std::to_string(status.error_code()) + ": " + status.error_message();
        std::cout << result << std::endl;
        return result;
    }

private:
    std::unique_ptr<MmsknService::Stub> stub_;

};



class GrpcHelper::Data
{
public:
    // Хост
    std::string host{ "localhost" };

    // Порт
    int port{ 50051 };

    // Параметры для SSL подключения
    bool useTls{ false };

    std::shared_ptr<grpc::ChannelCredentials> creds{ grpc::InsecureChannelCredentials() };

    std::string errorMessage;

    std::string serverAddress() const { return host + ":" + std::to_string(port); };
};


GrpcHelper::GrpcHelper() :
    data(std::make_shared<Data>())
{
}

const std::string GrpcHelper::host() const
{
    return data->host;
}

void GrpcHelper::setHost(const std::string& host)
{
    data->host = host;
}

int GrpcHelper::port() const
{
    return data->port;
}

void GrpcHelper::setPort(int port)
{
    data->port = port;
}

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

void GrpcHelper::setSslCredentials(bool useTls,
    std::string const& serverCertFile, std::string const& clientKeyFile, std::string const& clientCertFile)
{
    data->useTls = useTls;

    //std::cout << "DEBUG SslCredentials" <<
    //    " serverCertFile=" << serverCertFile << 
    //    " clientKeyFile=" << clientKeyFile <<
    //    " clientCertFile=" << clientCertFile << std::endl;

    // Создание объекта учетных данных SSL-канала
    grpc::SslCredentialsOptions options;


    // Буфер, содержащий PEM-кодировку корневых сертификатов сервера. 
    // Если этот параметр пуст, то будут использоваться корни по умолчанию. 
    // Значение по умолчанию можно переопределить, используя GRPC_DEFAULT_SSL_ROOTS_FILE_PATH
    // переменную окружения, указывающую на файл в файловой системе.
    options.pem_root_certs = readAll(serverCertFile);   // root.crt
    //std::cout << "DEBUG pem_root_certs" << options.pem_root_certs << std::endl;

    // Буфер, содержащий PEM-кодировку закрытого ключа клиента. 
    // Этот параметр может быть пустым, если у клиента нет закрытого ключа.
    options.pem_private_key = readAll(clientKeyFile);   // device.key

    // Буфер, содержащий PEM-кодировку цепочки сертификатов клиента.
    // Этот параметр может быть пустым, если у клиента нет сертификата цепи.
    options.pem_cert_chain = readAll(clientCertFile);   // localhost.crt

    data->creds = grpc::SslCredentials(options);

    // Create a channel using the credentials created in the previous step.
    //auto channel = grpc::CreateChannel(data->serverAddress, data->creds);

    /*
    grpc::ChannelArguments  args;
    //args.SetSslTargetNameOverride("2001:420:1101:1::b");
    args.SetSslTargetNameOverride("ems.cisco.com");

    auto channel_creds = grpc::SslCredentials(grpc::SslCredentialsOptions(options));
    data->creds = grpc::CreateCustomChannel(address_str, channel_creds, args);
    */
}


PictureRecieve* GrpcHelper::recieve_img_TV()
{
    data->errorMessage.clear();
    
    std::string buffer, message;
    MmsknServiceClient connectionService(grpc::CreateChannel(data->serverAddress(), data->creds));
    
    PictureRecieve* pictureRecieve = new PictureRecieve;
    bool res = connectionService.GetPictureTV(pictureRecieve, message);
    if (!res) {
        delete pictureRecieve;
        pictureRecieve = nullptr;
    }

    return pictureRecieve;
}

PictureRecieve* GrpcHelper::recieve_img_IR()
{
    data->errorMessage.clear();

    std::string buffer, message;
    MmsknServiceClient connectionService(grpc::CreateChannel(data->serverAddress(), data->creds));

    PictureRecieve* pictureRecieve = new PictureRecieve;
    bool res = connectionService.GetPictureIR(pictureRecieve, message);
    if (!res) {
        delete pictureRecieve;
        pictureRecieve = nullptr;
    }

    return pictureRecieve;
}

const std::string& GrpcHelper::lastErrorMessage() const
{
    return data->errorMessage;
}
