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
#pragma once

#include "ImageManager.h"
#include "../Records/RoundTrip.h"
#include <unordered_set>


class CStylesWriter;
class CPPTUserInfo;
class CStringWriter;
class CSlideShowInfo;


namespace PPT
{
	class CDocument;
	class CShapeWriter;

	class CPPTXWriter
	{
	public:
		CDocument*		m_pDocument;
		CPPTUserInfo*	m_pUserInfo;
		CMediaManager	m_oManager;

		CShapeWriter*	m_pShapeWriter;

		CPPTXWriter(const std::wstring & destPath);
		~CPPTXWriter();

		void CreateFile(CDocument	* pDocument);
		void CreateFile(CPPTUserInfo* pUserInfo);

		void CloseFile();

		void Write()
		{
		}
		std::wstring m_xmlApp;
		std::wstring m_xmlCore;

	protected:
		std::wstring m_strDestPath;

		void WriteApp			(NSFile::CFileBinary& oFile);
		void WriteContentTypes	();
		void WritePresInfo		();
		void WriteAll			();
		void WriteThemes		();
        bool HasRoundTrips      ()const;
        bool WriteRoundTripTheme(const CRecordSlide *pSlide, std::unordered_set<std::string> &writedFilesHash, int& nIndexTheme, int &nStartLayout);
        void WriteTheme			(CThemePtr pTheme, int & nIndexTheme, int & nStartLayout);
        void WriteSlides		();
        void WriteNotes			();
        void WriteLayoutAfterTheme (CThemePtr pTheme, const int nIndexTheme, int &nStartLayout);
        void WriteLayout		(CLayoutPtr pLayout, int nIndexLayout, int nStartLayout, int nIndexTheme);
//		void WriteRelsMaster    (std::wstring path, int type, )
        void WriteSlide			(int nIndexSlide);
        void WriteNotes			(int nIndexNotes);
        void WriteTiming        (CStringWriter& oWriter, CRelsGenerator &oRels, int nIndexSlide);
		
        void WriteTransition	(CStringWriter& oWriter, CSlideShowInfo& oSSInfo);
		void WriteColorScheme	(CStringWriter& oWriter, const std::wstring & name, const std::vector<CColor> & colors, bool extra = false);
		void WriteBackground	(CStringWriter& oWriter, CRelsGenerator& oRels, CBrush& oBackground);
		void WriteElement		(CStringWriter& oWriter, CRelsGenerator& oRels, CElementPtr pElement, CLayout* pLayout = NULL);
		void WriteGroup			(CStringWriter& oWriter, CRelsGenerator& oRels, CElementPtr pElement, CLayout* pLayout = NULL);
        void WriteTable			(CStringWriter& oWriter, CRelsGenerator& oRels, CElementPtr pElement, CLayout* pLayout = NULL);
        static std::vector<std::wstring> GrepPaths(const std::vector<std::wstring> &paths, const std::wstring &strRegEx);

	};
}
