#ifndef COBJECTBASE_H
#define COBJECTBASE_H

#include "../../../../../Common/3dParty/html/css/src/CNode.h"
#include "../../../../graphics/IRenderer.h"
#include "../../../../common/IGrObject.h"

#include "../SvgTypes.h"
#include "../SvgReader.h"
#include "CStyle.h"

class CSvgFile;

namespace SVG
{
	struct TSvgStyles
	{
		SvgColor     m_oFill;
		TStroke      m_oStroke;
		SvgDigit     m_oOpacity;

		TSvgStyles& operator+=(const TSvgStyles& oSvgStyles);
	};
	
	struct TSvgTransformation
	{
		SvgTransform m_oTransform;
		TClip        m_oClip;
		SvgColor     m_oMask;
		SvgDigit     m_oOpacity;
		bool         m_bDraw;
	};

	enum ObjectType
	{
		RendererObject,
		AppliedObject
	};

	class CObject : public IGrObject
	{
	protected:
		CObject(const NSCSS::CNode& oData);
		CObject(CSvgReader& oReader);
		CObject(const CObject& oObject);
	public:
		virtual ~CObject();

		virtual ObjectType GetType() const = 0;

		virtual void SetAttribute(const std::string& sName, CSvgReader& oReader);

		void SetData(const std::wstring wsStyles, unsigned short ushLevel, bool bHardMode = false);

		virtual void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) = 0;

		void SetTransform(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetClip(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetMask(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetDisplay(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetOpacity(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);

		std::wstring GetId() const;
		virtual std::vector<NSCSS::CNode> GetFullPath() const;
	private:
		bool ApplyTransform(IRenderer* pRenderer, const SvgTransform* pTransform, Aggplus::CMatrix& oOldMatrix) const;
		bool ApplyClip(IRenderer* pRenderer, const TClip* pClip, const CSvgFile *pFile, const TBounds& oBounds) const;
		bool ApplyMask(IRenderer* pRenderer, const SvgColor* pMask, const CSvgFile *pFile, const TBounds& oBounds) const;

		bool ApplyDef(IRenderer* pRenderer, const CSvgFile *pFile, const std::wstring& wsUrl, const TBounds& oBounds) const;

		friend class CRenderedObject;
		friend class CAppliedObject;

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
		friend class CGraphicsContainer;

		friend class CClipPath;

		NSCSS::CNode       m_oXmlNode;
		TSvgTransformation m_oTransformation;
	};

	enum CommandeMode
	{
		CommandeModeDraw,
		CommandeModeClip,
		CommandeModeMask
	};

	class CSvgCalculator;

	class CRenderedObject : public CObject
	{
	protected:
		CRenderedObject(const NSCSS::CNode& oData, CRenderedObject* pParent = NULL);
		CRenderedObject(CSvgReader& oReader, CRenderedObject* pParent = NULL);
		CRenderedObject(const CRenderedObject& oRenderedObject);
	public:
		virtual ~CRenderedObject();

		template <class T, typename... Args>
		static T* Create(CSvgReader& oReader, const CSvgCalculator* pSvgCalculator, CRenderedObject* pParent = NULL, Args&&... args);

		ObjectType GetType() const override;

		virtual void SetAttribute(const std::string& sName, CSvgReader& oReader) override;
		virtual void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		virtual void ReadChildrens(CSvgReader& oReader, const CSvgCalculator* pSvgCalculator);

		virtual bool Draw(IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pStyles = NULL, const CRenderedObject* pContextObject = NULL) const = 0;

		virtual TBounds GetBounds() const = 0;

		std::vector<NSCSS::CNode> GetFullPath() const override;
	private:
		void SetStroke(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetFill(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);

		bool StartPath(IRenderer* pRenderer, const CSvgFile *pFile, Aggplus::CMatrix& oOldTransform, CommandeMode oMode = CommandeModeDraw) const;
		void EndPath(IRenderer* pRenderer, const CSvgFile *pFile, const Aggplus::CMatrix& oOldTransform, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContextObject = NULL) const;

		virtual void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CSvgFile *pFile, int& nTypePath, const CRenderedObject* pContextObject = NULL) const;

		bool ApplyStroke(IRenderer* pRenderer, const TStroke* pStroke, bool bUseDefault = false, const CRenderedObject* pContextObject = NULL) const;
		bool ApplyFill(IRenderer* pRenderer, const SvgColor* pFill, const CSvgFile *pFile, bool bUseDefault = false, const CRenderedObject* pContextObject = NULL) const;
		bool ApplyOpacity(IRenderer* pRenderer, const SvgDigit* pOpacity) const;

		void SetDefaultData();

		friend class CUse;
		friend class CLine;
		friend class CRect;
		friend class CPath;
		friend class CMask;
		friend class CTSpan;
		friend class CImage;
		friend class CCircle;
		friend class CSwitch;
		friend class CPolygon;
		friend class CEllipse;
		friend class CPolyline;
		friend class CTextPath;
		friend class CClipPath;
		friend class CGraphicsContainer;

		TSvgStyles       m_oStyles;
		CRenderedObject *m_pParent;
	};

	template<class T, typename... Args>
	inline T* CRenderedObject::Create(CSvgReader& oReader, const CSvgCalculator* pSvgCalculator, CRenderedObject* pParent, Args&&... args)
	{
		T* pObject = new T(oReader, pParent, std::forward<Args>(args)...);

		if (NULL == pObject)
			return NULL;

		START_READ_ATTRIBUTES(oReader)
			pObject->SetAttribute(sAttributeName, oReader);
		END_READ_ATTRIBUTES(oReader)

		if (NULL != pSvgCalculator)
			pSvgCalculator->SetData(pObject);

		pObject->ReadChildrens(oReader, pSvgCalculator);

		return pObject;
	}

	class CAppliedObject : public CObject
	{
	protected:
		CAppliedObject(CSvgReader& oReader);
	public:
		virtual ~CAppliedObject();

		ObjectType GetType() const override;

		virtual bool Apply(IRenderer* pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds) = 0;
	};
}


#endif // COBJECTBASE_H
