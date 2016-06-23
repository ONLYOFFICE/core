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

#include "CONDFMTS.h"
#include <Logic/Biff_unions/CONDFMT.h>
#include <Logic/Biff_unions/CONDFMT12.h>
#include <Logic/Biff_records/CFEx.h>
#include <Logic/Biff_records/CF12.h>
#include <Logic/Biff_records/CF.h>
#include <Logic/Biff_records/CondFmt.h>
#include <Logic/Biff_records/CondFmt12.h>

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
		//proc.optional<CF12>(); // TODO: uncomment this and pass a base cell reference to the constructor
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
	while(count > 0)
	{
		m_arCFEx.insert(m_arCFEx.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}	

	for (int i = 0 ; i < m_arCFEx.size(); i++)
	{
		if (!m_arCFEx[i]) continue;

		CFEx * cfEx = dynamic_cast<CFEx *>(m_arCFEx[i].get());
		if (cfEx)
		{
			int ind_cf = cfEx->content.icf;

			for (int j = 0 ; j < m_arCONDFMT.size(); j++)
			{
				CONDFMT * CONDFMT_ = dynamic_cast<CONDFMT *>(m_arCONDFMT[j].get());
				if (CONDFMT_/* && cfEx->fIsCF12 == 0*/)
				{
					CondFmt *condFmt = dynamic_cast<CondFmt *>(CONDFMT_->m_CondFmt.get());
					if ((condFmt->nID == cfEx->nID) && (ind_cf < CONDFMT_->m_arCF.size()))
					{
						CF* cf = dynamic_cast<CF *>(CONDFMT_->m_arCF[ind_cf].get());
						if (cf)
							cf->m_CFEx = m_arCFEx[i];
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
							cf->m_CFEx = m_arCFEx[i];
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

	for (int i = 0 ; i < m_arCONDFMT.size(); i++)
	{
		if (!m_arCONDFMT[i]) continue;
		m_arCONDFMT[i]->serialize(stream);
	}

	return 0;
}
} // namespace XLS

