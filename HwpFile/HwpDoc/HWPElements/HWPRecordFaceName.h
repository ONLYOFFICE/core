#ifndef HWPRECORDFACENAME_H
#define HWPRECORDFACENAME_H

#include "../Common/XMLReader.h"
#include "../HWPDocInfo.h"
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

	HWP_STRING m_sFaceName;
	EAltType m_eSubstType;
	HWP_STRING m_sSubstFace;
	HWP_STRING m_sBasicFaceName;

	HWP_BYTE m_chFamilyType;
	HWP_BYTE m_chSerifStyle;
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
	CHWPRecordFaceName(CHWPDocInfo& oDocInfo, CXMLReader& oReader, EHanType eType);

	HWP_STRING GetFaceName() const;
};
}
#endif // HWPRECORDFACENAME_H
