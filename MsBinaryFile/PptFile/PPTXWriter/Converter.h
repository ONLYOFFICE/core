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
        std::wstring	m_strTempDirectory;
        std::wstring    m_strDstFileName;

		CDocument	*	m_pDocument;
		CPPTUserInfo*	m_pUserInfo;
		CMediaManager	m_oManager;

		CShapeWriter*	m_pShapeWriter;

		CPPTXWriter();
		~CPPTXWriter();

		void CreateFile(CDocument	* pDocument);
		void CreateFile(CPPTUserInfo* pUserInfo);

		void CloseFile();

		void Write()
		{
		}
		std::wstring    m_xmlApp;
		std::wstring    m_xmlCore;

	protected:
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
