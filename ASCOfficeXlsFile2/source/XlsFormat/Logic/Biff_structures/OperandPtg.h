#pragma once

#include "Ptg.h"

namespace XLS
{;

class OperandPtg : public Ptg
{
public:
	OperandPtg() {};
	OperandPtg(const unsigned __int16 ptg_id_init) : Ptg(ptg_id_init) {};
	enum PtgDataType
	{
		pdt_NO_TYPE_DATA = 0x0,	// Error value or type data is absent
		pdt_REFERENCE = 0x1,	// Specifies a reference to a range.
		pdt_VALUE = 0x2,		// Specifies a single value of a simple type. The type can be a Boolean, a number, a string, or an error code.
		pdt_ARRAY = 0x3			// Specifies an array of values.
	};


protected:

};

typedef boost::shared_ptr<OperandPtg> OperandPtgPtr;

} // namespace XLS

