#include "spobvp.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <iostream>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
//#include <filesystem>
//namespace fs = std::filesystem;

using namespace eac;
using namespace spobvp;


eac::EngineSettings get_engine_settings(eac::DeviceType device, const std::string& config_dir)
{
    eac::EngineSettings settings;
    settings.type = device;
    settings.max_batch_size = 1;
    settings.engine_path = config_dir;
    if (device == eac::DeviceType::Intel_CPU || device == eac::DeviceType::Intel_GPU)
    {
        settings.plugin_path = ".";
    }
    else if (device == eac::DeviceType::Nvidia_GPU)
    {
        settings.plugin_path = ".";
    }
    else
    {
        std::cerr << "Incorrect device type! Type: " << static_cast<int>(device) << std::endl;
        exit(EXIT_FAILURE);
    }
    return settings;
}



void SPOBVP::InitDetector(QString path_to_model)
{
    std::string onnx_model_path = path_to_model.toStdString();

    std::string token = fs::path(onnx_model_path).stem().string();
    std::transform(token.begin(), token.end(), token.begin(), ::toupper);

    std::string engines_path = fs::path(onnx_model_path).parent_path().string();

    DeviceType device = DeviceType::Intel_CPU;
    DetectorInitializationParams initialization_params;

    initialization_params.detector_type = DetectorType::SSD_RESNET;
    initialization_params.labels = {"Bus",                                "Car",
                                    "Group",                                "Home",
                                    "Tank",                                "Truck"};
    initialization_params.landmarks_size = 0;

    detector = std::unique_ptr<IDetector>(IDetector::create(initialization_params));
    if (!detector)
    {
        std::cerr << "Failed to create Detector object!\n";
        exit(EXIT_FAILURE);
    }

    status = detector->initialize("SSD_RESNET_18", onnx_model_path, get_engine_settings(device, engines_path));

    if (status != eac::BioCoreStatusCode::Success)
    {
        std::cout << "Detector status: " << "init error" << "\n" << status.description << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string settings
        {
            "{\n"
            "   \"input_data\":[\n"
            "      {\n"
            "         \"type\":\"img_path\",\n"
            "         \"confidence_thr\":0.25,\n"
            "         \"iou_thr\":0.7,\n"
            "         \"min_size\":0,\n"
            "         \"max_size\":1\n"
            "      }\n"
            "   ]\n"
            "}"
        };

    status = detector->set_settings(settings);
    if (status != eac::BioCoreStatusCode::Success)
    {
        std::cout << "Set settings to Detector status: " << "error" << "\n" << status.description << std::endl;
    }

}

SPOBVP_STATUS SPOBVP::Process(const ImageArray tv, const ImageArray ir, TargetCoordinates *detectTarget)
{

    detectTarget->x = -10;
    detectTarget->y = -10;
    detectTarget->found = false;
    double distance = 1000;
    eac::ImageView input(640, 480, tv.image, eac::GRAY_8bitppx);
    status = detector->process(input);
    if (status != eac::BioCoreStatusCode::Success)
    {
        std::cout << "Detector status: " << "error" << "! Description: " << status.description
                  << std::endl;
    }

    auto objects_count = detector->count();
    std::cout << "Object count: " << objects_count << std::endl;
    bool find = false;
    for (size_t i = 0; i < objects_count; ++i)
    {
        if(!find) //убрать условие чтобы поиск был для ближайшей к центру изображения цели
        {
            find = true;
            auto d_item = detector->get_detect_item(i);
            std::cout << "[ " << i << " ] " << d_item << std::endl;
            Bbox target_coord = d_item.get_bbox();

            float x = target_coord.get_relative_x()*640;
            float y = target_coord.get_relative_y()*480;
            float width = target_coord.get_relative_width()*640;
            float height = target_coord.get_relative_height()*480;
            double temp_dist = sqrt(pow(x + width/2.0 - 320,2) + pow(y + height/2.0 - 240, 2));
            if(temp_dist < distance){
                distance = temp_dist;
                detectTarget->x = x + width/2.0 ;//+ fco::firstColumn ;
                detectTarget->y = y + height/2.0 ;//+  fco::firstRow;
                detectTarget->w = width;
                detectTarget->h = height;
                detectTarget->found = true;
            }

        }

    }



    return SPOBVP_SUCCESS;
}
