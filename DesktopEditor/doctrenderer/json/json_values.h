#ifndef JSON_VALUES_H_
#define JSON_VALUES_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "json.h"

// uncomment to see debug output and exceptions
//#define JSON_DEBUG

namespace NSJSON
{
	class IBaseValue
	{
	public:
		IBaseValue();
		virtual ~IBaseValue();
	};

	class CPrimitive : public IBaseValue
	{
	private:
		enum PrimitiveType
		{
			ptBoolean,
			ptInteger,
			ptDouble,
			ptStringA,
			ptStringW
		};

	public:
		CPrimitive();
		CPrimitive(const CPrimitive& other);
		CPrimitive(bool value);
		CPrimitive(int value);
		CPrimitive(double value);
		CPrimitive(const std::string& str);
		CPrimitive(const std::wstring& wstr);
		~CPrimitive();

		// type check
		bool isBool() const;
		bool isInt() const;
		bool isDouble() const;
		bool isStringA() const;
		bool isStringW() const;

		// getters
		bool toBool() const;
		int toInt() const;
		double toDouble() const;
		std::string toStringA() const;
		std::wstring toStringW() const;

		// setters
		void set(const CPrimitive& other);
		void set(bool value);
		void set(int value);
		void set(double value);
		void set(const std::string& str);
		void set(const std::wstring& wstr);

	private:
		void clear();

	private:
		union
		{
			bool m_bool;
			int m_int;
			double m_double;
			std::string m_string;
			std::wstring m_wstring;
		};
		PrimitiveType m_type;
	};

	class CArray : public IBaseValue
	{
	public:
		CArray(int count);
		CArray(std::initializer_list<CValue> elements);
		~CArray();

	public:
		int getCount() const;
		CValue& get(int index);

	private:
		std::vector<CValue> m_values;
	};

	class CTypedArray : public IBaseValue
	{
	public:
		CTypedArray(BYTE* data = nullptr, int len = 0);
		~CTypedArray();

	public:
		BYTE* getData();
		int getCount() const;

	private:
		BYTE* m_data;
		int m_len;
	};

	class CObject : public IBaseValue
	{
	private:
		using storage_t = std::unordered_map<std::string, CValue>;

	public:
		CObject();
		~CObject();

	public:
		CValue& get(const std::string& name);
		std::vector<std::string> getPropertyNames();

	private:
		storage_t m_values;
	};
}

#endif	// JSON_VALUES_H_
