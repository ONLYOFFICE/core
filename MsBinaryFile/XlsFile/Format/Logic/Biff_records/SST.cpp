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
#include "SST.h"

#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

SST::SST(const unsigned short code_page)
:	code_page_(code_page)
{
}


SST::~SST()
{
}


BaseObjectPtr SST::clone()
{
	return BaseObjectPtr(new SST(*this));
}

void SST::readFields(CFRecord& record)
{
	record >> cstTotal >> cstUnique;
	long total = cstTotal;
	long unique = cstUnique;
		
	std::list<CFRecordPtr>& recs = continue_records[rt_Continue];
	int count = 0 ;
	while(!record.isEOF() || !recs.empty())
	{
		XLUnicodeRichExtendedStringPtr element(new XLUnicodeRichExtendedString(recs));

		if (record.getRdPtr() == record.getDataSize())
		{
			element->appendNextContinue(record, false);
		}
		else if (record.getRdPtr() + 3 > record.getDataSize()) 
		{
			element->appendNextContinue(record, true);
		}

		if (record.getRdPtr() + 3 > record.getDataSize())
			break;

		record >> *element;
		rgb.push_back(element);
		count++;
	}
}

void SST::writeFields(CFRecord& record)
{
	const auto MaxRecordSize = 8000;
    cstUnique = rgb.size();
	if(cstTotal == 0 && cstTotal < cstUnique)
		cstTotal = cstUnique;
    record << cstTotal << cstUnique;
    while(!rgb.empty())
    {
        auto oldPose = record.getRdPtr();
		rgb.at(0)->save(record);
		if(record.getRdPtr() >= MaxRecordSize)
        {
            record.RollRdPtrBack(record.getRdPtr() - oldPose);
            break;
        }
        rgb.erase(rgb.begin());
    }

}

int SST::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
	{
		for (size_t i = 0; i < rgb.size(); i++)
		{
			XLUnicodeRichExtendedString *richText = dynamic_cast<XLUnicodeRichExtendedString *>(rgb[i].get());

			if (richText == NULL) continue;

			CP_XML_NODE(L"si")
			{	
				try
				{
					//don't move inside - in some elements you must write r-rPr-t, in others just t is enough
					if (false == richText->rgRun.empty())
					{							
						richText->serialize(CP_XML_STREAM());
					}
					else
					{
						CP_XML_NODE(L"t")
						{
							size_t size = richText->str_.length();
							CP_XML_ATTR(L"xml:space", L"preserve");
							
							if (size > 0x7fff) //PLANILHA_DE_ALOCACAO_DE_TEMPO_w51.xls
							{
								CP_XML_STREAM() << STR::escape_ST_Xstring(XmlUtils::EncodeXmlString(richText->str_.substr(0, 0x7fff - 2)));
							}
							else
							{
								CP_XML_STREAM() << STR::escape_ST_Xstring(XmlUtils::EncodeXmlString(richText->str_));
							}
						}
					}
				}
				catch(...)
				{
					CP_XML_NODE(L"t");
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

