#ifndef CONVERTER2OOXML_H
#define CONVERTER2OOXML_H

#include "../HWPFile.h"
#include "../../../DesktopEditor/common/StringBuilder.h"

#include "../Paragraph/CtrlAutoNumber.h"
#include "../Paragraph/CtrlSectionDef.h"
#include "../Paragraph/CtrlShapeVideo.h"
#include "../Paragraph/CtrlCharacter.h"
#include "../Paragraph/CtrlColumnDef.h"
#include "../Paragraph/CtrlShapePic.h"
#include "../Paragraph/CtrlShapeOle.h"
#include "../Paragraph/TblCell.h"

#include "FootnoteConverter.h"
#include "OleConverter.h"
#include "NumberingConverter.h"

#include "../Common/WriterContext.h"

namespace HWP
{
struct TConversionState
{
	bool m_bOpenedP;
	bool m_bOpenedR;

	bool m_bNeedLineBreak;

	unsigned short m_ushSecdIndex;

	const CCtrlSectionDef* m_pSectionDef;
	const CCtrlColumnDef*  m_pColumnDef;

	TConversionState();
};

struct TRelationship
{
	HWP_STRING m_wsID;
	HWP_STRING m_wsType;
	HWP_STRING m_wsTarget;
};

enum class ECellCreator
{
	FILE,
	EMPTY,
	NOT_CREATED
};

class CConverter2OOXML
{
	CWriterContext * m_pContext;
	HWP_STRING m_sTempDirectory;

	NSStringUtils::CStringBuilder m_oStylesXml;   // styles.xml
	NSStringUtils::CStringBuilder m_oDocXml;      // document.xml
	NSStringUtils::CStringBuilder m_oNoteXmlRels; // footnotes.xml.rels
	NSStringUtils::CStringBuilder m_oWebSettings; // webSettings.xml
	NSStringUtils::CStringBuilder m_oContentTypes;// [Content_Types].xml

	VECTOR<TRelationship> m_arRelationships;

	CNumberingConverter m_oNumberingConverter;
	CFootnoteConverter m_oFootnoteConverter;
	COleConverter m_oOleConverter;

	unsigned short m_ushShapeCount;
	unsigned short m_ushPageCount;
	unsigned short m_ushTableCount;
	unsigned short m_ushEquationCount;

	void CreateEmptyFiles();
	void FillDefaultData();
	void Close();
	void Convert();

	bool IsRasterFormat(const HWP_STRING& sFormat);

	void WriteParagraphProperties(short shParaShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteTable(const CCtrlTable* pTable, short shParaShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteTableProperties(const CCtrlTable* pTable, short shParaShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteCell(const CTblCell* pCell, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState, ECellCreator eCellCreator);
	void WriteCellProperties(short shBorderFillID, NSStringUtils::CStringBuilder& oBuilder);
	void WriteCellBorder(const TBorder& oBorder, const HWP_STRING& sBorderName, NSStringUtils::CStringBuilder& oBuilder);

	void WriteGeometryShape(const CCtrlGeneralShape* pGeneralShape, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteEqEditShape(const CCtrlEqEdit* pEqEditShape, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteOleShape(const CCtrlShapeOle* pOleShape, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);

	void WriteSectionSettings(TConversionState& oState);
	void WritePicture(const CCtrlShapePic* pCtrlPic, NSStringUtils::CStringBuilder& oBuilder, const TConversionState& oState);
	void WriteVideo(const CCtrlShapeVideo* pCtrlVideo, NSStringUtils::CStringBuilder& oBuilder, const TConversionState& oState);
	bool SaveSVGFile(const HWP_STRING& sSVG, HWP_STRING& sFileName);
	HWP_STRING SavePicture(const HWP_STRING& sBinItemId);

	void WriteParaShapeProperties(short shParaShapeID, NSStringUtils::CStringBuilder& oBuilder, const TConversionState& oState);
	void WriteRunnerStyle(short shCharShapeID, NSStringUtils::CStringBuilder& oBuilder, const TConversionState& oState);

	void OpenDrawingNode(const CCtrlCommon* pCtrlShape, NSStringUtils::CStringBuilder& oBuilder);
	void CloseDrawingNode(const CCtrlCommon* pCtrlShape, NSStringUtils::CStringBuilder& oBuilder);
	void WriteShapeProperty(const CCtrlCommon* pCtrlShape, NSStringUtils::CStringBuilder& oBuilder);

	void OpenParagraph(short shParaShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void CloseParagraph(NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);

	void WriteText(const std::wstring& wsText, short shParaShapeID, short shCharShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteLineSettings(const CCtrlGeneralShape* pCtrlGeneralShape, NSStringUtils::CStringBuilder& oBuilder);
	void WriteLineSettings(ELineStyle2 eLineStyle, int nColor, int nThick, HWP_BYTE nCompoundLineType, NSStringUtils::CStringBuilder& oBuilder);
	void WriteBorderSettings(const CCtrlShapePic* pCtrlPic, NSStringUtils::CStringBuilder& oBuilder);

	void WriteAutoNumber(const CCtrlAutoNumber* pAutoNumber, short shParaShapeID, short shCharShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteCharacter(const CCtrlCharacter* pCharacter, short shParaShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);
	void WriteShape(const CCtrlGeneralShape* pShape, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);

	HWP_STRING AddRelationship(const HWP_STRING& wsType, const HWP_STRING& wsTarget);
	void AddContentType(const HWP_STRING& wsName, const HWP_STRING& wsType);
public:
	CConverter2OOXML();
	~CConverter2OOXML();

	void WriteParagraph(const CHWPPargraph* pParagraph, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState);

	void SetContext(CWriterContext* pContext);
	void SetTempDirectory(const HWP_STRING& sTempDirectory);
	bool ConvertToFile(const HWP_STRING& sFilePath);
	bool ConvertToDir(const HWP_STRING& sDirectoryPath);

	HWP_STRING GetTempDirectory() const;
};
}

#endif // CONVERTER2OOXML_H
