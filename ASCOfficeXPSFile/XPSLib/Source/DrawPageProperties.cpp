#include "stdafx.h"
#include "DrawPageProperties.h"

namespace XPS
{
	DrawPageProperties::DrawPageProperties()
		:	m_CurrentTransform(1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
	{
		//HRESULT res;
		/*res = */CoCreateInstance( __uuidof(AVSGraphics::CAVSFontManager), NULL, CLSCTX_INPROC_SERVER, __uuidof(AVSGraphics::IAVSFontManager), (void **)&(m_FontManager));
		//assert(res == S_OK);

		_bstr_t options(L"");
		m_FontManager->Initialize(options);

		m_TransformStack.push_back(m_CurrentTransform);
	}


	DrawPageProperties::~DrawPageProperties()
	{
		if ( m_FontManager != NULL )
		{
			m_FontManager->Release();
			m_FontManager = NULL;
		}
	}
} // namespace XPS