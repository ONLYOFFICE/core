#pragma once

#include "BiffAttribute.h"

namespace XLS
{;

class CFRecord;

class ChartNumNillable : public BiffAttribute
{
public:

	ChartNumNillable(const WORD nil_type);

	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	void setNilType(const WORD type);
	const bool isNil();

private:
	union
	{
		struct  
		{
			BYTE byte1;
			BYTE byte2;
			BYTE byte3;
			BYTE byte4;
			WORD type;
			WORD fExprO;
		};
		double xnum;
	} data;
	WORD nil_type_;
};

} // namespace XLS

