#pragma once

#include <jsoncpp/json/json.h>

namespace ConfigSystem
{
    void Initialize();
    void Finalize();
    
    Json::Value& GetConfig();

    void GetMatrixSettings(int& _rWidth, int& _rHeight, int& _rChainLength);
}