#pragma once

extern "C" void trans_xml( const wchar_t * xml_path,
                           const wchar_t * out_path,
                           const wchar_t * lib_path,
                           const wchar_t * xsl_filename,
                           int             xsl_filelist_start,
                           int             xsl_filenames_start,
						   const wchar_t * wsMode = NULL );