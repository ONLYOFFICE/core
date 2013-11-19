#pragma once
#include "RtfDocument.h"
#include "OOXContentTypesWriter.h"
#include "OOXRelsWriter.h"

[ event_source(native)]
class OOXWriter
{
public: __event void OnProgress( long nProgress, short* shCancel );

public: CString nCurFitId;
public: int nCurFitWidth;

public: OOXRelsWriter m_oDocRels;
public: OOXRelsWriter m_oRels;
public: CAtlArray<OOXRelsWriterPtr> m_oCustomRelsWriter;
public: OOXContentTypesWriter m_oContentTypes;

public: void* m_poDocumentWriter;
public: void* m_poFootnoteWriter;
public: void* m_poEndnoteWriter;
public: void* m_poDocPropAppWriter;
public: void* m_poDocPropCoreWriter;
public: void* m_poFontTableWriter;
public: void* m_poNumberingWriter;
public: void* m_poSettingsWriter;
public: void* m_poStylesWriter;

//TempFolder
public: CString m_sTempFolder;

public: CString m_sTargetFolder;
public: CString m_sDocumentFolder;
public: CAtlMap<CString, int> m_aBookmarksId;
public: OOXWriter( RtfDocument& oDocument, CString sPath );
public: ~OOXWriter();
public: bool Save();
public: bool SaveByItemStart();
public: bool SaveByItem();
public: bool SaveByItemEnd();
private: RtfDocument& m_oDocument;
};