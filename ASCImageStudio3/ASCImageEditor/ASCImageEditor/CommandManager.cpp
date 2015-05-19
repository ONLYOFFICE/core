// CommandManager.cpp : Implementation of CCommandManager

#include "stdafx.h"
#include "CommandManager.h"

STDMETHODIMP CCommandDescriptor::GetParamsCount(LONG* nCount)
{
	if (m_commandPtr)
		*nCount = m_commandPtr->GetParamsCount();
	else 
		*nCount = 0;
	return S_OK;
}

STDMETHODIMP CCommandDescriptor::GetParam(LONG nParam, IUnknown** Param)
{
	if (m_commandPtr)
		*Param = m_commandPtr->GetParam(nParam);
	else 
		*Param = NULL;
	return S_OK;
}

STDMETHODIMP CCommandDescriptor::StartChangingParamsGroup()
{
	if (m_commandPtr)
		m_commandPtr->StartChangingParamsGroup();
	return S_OK;
}
STDMETHODIMP CCommandDescriptor::FinishChangingParamsGroup()
{
	if (m_commandPtr)
		m_commandPtr->FinishChangingParamsGroup();
	return S_OK;
}