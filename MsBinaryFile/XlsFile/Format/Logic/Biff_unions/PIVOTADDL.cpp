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

#include "PIVOTADDL.h"

#include "../Biff_records/SXAddl.h"

namespace XLS
{


PIVOTADDL::PIVOTADDL() : current( &content), m_SXADDLCONDFMTS(NULL, -1), m_SXADDLSXFILTERS12(NULL, -1)
{
	_sxAddl elm(NULL, 0);
	current->push_back(elm);
}

PIVOTADDL::~PIVOTADDL()
{
}

BaseObjectPtr PIVOTADDL::clone()
{
	return BaseObjectPtr(new PIVOTADDL(*this));
}


/*
PIVOTADDL = SXAddl_SXCView_SXDId *Continue_SxaddlSxString 
			[SXAddl_SXCView_SXDVer10Info] 
			[SXAddl_SXCView_SXDVer12Info] *SXADDLCALCMEMBER *SXADDLHIERARCHY *SXADDLFIELD *UNKNOWNFRT 
			[SXAddl_SXCView_SXDTableStyleClient] 
			[SXAddl_SXCView_SXDCompactRwHdr *Continue_SxaddlSxString] 
			[SXAddl_SXCView_SXDCompactColHdr *Continue_SxaddlSxString] 
			[SXAddl_SXCView_SXDVerUpdInv] 
			[SXADDLCONDFMTS] 
			[SXADDLSXFILTERS12] *SXAddl_SXCView_SXDVerUpdInv *SXAddl_SXCView_SXDSXPIIvmb 
			[SXAddl_SXCView_SXDVerUpdInv] SXAddl_SXCView_SXDEnd
*/

const bool PIVOTADDL::loadContent(BinProcessor& proc)
{
	bool result = false;
	int level = 0;
	while (true)
	{
		CFRecordType::TypeId type = proc.getNextRecordType();	

		if (type != rt_SXAddl) break;
	
		proc.optional<SXAddl>();

		SXAddl* addl = dynamic_cast<SXAddl*>(elements_.back().get());				
		if (!addl) continue;
		
		if (result && addl->bStartElement)
		{
			level++;
			_sxAddl elm(current, level); 

			current = &current->back().levels;
			
			current->push_back(elm);
		}
		
		result = true;
		
		if (level == 0 && addl->bEndElement)
		{
			elements_.pop_back(); //end элемент не нужен в дальнейшем
			continue;
		}
		else if (addl->bEndElement)
		{ 			
			level--;
			std::vector<_sxAddl> *current_old = current;
			
			_sxAddl & current_level = current->back();
			
			current = current_level.prev;			
			current_level.prev = NULL;

			if (level == 0)
			{
				switch(addl->sxc)
				{
					case 0x01:	
					case 0x17:
								m_arSXADDLFIELD.push_back(current_level);		break;
					case 0x02:	m_arSXADDLHIERARCHY.push_back(current_level);	break;
					case 0x03:	m_arSXADDLCALCMEMBER.push_back(current_level);	break;
					case 0x1a:	m_SXADDLCONDFMTS	= current_level;			break;
					case 0x1b:	m_arSXADDLCONDFMT.push_back(current_level);		break;
					case 0x1c:	m_SXADDLSXFILTERS12 = current_level;			break;
				}
			}			
			
			elements_.pop_back(); //end элемент не нужен в дальнейшем
			continue;
		}
		else if (level == 0)
		{
			switch(addl->sxd)
			{
				case 0x00: m_SXAddl_SXCView_SXDId				= addl->content;	break;
 				case 0x02: m_SXAddl_SXCView_SXDVer10Info		= addl->content;    break;
				case 0x19: m_SXAddl_SXCView_SXDVer12Info		= addl->content;    break;
				case 0x1E: m_SXAddl_SXCView_SXDTableStyleClient = addl->content;	break;
			}
			if (addl->sxd == 0x00 || addl->sxd == 0x02 || addl->sxd == 0x19 || addl->sxd == 0x1e)
			{
				elements_.pop_back();
				continue;
			}
		}

		current->back().elements.push_back(elements_.back());
		elements_.pop_back();
	}
	current = NULL;
	return result;
}

} // namespace XLS

