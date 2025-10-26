#ifndef CELLIPSE_H
#define CELLIPSE_H

#include "CObjectBase.h"

namespace SVG
{
	class CEllipse : public CRenderedObject
	{
		friend class CObject;
		CEllipse(CSvgReader& oReader, CRenderedObject* pParent = NULL);
	public:
		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;

		bool Draw(IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CSvgFile *pFile, int& nTypePath, const CRenderedObject* pContexObject = NULL) const override;

		TBounds GetBounds(SvgMatrix* pTransform = nullptr) const override;

		SvgDigit m_oCx;
		SvgDigit m_oCy;
		SvgDigit m_oRx;
		SvgDigit m_oRy;
	};
}

#endif // CELLIPSE_H
