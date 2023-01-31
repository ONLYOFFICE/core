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

	int												m_nCurItap;			//для определение вложенности таблицы

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
