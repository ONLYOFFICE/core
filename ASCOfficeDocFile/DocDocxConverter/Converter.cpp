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

#include "Converter.h"


namespace DocFileFormat
{
	Converter::Converter()
	{
	}

	Converter::~Converter()
	{
	}
}

namespace DocFileFormat
{
	long Converter::Convert(WordDocument* doc, WordprocessingDocument* docx, const ProgressCallback* progress)
	{
		ConversionContext context( doc, docx );

		//Write numbering.xml
		NumberingMapping numberingMapping( &context );
		doc->listTable->Convert( &numberingMapping );

		if ( progress != NULL )
		{
			progress->OnProgress( progress->caller, DOC_ONPROGRESSEVENT_ID, 810000 );

			short cancel = 0;
			progress->OnProgressEx( progress->caller, DOC_ONPROGRESSEVENT_ID, 810000, &cancel );

			if ( cancel != 0 )
			{
				return S_FALSE;
			}
		}

		//write document.xml and the header and footers
		MainDocumentMapping mainDocMapping( &context, progress );
		doc->Convert( &mainDocMapping );

		if ( progress != NULL )
		{
			progress->OnProgress( progress->caller, DOC_ONPROGRESSEVENT_ID, 825000 );

			short cancel = 0;
			progress->OnProgressEx( progress->caller, DOC_ONPROGRESSEVENT_ID, 825000, &cancel );

			if ( cancel != 0 )
			{
				return S_FALSE;
			}
		}

		//Write styles.xml
		StyleSheetMapping styleSheetMapping( &context );
		doc->Styles->Convert( &styleSheetMapping );

		if ( progress != NULL )
		{
			progress->OnProgress( progress->caller, DOC_ONPROGRESSEVENT_ID, 850000 );

			short cancel = 0;
			progress->OnProgressEx( progress->caller, DOC_ONPROGRESSEVENT_ID, 850000, &cancel );

			if ( cancel != 0 )
			{
				return S_FALSE;
			}
		}

		//Write fontTable.xml
		FontTableMapping fontTableMapping( &context );
		doc->FontTable->Convert( &fontTableMapping );

		if ( progress != NULL )
		{
			progress->OnProgress( progress->caller, DOC_ONPROGRESSEVENT_ID, 875000 );

			short cancel = 0;
			progress->OnProgressEx( progress->caller, DOC_ONPROGRESSEVENT_ID, 875000, &cancel );

			if ( cancel != 0 )
			{
				return S_FALSE;
			}
		}

		//write the footnotes
		FootnotesMapping footnotesMapping( &context );
		doc->Convert( &footnotesMapping );

		if ( progress != NULL )
		{
			progress->OnProgress( progress->caller, DOC_ONPROGRESSEVENT_ID, 900000 );

			short cancel = 0;
			progress->OnProgressEx( progress->caller, DOC_ONPROGRESSEVENT_ID, 900000, &cancel );

			if ( cancel != 0 )
			{
				return S_FALSE;
			}
		}

		//write the endnotes
		EndnotesMapping endnotesMapping( &context );
		doc->Convert( &endnotesMapping );

		if ( progress != NULL )
		{
			progress->OnProgress( progress->caller, DOC_ONPROGRESSEVENT_ID, 925000 );

			short cancel = 0;
			progress->OnProgressEx( progress->caller, DOC_ONPROGRESSEVENT_ID, 925000, &cancel );

			if ( cancel != 0 )
			{
				return S_FALSE;
			}
		}

		//write the comments
		CommentsMapping commentsMapping( &context );
		doc->Convert( &commentsMapping );

		if ( progress != NULL )
		{
			progress->OnProgress( progress->caller, DOC_ONPROGRESSEVENT_ID, 950000 );

			short cancel = 0;
			progress->OnProgressEx( progress->caller, DOC_ONPROGRESSEVENT_ID, 950000, &cancel );

			if ( cancel != 0 )
			{
				return S_FALSE;
			}
		}

		//write settings.xml at last because of the rsid list
		SettingsMapping settingsMapping( &context );
		doc->DocProperties->Convert( &settingsMapping );

		if ( progress != NULL )
		{
			progress->OnProgress( progress->caller, DOC_ONPROGRESSEVENT_ID, 975000 );

			short cancel = 0;
			progress->OnProgressEx( progress->caller, DOC_ONPROGRESSEVENT_ID, 975000, &cancel );

			if ( cancel != 0 )
			{
				return S_FALSE;
			}
		}

		return S_OK;
	}

	long Converter::LoadAndConvert(const CString& strSrcFile, const CString& strDstDirectory, const ProgressCallback* progress)
	{

        long result = S_FALSE;


		WordDocument			doc(strSrcFile);
		WordprocessingDocument	docx(strDstDirectory, &doc);
		
		result = doc.LoadDocument(progress);

        if (result == S_OK)
		{
			result = Convert(&doc, &docx, progress);

            if (result == S_OK)
			{
				docx.SaveDocument();

                if (progress)progress->OnProgress(progress->caller, DOC_ONPROGRESSEVENT_ID, 1000000);

				short cancel = 0;
                if (progress)progress->OnProgressEx(progress->caller, DOC_ONPROGRESSEVENT_ID, 1000000, &cancel);

				if (0 != cancel)
				{
					return S_FALSE;
				}
			}
		}

		return result;
	}
}
