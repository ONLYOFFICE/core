#ifndef CHEADER_H
#define CHEADER_H

#include "CObjectBase.h"

namespace SVG
{
	class CHeader : public CObjectBase
	{
	public:
		CHeader(CObjectBase* pParent = NULL);
		virtual ~CHeader();

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode, const CGeneralStyle& oBaseStyle) override;
		bool Draw(IRenderer* pRenderer) const override;

		void GetBounds(double &dX, double &dY, double &dWidth, double &dHeight);
	private:
		void ApplyStyle(IRenderer* pRenderer, int& nTypePath) const override;
	};
}
#endif // CHEADER_H
