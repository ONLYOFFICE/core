#ifndef CONVERTER2OOXML_H
#define CONVERTER2OOXML_H

#include "../HWPFile_Private.h"
#include "../../../DesktopEditor/common/StringBuilder.h"

namespace HWP
{
class CConverter2OOXML
{
	CHWPFile_Private *m_pHWPFile;
	STRING m_sTempDirectory;

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
public:
	CConverter2OOXML();
	~CConverter2OOXML();

	void Clear();

	void SetHWPFile(CHWPFile_Private* pHWPFile);
	void SetTempDirectory(const STRING& sTempDirectory);
	bool ConvertTo(const STRING& sFilePath);
};
}

#endif // CONVERTER2OOXML_H
