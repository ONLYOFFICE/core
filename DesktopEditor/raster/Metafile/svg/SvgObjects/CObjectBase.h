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
			std::vector<std::wstring> arProperties, arValues;

			oNode.GetAllAttributes(arProperties, arValues);

			NSCSS::CNode oXmlNode;
			m_oXmlNode.m_sName  = oNode.GetName();

			for (unsigned int unIndex = 0; unIndex < arProperties.size(); ++unIndex)
			{
				if (L"class" == arProperties[unIndex])
					m_oXmlNode.m_sClass = arValues[unIndex];
				else if (L"id" == arProperties[unIndex])
					m_oXmlNode.m_sId = arValues[unIndex];
				else if (L"style" == arProperties[unIndex])
					m_oXmlNode.m_sStyle = arValues[unIndex];
				else
					m_oXmlNode.m_mAttrs.insert({arProperties[unIndex], arValues[unIndex]});
			}
		};

		std::vector<NSCSS::CNode> GetFullPath() const
		{
			if (NULL == m_pParent)
				return {m_oXmlNode};

			std::vector<NSCSS::CNode> arObjects = m_pParent->GetFullPath();
			arObjects.push_back({m_oXmlNode});
			return arObjects;
		}

		virtual void ApplyStyle(IRenderer* pRenderer, int& nTypePath) = 0;

		void ApplyDefaultStroke(IRenderer* pRenderer, int& nTypePath)
		{
			nTypePath += c_nStroke;
			pRenderer->put_PenColor(0);
		}

		void ApplyStroke(IRenderer* pRenderer, const CStyle& oStyle, int& nTypePath, bool bUseDedault = false)
		{
			if (SvgColorType::ColorHex == oStyle.GetStrokeColorType())
			{
				nTypePath += c_nStroke;
				pRenderer->put_PenColor(oStyle.GetStrokeColorN());

				double dStrokeWidth = oStyle.GetStrokeWidth();

				if (0 != dStrokeWidth)
					pRenderer->put_PenSize(dStrokeWidth);
			}
			else if (bUseDedault)
				ApplyDefaultStroke(pRenderer, nTypePath);
		}

		void ApplyDefaultFill(IRenderer* pRenderer, int& nTypePath)
		{
			nTypePath += c_nWindingFillMode;
			pRenderer->put_BrushColor1(0);
		}

		void ApplyFill(IRenderer* pRenderer, const CStyle& oStyle, int& nTypePath, bool bUseDedault = false)
		{
			if (SvgColorType::ColorHex == oStyle.GetFillType())
			{
				nTypePath += c_nWindingFillMode;
				pRenderer->put_BrushColor1(oStyle.GetFillN());
			}
			else if (SvgColorType::ColorNone == oStyle.GetFillType())
			{
				return;
			}
			else if (bUseDedault)
			{
				ApplyDefaultFill(pRenderer, nTypePath);
			}
		}

		void ApplyTransform(IRenderer* pRenderer, const CStyle& oStyle)
		{
			pRenderer->ResetTransform();

			Aggplus::CMatrix oMatrix = oStyle.GetTransform();

			pRenderer->SetTransform(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty());
		}

		friend class CLine;
		friend class CRect;
		friend class CCircle;
		friend class CEllipse;
		friend class CHeader;
		friend class CPath;
		friend class CText;
		friend class CTspan;
		friend class CPolyline;
		friend class CPolygon;

		CObjectBase          *m_pParent;
		const CGeneralStyle  *m_pStyle;
		NSCSS::CNode          m_oXmlNode;
	};
}


#endif // COBJECTBASE_H
