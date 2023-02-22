#ifndef CCONTAINER_H
#define CCONTAINER_H

#include "CObjectBase.h"

#include "../../../graphics/pro/Fonts.h"

class CSvgFile;

namespace SVG
{
	class CSvgParser;

	template<typename TypeObject>
	class CContainer
	{
	public:
		CContainer(){};
		~CContainer()
		{
			Clear();
		};

		void Clear()
		{
			for (TypeObject* pObject : m_arObjects)
				delete pObject;
		}

		 bool Empty() const
		 {
			return m_arObjects.empty();
		 };

		void AddObject(TypeObject* pObject)
		{
			if (NULL != pObject)
				m_arObjects.push_back(pObject);
		};
	private:
		std::vector<TypeObject*> m_arObjects;

		friend class CDefs;
		friend class CPattern;
		friend class CGradient;
		friend class CClipPath;
		friend class CGraphicsContainer;
	};

	class CGraphicsContainer : public CContainer<CSvgGraphicsObject>, public CSvgGraphicsObject
	{
	public:
		CGraphicsContainer(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);
		CGraphicsContainer(double dWidth, double dHeight, XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip = false) const override;

		TRect GetWindow() const;
		TRect GetViewBox() const;

		CSvgGraphicsObject* GetObject(const std::wstring& wsId) const;
	private:
		void ApplyStyle(IRenderer* pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		TBounds GetBounds() const override;

		friend class CPattern;

		TRect m_oWindow;
		TRect m_oViewBox;
	};
}

#endif // CCONTAINER_H
