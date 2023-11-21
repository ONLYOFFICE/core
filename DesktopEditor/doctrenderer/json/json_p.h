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
			vtObject
		};

	public:
		CTypedValue();

	public:
		std::shared_ptr<IBaseValue> m_value;
		ValueType m_type;
	};

	// Container for CTypedValue which is also allows to treat typed value as reference
	class CTypedValueContainer
	{
	public:
		CTypedValueContainer();
		CTypedValueContainer(const CTypedValueContainer& other);

		CTypedValueContainer& operator=(const CTypedValueContainer& other);

	public:
		std::shared_ptr<CTypedValue> m_typedValue;
		/* We need some sort of "reference mode" for values, returned from arrays (with CValue::Get(int))
		 * and from objects (with CValue::Get(const char*)), because they should not be simply the COPY of that values, but
		 * instead they should be some sort of REFERENCES to original values, that are stored in array or object.
		 * That is because we want to be able to change them.
		 * m_isReference is TRUE for that type of values. For all the rest it is FALSE.
		 */
		bool m_isReference;
	};
}

#endif	// JSON_PRIVATE_H_
