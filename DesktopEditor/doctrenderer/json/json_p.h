#ifndef JSON_PRIVATE_H_
#define JSON_PRIVATE_H_

#include <memory>

namespace NSJSON
{
	class IBaseValue;
	// Wrapper around IBaseValue with specific value type
	class CTypedValue
	{
	public:
		enum ValueType
		{
			vtUndefined,
			vtNull,
			vtPrimitive,
			vtArray,
			vtTypedArray,
			vtObject,
			vtImage
		};

	public:
		CTypedValue();
		CTypedValue(IBaseValue* value, ValueType type);
		~CTypedValue();
		// default copy behaviour
		CTypedValue(const CTypedValue& other) = default;
		CTypedValue& operator=(const CTypedValue& other) = default;

	public:
		std::shared_ptr<IBaseValue> m_value;
		ValueType m_type;
	};
}

#endif	// JSON_PRIVATE_H_
