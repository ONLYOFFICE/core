#pragma once
#include "stdafx.h"
#include "BaseShape.h"

#if defined(PPTX_DEF)
#include "PPTXShape/PPTXShape.h"
#endif

#if defined(PPT_DEF)
#include "PPTShape/PPTShape.h"
#endif

#if defined(ODP_DEF)
#include "OdpShape/OdpShape.h"
#endif

CBaseShape* CBaseShape::CreateByType(NSBaseShape::ClassType ClassType, int ShapeType)
{
#if defined(PPTX_DEF)
	if(ClassType == pptx)
	{
		return CPPTXShape::CreateByType((OOXMLShapes::ShapeType)ShapeType);
	}
#endif

#if defined(PPT_DEF)

#endif

#if defined(ODP_DEF)
	if(ClassType == NSBaseShape::odp)
	{
		return COdpShape::CreateByType((OdpShapes::ShapeType)ShapeType);
	}
#endif
	return NULL;
}

bool CBaseShape::SetType(NSBaseShape::ClassType ClassType, int ShapeType)
{
	if(ClassType != GetClassType())
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

#if defined(ODP_DEF)
	if(ClassType == NSBaseShape::odp)
	{
		return ((COdpShape*)this)->SetShapeType((OdpShapes::ShapeType)ShapeType);
	}
#endif

	return false;
}