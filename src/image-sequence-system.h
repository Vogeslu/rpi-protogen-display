#pragma once

#include "image-sequence.h"

namespace ImageSequenceSystem
{
    void Initialize();
    void Finalize();

    CImageSequence* FindImageSequence(std::string _SequenceName);
}