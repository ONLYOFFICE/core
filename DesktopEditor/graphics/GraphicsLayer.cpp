#include "GraphicsLayer.h"
#include <cmath>

namespace Aggplus
{
	CGraphicsLayer::CGraphicsLayer(BYTE *pBuffer, bool bExternalBuffer)
		: m_pBuffer(pBuffer), m_bExternalBuffer(NULL != pBuffer && bExternalBuffer)
	{
		SetDefaultSettings();
	}

	CGraphicsLayer::~CGraphicsLayer()
	{
		ClearBuffer();
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
		m_oSettings.m_uchOpacity = 255;
	}
	
	void CGraphicsLayer::ClearBuffer(bool bDeleteData)
	{
		if (bDeleteData && !m_bExternalBuffer)
			RELEASEARRAYOBJECTS(m_pBuffer);

		m_pBuffer = NULL;
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
			m_oSettings.m_uchOpacity = 255;
		else
		{
			m_oSettings.m_uchOpacity = ceil(255 * dOpacity);
		}
	}
	
	void CGraphicsLayer::SetOpacity(BYTE uchOpacity)
	{
		m_oSettings.m_uchOpacity = uchOpacity;
	}
}

