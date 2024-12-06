#ifndef CTRLHEADFOOT_H
#define CTRLHEADFOOT_H

#include "Ctrl.h"
#include "HWPPargraph.h"
#include "../HWPStream.h"
#include <vector>

namespace HWP
{
enum class EPageRange
{
	BOTH,
	EVEN,
	ODD
};

class CCtrlHeadFoot : public CCtrl
{
	int m_nSize;

	bool m_bIsHeader;
	int m_nAttr;
	EPageRange m_eWhichPage;
	int m_nSerialInSec;

	int m_nTextWidth;
	int m_nTextHeight;
	BYTE m_chRefLevelText;
	BYTE m_chRefLevelNum;

	std::vector<CHWPPargraph*> m_arParas;
public:
	CCtrlHeadFoot(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion, bool bIsHeader);
	~CCtrlHeadFoot();

	int GetSize() override;

	static int ParseListHeaderAppend(CCtrlHeadFoot& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLHEADFOOT_H
