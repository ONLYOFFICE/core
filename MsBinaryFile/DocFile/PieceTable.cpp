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

#include "PieceTable.h"

namespace DocFileFormat
{
	PieceTable::PieceTable (FileInformationBlock *fib, POLE::Stream *tableStream, POLE::Stream* wordStream)
	{
		if (fib->m_FibWord97.lcbClx < 1/* || !fib->m_FibBase.fComplex*/) return;

		// Read the bytes of complex file information
		unsigned char* clx	=	new unsigned char[fib->m_FibWord97.lcbClx];

		if (tableStream)
		{
			tableStream->seek(fib->m_FibWord97.fcClx);
			tableStream->read(clx, (int)fib->m_FibWord97.lcbClx);
		}

		Pieces					= std::list<PieceDescriptor>();
		FileCharacterPositions	= new std::map<int, int>();
		CharacterPositions		= new std::map<int, int>();

		int pos = 0;
		bool goon = true;
		while (goon)
		{
			try
			{
				unsigned char type = clx[pos];

				// check if the type of the entry is a piece table

				if (2 == type)
				{					
					// piecetable was found
					goon = false;

					int lcb = FormatUtils::BytesToInt32( clx, pos + 1, fib->m_FibWord97.lcbClx );

					//read the piece table
					unsigned char* piecetable = new unsigned char[lcb];
					memcpy(piecetable, (clx + pos + 5), lcb);

					//count of PCD _entries
					int n = (lcb - 4) / 12;

					//and n piece descriptors
					for (int i = 0; i < n; ++i)
					{
						//read the CP 
						int indexCp = i * 4;
						int cp = FormatUtils::BytesToInt32(piecetable, indexCp, lcb);

						//read the next CP
						int indexCpNext = (i+1) * 4;
						int cpNext = FormatUtils::BytesToInt32(piecetable, indexCpNext, lcb);

						//read the PCD
						int indexPcd = ((n + 1) * 4) + (i * 8);
						unsigned char *pcdBytes = new unsigned char[8];

						memcpy(pcdBytes, (piecetable + indexPcd), 8);

						PieceDescriptor pcd(pcdBytes, 8, fib->m_CodePage);

						pcd.cpStart	=	cp;
						pcd.cpEnd	=	cpNext;

						//add pcd
						Pieces.push_back(pcd);

						//add positions
						int f = (int)pcd.fc;
						int multi = 1;

						if ( pcd.code_page == ENCODING_UTF16 )
						{
							multi = 2;
						}

						for (int c = pcd.cpStart; c < pcd.cpEnd; ++c)
						{
							if (FileCharacterPositions->find(c) == FileCharacterPositions->end())
								FileCharacterPositions->insert(Int_Pair(c, f));

							if (FileCharacterPositions->find(f) == FileCharacterPositions->end())
								CharacterPositions->insert(Int_Pair(f, c));

							f += multi;
						}

						RELEASEARRAYOBJECTS( pcdBytes );
					}

					int maxCp = (int)FileCharacterPositions->size();

					FileCharacterPositions->insert( Int_Pair( maxCp, fib->m_FibBase.fcMac ) );
					CharacterPositions->insert( Int_Pair( fib->m_FibBase.fcMac, maxCp ) );

					RELEASEARRAYOBJECTS(piecetable);
				}				
				else if (type == 1)	// skip this entry 
				{
					// short cb = FormatUtils::BytesToInt16( clx, pos + 1, fib->m_FibWord97.lcbClx );
					// pos = pos + 1 + 2 + cb;

					pos = pos + 1 + 2 + clx[pos + 1];
				}
				else
				{
					goon = false;
				}
			}
			catch (...)
			{
				goon = false;
			}
		}

		RELEASEARRAYOBJECTS(clx);
		m_carriageIter = Pieces.begin();
	}

	PieceTable::~PieceTable ()
	{
		RELEASEOBJECT(FileCharacterPositions);
		RELEASEOBJECT(CharacterPositions);
	}
}

namespace DocFileFormat
{
	std::vector<wchar_t>* PieceTable::GetAllEncodingText(POLE::Stream* stream)
	{
		std::vector<wchar_t> *piecePairs = new std::vector<wchar_t>();

		for ( std::list<PieceDescriptor>::iterator iter = this->Pieces.begin(); iter != this->Pieces.end(); ++iter)
		{
			//get the FC end of this piece
			PieceDescriptor pcd = *iter;

			int pcdFcEnd = pcd.cpEnd - pcd.cpStart;

			if (pcd.code_page == ENCODING_UTF16)
			{
				pcdFcEnd *= 2;
			}

			pcdFcEnd += (int)pcd.fc;

			int cb = pcdFcEnd - (int)pcd.fc;

			unsigned char *bytes = new unsigned char[cb];

			//read all bytes 

			stream->seek(pcd.fc);
			stream->read(bytes, cb);

			FormatUtils::GetSTLCollectionFromBytes<std::vector<wchar_t> >(piecePairs, bytes, cb, pcd.code_page);

			RELEASEARRAYOBJECTS(bytes);
		}

		return piecePairs;
	}

	std::vector<wchar_t>* PieceTable::GetEncodingChars (int fcStart, int fcEnd, POLE::Stream* wordStream)
	{
		std::vector<wchar_t> *encodingChars = new std::vector<wchar_t>();

		for (std::list<PieceDescriptor>::iterator iter = Pieces.begin(); iter != Pieces.end(); ++iter)
		{
			PieceDescriptor pcd = *iter;

			//get the FC end of this piece
			int pcdFcEnd = pcd.cpEnd - pcd.cpStart;

			if ( pcd.code_page == ENCODING_UTF16 )
			{
				pcdFcEnd *= 2;
			}

			pcdFcEnd += (int)pcd.fc;

			if ( pcdFcEnd < fcStart )
			{
				//this piece is before the requested range
				continue;
			}
			else if ( ( fcStart >= (int)pcd.fc ) && ( fcEnd > pcdFcEnd ) )
			{
				//requested char range starts at this piece
				//read from fcStart to pcdFcEnd

				//get count of bytes
				int cb = pcdFcEnd - fcStart;
				unsigned char *bytes = new unsigned char[cb];

				//read all bytes

				wordStream->seek( fcStart);
				wordStream->read( bytes, cb);

				//get the chars
				FormatUtils::GetSTLCollectionFromBytes<std::vector<wchar_t>>( encodingChars, bytes, cb, pcd.code_page );

				RELEASEARRAYOBJECTS( bytes );
			}
			else if ( ( fcStart <= (int)pcd.fc ) && ( fcEnd >= pcdFcEnd ) )
			{
				//the full piece is part of the requested range
				//read from pc.fc to pcdFcEnd
				//get count of bytes

				int cb = pcdFcEnd - (int)pcd.fc;
				unsigned char *bytes = new unsigned char[cb];

				//read all bytes

				wordStream->seek( pcd.fc);
				wordStream->read( bytes, cb);

				//get the chars
				FormatUtils::GetSTLCollectionFromBytes<std::vector<wchar_t>>( encodingChars, bytes, cb, pcd.code_page );

				RELEASEARRAYOBJECTS( bytes );
			}
			else if ( ( fcStart < (int)pcd.fc ) && ( fcEnd >= (int)pcd.fc ) && ( fcEnd <= pcdFcEnd ) )
			{
				//requested char range ends at this piece
				//read from pcd.fc to fcEnd

				//get count of bytes
				int cb = fcEnd - (int)pcd.fc;
				unsigned char *bytes = new unsigned char[cb];

				//read all bytes

				wordStream->seek( pcd.fc);
				wordStream->read( bytes, cb);

				//get the chars
				FormatUtils::GetSTLCollectionFromBytes<std::vector<wchar_t>>(encodingChars, bytes, cb, pcd.code_page);

				RELEASEARRAYOBJECTS(bytes);

				break;
			}
			else if ((fcStart >= (int)pcd.fc) && (fcEnd <= pcdFcEnd))
			{
				//requested chars are completly in this piece
				//read from fcStart to fcEnd

				//get count of bytes
				int cb = fcEnd - fcStart;
				if (cb < 0) break;

				unsigned char *bytes = new unsigned char[cb];

				//read all bytes

				wordStream->seek( fcStart);
				wordStream->read( bytes, cb );

				//get the chars
				FormatUtils::GetSTLCollectionFromBytes<std::vector<wchar_t>>( encodingChars, bytes, cb, pcd.code_page );

				RELEASEARRAYOBJECTS( bytes );

				//set the list
				//chars = new List<char>(plainChars); !!!TODO!!!

				break;
			}
			else if ( fcEnd < (int)pcd.fc )
			{
				if ((int)pcd.fc > (int)fcEnd)
				{
					pcdFcEnd = 0;
					continue;
				}

				//this piece is beyond the requested range
				break;
			}
		}
		return encodingChars;
	}
	std::vector<wchar_t>* PieceTable::GetChars(int fcStart, int fcEnd, int cp, POLE::Stream* word)
	{
		std::vector<wchar_t>* encodingChars = new std::vector<wchar_t>();

		int fcSize = fcEnd - fcStart;

		bool read = true;

		for (std::list<PieceDescriptor>::iterator iter = Pieces.begin(); iter != Pieces.end(); ++iter)
		{
			PieceDescriptor pcd = (*iter);

			if (cp >= pcd.cpEnd)
				continue;

			int pcdFcEnd = pcd.cpEnd - pcd.cpStart;

			if (pcd.code_page == ENCODING_UTF16)
			{
				pcdFcEnd *= 2;
			}

			pcdFcEnd += (int)pcd.fc;

			if (pcdFcEnd < fcStart)
			{
				//this piece is before the requested range
				continue;
			}
			else if ((fcStart >= (int)pcd.fc) && (fcEnd > pcdFcEnd))
			{
				//requested char range starts at this piece
				//read from fcStart to pcdFcEnd

				int cb = pcdFcEnd - fcStart;
				if (cb < 0)	
					break;

				if (!ReadSymbolsBuffer((int)fcStart, cb, pcd.code_page, word, encodingChars))
					break;

				fcSize	-=	cb;

				if (read)
				{
					m_carriageIter	=	iter;	
					read			=	false;
				}
			}
			else if ((fcStart <= (int)pcd.fc) && (fcEnd >= pcdFcEnd))
			{
				//the full piece is part of the requested range
				//read from pc.fc to pcdFcEnd

				int cb = pcdFcEnd - (int)pcd.fc;
				if (cb < 0)
					break;

				if (!ReadSymbolsBuffer((int)pcd.fc, cb, pcd.code_page, word, encodingChars))
					break;

				fcSize	-=	cb;

				if (read)
				{
					m_carriageIter	=	iter;	
					read			=	false;
				}
			}
			else if ((fcStart < (int)pcd.fc) && (fcEnd >= (int)pcd.fc) && (fcEnd <= pcdFcEnd))
			{
				//requested char range ends at this piece
				//read from pcd.fc to fcEnd

				int cb = fcEnd - (int)pcd.fc;
				if (cb == 0) //ralph_scovile.doc
				{
					if (pcd.cpEnd - pcd.cpStart < 2) // com2010_0624pl01.doc
						cb = pcdFcEnd - (int)pcd.fc; 
					if (cb <= 0)
						break;
				}
				else if (cb < 0)
					break;

				if (!ReadSymbolsBuffer((int)pcd.fc, cb, pcd.code_page, word, encodingChars))
					break;

				if (read)
				{
					m_carriageIter	=	iter;	
					read			=	false;
				}

				break;
			}
			else if ((fcStart >= (int)pcd.fc) && (fcEnd <= pcdFcEnd))
			{
				//requested chars are completly in this piece
				//read from fcStart to fcEnd

				//get count of bytes
				int cb = fcEnd - fcStart;
				if (cb <= 0) 
					break;

				if (!ReadSymbolsBuffer((int)fcStart, cb, pcd.code_page, word, encodingChars))
					break;

				if (read)
				{
					m_carriageIter	=	iter;	
					read			=	false;
				}

				break;
			}
			else if (fcEnd < (int)pcd.fc)		//	this piece is beyond the requested range
			{	
				// имеет место быть перескок по стриму, поэтому корректируем начальную позицию

				//size_t count = encodingChars->size();
				//if (count && fcSize > 0)
				//{
				//	if (1 == count)
				//	{
				//		if (13 == encodingChars->operator[](0))
				//			break;
				//	}

				//	if (read)
				//	{
				//		m_carriageIter	=	iter;	
				//		read			=	false;
				//	}

				//	int length = pcdFcEnd - pcd.fc;
				//	if (length > fcSize)
				//	{
				//		ReadSymbolsBuffer((int)pcd.fc, fcSize, pcd.code_page, word, encodingChars);
				//		break;
				//	}

				//	ReadSymbolsBuffer((int)pcd.fc, length, pcd.code_page, word, encodingChars);
				//	fcSize	-=	length;

				//	continue;
				//}

				break;
			}
		}

		return encodingChars;
	}

	bool PieceTable::ReadSymbolsBuffer(int pos, int size, int coding, POLE::Stream* word, std::vector<wchar_t>* encodingChars)
	{
		unsigned char* bytes = new unsigned char[size];
		if (NULL == bytes)
			return false;

		word->seek(pos);
		word->read(bytes, size);


        FormatUtils::GetSTLCollectionFromBytes<std::vector<wchar_t>>(encodingChars, bytes, size, coding);

        RELEASEARRAYOBJECTS(bytes);

		return true;
	}
}
