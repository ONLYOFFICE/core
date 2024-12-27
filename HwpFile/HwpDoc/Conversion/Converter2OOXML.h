#ifndef CONVERTER2OOXML_H
#define CONVERTER2OOXML_H

#include "../HWPFile_Private.h"
#include "../../../DesktopEditor/common/StringBuilder.h"

#include "../Paragraph/CtrlSectionDef.h"
#include "../Paragraph/CtrlShapePic.h"
#include "../Paragraph/ParaText.h"
#include "../Paragraph/TblCell.h"

#include "../Section/Page.h"

namespace HWP
{
struct TConversionState
{
	bool m_bOpenedP;
	bool m_bOpenedR;
	bool m_bWasSpace;

	TConversionState();
};

enum class ECellCreator
{
	FILE,
	EMPTY,
	NOT_CREATED
};

class CConverter2OOXML
{
	CHWPFile_Private *m_pHWPFile;
	HWP_STRING m_sTempDirectory;

	NSStringUtils::CStringBuilder m_oStylesXml;   // styles.xml
	NSStringUtils::CStringBuilder m_oDocXmlRels;  // document.xml.rels
	NSStringUtils::CStringBuilder m_oNoteXmlRels; // footnotes.xml.rels
	NSStringUtils::CStringBuilder m_oDocXml;      // document.xml
	NSStringUtils::CStringBuilder m_oNoteXml;     // footnotes.xml
	NSStringUtils::CStringBuilder m_oNumberXml;   // numbering.xml
	NSStringUtils::CStringBuilder m_oWebSettings; // webSettings.xml

	void CreateEmptyFiles();
	void FillDefaultData();
	void Close();
	void Convert();

	bool IsRasterFormat(const HWP_STRING& sFormat);

	void WriteParagraph(const CHWPPargraph* pParagraph, TConversionState& oState);
	void WriteParagraphProperties(const CHWPPargraph* pParagraph, TConversionState& oState);
	void WriteTable(const CCtrlTable* pTable, short shParaShapeID, TConversionState& oState);
	void WriteTableProperties(const CCtrlTable* pTable, short shParaShapeID, TConversionState& oState);
	void WriteCell(const CTblCell* pCell, TConversionState& oState, ECellCreator eCellCreator);
	void WriteCellProperties(short shBorderFillID);
	void WriteBorder(const TBorder& oBorder, const HWP_STRING& sBorderName);

	void WriteSectionSettings(const CPage* pPage);
	void WritePicture(const CCtrlShapePic* pCtrlPic, const TConversionState& oState);
	bool SaveSVGFile(const HWP_STRING& sSVG, const HWP_STRING& sIndex);


	void WriteParaShapeProperties(short shParaShapeID, const TConversionState& oState);
	void WriteRunnerStyle(short shCharShapeID, const TConversionState& oState);

	bool GetBinBytes(const HWP_STRING& sID, CHWPStream& oBuffer, HWP_STRING& sFormat);
	const CCtrlSectionDef* FindSectionDef(const CHWPSection* pSection) const;
public:
	CConverter2OOXML();
	~CConverter2OOXML();

	void Clear();

	void SetHWPFile(CHWPFile_Private* pHWPFile);
	void SetTempDirectory(const HWP_STRING& sTempDirectory);
	bool ConvertTo(const HWP_STRING& sFilePath);
};
}

#endif // CONVERTER2OOXML_H
