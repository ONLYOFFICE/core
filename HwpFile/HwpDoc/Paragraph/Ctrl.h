#ifndef CTRL_H
#define CTRL_H

#include "../Common/Common.h"
#include "../Common/XMLReader.h"

namespace HWP
{
enum class ECtrlObjectType
{
	AutoNumber,
	Character,
	Click,
	ColumnDef,
	Form,
	HeadFoot,
	NewNumber,
	Note,
	PageNumPos,
	SectionDef,
	Shape,
	Table,
	ParaText,
	Common,
	Field,

	Empty
};

class CCtrl
{
	HWP_STRING m_sCtrlID;
protected:
	bool m_bFullFilled;
public:
	CCtrl();
	CCtrl(const HWP_STRING& sCtrlID);
	virtual ~CCtrl();

	virtual ECtrlObjectType GetCtrlType() const = 0;

	void SetID(const HWP_STRING& sCtrlID);
	HWP_STRING GetID() const;
	bool FullFilled() const;

	void SetFullFilled();

	static bool Equals(CCtrl* pFirstCtrl, CCtrl* pSecondCtrl);
	static CCtrl* GetCtrl(CXMLReader& oReader, int nVersion);
};
}

#endif // CTRL_H
