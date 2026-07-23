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

#include "UniMedia.h"

namespace PPTX
{
	namespace Logic
	{
		UniMedia::UniMedia()
		{
		}
		void UniMedia::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring name = XmlUtils::GetNameNoNS(oReader.GetName());

			if (name == _T("audioCd"))
				Media.reset(CreatePtrXmlContent<Logic::AudioCD>(oReader));
			else if (name == _T("wavAudioFile"))
				Media.reset(CreatePtrXmlContent<Logic::WavAudioFile>(oReader));
			else if (name == _T("audioFile"))
				Media.reset(CreatePtrXmlContent<Logic::MediaFile>(oReader));
			else if (name == _T("videoFile"))
				Media.reset(CreatePtrXmlContent<Logic::MediaFile>(oReader));
			else if (name == _T("quickTimeFile"))
				Media.reset(CreatePtrXmlContent<Logic::MediaFile>(oReader));
			else Media.reset();
		}
		void UniMedia::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring name = XmlUtils::GetNameNoNS(node.GetName());

			if (name == _T("audioCd"))
				Media.reset(CreatePtrXmlContent<Logic::AudioCD>(node));
			else if (name == _T("wavAudioFile"))
				Media.reset(CreatePtrXmlContent<Logic::WavAudioFile>(node));
			else if (name == _T("audioFile"))
				Media.reset(CreatePtrXmlContent<Logic::MediaFile>(node));
			else if (name == _T("videoFile"))
				Media.reset(CreatePtrXmlContent<Logic::MediaFile>(node));
			else if (name == _T("quickTimeFile"))
				Media.reset(CreatePtrXmlContent<Logic::MediaFile>(node));
			else Media.reset();
		}
		void UniMedia::GetMediaFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;
			if (element.GetNode(_T("a:audioCd"), oNode))
				Media.reset(CreatePtrXmlContent<Logic::AudioCD>(oNode));
			else if (element.GetNode(_T("a:wavAudioFile"), oNode))
				Media.reset(CreatePtrXmlContent<Logic::WavAudioFile>(oNode));
			else if (element.GetNode(_T("a:audioFile"), oNode))
				Media.reset(CreatePtrXmlContent<Logic::MediaFile>(oNode));
			else if (element.GetNode(_T("a:videoFile"), oNode))
				Media.reset(CreatePtrXmlContent<Logic::MediaFile>(oNode));
			else if (element.GetNode(_T("a:quickTimeFile"), oNode))
				Media.reset(CreatePtrXmlContent<Logic::MediaFile>(oNode));
			else Media.reset();
		}
		std::wstring UniMedia::toXML() const
		{
			if (Media.IsInit())
				return Media->toXML();
			return _T("");
		}
		void UniMedia::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
		}
		void UniMedia::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (Media.is_init())
				Media->toXmlWriter(pWriter);
		}
		bool UniMedia::is_init() const
		{
			return (Media.IsInit());
		}
		void UniMedia::FillParentPointersForChilds() {}
		void UniMedia::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				Media->SetParentPointer(pParent);
		}
	} // namespace Logic
} // namespace PPTX
