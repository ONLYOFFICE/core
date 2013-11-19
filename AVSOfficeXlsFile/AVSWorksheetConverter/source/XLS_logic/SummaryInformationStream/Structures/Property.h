#pragma once

namespace OLEPS
{;

// The only necessity of this class is to place all properties into a single array
class Property
{
public:
	Property();
	virtual ~Property() = 0;

	enum ValueTypes
	{
		VT_I2 = 0x0002,
		VT_I4 = 0x0003,
		VT_R4 = 0x0004,
		VT_R8 = 0x0005,
		VT_CY = 0x0006,
		VT_DATE = 0x0007,
		VT_BSTR = 0x0008,
		VT_ERROR = 0x000A,
		VT_BOOL = 0x000B,
		VT_DECIMAL = 0x000E,
		VT_I1 = 0x0010,
		VT_UI1 = 0x0011,
		VT_UI2 = 0x0012,
		VT_UI4 = 0x0013,
		VT_I8 = 0x0014,
		VT_UI8 = 0x0015,
		VT_INT = 0x0016,
		VT_UINT = 0x0017,
		VT_LPSTR = 0x001E,
		VT_LPWSTR = 0x001F,
		VT_FILETIME = 0x0040,
		VT_CF = 0x0047,
		VT_CLSID = 0x0047,
	};

};

typedef boost::shared_ptr<Property> PropertyPtr;

} // namespace OLEPS
