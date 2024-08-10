#pragma once

#include "image.cc"

#include <string>

namespace ImageLoader
{
    void Initialize();
    void Finalize();

    CImage* LoadImageFromPPMFile(std::string _FileName);
}