#ifndef _PDF_WRITER_SRC_FIELD_H
#define _PDF_WRITER_SRC_FIELD_H

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

#include "Objects.h"
#include "Types.h"
#include "Annotation.h"

namespace PdfWriter
{
	class CDocument;
	class CPage;
	class CAnnotAppearance;
	class CAnnotAppearanceObject;
	class CResourcesDict;
	class CFontDict;
	class CRadioGroupField;

	class CFieldBase : public CDictObject
	{
	public:
		CFieldBase(CXref* pXref, CDocument* pDocument);

		void SetReadOnlyFlag(bool isReadOnly);
		void SetRequiredFlag(bool isRequired);
		void SetNoExportFlag(bool isNoExport);
		void AddPageRect(CPage* pPage, const TRect& oRect);
		virtual void SetFieldName(const std::string& sName);
		virtual void SetFieldName(const std::wstring& wsName);
		void SetFieldHint(const std::wstring& wsHint);
		TRect& GetRect();
		CResourcesDict* GetResourcesDict();
		void SetTextAppearance(const std::wstring& wsValue, unsigned char* pCodes, unsigned int unCount, CFontDict* pFont, const TRgb& oColor, const double& dAlpha, double dFontSize = 10.0, double dX = 0.0, double dY = 0.0, double* pShifts = NULL, unsigned int unShiftsCount = 0);
		void SetTextValue(const std::wstring& wsValue);
		void SetFieldBorder(const EBorderSubtype& eSubtype, const TRgb& oColor, const double& fWidth, const unsigned short& nDashOn, const unsigned short& nDashOff, const unsigned short& nDashPhase);
		bool HaveBorder() const;
		const double& GetBorderSize() const;
		const TRgb& GetBorderColor() const;

	protected:

		void SetFlag(bool isFlag, int nBitPosition);		

	protected:

		CXref*     m_pXref;
		TRect      m_oRect;
		CDocument* m_pDocument;
		int        m_nBorderType;
		double     m_dBorderSize;
		TRgb       m_oBorderColor;
	};

	class CTextField : public CFieldBase
	{
	public:
		CTextField(CXref* pXRef, CDocument* pDocument);

		void SetMultilineFlag(bool isMultiLine);
		void SetPasswordFlag(bool isPassword);
		void SetFileSelectFlag(bool isFileSelect);
		void SetDoNotSpellCheckFlag(bool isDoNotSpellCheck);
		void SetDoNotScrollFlag(bool isDoNotScroll);
		void SetCombFlag(bool isFlag);
		void SetRichTextFlag(bool isRichText);

		void SetMaxLen(int nMaxLen);	

		int  GetMaxLen() const;
		bool IsCombFlag() const;

	private:
	};

	class CChoiceField : public CFieldBase
	{
	public:
		CChoiceField(CXref* pXref, CDocument* pDocument);

		void SetComboFlag(const bool& isCombo);
		void SetEditFlag(const bool& isEdit);
		void SetSortFlag(const bool& isSort);
		void SetMultiSelectFlag(const bool& isMultiSelect);
		void SetDoNotSpellCheck(const bool& isDoNotSpellCheck);

		void AddOption(const std::wstring& wsOption);

	private:

		CArrayObject* m_pOpt;

	};

	class CCheckBoxField : public CFieldBase
	{
	public:
		CCheckBoxField(CXref* pXref, CDocument* pDocument, CRadioGroupField* pGroup = NULL);

		void SetAppearance(const std::wstring& wsYesValue, unsigned char* pYesCodes, unsigned int unYesCount, CFontDict* pYesFont,
						   const std::wstring& wsOffValue, unsigned char* pOffCodes, unsigned int unOffCount, CFontDict* pOffFont,
						   const TRgb& oColor, const double& dAlpha, double dFontSize = 10.0, double dX = 0.0, double dY = 0.0);
		void SetValue(const bool& isYes);

	private:

		CRadioGroupField* m_pGroup;
	};

	class CRadioGroupField : public CFieldBase
	{
	public:
		CRadioGroupField(CXref* pXref, CDocument* pDocument);

		const std::string& GetGroupName() const;
		void SetGroupName(const std::string& sGroupName);
		CCheckBoxField* CreateKid();
		virtual void SetFieldName(const std::wstring& wsName) override;
		const std::wstring& GetFieldName() const;

	private:

		std::string   m_sGroupName;
		CArrayObject* m_pKids;
		std::wstring  m_wsFieldName;
	};

	class CPictureField : public CFieldBase
	{
	public:
		CPictureField(CXref* pXref, CDocument* pDocument);
		void SetAppearance();

	private:
	};

	class CAnnotAppearance : public CDictObject
	{
	public:
		CAnnotAppearance(CXref* pXRef, CFieldBase* pField);

		CAnnotAppearanceObject* GetNormal();
		CAnnotAppearanceObject* GetRollover();
		CAnnotAppearanceObject* GetDown();

	private:

		CXref*                  m_pXref;
		CAnnotAppearanceObject* m_pNormal;
		CAnnotAppearanceObject* m_pRollover;
		CAnnotAppearanceObject* m_pDown;
		CFieldBase*             m_pField;
	};

	class CCheckBoxAnnotAppearance : public CDictObject
	{
	public:
		CCheckBoxAnnotAppearance(CXref* pXref, CFieldBase* pField);
		
		CAnnotAppearanceObject* GetYes();
		CAnnotAppearanceObject* GetOff();

	private:

		CXref*                  m_pXref;
		CFieldBase*             m_pField;
		CAnnotAppearanceObject* m_pYes;
		CAnnotAppearanceObject* m_pOff;

	};

	class CAnnotAppearanceObject : public CDictObject
	{
	public:
		CAnnotAppearanceObject(CXref* pXRef, CFieldBase* pField);
		void DrawSimpleText(const std::wstring& wsText, unsigned char* pCodes, unsigned int unCount, const char* sFont, double dFontSize = 10.0, double dX = 0.0, double dY = 0.0, double dR = 0.0, double dG = 0.0, double dB = 0.0, const char* sExtGrStateName = NULL, double dW = 1.0, double dH = 1.0, double* pShifts = NULL, unsigned int unShiftsCount = 0);
		void DrawPicturePlaceholder();

	private:

		CXref*      m_pXref;
		CStream*    m_pStream;
		CFieldBase* m_pField;
	};

}

#endif // _PDF_WRITER_SRC_FIELD_H
