#include "lottie-system.h"

#include "singleton.h"

#include "config-system.h"

#include <vector>
#include <algorithm>

namespace
{
    class CLottieSystem : public CSingleton<CLottieSystem>
    {
    private:
        CLottieSystem();
        ~CLottieSystem() override = default;

    public:
        void Initialize();
        void Finalize();

    public:
        CImageSequence* CreateSequenceFromAnimation(std::string _AnimationFileName, int _Speed, int _WaitAfter, bool _Reverse);

    private:
        std::vector<CImage*> m_Images;

    private:
        friend class CSingleton<CLottieSystem>;
    };
}

namespace
{
    CLottieSystem::CLottieSystem()
    {
    }

    void CLottieSystem::Initialize()
    {
        printf("Initialize LottieSystem\n");
    }

    void CLottieSystem::Finalize()
    {
        printf("Finalize LottieSystem\n");

        printf("Deleting %ld images\n", m_Images.size());

        for(const auto& pImage : m_Images)
        {
            delete pImage;
        }
    }

    
    CImageSequence* CLottieSystem::CreateSequenceFromAnimation(std::string _AnimationFileName, int _Speed, int _WaitAfter, bool _Reverse)
    {
        const char* pAnimationFileName = _AnimationFileName.c_str();

        printf("Loading animation %s\n",pAnimationFileName);

        auto animation = rlottie::Animation::loadFromFile(pAnimationFileName);

        int Width, Height, ChainLength;

        ConfigSystem::GetMatrixSettings(Width, Height, ChainLength);

        Width *= ChainLength;

        uint32_t* pBuffer = static_cast<uint32_t*>(calloc(Width * 4 * Height, sizeof(uint32_t)));

        rlottie::Surface surface(pBuffer, Width, Height, Width * 4);
    
        size_t totalFrames = animation->totalFrame();

        std::vector<CImage*> images;

        const size_t pixelCount = Width * Height;

        for(size_t i = 0; i < totalFrames; ++i)
        {
            animation->renderSync(i, surface);
            
            FPixel* pPixels = new FPixel[pixelCount];

            for(int y = 0; y < Height; ++y)
            {
                for(int x = 0; x < Width; ++x)
                {
                    uint32_t* value = pBuffer + y * Width + x;

                    int red = (*value >> 16) & 0xff;
                    int green = (*value >> 8) & 0xff;
                    int blue = *value & 0xff;

                    pPixels[y * Width + x] = FPixel(red, green, blue);
                }   
            }
        
            CImage* pImage = new CImage(Width, Height, pPixels);

            images.push_back(pImage);
            m_Images.push_back(pImage);
        }

        if(_Reverse)
        {
            std::reverse(images.begin(), images.end());
        }

        free(pBuffer);

        CImageSequence* pImageSequence = new CImageSequence(images, totalFrames, _Speed, _WaitAfter);

        return pImageSequence;
    }
}

namespace LottieSystem
{
    void Initialize()
    {
        CLottieSystem::GetInstance().Initialize();
    }

    void Finalize()
    {
        CLottieSystem::GetInstance().Finalize();
    }

    
    CImageSequence* CreateSequenceFromAnimation(std::string _AnimationFileName, int _Speed, int _WaitAfter, bool _Reverse)
    {
        return CLottieSystem::GetInstance().CreateSequenceFromAnimation(_AnimationFileName, _Speed, _WaitAfter, _Reverse);
    }
}