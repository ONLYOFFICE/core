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
