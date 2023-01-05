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

			double dM11, dM12, dM21, dM22, dDx, dDy;

			oStyle.GetTransform(dM11, dM12, dM21, dM22, dDx, dDy);

			pRenderer->SetTransform(0.89 * dM11 * 25.4 / 96, dM12, dM21, 0.89 * dM22 * 25.4 / 96, dDx, dDy);
		}

		friend class CLine;
		friend class CRect;
		friend class CCircle;
		friend class CEllipse;
		friend class CPath;
		friend class CText;
		friend class CTspan;
		friend class CPolyline;
		friend class CPolygon;

		CObjectBase    *m_pParent;
		const CGeneralStyle  *m_pStyle;
		NSCSS::CNode    m_oXmlNode;
	};
}


#endif // COBJECTBASE_H
