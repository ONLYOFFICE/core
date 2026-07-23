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
