#pragma once

#include <cstdint>

struct FPixel {
    FPixel()
        : m_Red(0)
        , m_Green(0)
        , m_Blue(0)
    {
    }

    FPixel(uint8_t _Red, uint8_t _Green, uint8_t _Blue)
        : m_Red(_Red)
        , m_Green(_Green)
        , m_Blue(_Blue)
    {
    }

    uint8_t m_Red;
    uint8_t m_Green;
    uint8_t m_Blue;
};