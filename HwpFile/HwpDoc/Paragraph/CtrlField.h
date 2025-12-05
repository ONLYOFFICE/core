#ifndef CTRLFIELD_H
#define CTRLFIELD_H

#include "Ctrl.h"
#include "../HWPStream.h"

namespace HWP
{
enum class EFieldType
{
	Hyperlink,
	HyperlinkClosing,
	Bookmark,
	BookmarkClosing,

	Unknown
};

class CCtrlField : public CCtrl
{
	int m_nProperty;
	HWP_BYTE m_chEtcProperty;
	HWP_STRING m_sCommand;
	int m_nInstanceID;

	EFieldType m_eType;

	std::map<HWP_STRING, HWP_STRING> m_mStringParams;
	std::map<HWP_STRING, int> m_mIntegerParam;

	void UpdateType(const HWP_STRING& sCtrlID);
	void ReadType(CXMLReader& oReader, EHanType eType);
public:
	CCtrlField(const HWP_STRING& sCtrlID);
	CCtrlField(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlField(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType);

	ECtrlObjectType GetCtrlType() const override;

	void AddStringParam(const HWP_STRING& wsName, const HWP_STRING& wsValue);
	void AddIntegerParam(const HWP_STRING& wsName, int nValue);

	HWP_STRING GetStringParam(const HWP_STRING& wsName) const;
	int GetIntegerParam(const HWP_STRING& wsName) const;

	int GetInstanceID() const;
	EFieldType GetType() const;
	HWP_STRING GetCommand() const;
};
}

#endif // CTRLFIELD_H
