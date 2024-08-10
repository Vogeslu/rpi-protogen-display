#include "image.h"

CImage::CImage(int _Width, int _Height, FPixel* _pImage)
    : m_Width(_Width)
    , m_Height(_Height)
    , m_pImage(_pImage)
{
}

CImage::~CImage()
{
    Delete();
}

void CImage::Delete()
{
    delete[] m_pImage;
    Reset();
}

void CImage::Reset()
{
    m_pImage = NULL;
    m_Width = -1;
    m_Height = -1;
}

bool CImage::IsValid()
{
    return m_pImage && m_Width > 0 && m_Height > 0;
}

const FPixel& CImage::GetPixel(int _X, int _Y) const
{
    static FPixel black;
    
    if(_X < 0 || _X >= m_Width || _Y < 0 || _Y >= m_Height)
    {
        return black;
    }

    return m_pImage[_X + m_Width * _Y];
}