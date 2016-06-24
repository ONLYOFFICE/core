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
#ifndef DOCUMENT_RELS_WRITER
#define DOCUMENT_RELS_WRITER

#include "../../XlsxSerializerCom/Common/Common.h"

namespace Writers
{
	class DocumentRelsWriter
	{
		CString	m_sDir;
	public:
		DocumentRelsWriter(CString sDir):m_sDir(sDir)
		{
		}
		void Write()
		{
			CString s_Common;

			s_Common = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?> \
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"> \
<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"word/document.xml\"/> \
<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties\" Target=\"docProps/core.xml\"/> \
<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties\" Target=\"docProps/app.xml\"/> \
</Relationships>");

            OOX::CPath fileName = m_sDir + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR + _T(".rels");

            CFile oFile;
            oFile.CreateFile(fileName.GetPath());
            oFile.WriteStringUTF8(s_Common);
			oFile.CloseFile();
		}
	};
}
#endif	// #ifndef DOCUMENT_RELS_WRITER
