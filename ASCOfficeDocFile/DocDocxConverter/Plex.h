/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
