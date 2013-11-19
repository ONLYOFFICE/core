#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "utils.h"

#define MAX_PATH_LEN            256

#define XML_HEADER_STRING       L"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>"
#define PACKAGE_HEADER_STRING   L"\n<oox:package xmlns:oox=\"urn:oox\">"
#define PACKAGE_FOOTER_STRING   L"\n</oox:package>"

#define OPTIONS_HEADER_STRING	L"\n<oox:options>"
#define OPTIONS_FOOTER_STRING	L"\n</oox:options>"

#define LIST_OPTIONS_HEADER_STRING	L"\n<oox:list-options>"
#define LIST_OPTIONS_FOOTER_STRING	L"\n</oox:list-options>"

#define PART_HEADER_STRING      L"\n<oox:part oox:name=\"%s\">"
#define PART_FOOTER_STRING      L"\n</oox:part>"

#define array_size( arr )       ( ( sizeof( arr ) ) / ( sizeof( arr[ 0 ] ) ) )

typedef enum
{
    READER_MODE_INIT           = 0,
    READER_MODE_QUESTION_MARK,
    READER_MODE_CONTENT
}
reader_mode;

static const wchar_t input_files [ ] [ MAX_PATH_LEN ] =
{
    L"_rels/.rels",
    L"word/styles.xml",
    L"docProps/app.xml",
    L"docProps/core.xml",
    L"word/_rels/document.xml.rels",
	L"word/_rels/footnotes.xml.rels",
	L"word/_rels/endnotes.xml.rels",
    L"word/document.xml",
    L"word/numbering.xml",
    L"word/footnotes.xml",
    L"word/endnotes.xml",
	L"word/theme/theme1.xml"
};

#ifdef __cplusplus
extern "C"
#endif
void unite_oox( const wchar_t * input_dir, const wchar_t * output_file, const bool listInList )
{

    char      buff      [ 512 ]                         = { 0 };
    wchar_t   wbuff     [ MAX_PATH_LEN ]                = { 0 };
    size_t    len                                       = 0;
    size_t    i                                         = 0;
    FILE    * f                                         = NULL;

    LOG_MESSAGE( "unite_oox() function begin. Trying to split all xml data into a single file." );

    if ( !input_dir || !output_file )
    {
        LOG_ERROR( "Invalid input parameters." );
        return;
    }

    f = _wfopen( output_file, L"w" );
    
    if ( !f )
    {
        char buff [ 128 ] = { 0 };
        sprintf( buff, "Unable to open '%S' for output.", output_file );
        LOG_ERROR( buff );
        return;
    }

    fwprintf( f, XML_HEADER_STRING );
    fwprintf( f, PACKAGE_HEADER_STRING );
	fwprintf( f, OPTIONS_HEADER_STRING );
	fwprintf( f, LIST_OPTIONS_HEADER_STRING );
	fwprintf( f, L"%d", listInList);
	fwprintf( f, LIST_OPTIONS_FOOTER_STRING );
	fwprintf( f, OPTIONS_FOOTER_STRING );

    for ( i = 0; i < array_size( input_files ); ++i )
    {
        FILE       * tmp                           = NULL;
        wchar_t      tmp_path [ MAX_PATH_LEN * 2 ] = { 0 };
        reader_mode  mode                          = READER_MODE_INIT;

        _swprintf( tmp_path, L"%s/%s", input_dir, input_files[ i ] );
        tmp = _wfopen( tmp_path, L"r+" );
        if ( tmp )
        {
            fwprintf( f, PART_HEADER_STRING, input_files[ i ] );
            while ( !feof( tmp ) )
            {
                wchar_t c = fgetwc( tmp );
                switch ( mode )
                {
                case READER_MODE_INIT:
                    if ( c == L'?' )
                    {
                        mode = READER_MODE_QUESTION_MARK;
                    }
                    break;
                case READER_MODE_QUESTION_MARK:
                    mode = ( c == L'>' ) ? READER_MODE_CONTENT : READER_MODE_INIT;
                    break;
                case READER_MODE_CONTENT:
                    fputwc( c, f );
                    break;
                default:
                    break;
                }
            }
            fwprintf( f, PART_FOOTER_STRING );
            fclose( tmp );
        }
        else
        {
            sprintf( buff, "Missing input file: %S.", tmp_path );
            LOG_WARNING( buff );
        }
    }

    fwprintf( f, PACKAGE_FOOTER_STRING );
    fclose( f );

    LOG_MESSAGE( "unite_oox() function exited successfully." );

}
