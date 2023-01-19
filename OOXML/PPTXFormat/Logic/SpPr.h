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
#pragma once

#include "./../WrapperWritingElement.h"
#include "./../Limit/BWMode.h"
#include "Xfrm.h"
#include "UniFill.h"
#include "Ln.h"
#include "EffectProperties.h"
#include "Scene3d.h"
#include "Sp3d.h"
#include "Geometry.h"
#include "ExtP.h"

#include "../../../DesktopEditor/graphics/aggplustypes.h" //for RECT

namespace PPTX
{
	namespace Logic
	{
		class SpPr : public WrapperWritingElement
		{
		public:
			SpPr(std::wstring ns = L"a");
			virtual ~SpPr();	

			explicit SpPr(XmlUtils::CXmlNode& node);
			const SpPr& operator =(XmlUtils::CXmlNode& node);

			explicit SpPr(XmlUtils::CXmlLiteReader& oReader);
			const SpPr& operator =(XmlUtils::CXmlLiteReader& oReader);

			virtual OOX::EElementType getType () const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void fromXML(XmlUtils::CXmlNode& node);
			
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			void Merge(SpPr& spPr)const;

			nullable<Xfrm>				xfrm;
			Logic::Geometry				Geometry;
			UniFill						Fill;
			nullable<Ln>				ln;
			EffectProperties			EffectList;
			nullable<Scene3d>			scene3d;
			nullable<Sp3d>				sp3d;

			nullable_limit<Limit::BWMode> bwMode;

			std::vector<Ext>			extLst;

			std::wstring				m_namespace;

		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX
