#include "CtrlCommon.h"

namespace HWP
{
	EVRelTo GetVRelTo(int nValue)
	{
		switch(static_cast<EVRelTo>(nValue))
		{
			case EVRelTo::PAPER:
			case EVRelTo::PAGE:
			case EVRelTo::PARA:
				return static_cast<EVRelTo>(nValue);
			default:
				return EVRelTo::null;
		}
	}

	EHRelTo GetHRelTo(int nValue)
	{
		switch(static_cast<EHRelTo>(nValue))
		{
			case EHRelTo::PAPER:
			case EHRelTo::PAGE:
			case EHRelTo::COLUMN:
			case EHRelTo::PARA:
				return static_cast<EHRelTo>(nValue);
			default:
				return EHRelTo::null;
		}
	}

	EWidthRelTo GetWidthRelTo(int nValue)
	{
		switch(static_cast<EWidthRelTo>(nValue))
		{
			case EWidthRelTo::PAPER:
			case EWidthRelTo::PAGE:
			case EWidthRelTo::COLUMN:
			case EWidthRelTo::PARA:
			case EWidthRelTo::ABSOLUTE:
				return static_cast<EWidthRelTo>(nValue);
			default:
				return EWidthRelTo::null;
		}
	}

	EHeightRelTo GetHeightRelTo(int nValue)
	{
		switch(static_cast<EHeightRelTo>(nValue))
		{
			case EHeightRelTo::PAPER:
			case EHeightRelTo::PAGE:
			case EHeightRelTo::ABSOLUTE:
				return static_cast<EHeightRelTo>(nValue);
			default:
				return EHeightRelTo::null;
		}
	}

	EHorzAlign GetHorzAlign(int nValue)
	{
		switch(static_cast<EHorzAlign>(nValue))
		{
			case EHorzAlign::CENTER:
			case EHorzAlign::RIGHT:
			case EHorzAlign::INSIDE:
			case EHorzAlign::OUTSIDE:
				return static_cast<EHorzAlign>(nValue);
			case EHorzAlign::LEFT:
			default:
				return EHorzAlign::LEFT;
		}
	}

	ETextWrap GetTextWrap(int nValue)
	{
		switch(static_cast<ETextWrap>(nValue))
		{
			case ETextWrap::TOP_AND_BOTTOM:
			case ETextWrap::BEHIND_TEXT:
			case ETextWrap::IN_FRONT_OF_TEXT:
				return static_cast<ETextWrap>(nValue);
			case ETextWrap::SQUARE:
			default:
				return ETextWrap::SQUARE;
		}
	}

	CCtrlCommon::CCtrlCommon()
	{}

	CCtrlCommon::CCtrlCommon(const STRING& sCtrlID)
		: CCtrl(sCtrlID), m_eTextVerAlign(EVertAlign::TOP)
	{}

	CCtrlCommon::CCtrlCommon(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
		: CCtrl(sCtrlID)
	{
		BYTE *pOldCurentPos = oBuffer.GetCurPtr();

		oBuffer.ReadInt(m_nObjAttr);

		m_bTreatAsChar = CHECK_FLAG(m_nObjAttr, 0x01);
		m_bAffectLSpacing = CHECK_FLAG(m_nObjAttr, 0x04);
		m_eVertRelTo = GetVRelTo(m_nObjAttr >> 3 & 0x03);
		m_eVertAlign = GetVertAlign(m_nObjAttr >> 5 & 0x07);
		m_eHorzRelTo = GetHRelTo(m_nObjAttr >> 8 & 0x03);
		m_eHorzAlign = GetHorzAlign(m_nObjAttr >> 10 & 0x07);
		m_bFlowWithText = CHECK_FLAG(m_nObjAttr, 0x2000);
		m_bAllowOverlap = CHECK_FLAG(m_nObjAttr, 0x4000);
		m_eWidthRelto = GetWidthRelTo(m_nObjAttr >> 15 & 0x07);
		m_eHeightRelto = GetHeightRelTo(m_nObjAttr >> 18 & 0x03);
		m_eTextWrap = GetTextWrap(m_nObjAttr >> 21 & 0x07);
		m_chTextFlow = (BYTE)(m_nObjAttr >> 24 & 0x03);
		m_chNumeringType = (BYTE)(m_nObjAttr >> 26 & 0x07);

		oBuffer.ReadInt(m_nVertOffset);
		oBuffer.ReadInt(m_nHorzOffset);
		oBuffer.ReadInt(m_nWidth);
		oBuffer.ReadInt(m_nHeight);
		oBuffer.ReadInt(m_nZOrder);

		for (int nIndex = 0; nIndex < 4; ++nIndex)
			oBuffer.ReadShort(m_arOutMargin[nIndex]);

		oBuffer.ReadInt(m_nObjInstanceID);
		oBuffer.ReadInt(m_nBlockPageBreak);

		if (nSize > (oBuffer.GetCurPtr() - pOldCurentPos))
			oBuffer.ReadString(m_sObjDesc);

		m_nSize = oBuffer.GetCurPtr() - pOldCurentPos;
	}

	int CCtrlCommon::GetSize()
	{
		return m_nSize;
	}

	int CCtrlCommon::ParseCtrl(CCtrlCommon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	{
		const STRING sCtrlId(oBuffer.GetCurPtr(), 4); //TODO:: StandardCharsets.US_ASCII

		if ("nil$" == sCtrlId || "loc$" == sCtrlId || "cer$" == sCtrlId || "lle$" == sCtrlId ||
		    "cra$" == sCtrlId || "lop$" == sCtrlId || "ruc$" == sCtrlId || "deqe" == sCtrlId ||
		    "cip$" == sCtrlId || "elo$" == sCtrlId || "noc$" == sCtrlId || "div$" == sCtrlId ||
		    "tat$" == sCtrlId)
			return 4;

		return 0;

		//TODO:: проверить данный метод
		// в исходном коде есть дальнейшие действия
	}

	int CCtrlCommon::ParseCaption(CCtrlCommon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	{
		oBuffer.ReadInt(oObj.m_nCaptionAttr);
		oBuffer.ReadInt(oObj.m_nCaptionWidth);
		oBuffer.ReadInt(oObj.m_nCaptionSpacing);
		oBuffer.ReadInt(oObj.m_nCaptionMaxW);

		return 16;
	}
}
