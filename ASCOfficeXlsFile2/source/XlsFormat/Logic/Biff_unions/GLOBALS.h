#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of GLOBALS union of records 
class GLOBALS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(GLOBALS)
public:
	GLOBALS(const bool is_dialog_sheet);
	~GLOBALS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeGLOBALS;

	int serialize(std::wostream & stream);

	bool is_dialog;

	BaseObjectPtr				 m_DefColWidth;
	BaseObjectPtr				 m_DefaultRowHeight;
	BaseObjectPtr				 m_Guts;
};

} // namespace XLS

