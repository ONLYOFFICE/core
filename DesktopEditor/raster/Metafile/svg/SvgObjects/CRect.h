#ifndef CRECT_H
#define CRECT_H

#include "CObjectBase.h"

namespace SVG
{
	class CRect : public CObjectBase
	{
	public:
		CRect(CObjectBase* pParent = NULL, CGeneralStyle* pBaseStyle = NULL);
		virtual ~CRect();

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer) override;
	private:
		void ApplyStyle(IRenderer* pRenderer) override;

		double m_dX;
		double m_dY;
		double m_dWidth;
		double m_dHeight;
		double m_dRx;
		double m_dRy;
	};
}

#endif // CRECT_H
