#ifndef CTRLPAGENUMPOS_H
#define CTRLPAGENUMPOS_H

#include "Ctrl.h"
#include "../HWPStream.h"
#include "../HWPElements/HwpRecordTypes.h"

namespace HWP
{
enum class ENumPos
{
	NONE,
	LEFT_TOP,
	CENTER_TOP,
	RIGHT_TOP,
	LEFT_BOTTOM,
	BOTTOM_CENTER,
	RIGHT_BOTTOM,
	OUTER_TOP,
	OUTER_BOTTOM,
	INNER_TOP,
	INNER_BOTTOM
};

class CCtrlPageNumPos : public CCtrl
{
	int m_nSize;

	ENumPos m_ePos;
	ENumberShape2 m_eNumShape;
	std::string m_sUserDef;
	std::string m_sPrefix;
	std::string m_sPostfix;
	std::string m_sConstantDash;
public:
	CCtrlPageNumPos(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	int GetSize() override;
};
}

#endif // CTRLPAGENUMPOS_H
