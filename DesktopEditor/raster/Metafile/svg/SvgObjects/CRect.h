#ifndef CRECT_H
#define CRECT_H

#include "CObjectBase.h"

namespace SVG
{
	class CRect : public CObjectBase
	{
	public:
		CRect(CObjectBase* pParent = NULL);
		virtual ~CRect();

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		SvgDigit m_oX;
		SvgDigit m_oY;
		SvgDigit m_oWidth;
		SvgDigit m_oHeight;

		SvgDigit m_oRx;
		SvgDigit m_oRy;
	};
}

#endif // CRECT_H
