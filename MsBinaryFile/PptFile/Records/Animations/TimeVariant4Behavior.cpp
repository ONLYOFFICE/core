#include "TimeVariant4Behavior.h"

using namespace PPT;

//-------------------------------------------------------------------------------
#define CREATE_BY_Instanse(RECORD_Instanse, CLASS_RECORD_NAME)					\
    case RECORD_Instanse: { pRecord = new CLASS_RECORD_NAME(); break; }         \
//-------------------------------------------------------------------------------


IRecord *FactoryTimeVariant4Behavior::createByInstanse(unsigned short recInstance)
{
    IRecord* pRecord = nullptr;

    switch (recInstance)
    {
    CREATE_BY_Instanse(TL_TBPID_UnknownPropertyList,    CRecordTimeVariantString)
    CREATE_BY_Instanse(TL_TBPID_RuntimeContext,         CRecordTimeRuntimeContext)
    CREATE_BY_Instanse(TL_TBPID_MotionPathEditRelative, CRecordTimeVariantBool)
    CREATE_BY_Instanse(TL_TBPID_ColorColorModel,        CRecordTimeColorModel)
    CREATE_BY_Instanse(TL_TBPID_ColorDirection,         CRecordTimeColorDirection)
    CREATE_BY_Instanse(TL_TBPID_Override,               CRecordTimeOverride)
    CREATE_BY_Instanse(TL_TBPID_PathEditRotationAngle,  CRecordTimeVariantFloat)
    CREATE_BY_Instanse(TL_TBPID_PathEditRotationX,      CRecordTimeVariantFloat)
    CREATE_BY_Instanse(TL_TBPID_PathEditRotationY,      CRecordTimeVariantFloat)
    CREATE_BY_Instanse(TL_TBPID_PointsTypes,            CRecordTimePointsTypes)
    }

    return pRecord;
}
