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

#include "FEAT11.h"
#include "SORTDATA12.h"

#include "../Biff_records/FeatHdr11.h"
#include "../Biff_records/Feature12.h"
#include "../Biff_records/ContinueFrt11.h"
#include "../Biff_records/List12.h"
#include "../Biff_records/AutoFilter12.h"
#include "../Biff_records/ContinueFrt12.h"
#include "../Biff_records/List12.h"
#include "../Biff_records/SortData.h"

#include "../Biff_structures/List12BlockLevel.h"
#include "../Biff_structures/List12TableStyleClientInfo.h"
#include "../Biff_structures/List12DisplayName.h"
#include "../Biff_structures/Feat11FieldDataItem.h"
#include "../Biff_structures/AF12Criteria.h"
#include "../Biff_structures/SortCond12.h"

namespace XLS
{


FEAT11::FEAT11()
{
}


FEAT11::~FEAT11()
{
}

class Parenthesis_FEAT11_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_FEAT11_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_FEAT11_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.optional<Feature11>())
		{
			if(!proc.optional<Feature12>())
			{
				return false;
			}
		}
		int count = proc.repeated<List12>(0, 0);
		
		while (proc.optional<AutoFilter12>())
		{
			count = proc.repeated<ContinueFrt12>(0, 0);
		}		
		
		count = proc.repeated<List12>(0, 0);
		proc.optional<SORTDATA12>();
		return true;
	};
};


BaseObjectPtr FEAT11::clone()
{
	return BaseObjectPtr(new FEAT11(*this));
}


/*
FEAT11 = FeatHdr11 *((Feature11 / Feature12) *ContinueFrt11 *List12 [AutoFilter12 *ContinueFrt12] *List12 [SORTDATA12])
*/
const bool FEAT11::loadContent(BinProcessor& proc)
{
	if (!proc.mandatory<FeatHdr11>())
	{
		return false;
	}
	m_FeatHdr11 = elements_.back();
	elements_.pop_back();
	
	int count = proc.repeated<Parenthesis_FEAT11_1>(0, 0);

	while(!elements_.empty())
	{
		switch(elements_.front()->get_type())
		{
			case typeFeature11: 
			case typeFeature12:
			{
				_data new_data;
				new_data.m_Feature = elements_.front();

				m_arFEAT.push_back(new_data);
			}break;
			case typeList12:
			{
				if (m_arFEAT.back().m_AutoFilter12)
				{
					m_arFEAT.back().m_arList12_2.push_back(elements_.front());
				}
				else
				{
					m_arFEAT.back().m_arList12.push_back(elements_.front());
				}
			}break;
			case typeAutoFilter12:
			{
				m_arFEAT.back().m_AutoFilter12 = elements_.front();
			}break;
			case typeSORTDATA12:
			{
				m_arFEAT.back().m_SORTDATA12 = elements_.front();
			}break;
			default:
				break;
		}
		elements_.pop_front();
	}
	return true;
}

int FEAT11::serialize(std::wostream & strm, size_t index)
{
	FeatHdr11 * feature = dynamic_cast<FeatHdr11*>(m_FeatHdr11.get());

	Feature11		*feature11	= dynamic_cast<Feature11*>(m_arFEAT[index].m_Feature.get());
	Feature12		*feature12	= dynamic_cast<Feature12*>(m_arFEAT[index].m_Feature.get());
	AutoFilter12	*filter		= dynamic_cast<AutoFilter12*>(m_arFEAT[index].m_AutoFilter12.get());
	SORTDATA12		*sort		= dynamic_cast<SORTDATA12*>(m_arFEAT[index].m_SORTDATA12.get());

	if (feature12 && !feature11)
	{
		feature11 = &feature12->feature11;
	}
	
	List12BlockLevel			*block_level = NULL;
	List12TableStyleClientInfo	*table_style = NULL;
	List12DisplayName			*display_name = NULL;

	for (size_t i = 0; i < m_arFEAT[index].m_arList12.size(); i++)
	{
		List12* list_prop = dynamic_cast<List12*>(m_arFEAT[index].m_arList12[i].get());
		if (!list_prop) continue;

		if (!block_level)	block_level		= dynamic_cast<List12BlockLevel*>			(list_prop->rgbList12.get());
		if (!table_style)	table_style		= dynamic_cast<List12TableStyleClientInfo*>	(list_prop->rgbList12.get());
		if (!display_name)	display_name	= dynamic_cast<List12DisplayName*>			(list_prop->rgbList12.get());
	}
//----------------------------------------------------------------------------------------------------------------------------------
	std::wstring display, comment;
	if (display_name)
	{
		display = display_name->stListName.value();
		comment	= display_name->stListComment.value();
	}
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"table")
		{
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");	

			if (feature11)
			{
				if (display.empty()) display = feature11->rgbFeat.rgbName.value();

				CP_XML_ATTR(L"id",				feature11->rgbFeat.idList);
				CP_XML_ATTR(L"name",			feature11->rgbFeat.rgbName.value());
				CP_XML_ATTR(L"displayName",		display);
				CP_XML_ATTR(L"ref",				feature11->sqref);

				if (feature11->rgbFeat.crwHeader < 1 )
				{//ZXC_5693030.xls marker_mc.xls
					CP_XML_ATTR(L"headerRowCount",	0); 
				}
				if (!feature11->rgbFeat.fShownTotalRow)
				{
					CP_XML_ATTR(L"totalsRowShown",	0);
				}
				//CP_XML_ATTR(L"totalsRowCount",	feature11->rgbFeat.crwTotals);
				
				if (!comment.empty()) 
					CP_XML_ATTR(L"comment", comment);

				if (block_level)
				{
					if (!block_level->stData.value().empty())
						CP_XML_ATTR(L"dataCellStyle", block_level->stData.value());	
				}
				if (feature11->rgbFeat.fAutoFilter)
				{
					if(sort)
					{
						SortData *sort_data = dynamic_cast<SortData*>(sort->m_SortData.get());

						CP_XML_NODE(L"autoFilter")
						{
							CP_XML_ATTR(L"ref", sort_data->rfx);
						}
						CP_XML_NODE(L"sortState")
						{
							CP_XML_ATTR(L"ref", sort_data->rfx);
							for (size_t s = 0; s < sort_data->sortCond12Array.size(); s++)
							{
								SortCond12 *sort_cond = dynamic_cast<SortCond12*>(sort_data->sortCond12Array[s].get());
								CP_XML_NODE(L"sortCondition")
								{
									CP_XML_ATTR(L"descending", sort_cond->fSortDes);
									CP_XML_ATTR(L"ref", sort_cond->rfx.toString());
								}
							}
						}
					}
					else
					{
						CP_XML_NODE(L"autoFilter")
						{
							CP_XML_ATTR(L"ref", feature11->sqref);

							if (filter)
							{
								CP_XML_NODE(L"customFilters")
								{
									for (size_t k = 0 ; k < (std::min)((size_t)1, filter->arAF12Criteries.size()); k++)
									{
										AF12Criteria * af12Criteria = dynamic_cast<AF12Criteria *>(filter->arAF12Criteries[k].get());
										if (af12Criteria == NULL) continue;

										af12Criteria->doper.serialize(CP_XML_STREAM(), L"customFilter", af12Criteria->_str);
									}
								}
							}
							else
							{
								for (size_t i = 0; i < feature11->rgbFeat.arFieldData.size(); i++)
								{
									Feat11FieldDataItem* field = dynamic_cast<Feat11FieldDataItem*>(feature11->rgbFeat.arFieldData[i].get());
									if(!field) continue;

									if (field->AutoFilter.cbAutoFilter > 0 && field->AutoFilter.cbAutoFilter < 2080)
									{
										CP_XML_NODE(L"filterColumn")
										{
											CP_XML_ATTR(L"colId", field->idField - 1);

											if (field->AutoFilter.recAutoFilter.fTopN != 1)
											{
												CP_XML_NODE(L"customFilters")
												{
													if (field->AutoFilter.recAutoFilter.wJoin == 0) CP_XML_ATTR(L"and", 1); //and
													if (field->AutoFilter.recAutoFilter.wJoin == 1) CP_XML_ATTR(L"and", 0); //or

													field->AutoFilter.recAutoFilter.doper1.serialize(CP_XML_STREAM(), L"customFilter", L"");
													field->AutoFilter.recAutoFilter.doper2.serialize(CP_XML_STREAM(), L"customFilter", L"");
												}
											}
										}
									}
								}
							}
						}
					}
				}

				CP_XML_NODE(L"tableColumns")
				{
					CP_XML_ATTR(L"count", feature11->rgbFeat.arFieldData.size());
					for (size_t i = 0; i < feature11->rgbFeat.arFieldData.size(); i++)
					{
						Feat11FieldDataItem* field = dynamic_cast<Feat11FieldDataItem*>(feature11->rgbFeat.arFieldData[i].get());
						if(!field) continue;

						CP_XML_NODE(L"tableColumn")
						{
							CP_XML_ATTR(L"id", field->idField);
							CP_XML_ATTR(L"name", field->strCaption.value());

							if (!field->strTotal.value().empty())
								CP_XML_ATTR(L"totalsRowLabel", field->strTotal.value());	
							
							if (field->dxfFmtAgg.bExist && field->dxfFmtAgg.dxfId_ >= 0)
							{
								CP_XML_ATTR(L"dataDxfId", field->dxfFmtAgg.dxfId_); 
							}
							if (field->dxfFmtInsertRow.bExist && field->dxfFmtInsertRow.dxfId_ >= 0)
							{
								CP_XML_ATTR(L"totalsRowDxfId", field->dxfFmtInsertRow.dxfId_); 
							}
							switch(field->ilta)
							{
							case 0x00000000: break;
							case 0x00000001: CP_XML_ATTR(L"totalsRowFunction", L"average");	break;
							case 0x00000002: CP_XML_ATTR(L"totalsRowFunction", L"count");	break;
							case 0x00000003: CP_XML_ATTR(L"totalsRowFunction", L"countNums"); break;
							case 0x00000004: CP_XML_ATTR(L"totalsRowFunction", L"max");		break;
							case 0x00000005: CP_XML_ATTR(L"totalsRowFunction", L"min");		break;
							case 0x00000006: CP_XML_ATTR(L"totalsRowFunction", L"sum");		break;
							case 0x00000007: CP_XML_ATTR(L"totalsRowFunction", L"stdDev");	break;
							case 0x00000008: CP_XML_ATTR(L"totalsRowFunction", L"var");		break;
							case 0x00000009: CP_XML_ATTR(L"totalsRowFunction", L"custom");	break;
							}
							if (field->fmla.bFmlaExist)
							{
								CP_XML_NODE(L"calculatedColumnFormula")
								{
									CP_XML_STREAM() << field->fmla.fmla.getAssembledFormula();
								}
							}
							if (field->fLoadTotalFmla)
							{
								CP_XML_NODE(L"totalsRowFormula")
								{
									if (field->fLoadTotalArray)
										CP_XML_STREAM() << field->totalArrayFmla.getAssembledFormula();
									else 
										CP_XML_STREAM() << field->totalFmla.getAssembledFormula();
								}
							}
						}
					}
				}
			}

			if (table_style)
			{
				CP_XML_NODE(L"tableStyleInfo")
				{
					CP_XML_ATTR(L"name",			table_style->stListStyleName.value());	
					CP_XML_ATTR(L"showFirstColumn", table_style->fFirstColumn);	
					CP_XML_ATTR(L"showLastColumn",	table_style->fLastColumn);	
					CP_XML_ATTR(L"showRowStripes",	table_style->fRowStripes);	
					CP_XML_ATTR(L"showColumnStripes", table_style->fColumnStripes);	
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

