#ifndef CELLIPSE_H
#define CELLIPSE_H


#include "CObjectBase.h"

namespace SVG
{
	BEGIN_RENDERER_CHILDREN_H(Ellipse)
		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CSvgFile *pFile, int& nTypePath, const CRenderedObject* pContexObject = NULL) const override;

		TBounds GetBounds() const override;

		SvgDigit m_oCx;
		SvgDigit m_oCy;
		SvgDigit m_oRx;
		SvgDigit m_oRy;
	END_RENDERER_CHILDREN_H
}

#endif // CELLIPSE_H
