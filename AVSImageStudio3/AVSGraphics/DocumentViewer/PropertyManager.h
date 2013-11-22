#pragma once
#include "..\stdafx.h"

class CPropertyManager
{
public:
	BOOL*		m_pIsChanged;
	VARIANT*	m_pValues;		

public:
	CPropertyManager(LONG lCount)
	{
		m_pIsChanged	= new BOOL[lCount];
		m_pValues		= new VARIANT[lCount];

		for (LONG i = 0; i < lCount; ++i)
			m_pIsChanged[i] = FALSE;
	}
	~CPropertyManager()
	{
		RELEASEARRAYOBJECTS(m_pIsChanged);
		RELEASEARRAYOBJECTS(m_pValues);
	}

public:
	void SetProperty(const LONG& lIndex, LONG lValue)
	{
		m_pIsChanged[lIndex]	= TRUE;
		
		m_pValues[lIndex].vt	= VT_I4;
		m_pValues[lIndex].lVal	= lValue;
	}
	void SetProperty(const LONG& lIndex, double dVal)
	{
		m_pIsChanged[lIndex]		= TRUE;
		
		m_pValues[lIndex].vt		= VT_R8;
		m_pValues[lIndex].dblVal	= dVal;
	}
};