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

#include "Converter.h"

#include "WordDocument.h"

#include "TableMapping.h"

#include "StyleSheetMapping.h"
#include "FontTableMapping.h"
#include "FootnotesMapping.h"
#include "EndnotesMapping.h"
#include "NumberingMapping.h"
#include "CommentsMapping.h"
#include "SettingsMapping.h"
#include "MainDocumentMapping.h"
#include "WordprocessingDocument.h"
#include "ConversionContext.h"


namespace DocFileFormat
{
	Converter::Converter() : m_nUserLCID(-1)
	{
	}

	Converter::~Converter()
	{
	}
}

namespace DocFileFormat
{
	_UINT32 Converter::Convert(WordDocument* doc, WordprocessingDocument* docx)
	{
		if (!doc || !docx) return S_FALSE;
		
		ConversionContext context( doc, docx );

	//Write fontTable.xml
		if (doc->FontTable)
		{
			FontTableMapping fontTableMapping( &context );
			doc->FontTable->Convert( &fontTableMapping );
		}
	//Write styles.xml
		if (doc->Styles)
		{
			StyleSheetMapping styleSheetMapping( &context );
			doc->Styles->Convert( &styleSheetMapping );
		}

		//write document.xml and the header and footers
		MainDocumentMapping mainDocMapping( &context);
		doc->Convert( &mainDocMapping );

	//Write numbering.xml
		if (doc->listTable)
		{
			NumberingMapping numberingMapping( &context );
			doc->listTable->Convert( &numberingMapping );
		}

	//write the footnotes
		FootnotesMapping footnotesMapping( &context );
		doc->Convert( &footnotesMapping );

	//write the endnotes
		EndnotesMapping endnotesMapping( &context );
		doc->Convert( &endnotesMapping );

	//write the comments
		CommentsMapping commentsMapping( &context );
		doc->Convert( &commentsMapping );

	//write settings.xml at last because of the rsid list
		if (doc->DocProperties)
		{
			SettingsMapping settingsMapping( &context );
			doc->DocProperties->Convert( &settingsMapping );
		}

		return S_OK;
	}

	_UINT32 Converter::LoadAndConvert(const std::wstring& strSrcFile, const std::wstring& strDstDirectory, const std::wstring& password, bool &bMacros)
	{
		WordDocument			doc(m_sTempFolder, m_nUserLCID);
		WordprocessingDocument	docx(strDstDirectory, &doc);
		
		_UINT32 result = doc.LoadDocument(strSrcFile, password);

        if (result == 0)
		{
			result = Convert(&doc, &docx);

            if (result == 0)
			{
				result = docx.SaveDocument(bMacros);
			}
		}

		return result;
	}
}
