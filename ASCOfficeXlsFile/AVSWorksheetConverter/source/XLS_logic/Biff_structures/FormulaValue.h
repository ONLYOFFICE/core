#pragma once

#include "BiffAttribute.h"

namespace XLS
{;

class CFRecord;

class FormulaValue : public BiffAttribute
{
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	const BYTE getType() const;

private:
	union
	{
		struct  
		{
			BYTE byte1;
			BYTE byte2;
			BYTE byte3;
			BYTE byte4;
			BYTE byte5;
			BYTE byte6;
			WORD fExprO;
		};
		double xnum;
	} data;
};

} // namespace XLS

