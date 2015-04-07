#pragma once

#include "ByteStructure.h"

namespace DocFileFormat
{
	template<class T> class Plex
	{
		friend class CommentsMapping;
		friend class DocumentMapping;
		friend class WordDocument;
		friend class TextboxMapping;

	protected:
		static const int CP_LENGTH = 4;
		vector<int>				CharacterPositions;
		vector<ByteStructure*>	Elements;
		bool					m_bIsValid;

	public:
		Plex(int structureLength, POLE::Stream* stream, unsigned int fc, unsigned int lcb) : m_bIsValid(false)
		{
			if ((lcb > 0) && (NULL != stream))
			{
				VirtualStreamReader reader(stream, (ULONG)fc);

				m_bIsValid = true;

				int n = 0;

				if (structureLength > 0)
				{
					// this PLEX contains CPs and Elements
					n = ((int)lcb - CP_LENGTH) / (structureLength + CP_LENGTH);
				}
				else
				{
					// this PLEX only contains CPs
					n = ((int)lcb - CP_LENGTH) / CP_LENGTH;
				}

				// read the n + 1 CPs

				for (int i = 0; i < (n + 1); ++i)
				{
					CharacterPositions.push_back(reader.ReadInt32());
				}

				// read the n structs

				if (structureLength > 0)
				{
					for (int i = 0; i < n; ++i)
					{
						Elements.push_back(T().ConstructObject(&reader, structureLength));
					}
				}

			}
		}

		~Plex()
		{
			for (vector<ByteStructure*>::iterator iter = Elements.begin(); iter != Elements.end(); ++iter)
			{
				RELEASEOBJECT(*iter);
			}
		}


		// Retruns the struct that matches the given character position.

		inline ByteStructure* GetStruct(int cp)
		{
			int index = -1;

			for (int i = 0; i < (int)CharacterPositions.size(); ++i)
			{
				if (CharacterPositions[i] == cp)
				{
					index = i;
					break;
				}
			}

			if ((index >= 0) && (index < (int)Elements.size()))
				return this->Elements[index];

			return NULL;
		}

		inline bool IsCpExists(int cp) const
		{
			bool result = false;

			for (int i = 0; i < (int)CharacterPositions.size(); ++i)
			{
				if (CharacterPositions[i] == cp)
				{
					result = true;
					break;
				}
			}

			return result;
		}

		inline int operator [] (unsigned int index) const
		{
			if (index < CharacterPositions.size())
				return CharacterPositions[index];

			return -1;
		}

		inline bool IsValid()
		{
			return m_bIsValid;
		}
	};
}
