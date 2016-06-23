/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "../RtfDocument.h"
#include "../RtfField.h"
#include "../../../../Common/DocxFormat/Source/DocxFormat/Docx.h"

class RtfConvertationManager;

class OOXReader
{
public: 

    RtfConvertationManager *m_convertationManager;
	CString					m_sPath;

	int						m_nCurItap; //для определение вложенности таблицы

	int						m_nCurFittextId;
	std::map<int, CString>	m_aBookmarks;

	std::map<int, int>					m_mapPictureBullet;
	std::map<int, TextItemContainerPtr> m_mapFootnotes;
	std::map<int, TextItemContainerPtr> m_mapEndnotes;

	OOXIdGenerator			m_oOOXIdGenerator;

//Theme Fonts
	CString					m_smajorAscii;
	CString					m_smajorBidi;
	CString					m_smajorEastAsia;
	CString					m_smajorHAnsi;
	CString					m_sminorAscii;
	CString					m_sminorBidi;
	CString					m_sminorEastAsia;
	CString					m_sminorHAnsi;

//defaultStyles
	RtfCharStylePtr			m_oDefCharStyle;
	RtfParagraphStylePtr	m_oDefParStyle;
	RtfTableStylePtr		m_oDefTableStyle;

//TempFolder
	CString m_sTempFolder;

	OOXReader( RtfDocument& oDocument, std::wstring  path );
	bool Parse();

	OOX::IFileContainer		*m_currentContainer;
private: 
	void ParseColorTable( RtfDocument& oDocument );
	CString GetFolder	( CString sDocPath );
	RtfDocument &		m_oDocument;
};
