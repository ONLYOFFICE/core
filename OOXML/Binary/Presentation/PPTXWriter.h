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
#include "Converter.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/SystemUtils.h"

#include "DefaultNotesMaster.h"
#include "DefaultNotesTheme.h"

#include "../../PPTXFormat/NotesSlide.h"
#include "../../DocxFormat/VmlDrawing.h"
#include "../../PPTXFormat/HandoutMaster.h"

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
		
		std::vector<PPTX::HandoutMaster>	m_arHandoutMasters;
		std::vector<LONG>					m_arHandoutSlides_Master;
		std::vector<LONG>					m_arHandoutMasters_Theme;

		PPTX::Document						m_oDocument;
		PPTX::Presentation					m_oPresentation;
		PPTX::TableStyles					m_oTableStyles;
		OOX::CVmlDrawing					m_oVmlDrawing;
		OOX::CApp							m_oApp;
		OOX::CCore							m_oCore;
		nullable<PPTX::CustomProperties>	m_oCustomProperties;
		PPTX::ViewProps						m_oViewProps;
		PPTX::PresProps						m_oPresProps;
		PPTX::NotesSlide					m_oDefaultNote;

        bool m_bIsDefaultNoteMaster;

	public:
		CPPTXWriter();
		~CPPTXWriter();

		void Init(std::wstring strFolder, bool bMacro = false);
		void OpenPPTY(BYTE* pBuffer, int len, std::wstring srcFolder, std::wstring strThemesFolder);
		void ReadMasterInfo(LONG nIndexMaster);
		
		void SetRequiredDefaultsApp();
		void SetRequiredDefaultsCore();

		bool GetMacroEnabled();

		void CreateDefaultApp();
		void CreateDefaultCore();
		void CreateDefaultViewProps();
		void CreateDefaultTableStyles();
		void CreateDefaultPresProps();
		void CreateDefaultNotesMasters(int nIndexTheme);
		void CreateDefaultHandoutMasters(int nIndexTheme);
		void CreateDefaultNote();
	};
}
