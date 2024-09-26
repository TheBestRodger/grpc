#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "system/rc_shared.h"
#include "system/dmdefs.h"
#include "system/window_sys.h"
#include "system/window_func.h"
#include "core/work_cycle.h"
#include "settings/settings_store.h"

#include "nm/inode_monitor.h"
//#include "atmosphere/radiometry_query.h"
#include "core/framebuffer_helper.h"
#include "log/log.h"

#include "../examples/include/infrared_settings.h"

#include "IOStructs.h"
#include "tcp/tcp.h"

#ifdef USE_GRPC
#include "grpc/grpc_helper.h"
#endif

//#define PRINT_INFO

//rc_shared<cd::atmosphere_query::IRadiometryQueryManager> radiometryQueryManager;
rc_shared<work_cycle::FramebufferHelper> ir_framebuffer;

const size_t MAX_ETALON_SIZE = 32768;
unsigned char etalon[MAX_ETALON_SIZE] = { };

std::vector<unsigned int> ir_image_buffer;
std::vector<unsigned char> ir_bright_buff;

//const char* default_etalon_file_name = R"(D:\OESKN_SPO\bin\RelWithDebInfo\etalon.uso)";
const char* default_etalon_file_name = "";


void start_world(const std::string& etalon_file_name, size_t g_Width, size_t g_Height)
{
	// Инициализация визуализации
	auto config_file = settings::getPathToLocalConfigs("infrared_camera_param.yml");
	if (!settings::loadFile("infrared_tester", config_file.c_str()))
		throw std::runtime_error("Can't load infrared_tester.yml");
	std::cout << config_file;

	deskman::DMRect win_rect(0, 0, g_Width, g_Height); // Окно

	DWORD creationFlags = 0;
	creationFlags |= cd::syswindow::CWM_CAPTION;

	cd::syswindow::window_handle hwnd = cd::syswindow::Create(win_rect, "ММ ОЭСКН", 0, creationFlags);
	if (hwnd == cd::syswindow::INVALID_WINDOW)
		throw std::runtime_error("Cannot create window");

	cd::syswindow::ShowMouse(true);

	// Мир
	const char* app_xml = "App_infrared.xml";
	//const char* app_xml = "ObjectViewer.xml";
	settings::parseXml(app_xml);
	if (etalon_file_name.empty()) {
		work_cycle::initialize(0, nullptr, app_xml);
	}
	else {
		int argc = 1;
		const char* argv[]{
			etalon_file_name.c_str(),
			nullptr
		};
		work_cycle::initialize(argc, argv, app_xml);
	}

	//cd::world::setDateTime(2019, 07, 18, 16, 0, 0); // полнолуние
	cd::world::setDateTime(2019, 07, 18, 14, 0, 0); // полнолуние

	IRefCount::coCreateInstance("v21_loader_dll", ir_framebuffer);
	if (!ir_framebuffer)
		throw std::runtime_error("NO framebufferHelper");
	ir_framebuffer->init(g_Width, g_Height, 1);//1
}

void make_camera(rc_shared<cd::world::ICamera>& vcamera, size_t g_Width, size_t g_Height, int fov_g, int fog_v)
{
	// Камера 
	vcamera = cd::world::createCamera("Camera_3D_1");
	// ===========================================================
	// Для отладки если поставить
	// Широта: 43.43
	// Долгота: 39.93
	// Высота примерно: 2000
	// 
	// Курс и крен: 0
	// Тангаж: -88.8
	// 
	// Угла координатора 0
	// 
	// Должен появится вид сверху на Адлер
	// ===========================================================
	//
	vcamera->setPositionGeo(cd::geo::geovector_wgs84d(43.43, 39.94, 2000.0));
	vcamera->setRotation(cd::orientation_deg(-90, 0.0, 0.0));
	
	// Отрисовка камеры
	rc_shared<work_cycle::InfraredDrawCamera> drawCameraIR;
	drawCameraIR = work_cycle::createInfraredCamera(vcamera->getName());
	drawCameraIR->setHorizontalFov(cd::angle_degrees(fov_g), g_Width, g_Height);
	drawCameraIR->setViewport(0, 0, g_Width, g_Height);

	// Настройки камеры
	shared_ptr<InfraredSettings> irSettings;
	irSettings = cd::makeSharedPtr<InfraredSettings>();
	irSettings->readSettings();
	irSettings->setParameters(drawCameraIR);

	drawCameraIR->setTargetFrameBuffer(ir_framebuffer->framebufferId());
}

OESKN_in* simulation_mm65_TI()// удалить объект
{
	OESKN_in* p_data = new OESKN_in();
	p_data->distance = 45000;
	p_data->B = 43.40;
	p_data->L = 39.93;
	p_data->H = 2000;
	p_data->tet = -88.7;
	p_data->psi = 0;
	p_data->gam = 0;
	return p_data;
}

OESKN_in* simulation_mm65_TV()// удалить объект
{
	OESKN_in* p_data = new OESKN_in();
	p_data->distance = 45000;
	p_data->B = 43.40;
	p_data->L = 39.93;
	p_data->H = 2000;
	p_data->tet = -88.7;
	p_data->psi = 0;
	p_data->gam = 0;
	return p_data;
}

int main(int _argc, const char** _argv)
{
	//_argc = 2;
	//_argv[1] = "D:\\OESKN_SPO\\bin\\RelWithDebInfo\\etalon.uso";

	const size_t g_Width = 640, g_Height = 480;
	const double timeStep = 0.02; // 50 Гц/фпс

	const double fov_g = 14.16; // Угол обзора камеры координатора (горизонтальный)
	const double fov_v = 13.33; // Угол обзора камеры координатора (вертикальный)

	bool frontier = false, hadZG = false;// выход на рубеж, признак захвата

	std::string etalon_file_name;

	if (_argc < 2)
	{
		etalon_file_name = default_etalon_file_name;
		
		//std::cerr << "usage: " << _argv[0] << " <etalon file name>" << std::endl;
		//return 1;
	}
	else {
		etalon_file_name = _argv[1];
	}


	try
	{
		// Чтение эталона
		if (!etalon_file_name.empty())
		{
			std::ifstream in_file;
			in_file.open(etalon_file_name, std::ios::binary);
			if (!in_file.good())
				throw std::runtime_error("Failed to open etalon file");
			in_file.read((char*)etalon, MAX_ETALON_SIZE);
			in_file.close();
		}

#ifdef USE_GRPC
		const int portGRPC = 50051;
		GrpcHelper grpcHelper;
		grpcHelper.startServer(portGRPC);
#endif

		//boost::asio::io_context io_context_1;
		//boost::asio::io_context io_context_TV;
		//boost::asio::io_context io_context_IR;
		Picture_out out_pack = {};

		cd::syswindow::SysEvent ev;
		start_world(etalon_file_name, g_Width, g_Height);
		
		rc_shared<cd::world::ICamera> vcamera;
		make_camera(vcamera, g_Width, g_Height, fov_g, fov_v);

		bool is_running = true;
		double sdvig = 0;
		auto dataTI = simulation_mm65_TI();
		auto dataTV = simulation_mm65_TV();
		int send_5 = 0;
		while (is_running)
		{
			while (cd::syswindow::GetEvent(&ev))
			{
				if (ev.type == cd::syswindow::DMI_DESTROY && ev.param == 0)
				{
					is_running = false;
					break;
				}
			}


			//m_socket->receive(boost::asio::buffer(m_buffer), 0, ec); // Ожидание прихода исходных данных
			//OESKN_in* p_data = receive_data(io_context_1);
			//OESKN_in* p_data = reinterpret_cast<OESKN_in*>(m_buffer.data());
			dataTI->B += sdvig;
			dataTV->B += sdvig;
			sdvig += 0.00000005;

#ifdef PRINT_INFO
			std::cout << "Distance: " << dataTI->distance << std::endl;
			std::cout << "Latitude: " << dataTI->B << " Longitude: " << dataTI->L << " Height: " << dataTI->H << std::endl;
#endif

			// Установить положение камеры
			vcamera->setPositionGeo(cd::geo::geovector_wgs84d(dataTI->B, dataTI->L, dataTI->H));
			vcamera->setRotation(cd::orientation_rad(dataTI->tet, dataTI->psi, dataTI->gam));
			//======================================================================================

			//-- запускаем рабочий цикл 21 версии --
			cd::world::shiftTime(timeStep);
			work_cycle::life();
			ir_framebuffer->drawToScreen(0, 0);
			cd::syswindow::SwapBuffer(0, 0);
			
			// Отправка снимков
			{
				int w = ir_framebuffer->getWidht(); // рамка
				int h = ir_framebuffer->getHeight(); // рамка

				CDMembuf data;// пиксели

				if (!ir_framebuffer->getData(&data)) // получения пикселей
					throw std::runtime_error("FramebufferHelper::getData() == false");

				out_pack.pix.resize(data.Size());
				std::copy(data.Base(), data.Base() + data.Size(), out_pack.pix.begin());

				//Debug
				auto tmp = reinterpret_cast<uint8_t*>(data.Base());
				bool f1 = false;
				for (int i = 0; i < data.Size(); ++i)
					if (tmp[i] != 0) {
						f1 = true;
						break;
					}

				// если изначальная рамка отличается
				const int size = w * h;
				if (ir_image_buffer.size() < size)
					ir_image_buffer.resize(size);
				if (ir_bright_buff.size() < size)
					ir_bright_buff.resize(size);

				data.Seek(0);
				if (data.Read(ir_image_buffer.data(), ir_image_buffer.size() * sizeof(char)) != NMS_OK) // проверка
					throw std::runtime_error("CDMembuf::Read() != NMS_OK");

				out_pack.height = g_Height;
				out_pack.width = g_Width;
				out_pack.size_pix = data.Size();

				if (f1) {
					std::reverse(out_pack.pix.begin(), out_pack.pix.end());
#ifdef USE_GRPC
					grpcHelper.set_tv_data(out_pack);
					grpcHelper.set_ir_data(out_pack);
#else
					std::thread t1(send_TV_data, out_pack);
					std::thread t2(send_IR_data, out_pack);
					t1.join();
					t2.join();
					//send_TV_data(io_context_TV, out_pack);
					//send_IR_data(io_context_IR, out_pack);
#endif
				}
			}
		}
		 
		delete dataTI;
		delete dataTV;
	}
	catch (const std::exception& _err)
	{
		std::cerr << _err.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Something went wrong" << std::endl;
	}

	//if (radiometryQueryManager)
	//	radiometryQueryManager.reset();
	if (ir_framebuffer)
		ir_framebuffer.reset();

	work_cycle::destroy();
	return 0;
}
