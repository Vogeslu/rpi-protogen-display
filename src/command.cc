#include "command.h"

CCommand::CCommand(CImageSequence* _pImageSequence, std::string _CommandName)
    : m_pImageSequence(_pImageSequence)
    , m_CommandName(_CommandName)
{
}

CCommand::~CCommand()
{
}


const CImageSequence* CCommand::GetImageSequence() const
{
    return m_pImageSequence;
}

std::string CCommand::GetCommandName()
{
    return m_CommandName;
}