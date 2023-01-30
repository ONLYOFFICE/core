#ifndef CLINE_H
#define CLINE_H

#include "CObjectBase.h"

namespace SVG
{
	class CLine : public CObjectBase
	{
	public:
		CLine(CObjectBase* pParent = NULL);
		virtual ~CLine();

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode, const CGeneralStyle& oBaseStyle) override;
		bool Draw(IRenderer* pRenderer) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, int& nTypePath) const override;

		double m_dX1;
		double m_dY1;
		double m_dX2;
		double m_dY2;
	};
}

#endif // CLINE_H
