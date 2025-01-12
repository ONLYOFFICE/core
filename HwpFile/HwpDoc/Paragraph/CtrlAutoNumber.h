#ifndef CTRLAUTONUMBER_H
#define CTRLAUTONUMBER_H

#include "Ctrl.h"
#include "../HWPStream.h"
#include "../HWPElements/HwpRecordTypes.h"
#include "../Common/Common.h"

namespace HWP
{

enum class ENumType
{
	PAGE,
	FOOTNOTE,
	ENDNOTE,
	FIGURE,
	TABLE,
	EQUATION,
	TOTAL_PAGE,
	null
};

inline ENumType GetNumType(int nValue)
{
	switch(static_cast<ENumType>(nValue))
	{
		case ENumType::PAGE:
		case ENumType::FOOTNOTE:
		case ENumType::ENDNOTE:
		case ENumType::FIGURE:
		case ENumType::TABLE:
		case ENumType::EQUATION:
		case ENumType::TOTAL_PAGE:
			return static_cast<ENumType>(nValue);
		default:
			return ENumType::null;
	}
}

class CCtrlAutoNumber : public CCtrl
{
	ENumType m_eNumType;
	ENumberShape2 m_eNumShape;
	bool m_bSuperscript;
public:
	CCtrlAutoNumber(const HWP_STRING& sCtrlID);
	CCtrlAutoNumber(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	ECtrlObjectType GetCtrlType() const override;

	ENumType GetNumType() const;
};
}

#endif // CTRLAUTONUMBER_H
