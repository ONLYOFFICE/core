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

#include "SUPBOOK.h"

#include "../Biff_records/SupBook.h"
#include "../Biff_records/ExternName.h"
#include "../Biff_records/ExternSheet.h"
#include "../Biff_records/Continue.h"
#include "../Biff_records/XCT.h"
#include "../Biff_records/CRN.h"

#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

SUPBOOK::SUPBOOK() : nExternIndex(-1) 
{}

SUPBOOK::~SUPBOOK()
{}

class Parenthesis_SUPBOOK_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SUPBOOK_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SUPBOOK_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<XCT>())
		{
			return false;
		}
		proc.repeated<CRN>(0, 0);
		return true;
	};
};



BaseObjectPtr SUPBOOK::clone()
{
	return BaseObjectPtr(new SUPBOOK(*this));
}


// SUPBOOK = SupBook [*ExternName *(XCT *CRN)] [ExternSheet] *Continue
const bool SUPBOOK::loadContent(BinProcessor& proc)
{
	global_info = proc.getGlobalWorkbookInfo();

	SupBook supbook;
	if(!proc.mandatory(supbook))
	{
		return false;
	}
	m_SupBook = elements_.back();
	elements_.pop_back();

	if(0x3A01 != supbook.cch && 0x0401 != supbook.cch)	
	{
		nExternIndex = global_info->last_Extern_id++;
	}
	
	while(true)
	{
		ExternName extern_name(supbook.cch, supbook.bOleLink);
		if(!proc.optional(extern_name))
		{
			break;
		}
		m_arExternName.push_back(elements_.front());	elements_.pop_front();
//--------------------------------------------------------------------------------------------
		std::wstring name; 
		
		ExternDocName* docName = dynamic_cast<ExternDocName*>(extern_name.body.get());
		if(docName)
		{
			if (docName->ixals > 0 && docName->ixals < supbook.rgst.size())
			{
				name = supbook.rgst[docName->ixals - 1];
			}
			else
			{
				//name = docName->nameDefinition.getAssembledFormula(); //in  update !!
				if (name.empty())
				{
					name = docName->extName.value();
				}
			}
		}
		ExternDdeLinkNoOper* ddeLink = dynamic_cast<ExternDdeLinkNoOper*>(extern_name.body.get());
		if (ddeLink)
		{
			name = ddeLink->linkName.value();
		}

		ExternOleDdeLink* oleDdeLink = dynamic_cast<ExternOleDdeLink*>(extern_name.body.get());
		if (oleDdeLink)
		{
			name = oleDdeLink->linkName.value();
		}

		AddinUdf *addinUdf = dynamic_cast<AddinUdf*>(extern_name.body.get());
		if (addinUdf)
		{
			name = addinUdf->udfName.value(); //_xll.name(
		}
		
		if ( std::wstring::npos != name.find(L";") )
			name = L"'" + name + L"'";

		arNames.push_back(name);
	}

	int count = proc.repeated<Parenthesis_SUPBOOK_1>(0, 0);
	while(!elements_.empty())
	{
		if ("XCT" == elements_.front()->getClassName())
		{
			_xct xcf;

			xcf.m_XCT = elements_.front();
			count--;
			m_arXCT.push_back(xcf);
		}
		else
		{
			if (m_arXCT.empty())
			{
				break; // error !!!
			}
			m_arXCT.back().m_arCRN.push_back(elements_.front());
		}
		elements_.pop_front();
	}	
	if (proc.optional<ExternSheet>())
	{
		m_ExternSheet = elements_.back();
		elements_.pop_back();
	}
	return true;
}
bool SUPBOOK::IsExternal()
{
	SupBook *book = dynamic_cast<SupBook*>(m_SupBook.get());
	
	if (!book) return false;

	if(0x3A01 == book->cch || 0x0401 == book->cch)
	{
		return false;
	}
	return true;
}

int SUPBOOK::serialize(std::wostream & strm)
{
	SupBook *book = dynamic_cast<SupBook*>(m_SupBook.get());
	if (!book) return 0;
		
	if(0x3A01 == book->cch || 0x0401 == book->cch)
	{
		return 0;
	}

	if (book->bOleLink)
	{
		serialize_dde(strm);
	}
	else
	{
		serialize_book(strm);
	}

	return 0;
}
int SUPBOOK::serialize_book(std::wostream & strm)
{
	SupBook *book = dynamic_cast<SupBook*>(m_SupBook.get());

    if (book->virtPath.empty())
        return 0;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"externalBook")
		{ 
			CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships"); 
			CP_XML_ATTR(L"r:id", L"rId1"); 
			
            sExternPathLink = book->virtPath.back();

			XmlUtils::replace_all(sExternPathLink, L" ", L"%20");
			
			if (!m_arXCT.empty() && !book->rgst.empty())
			{	
				CP_XML_NODE(L"sheetNames")
				{ 
					for (size_t i = 0; i < book->rgst.size(); i++)
					{
						CP_XML_NODE(L"sheetName")
						{
							CP_XML_ATTR(L"val", book->rgst[i]); 
						}
					}
					for (size_t i = 0; i < m_arXCT.size(); i++)
					{
						XCT * name = dynamic_cast<XCT*>(m_arXCT[i].m_XCT.get());
						if ((name) && (name->itab_exist))
						{
						}
						else 
						{
							CP_XML_NODE(L"sheetName")
							{			
								CP_XML_ATTR(L"val", L"Sheet " + std::to_wstring(i + 1)); 
							}
						}
					}
				}
				if (false == arNames.empty())
				{
					CP_XML_NODE(L"definedNames")
					{
						for (size_t j = 0; j < arNames.size(); j++)
						{
							CP_XML_NODE(L"definedName")
							{
								CP_XML_ATTR(L"name", arNames[j]); 
								
								std::map<std::wstring, _def_name>::iterator pFind = mapNamesExt.find(arNames[j]);
								if (pFind != mapNamesExt.end())
								{
									CP_XML_ATTR(L"refersTo", pFind->second.fmla); 
									
									if (pFind->second.sheetId > 0)
										CP_XML_ATTR(L"sheetId", pFind->second.sheetId); 
								}
							}
						}
					}
				}
				CP_XML_NODE(L"sheetDataSet")
				{ 
					for (size_t i = 0; i < m_arXCT.size(); i++)
					{
						XCT * name = dynamic_cast<XCT*>(m_arXCT[i].m_XCT.get());
						if (!name) continue;
						
						CP_XML_NODE(L"sheetData")
						{
							CP_XML_ATTR(L"sheetId", name->itab); 
							
							int current_row = -1;							
							for (size_t j = 0; j < m_arXCT[i].m_arCRN.size(); j++)
							{
								CRN * cell = dynamic_cast<CRN*>(m_arXCT[i].m_arCRN[j].get());
								if (!cell) continue;
								if (cell->row != current_row)
								{										
									CP_XML_NODE(L"row")
									{
										current_row = cell->row;
										CP_XML_ATTR(L"r", cell->row + 1); 
										
										for (size_t k = j; k < m_arXCT[i].m_arCRN.size(); k++)
										{
											cell = dynamic_cast<CRN*>(m_arXCT[i].m_arCRN[k].get());
											if (cell->row != current_row)
											{
												j = k - 1;
												break;
											}

											for (unsigned char col = cell->colFirst, v = 0; col <= cell->colLast; col++, v++)
											{
												CP_XML_NODE(L"cell")
												{
													SerAr* val = dynamic_cast<SerAr*>(cell->crnOper[v].get());

													CellRef ref(cell->row, col, true, true);
													
													CP_XML_ATTR(L"r", ref.toString()); 

													std::wstring strVal = val->toString();
													if (val->fixed_type == SerAr::typeSerStr)
													{
														if (0 == strVal.find(L"\""))
															strVal = strVal.substr(1, strVal.length() - 2);
													}

													switch (val->fixed_type)
													{
														case SerAr::typeSerNil:		CP_XML_ATTR(L"t", L"nil");	break;
														case SerAr::typeSerNum:		CP_XML_ATTR(L"t", L"n");	break;
														case SerAr::typeSerStr:		CP_XML_ATTR(L"t", L"str");	break;
														case SerAr::typeSerBool:	CP_XML_ATTR(L"t", L"b");	break;
														case SerAr::typeSerErr:		CP_XML_ATTR(L"t", L"e");	break;
													}
													CP_XML_NODE(L"v")
													{
														CP_XML_STREAM() << XmlUtils::EncodeXmlString(strVal);
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			else if (!book->rgst.empty())
			{
				CP_XML_NODE(L"sheetNames")
				{ 
					for (size_t i = 0; i < book->rgst.size(); i++)
					{
						CP_XML_NODE(L"sheetName")
						{
							CP_XML_ATTR(L"val", book->rgst[i]); 
						}
					}
				}
				CP_XML_NODE(L"sheetDataSet")
				{ 
					for (size_t i = 0; i < book->rgst.size(); i++)
					{
						CP_XML_NODE(L"sheetData")
						{
							CP_XML_ATTR(L"sheetId", i); 
							CP_XML_ATTR(L"refreshError", 1); 
						}
					}
				}
			}
		}
	}
	return 0;
}
int SUPBOOK::serialize_dde(std::wostream & strm)
{
	SupBook *book = dynamic_cast<SupBook*>(m_SupBook.get());

    if (book->virtPath.empty())
        return 0;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"ddeLink")
		{
			CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships"); 
			CP_XML_ATTR(L"ddeService", book->virtPath[0]); 
			CP_XML_ATTR(L"ddeTopic", book->virtPath[1]); 

			CP_XML_NODE(L"ddeItems")
			{ 	
				for (size_t i = 0; i < m_arExternName.size(); i++)
				{
					ExternName * external_name = dynamic_cast<ExternName*>(m_arExternName[i].get());

					if (!external_name) continue;

					ExternOleDdeLink *oleDdeLink = dynamic_cast<ExternOleDdeLink*>(external_name->body.get());
					if ((oleDdeLink) && (oleDdeLink->lStgName == 0))
					{
						CP_XML_NODE(L"ddeItem")
						{  
							CP_XML_ATTR(L"name",	oleDdeLink->linkName.value());
							CP_XML_ATTR(L"advise",	external_name->fWantAdvise);

							oleDdeLink->moper.serialize(CP_XML_STREAM());
						}
					}
					//ole items in oleLink
				}
			}
		}

	}
	return 0;
}

} // namespace XLS

