#ifndef COBJECTBASE_H
#define COBJECTBASE_H

#include "../../../../Common/3dParty/html/css/src/CNode.h"
#include "../../../../Common/3dParty/html/css/src/StaticFunctions.h"
#include "../../../xml/include/xmlutils.h"
#include "../../IRenderer.h"
#include "../SvgTypes.h"

namespace SVG
{
	class CDefs;

	struct TSvgStyles
	{
		SvgColor     m_oFill;
		TStroke      m_oStroke;
		SvgTransform m_oTransform;
		TClip        m_oClip;
		SvgColor     m_oMask;

		TSvgStyles& operator+=(const TSvgStyles& oSvgStyles)
		{
			m_oTransform.SetMatrix(oSvgStyles.m_oTransform.GetMatrix().ToWString(), 0, false);

			m_oFill.SetValue(L'#' + oSvgStyles.m_oFill.ToWString(), 0, false);
			m_oFill.SetOpacity(std::to_wstring(oSvgStyles.m_oFill.GetOpacity()), 0, false);

			m_oStroke.m_oColor.SetValue(L'#' + oSvgStyles.m_oStroke.m_oColor.ToWString(), 0, false);
			m_oStroke.m_oWidth.SetValue(oSvgStyles.m_oStroke.m_oWidth.ToWString(), 0, false);

			if (m_oStroke.m_arDash.empty() && !oSvgStyles.m_oStroke.m_arDash.empty())
				m_oStroke.m_arDash = oSvgStyles.m_oStroke.m_arDash;

			if (m_oStroke.m_oLineCap.Empty() && !oSvgStyles.m_oStroke.m_oLineCap.Empty())
				m_oStroke.m_oLineCap = oSvgStyles.m_oStroke.m_oLineCap;

			if (m_oStroke.m_oLineJoin.Empty() && !oSvgStyles.m_oStroke.m_oLineJoin.Empty())
				m_oStroke.m_oLineJoin = oSvgStyles.m_oStroke.m_oLineJoin;

			if (m_oClip.m_oHref.Empty() && !oSvgStyles.m_oClip.m_oHref.Empty())
				m_oClip.m_oHref = oSvgStyles.m_oClip.m_oHref;

			if (m_oClip.m_oRule.Empty() && !oSvgStyles.m_oClip.m_oRule.Empty())
				m_oClip.m_oRule = oSvgStyles.m_oClip.m_oRule;

			if (m_oMask.Empty() && !oSvgStyles.m_oMask.Empty() && NSCSS::NSProperties::ColorUrl == oSvgStyles.m_oMask.GetType())
				m_oMask= oSvgStyles.m_oMask;

			return *this;
		}
	};

	template <typename TypeParent>
	class CSvgObject
	{
	public:
		CSvgObject(XmlUtils::CXmlNode& oNode, TypeParent* pParent = NULL)
		    : m_pParent(pParent)
		{
			if (!oNode.IsValid())
				return;

			std::vector<std::wstring> arProperties, arValues;

			oNode.GetAllAttributes(arProperties, arValues);

			m_oXmlNode.m_sName  = oNode.GetName();

			for (unsigned int unIndex = 0; unIndex < arProperties.size(); ++unIndex)
			{
				if (L"class" == arProperties[unIndex])
				{
					m_oXmlNode.m_sClass = arValues[unIndex];
					std::transform(m_oXmlNode.m_sClass.begin(), m_oXmlNode.m_sClass.end(), m_oXmlNode.m_sClass.begin(), std::towlower);
				}
				else if (L"id" == arProperties[unIndex])
				{
					m_oXmlNode.m_sId = arValues[unIndex];
				}
				else if (L"style" == arProperties[unIndex])
					m_oXmlNode.m_sStyle = arValues[unIndex];
				else
					m_oXmlNode.m_mAttrs.insert({arProperties[unIndex], arValues[unIndex]});
			}
		};

		void SetData(const std::wstring wsStyles, unsigned short ushLevel, bool bHardMode = false)
		{
			if (wsStyles.empty())
				return;

			SetData(NSCSS::NS_STATIC_FUNCTIONS::GetRules(wsStyles), ushLevel, bHardMode);
		};

		virtual void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) = 0;

		std::wstring GetId() const
		{
			return m_oXmlNode.m_sId;
		};

		std::vector<NSCSS::CNode> GetFullPath() const
		{
			if (NULL == m_pParent)
				return {m_oXmlNode};

			std::vector<NSCSS::CNode> arObjects = m_pParent->GetFullPath();
			arObjects.push_back(m_oXmlNode);
			return arObjects;
		};
	private:
		friend class CUse;
		friend class CLine;
		friend class CRect;
		friend class CPath;
		friend class CText;
		friend class CTSpan;
		friend class CImage;
		friend class CCircle;
		friend class CPolygon;
		friend class CEllipse;
		friend class CPolyline;

		TypeParent   *m_pParent;
		NSCSS::CNode  m_oXmlNode;
	};

	class CSvgGraphicsObject : public CSvgObject<CSvgGraphicsObject>
	{
	public:
		CSvgGraphicsObject(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);
		virtual ~CSvgGraphicsObject();

		virtual bool Draw(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip = false, const TSvgStyles* pStyles = NULL) const = 0;

		virtual TBounds GetBounds() const = 0;
	private:
		void SetStroke(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetFill(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetTransform(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetClip(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetMask(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);

		void StartPath(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip) const;
		void EndPath(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip, const TSvgStyles* pOtherStyles = NULL) const;

		virtual void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const = 0;

		bool Apply(IRenderer* pRenderer, const TStroke* pStroke, bool bUseDefault = false) const;
		bool Apply(IRenderer* pRenderer, const SvgColor* pFill, const CDefs *pDefs, bool bUseDefault = false) const;
		bool Apply(IRenderer* pRenderer, const SvgTransform* pTransform, Aggplus::CMatrix& oOldMatrix) const;
		bool Apply(IRenderer* pRenderer, const TClip* pClip, const CDefs *pDefs) const;

		bool ApplyMask(IRenderer* pRenderer, const SvgColor* pMask, const CDefs *pDefs) const;
		bool ApplyDef(IRenderer* pRenderer, const CDefs *pDefs, const std::wstring& wsUrl) const;

		friend class CUse;
		friend class CLine;
		friend class CRect;
		friend class CPath;
		friend class CMask;
		friend class CTSpan;
		friend class CImage;
		friend class CCircle;
		friend class CPolygon;
		friend class CEllipse;
		friend class CPolyline;
		friend class CTextPath;
		friend class CClipPath;
		friend class CGraphicsContainer;

		TSvgStyles m_oStyles;
	};

//	class CObjectBase
//	{
//	public:
//		CObjectBase(CObjectBase* pParent = NULL);
//		virtual ~CObjectBase();

//		virtual bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) = 0;
//		virtual bool Draw(IRenderer* pRenderer, CDefs *pDefs) const = 0;

//		std::vector<NSCSS::CNode> GetFullPath() const;

//		void SetData(const std::wstring wsStyles, unsigned short ushLevel, bool bHardMode = false);

//		virtual void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) = 0;

//		std::wstring GetId() const;
//	private:
//		void SaveNodeData(XmlUtils::CXmlNode& oNode);

//		void SetStroke(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
//		void SetFill(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
//		void SetTransform(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);

//		virtual void ApplyStyle(IRenderer* pRenderer, CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const = 0;

//		void ApplyDefaultStroke(IRenderer* pRenderer, int& nTypePath) const;
//		void ApplyStroke(IRenderer* pRenderer, int& nTypePath, bool bUseDedault = false) const;
//		void ApplyDefaultFill(IRenderer* pRenderer, int& nTypePath) const;
//		void ApplyFill(IRenderer* pRenderer, CDefs *pDefs, int& nTypePath, bool bUseDedault = false) const;
//		void ApplyTransform(IRenderer* pRenderer, Aggplus::CMatrix& oOldMatrix) const;

//		bool ApplyDef(IRenderer* pRenderer, CDefs *pDefs, const std::wstring& wsUrl) const;

//		virtual TBounds GetBounds() const = 0;

//		friend class CLine;
//		friend class CRect;
//		friend class CPath;
//		friend class CText;
//		friend class CTspan;
//		friend class CImage;
//		friend class CCircle;
//		friend class CPolygon;
//		friend class CEllipse;
//		friend class CPolyline;
//		friend class CStopElement;
//		friend class CGraphicsContainer;

//		CObjectBase   *m_pParent;
//		NSCSS::CNode   m_oXmlNode;

//		//Styles
//		SvgColor     m_oFill;
//		TStroke      m_oStroke;
//		SvgTransform m_oTransform;
//	};
}


#endif // COBJECTBASE_H
