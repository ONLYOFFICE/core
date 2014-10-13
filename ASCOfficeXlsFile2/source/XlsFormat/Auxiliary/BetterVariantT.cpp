#include "precompiled_xls.h"

#include "BetterVariantT.h"
//#include <Exception/AttributeDataWrong.h>

namespace AUX
{;

BetterVariantT::BetterVariantT(const _variant_t& varSrc)
:	_variant_t(varSrc)
{
}

// Extracts a VT_R8 into a double
//
BetterVariantT::operator double() const 
{
	if (V_VT(this) == VT_R8) {
		return V_R8(this); 
	}
	_variant_t varDest;
	if(FAILED(::VariantChangeTypeEx(static_cast<VARIANT*>(&varDest),
							const_cast<VARIANT*>(static_cast<const VARIANT*>(this)), 1252, 0, VT_R8)))
	{
		throw;// EXCEPT::LE::WrongIntermediateXMLFormat("Type mismatch", __FUNCTION__);
	}
	return V_R8(&varDest);
}


} // namespace AUX

