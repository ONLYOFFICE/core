#ifndef CCIRCLE_H
#define CCIRCLE_H

#include "CObjectBase.h"

namespace SVG
{
	class CCircle : public CRenderedObject
	{
		friend class CObject;
		CCircle(CSvgReader& oReader, CRenderedObject* pParent = NULL);
	public:
		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CSvgFile *pFile, int& nTypePath, const CRenderedObject* pContexObject = NULL) const override;

		TBounds GetBounds() const override;

		SvgDigit m_oCx;
		SvgDigit m_oCy;
		SvgDigit m_oR;
	};
}

#endif // CCIRCLE_H
