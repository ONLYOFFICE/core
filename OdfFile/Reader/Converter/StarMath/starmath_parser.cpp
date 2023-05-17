#include "starmath_parser.h"

#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <stack>
#include <vector>

namespace StarMathParser
{

typedef std::string TString;
typedef char TChar;

TString starMathVersion = "5.0";

// divider/priority
std::vector<std::pair<TString, int>> dividers =
{
	{"##", -1},
	{"#", -1},
};

// operator/priority. less value of priority means bigger priority: 0 - is the highest priority.
std::vector<std::pair<TString, int>> operators =
{
	{"=", 30}, // a = b
	{"+", 35},
	{"-", 35},
	{"/", 45},
	{"*", 45},
	{"^", 50},
	{"%", 45},
	{"'", 45},
	{"{", 99},
	{"}", 99},
	{"(", 100},
	{")", 100},

// a cdot b    = a⋅b
// a times b   = a×b
// neg a       = ¬a
// a and b     = a∧b
// a over b    = a/b
// a div b     = a÷b
// a or b      = a ∨ b
// a circ b    = a°b
// a <> b      = a≠2
// a approx 2 a≈2
// a divides b = a∣b
// a ndivides b = a∤b
// a<2         = a2
// a>2         = a2
// a simeq b   = a≃b
// a parallel b = a∥b
// a ortho b   = a⊥b
// a leslant b = ab
// a geslant b = ab
// a sim b     = a~b
// a equiv b   = a≡b
// a <= b      = a≤b
// a >= b      = a≥b
// a prop b    = a∝b
// a toward b  = ab
// a dlarrow b = a⇐b
// a dlrarrow b = a⇔b
// a drarrow b = a⇒b
// a in B      = a∈B
// a notin B   = a∉B
// A owns b    = A∋b
// emptyset    = ∅
// A intersection B = A∩B
// A union B   = A∪B
// A setminus B = A∖B
// A slash B   = A/ B
// aleph       = ℵ
// A subset B  = A⊂B
// A subseteq B = A⊆B
// A supset B  = A⊃B
// A supseteq B = A⊇B
// A nsubset B = A⊄B
// A nsubseteq B = A⊈B
// A nsupset B = A⊅B
// A nsupseteq B = A⊉B
//
// CONSTATNTS
// setN = ℕ
// setZ = ℤ
// setQ = ℚ
// setR = ℝ
// setC = ℂ



//    {"+-", 100},
//    {"-+", 100},
//    {"over", 100},
//    {"wideslash", 100},
};

std::vector<std::pair<TString, int>> functions =
{
	{"sqrt", 10},
	{"sin", 10},
	{"cos", 10},
	{"tan", 10},
};

std::vector<TChar> allowed =
{
	'.',
	'_',
	'%',
};

bool IsAllowedAlnum(const TString& input, int index, int& len)
{
	len = 0;
	TChar ch = input[index];
	auto res = std::find(std::begin(allowed), std::end(allowed), ch);
	if ((res != std::end(allowed)) || (bool)std::isalnum(ch))
	{
		len = 1;
		return true;
	}
	return false;
}

//int GetPriority(const TString& expr) {
//    if ("(" == expr || ")" == expr || "{" == expr || "}" == expr) {
//        return 0;
//    }
//
//    if ("+" == expr)
//}


class RuntimeException : std::exception
{
protected:
	TString m_sDescription;
public:
	RuntimeException(const TString& sError) : m_sDescription{sError}
	{
	}

	const TChar* what() const noexcept override
	{
		return m_sDescription.c_str();
	}
};

class UnknownSymbolException : public RuntimeException
{
public:
	UnknownSymbolException(const TString& sError, int index)
		: RuntimeException(sError)
	{
		std::stringstream ss;
		ss << " at column: " << index;
		m_sDescription += ss.str();
	}
};

bool IsAllowedSequence(const std::vector<std::pair<TString, int>>& allowed_sequence, const TString& input, int index, int& len)
{
	len = 0;
	for (const auto& pair : allowed_sequence)
	{
		auto res = input.find(pair.first, index);
		if (res != TString::npos && res == index)
		{
			len = pair.first.length();
			return true;
		}
	}

	return false;
}

int GetPriority(const TString& expr)
{
	for (const auto& pair : operators)
	{
		if (expr == pair.first)
		{
			return pair.second;
		}
	}
	return -1;
}

inline bool IsOneOf(const TString& expr, const std::vector<TString>& samples)
{
	return std::find(std::begin(samples), std::end(samples), expr) != std::end(samples);
}

inline TString GetStartedBrackedFor(const TString& expr)
{
	if (expr == ")")
	{
		return "(";
	}
	if (expr == "}")
	{
		return "{";
	}
	if (expr == ">")
	{
		return "<";
	}

	throw RuntimeException("unknown string bracket marker");
}

bool IsAttribute(const TString& expr, int& chain_size)
{
	if (expr == "color")
	{
		return (chain_size = 2)!=0;
	}

	return 0;
}

bool IsAttributeParam(const TString& attribute, const TString& param)
{
	if (attribute == "color")
	{
		if (IsOneOf(param, {"lime", "navy", "fuchsia"}))
		{
			return true;
		}
	}
	return false;
}

struct Attributes
{
	std::vector<TString> m_vAttributes;
};

std::vector<Attributes> setsOfAttributes;

struct TokenWithAttributes
{
	TString m_sToken = "";
	uint32_t m_nNestedLevel = 0;
	uint16_t m_nTypeOfBrackets = 0; // '()', '{}', '<>'
	int m_nAttributesSetIndex = -1;
};

class Iterator
{
public:
	virtual bool HasNext() const = 0;
	virtual TString GetNext() const = 0;
};

class ParserIterator : public Iterator
{
	TString input;
	mutable int index;
public:
	ParserIterator(const TString& expression, int index = 0): input{expression}, index{index} {}

	bool HasNext() const override
	{
		while ((index < input.length()) &&
				isspace(input[index]))
		{
			index++;
		}
		return input.length() > index;
	}

	TString GetNext() const override
	{
		TString ret = "";
		bool was_allowed_alnum = false;
		int chains = 0;
		do
		{
			while (index < input.length())
			{
				int len = 0;
				if (IsAllowedAlnum(input, index, len))
				{
					was_allowed_alnum = true;
					ret+=input.substr(index, len);
					index+=len;
				}
				else

					if ((was_allowed_alnum == true && IsAllowedSequence(dividers, input, index, len)) ||
							(was_allowed_alnum == true && IsAllowedSequence(operators, input, index, len)) || isspace(input[index]))
					{
						break;
					}
					else

						if ((was_allowed_alnum == false && IsAllowedSequence(dividers, input, index, len)) ||
								(was_allowed_alnum == false && IsAllowedSequence(operators, input, index, len)))
						{
							ret+=input.substr(index, len);
							index+=len;
							break;
						}
						else
						{
							throw UnknownSymbolException("unknown symbol", index);
						}
			}

//            // is attribute
//            if (ret == "color") {
//                if (hasNext()) {
//                    chains = 2; // "color name of color"
//                    ret+=" ";
//                }
//            }
			chains--;
		}
		while (chains > 0);

		return ret;
	}
};

std::vector<TString> splitByTokens(const std::shared_ptr<Iterator>& iter)
{
	std::vector<TString> res;
	while (iter->HasNext())
	{
		auto token = iter->GetNext();
		res.emplace_back(token);
	}
	return res;
}

uint16_t MakeGroupFlag(const TString& expr)
{
	if (expr == "{")
	{
		return '{}';
	}
	else if (expr == "(")
	{
		return '()';
	}

	return 0;
}

std::vector<TokenWithAttributes> SplitByTokens(const std::shared_ptr<Iterator>& iter)
{
	std::vector<TokenWithAttributes> res;
	std::vector<TString> groups;
	int level = 0;
	uint16_t group_flag = 0;
	int active_attribute_index = -1;
	while (iter->HasNext())
	{
		auto token = iter->GetNext();

		if (IsOneOf(token, {"{", "("}))
		{
			groups.push_back(token);
			group_flag = MakeGroupFlag(token);
		}
		else if (IsOneOf(token, {"}", ")"}))
		{
			if (!groups.empty())
			{
				groups.pop_back();
			}
			group_flag = MakeGroupFlag("");
			if (!groups.empty())
			{
				group_flag = MakeGroupFlag(groups.back());
			}
		}
		else
		{
			int chain_elements = 0;
			if (IsAttribute(token, chain_elements))
			{
				auto attribute_name = token;
				bool skipped = false;
				Attributes attr;
				attr.m_vAttributes.push_back(attribute_name);
				while (chain_elements > 1 && iter->HasNext())   // так как мы уже прочитали первый элемент цепочки
				{
					auto param = iter->GetNext();
					if (IsAttributeParam(attribute_name, param))
					{
						attr.m_vAttributes[attr.m_vAttributes.size()-1]+=" " + param;
					}
					else
					{
						// skip attribute with wrong params
						skipped = true;
						break;
					}
					chain_elements--;
				}

				if (!skipped)
				{
					setsOfAttributes.push_back(attr);
					active_attribute_index = setsOfAttributes.size()-1; // latest attributes are active
				}
				else
				{
					active_attribute_index = -1;
				}
			}
			else
			{
				level = groups.size();
				res.emplace_back(TokenWithAttributes{token, level, group_flag, active_attribute_index});
			}
		}
	}
	return res;
}


} // of namespace StarMathParser


//x = {-b +- { sqrt{ color lime {b} ^{2} - color navy {4.0E-20} color fuchsia {ac}  } } } over {2a}


