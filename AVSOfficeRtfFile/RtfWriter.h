#pragma once
#include "FileWriter.h"
#include "AtlDefine.h"
#include "..\AVSOfficeUniversalConverter\Utils.h"

class RtfDocument;

[ event_source(native)]
class RtfWriter
{
public: __event void OnProgress( long nProgress, short* shCancel );
private: RtfDocument& m_oDocument;
private: CString m_sFilename;
public: CString m_sTempFolder;
public: CAtlArray<CString> m_aTempFiles;
public: CAtlArray<CString> m_aTempFilesSectPr;
private: bool m_bFirst;
public: RtfWriter( RtfDocument& oDocument , CString sFilename, CString sFolder ):m_oDocument(oDocument)
		{
			m_sFilename = sFilename;
			m_sTempFolder = sFolder;
			m_bFirst = true;
			m_oCurTempFileWriter = NULL;
			m_oCurTempFileSectWriter = NULL;
		}
public: ~RtfWriter()
		{
			RELEASEOBJECT( m_oCurTempFileWriter );
			RELEASEOBJECT( m_oCurTempFileSectWriter );
			for( int i = 0; i < (int)m_aTempFiles.GetCount(); i++ )
				Utils::RemoveDirOrFile( m_aTempFiles[i] );
			for( int i = 0; i < (int)m_aTempFilesSectPr.GetCount(); i++ )
				Utils::RemoveDirOrFile( m_aTempFilesSectPr[i] );
			m_aTempFiles.RemoveAll();
		}
public: bool Save();
public: bool SaveByItemStart();
public: bool SaveByItem();
public: bool SaveByItemEnd();

private: int GetCount();
private: NFileWriter::CBufferedFileWriter* m_oCurTempFileWriter;
private: NFileWriter::CBufferedFileWriter* m_oCurTempFileSectWriter;

private: CString CreateRtfStart();
private: CString CreateRtfEnd( );
};