#ifndef CPOLYLINE_H
#define CPOLYLINE_H

#include "CObjectBase.h"

namespace SVG
{
	class CPolyline : public CObjectBase
	{
	public:
		CPolyline(CObjectBase* pParent = NULL);
		virtual ~CPolyline();

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, int& nTypePath) const override;

		void BeginDraw(IRenderer* pRenderer, int& nTypePath) const;
		void DrawLines(IRenderer* pRenderer) const;
		virtual void EndDraw(IRenderer* pRenderer, int& nTypePath) const;

		std::vector<double> m_arValues;
	};

	class CPolygon : public CPolyline
	{
	public:
		CPolygon(CObjectBase* pParent = NULL);
		virtual ~CPolygon();

		void EndDraw(IRenderer* pRenderer, int& nTypePath) const override;
	};
}

#endif // CPOLYLINE_H
