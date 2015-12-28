
#include "AXES.h"
#include <Logic/Biff_records/PlotArea.h>
#include <Logic/Biff_records/CatSerRange.h>
#include <Logic/Biff_records/Axis.h>

#include <Logic/Biff_unions/IVAXIS.h>
#include <Logic/Biff_unions/DVAXIS.h>
#include <Logic/Biff_unions/SERIESAXIS.h>
#include <Logic/Biff_unions/ATTACHEDLABEL.h>
#include <Logic/Biff_unions/FRAME.h>
#include <Logic/Biff_unions/AXS.h>

namespace XLS
{


AXES::AXES()
{
}


AXES::~AXES()
{
}


//  (DVAXIS DVAXIS)
class Parenthesis_AXES_3: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_AXES_3)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_AXES_3(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<DVAXIS>())
		{
			return false;
		}
		proc.mandatory<DVAXIS>();
		return true;
	};
};



// (IVAXIS DVAXIS [SERIESAXIS])
class Parenthesis_AXES_2: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_AXES_2)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_AXES_2(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<IVAXIS>())
		{
			return false;
		}

		proc.mandatory<DVAXIS>();
		proc.optional<SERIESAXIS>();
		return true;
	};
};



//  [IVAXIS DVAXIS [SERIESAXIS] / DVAXIS DVAXIS]
// this is equal to [(IVAXIS DVAXIS [SERIESAXIS]) / (DVAXIS DVAXIS)]
class Parenthesis_AXES_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_AXES_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_AXES_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		return	proc.optional<Parenthesis_AXES_2>() ||
				proc.optional<Parenthesis_AXES_3>();
	};
};



BaseObjectPtr AXES::clone()
{
	return BaseObjectPtr(new AXES(*this));
}


// AXES = [IVAXIS DVAXIS [SERIESAXIS] / DVAXIS DVAXIS] *3ATTACHEDLABEL [PlotArea FRAME]
const bool AXES::loadContent(BinProcessor& proc)
{
	if (proc.optional<Parenthesis_AXES_1>())
	{
		int count = elements_.size();

		while(count > 0)
		{
			IVAXIS		* iv	= dynamic_cast<IVAXIS*>		(elements_.front().get());
			DVAXIS		* dv	= dynamic_cast<DVAXIS*>		(elements_.front().get());
			SERIESAXIS	* ser	= dynamic_cast<SERIESAXIS*>	(elements_.front().get());

			int id		= -1;
			int type	= -1;

			Axis * axis = NULL;
			AXS * axs	= NULL;

			if (iv) 
			{	
				id		= iv->id;
				type	= 1;

				axis = dynamic_cast<Axis *>(iv->m_Axis.get());
				axs	= dynamic_cast<AXS *>(iv->m_AXS.get());
			}
			if (dv)
			{
				id		= dv->id;
				type	= 2;
				
				axis = dynamic_cast<Axis *>(dv->m_Axis.get());
				axs	= dynamic_cast<AXS *>(dv->m_AXS.get());
				
			}
			if (ser)
			{
				id		= ser->id;
				type	= 3;
			}
			if ((axis) && (axis->wType == (_UINT16)0) && axs)
			{
				if (!m_Wall_FRAME)	m_Wall_FRAME = axs->m_GELFRAME;
				if (!m_Wall_FRAME)	m_Wall_FRAME = axs->m_AreaFormat;
			}
			if ((axis) && (axis->wType == (_UINT16)1) && axs)
			{
				if (!m_Floor_FRAME)	m_Floor_FRAME = axs->m_GELFRAME;
				if (!m_Floor_FRAME)	m_Floor_FRAME = axs->m_AreaFormat;
			}

			if (id > 0)
			{
				m_arAxesId.push_back(std::pair<int, int>(id, type));
				m_arAxes.push_back(elements_.front());
			}
			elements_.pop_front();
			count--;
		}
	}

	int count = proc.repeated<ATTACHEDLABEL>(0, 3);
	while(count > 0)
	{
		m_arATTACHEDLABEL.insert(m_arATTACHEDLABEL.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}

	if(proc.optional<PlotArea>())
	{
		//m_PlotArea = elements_.back();
		elements_.pop_back(); //no data
	
		proc.mandatory<FRAME>();
		
		m_PlotArea_FRAME = elements_.back();
		elements_.pop_back();
	}

	return true;
}

int AXES::serialize(std::wostream & _stream)
{
	return serialize(_stream, false);
}

int AXES::serialize(std::wostream & _stream, bool secondary)
{
	CatSerRange * iv_CatSerRange = NULL;

	for (int i = 0 ; i < m_arAxes.size(); i++)
	{
		IVAXIS * iv = dynamic_cast<IVAXIS*>	(m_arAxes[i].get());
		if (iv)
		{
			iv_CatSerRange = dynamic_cast<CatSerRange*>	(iv->m_CatSerRange.get());			
			break;
		}

	}
	CP_XML_WRITER(_stream)    
	{
		for (int i = 0 ; i < m_arAxes.size(); i++)
		{
			IVAXIS		* iv	= dynamic_cast<IVAXIS*>		(m_arAxes[i].get());
			DVAXIS		* dv	= dynamic_cast<DVAXIS*>		(m_arAxes[i].get());
			SERIESAXIS	* ser	= dynamic_cast<SERIESAXIS*>	(m_arAxes[i].get());

			std::wstring node_ax_type = L"c:valAx";
			if (iv)
			{
				if (iv->bDataAxis) node_ax_type = L"c:dateAx";
				else node_ax_type = L"c:catAx";

				iv->m_bSecondary = secondary;
			}
			if (ser) 
			{
				node_ax_type = L"c:serAx";
				ser->m_bSecondary = secondary;
			}
			if (dv) 
			{
				dv->m_bSecondary = secondary;
			}
			CP_XML_NODE(node_ax_type)
			{
				if (i < m_arATTACHEDLABEL.size())
				{
					CP_XML_NODE(L"c:title")
					{
						ATTACHEDLABEL	*label = dynamic_cast<ATTACHEDLABEL *>	(m_arATTACHEDLABEL[i].get() );
						
						label->serialize(CP_XML_STREAM(), false);

					}
				}
				int axes_type = m_arAxes[i]->serialize(CP_XML_STREAM());
				
				//CP_XML_NODE(L"c:axPos")
				//{
				//	if (axes_type == 1)
				//	{
				//		if (secondary)	CP_XML_ATTR(L"val", L"t");
				//		else			CP_XML_ATTR(L"val", L"b");
				//	}
				//	else if (axes_type == 2)
				//	{
				//		if (secondary)	CP_XML_ATTR(L"val", L"r");
				//		else			CP_XML_ATTR(L"val", L"l");
				//	}
				//	else
				//	{
				//		if (secondary)	CP_XML_ATTR(L"val", L"t");
				//		else			CP_XML_ATTR(L"val", L"b");
				//	}
				//}

				for (int j = 0 ; j < m_arAxesId.size(); j++)
				{
					if (m_arAxesId[j].first != m_arAxesId[i].first && m_arAxesId[j].second != 3)
					{
						if (m_arAxesId[i].second == 3 && m_arAxesId[j].second == 1)continue;

						CP_XML_NODE(L"c:crossAx"){CP_XML_ATTR(L"val", m_arAxesId[j].first);}
					}
				}
				CP_XML_NODE(L"c:crossBetween")
				{
					if ((iv == NULL && iv_CatSerRange) && (iv_CatSerRange->fBetween == false))
					{
						CP_XML_ATTR(L"val", L"midCat"); 
					}
					else
					{
						CP_XML_ATTR(L"val", L"between"); 
					}
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

