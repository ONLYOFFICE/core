#ifndef CPOLYLINE_H
#define CPOLYLINE_H

#include "CObjectBase.h"

namespace SVG
{
	class CPolyline : public CSvgGraphicsObject
	{
	public:
		CPolyline(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip = false, const TSvgStyles* pOtherStyles = NULL) const override;

	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		TBounds GetBounds() const override;

		void DrawLines(IRenderer* pRenderer) const;
		virtual void EndDraw(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip) const;

		std::vector<double> m_arValues;
	};

	class CPolygon : public CPolyline
	{
	public:
		CPolygon(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);
		virtual ~CPolygon();

		void EndDraw(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip) const override;
	};
}

#endif // CPOLYLINE_H
