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

#include "FormattedDiskPagePAPX.h"

namespace DocFileFormat
{
	FormattedDiskPagePAPX::~FormattedDiskPagePAPX()
	{
		RELEASEARRAYOBJECTS(rgfc);
		RELEASEARRAYOBJECTS(rgbx);

		if (grppapx != NULL)
		{
			for (unsigned int i = 0; i < grppapxSize; i++)
			{
				RELEASEOBJECT(grppapx[i]);
			}

			RELEASEARRAYOBJECTS(grppapx);
		}
	}

	/*========================================================================================================*/

	FormattedDiskPagePAPX::FormattedDiskPagePAPX(POLE::Stream* wordStream, int offset, POLE::Stream* dataStream, int nWordVersion, bool fComplex) :
		FormattedDiskPage(), rgbx(NULL), grppapxSize(0), grppapx(NULL)
	{
		Type = Paragraph;
		WordStream = wordStream;

		//read the 512 bytes (FKP)
		unsigned char* bytes = NULL;
		bytes = new unsigned char[512];
		memset(bytes, 512, 0);

		WordStream->seek(offset);
		WordStream->read(bytes, 512);

		//get the count
		crun = bytes[511];
		if (crun < 1)
		{
			RELEASEARRAYOBJECTS(bytes);
			return;
		}
		//create and fill the array with the adresses
		rgfcSize = crun + 1;
		rgfc = new int[rgfcSize];

		int j = 0;

		for (unsigned int i = 0; i < rgfcSize; i++)
		{
			rgfc[i] = FormatUtils::BytesToInt32(bytes, j, 512);
			j += 4;
		}
		rgbx = new BX[crun];
		grppapxSize = crun;
		grppapx = new ParagraphPropertyExceptions * [grppapxSize];

		for (unsigned int i = 0; i < grppapxSize; i++)
		{
			grppapx[i] = NULL;
		}

		j = 4 * (crun + 1);

		unsigned char phe[12];

		for (unsigned char i = 0; i < crun; i++)
		{
			BX bx;
			bx.wordOffset = bytes[j];
			j++;

			if (/*fComplex ||*/ nWordVersion == 0)
			{
				memcpy(phe, (bytes + j), 12);

				//fill the rgbx array
				bx.phe = ParagraphHeight(phe, 12, false);

				j += 12;
			}
			else if (nWordVersion == 2)
			{
				memcpy(phe, (bytes + bx.wordOffset * 2 + j + 1), 6);

				//fill the rgbx array
				bx.phe = ParagraphHeight(phe, 6, false);
			}
			else
			{
				memcpy(phe, (bytes + j), 6);

				//fill the rgbx array
				bx.phe = ParagraphHeight(phe, 6, false);

				j += 6;
			}
			rgbx[i] = bx;

			if (bx.wordOffset != 0)
			{
				unsigned char padbyte = 0;
				unsigned char cw = bytes[bx.wordOffset * 2];
				//if that unsigned char is zero, it's a pad unsigned char, and the word count is the following unsigned char
				if (cw == 0)
				{
					padbyte = 1;
					cw = bytes[bx.wordOffset * 2 + 1];
				}
				if (cw != 0)
				{
					int sz = cw * 2;
					//read the bytes for papx
					unsigned char* papx = new unsigned char[sz];
					memcpy(papx, (bytes + (bx.wordOffset * 2) + padbyte + 1), sz);

					//parse PAPX and fill grppapx
					grppapx[i] = new ParagraphPropertyExceptions(papx, sz, dataStream, nWordVersion);

					RELEASEARRAYOBJECTS(papx);
				}

			}
			else
			{
				//create a PAPX which doesn't modify anything
				grppapx[i] = new ParagraphPropertyExceptions();
			}
		}

		RELEASEARRAYOBJECTS(bytes);
	}

	/*========================================================================================================*/

	/// Parses the 0Table (or 1Table) for FKP _entries containing PAPX
	std::vector<FormattedDiskPagePAPX*>* FormattedDiskPagePAPX::GetAllPAPXFKPs(FileInformationBlock* fib, POLE::Stream* wordStream, POLE::Stream* tableStream, POLE::Stream* dataStream)
	{
		std::vector<FormattedDiskPagePAPX*>* PAPXlist = new std::vector<FormattedDiskPagePAPX*>();

		if ((fib->m_FibWord97.lcbPlcfBtePapx < 8 && fib->m_nWordVersion > 0 && fib->m_FibBase.fComplex == false) || fib->m_FibWord97.lcbPlcfBtePapx < 4) return PAPXlist;

		//get bintable for PAPX
		unsigned char* binTablePapx = new unsigned char[fib->m_FibWord97.lcbPlcfBtePapx + 1];

		if (tableStream)
		{
			tableStream->seek(fib->m_FibWord97.fcPlcfBtePapx);
			tableStream->read(binTablePapx, fib->m_FibWord97.lcbPlcfBtePapx);
		}

		//there are n offsets and n-1 fkp's in the bin table

		if (fib->m_nWordVersion > 0/* && fib->m_FibBase.fComplex == false*/)
		{
			int	n = (((int)fib->m_FibWord97.lcbPlcfBtePapx - 8) / 6) + 1;

			unsigned int first = FormatUtils::BytesToInt32(binTablePapx, 0, fib->m_FibWord97.lcbPlcfBtePapx);
			unsigned int last = FormatUtils::BytesToInt32(binTablePapx, 4, fib->m_FibWord97.lcbPlcfBtePapx);

			int start_papx = 8;
			if (fib->m_FibWord97.lcbPlcfBtePapx - 8 > (n - 1) * 4)
			{
				start_papx += ((n - 1) * 4); //crun duplication 
			}

			int offset = 0;
			for (unsigned int i = start_papx; i < fib->m_FibWord97.lcbPlcfBtePapx; i += 2)
			{
				//indexed FKP is the xth 512byte page
				int fkpnr = FormatUtils::BytesToInt16(binTablePapx, i, fib->m_FibWord97.lcbPlcfBtePapx);

				//so starts at:
				int offset = fkpnr * 512;

				//parse the FKP and add it to the list
				PAPXlist->push_back(new FormattedDiskPagePAPX(wordStream, offset, dataStream, fib->m_nWordVersion, fib->m_FibBase.fComplex));
			}

			//if (PAPXlist->back()->rgfc[PAPXlist->back()->rgfcSize-1] < last)
			//{
			//	PAPXlist->back()->rgfc[PAPXlist->back()->rgfcSize-1] = last;
			//	//tableStream->read( binTablePapx, fib->m_FibWord97.lcbPlcfBtePapx);
			//	//offset+=512;
			//	//PAPXlist->push_back( new FormattedDiskPagePAPX( wordStream, offset, dataStream ) );
			//}
		}
		else
		{
			int n = (((int)fib->m_FibWord97.lcbPlcfBtePapx - 4) / 8) + 1;
			//Get the indexed PAPX FKPs
			for (unsigned int i = (n * 4); i < fib->m_FibWord97.lcbPlcfBtePapx; i += 4)
			{
				//indexed FKP is the xth 512byte page
				int fkpnr = FormatUtils::BytesToInt32(binTablePapx, i, fib->m_FibWord97.lcbPlcfBtePapx);

				//so starts at:
				int offset = fkpnr * 512;

				PAPXlist->push_back(new FormattedDiskPagePAPX(wordStream, offset, dataStream, fib->m_nWordVersion, fib->m_FibBase.fComplex));
			}

		}

		RELEASEARRAYOBJECTS(binTablePapx);

		return PAPXlist;
	}

	/*========================================================================================================*/

	/// Returns a list of all PAPX FCs between they given boundaries.
	std::vector<int>* FormattedDiskPagePAPX::GetFileCharacterPositions
	(
		int fcMin,
		int fcMax,
		FileInformationBlock* fib,
		POLE::Stream* wordStream,
		POLE::Stream* tableStream,
		POLE::Stream* dataStream
	)
	{
		std::vector<int>* cpList = new std::vector<int>();
		std::vector<FormattedDiskPagePAPX*>* fkps = FormattedDiskPagePAPX::GetAllPAPXFKPs(fib, wordStream, tableStream, dataStream);
		unsigned int i = 0;
		FormattedDiskPagePAPX* fkp = NULL;

		for (size_t i = 0; i < fkps->size(); ++i)
		{
			fkp = fkps->at(i);

			//the last entry of each is always the same as the first entry of the next FKP
			//so, ignore all last _entries except for the last FKP.
			int max_ = fkp->rgfcSize;

			if (i++ < fkps->size() - 1)
			{
				max_--;
			}

			for (int j = 0; j < max_; j++)
			{
				if ((fkp->rgfc[j] >= fcMin) && (fkp->rgfc[j] < fcMax))
				{
					cpList->push_back(fkp->rgfc[j]);
				}
			}

			RELEASEOBJECT(fkp);
		}

		fkps->clear();
		RELEASEOBJECT(fkps);

		return cpList;
	}

	/*========================================================================================================*/

	/// Returnes a list of all ParagraphPropertyExceptions which correspond to text 
	/// between the given offsets.
	std::vector<ParagraphPropertyExceptions*>* FormattedDiskPagePAPX::GetParagraphPropertyExceptions
	(
		int fcMin,
		int fcMax,
		FileInformationBlock* fib,
		POLE::Stream* wordStream,
		POLE::Stream* tableStream,
		POLE::Stream* dataStream
	)
	{
		std::vector<ParagraphPropertyExceptions*>* ppxList = new std::vector<ParagraphPropertyExceptions*>();
		std::vector<FormattedDiskPagePAPX*>* fkps = FormattedDiskPagePAPX::GetAllPAPXFKPs(fib, wordStream, tableStream, dataStream);

		for (size_t i = 0; i < fkps->size(); ++i)
		{
			FormattedDiskPagePAPX* fkp = fkps->at(i);

			for (unsigned int j = 0; j < fkp->grppapxSize; j++)
			{
				if ((fkp->rgfc[j] >= fcMin) && (fkp->rgfc[j] < fcMax))
				{
					ppxList->push_back(fkp->grppapx[j]);
				}
			}

			RELEASEOBJECT(fkp);

			fkps->clear();
			RELEASEOBJECT(fkps);
		}
		return ppxList;
	}
}