#ifndef CONVERTER2OOXML_H
#define CONVERTER2OOXML_H

#include "../HWPFile_Private.h"

namespace HWP
{
class CConverter2OOXML
{
	CHWPFile_Private *m_pHWPFile;
	STRING m_sTempDirectory;
public:
	CConverter2OOXML();

	void SetHWPFile(CHWPFile_Private* pHWPFile);
	void SetTempDirectory(const STRING& sTempDirectory);
	bool ConvertTo(const STRING& sFilePath);
};
}

#endif // CONVERTER2OOXML_H
