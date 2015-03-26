#ifndef _MATH_EQUATION_MATHSTRING_H
#define _MATH_EQUATION_MATHSTRING_H

#include <string>

namespace MathEquation
{
#define STRING_NOTFOUND std::string::npos

	class MathString 
	{
	public:

		MathString() {}
		MathString(const char* val) : Value(val){}
		MathString(std::string val) : Value(val){}

		MathString &operator=(MathString &other)
		{
			Value = other.Value;
			return (*this);
		}

		MathString &operator+=(const MathString &other)
		{
			Value += other.Value;
			return (*this);
		}

		MathString &operator+=(const char* sChars)
		{
			Value += std::string(sChars);
			return (*this);
		}

		MathString &operator+=(const char unChar)
		{
			char oChars[2];
			oChars[0] = unChar; oChars[1] = 0x00;
			Value += std::string((char*)&oChars);
			return (*this);
		}

		void Append(const char* str)
		{
			Value.append(str);
		}

		void Append(MathString str)
		{
			Value.append(str.Value);
		}

		void Append(unsigned short ushUnicode)
		{
			char oChars[2];
			oChars[0] = ushUnicode; oChars[1] = 0x00;
			Value += std::string((char*)&oChars);
		}

		void AppendAscii(const char *sChars)
		{
			Value += std::string(sChars);
		}

		void Erase(int nPos = 0, int nLen = std::string::npos)
		{
			Value.erase(nPos, nLen);
		}

		int Len()
		{
			return (int)Value.length();
		}

		BYTE GetChar(int nPos)
		{
			return Value[nPos];
		}

		void InsertAscii(const char* str, int nPos)
		{
			Value.insert(nPos, str);
		}

		void Insert(MathString str, int nPos)
		{
			Value.insert(nPos, str.Value);
		}

		int SearchBackward(char nChar)
		{
			return (int)Value.rfind(nChar);
		}

		static MathString CreateFromInt32(int nValue)
		{
			#ifdef _WIN32
				char intStr[32];
				_itoa_s(nValue, (char *)&intStr, 32, 10);
				return MathString(intStr);
			#else
				return std::to_string(nValue);;
			#endif
		}

		std::string GetValue()
		{
			return Value;
		}

	private:

		std::string Value;
	};
}

#endif
