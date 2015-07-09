#ifndef _XPS_XPSLIB_CONTEXTSTATE_H
#define _XPS_XPSLIB_CONTEXTSTATE_H

#include "Utils.h"

#include "../../DesktopEditor/graphics/Matrix.h"
#include "../../DesktopEditor/graphics/IRenderer.h"

#include <list>
#include <map>

namespace XPS
{
	class CContextState
	{
	public:

		CContextState(IRenderer* pRenderer);
		~CContextState();

		void   PushClip(const CWString& wsClip);
		void   PopClip();
		void   PushTransform(const double arrTransform[6]);
		void   PopTransform();
		double NormalizeTransform();

	private:

		void SetClipToRenderer(const CWString& wsClip);
		void SetTransformToRenderer();

	private:

		Aggplus::CMatrix            m_oCurrentTransform;
		std::list<Aggplus::CMatrix> m_lTransformStack;
		std::vector<CWString>       m_vClipStack;
		IRenderer*                  m_pRenderer;
	};
}

#endif //_XPS_XPSLIB_CONTEXTSTATE_H