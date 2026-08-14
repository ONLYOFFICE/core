/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#ifndef _PDF_WRITER_SRC_DOCUMENT_H
#define _PDF_WRITER_SRC_DOCUMENT_H

#include <vector>
#include <deque>
#include <string>
#include "Types.h"

#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/xmlsec/src/include/Certificate.h"

#include <ft2build.h>
#include FT_OUTLINE_H
#include FT_SIZES_H
#include FT_GLYPH_H
#include FT_TRUETYPE_IDS_H
#include FT_TRUETYPE_TABLES_H
#include FT_XFREE86_H
#include FT_ADVANCES_H

#ifdef CreateFont
#undef CreateFont
#endif

class CFontManager;

namespace PdfWriter
{
	class CCatalog;
	class COutline;
	class CXref;
	class CPageTree;
	class CPage;
	class CInfoDict;
	class CDictObject;
	class CEncryptDict;
	class CSignatureDict;
	class CStream;
	class CDestination;
	class CExtGrState;
	class CAnnotation;
	class CAction;
	class CImageDict;
	class CFontDict;
	class CFont14;
	class CFontEmbedded;
	class CFontCidTrueType;
	class CFontTrueType;
	class CJbig2Global;
	class CShading;
	class CImageTilePattern;
	class CPattern;
	class CAcroForm;
	class CTextField;
	class CResourcesDict;
	class CChoiceField;
	class CCheckBoxField;
	class CRadioGroupField;
	class CPictureField;
	class CSignatureField;
	class CDateTimeField;
	class CFieldBase;
	class CStreamData;
	class CXObject;
	class CObjectBase;
	class CStringObject;
	class CStructureTreeRoot;
	//----------------------------------------------------------------------------------------
	// CDocument
	//----------------------------------------------------------------------------------------
	class CDocument
	{
	public:

		CDocument();
		~CDocument();

		bool              CreateNew(bool bMetadata = true);
		void              Close();
		bool              SaveToFile(const std::wstring& wsPath);
		bool              SaveToMemory(BYTE** pData, int* pLength, bool bForWasm);
		bool              SaveNewWithPassword(CXref* pXref, CXref* _pXref, const std::wstring& wsPath, const std::wstring& wsOwnerPassword, const std::wstring& wsUserPassword, CDictObject* pTrailer);
			              
        void              SetPasswords(const std::wstring & wsOwnerPassword, const std::wstring & wsUserPassword);
		void              SetPermission(unsigned int unPermission);
		void              SetCompressionMode(unsigned int unMode);
		unsigned int      GetCompressionMode() const;
		void			  SetDocumentID(const std::wstring & wsDocumentID);
        void              SetTitle   (const std::string& sTitle);
        void              SetAuthor  (const std::string& sAuthor);
        void              SetSubject (const std::string& sSubject);
        void              SetKeywords(const std::string& sKeywords);

		void              SetRedactInfo(int nFlag);
		void              SetPDFAConformanceMode(bool isPDFA);
		void              SetPDFUAConformanceMode(bool isPDFUA);
		bool              IsPDFA()  const;
		bool              IsPDFUA() const;

		CPage*            AddPage();
		CPage*            GetPage    (const unsigned int& unPage);
		CObjectBase*      GetPageObj (const unsigned int& unPage);
		CPage*            GetEditPage(const unsigned int& unPage);
		int               FindPage   (CPage* pPage);
		unsigned int      GetPagesCount() const;
		void              AddPageLabel(EPageNumStyle eStyle, unsigned int unFirstPage, const char* sPrefix);
		void              AddPageLabel(unsigned int unPageIndex, EPageNumStyle eStyle, unsigned int unFirstPage, const char* sPrefix);
		COutline*         CreateOutline(COutline* pParent, const char* sTitle);
		COutline*         GetOutlines() { return m_pOutlines; }
		CDestination*     CreateDestination(CObjectBase* pPage, bool bInline = false);
		bool              AddMetaData(const std::wstring& sMetaName, BYTE* pMetaData, DWORD nMetaLength);

		void              AddExtGState(CExtGrState* pState);
		CExtGrState*      GetExtGState(double dAlphaStroke = -1, double dAlphaFill = -1, EBlendMode eMode = blendmode_Unknown, int nStrokeAdjustment = -1);
		CExtGrState*      GetStrokeAlpha(double dAlpha);
		CExtGrState*      GetFillAlpha(double dAlpha);
		CJbig2Global*     GetJbig2Global();

		CAnnotation*      CreateWidget(BYTE nType);
		CAnnotation*      CreateAnnot(BYTE nType);
		CAnnotation*      CreateLinkAnnot(const TRect& oRect, CDestination* pDest);
		CAnnotation*      CreateUriLinkAnnot(const TRect& oRect, const char* sUrl);
		void              AddAnnotation(const int& nID, CAnnotation* pAnnot);
		CAction*          CreateAction(BYTE nType);
					      
		CImageDict*       CreateImage();
		CXObject*         CreateForm(CImageDict* pImage, const std::string& sName);
		CXObject*         CreateForm();
		CFont14*          CreateFont14(const std::wstring& wsFontPath, unsigned int unIndex, EStandard14Fonts eType);
		CFont14*          FindFont14  (const std::wstring& wsFontPath, unsigned int unIndex);
		CFontEmbedded*    CreateFontEmbedded(const std::wstring& wsFontPath, unsigned int unIndex, const std::string& sFontKey, EFontType nType, CObjectBase* pObj,
											 const std::map<unsigned int, unsigned int>& mCodeToWidth, const std::map<unsigned int, unsigned int>& mCodeToUnicode, const std::map<unsigned int, unsigned int>& mCodeToGID);
		CFontEmbedded*    FindFontEmbedded  (const std::wstring& wsFontPath, unsigned int unIndex);
		CFontCidTrueType* CreateCidTrueTypeFont(const std::wstring& wsFontPath, unsigned int unIndex);
		CFontCidTrueType* FindCidTrueTypeFont(const std::wstring& wsFontPath, unsigned int unIndex);
		CFontTrueType*    CreateTrueTypeFont(const std::wstring& wsFontPath, unsigned int unIndex);
		CFontTrueType*    CreateTrueTypeFont(CFontCidTrueType* pCidFont);
		CFont14*          GetDefaultCheckboxFont();

		CImageTilePattern*CreateImageTilePattern(double dW, double dH, CImageDict* pImageDict, CMatrix* pMatrix = NULL, EImageTilePatternType eType = imagetilepatterntype_Default, double dXStepSpacing = 0, double dYStepSpacing = 0);
		CImageTilePattern*CreateHatchPattern(double dW, double dH, const BYTE& nR1, const BYTE& nG1, const BYTE& nB1, const BYTE& nAlpha1, const BYTE& nR2, const BYTE& nG2, const BYTE& nB2, const BYTE& nAlpha2, const std::wstring& wsHatch);
		CShading*         CreateAxialShading(CPage* pPage, double dX0, double dY0, double dX1, double dY1, unsigned char* pColors, unsigned char* pAlphas, double* pPoints, int nCount, CExtGrState*& pExtGrState);
		CShading*         CreateRadialShading(CPage* pPage, double dX0, double dY0, double dR0, double dX1, double dY1, double dR1, unsigned char* pColors, unsigned char* pAlphas, double* pPoints, int nCount, CExtGrState*& pExtGrState);

		CResourcesDict*   GetFieldsResources();
		CTextField*       CreateTextField();	
		CChoiceField*     CreateChoiceField();
		CCheckBoxField*   CreateCheckBoxField();
		CRadioGroupField* GetRadioGroupField(const std::wstring& wsGroupName);
		CPictureField*    CreatePictureField();
		CSignatureField*  CreateSignatureField();
		CDateTimeField*   CreateDateTimeField();
		void              AddWidget(CFieldBase* pWidget);
		bool              CheckFieldName(CFieldBase* pField, const std::string& sName);

		bool              HasImage(const std::wstring& wsImagePath, BYTE nAlpha);
		CImageDict*       GetImage(const std::wstring& wsImagePath, BYTE nAlpha);
		void              AddImage(const std::wstring& wsImagePath, BYTE nAlpha, CImageDict* pImage);
		CImageDict*       GetCurImage() { return m_pCurImage; }
		void              SetCurImage(CImageDict* pImage) { m_pCurImage = pImage; }
					  
		bool              CreatePageTree(CXref* pXref, CPageTree* pPageTree);
		bool              EditPdf(int nPosLastXRef, int nSizeXRef, CXref* pXref, CCatalog* pCatalog, CEncryptDict* pEncrypt, int nFormField);
		bool              EditResources(CXref* pXref, CResourcesDict* pResources);
		std::pair<int, int> GetPageRef(int nPageIndex);
		bool              EditPage(CXref* pXref, CPage* pPage, int nPageIndex);
		void              FixEditPage(CPage* pPage, int nPageIndex = 0);
		void              AddEditPage(CPage* pPage, int nPageIndex);
		CPage*            AddPage(int nPageIndex, CPage* _pNewPage = NULL);
		bool              DeletePage(int nPageIndex);
		bool              AddToFile(const std::wstring& wsPath, CXref* pXref, CDictObject* pTrailer, CXref* pInfoXref, CInfoDict* pInfo);
		void              AddObject(CObjectBase* pObj);
		bool              MovePage(int nPageIndex, int nPos);
		void              Sign(const TRect& oRect, CImageDict* pImage, const std::wstring &wsReason, const std::wstring &wsContact, const std::wstring &wsName, const std::wstring &wsLocation);
		bool              PrepareSignature(const std::wstring& wsPath);
		bool              FinalizeSignature(BYTE* pSignedData, DWORD dwDataLength);
		bool              EditAnnot (CXref* pXref, CAnnotation* pAnnot,  int nID);
		void              AddParent(int nID, CDictObject* pParent);
		CDictObject*      CreateParent(int nID);
		bool              EditParent(CXref* pXref, CDictObject* pParent, int nID);
		bool              DeleteAnnot(int nObjNum, int nObjGen);
		CAnnotation*      GetAnnot(int nID);
		CDictObject*      GetParent(int nID);
		CPage*            GetCurPage() { return m_pCurPage; }
		void              SetCurPage(CPage* pPage) { m_pCurPage = pPage; }
		bool              EditCO(const std::vector< std::pair<int, int> >& arrCO);
		void              AddOOField(const std::string& sName, BYTE* pMetaData, DWORD nMetaLength);
		std::string       SetParentKids(int nParentID);
		const std::map<int, CAnnotation*>& GetAnnots() { return m_mAnnotations; }
		const std::map<int, CDictObject*>& GetParents() { return m_mParents; }
		CPageTree*        GetPageTree() { return m_pPageTree; }
		CEncryptDict*     GetEncrypt() { return m_pEncryptDict; }
		CCatalog*         GetCatalog() { return m_pCatalog; }
		CStructureTreeRoot* GetStructureTreeRoot();
		void              AddShapeXML(const std::string& sXML);
		void              EndShapeXML();
		void              ClearPage();
		void              ClearPageFull();
		bool              EditXref(CXref* pXref);
		void              SetAcroForm(CDictObject* pObj);
		CDictObject*      GetAcroForm() { return m_pAcroForm; }
		CResourcesDict*   CreateResourcesDict(bool bInline, bool bProcSet);
		void              RemoveObj(CObjectBase* pObj);
		void              SetEncryption(CEncryptDict* pEncrypt, CObjectBase* pID);
		void              AddNameTree(CStringObject* pName, CDestination* pDest);
		CObjectBase*      FindObjByID(unsigned int nObjectId);
	private:		  
					  
		char*             GetTTFontTag();
		void              AddFreeTypeFont(CFontCidTrueType* pFont);
        FT_Library        GetFreeTypeLibrary();
		CExtGrState*      FindExtGrState(double dAlphaStroke = -1, double dAlphaFill = -1, EBlendMode eMode = blendmode_Unknown, int nStrokeAdjustment = -1);
		void              SaveToStream(CStream* pStream, const std::string& sAltTitle = "");
		void              PrepareEncryption();
		CDictObject*      CreatePageLabel(EPageNumStyle eStyle, unsigned int unFirstPage, const char* sPrefix);
		CShading*         CreateShading(CPage* pPage, double *pPattern, bool bAxial, unsigned char* pColors, unsigned char* pAlphas, double* pPoints, int nCount, CExtGrState*& pExtGrState);
		CShading*         CreateAxialShading(double dX0, double dY0, double dX1, double dY1, unsigned char* pColors, double* pPoints, int nCount);
		CShading*         CreateRadialShading(double dX0, double dY0, double dR0, double dX1, double dY1, double dR1, unsigned char* pColors, double* pPoints, int nCount);
		bool              CheckAcroForm();
		CRadioGroupField* FindRadioGroupField(const std::wstring& wsGroupName);
		void              Sign(const std::wstring& wsPath, unsigned int nSizeXRef, bool bNeedStreamXRef = false, unsigned int nPrevAddr = 0);

	private:

		struct TFontInfo
		{
			TFontInfo(const std::wstring& path, const unsigned int& index, CFontDict* font)
			{
				wsPath  = path;
				unIndex = index;
				pFont   = font;
			}

			std::wstring wsPath;
			unsigned int unIndex;
			CFontDict*   pFont;
		};
		struct TSignatureInfo
		{
			TSignatureInfo(const TRect& _oRect, CPage* _pPage, CImageDict* _pImage,
						   const std::wstring& _wsReason, const std::wstring& _wsContact, const std::wstring & _wsName, const std::wstring &_wsLocation)
			{
				oRect  = _oRect;
				pPage  = _pPage;
				pImage = _pImage;

				wsReason = _wsReason;
				wsContact = _wsContact;
				wsName = _wsName;
				wsLocation = _wsLocation;

				nSizeXRef = 0;
				nPrevAddr = 0;
				nFileSizeBefore = 0;
				bNeedStreamXRef = false;
				pField = NULL;
				pXref = NULL;
			}

			TRect oRect;
			CPage* pPage;
			CImageDict* pImage;

			std::wstring wsReason;
			std::wstring wsContact;
			std::wstring wsName;
			std::wstring wsLocation;

			std::wstring wsPath;
			unsigned int nSizeXRef;
			unsigned int nPrevAddr;
			unsigned int nFileSizeBefore;
			bool bNeedStreamXRef;
			CSignatureField* pField;
			CXref* pXref;
		};
		struct TImageInfo
		{
			TImageInfo(const std::wstring& _wsImagePath, BYTE _nAlpha, CImageDict* _pImage)
			{
				wsImagePath = _wsImagePath;
				nAlpha = _nAlpha;
				pImage = _pImage;
			}

			std::wstring wsImagePath;
			BYTE nAlpha;
			CImageDict* pImage;
		};

		CCatalog*                          m_pCatalog;
		COutline*                          m_pOutlines;
		CXref*                             m_pXref;
		CXref*                             m_pLastXref;
		CPageTree*                         m_pPageTree;
		CPage*                             m_pCurPage;
		int                                m_nCurPageNum;
		CImageDict*                        m_pCurImage;
		CInfoDict*                         m_pInfo;
		CDictObject*                       m_pTrailer;
		CDictObject*                       m_pResources;
		CStreamData*                       m_pMetaData;
		bool                               m_bEncrypt;
		CEncryptDict*                      m_pEncryptDict;
		std::deque<TSignatureInfo*>        m_vSignatures;
		std::vector<TImageInfo>            m_vImages;
		unsigned int                       m_unFormFields;
		unsigned int                       m_unCompressMode;
		std::vector<CExtGrState*>          m_vExtGrStates;
		std::vector<CExtGrState*>          m_vStrokeAlpha;
		std::vector<CExtGrState*>          m_vFillAlpha;
		char                               m_sTTFontTag[8]; // 6 characters + '+' + 0x00 ("BAAAAA+/0")
		CJbig2Global*                      m_pJbig2;
		std::vector<CShading*>             m_vShadings;
		std::vector<TFontInfo>             m_vCidTTFonts;
		std::vector<TFontInfo>             m_vTTFonts;
		std::vector<TFontInfo>             m_vFonts14;
		std::vector<TFontInfo>             m_vFontsEmbedded;
		CFont14*                           m_pDefaultCheckBoxFont;
		CDictObject*                       m_pTransparencyGroup;
		std::vector<CFontCidTrueType*>     m_vFreeTypeFonts;
		FT_Library                         m_pFreeTypeLibrary;
		bool                               m_bPDFAConformance;
		bool                               m_bPDFUAConformance;
		int                                m_nRedactInfo;
		std::wstring                       m_wsDocumentID;
		CDictObject*                       m_pAcroForm;
		CResourcesDict*                    m_pFieldsResources;
		std::vector<CRadioGroupField*>     m_vRadioGroups;
		std::vector<CDictObject*>          m_vMetaOForms;
		std::map<std::string, CFieldBase*> m_mFields;
		std::map<int, CAnnotation*>        m_mAnnotations;
		std::map<int, CDictObject*>        m_mParents;
		std::map<int, CPage*>              m_mEditPages;

		friend class CFontCidTrueType;
		friend class CFontTrueType;
	};
}

#endif // _PDF_WRITER_SRC_DOCUMENT_H

