#pragma once

#include "ByteStructure.h"

namespace DocFileFormat
{
	class FieldCharacter: public ByteStructure
	{
	public:
		struct grffldEnd
		{
			byte fDiffer:1;
			byte fZombieEmbed:1;
			byte fResultsDirty:1;
			byte fResultsEdited:1;
			byte fLocked:1;
			byte fPrivateResult:1;
			byte fNested:1;
			byte fHasSep:1;
		};

	public:
		static const int STRUCTURE_SIZE = 2;

		FieldCharacter():
		fldch(0), grffld(0)
		{
		}

		virtual ~FieldCharacter()
		{
		}

		virtual ByteStructure* ConstructObject (VirtualStreamReader* pReader, int length)
		{
			FieldCharacter* pFldChar	=	new FieldCharacter();
			if (pFldChar)
			{
				pFldChar->fldch				=	pReader->ReadByte();
				pFldChar->grffld			=	pReader->ReadByte();

#ifdef _DEBUG
				grffldEnd*	__grffldEnd	=	(grffldEnd*)(&pFldChar->grffld);
				int dbg						=	0;
#endif
			}

			return static_cast<ByteStructure*>(pFldChar);
		}

	private:

		byte fldch;
		byte grffld;
	};
}