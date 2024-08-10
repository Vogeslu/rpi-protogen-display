#include "image-loader.h"

#include "singleton.h"
#include "pixel.h"
#include "image-sequence.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <map>

namespace
{
    class CImageLoader : public CSingleton<CImageLoader>
    {
    private:
        CImageLoader();
        ~CImageLoader() override = default;

    public:
        void Initialize();
        void Finalize();
        
        CImage* LoadImageFromPPMFile(std::string _FileName);

    private:
        char* ReadLine(FILE* _pFile, char* _pBuffer, size_t _Len);

        CImage* FindExistingImage(std::string _FileName);

    private:
        std::map<std::string, CImage*> m_Images;

    private:
        friend class CSingleton<CImageLoader>;
    };
}

namespace
{
    CImageLoader::CImageLoader()
    {
    }

    void CImageLoader::Initialize()
    {
        printf("Initialize ImageLoader\n");
    }

    void CImageLoader::Finalize()
    {
        printf("Finalize ImageLoader\n");

        printf("Deleting %ld images\n", m_Images.size());

        for(const auto& [fileName, pImage] : m_Images)
        {
            delete pImage;
        }
    }
    
    CImage* CImageLoader::LoadImageFromPPMFile(std::string _FileName)
    {
        printf("Loading file \"%s\" for image\n", _FileName.c_str());

        CImage* existingImage = FindExistingImage(_FileName);

        if(existingImage != NULL)
        {
            printf("Reusing already loaded image\n");

            return existingImage;
        }

        FILE* pFile = fopen(_FileName.c_str(), "r");

        if(pFile == NULL && access(_FileName.c_str(), F_OK) == -1)
        {
            fprintf(stderr, "File \"%s\" doesn't exist\n", _FileName.c_str());
            return NULL;
        }

        if(pFile == NULL)
        {
            return NULL;
        }

        char headerBuf[256];
        const char* pLine = ReadLine(pFile, headerBuf, sizeof(headerBuf));

    #define EXIT_WITH_MSG(message) { \
        fprintf(stderr, "%s: %s | %s", _FileName.c_str(), message, pLine); \
        fclose(pFile); \
        return NULL; \
    }

        if(sscanf(pLine, "P6 ") == EOF)
        {
            EXIT_WITH_MSG("Can only handle P6 as PPM type.");
        }

        pLine = ReadLine(pFile, headerBuf, sizeof(headerBuf));

        int newWidth, newHeight;

        if(!pLine || sscanf(pLine, "%d %d ", &newWidth, &newHeight) != 2)
        {
            EXIT_WITH_MSG("Width/Height expected.");
        }

        int value;

        pLine = ReadLine(pFile, headerBuf, sizeof(headerBuf));

        if(!pLine || sscanf(pLine, "%d ", &value) != 1 || value != 255)
        {
            EXIT_WITH_MSG("Only 255 for maxval allowed.");
        }

        const size_t pixelCount = newWidth * newHeight;

        FPixel* pPixels = new FPixel[pixelCount];
        assert(sizeof(FPixel) == 3);

        if(fread(pPixels, sizeof(FPixel), pixelCount, pFile) != pixelCount)
        {
            pLine = "";
            EXIT_WITH_MSG("Not enough pixels read.");
        }

    #undef EXIT_WITH_MSG

        CImage* pImage = new CImage(newWidth, newHeight, pPixels);

        printf("Loaded file \"%s\" as image (width: %d, height: %d)\n", (char*) _FileName.c_str(), newWidth, newHeight);

        m_Images[_FileName.c_str()] = pImage;

        return pImage;
    }

    
    char* CImageLoader::ReadLine(FILE* _pFile, char* _pBuffer, size_t _Len)
    {
        char* pResult;

        do
        {
            pResult = fgets(_pBuffer, _Len, _pFile);
        } while (pResult != NULL && pResult[0] == '#');

        return pResult;
    }

    CImage* CImageLoader::FindExistingImage(std::string _FileName)
    {
        for(const auto& [fileName, pImage] : m_Images)
        {
            if(fileName.compare(_FileName) == 0)
            {
                return pImage;
            }
        }

        return NULL;
    }
}

namespace ImageLoader
{
    void Initialize()
    {
        CImageLoader::GetInstance().Initialize();
    }

    void Finalize()
    {
        CImageLoader::GetInstance().Finalize();
    }
    
    CImage* LoadImageFromPPMFile(std::string _FileName)
    {
        return CImageLoader::GetInstance().LoadImageFromPPMFile(_FileName);
    }
}