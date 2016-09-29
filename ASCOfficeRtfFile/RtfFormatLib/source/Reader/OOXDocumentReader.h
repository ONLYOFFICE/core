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

#include "OOXTextItemReader.h"
#include "../../../../Common/DocxFormat/Source/DocxFormat/Document.h"

class OOXDocumentReader
{
private: 
	OOXTextItemReader	m_oTextItemReader;
	OOXReader	*		m_poReader;
	RtfDocument	*		m_poDocument;


	OOX::CDocument* m_ooxDocument;
public: 
	OOXDocumentReader(OOX::CDocument* ooxDocument) 
	{
		m_ooxDocument = ooxDocument;
	}
	~OOXDocumentReader()
	{
	}
	
	bool Parse( ReaderParameter oParam )
	{
		if (m_ooxDocument == NULL) return false;

		m_poReader		= oParam.oReader;
		m_poDocument	= oParam.oRtf;
		
		//oParam.oRtf->m_oStatusSection.start_new = false;

		_section first_section;
		
		//if( true == oParam.oRtf->GetItem( first_section ) )
		{
			//first_section.props->m_oProperty.SetDefaultOOX();
			//if (m_ooxDocument->m_oSectPr.IsInit())// свойства последней секции
			//{
			//	OOXSectionPropertyReader oSectReader(m_ooxDocument->m_oSectPr.GetPointer());
			//	if (oSectReader.Parse( oParam, first_section.props->m_oProperty ))
			//	{
			//	}
			//}

			int last_section_start = 0;
			
			//считаем количесво секций и заполняем их свойства .. 
			for (long i = 0; i < m_ooxDocument->m_arrItems.size(); i++)
			{
				if (m_ooxDocument->m_arrItems[i] == NULL) continue;

				if (m_ooxDocument->m_arrItems[i]->getType() == OOX::et_w_p)
				{
					OOX::Logic::CParagraph * para = dynamic_cast<OOX::Logic::CParagraph *>(m_ooxDocument->m_arrItems[i]);
					
					if ((para) && (para->m_oParagraphProperty))
					{
						if (para->m_oParagraphProperty->m_oSectPr.IsInit() )
						{
							_section section(RtfSectionPtr( new RtfSection() ), last_section_start, i + 1);
							//if (i == last_section_start) section.end_para = 1;

							last_section_start = i + 1;

							section.props->m_oProperty.SetDefaultOOX();

							OOXSectionPropertyReader oSectReader(para->m_oParagraphProperty->m_oSectPr.GetPointer());
							if( true == oSectReader.Parse( oParam, section.props->m_oProperty ) )
							{
								oParam.oRtf->AddItem( section );
							}
						}
					}
				}
			}
			m_poDocument->RemoveItem(0);

			_section last_section;
			m_poDocument->GetItem(last_section);
			
			if (last_section.end_para < m_ooxDocument->m_arrItems.size())
			{
				_section section(RtfSectionPtr( new RtfSection() ), last_section.end_para, m_ooxDocument->m_arrItems.size());
				
				section.props->m_oProperty.SetDefaultOOX();
				if (m_ooxDocument->m_oSectPr.IsInit())// свойства последней секции
				{
					OOXSectionPropertyReader oSectReader(m_ooxDocument->m_oSectPr.GetPointer());
					if (oSectReader.Parse( oParam, section.props->m_oProperty ))
					{
					}
				}
				
				oParam.oRtf->AddItem( section );
			}


			for (int sect = 0 ; sect < m_poDocument->GetCount(); sect++)
			{
				m_oTextItemReader.m_oTextItems = m_poDocument->m_aArray[sect].props;

				for (long i = m_poDocument->m_aArray[sect].start_para; i < m_poDocument->m_aArray[sect].end_para; i++)
				{
					m_oTextItemReader.Parse(m_ooxDocument->m_arrItems[i], oParam );
				}
			}
		}
		return true;
	}
};