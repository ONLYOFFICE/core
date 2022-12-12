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
#include "Converter.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/SystemUtils.h"

#include "DefaultNotesMaster.h"
#include "DefaultNotesTheme.h"

namespace NSBinPptxRW
{
	class CPPTXWriter
	{
	private:
		CBinaryFileReader			m_oReader;
		CImageManager2				m_oImageManager;
		std::wstring				m_strDstFolder;

		std::map<BYTE, LONG>		m_mainTables;

		std::vector<PPTX::Theme>			m_arThemes;
		
		std::vector<PPTX::SlideMaster>		m_arSlideMasters;
		std::vector<_slideMasterInfo>		m_arSlideMasters_Theme;
		
		std::vector<PPTX::SlideLayout>		m_arSlideLayouts;
		std::vector<LONG>					m_arSlideLayouts_Master;

		std::vector<PPTX::Slide>			m_arSlides;
		std::vector<LONG>					m_arSlides_Layout;
		std::vector<LONG>					m_arSlides_Notes;

		std::vector<PPTX::NotesMaster>		m_arNotesMasters;
		std::vector<PPTX::NotesSlide>		m_arNotesSlides;
		std::vector<LONG>					m_arNotesSlides_Master;
		std::vector<LONG>					m_arNotesMasters_Theme;
		
		PPTX::Document					m_oDocument;
		PPTX::Presentation				m_oPresentation;
		PPTX::TableStyles				m_oTableStyles;
		OOX::CVmlDrawing				m_oVmlDrawing;
		PPTX::App						m_oApp;
		PPTX::Core						m_oCore;
		nullable<PPTX::CustomProperties>m_oCustomProperties;
		PPTX::ViewProps					m_oViewProps;
		PPTX::PresProps					m_oPresProps;
		PPTX::NotesSlide				m_oDefaultNote;

        bool m_bIsDefaultNoteMaster;

	public:
		CPPTXWriter();
		~CPPTXWriter();

		void Init(std::wstring strFolder, bool bMacro = false);
		void OpenPPTY(BYTE* pBuffer, int len, std::wstring srcFolder, std::wstring strThemesFolder);
		void ReadMasterInfo(LONG nIndexMaster);
		void SetRequiredDefaultsApp();
		void CreateDefaultApp();
		void SetRequiredDefaultsCore();

		void CreateDefaultCore();
		void CreateDefaultViewProps();
		void CreateDefaultTableStyles();
		void CreateDefaultPresProps();
		void CreateDefaultNotesMasters(int nIndexTheme);
		void CreateDefaultNote();
	};
}
