#include "GraphicsLayer.h"

namespace Aggplus
{
	CGraphicsLayer::CGraphicsLayer(BYTE *pBuffer, bool bExternalBuffer)
		: m_pBuffer(pBuffer), m_bExternalBuffer(NULL != pBuffer && bExternalBuffer)
	{
		SetDefaultSettings();
	}

	CGraphicsLayer::~CGraphicsLayer()
	{
		if (!m_bExternalBuffer)
			RELEASEARRAYOBJECTS(m_pBuffer);
	}

	bool CGraphicsLayer::Empty() const
	{
		return NULL == m_pBuffer;
	}
	
	BYTE *CGraphicsLayer::GetBuffer()
	{
		return m_pBuffer;
	}

	void CGraphicsLayer::SetDefaultSettings()
	{
		m_oSettings.m_dOpacity = 1.;
	}

	void CGraphicsLayer::SetSettings(const TGraphicsLayerSettings &oSettings)
	{
		m_oSettings = oSettings;
	}

	const TGraphicsLayerSettings &CGraphicsLayer::GetSettings() const
	{
		return m_oSettings;
	}

	void CGraphicsLayer::SetOpacity(double dOpacity)
	{
		if (dOpacity > 1. || dOpacity < 0.)
			m_oSettings.m_dOpacity = 1.;
		else
			m_oSettings.m_dOpacity = dOpacity;
	}
}

