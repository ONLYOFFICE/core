#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of AxcExt record in BIFF8
class AxcExt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(AxcExt)
	BASE_OBJECT_DEFINE_CLASS_NAME(AxcExt)
public:
	AxcExt();
	~AxcExt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeAxcExt;

	int serialize(std::wostream & _stream);

//-----------------------------
	_UINT16 catMin;
	_UINT16 catMax;
	_UINT16 catMajor;
	_UINT16 duMajor;
	_UINT16 catMinor;
	_UINT16 duMinor;
	_UINT16 duBase;
	_UINT16 catCrossDate;

	bool fAutoMin;
	bool fAutoMax;
	bool fAutoMajor;
	bool fAutoMinor;
	bool fDateAxis;
	bool fAutoBase;
	bool fAutoCross;
	bool fAutoDate;

};

} // namespace XLS

