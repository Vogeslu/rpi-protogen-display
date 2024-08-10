#pragma once

template<typename T>
class CSingleton
{
public:
    static T& GetInstance()
    {
        static T instance;
        return instance;
    }

protected:
    CSingleton() = default;
    virtual ~CSingleton() = default;

private:
    CSingleton(const CSingleton&) = delete;
    CSingleton& operator = (const CSingleton&) = delete;
};
