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

#include "AudioCD.h"

namespace PPTX
{
	namespace Logic
	{
		AudioCD::AudioCD()
		{
		}
		AudioCD& AudioCD::operator=(const AudioCD& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			stTrack		= oSrc.stTrack;
			endTrack	= oSrc.endTrack;

			stTime		= oSrc.stTime;
			endTime		= oSrc.endTime;

			return *this;
		}
		void AudioCD::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			//todooo
		}
		void AudioCD::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode oMem;

			oMem		= node.ReadNode(_T("a:st"));
			stTrack		= oMem.ReadAttributeInt(L"track");
			XmlMacroReadAttributeBase(oMem, L"time", stTime);

			oMem		= node.ReadNode(_T("a:end"));
			endTrack	= oMem.ReadAttributeInt(L"track");
			XmlMacroReadAttributeBase(oMem, L"time", endTime);

			Normalize();
		}
		std::wstring AudioCD::toXML() const
		{
			XmlUtils::CAttribute oAttr1;
			oAttr1.Write(_T("track"), stTrack);
			oAttr1.Write(_T("time"), stTime);

			XmlUtils::CAttribute oAttr2;
			oAttr2.Write(_T("track"), endTrack);
			oAttr2.Write(_T("time"), endTime);

			return _T("<a:audioCd>") + XmlUtils::CreateNode(_T("a:st"), oAttr1) + XmlUtils::CreateNode(_T("a:end"), oAttr2) + _T("</a:audioCd>");
		}
		void AudioCD::FillParentPointersForChilds(){}
		void AudioCD::Normalize()
		{
			stTrack = (std::max)(0, (std::min)(255, stTrack));
			stTrack = (std::max)(0, (std::min)(255, endTrack));

			stTime.normalize_positive();
			endTime.normalize_positive();
		}
	} // namespace Logic
} // namespace PPTX
