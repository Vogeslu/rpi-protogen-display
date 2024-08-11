#include "command-system.h"

#include "singleton.h"

#include "config-system.h"
#include "image-sequence-system.h"

namespace
{
    class CCommandSystem : public CSingleton<CCommandSystem>
    {
    private:
        CCommandSystem();
        ~CCommandSystem() override = default;

    public:
        void Initialize();
        void Finalize();

    public:
        void SetCurrentCommand(std::string _CommandName);
        CCommand* GetCurrentCommand();
        CCommand* FindCommand(std::string _CommandName);

    private:
        void LoadCommands();

    private:
        std::map<std::string, CCommand*> m_Commands;

        CCommand* m_pCurrentCommand;

    private:
        friend class CSingleton<CCommandSystem>;
    };
}

namespace
{
    CCommandSystem::CCommandSystem()
    {
    }

    void CCommandSystem::Initialize()
    {
        printf("Initialize CommandSystem\n");

        LoadCommands();
    }

    void CCommandSystem::Finalize()
    {
        printf("Finalize CommandSystem\n");

        printf("Deleting %ld commands\n", m_Commands.size());

        for(const auto& [commandName, pCommand] : m_Commands)
        {
            delete pCommand;
        }
    }
    
    void CCommandSystem::SetCurrentCommand(std::string _CommandName)
    {
        CCommand* pCommand = FindCommand(_CommandName);

        if(pCommand == NULL || pCommand == m_pCurrentCommand)
        {
            return;
        }

        printf("Updating current command to %s\n", _CommandName.c_str());

        m_pCurrentCommand->ClearCurrentTransition();
        pCommand->ClearCurrentTransition();
        
        CImageSequence* pCurrentImageSequence = (CImageSequence*) m_pCurrentCommand->GetImageSequence();
        CImageSequence* pTargetImageSequence = (CImageSequence*) pCommand->GetImageSequence();

        CImageSequence* pTransitionSequence = pCurrentImageSequence->FindTransitionToSequence(pTargetImageSequence);

        m_pCurrentCommand = pCommand;

        if(pTransitionSequence != NULL)
        {
            printf("Playing transition before %s\n", _CommandName.c_str());
            
            pTransitionSequence->Restart();

            m_pCurrentCommand->SetCurrentTransition(pTransitionSequence);
        }

        CImageSequence* pImageSequence = (CImageSequence*) m_pCurrentCommand->GetImageSequence();
        
        pImageSequence->Restart();
    }
    
    CCommand* CCommandSystem::GetCurrentCommand()
    {
        return m_pCurrentCommand;
    }
    
    CCommand* CCommandSystem::FindCommand(std::string _CommandName)
    {
        for(const auto& [commandName, pCommand] : m_Commands)
        {
            if(commandName.compare(_CommandName) == 0)
            {
                return pCommand;
            }
        }

        return NULL;
    }

    void CCommandSystem::LoadCommands()
    {
        Json::Value& rRoot = ConfigSystem::GetConfig();
        Json::Value commands = rRoot["commands"];

        for(auto const& commandName : commands.getMemberNames())
        {
            Json::Value commandValue = commands[commandName];

            const char* pCommandName = commandName.c_str();

            printf("Loading command %s\n", pCommandName);

            std::string sequenceName = commandValue["sequence"].asString();

            CImageSequence* pFoundSequence = ImageSequenceSystem::FindImageSequence(sequenceName);

            if(pFoundSequence == NULL)
            {
                printf("Requested image sequence %s does not exist\n", sequenceName.c_str());
                continue;
            } else {
                printf("Found image sequence %s\n", sequenceName.c_str());
            }

            CCommand* pCommand = new CCommand(pFoundSequence, commandName);
            
            m_Commands[commandName.c_str()] = pCommand;
        }

        std::string initialCommandName = rRoot["initialCommand"].asString();

        m_pCurrentCommand = FindCommand(initialCommandName);

        if(m_pCurrentCommand == NULL)
        {
            fprintf(stderr, "Initial command %s does not exist\n", initialCommandName.c_str());
        } else
        {
            printf("Initial command set to %s\n", initialCommandName.c_str());
        }
    }
}

namespace CommandSystem
{
    void Initialize()
    {
        CCommandSystem::GetInstance().Initialize();
    }

    void Finalize()
    {
        CCommandSystem::GetInstance().Finalize();
    }
    
    void SetCurrentCommand(std::string _CommandName)
    {
        CCommandSystem::GetInstance().SetCurrentCommand(_CommandName);
    }

    CCommand* GetCurrentCommand()
    {
        return CCommandSystem::GetInstance().GetCurrentCommand();
    }
    
    CCommand* FindCommand(std::string _CommandName)
    {
        return CCommandSystem::GetInstance().FindCommand(_CommandName);
    }
}