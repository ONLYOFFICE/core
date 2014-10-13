#include "precompiled_xls.h"
#include "TxORuns.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr TxORuns::clone()
{
	return BiffStructurePtr(new TxORuns(*this));
}

TxORuns::TxORuns()
{		
	m_runCount = 0;
}

TxORuns::~TxORuns()
{
}

//void TxORuns::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{	
//	std::for_each(rgTxoRuns.begin(), rgTxoRuns.end(), boost::bind(&Run::toXML, _1, xml_tag));
//	lastRun.toXML(xml_tag);
//}
//
//void TxORuns::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{	
//}

void TxORuns::store(CFRecord& record)
{	
}

void TxORuns::load(CFRecord& record)
{
	for (int i = 0; i < m_runCount; i++)
	{
		RunPtr run(new Run);
		run->load(record);
		rgTxoRuns.push_back(run);
	}
	lastRun.load(record);
}


} // namespace XLS

