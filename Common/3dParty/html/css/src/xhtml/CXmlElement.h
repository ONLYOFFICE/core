#ifndef CXMLELEMENT_H
#define CXMLELEMENT_H

#include <string>
#include <map>
#include "../ConstValues.h"


namespace { namespace CSSProperties = NSCSS::NSConstValues::NSProperties; }

class CXmlElement
{
	typedef std::map<CSSProperties::RunnerProperties,    std::wstring> RPropertiesMap;
	typedef std::map<CSSProperties::ParagraphProperties, std::wstring> PPropertiesMap;
	typedef std::map<CSSProperties::BasicProperties,     std::wstring> BPropertiesMap;
	
	RPropertiesMap m_mRStyleValues;
	PPropertiesMap m_mPStyleValues;
	BPropertiesMap m_mBasicValues;

	std::wstring ConvertPStyle(bool bIsLite = false) const;
	std::wstring ConvertRStyle(bool bIsLite = false) const;
	std::wstring ConvertBasicInfoStyle()             const;

	std::wstring GetStyleArguments() const;

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

	std::wstring GetStyle(bool bGetBasedInfo = true, bool bGetPInfo = true, bool bGetRInfo = true) const;
	std::wstring GetPStyle(bool bIsLite = false)  const;
	std::wstring GetRStyle(bool bIsLite = false)  const;
	std::wstring GetBasedOn() const;
	std::wstring GetStyleId() const;
	std::wstring GetName()    const;

	CXmlElement& operator+=(const CXmlElement& oElement);
	CXmlElement& operator= (const CXmlElement& oelement);
	bool        operator== (const CXmlElement& oElement) const;
};

#endif // CXMLELEMENT_H
