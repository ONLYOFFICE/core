#ifndef HWPDOCINFO_H
#define HWPDOCINFO_H

#include "HanType.h"
#include "HWPStream.h"
#include "HWPElements/HWPRecord.h"

#include <map>

namespace HWP
{
enum class ECompatDoc
{
	HWP,
	OLD_HWP,
	MS_WORD,
	UNKNOWN
};

class CHWPFile_Private;
class CHWPDocInfo
{
	EHanType   m_eHanType;
	// CHWPXFile *m_pParentHWPX;
	CHWPFile_Private  *m_pParentHWP;
	VECTOR<CHWPRecord*> m_arRecords;

	std::map<HWP_STRING, CHWPRecord*> m_mBinDatas;
	VECTOR<CHWPRecord*> m_arFaseNames;
	VECTOR<CHWPRecord*> m_arBorderFills;
	VECTOR<CHWPRecord*> m_arCharShapes;
	VECTOR<CHWPRecord*> m_arNumberings;
	VECTOR<CHWPRecord*> m_arBullets;
	VECTOR<CHWPRecord*> m_arParaShapes;
	VECTOR<CHWPRecord*> m_arStyles;
	VECTOR<CHWPRecord*> m_arTabDefs;

	ECompatDoc m_eCompatibleDoc;
public:
	CHWPDocInfo(EHanType eHanType);
	// CHWPDocInfo(CHWPXFile* pHWPXFile);
	CHWPDocInfo(CHWPFile_Private* pHWPFile);

	~CHWPDocInfo();

	bool Parse(CHWPStream& oBuffer, int nVersion);

	const CHWPRecord* GetRecord(int nIndex) const;
	const CHWPRecord* GetFaceName(int nIndex) const;
	const CHWPRecord* GetBorderFill(int nIndex) const;
	const CHWPRecord* GetCharShape(int nIndex) const;
	const CHWPRecord* GetNumbering(int nIndex) const;
	const CHWPRecord* GetBullet(int nIndex) const;
	const CHWPRecord* GetParaShape(int nIndex) const;
	const CHWPRecord* GetStyle(int nIndex) const;
	const CHWPRecord* GetTabDef(int nIndex) const;

	CHWPFile_Private* GetParentHWP();

	const CHWPRecord* GetBinData(const HWP_STRING& sID) const;
	EHanType GetHanType() const;
};
}

#endif // HWPDOCINFO_H
