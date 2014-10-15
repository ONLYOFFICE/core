#ifndef ASC_OFFICE_DRAWING_CONVERTER
#define ASC_OFFICE_DRAWING_CONVERTER

#ifdef _WIN32
#include <atlbase.h>
#include <atlstr.h>
#else
#include "../../../../DesktopEditor/common/ASCVariant.h"
#endif

#include "../Common/DocxFormat/Source/Base/Base.h"
#include "../ASCPresentationEditor/OfficeDrawing/Shapes/BaseShape/PPTShape/PPTShapeEnum.h"

#include <vector>
#include <map>

class IRenderer;
class CShape;
class CPPTShape;
class CFontManager;
class COfficeFontPicker;

namespace XmlUtils
{
	class CXmlNode;
}
namespace NSCommon
{
	template<typename Type> class smart_ptr;
}
namespace BinDocxRW
{
	class CDocxSerializer;
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
		std::vector<CString>	m_arParams;
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
		std::map<CString, CString> m_mapSettings;
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
		std::vector<CCSS> m_arStyles;

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
	std::map<LONG, VARIANT> m_Properties;

public:
	CElementProps();

	~CElementProps();

	void FinalRelease();

public:
    HRESULT(GetProperty)(LONG lId, VARIANT* pProp);
    HRESULT(SetProperty)(LONG lId, VARIANT prop);

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


		std::map<CString, CShape*>								m_mapShapeTypes;
		std::map<CString, NSCommon::smart_ptr<PPTX::CCommonRels>>	m_mapRels;
		CString									m_strCurrentRelsPath;

		NSBinPptxRW::CBinaryFileWriter*			m_pBinaryWriter;
		int										m_lNextId;

		int										m_lCurrentObjectTop;

		NSBinPptxRW::CBinaryFileReader*			m_pReader;
		NSBinPptxRW::CImageManager2*			m_pImageManager;
		NSBinPptxRW::CXmlWriter*				m_pXmlWriter;
		int										m_nCurrentIndexObject;
		IRenderer*								m_pOOXToVMLRenderer;
		BOOL									m_bIsUseConvertion2007;

		NSCommon::smart_ptr<PPTX::WrapperFile>*				m_pTheme;
		NSCommon::smart_ptr<PPTX::WrapperWritingElement>*	m_pClrMap;

		CString									m_strFontDirectory;
	public:

		CDrawingConverter();

		~CDrawingConverter();
	public:
        HRESULT SetMainDocument (BinDocxRW::CDocxSerializer* pDocument);
        HRESULT SetRelsPath (CString& bsRelsPath);
		CString GetRelsPath();
        HRESULT SetMediaDstPath (CString& bsMediaPath);

        HRESULT AddShapeType(CString& bsXml);
        HRESULT AddObject(CString& bsXml, CString** pMainProps);
        HRESULT GetThemeBinary(BYTE** ppBinary, long& lBinarySize, CString& bsThemeFilePath);

        HRESULT SaveThemeXml(LONG lStart, LONG lLength, CString& bsThemePath);
        HRESULT SaveObject(LONG lStart, LONG lLength, CString& bsMainProps, CString** bsXml);
        HRESULT SaveObjectEx(LONG lStart, LONG lLength, CString& bsMainProps, LONG lDocType, CString** bsXml);

        HRESULT GetRecordBinary(LONG lRecordType, CString& bsXml);
        HRESULT GetRecordXml(LONG lStart, LONG lLength, LONG lRecType, LONG lDocType, CString** bsXml);

        HRESULT SetDstContentRels();
        HRESULT SaveDstContentRels(CString& bsRelsPath);
        HRESULT WriteRels (CString& bsType, CString& bsTarget, CString& bsTargetMode, LONG* lId);
        HRESULT LoadClrMap (CString& bsXml);

        HRESULT(GetTxBodyBinary)(CString& bsXml);
        HRESULT(GetTxBodyXml)(LONG lStart, LONG lLength, CString** pbstrXml);
        HRESULT(SetFontDir)(CString& bsFontDir);

		HRESULT SetFontPicker(COfficeFontPicker* pFontPicker);
        HRESULT(SetAdditionalParam)(CString& ParamName, VARIANT ParamValue);
        HRESULT(GetAdditionalParam)(CString& ParamName, VARIANT* ParamValue);
	public:
		void SetFontManager(CFontManager* pFontManager);
	protected:
		bool ParceObject(CString& strXml, CString** pMainProps);
		void SendMainProps(CString& strMainProps, CString**& pMainProps);

		PPTX::Logic::SpTreeElem doc_LoadShape(XmlUtils::CXmlNode& oNode, CString**& pMainProps, bool bIsTop = true);
		PPTX::Logic::SpTreeElem doc_LoadGroup(XmlUtils::CXmlNode& oNode, CString**& pMainProps, bool bIsTop = true);

		CString GetVMLShapeXml(CPPTShape* pPPTShape);
		CString GetVMLShapeXml(PPTX::Logic::SpTreeElem& oElem);

		void CheckBrushShape(PPTX::Logic::SpTreeElem& oElem, XmlUtils::CXmlNode& oNode, PPTShapes::ShapeType eType, CPPTShape* pPPTShape);
		void CheckPenShape(PPTX::Logic::SpTreeElem& oElem, XmlUtils::CXmlNode& oNode, PPTShapes::ShapeType eType, CPPTShape* pPPTShape);

		void LoadCoordSize(XmlUtils::CXmlNode& oNode, ::CShape* pShape);
		CString GetDrawingMainProps(XmlUtils::CXmlNode& oNode, PPTX::CCSS& oCssStyles, CSpTreeElemProps& oProps);

		void ConvertMainPropsToVML(CString& bsMainProps, NSBinPptxRW::CXmlWriter& oWriter, PPTX::Logic::SpTreeElem& oElem);
		void ConvertShapeVML(PPTX::Logic::SpTreeElem& oShape, CString& bsMainProps, NSBinPptxRW::CXmlWriter& oWriter);
		void ConvertGroupVML(PPTX::Logic::SpTreeElem& oGroup, CString& bsMainProps, NSBinPptxRW::CXmlWriter& oWriter);

		void Clear();
		HRESULT SetCurrentRelsPath();
	};
}
#endif //OOX_IFILE_CONTAINER_INCLUDE_H_
