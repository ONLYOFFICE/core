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
#ifndef _BUILD_ANNOTFIELD_H_
#define _BUILD_ANNOTFIELD_H_

#include "config.h"
#include "IRenderer.h"

class GRAPHICS_DECL CAnnotFieldInfo : public IAnnotField
{
public:
	class GRAPHICS_DECL CMarkupAnnotPr
	{
	public:
		CMarkupAnnotPr() {}

	private:
		BYTE m_nRT;
		double m_dCA;
		std::wstring m_wsT;
		std::wstring m_wsRC;
		std::wstring m_wsSubj;
	};

	class GRAPHICS_DECL CTextAnnotPr
	{
	public:
		CTextAnnotPr() : m_bOpen(false), m_nName(2), m_nState(7), m_nStateModel(2) {}

		void SetOpen(bool bOpen)             { m_bOpen = bOpen; }
		void SetName(BYTE nName)             { m_nName = nName; }
		void SetState(BYTE nState)           { m_nState = nState; }
		void SetStateModel(BYTE nStateModel) { m_nStateModel = nStateModel; }

		bool IsOpen()        const { return m_bOpen; }
		BYTE GetName()       const { return m_nName; }
		BYTE GetState()      const { return m_nState; }
		BYTE GetStateModel() const { return m_nStateModel; }

	private:
		bool m_bOpen;
		BYTE m_nName;
		BYTE m_nState;
		BYTE m_nStateModel;
	};

public:
	CAnnotFieldInfo();
	virtual ~CAnnotFieldInfo();

	virtual void SetType(int nType);
	bool IsValid() const;

	// Common
	void SetBounds(const double& dX, const double& dY, const double& dW, const double& dH);
	void GetBounds(double& dX, double& dY, double& dW, double& dH) const;

	void SetBaseLineOffset(const double& dOffset) { m_dBaseLineOffset = dOffset; }
	double GetBaseLineOffset() const { return m_dBaseLineOffset; }

	void SetID(const int& nID) { m_nID = nID; }
	int GetID() const { return m_nID; }

	void SetAnnotFlag(const int& nAnnotFlag) { m_nAnnotFlag = nAnnotFlag; }
	int GetAnnotFlag() const { return m_nAnnotFlag; }

	void SetPage(const int& nPage) { m_nPage = nPage; }
	int GetPage() const { return m_nPage; }

	// void Set(const int& n) { m_n = n; }
	// int Get() const { return m_n; }

	void SetContents(const std::wstring& wsContents) { m_wsContents = wsContents; }
	const std::wstring& GetContents() const { return m_wsContents; }

	void SetC(const std::vector<double>& arrC) { m_arrC = arrC; }
	const std::vector<double>& GetC() const { return m_arrC; }

	void SetBorder(BYTE nType, double dWidth, double dGasp1, double dGasp2) {}

	bool isWidget() const;
	bool isMarkup() const;
	bool IsText()   const;
	bool IsInk()    const;
	bool IsLine()   const;

	CMarkupAnnotPr* GetMarkupAnnotPr();
	CTextAnnotPr*   GetTextAnnotPr();

protected:
	int          m_nType;

private:
	int          m_nID;
	int          m_nAnnotFlag;
	int          m_nPage;
	double       m_dX;
	double       m_dY;
	double       m_dW;
	double       m_dH;
	double       m_dBaseLineOffset;
	std::wstring m_wsContents;
	std::vector<double> m_arrC;

	CMarkupAnnotPr* m_pMarkupPr;
	CTextAnnotPr* m_pTextPr;
};

#endif // _BUILD_ANNOTFIELD_H_
