/*
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
#ifndef _PDF_WRITER_SRC_FIELD_H
#define _PDF_WRITER_SRC_FIELD_H

#include "Objects.h"
#include "Types.h"
#include "../../DesktopEditor/graphics/commands/FormField.h"

namespace PdfWriter
{
	class CDocument;
	class CPage;
	class CAnnotAppearance;
	class CAnnotAppearanceObject;
	class CResourcesDict;
	class CFontDict;
	class CRadioGroupField;
	class CImageDict;
	class CFontCidTrueType;
	class CSignatureDict;
	class CAnnotation;

	enum EBorderSubtype
	{
		border_subtype_Solid,
		border_subtype_Beveled,
		border_subtype_Dashed,
		border_subtype_Inset,
		border_subtype_Underlined
	};
	enum EAnnotType
	{
		AnnotUnknown        = -1,
		AnnotText           = 0,
		AnnotLink           = 1,
		AnnotSound          = 2,
		AnnotFreeText       = 3,
		AnnotStamp          = 4,
		AnnotSquare         = 5,
		AnnotCircle         = 6,
		AnnotStrikeOut      = 7,
		AnnotHighLight      = 8,
		AnnotUnderline      = 9,
		AnnotInk            = 10,
		AnnotFileAttachment = 11,
		AnnotPopup          = 12,
		AnnotLine           = 13,
		AnnotSquiggly       = 14,
		AnnotPolygon        = 15,
		AnnotPolyLine       = 16,
		AnnotCaret          = 17,
		AnnotWidget         = 18,
		AnnotRedact         = 25
	};
	enum EWidgetType
	{
		WidgetUnknown     = 26,
		WidgetPushbutton  = 27,
		WidgetRadiobutton = 28,
		WidgetCheckbox    = 29,
		WidgetText        = 30,
		WidgetCombobox    = 31,
		WidgetListbox     = 32,
		WisgetSignature   = 33
	};
	enum EAnnotHighlightMode
	{
		AnnotNoHighlight = 0,
		AnnotInvertBox,
		AnnotInvertBorder,
		AnnotDownAppearance,
		AnnotHighlightModeEOF
	};
	enum EAnnotIcon
	{
		AnnotIconComment      = 0,
		AnnotIconKey          = 1,
		AnnotIconNote         = 2,
		AnnotIconHelp         = 3,
		AnnotIconNewParagraph = 4,
		AnnotIconParagraph    = 5,
		AnnotIconInsert       = 6,

		AnnotIconMin          = 0,
		AnnotIconMax          = 6
	};

	class CFieldBase : public CDictObject
	{
	public:

		enum EFieldAlignType
		{
			Left   = 0,
			Center = 1,
			Right  = 2
		};

	public:
		CFieldBase(CXref* pXref, CDocument* pDocument);

		void SetReadOnlyFlag(bool isReadOnly);
		void SetRequiredFlag(bool isRequired);
		void SetNoExportFlag(bool isNoExport);
		void AddPageRect(CPage* pPage, const TRect& oRect);
		virtual void SetFieldName(const std::string& sName, bool isSkipCheck = false);
		virtual void SetFieldName(const std::wstring& wsName, bool isSkipCheck = false);
		void ClearKidRecords();
		void SetFieldHint(const std::wstring& wsHint);
		TRect& GetRect();
		virtual CResourcesDict* GetResourcesDict();
		void SetDefaultAppearance(CFontDict* pFont, const double& dFontSize, const TRgb& oColor);
		void SetTextAppearance(const std::wstring& wsValue, unsigned short* pCodes, unsigned int unCount, CFontDict* pFont, const TRgb& oColor, const double& dAlpha, double dFontSize = 10.0, double dX = 0.0, double dY = 0.0, CFontCidTrueType** ppFonts = NULL, double* pShifts = NULL);
		void StartTextAppearance(CFontDict* pFont, const double& dFontSize, const TRgb& oColor, const double& dAlpha);
		void AddLineToTextAppearance(const double& dX, const double& dY, unsigned short* pCodes, const unsigned int& unCodesCount, CFontCidTrueType** ppFonts = NULL, const double* pShifts = NULL);
		void EndTextAppearance();
		void SetTextValue(const std::wstring& wsValue);
		void SetFieldBorder(const EBorderSubtype& eSubtype, const TRgb& oColor, const double& fWidth, const unsigned short& nDashOn, const unsigned short& nDashOff, const unsigned short& nDashPhase);
		bool HaveBorder() const;
		const double& GetBorderSize() const;
		const TRgb& GetBorderColor() const;
		const bool& IsAutoFit() const;
		void SetAutoFit(const bool& isAutoFit);
		void SetParent(CFieldBase* pParent);
		void AddKid(CFieldBase* pChild);
		int GetKidsCount() const;
		bool HaveShd() const;
		void SetShd(const TRgb& oColor);
		const TRgb& GetShdColor() const;
		int GetFieldFlag() const;
		const char* GetFieldType() const;
		void SetAlign(const EFieldAlignType& eType);
		virtual void SetPlaceHolderText(const std::wstring& wsText, const TRgb& oNormalColor, const TRgb& oPlaceHolderColor);
		void UpdateKidsPlaceHolder();
		const std::wstring& GetPlaceHolderText();
		const TRgb& GetNormalColor();
		const TRgb& GetPlaceHolderColor();
		void SetFormat(const CFormFieldInfo::CTextFormFormat* pFormat);
		CFontDict* GetFont();

	protected:

		virtual void SetPlaceHolderText(const std::vector<std::wstring>& vPlaceHolders, const std::vector<TRgb>& vNormalColors, const std::vector<TRgb>& vPlaceHolderColors);
		void SetFlag(bool isFlag, int nBitPosition);
		CDictObject* GetAA();
		void AddScriptToAA(const std::string& sKey, const std::string& sScript, CDictObject* pAA = NULL);

	protected:

		CXref*            m_pXref;
		TRect             m_oRect;
		CDocument*        m_pDocument;
		int               m_nBorderType;
		double            m_dBorderSize;
		TRgb              m_oBorderColor;
		bool              m_bAutoFit;
		bool              m_bShd;
		TRgb              m_oShdColor;
		CDictObject*      m_pMK;
		CFieldBase*       m_pParent;
		CArrayObject*     m_pKids;
		CAnnotAppearance* m_pAppearance;
		std::wstring      m_wsPlaceHolderText;
		TRgb              m_oNormalColor;
		TRgb              m_oPlaceHolderColor;
		CDictObject*      m_pFocus;
		CDictObject*      m_pBlur;
		CFontDict*        m_pFont;
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

		bool m_bAutoFit;
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

		void AddOption(const std::wstring& wsOption, const bool& bPushBack = true);
		void SetSelectedIndex(const unsigned int& unIndex);
		void UpdateSelectedIndexToParent();
		virtual void SetPlaceHolderText(const std::wstring& wsText, const TRgb& oNormalColor, const TRgb& oPlaceHolderColor);

	private:

		CArrayObject* m_pOpt;

	};

	class CCheckBoxField : public CFieldBase
	{
	public:
		CCheckBoxField(CXref* pXref, CDocument* pDocument, CRadioGroupField* pGroup = NULL, const char* sYesName = NULL);

		void SetAppearance(const std::wstring& wsYesValue, unsigned short* pYesCodes, unsigned int unYesCount, CFontDict* pYesFont,
						   const std::wstring& wsOffValue, unsigned short* pOffCodes, unsigned int unOffCount, CFontDict* pOffFont,
						   const TRgb& oColor, const double& dAlpha, double dFontSize = 10.0, double dX = 0.0, double dY = 0.0);
		void SetAppearance(const int& nType, const TRgb& oColor, const double& dAlpha, double dFontSize = 10.0, double dX = 0.0, double dY = 0.0);
		void SetValue(const bool& isYes);		

	private:

		void SetDefaultCheckBoxAppearance(const std::string& sText);

	private:

		CRadioGroupField* m_pGroup;
		std::string       m_sYesName;
	};

	class CRadioGroupField : public CFieldBase
	{
	public:
		CRadioGroupField(CXref* pXref, CDocument* pDocument);

		CCheckBoxField* CreateKid(const wchar_t* wsChoiceName = NULL);
		virtual void SetFieldName(const std::wstring& wsName, bool isSkipCheck = false) override;
		const std::wstring& GetFieldName() const;

	private:

		CArrayObject* m_pKids;
		std::wstring  m_wsFieldName;
	};

	class CPictureField : public CFieldBase
	{
	public:

		enum EScaleType
		{
			Always  = 0x00,
			Bigger  = 0x01,
			Smaller = 0x02,
			Never   = 0x03
		};

	public:
		CPictureField(CXref* pXref, CDocument* pDocument);
		virtual void SetFieldName(const std::string& sName, bool isSkipCheck = false);
		virtual void SetFieldName(const std::wstring& wsName, bool isSkipCheck = false);
		void SetAppearance(CImageDict* pImage = NULL);
		void SetScaleType(const EScaleType& eType);
		void SetConstantProportions(const bool& bConstant);
		void SetRespectBorders(const bool& bRespectBorders);
		void SetShift(const double& dX, const double& dY);
		virtual CResourcesDict* GetResourcesDict();

	private:
		CDictObject*    m_pIF;
		EScaleType      m_eScaleType;
		bool            m_bConstantProportions;
		bool            m_bRespectBorders;
		double          m_dShiftX;
		double          m_dShiftY;
		CResourcesDict* m_pResources;
	};

	class CSignatureField : public CFieldBase
	{
	public:
		CSignatureField(CXref* pXref, CDocument* pDocument);
		CSignatureDict* GetSignatureDict () { return m_pSig; }

		void SetName(const std::wstring& wsValue);
		void SetContact(const std::wstring& wsValue);
		void SetReason(const std::wstring& wsValue);
		void SetPicture(const std::wstring& wsPath);
		void SetCert();
		void SetDate(bool bDate);
		void SetAppearance(CImageDict* pImage = NULL);
		virtual CResourcesDict* GetResourcesDict();

	private:
		CSignatureDict* m_pSig; // Словарь сигнатур
		CResourcesDict* m_pResources;
	};
	
	class CDateTimeField : public CFieldBase
	{
	public:
		CDateTimeField(CXref* pXRef, CDocument* pDocument);
		void SetFormat(const std::wstring& wsValue);
		void SetFormat(const std::string& sValue);
	};
	
	class CAnnotAppearance : public CDictObject
	{
	public:
		CAnnotAppearance(CXref* pXRef, CFieldBase*  pField);
		CAnnotAppearance(CXref* pXRef, CAnnotation* pAnnot);

		CAnnotAppearanceObject* GetNormal(CResourcesDict* pResources = NULL);
		CAnnotAppearanceObject* GetRollover(CResourcesDict* pResources = NULL);
		CAnnotAppearanceObject* GetDown(CResourcesDict* pResources = NULL);

	private:

		CXref*                  m_pXref;
		CAnnotAppearanceObject* m_pNormal;
		CAnnotAppearanceObject* m_pRollover;
		CAnnotAppearanceObject* m_pDown;
		CFieldBase*             m_pField;
		CAnnotation*            m_pAnnot;
	};

	class CCheckBoxAnnotAppearance : public CDictObject
	{
	public:
		CCheckBoxAnnotAppearance(CXref* pXref, CFieldBase* pField, const char* sYesName = NULL);
		CCheckBoxAnnotAppearance(CXref* pXref, CAnnotation* pAnnot, const char* sYesName = NULL);
		
		CAnnotAppearanceObject* GetYesN();
		CAnnotAppearanceObject* GetOffN();
		CAnnotAppearanceObject* GetYesD();
		CAnnotAppearanceObject* GetOffD();

	private:

		CXref*                  m_pXref;
		CAnnotAppearanceObject* m_pYesN;
		CAnnotAppearanceObject* m_pOffN;
		CAnnotAppearanceObject* m_pYesD;
		CAnnotAppearanceObject* m_pOffD;
	};

	class CAnnotAppearanceObject : public CDictObject
	{
	public:
		CAnnotAppearanceObject(CXref* pXRef, CFieldBase*  pField);
		CAnnotAppearanceObject(CXref* pXRef, CAnnotation* pAnnot, CResourcesDict* pResources = NULL);
		void DrawSimpleText(const std::wstring& wsText, unsigned short* pCodes, unsigned int unCount, CFontDict* pFont, double dFontSize = 10.0, double dX = 0.0, double dY = 0.0, double dR = 0.0, double dG = 0.0, double dB = 0.0, const char* sExtGrStateName = NULL, double dW = 1.0, double dH = 1.0, CFontCidTrueType** ppFonts = NULL, double* pShifts = NULL);
		void DrawPicture(const char* sImageName = NULL, const double& dX = 0.0, const double& dY = 0.0, const double& dW = 0.0, const double& dH = 0.0, const bool& bRespectBorder = false);
		void StartDrawText(CFontDict* pFont, const double& dFontSize, const double& dR, const double& dG, const double& dB, const char* sExtGStateName, const double& dWidth, const double& dHeight);
		void DrawTextLine(const double& dX, const double& dY, const unsigned short* pCodes, const unsigned int& unCount, CFontCidTrueType** ppFonts, const double* pShifts);
		void DrawTextLine(const double &dX, const double &dY, const std::wstring& wsText);
		void EndDrawText();
		void AddBBox(double dX, double dY, double dW, double dH);
		void AddMatrix(double sx, double shy, double shx, double sy, double tx, double ty);

		void StartDraw(const double& dWidth, const double& dHeight);
		void StartText(CFontDict* pFont, const double& dFontSize);
		void DrawPictureInline(const double& dWidth, const double& dHeight,const char* sImageName, const double& dX, const double& dY, const double& dW, const double& dH, const bool& bRespectBorder);
		void EndText();
		void EndDraw();

		void DrawTextCommentN(const std::string& sColor);
		void DrawTextCommentR(const std::string& sColor);
		void DrawTextCheck(const std::string& sColor);
		void DrawTextCheckmark();
		void DrawTextCircle(const std::string& sColor);
		void DrawTextCross(const std::string& sColor);
		void DrawTextCrossHairs(const std::string& sColor);
		void DrawTextHelp(const std::string& sColor);
		void DrawTextInsert(const std::string& sColor);
		void DrawTextKey(const std::string& sColor);
		void DrawTextNewParagraph(const std::string& sColor);
		void DrawTextNote(const std::string& sColor);
		void DrawTextParagraph(const std::string& sColor);
		void DrawTextRightArrow(const std::string& sColor);
		void DrawTextRightPointer(const std::string& sColor);
		void DrawTextStar(const std::string& sColor);
		void DrawTextUpArrow(const std::string& sColor);
		void DrawTextUpLeftArrow(const std::string& sColor);

		void DrawLine();

		void DrawCheckBoxCircle(int nRotate, bool bSet, bool bN);
		void DrawCheckBoxSquare(int nRotate, bool bSet, bool bN);

		CStream* GetStream() const { return m_pStream; }
		CFontDict* GetFont() { return m_pFont; }

		bool        m_bStart;

	private:
		void Init(CXref* pXref, CResourcesDict* pResources);

		void DrawBackground(const double& dWidth, const double& dHeight);
		void DrawBorder(const double& dWidth, const double& dHeight);

		CXref*      m_pXref;
		CStream*    m_pStream;
		CFieldBase* m_pField;
		double      m_dCurX;
		double      m_dCurY;
		CFontDict*  m_pFont;
		double      m_dFontSize;

		CAnnotation* m_pAnnot;
	};

}

#endif // _PDF_WRITER_SRC_FIELD_H
