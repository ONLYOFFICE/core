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
		if (m_ooxDocument  == NULL) return false;

		m_poReader = oParam.oReader;
		m_poDocument = oParam.oRtf;
	

		RtfSectionPtr oCurSection;
		if( true == oParam.oRtf->GetItem( oCurSection ) )
		{
			oCurSection->m_oProperty.SetDefaultOOX();
			//сначала считаем количесво секций и заполняем их свойства .. 

			for (long i = 0; i < m_ooxDocument->m_arrItems.size(); i++)
			{
				if (m_ooxDocument->m_arrItems[i] == NULL) continue;

				if (m_ooxDocument->m_arrItems[i]->getType() == OOX::et_w_p)
				{
					OOX::Logic::CParagraph * para = dynamic_cast<OOX::Logic::CParagraph *>(m_ooxDocument->m_arrItems[i]);
					
					if ((para) && (para->m_oParagraphProperty))
					{
						if (para->m_oParagraphProperty->m_oSectPr.IsInit())
						{
							OOXSectionPropertyReader oSectReader(para->m_oParagraphProperty->m_oSectPr.GetPointer());
							if( true == oSectReader.Parse( oParam, oCurSection->m_oProperty ) )
							{
								//создаем новую секцию
								oCurSection = RtfSectionPtr( new RtfSection() );
								oCurSection->m_oProperty.SetDefaultOOX();
								oParam.oRtf->AddItem( oCurSection );
							}
						}
					}
				}
			}
			if (m_ooxDocument->m_oSectPr.IsInit())// свойства последней секции
			{
				OOXSectionPropertyReader oSectReader(m_ooxDocument->m_oSectPr.GetPointer());
				oSectReader.Parse( oParam, oCurSection->m_oProperty );
			}

			RtfSectionPtr oFirstSection;
			if( true == m_poDocument->GetItem( oFirstSection, 0 ) )
			{
				m_oTextItemReader.m_oTextItems = oFirstSection;

				for (long i = 0; i < m_ooxDocument->m_arrItems.size(); i++)
				{
					m_oTextItemReader.Parse(m_ooxDocument->m_arrItems[i], oParam );
				}
			}
		}
		return true;
	}
};