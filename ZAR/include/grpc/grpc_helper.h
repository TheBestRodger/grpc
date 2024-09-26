#pragma once

#include <memory>
#include <string>

#include "../IOStructs.h"

class GrpcHelper
{

public:
    GrpcHelper();
    ~GrpcHelper();

    void startServer(int port = 50051);
    void shutdownServer();
    bool isServerWorking() const;

    void set_tv_data(Picture_out& data);
    void set_ir_data(Picture_out& data);

private:
    class Private;
    Private* m_data;
};
