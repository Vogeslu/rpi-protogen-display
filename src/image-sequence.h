#pragma once

#include <stdio.h>
#include <vector>
#include <chrono>
#include <cstdint>
#include <string>
#include <map>

#include "image.h"

using namespace std::chrono;

class CImageSequence
{
public:
    CImageSequence(std::vector<CImage*> _Images, int _ImageCount, int _Speed, int _WaitAfter);
    ~CImageSequence();

public:
    void AddTransition(CImageSequence* _TargetSequence, CImageSequence* _TransitionSequence);
    CImageSequence* FindTransitionToSequence(CImageSequence* _TargetSequence);

    void Delete();
    void Reset();
    bool IsValid();

    void Restart();

    const CImage* GetImage() const;
    bool NextImage();

    bool CheckImageSwitch();

private:
    uint64_t GetCurrentMilliseconds();

private:
    std::vector<CImage*> m_Images;
    int m_ImageCount;
    int m_CurrentImage;
    int m_Speed;
    int m_WaitAfter;
    std::map<CImageSequence*, CImageSequence*> m_Transitions;

    uint64_t m_LastImageSwitch;
};