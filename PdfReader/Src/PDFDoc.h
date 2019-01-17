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
#ifndef _PDF_READER_PDF_DOC_H
#define _PDF_READER_PDF_DOC_H

#include <stdio.h>
#include "XRef.h"
#include "Catalog.h"
#include "Page.h"
#include "ErrorConstants.h"

namespace PdfReader
{
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

		PDFDoc(GlobalParams *pGlobalParams, const std::wstring &wsFileName, const std::wstring &ownerPassword, const std::wstring &userPassword);
		~PDFDoc();

		// Нормально ли открылся PDF файл?
		bool CheckValidation()
		{
			return m_bValid;
		}

		// Получаем код ошибки, если PDF файл не открылся.
		EError GetErrorCode()
		{
			return m_eErrorCode;
		}


		// Считываем имя файла.
		std::wstring GetFileName()
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
		double GetPageMediaWidth(int nPageIndex)
		{
			return m_pCatalog->GetPage(nPageIndex)->GetMediaWidth();
		}
		double GetPageMediaHeight(int nPageIndex)
		{
			return m_pCatalog->GetPage(nPageIndex)->GetMediaHeight();
		}
		double GetPageCropWidth(int nPageIndex)
		{
			return m_pCatalog->GetPage(nPageIndex)->GetCropWidth();
		}
		double GetPageCropHeight(int nPageIndex)
		{
			return m_pCatalog->GetPage(nPageIndex)->GetCropHeight();
		}
		int    GetPageRotate(int nPageIndex)
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
		void DisplayPage(OutputDev *pOut, int nPageIndex, double dHorDPI, double dVerDPI, int nRotate, bool bUseMediaBox, bool bCrop, bool bPrinting, bool(*pAbortCheckCbk)(void *pData) = NULL, void *pAbortCheckCbkData = NULL);
		// Выводим сразу несколько страниц.
		void DisplayPages(OutputDev *pOut, int nFirstPageIndex, int nLastPageIndex, double dHorDPI, double dVerDPI, int nRotate, bool bUseMediaBox, bool bCrop, bool bPrinting, bool(*pAbortCheckCbk)(void *pData) = NULL, void *pAbortCheckCbkData = NULL);
		// Выводим часть страницы.
		void DisplayPageSlice(OutputDev *pOut, int nPageIndex, double dHorDPI, double dVerDPI, int nRotate, bool bUseMediaBox, bool bCrop, bool bPrinting, int nSliceX, int nSliceY, int nSliceW, int nSliceH, bool(*pAbortCheckCbk)(void *pData) = NULL, void *pAbortCheckCbkData = NULL);
		// Ищем страницу по ее объектным номерам. Возвращаем 0, если страница не найдена.
		int FindPage(int nNum, int nGen)
		{
			return m_pCatalog->FindPage(nNum, nGen);
		}


		// Возвращаем Links для текущей страницы.
		Links *GetLinks(int nPageIndex);
		// Ищем Destination по имени. Возвращаем Link destination или NULL, если <seName> не объект Destination.
		LinkDestination *FindDest(StringExt *seName)
		{
			return m_pCatalog->FindDest(seName);
		}


		// Обрабатываем Links для данной страницы.
		void ProcessLinks(OutputDev *pOut, int nPageIndex);
		// Возвращаем объект Оutline.
		Outline *GetOutline()
		{
			return m_pOutline;
		}
		// Зашифрован ли файл?
		bool IsEncrypted()
		{
			return m_pXref->CheckEncrypted();
		}

		// Проверяем различные ограничения.
		bool CheckPrint(bool bIgnoreOwnerPassword = false)
		{
			return m_pXref->CheckPrint(bIgnoreOwnerPassword);
		}
		bool CheckChange(bool bIgnoreOwnerPassword = false)
		{
			return m_pXref->CheckChange(bIgnoreOwnerPassword);
		}
		bool CheckCopy(bool bIgnoreOwnerPassword = false)
		{
			return m_pXref->CheckCopy(bIgnoreOwnerPassword);
		}
		bool CheckAddNotes(bool bIgnoreOwnerPassword = false)
		{
			return m_pXref->CheckAddNotes(bIgnoreOwnerPassword);
		}

		// Данный PDF файл является линеаризированным (см. Linearized PDF)
		bool IsLinearized();
		// Воозвращаем объек Info.
		Object *GetDocInfo(Object *pObject)
		{
			return m_pXref->GetDocInfo(pObject);
		}
		Object *GetDocInfoCopy(Object *pObject)
		{
			return m_pXref->GetDocInfoCopy(pObject);
		}

		// Возвращаем версию PDF файла.
		double GetPDFVersion()
		{
			return m_dPDFVersion;
		}

	private:

		bool Setup(const std::wstring &ownerPassword, const std::wstring &userPasswordd);
		void CheckHeader();
		bool CheckEncryption(const std::wstring &ownerPassword, const std::wstring &userPassword);

		std::wstring   m_wsFileName;  // Имя исходного файла.
		FILE*          m_pFile;       // Указатель на файловый поток
		unsigned char* m_pFileBuffer; // Буфер для файла

		BaseStream*    m_pStream;     // Поток, в который считан весь файл
		double         m_dPDFVersion; // Верисия PDF файла (1.1 - 1.7)
		XRef*          m_pXref;       // Таблица Xref 
		Catalog*       m_pCatalog;    // Указатель на объект Catalog
		Outline*       m_pOutline;    // Указатель на Outline (меню)
					   
		bool           m_bValid;      // Нормально ли открылся PDF файл
		EError         m_eErrorCode;  // Код ошибки
					   
		GlobalParams*  m_pGlobalParams;
	};
}
#endif // _PDF_READER_PDF_DOC_H
