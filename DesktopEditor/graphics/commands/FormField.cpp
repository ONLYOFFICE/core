﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "./FormField.h"
#include "../MetafileToRenderer.h"

CFormFieldInfo::CTextFormFormat::CTextFormFormat()
{
	m_eFormatType = EFormatType::None;
}
void CFormFieldInfo::CTextFormFormat::SetType(const EFormatType& eType)
{
	m_eFormatType = eType;
}
const CFormFieldInfo::EFormatType& CFormFieldInfo::CTextFormFormat::GetType() const
{
	return m_eFormatType;
}
void CFormFieldInfo::CTextFormFormat::AddSymbol(const unsigned int& unCodePoint)
{
	m_vSymbols.push_back(unCodePoint);
}
unsigned int CFormFieldInfo::CTextFormFormat::GetSymbolsCount() const
{
	return m_vSymbols.size();
}
unsigned int CFormFieldInfo::CTextFormFormat::GetSymbol(const unsigned int& unIndex) const
{
	if (unIndex >= m_vSymbols.size())
		return 0;

	return m_vSymbols.at(unIndex);
}
void CFormFieldInfo::CTextFormFormat::SetValue(const std::wstring& wsValue)
{
	m_wsValue = wsValue;
}
const std::wstring& CFormFieldInfo::CTextFormFormat::GetValue() const
{
	return m_wsValue;
}
bool CFormFieldInfo::CTextFormFormat::IsEmpty() const
{
	return (m_eFormatType == EFormatType::None && !m_vSymbols.size());
}
bool CFormFieldInfo::CTextFormFormat::IsMask() const
{
	return (m_eFormatType == EFormatType::Mask);
}
bool CFormFieldInfo::CTextFormFormat::IsDigit() const
{
	return (m_eFormatType == EFormatType::Digit);
}
bool CFormFieldInfo::CTextFormFormat::IsLetter() const
{
	return (m_eFormatType == EFormatType::Letter);
}
bool CFormFieldInfo::CTextFormFormat::IsRegExp() const
{
	return (m_eFormatType == EFormatType::RegExp);
}
const std::wstring& CFormFieldInfo::CTextFormFormat::GetMask() const
{
	return m_wsValue;
}
const std::wstring& CFormFieldInfo::CTextFormFormat::GetRegExp() const
{
	return m_wsValue;
}

//
CFormFieldInfo::CTextFormPr::CTextFormPr()
{
	m_unMaxCharacters = 0;
	m_bComb           = false;
	m_bAutoFit        = false;
	m_bMultiLine      = false;
}
void CFormFieldInfo::CTextFormPr::SetTextValue(const std::wstring& wsValue)
{
	m_wsTextValue = wsValue;
}
const std::wstring& CFormFieldInfo::CTextFormPr::GetTextValue() const
{
	return m_wsTextValue;
}
void CFormFieldInfo::CTextFormPr::SetMaxCharacters(const unsigned int unMax)
{
	m_unMaxCharacters = unMax;
}
unsigned int CFormFieldInfo::CTextFormPr::GetMaxCharacters() const
{
	return m_unMaxCharacters;
}
void CFormFieldInfo::CTextFormPr::SetComb(const bool& bComb)
{
	m_bComb = bComb;
}
bool CFormFieldInfo::CTextFormPr::IsComb() const
{
	return m_bComb;
}
void CFormFieldInfo::CTextFormPr::SetAutoFit(const bool& bAutoFit)
{
	m_bAutoFit = bAutoFit;
}
bool CFormFieldInfo::CTextFormPr::IsAutoFit() const
{
	return m_bAutoFit;
}
void CFormFieldInfo::CTextFormPr::SetMultiLine(const bool& bMultiLine)
{
	m_bMultiLine = bMultiLine;
}
bool CFormFieldInfo::CTextFormPr::IsMultiLine() const
{
	return m_bMultiLine;
}
void CFormFieldInfo::CTextFormPr::SetPlaceHolder(const std::wstring& wsPlaceHolder)
{
	m_wsPlaceHolder = wsPlaceHolder;
}
const std::wstring& CFormFieldInfo::CTextFormPr::GetPlaceHolder() const
{
	return m_wsPlaceHolder;
}
CFormFieldInfo::CTextFormFormat* CFormFieldInfo::CTextFormPr::GetFormat()
{
	return &m_oFormat;
}
const CFormFieldInfo::CTextFormFormat* CFormFieldInfo::CTextFormPr::GetFormatPr() const
{
	return &m_oFormat;
}

//
CFormFieldInfo::CDropDownFormPr::CDropDownFormPr()
{
	m_bEditComboBox = false;
}

void CFormFieldInfo::CDropDownFormPr::SetTextValue(const std::wstring& wsValue)
{
	m_wsTextValue = wsValue;
}
const std::wstring& CFormFieldInfo::CDropDownFormPr::GetTextValue() const
{
	return m_wsTextValue;
}
bool CFormFieldInfo::CDropDownFormPr::IsEditComboBox() const
{
	return m_bEditComboBox;
}
void CFormFieldInfo::CDropDownFormPr::SetEditComboBox(const bool& bEdit)
{
	m_bEditComboBox = bEdit;
}
unsigned int CFormFieldInfo::CDropDownFormPr::GetComboBoxItemsCount() const
{
	return m_vComboBoxItems.size();
}
const std::wstring& CFormFieldInfo::CDropDownFormPr::GetComboBoxItem(const unsigned int& unIndex) const
{
	return m_vComboBoxItems.at(unIndex);
}
void CFormFieldInfo::CDropDownFormPr::AddComboBoxItem(const std::wstring& wsItem)
{
	m_vComboBoxItems.push_back(wsItem);
}
void CFormFieldInfo::CDropDownFormPr::SetPlaceHolder(const std::wstring& wsPlaceHolder)
{
	m_wsPlaceHolder = wsPlaceHolder;
}
const std::wstring& CFormFieldInfo::CDropDownFormPr::GetPlaceHolder() const
{
	return m_wsPlaceHolder;
}

//
CFormFieldInfo::CCheckBoxFormPr::CCheckBoxFormPr()
{
	m_bChecked          = false;
	m_unType            = 0;
	m_unCheckedSymbol   = 0x2612;
	m_unUncheckedSymbol = 0x2610;
}
bool CFormFieldInfo::CCheckBoxFormPr::IsChecked() const
{
	return m_bChecked;
}
void CFormFieldInfo::CCheckBoxFormPr::SetChecked(const bool& bChecked)
{
	m_bChecked = bChecked;
}
unsigned int CFormFieldInfo::CCheckBoxFormPr::GetType() const
{
	return m_unType;
}
void CFormFieldInfo::CCheckBoxFormPr::SetType(const unsigned int& unType)
{
	m_unType = unType;
}
unsigned int CFormFieldInfo::CCheckBoxFormPr::GetCheckedSymbol() const
{
	return m_unCheckedSymbol;
}
void CFormFieldInfo::CCheckBoxFormPr::SetCheckedSymbol(const unsigned int& unCheckedSymbol)
{
	m_unCheckedSymbol = unCheckedSymbol;
}
unsigned int CFormFieldInfo::CCheckBoxFormPr::GetUncheckedSymbol() const
{
	return m_unUncheckedSymbol;
}
void CFormFieldInfo::CCheckBoxFormPr::SetUncheckedSymbol(const unsigned int& unUncheckedSymbol)
{
	m_unUncheckedSymbol = unUncheckedSymbol;
}
void CFormFieldInfo::CCheckBoxFormPr::SetCheckedFont(const std::wstring& wsFontName)
{
	m_wsCheckedFont = wsFontName;
}
const std::wstring& CFormFieldInfo::CCheckBoxFormPr::GetCheckedFontName() const
{
	return m_wsCheckedFont;
}
void CFormFieldInfo::CCheckBoxFormPr::SetUncheckedFont(const std::wstring& wsFontName)
{
	m_wsUncheckedFont = wsFontName;
}
const std::wstring& CFormFieldInfo::CCheckBoxFormPr::GetUncheckedFontName() const
{
	return m_wsUncheckedFont;
}
bool CFormFieldInfo::CCheckBoxFormPr::IsRadioButton() const
{
	return (0 == m_wsGroupKey.length());
}
void CFormFieldInfo::CCheckBoxFormPr::SetGroupKey(const std::wstring& wsGroupKey)
{
	m_wsGroupKey = wsGroupKey;
}
const std::wstring& CFormFieldInfo::CCheckBoxFormPr::GetGroupKey() const
{
	return m_wsGroupKey;
}

//
CFormFieldInfo::CPictureFormPr::CPictureFormPr()
{
	m_eScaleType           = EScaleType::Always;
	m_bRespectBorders      = false;
	m_bConstantProportions = true;
	m_lShiftX              = 500;
	m_lShiftY              = 500;
}
bool CFormFieldInfo::CPictureFormPr::IsConstantProportions() const
{
	return m_bConstantProportions;
}
void CFormFieldInfo::CPictureFormPr::SetConstantProportions(const bool& bConstant)
{
	m_bConstantProportions = bConstant;
}
bool CFormFieldInfo::CPictureFormPr::IsRespectBorders() const
{
	return m_bRespectBorders;
}
void CFormFieldInfo::CPictureFormPr::SetRespectBorders(const bool& bRespect)
{
	m_bRespectBorders = bRespect;
}
CFormFieldInfo::EScaleType CFormFieldInfo::CPictureFormPr::GetScaleType() const
{
	return m_eScaleType;
}
void CFormFieldInfo::CPictureFormPr::SetScaleType(const EScaleType& eType)
{
	m_eScaleType = eType;
}
void CFormFieldInfo::CPictureFormPr::SetShift(const LONG& lShiftX, const LONG& lShiftY)
{
	m_lShiftX = lShiftX;
	m_lShiftY = lShiftY;
}
const LONG& CFormFieldInfo::CPictureFormPr::GetShiftX() const
{
	return m_lShiftX;
}
const LONG& CFormFieldInfo::CPictureFormPr::GetShiftY() const
{
	return m_lShiftY;
}
void CFormFieldInfo::CPictureFormPr::SetPicturePath(const std::wstring& wsPath)
{
	m_wsPicturePath = wsPath;
}
const std::wstring& CFormFieldInfo::CPictureFormPr::GetPicturePath() const
{
	return m_wsPicturePath;
}

//
CFormFieldInfo::CSignatureFormPr::CSignatureFormPr()
{
}
void CFormFieldInfo::CSignatureFormPr::SetName(const std::wstring& wsValue)
{
	m_wsName = wsValue;
}
void CFormFieldInfo::CSignatureFormPr::SetContact(const std::wstring& wsValue)
{
	m_wsContact = wsValue;
}
void CFormFieldInfo::CSignatureFormPr::SetReason(const std::wstring& wsValue)
{
	m_wsReason = wsValue;
}
void CFormFieldInfo::CSignatureFormPr::SetPicturePath(const std::wstring& wsPath)
{
	m_wsPicturePath = wsPath;
}
void CFormFieldInfo::CSignatureFormPr::SetCert(const std::wstring& wsValue)
{
	m_wsCert = wsValue;
}
void CFormFieldInfo::CSignatureFormPr::SetDate(const bool& bDate)
{
	m_bDate = bDate;
}

const std::wstring& CFormFieldInfo::CSignatureFormPr::GetName() const
{
	return m_wsName;
}
const std::wstring& CFormFieldInfo::CSignatureFormPr::GetContact() const
{
	return m_wsContact;
}
const std::wstring& CFormFieldInfo::CSignatureFormPr::GetReason() const
{
	return m_wsReason;
}
const std::wstring& CFormFieldInfo::CSignatureFormPr::GetPicturePath() const
{
	return m_wsPicturePath;
}
const std::wstring& CFormFieldInfo::CSignatureFormPr::GetCert() const
{
	return m_wsCert;
}
bool CFormFieldInfo::CSignatureFormPr::GetDate() const
{
	return m_bDate;
}

//
CFormFieldInfo::CDateTimeFormPr::CDateTimeFormPr()
{
}
void CFormFieldInfo::CDateTimeFormPr::SetValue(const std::wstring& wsValue)
{
	m_wsValue = wsValue;
}
void CFormFieldInfo::CDateTimeFormPr::SetPlaceHolder(const std::wstring& wsValue)
{
	m_wsPlaceHolder = wsValue;
}
void CFormFieldInfo::CDateTimeFormPr::SetFormat(const std::wstring& wsValue)
{
	m_wsFormat = wsValue;
}
const std::wstring& CFormFieldInfo::CDateTimeFormPr::GetValue() const
{
	return m_wsValue;
}
const std::wstring& CFormFieldInfo::CDateTimeFormPr::GetPlaceHolder() const
{
	return m_wsPlaceHolder;
}
const std::wstring& CFormFieldInfo::CDateTimeFormPr::GetFormat() const
{
	return m_wsFormat;
}

//
CFormFieldInfo::CFormFieldInfo() : IAdvancedCommand(AdvancedCommandType::FormField)
{
	m_nType = 0;
	m_dX1 = 0.0;
	m_dY1 = 0.0;
	m_dX2 = 0.0;
	m_dY2 = 0.0;
	m_dBaseLineOffset = 0;

	m_bRequired    = false;
	m_bPlaceHolder = false;

	m_nBorderType = 0;

	// 0 - Right
	// 1 - Left
	// 2 - Center
	// 3 - Justify
	// 4 - Distributed
	m_nJc = 1;

	m_bHaveShd = false;
}
CFormFieldInfo::~CFormFieldInfo()
{
}

void CFormFieldInfo::SetType(int nType)
{
	m_nType = nType;
}
bool CFormFieldInfo::IsValid() const
{
	return (m_nType != 0);
}
// Common
void CFormFieldInfo::SetBounds(const double& dX, const double& dY, const double& dW, const double& dH)
{
	m_dX1 = dX;
	m_dY1 = dY;
	m_dX2 = dW + dX;
	m_dY2 = dH + dY;
}
void CFormFieldInfo::GetBounds(double& dX, double& dY, double& dW, double& dH) const
{
	dX = m_dX1;
	dY = m_dY1;
	dW = m_dX2 - m_dX1;
	dH = m_dY2 - m_dY1;
}
void CFormFieldInfo::SetBaseLineOffset(const double& dOffset)
{
	m_dBaseLineOffset = dOffset;
}
double CFormFieldInfo::GetBaseLineOffset() const
{
	return m_dBaseLineOffset;
}
void CFormFieldInfo::SetKey(const std::wstring& wsKey)
{
	m_wsKey = wsKey;
}
const std::wstring& CFormFieldInfo::GetKey() const
{
	return m_wsKey;
}
void CFormFieldInfo::SetHelpText(const std::wstring& wsHelpText)
{
	m_wsHelpText = wsHelpText;
}
const std::wstring& CFormFieldInfo::GetHelpText() const
{
	return m_wsHelpText;
}
void CFormFieldInfo::SetRequired(const bool& bRequired)
{
	m_bRequired = bRequired;
}
bool CFormFieldInfo::IsRequired() const
{
	return m_bRequired;
}
void CFormFieldInfo::SetPlaceHolder(const bool& bPlaceHolder)
{
	m_bPlaceHolder = bPlaceHolder;
}
bool CFormFieldInfo::IsPlaceHolder() const
{
	return m_bPlaceHolder;
}
void CFormFieldInfo::SetBorder(const int& nType, const double& dSize, const unsigned char& unR, const unsigned char& unG, const unsigned char& unB, const unsigned char& unA)
{
	m_nBorderType = nType;
	m_dBorderSize = dSize;
	m_lBorderColor =  (((LONG)(unA << 24)) & 0xFFFFFF) | (((LONG)(unR << 16)) & 0xFFFFFF) | (((LONG)(unG << 8)) & 0xFFFFFF) | (LONG)(unB);
}
bool CFormFieldInfo::HaveBorder() const
{
	return (0 != m_nBorderType);
}
double CFormFieldInfo::GetBorderSize() const
{
	return m_dBorderSize;
}
void CFormFieldInfo::GetBorderColor(unsigned char& unR, unsigned char& unG, unsigned char& unB, unsigned char& unA) const
{
	unA = ((m_lBorderColor >> 24) & 0xFF);
	unR = ((m_lBorderColor >> 16) & 0xFF);
	unG = ((m_lBorderColor >>  8) & 0xFF);
	unB = ((m_lBorderColor)       & 0xFF);
}
bool CFormFieldInfo::HaveShd() const
{
	return m_bHaveShd;
}
void CFormFieldInfo::SetShd(const unsigned char& unR, const unsigned char& unG, const unsigned char& unB, const unsigned char& unA)
{
	m_bHaveShd  = true;
	m_lShdColor =  (((LONG)(unA << 24)) & 0xFFFFFF) | (((LONG)(unR << 16)) & 0xFFFFFF) | (((LONG)(unG << 8)) & 0xFFFFFF) | (LONG)(unB);
}
void CFormFieldInfo::GetShdColor(unsigned char& unR, unsigned char& unG, unsigned char& unB, unsigned char& unA) const
{
	unA = ((m_lShdColor >> 24) & 0xFF);
	unR = ((m_lShdColor >> 16) & 0xFF);
	unG = ((m_lShdColor >>  8) & 0xFF);
	unB = ((m_lShdColor)       & 0xFF);
}
void CFormFieldInfo::SetJc(const unsigned char& unJc)
{
	m_nJc = unJc;
}
BYTE CFormFieldInfo::GetJc() const
{
	return m_nJc;
}

bool CFormFieldInfo::IsTextField() const
{
	return (m_nType == 1);
}
bool CFormFieldInfo::IsDropDownList() const
{
	return (m_nType == 2);
}
bool CFormFieldInfo::IsCheckBox() const
{
	return (m_nType == 3);
}
bool CFormFieldInfo::IsPicture() const
{
	return (m_nType == 4);
}
bool CFormFieldInfo::IsSignature() const
{
	return (m_nType == 5);
}
bool CFormFieldInfo::IsDateTime() const
{
	return (m_nType == 6);
};
CFormFieldInfo::CTextFormPr* CFormFieldInfo::GetTextFormPr()
{
	return &m_oTextPr;
}
const CFormFieldInfo::CTextFormPr* CFormFieldInfo::GetTextPr() const
{
	return &m_oTextPr;
}
CFormFieldInfo::CDropDownFormPr* CFormFieldInfo::GetDropDownFormPr()
{
	return &m_oDropDownPr;
}
const CFormFieldInfo::CDropDownFormPr* CFormFieldInfo::GetDropDownPr() const
{
	return &m_oDropDownPr;
}
CFormFieldInfo::CCheckBoxFormPr* CFormFieldInfo::GetCheckBoxFormPr()
{
	return &m_oCheckBoxPr;
}
const CFormFieldInfo::CCheckBoxFormPr* CFormFieldInfo::GetCheckBoxPr() const
{
	return &m_oCheckBoxPr;
}
CFormFieldInfo::CPictureFormPr* CFormFieldInfo::GetPictureFormPr()
{
	return &m_oPicturePr;
}
const CFormFieldInfo::CPictureFormPr* CFormFieldInfo::GetPicturePr() const
{
	return &m_oPicturePr;
}
CFormFieldInfo::CSignatureFormPr* CFormFieldInfo::GetSignatureFormPr()
{
	return &m_oSignaturePr;
}
const CFormFieldInfo::CSignatureFormPr* CFormFieldInfo::GetSignaturePr() const
{
	return &m_oSignaturePr;
}
CFormFieldInfo::CDateTimeFormPr* CFormFieldInfo::GetDateTimeFormPr()
{
	return &m_oDateTimePr;
}
const CFormFieldInfo::CDateTimeFormPr* CFormFieldInfo::GetDateTimePr() const
{
	return &m_oDateTimePr;
}

bool CFormFieldInfo::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector)
{
	double dX = pReader->ReadDouble();
	double dY = pReader->ReadDouble();
	double dW = pReader->ReadDouble();
	double dH = pReader->ReadDouble();

	SetBounds(dX, dY, dW, dH);
	SetBaseLineOffset(pReader->ReadDouble());

	int nFlags = pReader->ReadInt();

	if (nFlags & (1 << 0))
		SetKey(pReader->ReadString());

	if (nFlags & (1 << 1))
		SetHelpText(pReader->ReadString());

	SetRequired(nFlags & (1 << 2));
	SetPlaceHolder(nFlags & (1 << 3));

	if (nFlags & (1 << 6))
	{
		int nBorderType = pReader->ReadInt();
		double dBorderSize = pReader->ReadDouble();
		unsigned char unR = pReader->ReadByte();
		unsigned char unG = pReader->ReadByte();
		unsigned char unB = pReader->ReadByte();
		unsigned char unA = pReader->ReadByte();

		SetBorder(nBorderType, dBorderSize, unR, unG, unB, unA);
	}

	if (nFlags & (1 << 9))
	{
		unsigned char unR = pReader->ReadByte();
		unsigned char unG = pReader->ReadByte();
		unsigned char unB = pReader->ReadByte();
		unsigned char unA = pReader->ReadByte();
		SetShd(unR, unG, unB, unA);
	}

	if (nFlags & (1 << 10))
	{
		SetJc(pReader->ReadByte());
	}

	SetType(pReader->ReadInt());

	if (IsTextField())
	{
		CFormFieldInfo::CTextFormPr* pPr = GetTextFormPr();
		pPr->SetComb(nFlags & (1 << 20));

		if (nFlags & (1 << 21))
			pPr->SetMaxCharacters(pReader->ReadInt());

		if (nFlags & (1 << 22))
			pPr->SetTextValue(pReader->ReadString());

		pPr->SetMultiLine(nFlags & (1 << 23));
		pPr->SetAutoFit(nFlags & (1 << 24));

		if (nFlags & (1 << 25))
			pPr->SetPlaceHolder(pReader->ReadString());

		if (nFlags & (1 << 26))
		{
			CFormFieldInfo::CTextFormFormat* pFormat = pPr->GetFormat();
			pFormat->SetType((CFormFieldInfo::EFormatType)pReader->ReadByte());

			unsigned int unSymbolsCount = pReader->ReadInt();
			for (unsigned int unSymbolIndex = 0; unSymbolIndex < unSymbolsCount; ++unSymbolIndex)
			{
				pFormat->AddSymbol(pReader->ReadInt());
			}

			pFormat->SetValue(pReader->ReadString());
		}
	}
	else if (IsDropDownList())
	{
		CFormFieldInfo::CDropDownFormPr* pPr = GetDropDownFormPr();
		pPr->SetEditComboBox(nFlags & (1 << 20));

		int nItemsCount = pReader->ReadInt();
		for (int nIndex = 0; nIndex < nItemsCount; ++nIndex)
		{
			pPr->AddComboBoxItem(pReader->ReadString());
		}

		int nSelectedIndex = pReader->ReadInt();

		if (nFlags & (1 << 22))
			pPr->SetTextValue(pReader->ReadString());

		if (nFlags & (1 << 23))
			pPr->SetPlaceHolder(pReader->ReadString());
	}
	else if (IsCheckBox())
	{
		CFormFieldInfo::CCheckBoxFormPr* pPr = GetCheckBoxFormPr();
		pPr->SetChecked(nFlags & (1 << 20));
		pPr->SetType(pReader->ReadInt());
		pPr->SetCheckedSymbol(pReader->ReadInt());
		pPr->SetCheckedFont(pReader->ReadString());
		pPr->SetUncheckedSymbol(pReader->ReadInt());
		pPr->SetUncheckedFont(pReader->ReadString());

		if (nFlags & (1 << 21))
			pPr->SetGroupKey(pReader->ReadString());
	}
	else if (IsPicture())
	{
		CFormFieldInfo::CPictureFormPr* pPr = GetPictureFormPr();
		pPr->SetConstantProportions(nFlags & (1 << 20));
		pPr->SetRespectBorders(nFlags & (1 << 21));
		pPr->SetScaleType(CFormFieldInfo::EScaleType((nFlags >> 24) & 0xF));
		LONG lShiftX = pReader->ReadInt();
		LONG lShiftY = pReader->ReadInt();
		pPr->SetShift(lShiftX, lShiftY);

		if (nFlags & (1 << 22))
			pPr->SetPicturePath(pCorrector->GetImagePath(pReader->ReadString()));
	}
	else if (IsSignature())
	{
		CFormFieldInfo::CSignatureFormPr* pPr = GetSignatureFormPr();

			   // Поля Настройки подписи
			   // Сведения о подписывающем

			   // Имя
		if (nFlags & (1 << 20))
			pPr->SetName(pReader->ReadString());

			   // Должность Игнорируется

			   // Адрес электронной почты
		if (nFlags & (1 << 21))
			pPr->SetContact(pReader->ReadString());

			   // Инструкция для подписывающего Игнорируется

			   // Показывать дату подписи в строке подписи
		pPr->SetDate(nFlags & (1 << 22));

			   // Цель подписания документа (причина)
		if (nFlags & (1 << 23))
			pPr->SetReason(pReader->ReadString());

			   // Картинка
		if (nFlags & (1 << 24))
			pPr->SetPicturePath(pCorrector->GetImagePath(pReader->ReadString()));

			   // Необходимо передать сертификат, пароль, ключ, пароль ключа
		if (nFlags & (1 << 25))
			pPr->SetCert(pReader->ReadString());
	}
	else if (IsDateTime())
	{
		CFormFieldInfo::CDateTimeFormPr* pPr = GetDateTimeFormPr();

		if (nFlags & (1 << 22))
			pPr->SetValue(pReader->ReadString());

		if (nFlags & (1 << 25))
			pPr->SetPlaceHolder(pReader->ReadString());

		if (nFlags & (1 << 26))
			pPr->SetFormat(pReader->ReadString());
	}

	return IsValid();
}
