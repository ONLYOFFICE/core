#ifndef CONVERTER2OOXML_H
#define CONVERTER2OOXML_H

#include "../HWPFile_Private.h"
#include "../../../DesktopEditor/common/StringBuilder.h"

#include "../Paragraph/CtrlSectionDef.h"
#include "../Paragraph/CtrlShapePic.h"
#include "../Paragraph/CtrlShapeOle.h"
#include "../Paragraph/TblCell.h"

#include "../Section/Page.h"

#include "FootnoteConverter.h"
#include "OleConverter.h"
#include "NumberingConverter.h"

namespace HWP
{
struct TConversionState
{
	bool m_bOpenedP;
	bool m_bOpenedR;

	int m_nCountShapes;

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
	NSStringUtils::CStringBuilder m_oWebSettings; // webSettings.xml
	NSStringUtils::CStringBuilder m_oContentTypes;// [Content_Types].xml

	CNumberingConverter m_oNumberingConverter;
	CFootnoteConverter m_oFootnoteConverter;
	COleConverter m_oOleConverter;

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

	void WriteGeometryShape(const CCtrlGeneralShape* pGeneralShape, TConversionState& oState);
	void WriteEqEditShape(const CCtrlEqEdit* pEqEditShape, TConversionState& oState);
	void WriteOleShape(const CCtrlShapeOle* pOleShape, TConversionState& oState);

	void WriteSectionSettings(const CPage* pPage);
	void WritePicture(const CCtrlShapePic* pCtrlPic, const TConversionState& oState);
	bool SaveSVGFile(const HWP_STRING& sSVG, const HWP_STRING& sIndex);
	bool SavePicture(const HWP_STRING& sBinItemId);

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
	bool ConvertToFile(const HWP_STRING& sFilePath);
	bool ConvertToDir(const HWP_STRING& sDirectoryPath);
};
}

#endif // CONVERTER2OOXML_H
