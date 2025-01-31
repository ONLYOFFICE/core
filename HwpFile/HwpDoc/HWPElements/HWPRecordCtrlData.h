#ifndef HWPRECORDCTRLDATA_H
#define HWPRECORDCTRLDATA_H

#include "../HWPStream.h"
#include "HWPRecord.h"
#include "../Paragraph/Ctrl.h"

namespace HWP
{
enum class EParamItemType
{
	PIT_NULL,
	PIT_BSTR,
	PIT_I1,
	PIT_I2,
	PIT_I4,
	PIT_I,
	PIT_UI1,
	PIT_UI2,
	PIT_UI4,
	PIT_UI,
	PIT_SET = 0x8000,
	PIT_ARRAY = 0x8001,
	PIT_BINDATA = 0x8002
};

struct TParameterItem
{
	short m_shItemId;
	EParamItemType m_eItemType;
	CHWPStream m_oItemData;
};

struct TParameterSet
{
	short m_shParamSetId;
	short m_shNItems;
	LIST<TParameterItem> m_arItems;
};

class CHWPRecordCtrlData : public CHWPRecord
{
	LIST<TParameterSet> m_arParamSets;
public:
	CHWPRecordCtrlData(int nTagNum, int nLevel, int nSize);

	static int ParseCtrl(CCtrl& oCtrl, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDCTRLDATA_H
