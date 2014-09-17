#ifndef ASC_OFFICE_DRAWING_CONVERTER
#define ASC_OFFICE_DRAWING_CONVERTER

//todo
#include <atlbase.h>
#include <atlstr.h>
#include "../Common/DocxFormat/Source/Base/Base.h"
#include "../ASCPresentationEditor/OfficeDrawing/Shapes/BaseShape/PPTShape/PPTShapeEnum.h"

class CShape;
class CPPTShape;
namespace XmlUtils
{
	class CXmlNode;
}
namespace NSCommon
{
	template<typename Type> class smart_ptr;
}
namespace NSBinPptxRW
{
	class CBinaryFileWriter;
	class CBinaryFileReader;
	class CImageManager2;
	class CXmlWriter;
}
namespace PPTX
{
	class WrapperFile;
	class WrapperWritingElement;
	class CCommonRels;

	namespace Logic
	{
		class SpTreeElem;
	}

	class CStringTrimmer
	{
	public:
		CAtlArray<CString>	m_arParams;
		TCHAR				m_Separator;

	public:
		CStringTrimmer();
		~CStringTrimmer();

	public:
		void LoadFromString(CString& strParams);

		double GetParameter(int nIndex, double dDefault);
	};

	class CCSS
	{
	public:
		CAtlMap<CString, CString> m_mapSettings;
		CString m_strClassName;

	public:
		CCSS();
		~CCSS();
		AVSINLINE void Clear();

	public:
		void LoadFromString(CString& strParams);
		void LoadFromString2(CString& strParams);
	};

	class CStylesCSS
	{
	public: 
		CAtlArray<CCSS> m_arStyles;

	public:
		CStylesCSS();
		~CStylesCSS();
		AVSINLINE void Clear();

	public:
		void LoadStyles(CString& strParams);
	};
}
class CSpTreeElemProps
{
public:
	LONG X;
	LONG Y;
	LONG Width;
	LONG Height;

	bool IsTop;

public:
	CSpTreeElemProps();
};

class CElementProps
{
public:
	CAtlMap<LONG, VARIANT> m_Properties;

public:
	CElementProps();

	~CElementProps();

	void FinalRelease();

public:
	STDMETHOD(GetProperty)(LONG lId, VARIANT* pProp);
	STDMETHOD(SetProperty)(LONG lId, VARIANT prop);

public:
	static bool CopyProperty(VARIANT& oDst, const VARIANT& oSrc);
};

namespace NSBinPptxRW
{
	class CDrawingConverter 
	{
	public:
		class CElement
		{
		public:
			PPTX::WrapperWritingElement*	m_pElement;
			CElementProps*					m_pProps;

		public:
			CElement();
			~CElement();

			CElement& operator=(const CElement& oSrc);
			CElement(const CElement& oSrc);
		};


		CAtlMap<CString, ::CShape*>				m_mapShapeTypes;
		CAtlMap<CString, NSCommon::smart_ptr<PPTX::CCommonRels>>	m_mapRels;
		CString									m_strCurrentRelsPath;

		NSBinPptxRW::CBinaryFileWriter*			m_pBinaryWriter;
		int										m_lNextId;

		int										m_lCurrentObjectTop;

		NSBinPptxRW::CBinaryFileReader*			m_pReader;
		NSBinPptxRW::CImageManager2*			m_pImageManager;
		NSBinPptxRW::CXmlWriter*				m_pXmlWriter;
		int										m_nCurrentIndexObject;
		//todo
#ifdef AVS_USE_CONVERT_PPTX_TOCUSTOM_VML
		IASCRenderer*							m_pOOXToVMLRenderer;
#endif
		BOOL									m_bIsUseConvertion2007;

		NSCommon::smart_ptr<PPTX::WrapperFile>*				m_pTheme;
		NSCommon::smart_ptr<PPTX::WrapperWritingElement>*	m_pClrMap;

		CString									m_strFontDirectory;
	public:

		CDrawingConverter();

		~CDrawingConverter();
	public:
		STDMETHOD(SetMainDocument)(IUnknown* pDocument);
		STDMETHOD(SetRelsPath)(BSTR bsRelsPath);
		STDMETHOD(SetMediaDstPath)(BSTR bsMediaPath);

		STDMETHOD(AddShapeType)(BSTR bsXml);
		STDMETHOD(AddObject)(BSTR bsXml, BSTR* pMainProps, SAFEARRAY** ppBinary);
		STDMETHOD(AddObject2)(BSTR bsXml, SAFEARRAY* pBinaryObj, BSTR* pXmlOutput);
		STDMETHOD(GetThemeBinary)(BSTR bsThemeFilePath, SAFEARRAY** ppBinary);

		STDMETHOD(SaveThemeXml)(SAFEARRAY* pBinaryTheme, LONG lStart, LONG lLength, BSTR bsThemePath);
		STDMETHOD(SaveObject)(SAFEARRAY* pBinaryObj, LONG lStart, LONG lLength, BSTR bsMainProps, BSTR* bsXml);
		STDMETHOD(SaveObjectEx)(SAFEARRAY* pBinaryObj, LONG lStart, LONG lLength, BSTR bsMainProps, LONG lDocType, BSTR* bsXml);

		STDMETHOD(GetRecordBinary)(LONG lRecordType, BSTR bsXml, SAFEARRAY** ppBinary);
		STDMETHOD(GetRecordXml)(SAFEARRAY* pBinaryObj, LONG lStart, LONG lLength, LONG lRecType, LONG lDocType, BSTR* bsXml);

		STDMETHOD(SetDstContentRels)();
		STDMETHOD(SaveDstContentRels)(BSTR bsRelsPath);
		STDMETHOD(WriteRels)(BSTR bsType, BSTR bsTarget, BSTR bsTargetMode, LONG* lId);
		STDMETHOD(LoadClrMap)(BSTR bsXml);

		STDMETHOD(GetTxBodyBinary)(BSTR bsXml, SAFEARRAY** ppBinary);
		STDMETHOD(GetTxBodyXml)(SAFEARRAY* pBinary, LONG lStart, LONG lLength, BSTR *pbstrXml);
		STDMETHOD(SetFontDir)(BSTR bsFontDir);

		STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue);
		STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue);
	protected:
		bool ParceObject(CString& strXml, BSTR* pMainProps, SAFEARRAY** ppBinary);
		void SendMainProps(CString& strMainProps, BSTR*& pMainProps);

		PPTX::Logic::SpTreeElem doc_LoadShape(XmlUtils::CXmlNode& oNode, BSTR*& pMainProps, bool bIsTop = true);
		PPTX::Logic::SpTreeElem doc_LoadGroup(XmlUtils::CXmlNode& oNode, BSTR*& pMainProps, bool bIsTop = true);

		CString GetVMLShapeXml(CPPTShape* pPPTShape);
		CString GetVMLShapeXml(PPTX::Logic::SpTreeElem& oElem);

		void CheckBrushShape(PPTX::Logic::SpTreeElem& oElem, XmlUtils::CXmlNode& oNode, PPTShapes::ShapeType eType, CPPTShape* pPPTShape);
		void CheckPenShape(PPTX::Logic::SpTreeElem& oElem, XmlUtils::CXmlNode& oNode, PPTShapes::ShapeType eType, CPPTShape* pPPTShape);

		void LoadCoordSize(XmlUtils::CXmlNode& oNode, ::CShape* pShape);
		CString GetDrawingMainProps(XmlUtils::CXmlNode& oNode, PPTX::CCSS& oCssStyles, CSpTreeElemProps& oProps);

		void ConvertMainPropsToVML(BSTR bsMainProps, NSBinPptxRW::CXmlWriter& oWriter, PPTX::Logic::SpTreeElem& oElem);
		void ConvertShapeVML(PPTX::Logic::SpTreeElem& oShape, BSTR bsMainProps, NSBinPptxRW::CXmlWriter& oWriter);
		void ConvertGroupVML(PPTX::Logic::SpTreeElem& oGroup, BSTR bsMainProps, NSBinPptxRW::CXmlWriter& oWriter);

		void Clear();
		HRESULT SetCurrentRelsPath();
	};
}
#endif //OOX_IFILE_CONTAINER_INCLUDE_H_