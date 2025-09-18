#ifndef CTRLCOLUMNDEF_H
#define CTRLCOLUMNDEF_H

#include "../HWPElements/HwpRecordTypes.h"
#include "../HWPStream.h"
#include "Ctrl.h"
#include "../Common/XMLReader.h"

namespace HWP
{
class CCtrlColumnDef : public CCtrl
{
	int m_nAttr;
	short m_shColCount;
	bool m_bSameSz;
	short m_shSameGap;
	VECTOR<short> m_arColSzWidths;
	VECTOR<short> m_arColSzGaps;
	ELineStyle2 m_eColLineStyle;
	HWP_BYTE m_chColLineWidth;
	int m_nColLineColor;

	void ReadColumn(CXMLReader& oReader, EHanType eType, unsigned int& unColSzIndex);
public:
	CCtrlColumnDef(const HWP_STRING& sCtrlID);
	CCtrlColumnDef(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlColumnDef(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType);

	ECtrlObjectType GetCtrlType() const override;

	short GetColCount() const;
	ELineStyle2 GetColLineStyle() const;
};
}

#endif // CTRLCOLUMNDEF_H
