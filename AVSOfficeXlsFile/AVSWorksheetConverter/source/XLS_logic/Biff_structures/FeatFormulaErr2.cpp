#include "stdafx.h"
#include "FeatFormulaErr2.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


FeatFormulaErr2::FeatFormulaErr2()
{
}


FeatFormulaErr2::FeatFormulaErr2(CFRecord& record)
{
	load(record);
}


BiffStructurePtr FeatFormulaErr2::clone()
{
	return BiffStructurePtr(new FeatFormulaErr2(*this));
}


void FeatFormulaErr2::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"ffecCalcError", grffecIgnore.ffecCalcError);
	own_tag->setAttribute(L"ffecEmptyCellRef", grffecIgnore.ffecEmptyCellRef);
	own_tag->setAttribute(L"ffecNumStoredAsText", grffecIgnore.ffecNumStoredAsText);
	own_tag->setAttribute(L"ffecInconsistRange", grffecIgnore.ffecInconsistRange);
	own_tag->setAttribute(L"ffecInconsistFmla", grffecIgnore.ffecInconsistFmla);
	own_tag->setAttribute(L"ffecTextDateInsuff", grffecIgnore.ffecTextDateInsuff);
	own_tag->setAttribute(L"ffecUnprotFmla", grffecIgnore.ffecUnprotFmla);
	own_tag->setAttribute(L"ffecDataValidation", grffecIgnore.ffecDataValidation);
}


const bool FeatFormulaErr2::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### FeatFormulaErr2 record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! FeatFormulaErr2 record has no BiffStructure::fromXML() implemented.");
	return false;
}


void FeatFormulaErr2::store(CFRecord& record)
{
	record << grffecIgnore;
}


void FeatFormulaErr2::load(CFRecord& record)
{
	record >> grffecIgnore;
}


} // namespace XLS

