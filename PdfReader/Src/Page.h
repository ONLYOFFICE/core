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
#ifndef _PDF_READER_PAGE_H
#define _PDF_READER_PAGE_H

#include "Object.h"
#include "GlobalParams.h"

namespace PdfReader
{
	class Dict;
	class XRef;
	class OutputDev;
	class Links;
	class Catalog;

	//------------------------------------------------------------------------

	class PDFRectangle
	{
	public:

		PDFRectangle()
		{
			m_dLeft = m_dBottom = m_dRight = m_dTop = 0;
		}
		PDFRectangle(double dLeft, double dBottom, double dRight, double dTop)
		{
			m_dLeft   = dLeft;
			m_dBottom = dBottom;
			m_dRight  = dRight;
			m_dTop    = dTop;
		}
		bool IsValid()
		{
			return m_dLeft != 0 || m_dBottom != 0 || m_dRight != 0 || m_dTop != 0;
		}
		void ClipTo(PDFRectangle *pRect);

	public:

		double m_dLeft;
		double m_dBottom;
		double m_dRight;
		double m_dTop;
	};

	//------------------------------------------------------------------------
	// PageAttrs
	//------------------------------------------------------------------------

	class PageAttrs
	{
	public:

		// Строим новый объект PageAttrs либо с начальными настройками из
		// pAttrs, либо со стандартными начальными настройками(если pAttrs =
		// NULL).
		PageAttrs(PageAttrs *pAttrs, Dict *pDict);

		~PageAttrs();

		PDFRectangle *GetMediaBox()
		{
			return &m_oMediaBox;
		}
		PDFRectangle *GetCropBox()
		{
			return &m_pCropBox;
		}
		bool IsCropped()
		{
			return m_bHaveCropBox;
		}
		PDFRectangle *GetBleedBox()
		{
			return &m_oBleedBox;
		}
		PDFRectangle *GetTrimBox()
		{
			return &m_oTrimBox;
		}
		PDFRectangle *GetArtBox()
		{
			return &m_oArtBox;
		}
		int           GetRotate()
		{
			return m_nRotate;
		}
		StringExt    *GetLastModified()
		{
			return (m_oLastModified.IsString() ? m_oLastModified.GetString() : (StringExt *)NULL);
		}
		Dict         *GetBoxColorInfo()
		{
			return (m_oBoxColorInfo.IsDict() ? m_oBoxColorInfo.GetDict() : (Dict *)NULL);
		}
		Dict         *GetGroup()
		{
			return (m_oGroup.IsDict() ? m_oGroup.GetDict() : (Dict *)NULL);
		}
		Stream       *GetMetadata()
		{
			return (m_oMetadata.IsStream() ? m_oMetadata.GetStream() : (Stream *)NULL);
		}
		Dict         *GetPieceInfo()
		{
			return (m_oPieceInfo.IsDict() ? m_oPieceInfo.GetDict() : (Dict *)NULL);
		}
		Dict         *GetSeparationInfo()
		{
			return (m_oSeparationInfo.IsDict() ? m_oSeparationInfo.GetDict() : (Dict *)NULL);
		}
		Dict         *GetResourceDict()
		{
			return (m_oResources.IsDict() ? m_oResources.GetDict() : (Dict *)NULL);
		}

	private:

		bool ReadBox(Dict *pDict, char *sKey, PDFRectangle *pBox);

	private:

		PDFRectangle m_oMediaBox;
		PDFRectangle m_pCropBox;
		bool         m_bHaveCropBox;
		PDFRectangle m_oBleedBox;
		PDFRectangle m_oTrimBox;
		PDFRectangle m_oArtBox;

		int          m_nRotate;

		Object       m_oLastModified;
		Object       m_oBoxColorInfo;
		Object       m_oGroup;
		Object       m_oMetadata;
		Object       m_oPieceInfo;
		Object       m_oSeparationInfo;
		Object       m_oResources;
	};

	//------------------------------------------------------------------------
	// Page
	//------------------------------------------------------------------------

	class Page
	{
	public:

		Page(GlobalParams *pGlobalParams, XRef *pXref, int nNum, Dict *pPageDict, PageAttrs *pAttrs);

		~Page();

		bool IsValid()
		{
			return m_bValid;
		}

		int GetNum()
		{
			return m_nNumber;
		}
		PDFRectangle *GetMediaBox()
		{
			return m_pAttrs->GetMediaBox();
		}
		PDFRectangle *GetCropBox()
		{
			return m_pAttrs->GetCropBox();
		}
		bool IsCropped()
		{
			return m_pAttrs->IsCropped();
		}
		double GetMediaWidth()
		{
			return m_pAttrs->GetMediaBox()->m_dRight - m_pAttrs->GetMediaBox()->m_dLeft;
		}
		double GetMediaHeight()
		{
			return m_pAttrs->GetMediaBox()->m_dTop - m_pAttrs->GetMediaBox()->m_dBottom;
		}
		double GetCropWidth()
		{
			return m_pAttrs->GetCropBox()->m_dRight - m_pAttrs->GetCropBox()->m_dLeft;
		}
		double GetCropHeight()
		{
			return m_pAttrs->GetCropBox()->m_dTop - m_pAttrs->GetCropBox()->m_dBottom;
		}
		PDFRectangle *GetBleedBox()
		{
			return m_pAttrs->GetBleedBox();
		}
		PDFRectangle *GetTrimBox()
		{
			return m_pAttrs->GetTrimBox();
		}
		PDFRectangle *GetArtBox()
		{
			return m_pAttrs->GetArtBox();
		}
		int GetRotate()
		{
			return m_pAttrs->GetRotate();
		}
		StringExt *GetLastModified()
		{
			return m_pAttrs->GetLastModified();
		}
		Dict *GetBoxColorInfo()
		{
			return m_pAttrs->GetBoxColorInfo();
		}
		Dict *GetGroup()
		{
			return m_pAttrs->GetGroup();
		}
		Stream *GetMetadata()
		{
			return m_pAttrs->GetMetadata();
		}
		Dict *GetPieceInfo()
		{
			return m_pAttrs->GetPieceInfo();
		}
		Dict *GetSeparationInfo()
		{
			return m_pAttrs->GetSeparationInfo();
		}

		// Resource
		Dict *GetResourceDict()
		{
			return m_pAttrs->GetResourceDict();
		}

		// Annotations
		Object *GetAnnots(Object *pObject)
		{
			return m_oAnnots.Fetch(m_pXref, pObject);
		}

		// Links
		Links *GetLinks(Catalog *catalog);

		// Contents
		Object *GetContents(Object *pObject)
		{
			return m_oContents.Fetch(m_pXref, pObject);
		}

		// 
		void Display(OutputDev *pOut, double dHorizDPI, double dVertDPI, int nRotate, bool bUseMediaBox, bool bCrop, bool bPrinting, Catalog *pCatalog, bool(*abortCheckCbk)(void *pData) = NULL, void *abortCheckCbkData = NULL);

		void DisplaySlice(OutputDev *pOut, double dHorizDPI, double dVertDPI, int nRotate, bool bUseMediaBox, bool bCrop, int nSliceX, int nSliceY, int nSliceW, int nSliceH, bool bPrinting, Catalog *pCatalog, bool(*abortCheckCbk)(void *pData) = NULL, void *abortCheckCbkData = NULL);

		void MakeBox(double dHorizDPI, double dVertDPI, int nRotate, bool bUseMediaBox, bool bUpSideDown, double dSliceX, double dSliceY, double dSliceW, double dSliceH, PDFRectangle *pBox, bool *pbCrop);

		void ProcessLinks(OutputDev *pOut, Catalog *pCatalog);

		void GetDefaultCTM(double *pCTM, double dHorizDPI, double dVertDPI, int nRotate, bool bUseMediaBox, bool bUpSideDown);

	private:

		XRef      *m_pXref;     // Таблица xref для данного PDF документа
		int        m_nNumber;   // Номер страницы
		PageAttrs *m_pAttrs;    // Page attributes
		Object     m_oAnnots;   // Annotations
		Object     m_oContents; // Содержимое страницы
		Object     m_oContents2;
		bool       m_bValid;    // 

		GlobalParams *m_pGlobalParams;
	};
}

#endif // _PDF_READER_PAGE_H
