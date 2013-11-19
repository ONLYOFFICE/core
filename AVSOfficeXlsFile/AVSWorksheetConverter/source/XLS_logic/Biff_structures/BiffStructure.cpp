#include "stdafx.h"
#include "BiffStructure.h"
#include <XLS_bin/CFRecord.h>
#include <Exception/UnexpectedProgramPath.h>
#include <Exception/AttributeNotFound.h>
#include <Exception/ChildNodeNotFound.h>
#include <Exception/WrongAPIUsage.h>

namespace XLS
{;


// this function will never be called ( look at operator>>(CFRecord& record, T& val))
void BiffStructure_NoVtbl::store(CFRecord& record)
{
#pragma message("####################### BiffStructure_NoVtbl record has no BiffStructure::store() implemented")
	Log::error(" Error!!! BiffStructure_NoVtbl record has no BiffStructure::store() implemented.");
	//record << something;
}


// this function will never be called ( look at operator>>(CFRecord& record, T& val))
void BiffStructure_NoVtbl::load(CFRecord& record)
{
	throw EXCEPT::LE::HowCouldItHappen("Unexpected behaviour.", __FUNCTION__);
}


bool DiffBiff(BiffStructure_NoVtbl& val)
{
	return true;
}


bool DiffBiff(BiffStructure& val)
{
	return false;
}


const AUX::BetterVariantT BiffStructure::getStructAttribute(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name)
{
	_variant_t var = parent->getAttribute(attrib_name);
	if(VT_NULL == var.vt)
	{
		throw EXCEPT::LE::AttributeNotFound(static_cast<char*>(attrib_name), static_cast<char*>(parent->GetnodeName()));
	}
	return var;
}


MSXML2::IXMLDOMElementPtr BiffStructure::getStructNode(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& tag_name)
{
	MSXML2::IXMLDOMElementPtr node = parent->selectSingleNode(tag_name + L"[position() = 1]");
	if(node)
	{

	}
	return node;
}


// This is a stub for that descendants of BiffStructure those are not nested from BiffStructTagged
void BiffStructure::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	// Can only be called by mistake;
	throw EXCEPT::LE::WrongAPIUsage("The specified function is not overridden in '" + std::string(static_cast<char*>(getClassName())) + "' class.", __FUNCTION__);
}


// This is a stub for that descendants of BiffStructure those are not nested from BiffAttribute
const bool BiffStructure::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
	// Can only be called by mistake;
	throw EXCEPT::LE::WrongAPIUsage("The specified function is not overridden in '" + std::string(static_cast<char*>(getClassName())) + "' class.", __FUNCTION__);
}


// This is a stub for that descendants of BiffStructure those are not nested from BiffAttribute
const bool BiffStructure::fromXML(MSXML2::IXMLDOMElementPtr parent, const size_t position, const bool is_mandatory)
{
	// Can only be called by mistake;
	throw EXCEPT::LE::WrongAPIUsage("The specified function is not overridden in '" + std::string(static_cast<char*>(getClassName())) + "' class.", __FUNCTION__);
}



}// namespace XLS
