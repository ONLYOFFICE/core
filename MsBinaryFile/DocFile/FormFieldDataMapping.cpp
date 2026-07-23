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

#include "FormFieldDataMapping.h"

namespace DocFileFormat
{	
	FormFieldDataMapping::FormFieldDataMapping(XMLTools::CStringXmlWriter* writer, ConversionContext* context, IMapping* caller)
																	: AbstractOpenXmlMapping(writer), m_context(NULL),_caller(NULL)
	{
		m_context	=	context;
		_caller		=	caller;
	}

	void FormFieldDataMapping::Apply(IVisitable* visited)
	{
		FormFieldData* ffData = static_cast<FormFieldData*>(visited);

		if ( ffData == NULL ) return;

		if (ffData->FFData.bExist)
		{
			m_pXmlWriter->WriteNodeBegin( L"w:ffData");

				m_pXmlWriter->WriteNodeBegin( L"w:name", true);
					m_pXmlWriter->WriteAttribute(L"w:val", XmlUtils::EncodeXmlString(ffData->FFData.xstzName));
				m_pXmlWriter->WriteNodeEnd(L"w:name", true, true );

				m_pXmlWriter->WriteNodeBegin(L"w:enabled", true);
				if (ffData->FFData.fProt)
					m_pXmlWriter->WriteAttribute(L"w:val", 0);
				m_pXmlWriter->WriteNodeEnd(L"w:enabled", true, true);

				m_pXmlWriter->WriteNodeBegin( L"w:calcOnExit", true);
					m_pXmlWriter->WriteAttribute(L"w:val", ffData->FFData.fRecalc);
				m_pXmlWriter->WriteNodeEnd(L"w:calcOnExit", true, true );

				if (false == ffData->FFData.xstzHelpText.empty())
				{
					m_pXmlWriter->WriteNodeBegin(L"w:helpText", true);
					m_pXmlWriter->WriteAttribute(L"w:type", ffData->FFData.fOwnHelp ? L"text" : L"autoText");
					m_pXmlWriter->WriteAttribute(L"w:val", XmlUtils::EncodeXmlString(ffData->FFData.xstzHelpText));
					m_pXmlWriter->WriteNodeEnd(L"w:helpText", true, true);
				}
				if (false == ffData->FFData.xstzStatText.empty())
				{
					m_pXmlWriter->WriteNodeBegin(L"w:statusText", true);
					m_pXmlWriter->WriteAttribute(L"w:type", ffData->FFData.fOwnStat ? L"text" : L"autoText");
					m_pXmlWriter->WriteAttribute(L"w:val", XmlUtils::EncodeXmlString(ffData->FFData.xstzStatText));
					m_pXmlWriter->WriteNodeEnd(L"w:statusText", true, true);
				}
				if (false == ffData->FFData.xstzEntryMcr.empty())
				{
					m_pXmlWriter->WriteNodeBegin(L"w:entryMacro", true);
					m_pXmlWriter->WriteAttribute(L"w:val", XmlUtils::EncodeXmlString(ffData->FFData.xstzEntryMcr));
					m_pXmlWriter->WriteNodeEnd(L"w:entryMacro", true, true);
				}
				if (false == ffData->FFData.xstzExitMcr.empty())
				{
					m_pXmlWriter->WriteNodeBegin(L"w:exitMacro", true);
					m_pXmlWriter->WriteAttribute(L"w:val", XmlUtils::EncodeXmlString(ffData->FFData.xstzExitMcr));
					m_pXmlWriter->WriteNodeEnd(L"w:exitMacro", true, true);
				}
				if (ffData->FFData.iType == 1)
				{
					m_pXmlWriter->WriteNodeBegin(L"w:checkBox");

					if (ffData->FFData.iSize)
					{
						m_pXmlWriter->WriteNodeBegin(L"w:size", true);
						m_pXmlWriter->WriteAttribute(L"w:val", ffData->FFData.hps);
						m_pXmlWriter->WriteNodeEnd(L"w:size", true, true);
					}
					else
					{
						m_pXmlWriter->WriteNodeBegin(L"w:sizeAuto", true);
						m_pXmlWriter->WriteNodeEnd(L"w:sizeAuto", true, true);
					}

					m_pXmlWriter->WriteNodeBegin(L"w:default", true);
					m_pXmlWriter->WriteAttribute(L"w:val", ffData->FFData.wDef);
					m_pXmlWriter->WriteNodeEnd(L"w:default", true, true);

					m_pXmlWriter->WriteNodeEnd(L"w:checkBox");
				}
				else if (ffData->FFData.iType == 2)
				{
					m_pXmlWriter->WriteNodeBegin(L"w:ddList");


					m_pXmlWriter->WriteNodeEnd(L"w:ddList");
				}
				else
				{
					m_pXmlWriter->WriteNodeBegin(L"w:textInput");

					m_pXmlWriter->WriteNodeBegin(L"w:maxLength", true);
					m_pXmlWriter->WriteAttribute(L"w:val", ffData->FFData.cch_field);
					m_pXmlWriter->WriteNodeEnd(L"w:maxLength", true, true);

					m_pXmlWriter->WriteNodeEnd(L"w:textInput");
				}

			m_pXmlWriter->WriteNodeEnd( L"w:ffData" );
		}
	}
}
