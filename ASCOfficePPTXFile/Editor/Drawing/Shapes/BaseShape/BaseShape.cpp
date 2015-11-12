#pragma once

#include "BaseShape.h"

#if defined(PPTX_DEF)
#include "PPTXShape/PPTXShape.h"
#endif

#if defined(PPT_DEF)
#include "PPTShape/PPTShape.h"
#endif


NSPresentationEditor::CBaseShape* NSPresentationEditor::CBaseShape::CreateByType(NSPresentationEditor::NSBaseShape::ClassType ClassType, int ShapeType)
{
#if defined(PPTX_DEF)
	if(ClassType == pptx)
	{
		return CPPTXShape::CreateByType((OOXMLShapes::ShapeType)ShapeType);
	}
#endif

#if defined(PPT_DEF)
	if (ClassType == ppt)
	{
		return CPPTShape::CreateByType((PPTShapes::ShapeType)ShapeType);
	}
#endif


	return NULL;
}

bool NSPresentationEditor::CBaseShape::SetType(NSPresentationEditor::NSBaseShape::ClassType ClassType, int ShapeType)
{
	if (ClassType != GetClassType())
		return false;

#if defined(PPTX_DEF)
	if(ClassType == pptx)
	{
		return ((CPPTXShape*)this)->SetShapeType((OOXMLShapes::ShapeType)ShapeType);
	}
#endif

#if defined(PPT_DEF)
	if(ClassType == ppt)
	{
		return ((CPPTShape*)this)->SetShapeType((PPTShapes::ShapeType)ShapeType);
	}
#endif



	return false;
}