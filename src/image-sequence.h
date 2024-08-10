#pragma once

#include <stdio.h>
#include <vector>
#include <chrono>
#include <cstdint>

#include "image.h"

using namespace std::chrono;

class CImageSequence
{
public:
    CImageSequence(std::vector<CImage*> _Images, int _ImageCount, int _Speed, int _WaitAfter);
    ~CImageSequence();

public:
    void Delete();
    void Reset();
    bool IsValid();

    void Restart();

    const CImage* GetImage() const;
    void NextImage();

    void CheckImageSwitch();

private:
    uint64_t GetCurrentMilliseconds();

private:
    std::vector<CImage*> m_Images;
    int m_ImageCount;
    int m_CurrentImage;
    int m_Speed;
    int m_WaitAfter;

    uint64_t m_LastImageSwitch;
};