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
					//внутрь не втаскиваем- в некоторых элементах обязательно писать r-rPr-t в некоторых достаточно t
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

