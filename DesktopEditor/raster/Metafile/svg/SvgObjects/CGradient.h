#ifndef CGRADIENT_H
#define CGRADIENT_H

#include "CContainer.h"
#include "CObjectBase.h"

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

	class CStopElement : public CObject
	{
	public:
		CStopElement(XmlUtils::CXmlNode& oNode);

		ObjectType GetType() const override;

		SvgDigit GetOffset() const;
		SvgColor GetColor()  const;

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;
	private:
		SvgDigit m_oOffset;
		SvgColor m_oColor;
	};

	class CGradient : public CContainer<CStopElement>
	{
	public:
		CGradient();

		bool Apply(IRenderer* pRenderer) const;
	private:

		std::wstring              m_wsXlinkHref;
		GradientUnits             m_enGradientUnits;
		SpreadMethod              m_enSpreadMethod;
	};

	class CLinearGradient : public CGradient, public CAppliedObject
	{
	public:
		CLinearGradient(XmlUtils::CXmlNode& oNode);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Apply(IRenderer* pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds) override;
	private:
		SvgDigit m_oX1;
		SvgDigit m_oY1;
		SvgDigit m_oX2;
		SvgDigit m_oY2;
	};

	class CRadialGradient : public CGradient, public CAppliedObject
	{
	public:
		CRadialGradient(XmlUtils::CXmlNode& oNode);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Apply(IRenderer* pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds) override;
	private:
		SvgDigit m_oCx;
		SvgDigit m_oCy;
		SvgDigit m_oR;
	};
}

#endif // CGRADIENT_H
