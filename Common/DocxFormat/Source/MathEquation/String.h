#ifndef _MATH_EQUATION_STRING_H
#define _MATH_EQUATION_STRING_H

namespace MathEquation
{
#define STRING_NOTFOUND std::string::npos

	class String 
	{
	public:

		String() {}
		String(const char* val) : Value(val){}
		String(std::string val) : Value(val){}

		String &operator=(String &other)
		{
			Value = other.Value;
			return (*this);
		}

		String &operator+=(const String &other)
		{
			Value += other.Value;
			return (*this);
		}

		String &operator+=(const char* sChars)
		{
			Value += std::string(sChars);
			return (*this);
		}

		String &operator+=(const char unChar)
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

		void Append(String str)
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

		void Insert(String str, int nPos)
		{
			Value.insert(nPos, str.Value);
		}

		int SearchBackward(char nChar)
		{
			return (int)Value.rfind(nChar);
		}

		static String CreateFromInt32(int nValue)
		{
			char intStr[32];
			_itoa_s(nValue, (char *)&intStr, 32, 10);

			return String(intStr);
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