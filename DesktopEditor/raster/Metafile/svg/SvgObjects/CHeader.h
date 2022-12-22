#ifndef CHEADER_H
#define CHEADER_H

#include "CObjectBase.h"

namespace SVG
{
	class CHeader : public CObjectBase
	{
	public:
		CHeader(CObjectBase* pParent = NULL, CGeneralStyle* pBaseStyle = NULL);
		virtual ~CHeader();

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer) override;

		void GetBounds(double &dX, double &dY, double &dWidth, double &dHeight);
	private:
		void ApplyStyle(IRenderer* pRenderer) override;

		double m_dX;
		double m_dY;
		double m_dWidth;
		double m_dHeight;
	};
}
#endif // CHEADER_H
