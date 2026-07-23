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
