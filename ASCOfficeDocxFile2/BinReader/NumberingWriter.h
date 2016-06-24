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
#ifndef NUMBERING_WRITER
#define NUMBERING_WRITER

#include "ContentTypesWriter.h"
#include "DocumentRelsWriter.h"

namespace Writers
{
	static CString g_string_n_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:numbering xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 wp14\">");
	static CString g_string_n_End = _T("</w:numbering>");

	class NumberingWriter
	{
		XmlUtils::CStringWriter	m_oWriter;
		CString	m_sDir;
		ContentTypesWriter& m_oContentTypesWriter;
	public:
		XmlUtils::CStringWriter	m_oANum;
		XmlUtils::CStringWriter	m_oNumList;
	public:
		NumberingWriter(CString sDir, ContentTypesWriter& oContentTypesWriter):m_sDir(sDir),m_oContentTypesWriter(oContentTypesWriter)
		{
		}
		bool IsEmpty()
		{
			return 0 == m_oANum.GetCurSize();
		}
		void Write()
		{
			if(false == IsEmpty())
			{
				m_oWriter.WriteString(g_string_n_Start);
				m_oWriter.Write(m_oANum);
				m_oWriter.Write(m_oNumList);
				m_oWriter.WriteString(g_string_n_End);

                OOX::CPath filePath = m_sDir + FILE_SEPARATOR_STR +_T("word") + FILE_SEPARATOR_STR + _T("numbering.xml");

				CFile oFile;
				oFile.CreateFile(filePath.GetPath());

				oFile.WriteStringUTF8(m_oWriter.GetData());
				oFile.CloseFile();

				//ContentType
				m_oContentTypesWriter.AddOverride(CString(_T("/word/numbering.xml")), CString(_T("application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml")));
				//Rels
				//m_oDocumentRelsWriter.AddRels(_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering"), _T("numbering.xml"));
			}
		}
	};
}
#endif	// #ifndef NUMBERING_WRITER
