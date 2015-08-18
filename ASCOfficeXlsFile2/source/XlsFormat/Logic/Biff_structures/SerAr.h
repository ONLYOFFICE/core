#pragma once

#include "BiffStructure.h"

namespace XLS
{

class SerAr;
typedef boost::shared_ptr<SerAr> SerArPtr;

class SerAr : public BiffStructure
{
public:
    enum SerType
    {
        typeSerNil  = 0x00,
        typeSerNum  = 0x01,
        typeSerStr  = 0x02,
        typeSerBool = 0x04,
        typeSerErr  = 0x10

    };

	static SerArPtr createSerAr(const unsigned char type);

	virtual const std::wstring toString() const = 0;

	static const ElementType	type = typeSerAr;

    SerType fixed_type;
};

typedef boost::shared_ptr<SerAr> SerArPtr;

} // namespace XLS
