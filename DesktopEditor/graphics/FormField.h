/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#ifndef _BUILD_FORMFIELD_H_
#define _BUILD_FORMFIELD_H_

#include "config.h"
#include "IRenderer.h"

class GRAPHICS_DECL CFormFieldInfo : public IFormField
{
public:
	enum EScaleType
	{
		Always  = 0x00,
		Bigger  = 0x01,
		Smaller = 0x02,
		Never   = 0x03
	};

	enum EFormatType
	{
		None   = 0,
		Digit  = 1,
		Letter = 2,
		Mask   = 3,
		RegExp = 4
	};

	class GRAPHICS_DECL CTextFormFormat
	{
	public:
		CTextFormFormat();

		void SetType(const EFormatType& eType);
		const EFormatType& GetType() const;

		void AddSymbol(const unsigned int& unCodePoint);
		unsigned int GetSymbolsCount() const;
		unsigned int GetSymbol(const unsigned int& unIndex) const;
		void SetValue(const std::wstring& wsValue);
		const std::wstring& GetValue() const;

		bool IsEmpty() const;
		bool IsMask() const;
		bool IsDigit() const;
		bool IsLetter() const;
		bool IsRegExp() const;

		const std::wstring& GetMask() const;
		const std::wstring& GetRegExp() const;

	private:

		EFormatType               m_eFormatType;
		std::vector<unsigned int> m_vSymbols;
		std::wstring              m_wsValue; // mask or regexp
	};

	class GRAPHICS_DECL CTextFormPr
	{
	public:
		CTextFormPr();

		void SetTextValue(const std::wstring& wsValue);
		const std::wstring& GetTextValue() const;

		void SetMaxCharacters(const unsigned int unMax);
		unsigned int GetMaxCharacters() const;

		void SetComb(const bool& bComb);
		bool IsComb() const;

		void SetAutoFit(const bool& bAutoFit);
		bool IsAutoFit() const;

		void SetMultiLine(const bool& bMultiLine);
		bool IsMultiLine() const;

		void SetPlaceHolder(const std::wstring& wsPlaceHolder);
		const std::wstring& GetPlaceHolder() const;

		CTextFormFormat* GetFormat();
		const CTextFormFormat* GetFormatPr() const;

	private:
		std::wstring    m_wsTextValue;
		unsigned int    m_unMaxCharacters;
		bool            m_bComb;
		bool            m_bAutoFit;
		bool            m_bMultiLine;
		std::wstring    m_wsPlaceHolder;
		CTextFormFormat m_oFormat;
	};

	class GRAPHICS_DECL CDropDownFormPr
	{
	public:
		CDropDownFormPr();

		void SetTextValue(const std::wstring& wsValue);
		const std::wstring& GetTextValue() const;

		bool IsEditComboBox() const;
		void SetEditComboBox(const bool& bEdit);
		unsigned int GetComboBoxItemsCount() const;
		const std::wstring& GetComboBoxItem(const unsigned int& unIndex) const;
		void AddComboBoxItem(const std::wstring& wsItem);

		void SetPlaceHolder(const std::wstring& wsPlaceHolder);
		const std::wstring& GetPlaceHolder() const;

	private:

		std::wstring              m_wsTextValue;
		bool                      m_bEditComboBox;
		std::vector<std::wstring> m_vComboBoxItems;
		std::wstring              m_wsPlaceHolder;
	};

	class GRAPHICS_DECL CCheckBoxFormPr
	{
	public:
		CCheckBoxFormPr();

		bool IsChecked() const;
		void SetChecked(const bool& bChecked);

		unsigned int GetType() const;
		void SetType(const unsigned int& unType);

		unsigned int GetCheckedSymbol() const;
		void SetCheckedSymbol(const unsigned int& unCheckedSymbol);

		unsigned int GetUncheckedSymbol() const;
		void SetUncheckedSymbol(const unsigned int& unUncheckedSymbol);

		void SetCheckedFont(const std::wstring& wsFontName);
		const std::wstring& GetCheckedFontName() const;

		void SetUncheckedFont(const std::wstring& wsFontName);
		const std::wstring& GetUncheckedFontName() const;

		bool IsRadioButton() const;

		void SetGroupKey(const std::wstring& wsGroupKey);
		const std::wstring& GetGroupKey() const;

	private:
		bool         m_bChecked;
		unsigned int m_unType;
		unsigned int m_unCheckedSymbol;
		unsigned int m_unUncheckedSymbol;
		std::wstring m_wsCheckedFont;
		std::wstring m_wsUncheckedFont;
		std::wstring m_wsGroupKey;
	};

	class GRAPHICS_DECL CPictureFormPr
	{
	public:
		CPictureFormPr();

		bool IsConstantProportions() const;
		void SetConstantProportions(const bool& bConstant);

		bool IsRespectBorders() const;
		void SetRespectBorders(const bool& bRespect);

		EScaleType GetScaleType() const;
		void SetScaleType(const EScaleType& eType);

		void SetShift(const LONG& lShiftX, const LONG& lShiftY);
		const LONG& GetShiftX() const;
		const LONG& GetShiftY() const;

		void SetPicturePath(const std::wstring& wsPath);
		const std::wstring& GetPicturePath() const;

	private:
		EScaleType   m_eScaleType;
		bool         m_bRespectBorders;
		bool         m_bConstantProportions;
		LONG         m_lShiftX;
		LONG         m_lShiftY;
		std::wstring m_wsPicturePath;
	};

	class GRAPHICS_DECL CSignatureFormPr
	{
	public:
		CSignatureFormPr();

		void SetName(const std::wstring& wsValue);
		void SetContact(const std::wstring& wsValue);
		void SetReason(const std::wstring& wsValue);
		void SetPicturePath(const std::wstring& wsPath);
		void SetCert(const std::wstring& wsValue);
		void SetDate(const bool& bDate);

		const std::wstring& GetName() const;
		const std::wstring& GetContact() const;
		const std::wstring& GetReason() const;
		const std::wstring& GetPicturePath() const;
		const std::wstring& GetCert() const;
		bool GetDate() const;

	private:
		std::wstring m_wsName;
		std::wstring m_wsContact;
		std::wstring m_wsReason;
		std::wstring m_wsPicturePath;
		std::wstring m_wsCert;
		bool         m_bDate;
	};

public:
	CFormFieldInfo();
	virtual ~CFormFieldInfo();

	void SetType(int nType);
	bool IsValid() const;

	// Common
	void SetBounds(const double& dX, const double& dY, const double& dW, const double& dH);
	void GetBounds(double& dX, double& dY, double& dW, double& dH) const;

	void SetBaseLineOffset(const double& dOffset);
	double GetBaseLineOffset() const;

	void SetKey(const std::wstring& wsKey);
	const std::wstring& GetKey() const;

	void SetHelpText(const std::wstring& wsHelpText);
	const std::wstring& GetHelpText() const;

	void SetRequired(const bool& bRequired);
	bool IsRequired() const;

	void SetPlaceHolder(const bool& bPlaceHolder);
	bool IsPlaceHolder() const;

	void SetBorder(const int& nType, const double& dSize, const unsigned char& unR, const unsigned char& unG, const unsigned char& unB, const unsigned char& unA);
	bool HaveBorder() const;
	double GetBorderSize() const;
	void GetBorderColor(unsigned char& unR, unsigned char& unG, unsigned char& unB, unsigned char& unA) const;

	bool HaveShd() const;
	void SetShd(const unsigned char& unR, const unsigned char& unG, const unsigned char& unB, const unsigned char& unA);
	void GetShdColor(unsigned char& unR, unsigned char& unG, unsigned char& unB, unsigned char& unA) const;

	void SetJc(const unsigned char& unJc);
	BYTE GetJc() const;

	bool IsTextField() const;
	bool IsDropDownList() const;
	bool IsCheckBox() const;
	bool IsPicture() const;
	bool IsSignature() const;

	CTextFormPr* GetTextFormPr();
	const CTextFormPr* GetTextPr() const;

	CDropDownFormPr* GetDropDownFormPr();
	const CDropDownFormPr* GetDropDownPr() const;

	CCheckBoxFormPr* GetCheckBoxFormPr();
	const CCheckBoxFormPr* GetCheckBoxPr() const;

	CPictureFormPr* GetPictureFormPr();
	const CPictureFormPr* GetPicturePr() const;

	CSignatureFormPr* GetSignatureFormPr();
	const CSignatureFormPr* GetSignaturePr() const;

private:
	int          m_nType;
	double       m_dX;
	double       m_dY;
	double       m_dW;
	double       m_dH;
	double       m_dBaseLineOffset;
	std::wstring m_wsKey;
	std::wstring m_wsHelpText;
	bool         m_bRequired;
	bool         m_bPlaceHolder;
	int          m_nBorderType;
	double       m_dBorderSize;
	LONG         m_lBorderColor;
	bool         m_bHaveShd;
	LONG         m_lShdColor;
	BYTE         m_nJc;

	CTextFormPr      m_oTextPr;
	CDropDownFormPr  m_oDropDownPr;
	CCheckBoxFormPr  m_oCheckBoxPr;
	CPictureFormPr   m_oPicturePr;
	CSignatureFormPr m_oSignaturePr;
};

#endif // _BUILD_FORMFIELD_H_
