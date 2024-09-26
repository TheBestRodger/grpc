#ifndef SPOBVP_H
#define SPOBVP_H
#include "eac_errors.h"
#include "object_detection_types.h"
#include "idetector.h"
#include "structs.h"
#include <cmath>
#include <QDebug>
#endif // SPOBVP_H
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
namespace spobvp{
    enum SPOBVP_STATUS
    {
        SPOBVP_SUCCESS = 0,	//Успешное выполнение модели
        SPOBVP_NULLPTR = 1,	//Один из переданных входных указателей - нулевой
        SPOBVP_BADSIZE = 2	//Размер входного изображения не равен 640x480
    };

    class SPOBVP
    {
    public:
        std::unique_ptr<eac::IDetector> detector;
        eac::BioCoreStatus status;
        bool flag_ZG = false;
        void InitDetector(QString path_to_model);
        SPOBVP_STATUS Process(const ImageArray tv, const ImageArray ir, TargetCoordinates *detectTarget);
    };
}


inline std::ostream& operator<<(std::ostream& os, const eac::Bbox& bbox)
{
    os << "x: " << bbox.get_relative_x() << " y: " << bbox.get_relative_y()
    << " width: " << bbox.get_relative_width() << " height: " << bbox.get_relative_height()
    << " id: " << bbox.get_id();
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const eac::DetectItem& item)
{
    os << "bbox: " << item.get_bbox() << " category: " << item.get_category_name() << " confidence: " << item.get_confidence();
    return os;
}




