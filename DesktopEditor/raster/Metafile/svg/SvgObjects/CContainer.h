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
		CContainer(double dWidth, double dHeight, CObjectBase* pParent = NULL);
		virtual ~CContainer();

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer, CDefs *pDefs) const override;

		void Clear();

		bool Empty() const;

		TRect GetWindow() const;
		TRect GetViewBox() const;

		void AddObject(CObjectBase* pObject);
	private:
		void ApplyStyle(IRenderer* pRenderer, CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		std::vector<CObjectBase*> m_arObjects;

		friend class CDefs;
		friend class CPattern;

		TRect m_oWindow;
		TRect m_oViewBox;
	};
}

#endif // CCONTAINER_H
