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

		bool ReadHeader(XmlUtils::CXmlNode &oElement, const CGeneralStyle& oBaseStyle);

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode, const CGeneralStyle& oBaseStyle) override;
		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode, const CSvgParser& oParser, CSvgFile* pFile);
		bool Draw(IRenderer* pRenderer) const override;

		void Clear();

		bool Empty() const;

		CHeader* GetHeader() const;

		void AddObject(CObjectBase* pObject);
	private:
		void ApplyStyle(IRenderer* pRenderer, int& nTypePath) const override;

		CHeader                  *m_pHeader;
		std::vector<CObjectBase*> m_arObjects;
	};
}

#endif // CCONTAINER_H
