#include "OOXDocumentWriter.h"
//#include "OOXContentTypesWriter.h"
#include "OOXRelsWriter.h"
#include "OOXDocumentWriter.h"
#include "OOXNumberingWriter.h"
#include "OOXFontTableWriter.h"
#include "OOXStylesWriter.h"
#include "OOXSettingsWriter.h"
#include "OOXThemeWriter.h"
#include "OOXDocPropAppWriter.h"
#include "OOXDocPropCoreWriter.h"
#include "OOXFootnoteWriter.h"

//#include "../../../../ASCOfficeDocxFile2/BinReader/ContentTypesWriter.h"

CString OOXDocumentWriter::CreateXmlStart()
{
	//пишем Footnotes
	RenderParameter oNewParam;
	oNewParam.poDocument = &m_oDocument;
	oNewParam.poWriter = &m_oWriter;
	oNewParam.poRels = &m_oWriter.m_oDocRels;
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;



	//пишем document.xml
	CString sResult = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n");
	sResult.Append( _T("<w:document") );
	sResult.Append( _T(" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\"") );
	sResult.Append( _T(" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\"") );
	sResult.Append( _T(" xmlns:v=\"urn:schemas-microsoft-com:vml\"") );
	sResult.Append( _T(" xmlns:o=\"urn:schemas-microsoft-com:office:office\"") );
	sResult.Append( _T(" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\"") );
	sResult.Append( _T(" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\"") );
	sResult.Append( _T(" xmlns:w10=\"urn:schemas-microsoft-com:office:word\"") );
	sResult.Append( _T(" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"") );
	sResult.Append( _T(">") );

	sResult.Append( _T("<w:body>") );
	return sResult;
}
CString OOXDocumentWriter::CreateXmlEnd( )
{
	CString sResult ;

	//пишем все кроме document.xml
	RenderParameter oNewParam;
	oNewParam.poDocument = &m_oDocument;
	oNewParam.poWriter = &m_oWriter;
	oNewParam.poRels = &m_oWriter.m_oDocRels;
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

	OOXEndnoteWriter* poEndnoteWriter = static_cast<OOXEndnoteWriter*>( m_oWriter.m_poEndnoteWriter );
	OOXFootnoteWriter* poFootnoteWriter = static_cast<OOXFootnoteWriter*>( m_oWriter.m_poFootnoteWriter );
	if( NULL != m_oDocument.m_oFootnoteCon )
	{
		oNewParam.poRels = poFootnoteWriter->m_oRelsWriter.get();
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		poFootnoteWriter->AddFootnoteBegin( _T("continuationSeparator"), 1, m_oDocument.m_oFootnoteCon->RenderToOOX( oNewParam ) );
	}
	if( NULL != m_oDocument.m_oFootnoteSep )
	{
		oNewParam.poRels = poFootnoteWriter->m_oRelsWriter.get();
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		poFootnoteWriter->AddFootnoteBegin( _T("separator"), 0, m_oDocument.m_oFootnoteSep->RenderToOOX(oNewParam) );
	}
	if( NULL != m_oDocument.m_oEndnoteCon )
	{
		oNewParam.poRels = poEndnoteWriter->m_oRelsWriter.get();
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		poEndnoteWriter->AddEndnoteBegin( _T("continuationSeparator"), 1, m_oDocument.m_oEndnoteSep->RenderToOOX(oNewParam) );
	}
	if( NULL != m_oDocument.m_oEndnoteSep )
	{
		oNewParam.poRels = poEndnoteWriter->m_oRelsWriter.get();
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		poEndnoteWriter->AddEndnoteBegin( _T("separator"), 0, m_oDocument.m_oEndnoteSep->RenderToOOX(oNewParam) );
	}

	//fontTable.xml
	OOXFontTableWriter* poFontTableWriter = static_cast<OOXFontTableWriter*>( m_oWriter.m_poFontTableWriter );
	oNewParam.poRels = poFontTableWriter->m_oRelsWriter.get();
	oNewParam.nType = RENDER_TO_OOX_PARAM_FONTTABLE;
	poFontTableWriter->AddContent( m_oDocument.m_oFontTable.RenderToOOX( oNewParam ) );

	//settings.xml
	OOXSettingsWriter* poSettingsWriter = static_cast<OOXSettingsWriter*>( m_oWriter.m_poSettingsWriter );
	oNewParam.poRels = poSettingsWriter->m_oRelsWriter.get();
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
	poSettingsWriter->AddContent( m_oDocument.m_oProperty.RenderToOOX(oNewParam) );
	//poSettingsWriter->AddContent( m_oDocument.m_oColorSchemeMapping.RenderToOOX(oNewParam) );
	poSettingsWriter->AddContent( m_oDocument.m_oMathProp.RenderToOOX(oNewParam) );

	//numbering.xml
	OOXNumberingWriter* poNumberingWriter = static_cast<OOXNumberingWriter*>( m_oWriter.m_poNumberingWriter );
	oNewParam.poRels = poNumberingWriter->m_oRelsWriter.get();
	oNewParam.nType = RENDER_TO_OOX_PARAM_NUMBERING;
	poNumberingWriter->AddNumbering( m_oDocument.m_oListTabel.RenderToOOX(oNewParam) );
	poNumberingWriter->AddNumbering( m_oDocument.m_oListOverrideTabel.RenderToOOX(oNewParam) );

	//core.xml
	OOXDocPropCoreWriter* poDocPropCoreWriter = static_cast<OOXDocPropCoreWriter*>( m_oWriter.m_poDocPropCoreWriter );
	oNewParam.poRels = poDocPropCoreWriter->m_oRelsWriter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_CORE;
	poDocPropCoreWriter->AddContent( m_oDocument.m_oInformation.RenderToOOX(oNewParam) );

	//app.xml
	OOXDocPropAppWriter* poDocPropAppWriter = static_cast<OOXDocPropAppWriter*>( m_oWriter.m_poDocPropAppWriter );
	oNewParam.poRels = poDocPropAppWriter->m_oRelsWriter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_APP;
	poDocPropAppWriter->AddContent( m_oDocument.m_oInformation.RenderToOOX(oNewParam) );

	////styles.xml
	//OOXStylesWriter* poStylesWriter = static_cast<OOXStylesWriter*>( m_oWriter.m_poStylesWriter );
	//oNewParam.poRels = poStylesWriter->m_oRelsWriter.get();
	//oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
	//CString sTempParaDef = m_oDocument.m_oDefaultParagraphProp.RenderToOOX(oNewParam);
	//CString sTempCharDef = m_oDocument.m_oDefaultCharProp.RenderToOOX(oNewParam);
	////CString sTempStyle = m_oDocument.m_oLatentStyleTable.RenderToOOX(oNewParam);
	//sTempStyle.Append( m_oDocument.m_oStyleTable.RenderToOOX(oNewParam) );
	//CString sStyles;
	//if( false == sTempParaDef.IsEmpty() || false == sTempCharDef.IsEmpty() )
	//{
	//	sStyles.Append(_T("<w:docDefaults>"));
	//	if( false == sTempParaDef.IsEmpty() )
	//	{
	//		sStyles.Append(_T("<w:pPrDefault><w:pPr>"));
	//		sStyles.Append( sTempParaDef );
	//		sStyles.Append(_T("</w:pPr></w:pPrDefault>"));
	//	}
	//	if( false == sTempCharDef.IsEmpty() )
	//	{
	//		sStyles.Append(_T("<w:rPrDefault><w:rPr>"));
	//		sStyles.Append( sTempCharDef );
	//		sStyles.Append(_T("</w:rPr></w:rPrDefault>"));
	//	}
	//	sStyles.Append(_T("</w:docDefaults>"));
	//}
	//sStyles.Append( sTempStyle );
	//poStylesWriter->AddContent( sStyles );

	////"theme.xml"
	//OOXStylesWriter* poStylesWriter = static_cast<OOXStylesWriter*>( m_oWriter. );
	//CString sTempColorTbl = m_oColorTable.RenderToOOX(poWriter, poDocument, "theme.xml",_T("")  );
	//CString sTempFontTbl = m_oFontTable.RenderToOOX(poWriter, poDocument, "theme.xml",_T("")  );
	//if( false == sTempColorTbl.IsEmpty() || false == sTempFontTbl.IsEmpty() )
	//{
	//	sResult.Append("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>");
	//	sResult.Append("<a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" >");
	//	sResult.Append("<a:themeElements>");
	//	sResult.Append( sTempColorTbl );
	//	sResult.Append( sTempFontTbl );
	//	sResult.Append( "<a:fmtScheme name=\"Office\"><a:fillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill rotWithShape=\"1\"><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"50000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"35000\"><a:schemeClr val=\"phClr\"><a:tint val=\"37000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:tint val=\"15000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"1\"/></a:gradFill><a:gradFill rotWithShape=\"1\"><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:shade val=\"51000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"80000\"><a:schemeClr val=\"phClr\"><a:shade val=\"93000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"94000\"/><a:satMod val=\"135000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"0\"/></a:gradFill></a:fillStyleLst><a:lnStyleLst><a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"><a:shade val=\"95000\"/><a:satMod val=\"105000\"/></a:schemeClr></a:solidFill><a:prstDash val=\"solid\"/></a:ln><a:ln w=\"25400\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:prstDash val=\"solid\"/></a:ln><a:ln w=\"38100\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:prstDash val=\"solid\"/></a:ln></a:lnStyleLst><a:effectStyleLst><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"20000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"38000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst><a:scene3d><a:camera prst=\"orthographicFront\"><a:rot lat=\"0\" lon=\"0\" rev=\"0\"/></a:camera><a:lightRig rig=\"threePt\" dir=\"t\"><a:rot lat=\"0\" lon=\"0\" rev=\"1200000\"/></a:lightRig></a:scene3d><a:sp3d><a:bevelT w=\"63500\" h=\"25400\"/></a:sp3d></a:effectStyle></a:effectStyleLst><a:bgFillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill rotWithShape=\"1\"><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"40000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"40000\"><a:schemeClr val=\"phClr\"><a:tint val=\"45000\"/><a:shade val=\"99000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"20000\"/><a:satMod val=\"255000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\"><a:fillToRect l=\"50000\" t=\"-80000\" r=\"50000\" b=\"180000\"/></a:path></a:gradFill><a:gradFill rotWithShape=\"1\"><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"80000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"30000\"/><a:satMod val=\"200000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\"><a:fillToRect l=\"50000\" t=\"50000\" r=\"50000\" b=\"50000\"/></a:path></a:gradFill></a:bgFillStyleLst></a:fmtScheme>" );
	//	sResult.Append("</a:themeElements>");
	//	sResult.Append("</a:theme>");
	//}

	//пишем финальные свойства секции
	oNewParam.poDocument = &m_oDocument;
	oNewParam.poWriter = &m_oWriter;
	oNewParam.poRels = &m_oWriter.m_oDocRels;
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
	sResult.Append( m_oDocument[0]->m_oProperty.RenderToOOX(oNewParam) );

	sResult.Append( _T("</w:body>") );
	sResult.Append( _T("</w:document>") );

	return sResult;
}