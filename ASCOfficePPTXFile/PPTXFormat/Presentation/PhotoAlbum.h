/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once
#ifndef PPTX_PRESENTATION_PHOTOALBUM_INCLUDE_H_
#define PPTX_PRESENTATION_PHOTOALBUM_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/FrameShape.h"
#include "./../Limit/AlbumLayout.h"

namespace PPTX
{
	namespace nsPresentation
	{
		class PhotoAlbum : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(PhotoAlbum)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"bw", bw);
				node.ReadAttributeBase(L"frame", frame);
				node.ReadAttributeBase(L"layout", layout);
				node.ReadAttributeBase(L"showCaptions", showCaptions);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("bw"), bw);
				oAttr.WriteLimitNullable(_T("frame"), frame);
				oAttr.WriteLimitNullable(_T("layout"), layout);
				oAttr.Write(_T("showCaptions"), showCaptions);

				return XmlUtils::CreateNode(_T("p:photoAlbum"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				// TODO:
			}

		public:
			nullable_bool							bw;
			nullable_limit<Limit::FrameShape>		frame;
			nullable_limit<Limit::AlbumLayout>		layout;
			nullable_bool							showCaptions;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsPresentation
} // namespace PPTX

#endif // PPTX_PRESENTATION_PHOTOALBUM_INCLUDE_H_