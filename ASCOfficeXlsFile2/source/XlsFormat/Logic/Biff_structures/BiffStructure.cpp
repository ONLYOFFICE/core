#include "precompiled_xls.h"
#include "BiffStructure.h"
#include <Binary/CFRecord.h>
#include <Document/Document.h>

namespace XLS
{;

Document* BiffStructure::get_document() 
{
	return document_;
}

void BiffStructure::set_document(Document* d) 
{
	document_ = d;
} 

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
	throw;// EXCEPT::LE::HowCouldItHappen("Unexpected behaviour.", __FUNCTION__);
}


bool DiffBiff(BiffStructure_NoVtbl& val)
{
	return true;
}


bool DiffBiff(BiffStructure& val)
{
	return false;
}

//
//const AUX::BetterVariantT BiffStructure::getStructAttribute(BiffStructurePtr & parent, const std::wstring & attrib_name)
//{
//	_variant_t var = parent->getAttribute(attrib_name);
//	if(VT_NULL == var.vt)
//	{
//		throw;// EXCEPT::LE::AttributeNotFound(static_cast<char*>(attrib_name), static_cast<char*>(parent->GetnodeName()));
//	}
//	return var;
//}
//
//
//MSXML2::IXMLDOMElementPtr BiffStructure::getStructNode(BiffStructurePtr & parent, const std::wstring & tag_name)
//{
//	MSXML2::IXMLDOMElementPtr node = parent->selectSingleNode(tag_name + L"[position() = 1]");
//	if(node)
//	{
//
//	}
//	return node;
//}
//
//
//// This is a stub for that descendants of BiffStructure those are not nested from BiffStructure
//void BiffStructure::toXML(BiffStructurePtr & parent)
//{
//	// Can only be called by mistake;
//	throw;// EXCEPT::LE::WrongAPIUsage("The specified function is not overridden in '" + std::string(static_cast<char*>(getClassName())) + "' class.", __FUNCTION__);
//}
//
//
//// This is a stub for that descendants of BiffStructure those are not nested from BiffAttribute
//const bool BiffStructure::fromXML(BiffStructurePtr & parent)
//{
//	// Can only be called by mistake;
//	throw;// EXCEPT::LE::WrongAPIUsage("The specified function is not overridden in '" + std::string(static_cast<char*>(getClassName())) + "' class.", __FUNCTION__);
//}
//
//
//// This is a stub for that descendants of BiffStructure those are not nested from BiffAttribute
//const bool BiffStructure::fromXML(BiffStructurePtr & parent, const size_t position, const bool is_mandatory)
//{
//	// Can only be called by mistake;
//	throw;// EXCEPT::LE::WrongAPIUsage("The specified function is not overridden in '" + std::string(static_cast<char*>(getClassName())) + "' class.", __FUNCTION__);
//}
//


}// namespace XLS
