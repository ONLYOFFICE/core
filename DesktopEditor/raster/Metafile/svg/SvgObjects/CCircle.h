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

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer, const CGeneralStyle* pBaseStyle) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, int& nTypePath, const CGeneralStyle* pBaseStyle) const override;

		double m_dCx;
		double m_dCy;
		double m_dR;
	};
}

#endif // CCIRCLE_H
