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
		: m_nVertOffset(0), m_nHorzOffset(0)
	{}

	CCtrlCommon::CCtrlCommon(const HWP_STRING& sCtrlID)
		: CCtrl(sCtrlID), m_eTextVerAlign(EVertAlign::TOP)
	{}

	CCtrlCommon::CCtrlCommon(const CCtrlCommon& oCtrlCommon)
		: CCtrl(oCtrlCommon.GetID())
	{
		m_nSize = oCtrlCommon.m_nSize;

		m_nObjAttr = oCtrlCommon.m_nObjAttr;
		m_bTreatAsChar = oCtrlCommon.m_bTreatAsChar;
		m_bAffectLSpacing = oCtrlCommon.m_bAffectLSpacing;
		m_eVertRelTo = oCtrlCommon.m_eVertRelTo;
		m_eVertAlign = oCtrlCommon.m_eVertAlign;
		m_eHorzRelTo = oCtrlCommon.m_eHorzRelTo;
		m_eHorzAlign = oCtrlCommon.m_eHorzAlign;
		m_bFlowWithText = oCtrlCommon.m_bFlowWithText;
		m_bAllowOverlap = oCtrlCommon.m_bAllowOverlap;
		m_eWidthRelTo = oCtrlCommon.m_eWidthRelTo;
		m_eHeightRelTo = oCtrlCommon.m_eHeightRelTo;
		m_eTextWrap = oCtrlCommon.m_eTextWrap;
		m_chTextFlow = oCtrlCommon.m_chTextFlow;
		m_chNumeringType = oCtrlCommon.m_chNumeringType;

		m_nVertOffset = oCtrlCommon.m_nVertOffset;
		m_nHorzOffset = oCtrlCommon.m_nHorzOffset;
		m_nWidth = oCtrlCommon.m_nWidth;
		m_nHeight = oCtrlCommon.m_nHeight;
		m_nZOrder = oCtrlCommon.m_nZOrder;

		for (unsigned int unIndex = 0; unIndex < 4; ++unIndex)
			m_arOutMargin[unIndex] = oCtrlCommon.m_arOutMargin[unIndex];

		m_nObjInstanceID = oCtrlCommon.m_nObjInstanceID;
		m_nBlockPageBreak = oCtrlCommon.m_nBlockPageBreak;
		m_sObjDesc = oCtrlCommon.m_sObjDesc;

		m_arParas.resize(oCtrlCommon.m_arParas.size());
		for (unsigned int unIndex = 0; unIndex < oCtrlCommon.m_arParas.size(); ++unIndex)
		{
			oCtrlCommon.m_arParas[unIndex]->AddRef();
			m_arParas[unIndex] = oCtrlCommon.m_arParas[unIndex];
		}

		m_nCaptionAttr = oCtrlCommon.m_nCaptionAttr;
		m_nCaptionWidth = oCtrlCommon.m_nCaptionWidth;
		m_nCaptionSpacing = oCtrlCommon.m_nCaptionSpacing;
		m_nCaptionMaxW = oCtrlCommon.m_nCaptionMaxW;

		m_arCaption.resize(oCtrlCommon.m_arCaption.size());
		for (unsigned int unIndex = 0; unIndex < oCtrlCommon.m_arCaption.size(); ++unIndex)
		{
			oCtrlCommon.m_arCaption[unIndex]->AddRef();
			m_arCaption[unIndex] = oCtrlCommon.m_arCaption[unIndex];
		}

		m_eTextVerAlign = oCtrlCommon.m_eTextVerAlign;
	}

	CCtrlCommon::CCtrlCommon(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
		: CCtrl(sCtrlID)
	{
		oBuffer.SavePosition();

		oBuffer.ReadInt(m_nObjAttr);

		m_bTreatAsChar = CHECK_FLAG(m_nObjAttr, 0x01);
		m_bAffectLSpacing = CHECK_FLAG(m_nObjAttr, 0x04);
		m_eVertRelTo = GetVRelTo(m_nObjAttr >> 3 & 0x03);
		m_eVertAlign = GetVertAlign(m_nObjAttr >> 5 & 0x07);
		m_eHorzRelTo = GetHRelTo(m_nObjAttr >> 8 & 0x03);
		m_eHorzAlign = GetHorzAlign(m_nObjAttr >> 10 & 0x07);
		m_bFlowWithText = CHECK_FLAG(m_nObjAttr, 0x2000);
		m_bAllowOverlap = CHECK_FLAG(m_nObjAttr, 0x4000);
		m_eWidthRelTo = ::HWP::GetWidthRelTo(m_nObjAttr >> 15 & 0x07);
		m_eHeightRelTo = ::HWP::GetHeightRelTo(m_nObjAttr >> 18 & 0x03);
		m_eTextWrap = ::HWP::GetTextWrap(m_nObjAttr >> 21 & 0x07);
		m_chTextFlow = (HWP_BYTE)(m_nObjAttr >> 24 & 0x03);
		m_chNumeringType = (HWP_BYTE)(m_nObjAttr >> 26 & 0x07);

		oBuffer.ReadInt(m_nVertOffset);
		oBuffer.ReadInt(m_nHorzOffset);
		oBuffer.ReadInt(m_nWidth);
		oBuffer.ReadInt(m_nHeight);
		oBuffer.ReadInt(m_nZOrder);

		for (int nIndex = 0; nIndex < 4; ++nIndex)
			oBuffer.ReadShort(m_arOutMargin[nIndex]);

		oBuffer.ReadInt(m_nObjInstanceID);
		oBuffer.ReadInt(m_nBlockPageBreak);

		if (nSize > oBuffer.GetDistanceToLastPos())
			oBuffer.ReadString(m_sObjDesc, EStringCharacter::UTF16);

		m_nSize = oBuffer.GetDistanceToLastPos(true);
	}

	CCtrlCommon::~CCtrlCommon()
	{
		for (CHWPPargraph* pParagraph : m_arParas)
		{
			if (0 == pParagraph->Release())
				pParagraph = nullptr;
		}

		for (CCapParagraph* pCapParagraph : m_arCaption)
		{
			if (0 == pCapParagraph->Release())
				pCapParagraph = nullptr;
		}
	}

	void CCtrlCommon::SetTextVerAlign(EVertAlign eVertAlign)
	{
		m_eTextVerAlign = eVertAlign;
	}

	void CCtrlCommon::AddParagraph(CHWPPargraph* pParagraph)
	{
		m_arParas.push_back(pParagraph);
	}

	void CCtrlCommon::AddCaption(CCapParagraph* pCapPara)
	{
		m_arCaption.push_back(pCapPara);
	}

	int CCtrlCommon::GetSize()
	{
		return m_nSize;
	}

	CHWPPargraph* CCtrlCommon::GetLastPara()
	{
		return (!m_arParas.empty()) ? m_arParas.back() : nullptr;
	}

	unsigned int CCtrlCommon::GetCountParagraphs() const
	{
		return m_arParas.size();
	}

	const CHWPPargraph* CCtrlCommon::GetParagraphs(unsigned int unIndex) const
	{
		if (unIndex >= m_arParas.size())
			return nullptr;

		return m_arParas[unIndex];
	}

	int CCtrlCommon::GetHorzOffset() const
	{
		return m_nHorzOffset;
	}

	int CCtrlCommon::GetVertOffset() const
	{
		return m_nVertOffset;
	}

	EVRelTo CCtrlCommon::GetVertRelTo() const
	{
		return m_eVertRelTo;
	}

	EHRelTo CCtrlCommon::GetHorzRelTo() const
	{
		return m_eHorzRelTo;
	}

	int CCtrlCommon::GetCaptionWidth() const
	{
		return m_nCaptionWidth;
	}

	bool CCtrlCommon::CaptionsEmpty() const
	{
		return m_arCaption.empty();
	}

	HWP_STRING CCtrlCommon::GetDesc() const
	{
		return m_sObjDesc;
	}

	EWidthRelTo CCtrlCommon::GetWidthRelTo() const
	{
		return m_eWidthRelTo;
	}

	EHeightRelTo CCtrlCommon::GetHeightRelTo() const
	{
		return m_eHeightRelTo;
	}

	int CCtrlCommon::GetWidth() const
	{
		return m_nWidth;
	}

	int CCtrlCommon::GetHeight() const
	{
		return m_nHeight;
	}

	ETextWrap CCtrlCommon::GetTextWrap() const
	{
		return m_eTextWrap;
	}

	int CCtrlCommon::GetZOrder() const
	{
		return m_nZOrder;
	}

	int CCtrlCommon::ParseCtrl(CCtrlCommon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	{
		HWP_STRING sCtrlId;
		oBuffer.ReadString(sCtrlId, 4, EStringCharacter::ASCII);

		if (L"nil$" == sCtrlId || L"loc$" == sCtrlId || L"cer$" == sCtrlId || L"lle$" == sCtrlId ||
		    L"cra$" == sCtrlId || L"lop$" == sCtrlId || L"ruc$" == sCtrlId || L"deqe" == sCtrlId ||
		    L"cip$" == sCtrlId || L"elo$" == sCtrlId || L"noc$" == sCtrlId || L"div$" == sCtrlId ||
		    L"tat$" == sCtrlId)
			return 4;

		oBuffer.Skip(-4);
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
