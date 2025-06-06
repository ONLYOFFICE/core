#include "FootnoteConverter.h"

#include "../../../DesktopEditor/common/File.h"

#include "../Paragraph/CtrlAutoNumber.h"
#include "../Paragraph/ParaText.h"

#include "Converter2OOXML.h"

namespace HWP
{
CFootnoteConverter::CFootnoteConverter()
	: m_ushCountFootnotes(0), m_ushCountEndnotes(0),
      m_ushHeaderCount(0), m_ushFooterCount(0)
{}

std::wstring CFootnoteConverter::CreateNote(const CCtrlNote* pNote, CConverter2OOXML& oConverter)
{
	if (nullptr == pNote)
		return std::wstring();

	NSStringUtils::CStringBuilder *pXMLBuilder = &m_oFootnoteXml;
	std::wstring wsPrefix = L"foot";
	std::wstring wsIndex;

	if (L"  ne" == pNote->GetID())
	{
		wsPrefix = L"end";
		pXMLBuilder = &m_oEndnoteXml;
		wsIndex = std::to_wstring(++m_ushCountEndnotes);
	}
	else
		wsIndex = std::to_wstring(++m_ushCountFootnotes);

	pXMLBuilder->WriteString(L"<w:" + wsPrefix + L"note w:id=\"" + wsIndex + L"\">");

	TConversionState oState;
	oState.m_bIsNote = true;

	for (const CHWPPargraph* pParagraph : pNote->GetParagraphs())
		oConverter.WriteParagraph(pParagraph, *pXMLBuilder, oState);

	pXMLBuilder->WriteString(L"</w:" + wsPrefix + L"note>");

	return L"<w:" + wsPrefix + L"noteReference w:id=\"" + wsIndex + L"\"/>";
}

std::wstring CFootnoteConverter::CreateHeadOrFoot(const CCtrlHeadFoot* pCtrlHeadFoot, CConverter2OOXML& oConverter)
{
	if (nullptr == pCtrlHeadFoot)
		return std::wstring();

	NSStringUtils::CStringBuilder oNewDocumentBuilder;

	TConversionState oState;

	VECTOR<TRelationship> arRelationships;
	oState.m_pRelationships = &arRelationships;

	for (const CHWPPargraph* pParagraphs : pCtrlHeadFoot->GetParagraphs())
		oConverter.WriteParagraph(pParagraphs, oNewDocumentBuilder, oState);

	const std::wstring wsFileName{(pCtrlHeadFoot->IsHeader() ? (L"header" + std::to_wstring(++m_ushHeaderCount)) : (L"footer" + std::to_wstring(++m_ushFooterCount))) + L".xml"};

	NSFile::CFileBinary oFile;
	if (!oFile.CreateFileW(oConverter.GetTempDirectory() + L"/word/" + wsFileName))
		return std::wstring();

	const std::wstring wsNodeName{pCtrlHeadFoot->IsHeader() ? L"hdr" : L"ftr"};

	oFile.WriteStringUTF8(L"<w:" + wsNodeName + L" xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" "
	                      "xmlns:cx=\"http://schemas.microsoft.com/office/drawing/2014/chartex\" "
	                      "xmlns:cx1=\"http://schemas.microsoft.com/office/drawing/2015/9/8/chartex\" "
	                      "xmlns:cx2=\"http://schemas.microsoft.com/office/drawing/2015/10/21/chartex\" "
	                      "xmlns:cx3=\"http://schemas.microsoft.com/office/drawing/2016/5/9/chartex\" "
	                      "xmlns:cx4=\"http://schemas.microsoft.com/office/drawing/2016/5/10/chartex\" "
	                      "xmlns:cx5=\"http://schemas.microsoft.com/office/drawing/2016/5/11/chartex\" "
	                      "xmlns:cx6=\"http://schemas.microsoft.com/office/drawing/2016/5/12/chartex\" "
	                      "xmlns:cx7=\"http://schemas.microsoft.com/office/drawing/2016/5/13/chartex\" "
	                      "xmlns:cx8=\"http://schemas.microsoft.com/office/drawing/2016/5/14/chartex\" "
	                      "xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" "
	                      "xmlns:aink=\"http://schemas.microsoft.com/office/drawing/2016/ink\" "
	                      "xmlns:am3d=\"http://schemas.microsoft.com/office/drawing/2017/model3d\" "
	                      "xmlns:o=\"urn:schemas-microsoft-com:office:office\" "
	                      "xmlns:oel=\"http://schemas.microsoft.com/office/2019/extlst\" "
	                      "xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" "
	                      "xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" "
	                      "xmlns:v=\"urn:schemas-microsoft-com:vml\" "
	                      "xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" "
	                      "xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" "
	                      "xmlns:w10=\"urn:schemas-microsoft-com:office:word\" "
	                      "xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" "
	                      "xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" "
	                      "xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" "
	                      "xmlns:w16cex=\"http://schemas.microsoft.com/office/word/2018/wordml/cex\" "
	                      "xmlns:w16cid=\"http://schemas.microsoft.com/office/word/2016/wordml/cid\" "
	                      "xmlns:w16=\"http://schemas.microsoft.com/office/word/2018/wordml\" "
	                      "xmlns:w16du=\"http://schemas.microsoft.com/office/word/2023/wordml/word16du\" "
	                      "xmlns:w16sdtdh=\"http://schemas.microsoft.com/office/word/2020/wordml/sdtdatahash\" "
	                      "xmlns:w16sdtfl=\"http://schemas.microsoft.com/office/word/2024/wordml/sdtformatlock\" "
	                      "xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\" "
	                      "xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" "
	                      "xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" "
	                      "xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" "
	                      "xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" "
	                      "mc:Ignorable=\"w14 w15 w16se w16cid w16 w16cex w16sdtdh w16sdtfl w16du wp14\">");
	oFile.WriteStringUTF8(oNewDocumentBuilder.GetData());
	oFile.WriteStringUTF8(L"</w:" + wsNodeName + L">");

	oFile.CloseFile();

	// TODO:: пока это копия из Converter2OOXML
	NSFile::CFileBinary oRelsWriter;
	if (oRelsWriter.CreateFileW(oConverter.GetTempDirectory() + L"/word/_rels/" + wsFileName + L".rels"))
	{
		oRelsWriter.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");

		for (const TRelationship& oRelationship : arRelationships)
		{
			oRelsWriter.WriteStringUTF8(L"<Relationship Id=\"" + oRelationship.m_wsID + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/" + oRelationship.m_wsType + L"\" Target=\"" + oRelationship.m_wsTarget + L'\"');

			if (L"hyperlink" == oRelationship.m_wsType)
				oRelsWriter.WriteStringUTF8(L" TargetMode=\"External\"/>");
			else
				oRelsWriter.WriteStringUTF8(L"/>");
			}

		oRelsWriter.WriteStringUTF8(L"</Relationships>");

		oRelsWriter.CloseFile();
	}

	return wsFileName;
}

std::wstring CFootnoteConverter::CreatePageNum(const CCtrlPageNumPos* pCtrlPageNumPos, CConverter2OOXML& oConverter)
{
	if (nullptr == pCtrlPageNumPos || ENumberShape2::DIGIT != pCtrlPageNumPos->GetFormatType() ||
	    ENumPos::NONE == pCtrlPageNumPos->GetPos())
		return std::wstring();

	std::wstring wsNodeName, wsJs;

	switch(pCtrlPageNumPos->GetPos())
	{
		case ENumPos::TOP_LEFT:
		{
			wsNodeName = L"hdr";
			wsJs = L"left";
			break;
		}
		case ENumPos::TOP_CENTER:
		case ENumPos::TOP_OUTER:
		case ENumPos::TOP_INNER:
		{
			wsNodeName = L"hdr";
			wsJs = L"center";
			break;
		}
		case ENumPos::TOP_RIGHT:
		{
			wsNodeName = L"hdr";
			wsJs = L"right";
			break;
		}
		case ENumPos::BOTTOM_LEFT:
		{
			wsNodeName = L"ftr";
			wsJs = L"left";
			break;
		}
		case ENumPos::BOTTOM_CENTER:
		case ENumPos::BOTTOM_OUTER:
		case ENumPos::BOTTOM_INNER:
		{
			wsNodeName = L"ftr";
			wsJs = L"center";
			break;
		}
		case ENumPos::BOTTOM_RIGHT:
		{
			wsNodeName = L"ftr";
			wsJs = L"right";
			break;
		}
		case ENumPos::NONE:
			break;
	}

	const std::wstring wsFileName((L"hdr" == wsNodeName) ? (L"header" + std::to_wstring(++m_ushHeaderCount)) : (L"footer" + std::to_wstring(++m_ushFooterCount)) + L".xml");

	NSFile::CFileBinary oFile;
	if (!oFile.CreateFileW(oConverter.GetTempDirectory() + L"/word/" + wsFileName))
		return std::wstring();

	oFile.WriteStringUTF8(L"<w:" + wsNodeName);
	oFile.WriteStringUTF8(L" xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" "
	                      "xmlns:cx=\"http://schemas.microsoft.com/office/drawing/2014/chartex\" "
	                      "xmlns:cx1=\"http://schemas.microsoft.com/office/drawing/2015/9/8/chartex\" "
	                      "xmlns:cx2=\"http://schemas.microsoft.com/office/drawing/2015/10/21/chartex\" "
	                      "xmlns:cx3=\"http://schemas.microsoft.com/office/drawing/2016/5/9/chartex\" "
	                      "xmlns:cx4=\"http://schemas.microsoft.com/office/drawing/2016/5/10/chartex\" "
	                      "xmlns:cx5=\"http://schemas.microsoft.com/office/drawing/2016/5/11/chartex\" "
	                      "xmlns:cx6=\"http://schemas.microsoft.com/office/drawing/2016/5/12/chartex\" "
	                      "xmlns:cx7=\"http://schemas.microsoft.com/office/drawing/2016/5/13/chartex\" "
	                      "xmlns:cx8=\"http://schemas.microsoft.com/office/drawing/2016/5/14/chartex\" "
	                      "xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" "
	                      "xmlns:aink=\"http://schemas.microsoft.com/office/drawing/2016/ink\" "
	                      "xmlns:am3d=\"http://schemas.microsoft.com/office/drawing/2017/model3d\" "
	                      "xmlns:o=\"urn:schemas-microsoft-com:office:office\" "
	                      "xmlns:oel=\"http://schemas.microsoft.com/office/2019/extlst\" "
	                      "xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" "
	                      "xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" "
	                      "xmlns:v=\"urn:schemas-microsoft-com:vml\" "
	                      "xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" "
	                      "xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" "
	                      "xmlns:w10=\"urn:schemas-microsoft-com:office:word\" "
	                      "xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" "
	                      "xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" "
	                      "xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" "
	                      "xmlns:w16cex=\"http://schemas.microsoft.com/office/word/2018/wordml/cex\" "
	                      "xmlns:w16cid=\"http://schemas.microsoft.com/office/word/2016/wordml/cid\" "
	                      "xmlns:w16=\"http://schemas.microsoft.com/office/word/2018/wordml\" "
	                      "xmlns:w16du=\"http://schemas.microsoft.com/office/word/2023/wordml/word16du\" "
	                      "xmlns:w16sdtdh=\"http://schemas.microsoft.com/office/word/2020/wordml/sdtdatahash\" "
	                      "xmlns:w16sdtfl=\"http://schemas.microsoft.com/office/word/2024/wordml/sdtformatlock\" "
	                      "xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\" "
	                      "xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" "
	                      "xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" "
	                      "xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" "
	                      "xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">");

	oFile.WriteStringUTF8(L"<w:sdt><w:sdtContent><w:p>");

	oFile.WriteStringUTF8(L"<w:pPr>");
	oFile.WriteStringUTF8(L"<w:jc w:val=\"" + wsJs + L"\"/>");
	oFile.WriteStringUTF8(L"</w:pPr>");

	if (!pCtrlPageNumPos->GetPrefix().empty())
		oFile.WriteStringUTF8(L"<w:r><w:rPr><w:sz w:val=\"28\"/><w:szCs w:val=\"28\"/></w:rPr><w:t>" + pCtrlPageNumPos->GetPrefix() + L"</w:t></w:r>");

	oFile.WriteStringUTF8(L"<w:r><w:fldChar w:fldCharType=\"begin\"/></w:r>");
	oFile.WriteStringUTF8(L"<w:r><w:instrText>PAGE \\* MERGEFORMAT</w:instrText></w:r>");
	oFile.WriteStringUTF8(L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>");
	oFile.WriteStringUTF8(L"<w:r><w:rPr><w:sz w:val=\"28\"/><w:szCs w:val=\"28\"/></w:rPr><w:t>2</w:t></w:r>");
	oFile.WriteStringUTF8(L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>");

	if (!pCtrlPageNumPos->GetPostfix().empty())
		oFile.WriteStringUTF8(L"<w:r><w:rPr><w:sz w:val=\"28\"/><w:szCs w:val=\"28\"/></w:rPr><w:t>" + pCtrlPageNumPos->GetPostfix() + L"</w:t></w:r>");

	oFile.WriteStringUTF8(L"</w:p></w:sdtContent></w:sdt>");

	oFile.WriteStringUTF8(L"</w:" + wsNodeName + L">");

	oFile.CloseFile();

	return wsFileName;
}

bool CFootnoteConverter::SaveToFile(const std::wstring& wsDirectory)
{
	if (0 == m_ushCountFootnotes && 0 == m_ushCountEndnotes && 0 == m_ushFooterCount && 0 == m_ushHeaderCount)
		return false;

	NSFile::CFileBinary oFootnoteWriter;
	if (!oFootnoteWriter.CreateFileW(wsDirectory + L"footnotes.xml"))
		return false;

	NSFile::CFileBinary oEndNotesWriter;
	if (!oEndNotesWriter.CreateFile(wsDirectory + L"endnotes.xml"))
		return false;

	oFootnoteWriter.WriteStringUTF8(L"<w:footnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:cx=\"http://schemas.microsoft.com/office/drawing/2014/chartex\" xmlns:cx1=\"http://schemas.microsoft.com/office/drawing/2015/9/8/chartex\" xmlns:cx2=\"http://schemas.microsoft.com/office/drawing/2015/10/21/chartex\" xmlns:cx3=\"http://schemas.microsoft.com/office/drawing/2016/5/9/chartex\" xmlns:cx4=\"http://schemas.microsoft.com/office/drawing/2016/5/10/chartex\" xmlns:cx5=\"http://schemas.microsoft.com/office/drawing/2016/5/11/chartex\" xmlns:cx6=\"http://schemas.microsoft.com/office/drawing/2016/5/12/chartex\" xmlns:cx7=\"http://schemas.microsoft.com/office/drawing/2016/5/13/chartex\" xmlns:cx8=\"http://schemas.microsoft.com/office/drawing/2016/5/14/chartex\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:aink=\"http://schemas.microsoft.com/office/drawing/2016/ink\" xmlns:am3d=\"http://schemas.microsoft.com/office/drawing/2017/model3d\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:oel=\"http://schemas.microsoft.com/office/2019/extlst\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:w16cex=\"http://schemas.microsoft.com/office/word/2018/wordml/cex\" xmlns:w16cid=\"http://schemas.microsoft.com/office/word/2016/wordml/cid\" xmlns:w16=\"http://schemas.microsoft.com/office/word/2018/wordml\" xmlns:w16du=\"http://schemas.microsoft.com/office/word/2023/wordml/word16du\" xmlns:w16sdtdh=\"http://schemas.microsoft.com/office/word/2020/wordml/sdtdatahash\" xmlns:w16sdtfl=\"http://schemas.microsoft.com/office/word/2024/wordml/sdtformatlock\" xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 w16se w16cid w16 w16cex w16sdtdh w16sdtfl w16du wp14\">");
	oFootnoteWriter.WriteStringUTF8(L"<w:footnote w:type=\"separator\" w:id=\"-1\"> <w:p><w:pPr><w:spacing w:after=\"0\" w:line=\"240\" w:lineRule=\"auto\"/></w:pPr><w:r><w:separator/></w:r></w:p></w:footnote>");
	oFootnoteWriter.WriteStringUTF8(L"<w:footnote w:type=\"continuationSeparator\" w:id=\"0\"><w:p><w:pPr><w:spacing w:after=\"0\" w:line=\"240\" w:lineRule=\"auto\"/></w:pPr><w:r><w:continuationSeparator/></w:r></w:p></w:footnote>");
	oFootnoteWriter.WriteStringUTF8(m_oFootnoteXml.GetData());
	oFootnoteWriter.WriteStringUTF8(L"</w:footnotes>");

	oFootnoteWriter.CloseFile();

	oEndNotesWriter.WriteStringUTF8(L"<w:endnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:cx=\"http://schemas.microsoft.com/office/drawing/2014/chartex\" xmlns:cx1=\"http://schemas.microsoft.com/office/drawing/2015/9/8/chartex\" xmlns:cx2=\"http://schemas.microsoft.com/office/drawing/2015/10/21/chartex\" xmlns:cx3=\"http://schemas.microsoft.com/office/drawing/2016/5/9/chartex\" xmlns:cx4=\"http://schemas.microsoft.com/office/drawing/2016/5/10/chartex\" xmlns:cx5=\"http://schemas.microsoft.com/office/drawing/2016/5/11/chartex\" xmlns:cx6=\"http://schemas.microsoft.com/office/drawing/2016/5/12/chartex\" xmlns:cx7=\"http://schemas.microsoft.com/office/drawing/2016/5/13/chartex\" xmlns:cx8=\"http://schemas.microsoft.com/office/drawing/2016/5/14/chartex\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:aink=\"http://schemas.microsoft.com/office/drawing/2016/ink\" xmlns:am3d=\"http://schemas.microsoft.com/office/drawing/2017/model3d\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:oel=\"http://schemas.microsoft.com/office/2019/extlst\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:w16cex=\"http://schemas.microsoft.com/office/word/2018/wordml/cex\" xmlns:w16cid=\"http://schemas.microsoft.com/office/word/2016/wordml/cid\" xmlns:w16=\"http://schemas.microsoft.com/office/word/2018/wordml\" xmlns:w16du=\"http://schemas.microsoft.com/office/word/2023/wordml/word16du\" xmlns:w16sdtdh=\"http://schemas.microsoft.com/office/word/2020/wordml/sdtdatahash\" xmlns:w16sdtfl=\"http://schemas.microsoft.com/office/word/2024/wordml/sdtformatlock\" xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 w16se w16cid w16 w16cex w16sdtdh w16sdtfl w16du wp14\">");
	oEndNotesWriter.WriteStringUTF8(L"<w:endnote w:type=\"separator\" w:id=\"-1\"><w:p><w:r><w:separator/></w:r></w:p></w:endnote><w:endnote w:type=\"continuationSeparator\" w:id=\"0\"><w:p><w:r><w:continuationSeparator/></w:r></w:p></w:endnote>");
	oEndNotesWriter.WriteStringUTF8(m_oEndnoteXml.GetData());
	oEndNotesWriter.WriteStringUTF8(L"</w:endnotes>");

	oEndNotesWriter.CloseFile();

	return true;
}
}

unsigned short HWP::CFootnoteConverter::GetFootnoteCount() const
{
	return m_ushCountFootnotes;
}

unsigned short HWP::CFootnoteConverter::GetEndnoteCount() const
{
	return m_ushCountEndnotes;
}
