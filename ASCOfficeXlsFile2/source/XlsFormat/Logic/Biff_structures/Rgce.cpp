
#include "Rgce.h"
#include "PtgFactory.h"
#include <Binary/CFRecord.h>
//#include <Exception/StructureSizeNotSet.h>
#include "PtgExp.h"
#include "PtgTbl.h"

namespace XLS
{;


Rgce::Rgce(const CellRef& cell_base_ref_init)
:	cell_base_ref(cell_base_ref_init)
{
}


BiffStructurePtr Rgce::clone()
{
	return BiffStructurePtr(new Rgce(*this));
}

//void Rgce::toXML(BiffStructurePtr & parent)
//{
//	// Filtering Ptg records before serializing to XML. No Ptg except PtgExp is necessary for XSL conversion
//	PtgPtr ptg_exp;
//	for (PtgVectorIterator it = sequence.begin(), itEnd = sequence.end(); it != itEnd; ++it)
//	{
//		ptg_exp = boost::dynamic_pointer_cast<PtgExp>(*it);
//		if(ptg_exp)
//		{
//			MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//			ptg_exp->toXML(own_tag);
//			return;
//		}
//	}
//
//}


void Rgce::load(CFRecord& record, const size_t cce_val)
{
	cce = cce_val;
	size_t where_to_stop = record.getRdPtr() + cce_val;
	while(record.getRdPtr() < where_to_stop)
	{
		sequence.push_back(PtgFactory::loadPtg(record, cell_base_ref));
	}
}

//
//void Rgce::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	PtgExpPtr ptg_exp(new PtgExp);
//	if(ptg_exp->fromXML(xml_tag))
//	{
//		sequence.push_back(ptg_exp);
//	}
//	PtgTblPtr ptg_tbl(new PtgTbl);
//	if(ptg_tbl->fromXML(xml_tag))
//	{
//		sequence.push_back(ptg_tbl);
//	}
//}


void Rgce::store(CFRecord& record)
{
	size_t rgce_start_offset = record.getDataSize();

	for(PtgVector::const_iterator it = sequence.begin(), itEnd = sequence.end(); it != itEnd; ++it)
	{
		record << **it;
	}
	cce = record.getDataSize() - rgce_start_offset;
}


void Rgce::load(CFRecord& record)
{
	size_t where_to_stop = record.getRdPtr() + cce.get_value_or(0);
	while(record.getRdPtr() < where_to_stop)
	{
		sequence.push_back(PtgFactory::loadPtg(record, cell_base_ref));
	}
}


const size_t Rgce::getCCE() const
{
	return cce.get_value_or(0);
}


void Rgce::setCCE(const size_t cce_val)
{
	cce = cce_val;
}


const PtgVector& Rgce::getPtgs() const
{
	return sequence;
}


void Rgce::addPtg(const PtgPtr ptg)
{
	sequence.push_back(ptg);
}


const bool Rgce::isEmpty() const
{
	return sequence.empty();
}


const CellRef& Rgce::getLocation() const
{
	return cell_base_ref;
}

} // namespace XLS

