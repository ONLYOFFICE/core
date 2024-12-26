#ifndef CONVERTER2OOXML_H
#define CONVERTER2OOXML_H

#include "../HWPFile_Private.h"
#include "../../../DesktopEditor/common/StringBuilder.h"

#include "HwpDoc/Paragraph/CtrlShapePic.h"
#include "HwpDoc/Paragraph/ParaText.h"
#include "HwpDoc/Section/Page.h"

namespace HWP
{
struct TConversionState
{
	bool m_bOpenedP;
	bool m_bOpenedR;

	TConversionState();
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

	void WriteSectionSettings(const CPage* pPage);
	void WritePicture(const CCtrlShapePic* pCtrlPic, const TConversionState& oState);
	bool SaveSVGFile(const HWP_STRING& sSVG, const HWP_STRING& sIndex);

	void WriteParaStyle(short shStyleID, short shParaShapeID, short shCharShapeID, const TConversionState& oState);

	bool GetBinBytes(const HWP_STRING& sID, CHWPStream& oBuffer, HWP_STRING& sFormat);
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
