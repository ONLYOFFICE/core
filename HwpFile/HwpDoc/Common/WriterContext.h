#ifndef WRITERCONTEXT_H
#define WRITERCONTEXT_H

#include "../HanType.h"
#include "../HWPFile.h"
#include "../HWPXFile.h"
#include "../HWPMLFile.h"

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
	EHanType    m_eType;
	CHWPFile*   m_pHWPFile;
	CHWPXFile*  m_pHWPXFile;
	CHWPMLFile* m_pHWPMLFile;
public:
	CWriterContext();
	~CWriterContext();

	void Clear();
	EHanType GetType();

	VECTOR<const CHWPSection*> GetSections();
	static EHanType DetectHancom(const HWP_STRING& sPathToFile);
	bool Detect();
	bool Open(const HWP_STRING& sPathToFile, EHanType eHanType);
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
	bool GetBinBytes(const HWP_STRING& sId, CHWPStream& oBuffer, HWP_STRING& sFileName);
	HWP_STRING GetBinFormat(const HWP_STRING& sId);
};
}

#endif // WRITERCONTEXT_H
