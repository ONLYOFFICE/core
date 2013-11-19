#pragma once

struct IOperand
{
	virtual operator byte*() const			=	0;
	virtual operator const byte*() const	=	0;
	virtual unsigned int Size() const		=	0;
	
	virtual ~IOperand() {}
};
