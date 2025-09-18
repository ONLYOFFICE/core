#ifndef CTRLAUTONUMBER_H
#define CTRLAUTONUMBER_H

#include "Ctrl.h"
#include "../HWPStream.h"
#include "../HWPElements/HwpRecordTypes.h"
#include "../Common/Common.h"
#include "../Common/XMLReader.h"

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
	TOTAL_PAGE
};

ENumType GetNumType(int nValue);
ENumType GetNumType(std::string sValue, EHanType eType);

class CCtrlAutoNumber : public CCtrl
{
	ENumType m_eNumType;
	ENumberShape2 m_eNumShape;
	bool m_bSuperscript;
public:
	CCtrlAutoNumber(const HWP_STRING& sCtrlID);
	CCtrlAutoNumber(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlAutoNumber(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType);

	ECtrlObjectType GetCtrlType() const override;

	ENumType GetNumType() const;
};
}

#endif // CTRLAUTONUMBER_H
