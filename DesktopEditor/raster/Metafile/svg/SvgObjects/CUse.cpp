/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "CUse.h"
#include "../CSvgFile.h"

namespace SVG
{
	CUse::CUse(CSvgReader& oReader, CRenderedObject *pParent)
		: CRenderedObject(oReader, pParent)
	{}

	CUse::~CUse()
	{}

	void CUse::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		if ("x" == sName)
			m_oX.SetValue(oReader.GetText());
		else if ("y" == sName)
			m_oY.SetValue(oReader.GetText());
		else if ("width" == sName)
			m_oWidth.SetValue(oReader.GetText());
		else if ("height" == sName)
			m_oHeight.SetValue(oReader.GetText());
		else if ("href" == sName || "xlink:href" == sName)
			m_wsHref = oReader.GetText();
		else
			CRenderedObject::SetAttribute(sName, oReader);
	}

	void CUse::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		CRenderedObject::SetData(mAttributes, ushLevel, bHardMode);

		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);
	}

	bool CUse::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles* pOtherStyles, const CRenderedObject* pContexObject) const
	{
		if (NULL == pRenderer || !m_oTransformation.m_bDraw)
			return false;

		Aggplus::CMatrix oOldTransform;

		if (!StartPath(pRenderer, pFile, oOldTransform))
			return false;

		double dM11, dM12, dM21, dM22, dDx, dDy;
		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dDx, &dDy);

		Aggplus::CMatrix oNewTransform(dM11, dM12, dM21, dM22, dDx, dDy);
		oNewTransform.Translate(m_oX.ToDouble(NSCSS::Pixel), m_oY.ToDouble(NSCSS::Pixel));

		pRenderer->SetTransform(oNewTransform.sx(), oNewTransform.shy(), oNewTransform.shx(), oNewTransform.sy(), oNewTransform.tx(), oNewTransform.ty());

		const CRenderedObject *pFoundObj = dynamic_cast<CRenderedObject*>(pFile->GetMarkedObject(m_wsHref));

		bool bResult = false;

		if (NULL != pFoundObj && this != pFoundObj)
		{
			if (NULL != pOtherStyles)
			{
				TSvgStyles oNewStyles(m_oStyles);
				oNewStyles += *pOtherStyles;
				bResult = pFoundObj->Draw(pRenderer, pFile, oMode, &oNewStyles, this);
			}
			else
				bResult = pFoundObj->Draw(pRenderer, pFile, oMode, &m_oStyles, this);
		}

		EndPath(pRenderer, pFile, oOldTransform);

		return bResult;
	}

	TBounds CUse::GetBounds(SvgMatrix* pTransform) const
	{
		return TBounds{0., 0., 0., 0.};
	}
}
