#ifndef HWPRECORDFACENAME_H
#define HWPRECORDFACENAME_H

#include "HwpDoc/HWPDocInfo.h"
#include "../HWPStream.h"
#include "HWPRecord.h"

namespace HWP
{

enum class EAltType
{
	UNKNOWN,
	FFT,
	HFT
};

class CHWPRecordFaceName : public CHWPRecord
{
	CHWPDocInfo *m_pParent;

	bool m_bBasicFaceExists;
	bool m_bAttrExists;
	bool m_bSubstExists;

	STRING m_sFaceName;
	EAltType m_eSubstType;
	STRING m_sSubstFace;
	STRING m_sBasicFaceName;

	BYTE m_chFamilyType;
	BYTE m_chSerifStyle;
	short m_shWeight;
	short m_shPropotion;
	short m_shContrast;
	short m_shStrokeVariation;
	short m_shArmStyle;
	short m_shLetterform;
	short m_shMidLine;
	short m_shXHeight;
public:
	CHWPRecordFaceName(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}
#endif // HWPRECORDFACENAME_H
