#pragma once

struct IOperand
{
	virtual operator unsigned char*() const			=	0;
	virtual operator const unsigned char*() const	=	0;
	virtual unsigned int Size() const		=	0;
	
	virtual ~IOperand() {}
};
