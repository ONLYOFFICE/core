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
#include "../../Format/RtfDocument.h"
#include "../../Format/RtfField.h"
#include "../../../OOXML/DocxFormat/Docx.h"
#include "../../../OOXML/DocxFormat/App.h"
#include "../../../OOXML/DocxFormat/Core.h"
#include "../../../OOXML/DocxFormat/Document.h"
#include "../../../OOXML/DocxFormat/FontTable.h"
#include "../../../OOXML/DocxFormat/Numbering.h"
#include "../../../OOXML/DocxFormat/Comments.h"
#include "../../../OOXML/DocxFormat/Styles.h"
#include "../../../OOXML/DocxFormat/Footnote.h"
#include "../../../OOXML/DocxFormat/Endnote.h"
#include "../../../OOXML/DocxFormat/Settings/WebSettings.h"
#include "../../../OOXML/DocxFormat/Settings/Settings.h"
#include "../../../OOXML/DocxFormat/External/HyperLink.h"
#include "../../../OOXML/DocxFormat/Media/Image.h"
#include "../../../OOXML/DocxFormat/Media/OleObject.h"
#include "../../../OOXML/DocxFormat/Media/ActiveX.h"
#include "../../../OOXML/DocxFormat/Media/VbaProject.h"
#include "../../../OOXML/DocxFormat/Media/JsaProject.h"
#include "../../../OOXML/DocxFormat/HeaderFooter.h"

#include "../../../OOXML/PPTXFormat/Theme.h"

class RtfConvertationManager;

class OOXReader
{
public: 

    RtfConvertationManager*							m_convertationManager;
	std::wstring									m_sPath;

	int												m_nCurItap = 0;			//for determining table nesting level
	bool											m_bInTable = false;

	int												m_nCurOleChartId;
	int												m_nCurFittextId;
	
	std::map<int, std::wstring>						m_aBookmarks;

	struct _comment
	{
		std::wstring	ref;
		int				index;
	};
	std::map<int, _comment>							m_mapComments;		//nId, ref & index added
	std::map<int, int>								m_mapCommentsPara;	//paraId, index added

	std::map<int, int>								m_mapPictureBullet;
	std::map<int, TextItemContainerPtr>				m_mapFootnotes;
	std::map<int, TextItemContainerPtr>				m_mapEndnotes;

	std::map<std::wstring, OOX::Vml::CShapeType*>	m_mapShapeTypes;	
//------------------------------------------------------------------------
	OOXIdGenerator					m_oOOXIdGenerator;

//Theme Fonts
	std::wstring					m_smajorAscii;
	std::wstring					m_smajorBidi;
	std::wstring					m_smajorEastAsia;
	std::wstring					m_smajorHAnsi;
	std::wstring					m_sminorAscii;
	std::wstring					m_sminorBidi;
	std::wstring					m_sminorEastAsia;
	std::wstring					m_sminorHAnsi;

//defaultStyles
	RtfCharStylePtr					m_oDefCharStyle;
	RtfParagraphStylePtr			m_oDefParStyle;
	RtfTableStylePtr				m_oDefTableStyle;

//TempFolder
	std::wstring					m_sTempFolder;

	OOXReader( RtfDocument& oDocument, std::wstring  path );
	bool Parse();

	OOX::IFileContainer		*m_currentContainer;
private: 
	void ParseColorTable( RtfDocument& oDocument );
	std::wstring GetFolder	( std::wstring sDocPath );
	RtfDocument &		m_oDocument;
};
