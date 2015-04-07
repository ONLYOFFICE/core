#pragma once

#include "../RtfDocument.h"

#include "OOXContentTypesWriter.h"
#include "OOXRelsWriter.h"

class OOXWriter
{
public: 
	__event void OnProgress( long nProgress, short* shCancel );
	
	CString nCurFitId;
	int nCurFitWidth;

	OOXRelsWriter m_oDocRels;
	OOXRelsWriter m_oRels;
	
	std::vector<OOXRelsWriterPtr>	m_oCustomRelsWriter;
	OOXContentTypesWriter			m_oContentTypes;

	void* m_poDocumentWriter;
	void* m_poFootnoteWriter;
	void* m_poEndnoteWriter;
	void* m_poDocPropAppWriter;
	void* m_poDocPropCoreWriter;
	void* m_poFontTableWriter;
	void* m_poNumberingWriter;
	void* m_poSettingsWriter;
	void* m_poStylesWriter;

//TempFolder
	CString m_sTempFolder;

	CString m_sTargetFolder;
	CString m_sDocumentFolder;
	std::map<CString, int> m_aBookmarksId;
	
	OOXWriter( RtfDocument& oDocument, CString sPath );
	~OOXWriter();
	
	bool Save();
	bool SaveByItemStart();
	bool SaveByItem();
	bool SaveByItemEnd();

private: 
	RtfDocument& m_oDocument;
};