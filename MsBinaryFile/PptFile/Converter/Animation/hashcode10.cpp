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
