#pragma once

#include "pixel.h"

class CImage {
public:
    CImage(int _Width, int _Height, FPixel* _pImage);
    ~CImage();

public:
    void Delete();
    void Reset();
    bool IsValid();

    const FPixel& GetPixel(int _X, int _Y) const;

private:
    int m_Width;
    int m_Height;
    FPixel* m_pImage;
};