#ifndef CRECT_H
#define CRECT_H

#include "CObjectBase.h"

namespace SVG
{
	class CRect : public CRenderedObject
	{
		friend class CObject;
		CRect(CSvgReader& oReader, CRenderedObject* pParent = NULL);
	public:
		virtual ~CRect();

		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;

	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CSvgFile* pFile, int& nTypePath, const CRenderedObject* pContexObject = NULL) const override;

		TBounds GetBounds() const override;

		TRect    m_oRect;

		SvgDigit m_oRx;
		SvgDigit m_oRy;
	};
}

#endif // CRECT_H
