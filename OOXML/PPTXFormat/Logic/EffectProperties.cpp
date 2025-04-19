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
#include "EffectProperties.h"

namespace PPTX
{
	namespace Logic
	{
		EffectProperties& EffectProperties::operator=(const EffectProperties& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			List.reset();
			if (oSrc.List.IsInit())
			{
				if (oSrc.List.is<EffectLst>())
				{
					List.reset(new EffectLst());
					List.as<EffectLst>() = oSrc.List.as<EffectLst>();
				}
				else if (oSrc.List.is<EffectDag>())
				{
					List.reset(new EffectDag());
					List.as<EffectDag>() = oSrc.List.as<EffectDag>();
				}
			}
			return *this;
		}
		OOX::EElementType EffectProperties::getType() const
		{
			if (List.IsInit())
				return List->getType();
			return OOX::et_Unknown;
		}
		void EffectProperties::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (strName == L"effectLst")
			{
				Logic::EffectLst* pEffectLst = new Logic::EffectLst();
				*pEffectLst = oReader;
				List.reset(pEffectLst);
			}
			else if (strName == L"effectDag")
			{
				Logic::EffectDag* pEffectDag = new Logic::EffectDag();
				*pEffectDag = oReader;
				List.reset(pEffectDag);
			}
			else
				List.reset();
		}
		void EffectProperties::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG pos = pReader->GetPos();
			ULONG rec_len = pReader->GetULong();
			if (0 == rec_len)
				return;

			BYTE rec = pReader->GetUChar();

			switch(rec)
			{
			case EFFECTPROPERTIES_TYPE_LIST:	List = new PPTX::Logic::EffectLst(); break;
			case EFFECT_TYPE_DAG:				List = new PPTX::Logic::EffectDag(); break;
			}

			pReader->Seek(pos);
			if (List.is_init())
			{
				List->fromPPTY(pReader);
			}
			else
			{
				pReader->SkipRecord();
			}
		}
		void EffectProperties::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring strName = XmlUtils::GetNameNoNS(node.GetName());

			if (L"effectLst" == strName)
			{
				Logic::EffectLst* pEffectLst = new Logic::EffectLst();
				pEffectLst->fromXML(node);
				List.reset(pEffectLst);
			}
			else if (L"effectDag" == strName)
			{
				Logic::EffectDag* pEffectDag = new Logic::EffectDag();
				pEffectDag->fromXML(node);
				List.reset(pEffectDag);
			}
			else List.reset();
		}
		std::wstring EffectProperties::toXML() const
		{
			if (!List.IsInit())
				return L"";
			return List->toXML();
		}
		void EffectProperties::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (List.is_init())
				List->toXmlWriter(pWriter);
		}

		void EffectProperties::Merge(EffectProperties& effectProperties) const
		{
			if (List.IsInit() && List.is<EffectLst>())
			{
				effectProperties.List.reset(new EffectLst());
				List.as<EffectLst>().Merge(effectProperties.List.as<EffectLst>());
			}
		}

		void EffectProperties::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (List.is_init())
				List->toPPTY(pWriter);
		}

	} // namespace Logic
} // namespace PPTX
