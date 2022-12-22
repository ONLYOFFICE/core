#ifndef COBJECTBASE_H
#define COBJECTBASE_H

#include "../../../xml/include/xmlutils.h"
#include "../../IRenderer.h"
#include "CStyle.h"

namespace SVG
{
	class CObjectBase
	{
	public:
		CObjectBase(CObjectBase* pParent = NULL, CGeneralStyle* pBaseStyle = NULL) : m_pParent(pParent), m_pStyle(pBaseStyle) {};
		virtual ~CObjectBase(){};

		virtual bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) = 0;
		virtual bool Draw(IRenderer* pRenderer) = 0;
	private:
		void SaveNodeData(XmlUtils::CXmlNode& oNode)
		{
			if (NULL == m_pStyle)
				return;

			NSCSS::CNode oXmlNode;

			m_oXmlNode.m_sName  = oNode.GetName();
			m_oXmlNode.m_sClass = oNode.GetAttribute(L"class");
			m_oXmlNode.m_sId    = oNode.GetAttribute(L"id");
			m_oXmlNode.m_sStyle = oNode.GetAttribute(L"style");

			std::vector<std::wstring> arProperties, arValues;

			oNode.GetAllAttributes(arProperties, arValues);

			for (unsigned int unIndex = 0; unIndex < arProperties.size(); ++unIndex)
				m_oXmlNode.m_mAttrs.insert({arProperties[unIndex], arValues[unIndex]});
		};

		virtual void ApplyStyle(IRenderer* pRenderer) = 0;

		friend class CLine;
		friend class CRect;
		friend class CCircle;
		friend class CEllipse;
		friend class CPath;
		friend class CText;
		friend class CTspan;

		CObjectBase    *m_pParent;
		const CGeneralStyle  *m_pStyle;
		NSCSS::CNode    m_oXmlNode;
	};
}


#endif // COBJECTBASE_H
