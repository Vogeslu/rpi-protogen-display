#include "redis-system.h"

#include "singleton.h"

#include "command-system.h"

#include <stdio.h>
#include <vector>
#include <chrono>
#include <cstdint>
#include <hiredis/hiredis.h>

#define REDIS_COMMAND_KEY "protogen-command"
#define REDIS_LOOKUP_TIMEOUT 50

namespace
{
    class CRedisSystem : public CSingleton<CRedisSystem>
    {
    private:
        CRedisSystem();
        ~CRedisSystem() override = default;

    public:
        void Initialize();
        void Finalize();

    public:
        void Run();

    private:
        uint64_t GetCurrentMilliseconds();

    private:
        redisContext* m_pRedisContext;
        redisReply* m_pRedisReply;

        char* m_LastCommandReply;

        uint64_t m_LastRedisLookup;

    private:
        friend class CSingleton<CRedisSystem>;
    };
}

namespace
{
    CRedisSystem::CRedisSystem()
        : m_pRedisContext(NULL)
        , m_pRedisReply(NULL)
        , m_LastCommandReply(NULL)
    {
    }

    void CRedisSystem::Initialize()
    {
        printf("Initialize RedisSystem\n");

        struct timeval timeout = { 1, 500000 };

        m_pRedisContext = redisConnectWithTimeout("127.0.0.1", 6379, timeout);

        if (m_pRedisContext == NULL || m_pRedisContext->err) {
            if (m_pRedisContext) {
                printf("Redis connection error: %s\n", m_pRedisContext->errstr);
            } else {
                printf("Redis Connection error: can't allocate redis context\n");
            }
        } else {
            printf("Connected to redis server\n");
        }
    }

    void CRedisSystem::Finalize()
    {
        printf("Finalize RedisSystem\n");

        redisFree(m_pRedisContext);
    }

    void CRedisSystem::Run()
    {
        uint64_t milliseconds = GetCurrentMilliseconds();

        if ((m_LastRedisLookup + REDIS_LOOKUP_TIMEOUT) > milliseconds)
        {
            return;
        }

        m_LastRedisLookup = milliseconds;

        if (m_pRedisContext == NULL) 
        {
            struct timeval timeout = { 1, 500000 };

            m_pRedisContext = redisConnectWithTimeout("127.0.0.1", 6379, timeout);

            if (m_pRedisContext == NULL || m_pRedisContext->err) {
                if (m_pRedisContext) {
                    printf("Redis connection error: %s\n", m_pRedisContext->errstr);
                } else {
                    printf("Redis Connection error: can't allocate redis context\n");
                }

                return;
            }
            else {
                printf("Connected to redis server\n");
            }
        }

        m_pRedisReply = static_cast<redisReply*>(redisCommand(m_pRedisContext,"GET " REDIS_COMMAND_KEY));

        if(m_pRedisReply != NULL && m_pRedisReply->str != NULL && (m_LastCommandReply == NULL || strcmp(m_pRedisReply->str, m_LastCommandReply) != 0))
        {
            m_LastCommandReply = m_pRedisReply->str;

            std::string redisCommand(m_LastCommandReply);

            CommandSystem::SetCurrentCommand(redisCommand);
        }

        freeReplyObject(m_pRedisReply);
    }

    uint64_t CRedisSystem::GetCurrentMilliseconds()
    {
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }
}

namespace RedisSystem
{
    void Initialize()
    {
        CRedisSystem::GetInstance().Initialize();
    }

    void Finalize()
    {
        CRedisSystem::GetInstance().Finalize();
    }

    void Run()
    {
        CRedisSystem::GetInstance().Run();
    }
}