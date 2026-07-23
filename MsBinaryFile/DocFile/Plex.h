/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
		int							CP_LENGTH;
		std::vector<int>			CharacterPositions;
		std::vector<ByteStructure*>	Elements;
		bool						m_bIsValid;

		std::map<int, size_t>		mapCP;
	public:
		Plex(int structureLength, POLE::Stream* stream, unsigned int fc, unsigned int lcb, int nWordVersion) 
			: m_bIsValid(false), CP_LENGTH(/*nWordVersion == 2 ? 2 :*/ 4)
		{
			if ((lcb > 0) && (NULL != stream))
			{
				VirtualStreamReader reader(stream, (ULONG)fc, nWordVersion);

				if (fc > reader.GetSize()) return;

				m_bIsValid = true;

				int n = 0;

				if (structureLength > 0)
				{
					// this PLEX contains CPs and Elements
                    //n = ((int)lcb - CP_LENGTH) / (structureLength + CP_LENGTH);
                    int totalSize = (int)lcb - CP_LENGTH;
                    int elementSize = structureLength + CP_LENGTH;

                    if (elementSize > 0)
                    {
                        n = totalSize / elementSize;

                        if (totalSize % elementSize != 0 && structureLength == 6)
                        {
                            structureLength = 4;
                            elementSize = structureLength + CP_LENGTH;
                            n = totalSize/elementSize;
                        }
                    }
				}
				else
				{
					// this PLEX only contains CPs
					n = ((int)lcb - CP_LENGTH) / CP_LENGTH;
				}

				// read the n + 1 CPs

				for (int i = 0; i < (n + 1); ++i)
				{
					int val = reader.ReadInt32();

					mapCP.insert(std::make_pair(val, CharacterPositions.size()));
					CharacterPositions.push_back(val);
				}

				// read the n structs

				if (structureLength > 0)
				{
					for (int i = 0; i < n; ++i)
					{
                        T object;
                        Elements.push_back(object.ConstructObject(&reader, structureLength));
					}
				}

			}
		}

		~Plex()
		{
			for (size_t i = 0; i < Elements.size(); ++i)
			{
				RELEASEOBJECT(Elements[i]);
			}
		}
		inline ByteStructure* GetStruct(size_t index)
		{
			if ((index >= 0) && (index < (int)Elements.size()))
				return this->Elements[index];

			return NULL;
		}
		inline ByteStructure* GetStructByCP(int cp)
		{
			std::map<int, size_t>::iterator pFind = mapCP.find(cp);

			if (pFind != mapCP.end())
			{
				return GetStruct(pFind->second);
			}
			return NULL;			
		}

		inline bool IsCpExists(int cp) const
		{
			bool result = false;

			std::map<int, size_t>::const_iterator pFind = mapCP.find(cp);

			if (pFind != mapCP.end())
			{
				result = true;
			}

			return result;
		}

		inline int operator [] (unsigned int index) const
		{
			if (index < (unsigned int)CharacterPositions.size())
				return CharacterPositions[index];

			return -1;
		}

		inline bool IsValid()
		{
			return m_bIsValid;
		}
	};
}
