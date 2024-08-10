#pragma once

#include "command.cc"

namespace CommandSystem
{
    void Initialize();
    void Finalize();

    void SetCurrentCommand(std::string _CommandName);
    CCommand* GetCurrentCommand();

    CCommand* FindCommand(std::string _CommandName);
}