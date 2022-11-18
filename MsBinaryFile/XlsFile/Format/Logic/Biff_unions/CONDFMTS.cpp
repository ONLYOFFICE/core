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

#include "CONDFMTS.h"
#include "CONDFMT.h"
#include "CONDFMT12.h"

#include "../Biff_records/CFEx.h"
#include "../Biff_records/CF12.h"
#include "../Biff_records/CF.h"
#include "../Biff_records/CondFmt.h"
#include "../Biff_records/CondFmt12.h"

namespace XLS
{


CONDFMTS::CONDFMTS()
{
}


CONDFMTS::~CONDFMTS()
{
}


class Parenthesis_CONDFMTS_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_CONDFMTS_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_CONDFMTS_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		return proc.optional<CONDFMT>() ||
				proc.optional<CONDFMT12>();
	};
};

class Parenthesis_CONDFMTS_2: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_CONDFMTS_2)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_CONDFMTS_2(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<CFEx>())
		{
			return false;
		}
		CellRef ref;
		CF12 cf (ref);
		proc.optional(cf); 
		return true;
	};
};



BaseObjectPtr CONDFMTS::clone()
{
	return BaseObjectPtr(new CONDFMTS(*this));
}


// CONDFMTS = *(CONDFMT / CONDFMT12) *(CFEx [CF12])
const bool CONDFMTS::loadContent(BinProcessor& proc)
{
	bool res = false;
	
	int count = proc.repeated<Parenthesis_CONDFMTS_1>(0, 0);
	if (count > 0) res = true;
	while(count > 0)
	{
		m_arCONDFMT.insert(m_arCONDFMT.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}

	count = proc.repeated<Parenthesis_CONDFMTS_2>(0, 0);
	if (count > 0) res = true;
	
	while(!elements_.empty())
	{
		if (elements_.front()->get_type() == typeCFEx)
		{
			_data_ex data_ex;
			data_ex.ex = elements_.front();
			m_arCFEx.push_back(data_ex);
		}
		else
		{
			if (!m_arCFEx.empty())
				m_arCFEx.back().cf12 = elements_.front();
		}
		elements_.pop_front();
	}
//----------------------------------------------------------------------------
	for (size_t i = 0 ; i < m_arCFEx.size(); i++)
	{
		CFEx * cfEx = dynamic_cast<CFEx *>(m_arCFEx[i].ex.get());
		if (cfEx)
		{
			size_t ind_cf = (cfEx->fIsCF12 == 0) ? cfEx->content.icf : i;

			for (size_t j = 0 ; j < m_arCONDFMT.size(); j++)
			{
				CONDFMT * CONDFMT_ = dynamic_cast<CONDFMT *>(m_arCONDFMT[j].get());
				CondFmt *condFmt = CONDFMT_ ? dynamic_cast<CondFmt *>(CONDFMT_->m_CondFmt.get()) : NULL;
				
				if ((condFmt) && (condFmt->nID == cfEx->nID))
				{
					if (ind_cf >= CONDFMT_->m_arCF.size() && m_arCFEx[i].cf12)//4-ое и более условия - 5804543.xls
					{
						CONDFMT_->m_arCF.push_back(m_arCFEx[i].cf12);
						
						//reset base ref
						CF12* cf12 = dynamic_cast<CF12 *>(m_arCFEx[i].cf12.get());
						if (cf12)
						{
							cf12->rgce1.set_base_ref(condFmt->getLocation());
							cf12->rgce2.set_base_ref(condFmt->getLocation());
							cf12->fmlaActive.set_base_ref(condFmt->getLocation());
						}

					}
					if (ind_cf < CONDFMT_->m_arCF.size())
					{
						CF* cf = dynamic_cast<CF *>(CONDFMT_->m_arCF[ind_cf].get());
						if (cf)
						{
							cf->m_CFEx = m_arCFEx[i].ex;
							cf->m_CF12 = m_arCFEx[i].cf12;
						}
						CF12* cf2 = dynamic_cast<CF12 *>(CONDFMT_->m_arCF[ind_cf].get());
						if (cf2)
						{
							cf2->m_CFEx = m_arCFEx[i].ex;
							cf2->m_CF12_2 = m_arCFEx[i].cf12;
						}
					}
				}
				CONDFMT12 * CONDFMT12_ = dynamic_cast<CONDFMT12 *>(m_arCONDFMT[j].get());
				if (CONDFMT12_ /*&& cfEx->fIsCF12 != 0*/)
				{
					CondFmt12 *condFmt12 = dynamic_cast<CondFmt12 *>(CONDFMT12_->m_CondFmt12.get());
					if ((condFmt12->mainCF.nID == cfEx->nID) && (ind_cf < CONDFMT12_->m_arCF12.size()))
					{
						CF12* cf = dynamic_cast<CF12 *>(CONDFMT12_->m_arCF12[ind_cf].get());
						if (cf)
						{
							cf->m_CFEx = m_arCFEx[i].ex;
							cf->m_CF12_2 = m_arCFEx[i].cf12;
						}
					}
				}
			}
		}
	}


	return res;
}

int CONDFMTS::serialize(std::wostream & stream)
{
	if (m_arCONDFMT.empty()) return 0;

	for (size_t i = 0 ; i < m_arCONDFMT.size(); i++)
	{
		if (!m_arCONDFMT[i]) continue;
		m_arCONDFMT[i]->serialize(stream);
	}

	return 0;
}
} // namespace XLS

