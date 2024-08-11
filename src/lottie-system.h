#pragma once

#include "rlottie.h"

#include "image-sequence.h"

#include <string>

namespace LottieSystem
{
    void Initialize();
    void Finalize();

    CImageSequence* CreateSequenceFromAnimation(std::string _AnimationFileName, int _Speed, int _WaitAfter, bool _Reverse);
}