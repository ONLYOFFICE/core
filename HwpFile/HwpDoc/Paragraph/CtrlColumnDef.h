#ifndef CTRLCOLUMNDEF_H
#define CTRLCOLUMNDEF_H

#include "../HWPElements/HwpRecordTypes.h"
#include "../HWPStream.h"
#include "Ctrl.h"
#include <vector>

namespace HWP
{
class CCtrlColumnDef : public CCtrl
{
	int m_nSize;

	int m_nAttr;
	short m_shColCount;
	bool m_bSameSz;
	short m_shSameGap;
	std::vector<short> m_arColSzWidths;
	std::vector<short> m_arColSzGaps;
	ELineStyle2 m_eColLineStyle;
	BYTE m_chColLineWidth;
	int m_nColLineColor;
public:
	CCtrlColumnDef(const std::string& sCtrlID);
	CCtrlColumnDef(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	int GetSize() override;
};
}

#endif // CTRLCOLUMNDEF_H
