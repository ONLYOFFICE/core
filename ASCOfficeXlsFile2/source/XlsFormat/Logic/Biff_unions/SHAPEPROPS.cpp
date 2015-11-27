
#include "SHAPEPROPS.h"
#include <Logic/Biff_records/ShapePropsStream.h>
#include <Logic/Biff_records/ContinueFrt12.h>

#include <simple_xml_writer.h>
#include <utils.h>

namespace XLS
{

SHAPEPROPS::SHAPEPROPS()
{
}


SHAPEPROPS::~SHAPEPROPS()
{
}


BaseObjectPtr SHAPEPROPS::clone()
{
	return BaseObjectPtr(new SHAPEPROPS(*this));
}


// SHAPEPROPS = ShapePropsStream *ContinueFrt12
const bool SHAPEPROPS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<ShapePropsStream>())
	{
		return false;
	}
	m_ShapePropsStream = elements_.back();
	elements_.pop_back();

	proc.repeated<ContinueFrt12>(0, 0);

	return true;
}

int SHAPEPROPS::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
	}
	return 0;
}

} // namespace XLS

