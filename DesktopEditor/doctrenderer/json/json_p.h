#ifndef JSON_PRIVATE_H_
#define JSON_PRIVATE_H_

#include <memory>

namespace NSJSON
{
	class IBaseValue;
	class CValueContainer
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
		CValueContainer();
		CValueContainer(ValueType type, IBaseValue* pValue);

		static CValueContainer DeepCopy(const CValueContainer& other);

	public:
		std::shared_ptr<IBaseValue> m_value;
		ValueType m_type;
	};
}

#endif	// JSON_PRIVATE_H_
