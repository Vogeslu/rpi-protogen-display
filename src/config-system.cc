#include "config-system.h"

#include "singleton.h"

#include <fstream>

#define CONFIG_FILE "config.json"

namespace
{
    class CConfigSystem : public CSingleton<CConfigSystem>
    {
    private:
        CConfigSystem();
        ~CConfigSystem() override = default;

    public:
        void Initialize();
        void Finalize();

        Json::Value& GetConfig();

        void GetMatrixSettings(int& _rWidth, int& _rHeight, int& _rChainLength);

    private:
        void LoadConfig();

    private:
        Json::Value m_Root;

        int m_Width;
        int m_Height;
        int m_ChainLength;

    private:
        friend class CSingleton<CConfigSystem>;
    };
}

namespace
{
    CConfigSystem::CConfigSystem()
        : m_Width(64)
        , m_Height(32)
        , m_ChainLength(1)
    {
    }

    void CConfigSystem::Initialize()
    {
        printf("Initialize ConfigSystem\n");

        LoadConfig();
    }

    void CConfigSystem::Finalize()
    {
        printf("Finalize ConfigSystem\n");
    }
    
    void CConfigSystem::LoadConfig()
    {
        printf("Loading config\n");

        std::ifstream ifs(CONFIG_FILE);

        Json::Reader reader;

        reader.parse(ifs, m_Root);

        m_Width = m_Root.get("width", 64).asInt();
        m_Height = m_Root.get("height", 32).asInt();
        m_ChainLength = m_Root.get("chainLength", 2).asInt();
    }
    
    Json::Value& CConfigSystem::GetConfig()
    {
        return m_Root;
    }

    void CConfigSystem::GetMatrixSettings(int& _rWidth, int& _rHeight, int& _rChainLength)
    {
        _rWidth = m_Width;
        _rHeight = m_Height;
        _rChainLength = m_ChainLength;
    }
}

namespace ConfigSystem
{
    void Initialize()
    {
        CConfigSystem::GetInstance().Initialize();
    }

    void Finalize()
    {
        CConfigSystem::GetInstance().Finalize();
    }
    
    Json::Value& GetConfig()
    {
        return CConfigSystem::GetInstance().GetConfig();
    }

    void GetMatrixSettings(int& _rWidth, int& _rHeight, int& _rChainLength)
    {
        CConfigSystem::GetInstance().GetMatrixSettings(_rWidth, _rHeight, _rChainLength);
    }
}