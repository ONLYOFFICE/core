#include "HWPRecordCtrlData.h"
#include "../Paragraph/CtrlField.h"

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

	oBuffer.SavePosition();

	if (ECtrlObjectType::Field == oCtrl.GetCtrlType())
	{
		short shSetID, shNumberItems;

		oBuffer.ReadShort(shSetID);
		oBuffer.ReadShort(shNumberItems);

		short shItemID, shItemType;
		for (unsigned short ushIndex = 0; ushIndex < shNumberItems; ++ushIndex)
		{
			oBuffer.ReadShort(shItemID);
			short shUnknownValue;
			oBuffer.ReadShort(shUnknownValue); // Unknown Data
			oBuffer.ReadShort(shItemType);

			switch (GetParamItemType(shItemType))
			{
				case EParamItemType::PIT_NULL: oBuffer.Skip(4); break;
				case EParamItemType::PIT_BSTR:
				{
					HWP_STRING sValue;
					oBuffer.ReadString(sValue, EStringCharacter::UTF16);

					if (EFieldType::Bookmark == ((CCtrlField&)oCtrl).GetType())
						((CCtrlField&)oCtrl).AddStringParam(L"bookmarkname", sValue);

					break;
				}
				case EParamItemType::PIT_I1: oBuffer.Skip(1); break;
				case EParamItemType::PIT_I2: oBuffer.Skip(2); break;
				case EParamItemType::PIT_I4: oBuffer.Skip(4); break;
				case EParamItemType::PIT_I:  oBuffer.Skip(4); break;
				case EParamItemType::PIT_UI1: oBuffer.Skip(1); break;
				case EParamItemType::PIT_UI2: oBuffer.Skip(2); break;
				case EParamItemType::PIT_UI4: oBuffer.Skip(4); break;
				case EParamItemType::PIT_UI: oBuffer.Skip(4); break;
				case EParamItemType::PIT_SET:
				case EParamItemType::PIT_ARRAY:
				case EParamItemType::PIT_BINDATA:
					break;
			}
		}
	}

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
	return nSize;
}
}
