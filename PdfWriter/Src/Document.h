#ifndef _PDF_WRITER_SRC_DOCUMENT_H
#define _PDF_WRITER_SRC_DOCUMENT_H

#include <vector>
#include <string>
#include "Types.h"

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
	class CStream;
	class CDestination;
	class CExtGrState;
	class CAnnotation;
	class CImageDict;
	class CFontDict;
	class CFont14;
	class CFontCidTrueType;
	class CJbig2Global;
	class CShading;
	class CImageTilePattern;
	//----------------------------------------------------------------------------------------
	// CDocument
	//----------------------------------------------------------------------------------------
	class CDocument
	{
	public:

		CDocument();
		~CDocument();

		bool              CreateNew();
		void              Close();
		bool              SaveToFile(const std::wstring& wsPath);
			              
		void              SetEncryptionMode(EEncryptMode eMode, unsigned int unKeyLen = 0);
		void              SetPassword(const char* sOwnerPassword, const char* sUserPassword);
		void              SetPermission(unsigned int unPermission);
		void              SetCompressionMode(unsigned int unMode);
			              
		CPage*            AddPage();
		void              AddPageLabel(EPageNumStyle eStyle, unsigned int unFirstPage, const char* sPrefix);
		void              AddPageLabel(unsigned int unPageIndex, EPageNumStyle eStyle, unsigned int unFirstPage, const char* sPrefix);
		COutline*         CreateOutline(COutline* pParent, const char* sTitle);
		CDestination*     CreateDestination(unsigned int unPageIndex);
					      
		CExtGrState*      GetExtGState(double dAlphaStroke = -1, double dAlphaFill = -1, EBlendMode eMode = blendmode_Unknown, int nStrokeAdjustment = -1);
		CJbig2Global*     GetJbig2Global();
					      
		CAnnotation*      CreateTextAnnot(unsigned int unPageNum, TRect oRect, const char* sText);
		CAnnotation*      CreateLinkAnnot(unsigned int unPageNum, TRect oRect, CDestination* pDest);
		CAnnotation*      CreateUriLinkAnnot(unsigned int unPageNum, TRect oRect, const char* sUri);
					      
		CImageDict*       CreateImage();
		CFont14*          CreateFont14(EStandard14Fonts eType);
		CFontCidTrueType* CreateTrueTypeFont(const std::wstring& wsFontPath, unsigned int unIndex);

		CShading*         CreateAxialShading(double dX0, double dY0, double dX1, double dY1, unsigned char* pColors, double* pPoints, int nCount);
		CShading*         CreateRaidalShading(double dX0, double dY0, double dR0, double dX1, double dY1, double dR1, unsigned char* pColors, double* pPoints, int nCount);
		CImageTilePattern*CreateImageTilePattern(double dW, double dH, CImageDict* pImageDict, EImageTilePatternType eType = imagetilepatterntype_Default);
		CImageTilePattern*CreateHatchPattern(double dW, double dH, const BYTE& nR1, const BYTE& nG1, const BYTE& nB1, const BYTE& nAlpha1, const BYTE& nR2, const BYTE& nG2, const BYTE& nB2, const BYTE& nAlpha2, const std::wstring& wsHatch);
					  
	private:		  
					  
		char*             GetTTFontTag();
		CExtGrState*      FindExtGrState(double dAlphaStroke = -1, double dAlphaFill = -1, EBlendMode eMode = blendmode_Unknown, int nStrokeAdjustment = -1);
		void              SaveToStream(CStream* pStream);
		void              PrepareEncryption();
		CDictObject*      CreatePageLabel(EPageNumStyle eStyle, unsigned int unFirstPage, const char* sPrefix);

	private:

		struct TFontInfo
		{
			TFontInfo(const std::wstring& path, const unsigned int& index, CFontCidTrueType* font)
			{
				wsPath  = path;
				unIndex = index;
				pFont   = font;
			}

			std::wstring      wsPath;
			unsigned int      unIndex;
			CFontCidTrueType* pFont;
		};

		CCatalog*                 m_pCatalog;
		COutline*                 m_pOutlines;
		CXref*                    m_pXref;
		CPageTree*                m_pPageTree;
		CPage*                    m_pCurPage;
		int                       m_nCurPageNum;
		CInfoDict*                m_pInfo;
		CDictObject*              m_pTrailer;
		CDictObject*              m_pResources;
		bool                      m_bEncrypt;
		CEncryptDict*             m_pEncryptDict;
		unsigned int              m_unCompressMode;
		std::vector<CPage*>       m_vPages;
		std::vector<CExtGrState*> m_vExtGrStates;
		char                      m_sTTFontTag[8]; // 6 символов + '+' + 0x00 ("BAAAAA+/0")
		CJbig2Global*             m_pJbig2;
		std::vector<CShading*>    m_vShadings;
		std::vector<TFontInfo>    m_vTTFonts;

		friend class CFontCidTrueType;
	};
}

#endif // _PDF_WRITER_SRC_DOCUMENT_H

