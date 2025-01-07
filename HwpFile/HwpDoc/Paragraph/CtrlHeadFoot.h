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
	HWP_BYTE m_chRefLevelText;
	HWP_BYTE m_chRefLevelNum;

	VECTOR<CHWPPargraph*> m_arParas;
public:
	CCtrlHeadFoot(const HWP_STRING& sCtrlID);
	CCtrlHeadFoot(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion, bool bIsHeader);

	int GetSize() override;
	bool IsHeader() const;

	void AddParagraph(CHWPPargraph* pParagraph);

	VECTOR<const CHWPPargraph*> GetParagraphs() const;

	static int ParseListHeaderAppend(CCtrlHeadFoot& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLHEADFOOT_H
