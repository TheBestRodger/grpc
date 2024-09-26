#pragma once
#include <cstdint>
#include <vector>
#pragma pack(push, 1)

struct PictureRecieve
{
    std::vector<uint8_t> pix;
    unsigned size_pix{};
    
    unsigned height{};
    unsigned width{};
};

struct OESKN_out
{
    double fiv{};
    double fig{};

    double pelengHor{};
    double pelengVer{};

    double omv{};
    double omg{};

    int ZG{};
};



#pragma pack(pop)