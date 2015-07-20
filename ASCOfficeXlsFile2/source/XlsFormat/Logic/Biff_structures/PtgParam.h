#pragma once

#include <string>
#include <vector>

namespace XLS
{;

class PtgParam
{
public:
	typedef enum {
		ptNONE = 0,
		ptCACHE,
		ptUNDEFINED_NAME,
		ptELF_RADICAL
	} ParamType;

	PtgParam(const std::wstring unparsed_line);
	PtgParam(const ParamType type, const std::wstring clear_line);

	const ParamType getType() const;
	const std::wstring getClearLine() const;
	const long getFirstParam() const;
	const long getSecondParam() const;
	const long getParam(const size_t pos) const;
	const long getParamsNum() const;
	const std::wstring toString() const;

	void addParam(const long param);



private:
	const ParamType str2type(const std::wstring str) const;
	const std::wstring type2str(const ParamType type) const;

private:
	ParamType type_;
	std::vector<long> params;
	std::wstring clear_line_;
	
};


} // namespace XLS

