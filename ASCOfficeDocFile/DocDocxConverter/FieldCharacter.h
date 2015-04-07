#pragma once

#include "ByteStructure.h"

namespace DocFileFormat
{
	class FieldCharacter: public ByteStructure
	{
	public:
		struct grffldEnd
		{
			unsigned char fDiffer:1;
			unsigned char fZombieEmbed:1;
			unsigned char fResultsDirty:1;
			unsigned char fResultsEdited:1;
			unsigned char fLocked:1;
			unsigned char fPrivateResult:1;
			unsigned char fNested:1;
			unsigned char fHasSep:1;
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

		unsigned char fldch;
		unsigned char grffld;
	};
}