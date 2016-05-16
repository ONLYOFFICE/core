#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/DXFN.h>
#include <Logic/Biff_structures/CFParsedFormulaNoCCE.h>

namespace XLS
{


// Logical representation of CF record in BIFF8
class CF: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CF)
	BASE_OBJECT_DEFINE_CLASS_NAME(CF)
public:
	CF(const CellRef& cell_base_ref);
	~CF();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCF;

	int serialize(std::wostream & stream);
//-----------------------------
	unsigned char	ct;
	unsigned char	cp;

	int				dxfId_;
	int				ipriority_;
	
	DXFN			rgbdxf;
	
	CFParsedFormulaNoCCE rgce1;
	CFParsedFormulaNoCCE rgce2;

	BaseObjectPtr	m_CFEx;
};

typedef boost::shared_ptr<CF> CFPtr;

} // namespace XLS

