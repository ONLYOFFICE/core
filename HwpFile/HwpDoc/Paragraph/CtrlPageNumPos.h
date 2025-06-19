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
	TOP_LEFT,
	TOP_CENTER,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_CENTER,
	BOTTOM_RIGHT,
	TOP_OUTER,
	BOTTOM_OUTER,
	TOP_INNER,
	BOTTOM_INNER
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

	ENumPos GetPos() const;

	HWP_STRING GetPrefix() const;
	HWP_STRING GetPostfix() const;

	ENumberShape2 GetFormatType() const;

	ECtrlObjectType GetCtrlType() const override;
};
}

#endif // CTRLPAGENUMPOS_H
