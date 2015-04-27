
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

        if (result != S_FALSE)
		{
			result = Convert(&doc, &docx, progress);

            if (result != S_FALSE)
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
