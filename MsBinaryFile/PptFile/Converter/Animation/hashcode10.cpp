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
#include "hashcode10.h"

namespace PPT
{
	namespace Converter 
	{
		std::array<_UINT32, HashCode10::RASize> HashCode10::InitRandomArray()
		{
			std::array<_UINT32, 256 * 256> randomArray;
			std::fill(randomArray.begin(), randomArray.end(), 0);
			_UINT32 randomSeed = 1;
			for (int iRow = 0; iRow < 256; iRow++)
				for (int iCol = 0; iCol < 256; iCol++)
				{
					_UINT32 r0 = randomSeed;
					_UINT32 r1 = ((r0 * 0x000343FD + 0x00269EC3) >> 16) & 0x00007FFF;
					_UINT32 r2 = ((r1 * 0x000343FD + 0x00269EC3) >> 16) & 0x00007FFF;
					_UINT32 r3 = ((r2 * 0x000343FD + 0x00269EC3) >> 16) & 0x00007FFF;
					_UINT32 r4 = ((r3 * 0x000343FD + 0x00269EC3) >> 16) & 0x00007FFF;
					randomSeed = r4;
					r1 = (r1 % 0x100) << 0;
					r2 = (r2 % 0x100) << 8;
					r3 = (r3 % 0x100) << 16;
					r4 = (r4 % 0x100) << 24;
					randomArray[iRow * 256 + iCol] = r4 | r3 | r2 | r1;
				}


			return randomArray;
		}

		_UINT32 HashCode10::GetHash1995(const std::vector<Intermediate::SOldAnimation>& anim95)
		{
			auto hashTabble = InitRandomArray();
			auto& randomArray = hashTabble;
			_UINT32 hash = 0;
			for (const auto& animInfoAtom : anim95)
			{
				std::array<BYTE, 36> asByteArr{};   // {} - init 0
				if (animInfoAtom.anim != nullptr)
					asByteArr = animInfoAtom.anim->m_AnimationAtom.asByteArr;

				const auto byteLen = 36;
				const _UINT32 shapeId = animInfoAtom.shapeId;
				for (int byteIndex = 0; byteIndex < byteLen; byteIndex++)
				{
					int rowIndex = (shapeId * (byteIndex + 1)) % 256;
					hash ^= randomArray[rowIndex * 256 + asByteArr[byteIndex]];
				}

			}

			return hash;
		}

		std::vector<Intermediate::SOldAnimation> HashCode10::MergeSlideShapesWithAnim95Struct(const std::vector<Intermediate::SOldAnimation> &vecAnim95, std::unordered_set<_INT32> sldShapesID)
		{
			std::vector<Intermediate::SOldAnimation> merged;
			for (const auto& anim : vecAnim95)
			{
				merged.push_back(anim);
				auto iter = sldShapesID.find(anim.shapeId);
				if (iter != sldShapesID.end())
					sldShapesID.erase(iter);
			}
			for (const auto& spid : sldShapesID)
			{
				Intermediate::SOldAnimation emptyAnim;
				emptyAnim.shapeId = spid;
				merged.push_back(emptyAnim);
			}

			return merged;
		}

		_UINT32 HashCode10::GetWroteHash(CRecordPP10SlideBinaryTagExtension *pTagExt)
		{
			return pTagExt->m_pHashCode10Atom->m_nHash;
		}

		bool HashCode10::IsValidHash(const Intermediate::SlideAnimation &sldAnim) const
		{
			if (sldAnim.pAnim_2010 == nullptr || sldAnim.pAnim_2010->m_haveHashCode == false || sldAnim.pAnim_2010->m_pHashCode10Atom == nullptr)
				return false;

			auto wroteHash = GetWroteHash(sldAnim.pAnim_2010);
			auto calculatedHash = GetHash1995(
				MergeSlideShapesWithAnim95Struct(sldAnim.arrAnim_1995, sldAnim.realShapesIds));

			return wroteHash == calculatedHash;
		}

	}
}
