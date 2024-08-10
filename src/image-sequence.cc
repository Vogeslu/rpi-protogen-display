#include "image-sequence.h"

#include <assert.h>

CImageSequence::CImageSequence(std::vector<CImage*> _Images, int _ImageCount, int _Speed, int _WaitAfter)
    : m_Images(_Images)
    , m_ImageCount(_ImageCount)
    , m_CurrentImage(0)
    , m_Speed(_Speed)
    , m_WaitAfter(_WaitAfter)
{
    m_LastImageSwitch = GetCurrentMilliseconds();
}

CImageSequence::~CImageSequence()
{
    Delete();
}

void CImageSequence::Delete()
{
    Reset();
}

void CImageSequence::Reset()
{
    m_Images.clear();
    m_ImageCount = -1;
    m_CurrentImage = 0;
    m_Speed = 0;
    m_WaitAfter = 0;
}

void CImageSequence::Restart()
{
    m_CurrentImage = 0;
    m_LastImageSwitch = GetCurrentMilliseconds();
}

bool CImageSequence::IsValid()
{
    return !m_Images.empty() && m_ImageCount > 0;
}

const CImage* CImageSequence::GetImage() const
{
    assert(m_CurrentImage < m_ImageCount);

    return m_Images[m_CurrentImage];
}

void CImageSequence::NextImage()
{
    m_CurrentImage = (m_CurrentImage + 1) % m_ImageCount;
}

void CImageSequence::CheckImageSwitch()
{
    uint64_t milliseconds = GetCurrentMilliseconds();

    int waitTime = (m_CurrentImage + 1) >= m_ImageCount ? m_WaitAfter : m_Speed;

    if ((m_LastImageSwitch + waitTime) > milliseconds)
    {
        return;
    }

    NextImage();
    m_LastImageSwitch = milliseconds;
}

uint64_t CImageSequence::GetCurrentMilliseconds()
{
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}