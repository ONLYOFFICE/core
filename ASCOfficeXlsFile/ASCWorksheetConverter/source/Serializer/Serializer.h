#pragma once

#include <Document/Document.h>

//namespace XLS
//{;

class Document;

class Serializer
{
public:
	virtual const bool read(Document& doc, const _bstr_t& from) = 0;
	virtual const bool write(const Document& doc, const _bstr_t& to) = 0;

};




//} // namespace XLS

