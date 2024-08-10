#include "image-sequence-system.h"

#include "singleton.h"
#include "image-loader.h"

#include "config-system.h"
#include "lottie-system.h"

#include <vector>

namespace
{
    class CImageSequenceSystem : public CSingleton<CImageSequenceSystem>
    {
    private:
        CImageSequenceSystem();
        ~CImageSequenceSystem() override = default;

    public:
        void Initialize();
        void Finalize();

    public:
        CImageSequence* FindImageSequence(std::string _SequenceName);

    private:
        void LoadImageSequences();

    private:
        std::map<std::string, CImageSequence*> m_ImageSequences;

    private:
        friend class CSingleton<CImageSequenceSystem>;
    };
}

namespace
{
    CImageSequenceSystem::CImageSequenceSystem()
    {
    }

    void CImageSequenceSystem::Initialize()
    {
        printf("Initialize ImageSequenceSystem\n");

        LoadImageSequences();
    }

    void CImageSequenceSystem::Finalize()
    {
        printf("Finalize ImageSequenceSystem\n");

        printf("Deleting %ld image sequences\n", m_ImageSequences.size());

        for(const auto& [sequenceName, pImageSequence] : m_ImageSequences)
        {
            delete pImageSequence;
        }
    }
    
    CImageSequence* CImageSequenceSystem::FindImageSequence(std::string _SequenceName)
    {
        for(const auto& [sequenceName, pImageSequence] : m_ImageSequences)
        {
            if(sequenceName.compare(_SequenceName) == 0)
            {
                return pImageSequence;
            }
        }

        return NULL;
    }
    
    void CImageSequenceSystem::LoadImageSequences()
    {
        Json::Value& rRoot = ConfigSystem::GetConfig();
        Json::Value sequences = rRoot["sequences"];

        for(auto const& sequenceName : sequences.getMemberNames())
        {
            Json::Value sequenceValue = sequences[sequenceName];

            const char* pSequenceName = sequenceName.c_str();

            printf("Loading sequence %s\n", pSequenceName);

            std::string type = sequenceValue.get("type", "images").asString();

            CImageSequence* pImageSequence = NULL;
            
            if(type.compare(std::string("images")) == 0)
            {
                int speed = sequenceValue.get("speed", 0).asInt();
                int waitAfter = sequenceValue.get("waitAfter", speed).asInt();

                Json::Value imagesValue = sequenceValue["images"];

                const int imageCount = imagesValue.size();

                std::vector<CImage*> images;

                for(int i = 0; i < imageCount; ++i)
                {
                    std::string filePath = imagesValue[i].asString();
                    
                    images.push_back(ImageLoader::LoadImageFromPPMFile(filePath));
                }

                pImageSequence = new CImageSequence(images, imageCount, speed, waitAfter);
            } else
            {
                std::string animation = sequenceValue.get("animation", "").asString();
                int speed = sequenceValue.get("speed", 20).asInt();
                int waitAfter = sequenceValue.get("waitAfter", speed).asInt();

                pImageSequence = LottieSystem::CreateSequenceFromAnimation(animation, speed, waitAfter);
            }
            
            m_ImageSequences[sequenceName.c_str()] = pImageSequence;
        }
    }
}

namespace ImageSequenceSystem
{
    void Initialize()
    {
        CImageSequenceSystem::GetInstance().Initialize();
    }

    void Finalize()
    {
        CImageSequenceSystem::GetInstance().Finalize();
    }

    CImageSequence* FindImageSequence(std::string _SequenceName)
    {
        return CImageSequenceSystem::GetInstance().FindImageSequence(_SequenceName);
    }
}