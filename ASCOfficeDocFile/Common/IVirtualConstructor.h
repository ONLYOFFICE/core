#pragma once

struct IVirtualConstructor
{
	virtual IVirtualConstructor* New() const = 0;
	virtual IVirtualConstructor* Clone() const = 0;
	virtual ~IVirtualConstructor() {}
};
