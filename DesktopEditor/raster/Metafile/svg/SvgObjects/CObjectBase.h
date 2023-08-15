#ifndef COBJECTBASE_H
#define COBJECTBASE_H

#include "../../../../../Common/3dParty/html/css/src/CNode.h"
#include "../../../../../Common/3dParty/html/css/src/StaticFunctions.h"
#include "../../../../xml/include/xmlutils.h"
#include "../../../../graphics/IRenderer.h"
#include "../SvgTypes.h"

class CSvgFile;

namespace SVG
{
	struct TSvgStyles
	{
		SvgColor     m_oFill;
		TStroke      m_oStroke;
		SvgTransform m_oTransform;
		TClip        m_oClip;
		SvgColor     m_oMask;
		bool         m_bDisplay;

		TSvgStyles& operator+=(const TSvgStyles& oSvgStyles)
		{
			m_oTransform.SetMatrix(oSvgStyles.m_oTransform.GetMatrix().ToWString(), 0, false);

			if (!oSvgStyles.m_oFill.Empty())
				m_oFill.SetValue(L'#' + oSvgStyles.m_oFill.ToWString(), 0, false);

			m_oFill.SetOpacity(std::to_wstring(oSvgStyles.m_oFill.GetOpacity()), 0, false);

			if (!oSvgStyles.m_oStroke.m_oColor.Empty())
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

	enum ObjectType
	{
		RendererObject,
		AppliedObject
	};

	class CObject
	{
	public:
		CObject(const NSCSS::CNode& oData);
		CObject(XmlUtils::CXmlNode& oNode);
		virtual ~CObject();

		virtual ObjectType GetType() const = 0;

		void SetData(const std::wstring wsStyles, unsigned short ushLevel, bool bHardMode = false);

		virtual void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) = 0;

		std::wstring GetId() const;
		virtual std::vector<NSCSS::CNode> GetFullPath() const;

	private:
		friend class CRenderedObject;

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

		NSCSS::CNode  m_oXmlNode;
	};

	enum CommandeMode
	{
		CommandeModeDraw,
		CommandeModeClip,
		CommandeModeMask
	};

	class CRenderedObject : public CObject
	{
	public:
		CRenderedObject(const NSCSS::CNode& oData, CRenderedObject* pParent = NULL);
		CRenderedObject(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL);
		virtual ~CRenderedObject();

		ObjectType GetType() const override;

		virtual void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		virtual bool Draw(IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pStyles = NULL) const = 0;

		virtual TBounds GetBounds() const = 0;

		std::vector<NSCSS::CNode> GetFullPath() const override;
	private:
		void SetDefaultStyles();

		void SetStroke(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetFill(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetTransform(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetClip(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetMask(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetDisplay(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);

		bool StartPath(IRenderer* pRenderer, const CSvgFile *pFile, Aggplus::CMatrix& oOldTransform, CommandeMode oMode = CommandeModeDraw) const;
		void EndPath(IRenderer* pRenderer, const CSvgFile *pFile, const Aggplus::CMatrix& oOldTransform, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL) const;

		virtual void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CSvgFile *pFile, int& nTypePath) const;

		bool Apply(IRenderer* pRenderer, const TStroke* pStroke, bool bUseDefault = false) const;
		bool Apply(IRenderer* pRenderer, const SvgColor* pFill, const CSvgFile *pFile, bool bUseDefault = false) const;
		bool Apply(IRenderer* pRenderer, const SvgTransform* pTransform, Aggplus::CMatrix& oOldMatrix) const;
		bool Apply(IRenderer* pRenderer, const TClip* pClip, const CSvgFile *pFile) const;

		bool ApplyMask(IRenderer* pRenderer, const SvgColor* pMask, const CSvgFile *pFile) const;
		bool ApplyDef(IRenderer* pRenderer, const CSvgFile *pFile, const std::wstring& wsUrl) const;

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

		TSvgStyles       m_oStyles;
		CRenderedObject *m_pParent;
	};

	class CAppliedObject : public CObject
	{
	public:
		CAppliedObject(XmlUtils::CXmlNode& oNode);
		virtual ~CAppliedObject();

		ObjectType GetType() const override;

		virtual bool Apply(IRenderer* pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds) = 0;
	};
}


#endif // COBJECTBASE_H
