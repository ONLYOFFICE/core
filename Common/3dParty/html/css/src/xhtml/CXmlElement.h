#ifndef CXMLELEMENT_H
#define CXMLELEMENT_H

#include <string>
#include <map>
#include "../ConstValues.h"


namespace { namespace CSSProperties = NSCSS::NSConstValues::NSProperties; }

class CXmlElement
{
	std::map<CSSProperties::RunnerProperties,    std::wstring> m_mRStyleValues;
	std::map<CSSProperties::ParagraphProperties, std::wstring> m_mPStyleValues;
	std::map<CSSProperties::BasicProperties,     std::wstring> m_mBasicValues;

	std::wstring ConvertPStyle()         const;
	std::wstring ConvertRStyle()         const;
	std::wstring ConvertBasicInfoStyle() const;

public:
	CXmlElement();
	CXmlElement(const std::wstring& sNameDefaultElement);
	~CXmlElement();

	bool Empty() const;

	void CreateDefaultElement(const std::wstring& sNameDefaultElement);
	void Clear();

	void AddPropertiesInP  (const CSSProperties::ParagraphProperties& enProperties, const std::wstring& sValue);
	void AddPropertiesInR  (const CSSProperties::RunnerProperties&    enProperties, const std::wstring& sValue);
	void AddBasicProperties(const CSSProperties::BasicProperties&     enProperties, const std::wstring& sValue);

	std::wstring GetLitePStyle() const;
	std::wstring GetLiteRStyle() const;
	std::wstring GetPStyle()    const;
	std::wstring GetRStyle()    const;
	std::wstring GetStyle()     const;
	std::wstring GetBasedOn()   const;
	std::wstring GetStyleId()   const;
	std::wstring GetName()      const;

	CXmlElement& operator+=(const CXmlElement& oElement);
	CXmlElement& operator= (const CXmlElement& oelement);
	bool        operator== (const CXmlElement& oElement);
};

#endif // CXMLELEMENT_H
