#include "render-system.h"

#include "singleton.h"

#include "led-matrix.h"

#include "command-system.h"
#include "config-system.h"

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;
using rgb_matrix::FrameCanvas;

namespace
{
    class CRenderSystem : public CSingleton<CRenderSystem>
    {
    private:
        CRenderSystem();
        ~CRenderSystem() override = default;

    public:
        void Initialize();
        void Finalize();

    public:
        void Run();

    private:
        RGBMatrix* m_pRgbMatrix;
        FrameCanvas* m_pOffscreenCanvas;

        CImage* m_pPreviousImage;

    private:
        friend class CSingleton<CRenderSystem>;
    };
}

namespace
{
    CRenderSystem::CRenderSystem()
    {
    }

    void CRenderSystem::Initialize()
    {
        printf("Initialize RenderSystem\n");

        RGBMatrix::Options options;
        rgb_matrix::RuntimeOptions runtime_opt;
        
        int Width, Height, ChainLength;

        ConfigSystem::GetMatrixSettings(Width, Height, ChainLength);

        options.hardware_mapping = "regular";
        options.rows = Height;
        options.cols = Width;
        options.chain_length = ChainLength;
        options.parallel = 1;
        
        m_pRgbMatrix = RGBMatrix::CreateFromOptions(options, runtime_opt);

        if(m_pRgbMatrix == NULL)
        {
            fprintf(stderr, "Failed initializing rgb matrix / canvas");
            return;
        }

        m_pOffscreenCanvas = m_pRgbMatrix->CreateFrameCanvas();
    }

    void CRenderSystem::Finalize()
    {
        printf("Finalize RenderSystem\n");

        delete m_pRgbMatrix;
    }

    void CRenderSystem::Run()
    {
        if(m_pRgbMatrix == NULL)
        {
            return;
        }
        
        CCommand* pCurrentCommand = CommandSystem::GetCurrentCommand();

        if(pCurrentCommand == NULL)
        {
            printf("Current command / initial command is null\n");
            return;
        }

        CImageSequence* pImageSequence = (CImageSequence*) pCurrentCommand->GetImageSequence();

        CImage* pImage = (CImage*) pImageSequence->GetImage();

        if(pImage == m_pPreviousImage)
        {
            bool endReached = pImageSequence->CheckImageSwitch();

            if(endReached && pCurrentCommand->IsInTransition())
            {
                pCurrentCommand->ClearCurrentTransition();
            }
            
            return;
        }
        
        m_pPreviousImage = pImage;

        const int screenHeight = m_pRgbMatrix->height();
        const int screenWidth = m_pRgbMatrix->width();

        for(int x = 0; x < screenWidth; ++x)
        {
            for(int y = 0; y < screenHeight; ++y)
            {
                const FPixel& rPixel = pImage->GetPixel(x, y);

                m_pOffscreenCanvas->SetPixel(x, y, rPixel.m_Red, rPixel.m_Green, rPixel.m_Blue);
            }   
        }

        m_pOffscreenCanvas = m_pRgbMatrix->SwapOnVSync(m_pOffscreenCanvas);
        
        bool endReached = pImageSequence->CheckImageSwitch();

        if(endReached && pCurrentCommand->IsInTransition())
        {
            pCurrentCommand->ClearCurrentTransition();
        }
    }
}

namespace RenderSystem
{
    void Initialize()
    {
        CRenderSystem::GetInstance().Initialize();
    }

    void Finalize()
    {
        CRenderSystem::GetInstance().Finalize();
    }

    void Run()
    {
        CRenderSystem::GetInstance().Run();
    }
}