#include "HWPRecordCtrlData.h"

namespace HWP
{

EParamItemType GetParamItemType(int nValue)
{
	switch(static_cast<EParamItemType>(nValue))
	{
		case EParamItemType::PIT_NULL:
		case EParamItemType::PIT_BSTR:
		case EParamItemType::PIT_I1:
		case EParamItemType::PIT_I2:
		case EParamItemType::PIT_I4:
		case EParamItemType::PIT_I:
		case EParamItemType::PIT_UI1:
		case EParamItemType::PIT_UI2:
		case EParamItemType::PIT_UI4:
		case EParamItemType::PIT_UI:
		case EParamItemType::PIT_SET:
		case EParamItemType::PIT_ARRAY:
		case EParamItemType::PIT_BINDATA:
			return static_cast<EParamItemType>(nValue);
		default:
			return EParamItemType::PIT_NULL;
	}
}

CHWPRecordCtrlData::CHWPRecordCtrlData(int nTagNum, int nLevel, int nSize)
	: CHWPRecord(nTagNum, nLevel, nSize)
{}

int CHWPRecordCtrlData::ParseCtrl(CCtrl& oCtrl, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	// TODO:: Вернуться к этому моменту
	// Само по себе содержание документа Hankom не позволяет понять, как его интерпретировать.

	oBuffer.Skip(nSize);
	return nSize;
}
}
