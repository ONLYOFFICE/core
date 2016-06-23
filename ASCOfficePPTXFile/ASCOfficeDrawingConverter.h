/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef ASC_OFFICE_DRAWING_CONVERTER
#define ASC_OFFICE_DRAWING_CONVERTER

#if defined(_WIN32) || defined (_WIN64)
	#include <atlbase.h>
	#include <atlstr.h>
#else
	#include "../DesktopEditor/common/ASCVariant.h"
#endif

#include "../Common/DocxFormat/Source/Base/Base.h"
#include "../Common/DocxFormat/Source/Base/Nullable.h"

#include "./Editor/Drawing/Shapes/BaseShape/PPTShape/PPTShapeEnum.h"

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
		class Xfrm;
		class Shape;
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
        bool									m_bIsUseConvertion2007;

		NSCommon::smart_ptr<PPTX::WrapperFile>*				m_pTheme;
		NSCommon::smart_ptr<PPTX::WrapperWritingElement>*	m_pClrMap;

		CString									m_strFontDirectory;
	public:

		CDrawingConverter();

		~CDrawingConverter();
	public:
        HRESULT SetMainDocument (BinDocxRW::CDocxSerializer* pDocument);
        HRESULT SetRelsPath (const CString& bsRelsPath);
		CString GetRelsPath();
        HRESULT SetMediaDstPath (const CString& bsMediaPath);
        HRESULT SetEmbedDstPath (const CString& bsEmbedPath);

        HRESULT AddShapeType(const CString& bsXml);
        HRESULT AddObject(const CString& bsXml, CString** pMainProps);
        HRESULT GetThemeBinary(BYTE** ppBinary, long& lBinarySize, const CString& bsThemeFilePath);

        HRESULT SaveThemeXml(LONG lStart, LONG lLength, const CString& bsThemePath);
        HRESULT SaveObject(LONG lStart, LONG lLength, const CString& bsMainProps, CString** bsXml);
        HRESULT SaveObjectEx(LONG lStart, LONG lLength, const CString& bsMainProps, LONG lDocType, CString** bsXml);

        HRESULT GetRecordBinary(LONG lRecordType, const CString& bsXml);
        HRESULT GetRecordXml(LONG lStart, LONG lLength, LONG lRecType, LONG lDocType, CString** bsXml);

        HRESULT SetDstContentRels();
        HRESULT SaveDstContentRels(const CString& bsRelsPath);
        HRESULT WriteRels (const CString& bsType, const CString& bsTarget, const CString& bsTargetMode, LONG* lId);
        HRESULT LoadClrMap (const CString& bsXml);

        HRESULT(GetTxBodyBinary)(const CString& bsXml);
        HRESULT(GetTxBodyXml)(LONG lStart, LONG lLength, CString** pbstrXml);
        HRESULT(SetFontDir)(const CString& bsFontDir);

		HRESULT SetFontPicker(COfficeFontPicker* pFontPicker);
        HRESULT(SetAdditionalParam)(const CString& ParamName, BYTE *pArray, size_t szCount);
        HRESULT(SetAdditionalParam)(const CString& ParamName, VARIANT ParamValue);
        HRESULT(GetAdditionalParam)(const CString& ParamName, BYTE **pArray, size_t& szCount);
        HRESULT(GetAdditionalParam)(const CString& ParamName, VARIANT* ParamValue);
	public:
		void SetFontManager(CFontManager* pFontManager);
	protected:
		nullable<PPTX::Logic::Xfrm> m_oxfrm_override;

		bool ParceObject(const CString& strXml, CString** pMainProps);
		void SendMainProps(const CString& strMainProps, CString**& pMainProps);

		PPTX::Logic::SpTreeElem doc_LoadShape(XmlUtils::CXmlNode& oNodeShape, CString**& pMainProps, bool bIsTop = true);
		PPTX::Logic::SpTreeElem doc_LoadGroup(XmlUtils::CXmlNode& oNode, CString**& pMainProps, bool bIsTop = true);

		CString GetVMLShapeXml(CPPTShape* pPPTShape);
		CString GetVMLShapeXml(PPTX::Logic::SpTreeElem& oElem);

		void CheckBrushShape(PPTX::Logic::SpTreeElem& oElem, XmlUtils::CXmlNode& oNode, PPTShapes::ShapeType eType, CPPTShape* pPPTShape);
		void CheckPenShape(PPTX::Logic::SpTreeElem& oElem, XmlUtils::CXmlNode& oNode, PPTShapes::ShapeType eType, CPPTShape* pPPTShape);

		void LoadCoordSize(XmlUtils::CXmlNode& oNode, ::CShape* pShape);
		CString GetDrawingMainProps(XmlUtils::CXmlNode& oNode, PPTX::CCSS& oCssStyles, CSpTreeElemProps& oProps);

		void ConvertMainPropsToVML(const CString& bsMainProps, NSBinPptxRW::CXmlWriter& oWriter, PPTX::Logic::SpTreeElem& oElem);
		void ConvertPicVML(PPTX::Logic::SpTreeElem& oElem, const CString& bsMainProps, NSBinPptxRW::CXmlWriter& oWriter);
		void ConvertShapeVML(PPTX::Logic::SpTreeElem& oShape, const CString& bsMainProps, NSBinPptxRW::CXmlWriter& oWriter);
		void ConvertGroupVML(PPTX::Logic::SpTreeElem& oGroup, const CString& bsMainProps, NSBinPptxRW::CXmlWriter& oWriter);

		void ConvertTextVML(XmlUtils::CXmlNode &nodeTextBox, PPTX::Logic::Shape* pShape);

		void Clear();
		HRESULT SetCurrentRelsPath();
	};
}
#endif //OOX_IFILE_CONTAINER_INCLUDE_H_
