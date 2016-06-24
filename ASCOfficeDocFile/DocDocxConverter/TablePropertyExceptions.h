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

#include "PropertyExceptions.h"
#include "ParagraphPropertyExceptions.h"
#include "VirtualStreamReader.h"

namespace DocFileFormat
{
	class TablePropertyExceptions: public PropertyExceptions
	{
	public:
		/// Parses the bytes to retrieve a TAPX
		TablePropertyExceptions(unsigned char* bytes, int size) : PropertyExceptions(bytes, size), m_bSkipShading97 (FALSE)
		{
			//not yet implemented
		}

		virtual ~TablePropertyExceptions()
		{

		}

		/// Extracts the TAPX SPRMs out of a PAPX
		TablePropertyExceptions (ParagraphPropertyExceptions* papx, POLE::Stream* dataStream) : PropertyExceptions()
		{
			VirtualStreamReader oBinReader(dataStream, 0);

			m_bSkipShading97	=	FALSE;

			for (list<SinglePropertyModifier>::iterator oSpmIter = papx->grpprl->begin(); oSpmIter != papx->grpprl->end(); ++oSpmIter)
			{
				if (oSpmIter->OpCode == sprmTDefTableShd || oSpmIter->OpCode == sprmTDefTableShd2nd || 
					oSpmIter->OpCode == sprmTDefTableShd2nd || oSpmIter->OpCode == sprmTDefTableShd3rd)
				{
					m_bSkipShading97 = TRUE;
				}

				if (oSpmIter->Type == TAP)
				{
					grpprl->push_back(*oSpmIter);
				}
				else if ((int)(oSpmIter->OpCode) == sprmPTableProps)
				{
					//there is a native TAP in the data stream
					unsigned int fc = FormatUtils::BytesToUInt32(oSpmIter->Arguments, 0, oSpmIter->argumentsSize);

					//get the size of the following grpprl
                    oBinReader.Seek(fc, 0/* STREAM_SEEK_SET*/);
					unsigned char* sizebytes = oBinReader.ReadBytes(2, true);
					unsigned short grpprlSize = FormatUtils::BytesToUInt16(sizebytes, 0, 2);

					//read the grpprl
					unsigned char* grpprlBytes = oBinReader.ReadBytes(grpprlSize, true);

					//parse the grpprl
					PropertyExceptions externalPx(grpprlBytes, grpprlSize);

					for (list<SinglePropertyModifier>::iterator oIter = externalPx.grpprl->begin(); oIter != externalPx.grpprl->end(); ++oIter)
					{
						if (oIter->Type == TAP)
						{
							grpprl->push_back(*oIter);
						}
					}

					RELEASEARRAYOBJECTS(grpprlBytes);
					RELEASEARRAYOBJECTS(sizebytes);
				}
			}
		}

		inline bool IsSkipShading97 ()
		{
			return m_bSkipShading97;
		}

	private:

		bool	m_bSkipShading97;	//	пропускать правило от Word97 
	};
}
