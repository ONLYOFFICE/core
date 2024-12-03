#ifndef CTRLAUTONUMBER_H
#define CTRLAUTONUMBER_H

#include "Ctrl.h"
#include "../HWPStream.h"
#include "../HWPElements/HwpRecordTypes.h"

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

class CCtrlAutoNumber : public CCtrl
{
	int m_nSize;

	ENumType m_eNumType;
	ENumberShape2 m_eNumShape;
	bool m_bSuperscript;
public:
	CCtrlAutoNumber(const std::string& sCtrlID);
	CCtrlAutoNumber(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	int GetSize() override;
};
}

#endif // CTRLAUTONUMBER_H
