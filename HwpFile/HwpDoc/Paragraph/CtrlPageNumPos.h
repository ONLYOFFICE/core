#ifndef CTRLPAGENUMPOS_H
#define CTRLPAGENUMPOS_H

#include "Ctrl.h"
#include "../HWPStream.h"
#include "../HWPElements/HwpRecordTypes.h"
#include "../Common/XMLNode.h"

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
	ENumPos m_ePos;
	ENumberShape2 m_eNumShape;
	HWP_STRING m_sUserDef;
	HWP_STRING m_sPrefix;
	HWP_STRING m_sPostfix;
	HWP_STRING m_sConstantDash;
public:
	CCtrlPageNumPos(const HWP_STRING& sCtrlID);
	CCtrlPageNumPos(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlPageNumPos(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion);

	ECtrlObjectType GetCtrlType() const override;
};
}

#endif // CTRLPAGENUMPOS_H
