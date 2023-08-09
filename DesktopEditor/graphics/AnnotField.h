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
protected:
	class CMarkupAnnot
	{
		CMarkupAnnot();
	};
public:
	class GRAPHICS_DECL CTextAnnotPr : public CMarkupAnnot
	{
	public:
		CTextAnnotPr();

	private:
		bool m_bOpen;
		BYTE m_nName;
		BYTE m_nState;
		BYTE m_nStateModel;
	};

public:
	CAnnotFieldInfo();
	virtual ~CAnnotFieldInfo();

	void SetType(int nType);
	bool IsValid() const;

	// Common
	void SetBounds(const double& dX, const double& dY, const double& dW, const double& dH);
	void GetBounds(double& dX, double& dY, double& dW, double& dH) const;

	void SetBaseLineOffset(const double& dOffset);
	double GetBaseLineOffset() const;

	bool isWidget() const;
	bool IsText()   const;
	bool IsInk()    const;
	bool IsLine()   const;

	CTextAnnotPr* GetTextAnnotPr();

protected:
	int          m_nType;

private:
	double       m_dX;
	double       m_dY;
	double       m_dW;
	double       m_dH;
	double       m_dBaseLineOffset;

	CTextAnnotPr* m_oTextPr;
};

#endif // _BUILD_ANNOTFIELD_H_
