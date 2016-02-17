#pragma once

#include "BaseShape.h"

#include "CustomShape.h"


NSGuidesVML::CBaseShape* NSGuidesVML::CBaseShape::CreateByType( int ShapeType)
{
    return CCustomShape::CreateByType((oox::MSOSPT)ShapeType);
}

bool NSGuidesVML::CBaseShape::SetType( int ShapeType)
{

    return ((CCustomShape*)this)->SetShapeType((oox::MSOSPT)ShapeType);
}
