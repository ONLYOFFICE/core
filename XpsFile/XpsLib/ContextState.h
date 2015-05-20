#ifndef _XPS_XPSLIB_CONTEXTSTATE_H
#define _XPS_XPSLIB_CONTEXTSTATE_H

#include "../../DesktopEditor/graphics/Matrix.h"
#include "../../DesktopEditor/graphics/IRenderer.h"

#include <list>
#include <map>

namespace XPS
{
	class CContextState
	{
	public:

		CContextState();
		~CContextState();

		void AddFigure(const std::wstring& wsKey, const std::wstring& wsName);
		std::wstring GetFigure(const std::wstring& wsKey);
		void PushTransform(const double arrTransform[6]);
		void PopTransform();
		double NormalizeTransform();
		void SetTransformToRenderer(IRenderer* pRenderer);

	public:

		Aggplus::CMatrix                     m_oCurrentTransform;
		std::list<Aggplus::CMatrix>          m_lTransformStack;
		std::map<std::wstring, std::wstring> m_mFigures;
	};
}

#endif //_XPS_XPSLIB_CONTEXTSTATE_H