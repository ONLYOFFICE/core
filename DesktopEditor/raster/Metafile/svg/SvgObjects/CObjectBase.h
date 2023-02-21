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
					std::transform(m_oXmlNode.m_sId.begin(), m_oXmlNode.m_sId.end(), m_oXmlNode.m_sId.begin(), std::towlower);
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

			const std::vector<std::wstring> arWords = NSCSS::NS_STATIC_FUNCTIONS::GetWordsWithSigns(wsStyles, L":;");

			if (arWords.empty())
				return;

			std::wstring wsProperty, wsValue;

			std::map<std::wstring, std::wstring> mAttributes;

			for (std::vector<std::wstring>::const_iterator iWord = arWords.begin(); iWord != arWords.end(); ++iWord)
			{
				if ((*iWord).back() == L':')
				{
					wsProperty = *iWord;
					wsProperty.pop_back();
				}
				else
				{
					wsValue += *iWord;

					if (L' ' == wsValue.front())
						wsValue.erase(0, 1);

					if (!wsValue.empty() && ((*iWord).back() == L';' || iWord == (arWords.end() - 1)))
					{
						if (wsValue.back() == L';')
							wsValue.pop_back();

						std::transform(wsProperty.begin(), wsProperty.end(), wsProperty.begin(), tolower);
						std::transform(wsValue.begin(), wsValue.end(), wsValue.begin(), tolower);
						mAttributes[wsProperty] = wsValue;
						wsProperty.clear();
						wsValue.clear();
					}
				}
			}

			if (!mAttributes.empty())
				SetData(mAttributes, ushLevel, bHardMode);
		};

		virtual void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false)
		{

		};

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
		friend class CLine;
		friend class CRect;
		friend class CPath;
		friend class CText;
		friend class CTspan;
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

		virtual bool Draw(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip = false) const = 0;

		virtual TBounds GetBounds() const = 0;
	private:
		void SetStroke(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetFill(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetTransform(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetClip(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);

		void StartPath(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip) const;
		void StartStandardPath(IRenderer* pRenderer, const CDefs *pDefs) const;
		void StartClipPath(IRenderer* pRenderer) const;
		void EndPath(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip) const;
		void EndStandardPath(IRenderer* pRenderer, const CDefs *pDefs) const;
		void EndClipPath(IRenderer* pRenderer) const;

		virtual void ApplyStyle(IRenderer* pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const = 0;

		void ApplyDefaultStroke(IRenderer* pRenderer, int& nTypePath) const;
		void ApplyStroke(IRenderer* pRenderer, int& nTypePath, bool bUseDedault = false) const;
		void ApplyDefaultFill(IRenderer* pRenderer, int& nTypePath) const;
		void ApplyFill(IRenderer* pRenderer, const CDefs *pDefs, int& nTypePath, bool bUseDedault = false) const;
		void ApplyClip(IRenderer* pRenderer, const CDefs *pDefs) const;
		void ApplyTransform(IRenderer* pRenderer, Aggplus::CMatrix& oOldMatrix) const;

		bool ApplyDef(IRenderer* pRenderer, const CDefs *pDefs, const std::wstring& wsUrl) const;

		friend class CLine;
		friend class CRect;
		friend class CPath;
		friend class CText;
		friend class CTspan;
		friend class CImage;
		friend class CCircle;
		friend class CPolygon;
		friend class CEllipse;
		friend class CPolyline;

		//Styles
		SvgColor     m_oFill;
		TStroke      m_oStroke;
		SvgTransform m_oTransform;
		SvgColor     m_oClip;
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
