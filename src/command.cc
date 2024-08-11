#include "command.h"

CCommand::CCommand(CImageSequence* _pImageSequence, std::string _CommandName)
    : m_pImageSequence(_pImageSequence)
    , m_CommandName(_CommandName)
    , m_pTransitionImageSequence(NULL)
{
}

CCommand::~CCommand()
{
}


const CImageSequence* CCommand::GetImageSequence() const
{
    if(m_pTransitionImageSequence != NULL)
    {
        return m_pTransitionImageSequence;
    }

    return m_pImageSequence;
}

std::string CCommand::GetCommandName()
{
    return m_CommandName;
}

void CCommand::ClearCurrentTransition()
{
    m_pTransitionImageSequence = NULL;
}

void CCommand::SetCurrentTransition(CImageSequence* _pTransitionImageSequence)
{
    m_pTransitionImageSequence = _pTransitionImageSequence;
}

bool CCommand::IsInTransition()
{
    return m_pTransitionImageSequence != NULL;
}