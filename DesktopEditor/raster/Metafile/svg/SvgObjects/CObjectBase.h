#ifndef COBJECTBASE_H
#define COBJECTBASE_H

#include "../../../xml/include/xmlutils.h"
#include "./../IRenderer.h"
#include "CStyle.h"

namespace SVG
{
	class CObjectBase
	{
	public:
		CObjectBase(CObjectBase* pParent = NULL) : m_pParent(pParent) {};
		virtual ~CObjectBase(){};

		virtual bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) = 0;
		virtual bool Draw(IRenderer* pRenderer) = 0;
	private:
		void UpdateStyle(XmlUtils::CXmlNode& oNode)
		{
			NSCSS::CNode oXmlNode;

			oXmlNode.m_sName  = oNode.GetName();
			oXmlNode.m_sClass = oNode.GetAttribute(L"class");
			oXmlNode.m_sId    = oNode.GetAttribute(L"id");
			oXmlNode.m_sStyle = oNode.GetAttribute(L"style");

			std::vector<std::wstring> arProperties, arValues;

			oNode.GetAllAttributes(arProperties, arValues);

			for (unsigned int unIndex = 0; unIndex < arProperties.size(); ++unIndex)
				oXmlNode.m_mAttrs.insert({arProperties[unIndex], arValues[unIndex]});

			m_oStyle.SetStyle({oXmlNode});
		};

		virtual void ApplyStyle(IRenderer* pRenderer) = 0;

		friend class CLine;
		friend class CRect;
		friend class CCircle;
		friend class CEllipse;
		friend class CPath;
		friend class CText;
		friend class CTspan;

		CObjectBase* m_pParent;
		CStyle   m_oStyle;
	};
}


#endif // COBJECTBASE_H
