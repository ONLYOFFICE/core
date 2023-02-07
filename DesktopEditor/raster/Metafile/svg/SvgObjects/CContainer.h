#ifndef CCONTAINER_H
#define CCONTAINER_H

#include "CObjectBase.h"

#include "../../../graphics/pro/Fonts.h"

class CSvgFile;

namespace SVG
{
	class CSvgParser;

	class CContainer : public CObjectBase
	{
	public:
		CContainer(CObjectBase* pParent = NULL);
		virtual ~CContainer();

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer) const override;

		void Clear();

		bool Empty() const;

		SvgDigit GetX() const;
		SvgDigit GetY() const;
		SvgDigit GetWidth() const;
		SvgDigit GetHeight() const;

		void AddObject(CObjectBase* pObject);
	private:
		void ApplyStyle(IRenderer* pRenderer, int& nTypePath) const override;

		std::vector<CObjectBase*> m_arObjects;

		SvgDigit m_oX;
		SvgDigit m_oY;
		SvgDigit m_oWidth;
		SvgDigit m_oHeight;
	};
}

#endif // CCONTAINER_H
