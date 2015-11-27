#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of ATTACHEDLABEL union of records 
class ATTACHEDLABEL: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ATTACHEDLABEL)
public:
	ATTACHEDLABEL();
	~ATTACHEDLABEL();

	BaseObjectPtr clone();

	virtual const bool	loadContent		(BinProcessor& proc);
	
	int					serialize		(std::wostream & _stream);
	int					serialize_rPr	(std::wostream & _stream, int iFmt, bool defRPr = false);

	GlobalWorkbookInfoPtr	pGlobalWorkbookInfoPtr;

	BaseObjectPtr m_Pos;
	BaseObjectPtr m_FontX;
	BaseObjectPtr m_AlRuns;
	BaseObjectPtr m_AI;
	BaseObjectPtr m_FRAME;
	BaseObjectPtr m_ObjectLink;
	BaseObjectPtr m_DataLabExtContents;
	BaseObjectPtr m_TEXTPROPS;

	short m_iLinkObject;
};

} // namespace XLS

