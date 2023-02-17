#ifndef CGRADIENT_H
#define CGRADIENT_H

#include "CDefs.h"
#include "CContainer.h"

namespace SVG
{
	typedef enum
	{
		ObjectBoundingBox,
		UserSpaceOnUse
	}GradientUnits;

	typedef enum
	{
		Pad,
		Reflect,
		Repeat
	}SpreadMethod;

	class CStopElement : public CObjectBase
	{
	public:
		CStopElement(CObjectBase* pParent = NULL);

		SvgDigit GetOffset() const;
		SvgColor GetColor()  const;

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer, CDefs *pDefs) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		TBounds GetBounds() const override;

		SvgDigit m_oOffset;
		SvgColor m_oColor;
	};

	class CGradient : public CContainer
	{
	public:
		CGradient(CObjectBase* pParent = NULL);

		bool Apply(IRenderer* pRenderer) const;
	private:

		std::wstring              m_wsXlinkHref;
		GradientUnits             m_enGradientUnits;
		SpreadMethod              m_enSpreadMethod;
	};

	class CLinearGradient : public CGradient, public IDefObject
	{
	public:
		CLinearGradient(CObjectBase* pParent = NULL);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Apply(IRenderer* pRenderer, CDefs* pDefs, const TBounds &oObjectBounds) override;
	private:
		SvgDigit m_oX1;
		SvgDigit m_oY1;
		SvgDigit m_oX2;
		SvgDigit m_oY2;
	};

	class CRadialGradient : public CGradient, public IDefObject
	{
	public:
		CRadialGradient(CObjectBase* pParent = NULL);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Apply(IRenderer* pRenderer, CDefs* pDefs, const TBounds &oObjectBounds) override;
	private:
		SvgDigit m_oCx;
		SvgDigit m_oCy;
		SvgDigit m_oR;
	};
}

#endif // CGRADIENT_H
