/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "Records.h"

#include "../Records/RecordsIncluder.h"
#include "../Records/Animations/_includer.h"
#include "../Enums/RecordType.h"

#include <iostream>
#include <iomanip>

std::wstring CUnknownRecord::ReadStringW(const CFStreamPtr &pStream, int lLen)
{
    if (!pStream) return (L"");

    unsigned char* pData = new unsigned char[2 * (lLen + 1)];
    memset (pData, 0, 2 * (lLen + 1));

    pStream->read(pData, 2 * lLen);

    if (sizeof(wchar_t) == 4)
    {
        ConversionResult eUnicodeConversionResult;
        UTF32 *pStrUtf32 = new UTF32 [lLen + 1];
        pStrUtf32[lLen] = 0 ;

        const UTF16 *pStrUtf16_Conv = (const UTF16 *) pData;
        UTF32		*pStrUtf32_Conv =                 pStrUtf32;

        eUnicodeConversionResult = ConvertUTF16toUTF32 ( &pStrUtf16_Conv
                                                         , &pStrUtf16_Conv[lLen]
                                                         , &pStrUtf32_Conv
                                                         , &pStrUtf32 [lLen]
                                                         , strictConversion);

        if (conversionOK != eUnicodeConversionResult)
        {
            delete [] pStrUtf32;
            return (L"");
        }
        std::wstring res((wchar_t*)pStrUtf32, lLen);
        if (pStrUtf32) delete [] pStrUtf32;
        return res;
    }
    else
    {
        std::wstring str((wchar_t*)pData);
        delete[] pData;
        return str;
    }

}
std::string CUnknownRecord::ReadStringA(const CFStreamPtr &pStream, int lLen)
{
    if (!pStream) return ("");

    char* pData = new char[lLen + 1];

	pStream->read((unsigned char*)pData, lLen);

    pData[lLen] = 0;

    std::string str(pData, lLen);

    delete[] pData;
    return str;
}

void CRecordsContainer::ReadFromStream(SRecordHeader & oHeader, const CFStreamPtr &pStream)
{
	m_oHeader = oHeader;
	m_arRecords.clear();

	LONG lPosition = pStream->getStreamPointer();
	
	m_oHeader = oHeader;

	UINT lCurLen = 0;
	ULONG lReadLen = 0;
	SRecordHeader oRec;
	
	while (lCurLen < m_oHeader.RecLen)
	{
		if (oRec.ReadFromStream(pStream) == FALSE )
		{
			break;
		}
		
		IRecord* pRecord = CreateByType(oRec);
		pRecord->ReadFromStream(oRec, pStream);

		m_arRecords.push_back(pRecord);
		lCurLen += (8 + oRec.RecLen);
	}
	if (lCurLen != m_oHeader.RecLen)
	{
		// нужно разобраться, что тут такое!!!
		LONG lPosition = 0;
		pStream->seekFromBegin(lPosition + m_oHeader.RecLen);
	}
}

void CRecordsContainer::ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
{
	m_oHeader = oHeader;
	m_arRecords.clear();

	LONG lPosition = 0;
	StreamUtils::StreamPosition(lPosition, pStream);
	
	m_oHeader = oHeader;

	UINT lCurLen = 0;
	ULONG lReadLen = 0;
	SRecordHeader oRec;
	while (lCurLen < m_oHeader.RecLen)
	{
		if (oRec.ReadFromStream(pStream) == FALSE )
		{
			break;
		}
		
		IRecord* pRecord = CreateByType(oRec);
		pRecord->ReadFromStream(oRec, pStream);

		m_arRecords.push_back(pRecord);
		lCurLen += (8 + oRec.RecLen);
	}
	if (lCurLen != m_oHeader.RecLen)
	{
		// нужно разобраться, что тут такое!!!
		LONG lPosition = 0;
		StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
	}
}

IRecord* CreateByType(SRecordHeader oHeader)
{

    char str[1024]={};
    sprintf(str, "Record type: %x\t" , oHeader.RecType);
    std::cout << str << std::setw(32) << GetRecordName((_UINT32)oHeader.RecType)
              << "\tlen = " << oHeader.RecLen << "\n";

    if ((_UINT32)oHeader.RecType == 0x1014) {
       std::cout << "Animation!\n";
    }

	IRecord* pRecord = NULL;
    switch (oHeader.RecType)
    {
    CREATE_BY_TYPE(RT_AnimationInfo, CRecordAnimationInfoContainer)
    CREATE_BY_TYPE(RT_AnimationInfoAtom, CRecordAnimationInfoAtom)
    CREATE_BY_TYPE(RT_HashCodeAtom, CRecordHashCode10Atom)
    CREATE_BY_TYPE(RT_BuildList, CRecordBuildListContainer)
    CREATE_BY_TYPE(RT_ParaBuild, CRecordParaBuildContainer)
    CREATE_BY_TYPE(RT_ChartBuild, CRecordChartBuildContainer)
    CREATE_BY_TYPE(RT_DiagramBuild, CRecordDiagramBuildContainer)
    CREATE_BY_TYPE(RT_ParaBuildAtom, CRecordParaBuildAtom)
    CREATE_BY_TYPE(RT_LevelInfoAtom, CRecordLevelInfoAtom)
    CREATE_BY_TYPE(RT_ChartBuildAtom, CRecordChartBuildAtom)
    CREATE_BY_TYPE(RT_DiagramBuildAtom, CRecordDiagramBuildAtom)
    CREATE_BY_TYPE(RT_TimeExtTimeNodeContainer, CRecordExtTimeNodeContainer)
    CREATE_BY_TYPE(RT_TimeSubEffectContainer, CRecordSubEffectContainer)
    CREATE_BY_TYPE(RT_TimeNode, CRecordTimeNodeAtom)
    CREATE_BY_TYPE(RT_TimePropertyList, CRecordTimePropertyList4TimeNodeContainer)
    CREATE_BY_TYPE(RT_TimeAnimateBehaviorContainer, CRecordTimeAnimateBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeAnimateBehavior, CRecordTimeAnimateBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeAnimationValueList, CRecordTimeAnimationValueListContainer)
    CREATE_BY_TYPE(RT_TimeAnimationValue, CRecordTimeAnimationValueAtom)
    CREATE_BY_TYPE(RT_TimeBehaviorContainer, CRecordTimeBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeVariantList, CRecordTimeStringListContainer)
    CREATE_BY_TYPE(RT_TimeClientVisualElement, CRecordClientVisualElementContainer)
    CREATE_BY_TYPE(RT_VisualPageAtom, CRecordVisualPageAtom)
//    CREATE_BY_TYPE(RT_VisualShapeAtom, CRecordVisualShapeOrSoundAtom)
    CREATE_BY_TYPE(RT_VisualShapeAtom, CRecordVisualSoundAtom)
    CREATE_BY_TYPE(RT_TimeColorBehaviorContainer, CRecordTimeColorBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeColorBehavior, CRecordTimeColorBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeEffectBehaviorContainer, CRecordTimeEffectBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeEffectBehavior, CRecordTimeEffectBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeMotionBehaviorContainer, CRecordTimeMotionBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeMotionBehavior, CRecordTimeMotionBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeRotationBehaviorContainer, CRecordTimeRotationBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeRotationBehavior, CRecordTimeRotationBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeScaleBehaviorContainer, CRecordTimeScaleBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeScaleBehavior, CRecordTimeScaleBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeSetBehaviorContainer, CRecordTimeSetBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeSetBehavior, CRecordTimeSetBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeCommandBehaviorContainer, CRecordTimeCommandBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeCommandBehavior, CRecordTimeCommandBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeIterateData, CRecordTimeIterateDataAtom)
    CREATE_BY_TYPE(RT_TimeSequenceData, CRecordTimeSequenceDataAtom)
    CREATE_BY_TYPE(RT_TimeConditionContainer, CRecordTimeConditionContainer)
    CREATE_BY_TYPE(RT_TimeCondition, CRecordTimeConditionAtom)
    CREATE_BY_TYPE(RT_TimeModifier, CRecordTimeModifierAtom)
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()

    default:
        {
        }break;
    };

	if (NULL == pRecord)
	{
		if (oHeader.IsContainer())
		{
			pRecord = new CRecordsContainer();
		}
		else
		{
			pRecord = new CUnknownRecord();
#ifdef _DEBUG
			char str[1024]={};
			sprintf(str, "Unknown record type: %x\t" , oHeader.RecType);
			std::cout << str << GetRecordName((_UINT32)oHeader.RecType) << "\n"; 
#endif
		}
	}
	return pRecord;
}
