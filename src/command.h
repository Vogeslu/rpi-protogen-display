#pragma once

#include "image-sequence.h"

class CCommand
{
public:
    CCommand(CImageSequence* _pImageSequence, std::string _CommandName);
    ~CCommand();

public:
    const CImageSequence* GetImageSequence() const;
    std::string GetCommandName();

    void ClearCurrentTransition();
    void SetCurrentTransition(CImageSequence* _pTransitionImageSequence);

    bool IsInTransition();

private:
    CImageSequence* m_pImageSequence;
    std::string m_CommandName;

    CImageSequence* m_pTransitionImageSequence;
};