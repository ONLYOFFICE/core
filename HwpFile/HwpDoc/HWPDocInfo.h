#ifndef HWPDOCINFO_H
#define HWPDOCINFO_H

#include "HWPFile.h"
#include "HanType.h"
#include "HWPStream.h"
#include "HWPElements/HWPRecord.h"

#include <map>
#include <list>

namespace HWP
{
class CHWPDocInfo
{
	EHanType   m_eHanType;
	// CHWPXFile *m_pParentHWPX;
	CHWPFile  *m_pParentHWP;
	std::list<CHWPRecord*> m_lRecords;

	std::map<std::string, CHWPRecord*> m_mBinDatas;
	std::list<CHWPRecord*> m_lFaseNames;
	std::list<CHWPRecord*> m_lBorderFills;
	std::list<CHWPRecord*> m_lCharShapes;
	std::list<CHWPRecord*> m_lNumberings;
	std::list<CHWPRecord*> m_lBullets;
	std::list<CHWPRecord*> m_lParaShapes;
	std::list<CHWPRecord*> m_lStyles;
	std::list<CHWPRecord*> m_lTabDefs;

	// CCompatDoc m_oCompatibleDoc;

public:
	CHWPDocInfo(EHanType eHanType);
	// CHWPDocInfo(CHWPXFile* pHWPXFile);
	CHWPDocInfo(CHWPFile* pHWPFile);

	~CHWPDocInfo();

	bool Parse(CHWPStream& oBuffer, int nVersion);
};
}

#endif // HWPDOCINFO_H
