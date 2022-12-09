#pragma once

#include "../../../../OOXML/PPTXFormat/Logic/Timing/CTn.h"
#include "../../Records/Animations/ExtTimeNodeContainer.h"

namespace PPT
{
	namespace Intermediate
	{
		struct ShapeAnim
		{
			_INT32 spid = -1;
			_INT32 presetID = -1;   // where 1 is any animation or simpless: appear

			inline bool IsValid()const
			{
				return spid > 0 && presetID > 0;
			}
		};

		bool operator==(const ShapeAnim &s1, const ShapeAnim &s2);

		// look at enum TimeNodeLevel
		ShapeAnim ParseExisting3Level_CTn(const PPTX::Logic::CTn &oCTn);
		ShapeAnim ParseExisting3Level_ETNC(const CRecordExtTimeNodeContainer *pETNC);

		ShapeAnim ParseExisting4Level_CTn(const PPTX::Logic::CTn &oCTn);
		ShapeAnim ParseExisting4Level_ETNC(const CRecordExtTimeNodeContainer *pETNC);

		ShapeAnim ParseExisting5Level_CTn(const PPTX::Logic::CTn &oCTn);
		ShapeAnim ParseExisting5Level_ETNC(const CRecordExtTimeNodeContainer *pETNC);

		CRecordTimeBehaviorContainer* GetTimeBehaviorContainer(CRecordExtTimeNodeContainer *pETNC);
		_INT32 GetSpID(CRecordExtTimeNodeContainer *pETNC);

	}
}
