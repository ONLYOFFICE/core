/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
