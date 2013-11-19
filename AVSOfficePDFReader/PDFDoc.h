#ifndef _PDF_DOC_H
#define _PDF_DOC_H

#include <stdio.h>
#include "XRef.h"
#include "Catalog.h"
#include "Page.h"

class StringExt;
class BaseStream;
class OutputDev;
class Links;
class LinkAction;
class LinkDest;
class Outline;
//-------------------------------------------------------------------------------------------------------------------------------
// PDFDoc
//-------------------------------------------------------------------------------------------------------------------------------

class PDFDoc 
{
public:

	PDFDoc(GlobalParams *pGlobalParams, CString wsFileName, StringExt *seOwnerPassword = NULL, StringExt *seUserPassword = NULL);
	PDFDoc(GlobalParams *pGlobalParams, wchar_t *sFileName, int nFileNameLen, StringExt *seOwnerPassword = NULL, StringExt *seUserPassword = NULL);
	PDFDoc(GlobalParams *pGlobalParams, BaseStream *pStream, StringExt *seOwnerPassword = NULL, StringExt *seUserPassword = NULL);
	~PDFDoc();

	// Нормально ли открылся PDF файл?
	BOOL CheckValidation() 
	{ 
		return m_bValid; 
	}


	// Получаем код ошибки, если PDF файл не открылся.
	int GetErrorCode() 
	{ 
		return m_nErrorCode; 
	}


	// Считываем имя файла.
	CString GetFileName() 
	{ 
		return m_wsFileName; 
	}


	// Считываем таблицу Xref.
	XRef *GetXRef() 
	{ 
		return m_pXref; 
	}


	// Считываем объект Сatalog.
	Catalog *GetCatalog() 
	{ 
		return m_pCatalog; 
	}


	// Считываем поток.
	BaseStream *GetBaseStream() 
	{ 
		return m_pStream; 
	}


	// Cчитываем параметры страницы.
	double GetPageMediaWidth (int nPageIndex)
    { 
		return m_pCatalog->GetPage(nPageIndex)->GetMediaWidth(); 
	}
	double GetPageMediaHeight(int nPageIndex)
    { 
		return m_pCatalog->GetPage(nPageIndex)->GetMediaHeight(); 
	}
	double GetPageCropWidth  (int nPageIndex)
    { 
		return m_pCatalog->GetPage(nPageIndex)->GetCropWidth(); 
	}
	double GetPageCropHeight (int nPageIndex)
    { 
		return m_pCatalog->GetPage(nPageIndex)->GetCropHeight(); 
	}
	int    GetPageRotate     (int nPageIndex)
    { 
		return m_pCatalog->GetPage(nPageIndex)->GetRotate(); 
	}


	// Считываем количество страниц.
	int GetPagesCount() 
	{ 
		return m_pCatalog->GetPagesCount(); 
	}


	// Возвращаем содержимое потока метаданных объекта Catalog.
	StringExt *ReadMetadata() 
	{ 
		return m_pCatalog->ReadMetadata(); 
	}


	// Возвращаем объект StructureTreeRoot.
	Object *GetStructTreeRoot() 
	{ 
		return m_pCatalog->GetStructTreeRoot(); 
	}


	// Выводим данную страницу на устройство pOut.
	void DisplayPage(OutputDev *pOut, int nPageIndex, double dHorDPI, double dVerDPI, int nRotate, BOOL bUseMediaBox, BOOL bCrop, BOOL bPrinting, BOOL (*pAbortCheckCbk)(void *pData) = NULL, void *pAbortCheckCbkData = NULL);
	// Выводим сразу несколько страниц.
	void DisplayPages(OutputDev *pOut, int nFirstPageIndex, int nLastPageIndex, double dHorDPI, double dVerDPI, int nRotate, BOOL bUseMediaBox, BOOL bCrop, BOOL bPrinting, BOOL (*pAbortCheckCbk)(void *pData) = NULL, void *pAbortCheckCbkData = NULL);
	// Выводим часть страницы.
	void DisplayPageSlice(OutputDev *pOut, int nPageIndex, double dHorDPI, double dVerDPI, int nRotate, BOOL bUseMediaBox, BOOL bCrop, BOOL bPrinting, int nSliceX, int nSliceY, int nSliceW, int nSliceH, BOOL (*pAbortCheckCbk)(void *pData) = NULL, void *pAbortCheckCbkData = NULL);
	// Ищем страницу по ее объектным номерам. Возвращаем 0, если страница не найдена.
	int FindPage(int nNum, int nGen) 
	{ 
		return m_pCatalog->FindPage( nNum, nGen ); 
	}


	// Возвращаем Links для текущей страницы.
	Links *GetLinks(int nPageIndex);
	// Ищем Destination по имени. Возвращаем Link destination или NULL, если <seName> не объект Destination.
	LinkDestination *FindDest(StringExt *seName)
	{ 
		return m_pCatalog->FindDest( seName ); 
	}


	// Обрабатываем Links для данной страницы.
	void ProcessLinks(OutputDev *pOut, int nPageIndex);
	// Возвращаем объект Оutline.
	Outline *GetOutline() 
	{
		return m_pOutline; 
	}
	// Зашифрован ли файл?
	BOOL IsEncrypted() 
	{ 
		return m_pXref->CheckEncrypted(); 
	}

	// Проверяем различные ограничения.
	BOOL CheckPrint   (BOOL bIgnoreOwnerPassword = FALSE)
    { 
		return m_pXref->CheckPrint( bIgnoreOwnerPassword ); 
	}
	BOOL CheckChange  (BOOL bIgnoreOwnerPassword = FALSE)
    { 
		return m_pXref->CheckChange( bIgnoreOwnerPassword ); 
	}
	BOOL CheckCopy    (BOOL bIgnoreOwnerPassword = FALSE)
    { 
		return m_pXref->CheckCopy( bIgnoreOwnerPassword ); 
	}
	BOOL CheckAddNotes(BOOL bIgnoreOwnerPassword = FALSE)
    { 
		return m_pXref->CheckAddNotes( bIgnoreOwnerPassword ); 
	}

	// Данный PDF файл является линеаризированным (см. Linearized PDF)
	BOOL IsLinearized();
	// Воозвращаем объек Info.
	Object *GetDocInfo(Object *pObject) 
	{ 
		return m_pXref->GetDocInfo( pObject ); 
	}
	Object *GetDocInfoCopy(Object *pObject) 
	{ 
		return m_pXref->GetDocInfoCopy( pObject ); 
	}

	// Возвращаем версию PDF файла.
	double GetPDFVersion() 
	{ 
		return m_dPDFVersion; 
	}

	// Сохраняем копию данного файла.
	BOOL SaveAs(CStringW wsName);



private:

	BOOL Setup(StringExt *seOwnerPassword, StringExt *seUserPassword);
	void CheckHeader();
	BOOL CheckEncryption(StringExt *seOwnerPassword, StringExt *seUserPassword);

private:

	CString     m_wsFileName;  // Имя исходного файла.
	FILE       *m_pFile;       // Указатель на файловый поток
	char       *m_pFileBuffer;
	HANDLE      m_hFile;
	HANDLE      m_hMapFile;
	BaseStream *m_pStream;     // Поток, в который считан весь файл
	double      m_dPDFVersion; // Верисия PDF файла (1.1 - 1.7)
	XRef       *m_pXref;       // Таблица Xref 
	Catalog    *m_pCatalog;    // Указатель на объект Catalog
	Outline    *m_pOutline;    // Указатель на Outline (меню)

	BOOL        m_bValid;      // Нормально ли открылся PDF файл
	int         m_nErrorCode;  // Код ошибки

	GlobalParams *m_pGlobalParams;
};

#endif /* _PDF_DOC_H */
