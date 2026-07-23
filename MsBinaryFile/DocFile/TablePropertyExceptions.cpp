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

#include "TablePropertyExceptions.h"

namespace DocFileFormat
{	
	/// Parses the bytes to retrieve a TAPX
	TablePropertyExceptions::TablePropertyExceptions(unsigned char* bytes, int size, int nWordVersion) :
						PropertyExceptions(bytes, size, nWordVersion), m_bSkipShading97 (FALSE)
	{
		//not yet implemented
	}
	TablePropertyExceptions::~TablePropertyExceptions()
	{

	}

	/// Extracts the TAPX SPRMs out of a PAPX
	TablePropertyExceptions::TablePropertyExceptions (ParagraphPropertyExceptions* papx, POLE::Stream* dataStream, int nWordVersion) :
																								PropertyExceptions()
	{
		VirtualStreamReader oBinReader(dataStream, 0, nWordVersion);

		m_bSkipShading97	=	FALSE;

		for (std::vector<SinglePropertyModifier>::iterator oSpmIter = papx->grpprl->begin(); oSpmIter != papx->grpprl->end(); ++oSpmIter)
		{
			if (oSpmIter->OpCode == sprmTDefTableShd	||
				oSpmIter->OpCode == sprmTDefTableShd2nd ||
				oSpmIter->OpCode == sprmTDefTableShd3rd)
			{
				m_bSkipShading97 = TRUE;
			}

			if (oSpmIter->Type == TAP)
			{
				grpprl->push_back(*oSpmIter);
			}
			else if (oSpmIter->OpCode == sprmPTableProps)
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
				PropertyExceptions externalPx(grpprlBytes, grpprlSize, nWordVersion);

				for (std::vector<SinglePropertyModifier>::iterator oIter = externalPx.grpprl->begin(); oIter != externalPx.grpprl->end(); ++oIter)
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
}
