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

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		SvgDigit m_oCx;
		SvgDigit m_oCy;
		SvgDigit m_oR;
	};
}

#endif // CCIRCLE_H
