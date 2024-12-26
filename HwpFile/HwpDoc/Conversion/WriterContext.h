#ifndef WRITERCONTEXT_H
#define WRITERCONTEXT_H

#include "../HWPFile.h"
#include "../HanType.h"
#include "../HWPSection.h"

#include "../HWPElements/HWPRecordParaShape.h"
#include "../HWPElements/HWPRecordStyle.h"
#include "../HWPElements/HWPRecordCharShape.h"
#include "../HWPElements/HWPRecordNumbering.h"
#include "../HWPElements/HWPRecordBullet.h"
#include "../HWPElements/HwpRecordTabDef.h"

namespace HWP
{
class CWriterContext
{
	EHanType m_eType;
	CHWPFile_Private* m_pHWPFile;
	int m_nVersion;
public:
	CWriterContext();
	~CWriterContext();

	VECTOR<CHWPSection*> GetSections();
	HWP_STRING DetectHancom(const HWP_STRING& sPathToFile);
	bool Detect();
	bool Open(const HWP_STRING& sPathToFile, const HWP_STRING& sHanType);
	void Close();

	const CHWPDocInfo* GetDocInfo();
	const CHWPRecordBorderFill* GetBorderFill(short shId);
	const CHWPRecordParaShape* GetParaShape(int nId);
	const CHWPRecordStyle* GetParaStyle(short shId);
	const CHWPRecordCharShape* GetCharShape(int nId);
	const CHWPRecordNumbering* GetNumbering(short shId);
	const CHWPRecordBullet* GetBullet(short shId);
	const CHwpRecordTabDef* GetTabDef(short shId);

	HWP_STRING GetBinFilename(const HWP_STRING& sId);
	bool GetBinBytes(const HWP_STRING& sId, CHWPStream& oBuffer);
	HWP_STRING GetBinFormat(const HWP_STRING& sId);
};
}

#endif // WRITERCONTEXT_H
