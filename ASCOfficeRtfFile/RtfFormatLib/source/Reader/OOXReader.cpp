#include "OOXReader.h"
#include "OOXSettingsReader.h"
#include "OOXFontTableReader.h"
#include "OOXStyleTableReader.h"
#include "OOXNumberingReader.h"
#include "OOXDocumentReader.h"
#include "OOXAppReader.h"
#include "OOXCoreReader.h"
#include "OOXFootnotesReader.h"
#include "OOXThemeReader.h"

OOXReader::OOXReader( RtfDocument& oDocument,CString path ): m_oDocument(oDocument)
{
	m_sPath = path;
	m_nCurItap = 0;

	m_convertationManager = NULL;
}
bool OOXReader::Parse()
{
	OOX::CDocx inputDocxFile;
	inputDocxFile.Read(m_sPath);
	
	ReaderParameter oReaderParameter;

	oReaderParameter.oDocx		= &inputDocxFile;
	oReaderParameter.oRtf		= &m_oDocument;
	oReaderParameter.oReader	= this;

	OOX::CSettings* settings = inputDocxFile.GetSettings();		
	if (settings)
	{ //важно чтобы первыми читались именно settings (например для footnote)
		OOXSettingsReader oSettingsReader( settings );
		oSettingsReader.Parse( oReaderParameter );
	}

	OOX::CFontTable* fontTable = inputDocxFile.GetFontTable();		
	if (fontTable)
	{
		OOXFontTableReader oFontTableReader( fontTable );
		oFontTableReader.Parse( oReaderParameter );
	}
	
	OOX::CTheme* theme = inputDocxFile.GetTheme();		
	if (theme)
	{
		OOXThemeReader oThemeReader( theme );
		oThemeReader.Parse( oReaderParameter );
	}

	OOX::CNumbering* numbering = inputDocxFile.GetNumbering();
	if (numbering)
	{
		OOXNumberingReader oNumberingReader( numbering );
		oNumberingReader.Parse(oReaderParameter );
	}
	
	OOX::CStyles* styles = inputDocxFile.GetStyles();		
	if (styles)
	{
		OOXStyleTableReader oStyleTableReader( styles );
		oStyleTableReader.Parse( oReaderParameter );
	}
	
	OOX::CApp* app = inputDocxFile.GetApp();		
	if (app)
	{
		OOXAppReader oDocPropAppReader( app );
		oDocPropAppReader.Parse( oReaderParameter );
	}
	
	OOX::CCore* core = inputDocxFile.GetCore();		
	if (core)
	{
		OOXCoreReader oDocPropCoreReader( core);
		oDocPropCoreReader.Parse(oReaderParameter  );
	}
	
	OOX::CFootnotes* footnotes  = inputDocxFile.GetFootnotes();		
	if (footnotes)
	{
		OOXFootnotesReader oFootnotesReader( footnotes );
		oFootnotesReader.Parse( oReaderParameter);
	}

	OOX::CEndnotes* endnotes = inputDocxFile.GetEndnotes();		
	if (endnotes)
	{
		OOXFootnotesReader oEndnotesReader( endnotes );
		oEndnotesReader.Parse( oReaderParameter);  
	}

	OOX::CDocument* document = inputDocxFile.GetDocument();		
	if (document)
	{
		OOXDocumentReader oDocReader( document );
		oDocReader.Parse( oReaderParameter );
	}

	return true;
}
CString OOXReader::GetFolder( CString sDocPath )
{
	int nLastSlash = sDocPath.ReverseFind('\\');
	CString sLeft = sDocPath.Left(nLastSlash  );
	return sLeft;
}