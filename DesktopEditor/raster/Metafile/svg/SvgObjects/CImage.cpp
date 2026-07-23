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

#include "CImage.h"

#include "../CSvgFile.h"
#include "../../graphics/Image.h"
#include "../../../BgraFrame.h"
#include "../../../../common/Path.h"
#include "../../../../common/ProcessEnv.h"
#include "../../../../common/Base64.h"
#include "../../../../common/File.h"

namespace SVG
{
	CImage::CImage(CSvgReader& oReader, CRenderedObject* pParent)
		: CRenderedObject(oReader, pParent)
	{}

	void CImage::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		if ("x" == sName)
			m_oRect.m_oX.SetValue(oReader.GetText());
		else if ("y" == sName)
			m_oRect.m_oY.SetValue(oReader.GetText());
		else if ("width" == sName)
			m_oRect.m_oWidth.SetValue(oReader.GetText());
		else if ("height" == sName)
			m_oRect.m_oHeight.SetValue(oReader.GetText());
		else if ("href" == sName || "xlink:href" == sName)
			m_wsHref = oReader.GetText(); // TODO:: In the future, it may be worth implementing a separate CHref class for all link types
		else
			CRenderedObject::SetAttribute(sName, oReader);
	}

	bool CImage::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles, const CRenderedObject* pContexObject) const
	{
		Aggplus::CMatrix oOldTransform;

		if (m_wsHref.empty() || !StartPath(pRenderer, pFile, oOldTransform, oMode))
			return false;

		TBounds oBounds = (NULL != m_pParent) ? m_pParent->GetBounds() : TBounds{0., 0., 0., 0.};

		double dParentWidth  = oBounds.m_dRight  - oBounds.m_dLeft;
		double dParentHeight = oBounds.m_dBottom - oBounds.m_dTop;

		double dX      = m_oRect.m_oX     .ToDouble(NSCSS::Pixel, dParentWidth);
		double dY      = m_oRect.m_oY     .ToDouble(NSCSS::Pixel, dParentHeight);
		double dWidth  = m_oRect.m_oWidth .ToDouble(NSCSS::Pixel, dParentWidth);
		double dHeight = m_oRect.m_oHeight.ToDouble(NSCSS::Pixel, dParentHeight);

		BYTE* pBuffer = NULL;
		DWORD ulSize = 0;
		size_t unStart = m_wsHref.find(L"data:image");

		if (std::wstring::npos != unStart)
		{
			size_t unType = m_wsHref.find(L";base64", unStart);

			if (std::wstring::npos == unType)
				return false;

			const std::wstring wsImageType = m_wsHref.substr(unStart + 11, unType - unStart - 11);

			if (L"png" != wsImageType && L"jpeg" != wsImageType)
				return false;
	
			std::wstring wsImageData = m_wsHref.substr(unType + 8, m_wsHref.length() - unType - 8);
			ulSize = NSBase64::Base64DecodeGetRequiredLength(wsImageData.length());

			pBuffer = new(std::nothrow) BYTE[ulSize];

			if (NULL == pBuffer)
				return false;

			NSBase64::Base64Decode(wsImageData.c_str(), wsImageData.length(), pBuffer, &(int&)ulSize);
		}
		#ifndef METAFILE_DISABLE_FILESYSTEM
		else
		{
			std::wstring wsFilePath = NSSystemPath::ShortenPath(m_wsHref);
	
			bool bIsAllowExternalLocalFiles = true;
			if (NSProcessEnv::IsPresent(NSProcessEnv::Converter::gc_allowPrivateIP))
				bIsAllowExternalLocalFiles = NSProcessEnv::GetBoolValue(NSProcessEnv::Converter::gc_allowPrivateIP);

			if (!bIsAllowExternalLocalFiles && wsFilePath.length() >= 3 && L"../" == wsFilePath.substr(0, 3))
				return false;
	
			wsFilePath = pFile->GetWorkingDirectory() + L'/' + wsFilePath;
	
			if (!NSFile::CFileBinary::Exists(wsFilePath) || !NSFile::CFileBinary::ReadAllBytes(wsFilePath, &pBuffer, ulSize))
				return false;
		}
		#endif

		if (NULL == pBuffer)
			return false;

		CBgraFrame oBgraFrame;
		oBgraFrame.Decode(pBuffer, ulSize);

		double dImageW = oBgraFrame.get_Width();
		double dImageH = oBgraFrame.get_Height();

		Aggplus::CImage oImage;
		oImage.Create(oBgraFrame.get_Data(), dImageW, dImageH, -4 * dImageW, true);

		if (dImageW / dWidth > dImageH / dHeight)
		{
			double dValue = dImageW / dWidth;
			dY += (dHeight - (dImageH / dValue)) / 2.;
			dHeight = dImageH / dValue;
		}
		else if (dImageW / dWidth < dImageH / dHeight)
		{
			double dValue = dImageH / dHeight;
			dX += (dWidth - (dImageW / dValue)) / 2.;
			dWidth = dImageW / dValue;
		}

		if (CommandeModeClip != oMode)
			pRenderer->DrawImage(&oImage, dX, dY, dWidth, dHeight);
		else
		{
			pRenderer->PathCommandMoveTo(dX, dY);
			pRenderer->PathCommandLineTo(dX + dWidth, dY);
			pRenderer->PathCommandLineTo(dX + dWidth, dY + dHeight);
			pRenderer->PathCommandLineTo(dX, dY + dHeight);
			pRenderer->PathCommandClose();
		}

		EndPath(pRenderer, pFile, oOldTransform, oMode, pOtherStyles, pContexObject);

		delete[] pBuffer;

		return true;
	}

	TBounds CImage::GetBounds(SvgMatrix* pTransform) const
	{
		TBounds oBounds;

		oBounds.m_dLeft   = m_oRect.m_oX.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop    = m_oRect.m_oY.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  = oBounds.m_dLeft + m_oRect.m_oWidth.ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom = oBounds.m_dTop  + m_oRect.m_oHeight.ToDouble(NSCSS::Pixel);

		if (nullptr != pTransform)
		{
			*pTransform += m_oTransformation.m_oTransform.GetMatrix();

			pTransform->GetFinalValue().TransformPoint(oBounds.m_dLeft,  oBounds.m_dTop   );
			pTransform->GetFinalValue().TransformPoint(oBounds.m_dRight, oBounds.m_dBottom);

			*pTransform -= m_oTransformation.m_oTransform.GetMatrix();
		}

		return oBounds;
	}
}
