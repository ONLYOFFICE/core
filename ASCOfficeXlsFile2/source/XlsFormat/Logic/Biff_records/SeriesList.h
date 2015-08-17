#pragma once

#include "BiffRecord.h"

namespace XLS
{

class RGISeriesListSpecial : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(RGISeriesListSpecial)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeRGISeriesListSpecial;

	//-----------------------------
	std::vector<unsigned short> series;
};




// Logical representation of SeriesList record in BIFF8
class SeriesList: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SeriesList)
	BASE_OBJECT_DEFINE_CLASS_NAME(SeriesList)
public:
	SeriesList();
	~SeriesList();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSeriesList;

//-----------------------------
	RGISeriesListSpecial rgiser;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_COMPLEX(rgiser)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

