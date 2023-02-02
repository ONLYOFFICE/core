#ifndef COBJECTBASE_H
#define COBJECTBASE_H

#include "../../../xml/include/xmlutils.h"
#include "../../IRenderer.h"
#include "CStyle.h"

#define MapCI std::map<std::wstring, std::wstring>::const_iterator

namespace SVG
{
	class CObjectBase
	{
	public:
		CObjectBase(CObjectBase* pParent = NULL) : m_pParent(pParent){};
		virtual ~CObjectBase(){};

		virtual bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode, const CGeneralStyle& oBaseStyle) = 0;
		virtual bool Draw(IRenderer* pRenderer) const = 0;
	private:
		void SaveNodeData(XmlUtils::CXmlNode& oNode, const CGeneralStyle& oBaseStyle)
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

			m_oStyle = oBaseStyle.GetStyle(GetFullPath());
		};

		std::vector<NSCSS::CNode> GetFullPath() const
		{
			if (NULL == m_pParent)
				return {m_oXmlNode};

			std::vector<NSCSS::CNode> arObjects = m_pParent->GetFullPath();
			arObjects.push_back({m_oXmlNode});
			return arObjects;
		}

		virtual void ApplyStyle(IRenderer* pRenderer, int& nTypePath) const = 0;

		void ApplyDefaultStroke(IRenderer* pRenderer, int& nTypePath) const
		{
			nTypePath += c_nStroke;
			pRenderer->put_PenColor(0);
		}

		void ApplyStroke(IRenderer* pRenderer, int& nTypePath, bool bUseDedault = false) const
		{
			if (!m_oStyle.m_oStroke.GetColor().Empty())
			{
				nTypePath += c_nStroke;
				pRenderer->put_PenColor(m_oStyle.m_oStroke.GetColor().ToInt());

				double dStrokeWidth = m_oStyle.m_oStroke.GetWidth().ToDouble();

				if (0 != dStrokeWidth)
					pRenderer->put_PenSize(dStrokeWidth);
			}
			else if (bUseDedault)
				ApplyDefaultStroke(pRenderer, nTypePath);

		}

		void ApplyDefaultFill(IRenderer* pRenderer, int& nTypePath) const
		{
			nTypePath += c_nWindingFillMode;
			pRenderer->put_BrushColor1(0);
		}

		void ApplyFill(IRenderer* pRenderer, int& nTypePath, bool bUseDedault = false) const
		{
			if (NSCSS::NSProperties::ColorType::ColorNone == m_oStyle.m_oBackground.GetColor().GetType())
				return;
			else if (NSCSS::NSProperties::ColorType::ColorHEX == m_oStyle.m_oBackground.GetColor().GetType() ||
			         NSCSS::NSProperties::ColorType::ColorRGB == m_oStyle.m_oBackground.GetColor().GetType())
			{
				nTypePath += c_nWindingFillMode;
				pRenderer->put_BrushColor1(m_oStyle.m_oBackground.GetColor().ToInt());
			}
			else if (bUseDedault)
			{
				ApplyDefaultFill(pRenderer, nTypePath);
			}
		}

		void ApplyTransform(IRenderer* pRenderer) const
		{
			pRenderer->ResetTransform();

			Aggplus::CMatrix oMatrix = m_oStyle.m_oTransform.GetMatrix().GetValue();

			pRenderer->SetTransform(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty());
		}

		void ApplyFont(IRenderer* pRenderer, double dScaleX) const
		{
			std::wstring wsName = m_oStyle.m_oFont.GetFamily().ToWString();

			if (!wsName.empty())
				pRenderer->put_FontName(wsName.substr(1, wsName.length() - 2));

			double dSize = m_oStyle.m_oFont.GetSize().ToDouble() /** dScaleX*/;

			if (0 != dSize)
				pRenderer->put_FontSize(dSize / 25.4 * 72);

			int lStyle = 0;
			if (m_oStyle.m_oFont.GetWeight() == L"bold")
				lStyle |= 0x01;
			if (m_oStyle.m_oFont.GetStyle() == L"italic")
				lStyle |= 0x02;
//			if (pFont->IsUnderline())
//				lStyle |= (1 << 2);
//			if (pFont->IsStrikeOut())
//				lStyle |= (1 << 7);

			pRenderer->put_FontStyle(lStyle);

			pRenderer->put_BrushType(c_BrushTypeSolid);

			int nColor = m_oStyle.m_oBackground.GetColor().ToInt();

			if (-1 == nColor)
				pRenderer->put_BrushColor1(0);
			else
				pRenderer->put_BrushColor1(nColor);

			pRenderer->put_BrushAlpha1(255);
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
		friend class CContainer;

		CObjectBase   *m_pParent;
		NSCSS::CNode   m_oXmlNode;
		CStyle         m_oStyle;
	};
}


#endif // COBJECTBASE_H
