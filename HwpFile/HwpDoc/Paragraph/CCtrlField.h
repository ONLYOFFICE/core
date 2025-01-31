#ifndef CCTRLFIELD_H
#define CCTRLFIELD_H

#include "Ctrl.h"
#include "../HWPStream.h"

namespace HWP
{
enum class EFieldType
{
	Hyperlink,
	HyperlinkClosing,

	Unknown
};

class CCtrlField : public CCtrl
{
	int m_nProperty;
	HWP_BYTE m_chEtcProperty;
	HWP_STRING m_sCommand;
	int m_nInstanceID;

	EFieldType m_eType;
public:
	CCtrlField(const HWP_STRING& sCtrlID);
	CCtrlField(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlField(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion);

	ECtrlObjectType GetCtrlType() const override;

	int GetInstanceID() const;
	EFieldType GetType() const;
	HWP_STRING GetCommand() const;
};
}

#endif // CCTRLFIELD_H
