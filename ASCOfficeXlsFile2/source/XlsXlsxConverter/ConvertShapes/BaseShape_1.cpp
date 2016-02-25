#pragma once

#include "BaseShape.h"

#include "CustomShape.h"

namespace NSCustomShapesConvert
{
CBaseShape* CBaseShape::CreateByType( int ShapeType)
{
    return CCustomShape::CreateByType((oox::MSOSPT)ShapeType);
}

bool CBaseShape::SetType( int ShapeType)
{

    return ((CCustomShape*)this)->SetShapeType((oox::MSOSPT)ShapeType);
}
}
