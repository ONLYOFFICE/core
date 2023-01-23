/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "../../../DesktopEditor/common/ASCVariant.h"

#include "../../Base/Base.h"
#include "../../Base/Nullable.h"

#include "../../../MsBinaryFile/Common/Vml/PPTShape/PptShapeEnum.h"

#include <vector>
#include <map>

#include "../../../DesktopEditor/graphics/pro/Fonts.h"

class IRenderer;
class COfficeFontPicker;

class CShape;
typedef boost::shared_ptr<CShape> CShapePtr;

class CPPTShape;

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
namespace OOX
{
	class CContentTypes;
	class IFileContainer;
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
	class Theme;

	namespace Logic
	{
		class SpTreeElem;
		class Xfrm;
		class Shape;
		class ClrMap;
	}

	class CStringTrimmer
	{
	public:
		std::vector<std::wstring>	m_arParams;
        wchar_t                     m_Separator;

	public:
		CStringTrimmer();
		~CStringTrimmer();

	public:
		void LoadFromString(std::wstring& strParams);

		double GetParameter(int nIndex, double dDefault);
	};

	class CCSS
	{
	public:
        std::map<std::wstring, std::wstring>    m_mapSettings;
        std::wstring                            m_strClassName;

	public:
		CCSS();
		~CCSS();
		void Clear();

	public:
		void LoadFromString(std::wstring& strParams);
		void LoadFromString2(std::wstring& strParams);
	};

	class CStylesCSS
	{
	public: 
		std::vector<CCSS> m_arStyles;

	public:
		CStylesCSS();
		~CStylesCSS();
		void Clear();

	public:
		void LoadStyles(std::wstring& strParams);
	};
}
class CSpTreeElemProps
{
public:
    long X;
    long Y;
    long Width;
    long Height;

	bool IsTop;

public:
	CSpTreeElemProps();
};

class CElementProps
{
public:
    std::map<long, ASC_VARIANT> m_Properties;

public:
	CElementProps();

	~CElementProps();

	void FinalRelease();

public:
    HRESULT(GetProperty)(long lId, ASC_VARIANT* pProp);
    HRESULT(SetProperty)(long lId, ASC_VARIANT prop);

public:
    static bool CopyProperty(ASC_VARIANT& oDst, const ASC_VARIANT& oSrc);

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

			CElement();
			~CElement();

			CElement& operator=(const CElement& oSrc);
			CElement(const CElement& oSrc);
		};


        std::map<std::wstring, CShapePtr>					m_mapShapeTypes;

        NSBinPptxRW::CBinaryFileWriter*                     m_pBinaryWriter;
        int                                                 m_lNextId;
		unsigned int										m_nDrawingMaxZIndex = 0; // для смешанных записей pict & Drawing 

        int                                                 m_lCurrentObjectTop;

        NSBinPptxRW::CBinaryFileReader*                     m_pReader;
        NSBinPptxRW::CImageManager2*                        m_pImageManager;
        NSBinPptxRW::CXmlWriter*                            m_pXmlWriter;
        int                                                 m_nCurrentIndexObject;
        IRenderer*                                          m_pOOXToVMLRenderer;
        bool                                                m_bIsUseConvertion2007;

		NSCommon::smart_ptr<PPTX::Theme>*					m_pTheme;
		NSCommon::smart_ptr<PPTX::Logic::ClrMap>*			m_pClrMap;

        std::wstring                                        m_strFontDirectory;

		CDrawingConverter();
		~CDrawingConverter();

		void							SetRels(OOX::IFileContainer *container);
		void							SetRels(smart_ptr<OOX::IFileContainer> container);
		smart_ptr<OOX::IFileContainer>	GetRels();
		
		void SetMainDocument (BinDocxRW::CDocxSerializer* pDocument);

        void SetSrcPath         (const std::wstring& sPath, int nDocType = 1/*XMLWRITER_DOC_TYPE_DOCX*/);
        void SetDstPath         (const std::wstring& sPath);
		
		void SetTempPath		(const std::wstring& sPath);
		std::wstring GetTempPath();

        void SetMediaDstPath    (const std::wstring& sMediaPath);
        void SetEmbedDstPath    (const std::wstring& sEmbedPath);

		void ClearShapeTypes	();
		HRESULT AddShapeType	(const std::wstring& sXml);
		void AddShapeType		(XmlUtils::CXmlNode& oNodeST);

        HRESULT AddObject           (const std::wstring& sXml, std::wstring** pMainProps);

		void ConvertVml(const std::wstring& sXml, std::vector<nullable<PPTX::Logic::SpTreeElem>> &elements);

        HRESULT SaveObject          (long lStart, long lLength, const std::wstring& sMainProps, std::wstring & sXml);
        HRESULT SaveObjectEx        (long lStart, long lLength, const std::wstring& sMainProps, int nDocType, std::wstring & sXml);

        void SaveObjectExWriterInit     (NSBinPptxRW::CXmlWriter& oXmlWriter, int lDocType);
        void SaveObjectExWriterRelease  (NSBinPptxRW::CXmlWriter& oXmlWriter);

		PPTX::Logic::SpTreeElem ObjectFromXml(const std::wstring& sXml, std::wstring** pMainProps);
		std::wstring ObjectToVML		(const std::wstring& sXml);
		std::wstring ObjectToDrawingML	(const std::wstring& sXml, int nDocType);

        std::wstring SaveObjectBackground(LONG lStart, LONG lLength);

        HRESULT GetRecordBinary     (long lRecordType, const std::wstring& sXml);
        HRESULT GetRecordXml        (long lStart, long lLength, long lRecType, int lDocType, std::wstring & sXml);

        void SetDstContentRels   ();
        void SaveDstContentRels  (const std::wstring& sRelsPath);
       
		HRESULT LoadClrMap          (const std::wstring& sXml);

        HRESULT(SetFontDir)         (const std::wstring& sFontDir);

        HRESULT SetFontPicker       (COfficeFontPicker* pFontPicker);

        HRESULT SetAdditionalParam(const std::wstring& ParamName, BYTE *pArray, size_t szCount);
        HRESULT GetAdditionalParam(const std::wstring& ParamName, BYTE **pArray, size_t& szCount);

		void WriteRels				(const std::wstring& sType, const std::wstring& sTarget, const std::wstring& sTargetMode, unsigned int* lId);
		void Registration			(const std::wstring& sType, const std::wstring& oDirectory, const std::wstring& oFilename);

        void SetFontManager         (NSFonts::IFontManager* pFontManager);

        OOX::CContentTypes* GetContentTypes();

	protected:
		nullable<PPTX::Logic::Xfrm> m_oxfrm_override;

        bool ParceObject        (const std::wstring& strXml, std::wstring** pMainProps);
        void SendMainProps      (const std::wstring& strMainProps, std::wstring**& pMainProps);

		void ConvertDiagram		(PPTX::Logic::SpTreeElem *result, XmlUtils::CXmlNode& oNode, std::wstring**& pMainProps, bool bIsTop = true);
		void ConvertShape		(PPTX::Logic::SpTreeElem *result, XmlUtils::CXmlNode& oNode, std::wstring**& pMainProps, bool bIsTop = true);
		void ConvertGroup		(PPTX::Logic::SpTreeElem *result, XmlUtils::CXmlNode& oNode, std::wstring**& pMainProps, bool bIsTop = true);
		void ConvertDrawing		(PPTX::Logic::SpTreeElem *result, XmlUtils::CXmlNode& oNode, std::wstring**& pMainProps, bool bIsTop = true);
		void ConvertWordArtShape(PPTX::Logic::SpTreeElem* result, XmlUtils::CXmlNode& oNode, CPPTShape* pPPTShape);

        std::wstring GetVMLShapeXml      (CPPTShape* pPPTShape);
        std::wstring GetVMLShapeXml      (PPTX::Logic::SpTreeElem& oElem);

        void CheckBrushShape        (PPTX::Logic::SpTreeElem* oElem, XmlUtils::CXmlNode& oNode, CPPTShape* pPPTShape);
        void CheckPenShape          (PPTX::Logic::SpTreeElem* oElem, XmlUtils::CXmlNode& oNode, CPPTShape* pPPTShape);
		void CheckBorderShape		(PPTX::Logic::SpTreeElem* oElem, XmlUtils::CXmlNode& oNode, CPPTShape* pPPTShape);

        void LoadCoordSize			(XmlUtils::CXmlNode& oNode, ::CShapePtr pShape);
		void LoadCoordPos			(XmlUtils::CXmlNode& oNode, ::CShapePtr pShape);
       
		std::wstring GetDrawingMainProps (XmlUtils::CXmlNode& oNode, PPTX::CCSS& oCssStyles, CSpTreeElemProps& oProps);

        void ConvertMainPropsToVML  (const std::wstring& sMainProps, NSBinPptxRW::CXmlWriter& oWriter, PPTX::Logic::SpTreeElem& oElem);
        void ConvertPicVML          (PPTX::Logic::SpTreeElem& oElem, const std::wstring& sMainProps, NSBinPptxRW::CXmlWriter& oWriter);
        void ConvertShapeVML        (PPTX::Logic::SpTreeElem& oShape, const std::wstring& sMainProps, NSBinPptxRW::CXmlWriter& oWriter, bool bSignature = false);
        void ConvertGroupVML        (PPTX::Logic::SpTreeElem& oGroup, const std::wstring& sMainProps, NSBinPptxRW::CXmlWriter& oWriter);

        void ConvertTextVML         (XmlUtils::CXmlNode &nodeTextBox, PPTX::Logic::Shape* pShape);

        void    Clear();
		HRESULT SetCurrentRelsPath();
	};
}
#endif //OOX_IFILE_CONTAINER_INCLUDE_H_
