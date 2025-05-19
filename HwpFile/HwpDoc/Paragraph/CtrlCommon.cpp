#include "CtrlCommon.h"
#include "../Common/Common.h"

#include "CapParagraph.h"

namespace HWP
{
	EVertAlign GetVertAlign(int nValue)
	{
		SWITCH(EVertAlign, nValue)
		{
			DEFAULT(EVertAlign::TOP);
			CASE(EVertAlign::CENTER);
			CASE(EVertAlign::BOTTOM);
			CASE(EVertAlign::INSIDE);
			CASE(EVertAlign::OUTSIDE);
		}
	}

	EVertAlign GetVertAlign(const HWP_STRING& sValue)
	{
		IF_STRING_IN_ENUM(CENTER, sValue, EVertAlign);
		ELSE_IF_STRING_IN_ENUM(BOTTOM, sValue, EVertAlign);
		ELSE_IF_STRING_IN_ENUM(INSIDE, sValue, EVertAlign);
		ELSE_IF_STRING_IN_ENUM(OUTSIDE, sValue, EVertAlign);
		ELSE_STRING_IN_ENUM(TOP, EVertAlign);
	}

	EVRelTo GetVRelTo(int nValue)
	{
		SWITCH(EVRelTo, nValue)
		{
			CASE(EVRelTo::PARA);
			CASE(EVRelTo::PAGE);
			DEFAULT(EVRelTo::PAPER);
		}
	}

	EVRelTo GetVRelTo(const HWP_STRING& sValue)
	{
		IF_STRING_IN_ENUM(PARA, sValue, EVRelTo);
		ELSE_IF_STRING_IN_ENUM(PAGE, sValue, EVRelTo);
		ELSE_IF_STRING_IN_ENUM(PAPER, sValue, EVRelTo);
		ELSE_STRING_IN_ENUM(PARA, EVRelTo);
	}

	EHRelTo GetHRelTo(int nValue)
	{
		SWITCH(EHRelTo, nValue)
		{
			CASE(EHRelTo::PAGE);
			CASE(EHRelTo::PARA);
			CASE(EHRelTo::COLUMN);
			DEFAULT(EHRelTo::PAPER);
		}
	}

	EHRelTo GetHRelTo(const HWP_STRING& sValue)
	{
		IF_STRING_IN_ENUM(PAGE, sValue, EHRelTo);
		ELSE_IF_STRING_IN_ENUM(PARA, sValue, EHRelTo);
		ELSE_IF_STRING_IN_ENUM(COLUMN, sValue, EHRelTo);
		ELSE_STRING_IN_ENUM(PAPER, EHRelTo);
	}

	EWidthRelTo GetWidthRelTo(int nValue)
	{
		SWITCH(EWidthRelTo, nValue)
		{
			CASE(EWidthRelTo::PAGE);
			CASE(EWidthRelTo::PARA);
			CASE(EWidthRelTo::COLUMN);
			CASE(EWidthRelTo::ABSOLUTE);
			DEFAULT(EWidthRelTo::PAPER);
		}
	}

	EWidthRelTo GetWidthRelTo(const HWP_STRING& sValue)
	{
		IF_STRING_IN_ENUM(PAGE, sValue, EWidthRelTo);
		ELSE_IF_STRING_IN_ENUM(PARA, sValue, EWidthRelTo);
		ELSE_IF_STRING_IN_ENUM(COLUMN, sValue, EWidthRelTo);
		ELSE_IF_STRING_IN_ENUM(ABSOLUTE, sValue, EWidthRelTo);
		ELSE_STRING_IN_ENUM(PAPER, EWidthRelTo);
	}

	EHeightRelTo GetHeightRelTo(int nValue)
	{
		SWITCH(EHeightRelTo, nValue)
		{
			CASE(EHeightRelTo::PAGE);
			CASE(EHeightRelTo::ABSOLUTE);
			DEFAULT(EHeightRelTo::PAPER);
		}
	}

	EHeightRelTo GetHeightRelTo(const HWP_STRING& sValue)
	{
		IF_STRING_IN_ENUM(PAGE, sValue, EHeightRelTo);
		ELSE_IF_STRING_IN_ENUM(ABSOLUTE, sValue, EHeightRelTo);
		ELSE_STRING_IN_ENUM(PAPER, EHeightRelTo);
	}

	EHorzAlign GetHorzAlign(int nValue)
	{
		SWITCH(EHorzAlign, nValue)
		{
			CASE(EHorzAlign::CENTER);
			CASE(EHorzAlign::RIGHT);
			CASE(EHorzAlign::INSIDE);
			CASE(EHorzAlign::OUTSIDE);
			DEFAULT(EHorzAlign::LEFT);
		}
	}

	EHorzAlign GetHorzAlign(const HWP_STRING& sValue)
	{
		IF_STRING_IN_ENUM(CENTER, sValue, EHorzAlign);
		ELSE_IF_STRING_IN_ENUM(RIGHT, sValue, EHorzAlign);
		ELSE_IF_STRING_IN_ENUM(INSIDE, sValue, EHorzAlign);
		ELSE_IF_STRING_IN_ENUM(OUTSIDE, sValue, EHorzAlign);
		ELSE_STRING_IN_ENUM(LEFT, EHorzAlign);
	}

	ETextWrap GetTextWrap(int nValue)
	{
		SWITCH(ETextWrap, nValue)
		{
			CASE(ETextWrap::TOP_AND_BOTTOM);
			CASE(ETextWrap::BEHIND_TEXT);
			CASE(ETextWrap::IN_FRONT_OF_TEXT);
			DEFAULT(ETextWrap::SQUARE);
		}
	}

	ETextWrap GetTextWrap(const HWP_STRING& sValue)
	{
		IF_STRING_IN_ENUM(TOP_AND_BOTTOM, sValue, ETextWrap);
		ELSE_IF_STRING_IN_ENUM(BEHIND_TEXT, sValue, ETextWrap);
		ELSE_IF_STRING_IN_ENUM(IN_FRONT_OF_TEXT, sValue, ETextWrap);
		ELSE_STRING_IN_ENUM(SQUARE, ETextWrap);
	}

	CCtrlCommon::CCtrlCommon()
	    : m_nVertOffset(0), m_nHorzOffset(0), m_arOutMargin{0, 0, 0, 0}, m_arInMargin{0, 0, 0, 0}
	{}

	CCtrlCommon::CCtrlCommon(const HWP_STRING& sCtrlID)
		: CCtrl(sCtrlID), m_arOutMargin{0, 0, 0, 0}, m_arInMargin{0, 0, 0, 0}, m_eTextVerAlign(EVertAlign::TOP)
	{}

	CCtrlCommon::CCtrlCommon(const CCtrlCommon& oCtrlCommon)
		: CCtrl(oCtrlCommon.GetID()), m_arOutMargin{0, 0, 0, 0}, m_arInMargin{0, 0, 0, 0}
	{
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
		: CCtrl(sCtrlID), m_arOutMargin{0, 0, 0, 0}, m_arInMargin{0, 0, 0, 0}
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

		if (nSize > oBuffer.GetDistanceToLastPos(true))
			oBuffer.ReadString(m_sObjDesc, EStringCharacter::UTF16);
	}

	CCtrlCommon::CCtrlCommon(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	    : CCtrl(sCtrlID), m_eTextVerAlign(EVertAlign::TOP), m_arOutMargin{0, 0, 0, 0}, m_arInMargin{0, 0, 0, 0}
	{
		m_nObjInstanceID = std::abs(oNode.GetAttributeInt(L"id"));

		HWP_STRING sType = oNode.GetAttribute(L"textFlow");

		if (L"BOTH_SIDES" == sType)
			m_chTextFlow = 0;
		else if (L"LEFT_ONLY" == sType)
			m_chTextFlow = 1;
		else if (L"RIGHT_ONLY" == sType)
			m_chTextFlow = 2;
		else if (L"LARGEST_ONLY" == sType)
			m_chTextFlow = 3;

		sType = oNode.GetAttribute(L"textWrap");

		if (L"SQUARE" == sType)
			m_eTextWrap = ETextWrap::SQUARE;
		else if (L"TOP_AND_BOTTOM" == sType)
			m_eTextWrap = ETextWrap::TOP_AND_BOTTOM;
		else if (L"BEHIND_TEXT" == sType)
			m_eTextWrap = ETextWrap::BEHIND_TEXT;
		else if (L"IN_FRONT_OF_TEXT" == sType)
			m_eTextWrap = ETextWrap::IN_FRONT_OF_TEXT;

		m_nZOrder = oNode.GetAttributeInt(L"zOrder");

		sType = oNode.GetAttribute(L"numberingType");

		if (L"NONE" == sType)
			m_chNumeringType = 0;
		else if (L"PICTURE" == sType)
			m_chNumeringType = 1;
		else if (L"TABLE" == sType)
			m_chNumeringType = 2;
		else if (L"EQUATION" == sType)
			m_chNumeringType = 3;

		for (CXMLNode& oChild : oNode.GetChilds())
		{
			if (L"hp:sz" == oChild.GetName())
			{
				m_nWidth = oChild.GetAttributeInt(L"width");
				m_eWidthRelTo = ::HWP::GetWidthRelTo(oChild.GetAttribute(L"widthRelTo"));
				m_nHeight = oChild.GetAttributeInt(L"height");
				m_eHeightRelTo = ::HWP::GetHeightRelTo(oChild.GetAttribute(L"heightRelTo"));
			}
			else if (L"hp:pos" == oChild.GetName())
			{
				m_bTreatAsChar = oChild.GetAttributeBool(L"treatAsChar");

				if (m_bTreatAsChar)
					m_bAffectLSpacing = oChild.GetAttributeBool(L"affectLSpacing");
				else
					m_bAllowOverlap = oChild.GetAttributeBool(L"allowOverlap");

				m_eVertRelTo = GetVRelTo(oChild.GetAttribute(L"vertRelTo"));
				m_eHorzRelTo = GetHRelTo(oChild.GetAttribute(L"horzRelTo"));

				if (EVRelTo::PARA == m_eVertRelTo)
					m_bFlowWithText = oChild.GetAttributeBool(L"flowWithText");

				m_eVertAlign = GetVertAlign(oChild.GetAttribute(L"vertAlign"));
				m_eHorzAlign = GetHorzAlign(oChild.GetAttribute(L"horzAlign"));
				m_nVertOffset = oChild.GetAttributeInt(L"vertOffset");
				m_nHorzOffset = oChild.GetAttributeInt(L"horzOffset");
			}
			else if (L"hp:outMargin" == oChild.GetName())
			{
				m_arOutMargin[0] = oChild.GetAttributeInt(L"left");
				m_arOutMargin[1] = oChild.GetAttributeInt(L"right");
				m_arOutMargin[2] = oChild.GetAttributeInt(L"top");
				m_arOutMargin[3] = oChild.GetAttributeInt(L"bottom");
			}
			else if (L"hp:inMargin" == oChild.GetName())
			{
				m_arInMargin[0] = oChild.GetAttributeInt(L"left");
				m_arInMargin[1] = oChild.GetAttributeInt(L"right");
				m_arInMargin[2] = oChild.GetAttributeInt(L"top");
				m_arInMargin[3] = oChild.GetAttributeInt(L"bottom");
			}
			else if (L"hp:caption" == oChild.GetName())
			{
				HWP_STRING sType = oChild.GetAttribute(L"side");

				if (L"LEFT" == sType)
					m_nCaptionAttr = 0b00;
				else if (L"RIGHT" == sType)
					m_nCaptionAttr = 0b01;
				else if (L"TOP" == sType)
					m_nCaptionAttr = 0b10;
				else if (L"BOTTOM" == sType)
					m_nCaptionAttr = 0b11;

				if (oChild.GetAttributeBool(L"fullSz"))
					m_nCaptionAttr |= 0b100;

				m_nCaptionWidth = oChild.GetAttributeInt(L"width");
				m_nCaptionSpacing = oChild.GetAttributeInt(L"gap");
				m_nCaptionMaxW = oChild.GetAttributeInt(L"lastWidth");

				for (CXMLNode& oSubList : oChild.GetChilds(L"hp:subList"))
				{
					for (CXMLNode& oParagraph : oSubList.GetChilds(L"hp:p"))
						m_arCaption.push_back(new CCapParagraph(oParagraph, nVersion));
				}
			}
		}
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

	ECtrlObjectType CCtrlCommon::GetCtrlType() const
	{
		return ECtrlObjectType::Common;
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

	CHWPPargraph* CCtrlCommon::GetLastPara()
	{
		return (!m_arParas.empty()) ? m_arParas.back() : nullptr;
	}

	unsigned int CCtrlCommon::GetCountParagraphs() const
	{
		return (unsigned int)m_arParas.size();
	}

	const CHWPPargraph* CCtrlCommon::GetParagraphs(unsigned int unIndex) const
	{
		if (unIndex >= m_arParas.size())
			return nullptr;

		return m_arParas[unIndex];
	}

	short CCtrlCommon::GetLeftOutMargin() const
	{
		return m_arOutMargin[0];
	}

	short CCtrlCommon::GetTopOutMargin() const
	{
		return m_arOutMargin[2];
	}

	short CCtrlCommon::GetRightOutMargin() const
	{
		return m_arOutMargin[1];
	}

	short CCtrlCommon::GetBottomOutMargin() const
	{
		return m_arOutMargin[3];
	}

	short CCtrlCommon::GetLeftInMargin() const
	{
		return m_arInMargin[0];
	}

	short CCtrlCommon::GetTopInMargin() const
	{
		return m_arInMargin[2];
	}

	short CCtrlCommon::GetRightInMargin() const
	{
		return m_arInMargin[1];
	}

	short CCtrlCommon::GetBottomInMargin() const
	{
		return m_arInMargin[3];
	}

	bool CCtrlCommon::GetTreatAsChar() const
	{
		return m_bTreatAsChar;
	}

	bool CCtrlCommon::GetFlowWithText() const
	{
		return m_bFlowWithText;
	}

	int CCtrlCommon::GetHorzOffset() const
	{
		if (INT_MAX == m_nHorzOffset || INT_MIN == m_nHorzOffset)
			return 0;

		return m_nHorzOffset;
	}

	int CCtrlCommon::GetVertOffset() const
	{
		if (INT_MAX == m_nVertOffset || INT_MIN == m_nVertOffset)
			return 0;

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

	HWP_BYTE CCtrlCommon::GetTextFlow() const
	{
		return m_chTextFlow;
	}

	int CCtrlCommon::GetZOrder() const
	{
		return m_nZOrder;
	}

	bool CCtrlCommon::HaveCaption() const
	{
		return !m_arCaption.empty();
	}

	std::vector<const CCapParagraph*> CCtrlCommon::GetCaptionParas() const
	{
		RETURN_VECTOR_CONST_PTR(CCapParagraph, m_arCaption);
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
