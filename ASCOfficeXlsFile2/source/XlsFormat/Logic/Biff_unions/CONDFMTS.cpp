
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

