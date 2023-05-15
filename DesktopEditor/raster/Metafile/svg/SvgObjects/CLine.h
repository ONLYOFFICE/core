#ifndef CLINE_H
#define CLINE_H

#include "CPath.h"

namespace SVG
{
	class CLine : public CPath
	{
	public:
		CLine(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;
	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;
	};
}

#endif // CLINE_H
