#ifndef CCIRCLE_H
#define CCIRCLE_H

#include "CObjectBase.h"

namespace SVG
{
	class CCircle : public CObjectBase
	{
	public:
		CCircle(CObjectBase* pParent = NULL);
		virtual ~CCircle();

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode, const CGeneralStyle& oBaseStyle) override;
		bool Draw(IRenderer* pRenderer) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, int& nTypePath) const override;

		double m_dCx;
		double m_dCy;
		double m_dR;
	};
}

#endif // CCIRCLE_H
