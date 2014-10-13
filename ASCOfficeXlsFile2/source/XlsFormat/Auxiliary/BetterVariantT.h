#pragma once

#include <comutil.h>

namespace AUX
{;

class BetterVariantT : public _variant_t
{
public:
	BetterVariantT(const _variant_t& varSrc) ;
	operator double() const ;                          // Extracts a double from a VT_R8

};



} // namespace AUX



