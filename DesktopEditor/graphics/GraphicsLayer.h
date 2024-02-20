#ifndef CGRAPHICSLAYER_H
#define CGRAPHICSLAYER_H

#include "Defines.h"
#include "./config.h"
#include "../common/IGrObject.h"

#include <iostream>

namespace Aggplus
{
	struct TGraphicsLayerSettings
	{
		BYTE m_uchOpacity;
	};

	class GRAPHICS_DECL CGraphicsLayer : public IGrObject
	{
	public:
		CGraphicsLayer(BYTE* pBuffer, bool bExternalBuffer = true);
		~CGraphicsLayer();

		bool Empty() const;
		BYTE* GetBuffer();

		void SetDefaultSettings();
		void ClearBuffer(bool bDeleteData = true);

		void SetSettings(const TGraphicsLayerSettings& oSettings);
		const TGraphicsLayerSettings& GetSettings() const;

		void SetOpacity(double dOpacity);
		void SetOpacity(BYTE uchOpacity);

	private:
		BYTE*                  m_pBuffer;
		bool                   m_bExternalBuffer;

		TGraphicsLayerSettings m_oSettings;
	};

}

#endif // CGRAPHICSLAYER_H
