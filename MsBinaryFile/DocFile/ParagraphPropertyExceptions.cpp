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

#include "ParagraphPropertyExceptions.h"

namespace DocFileFormat
{
	ParagraphPropertyExceptions::ParagraphPropertyExceptions() : PropertyExceptions(), istd(0)
	{
	}
	ParagraphPropertyExceptions::ParagraphPropertyExceptions( const std::list<SinglePropertyModifier>& grpprl ):
														  PropertyExceptions( grpprl ), istd(0)
	{
	}
	ParagraphPropertyExceptions::~ParagraphPropertyExceptions()
	{
	}

	ParagraphPropertyExceptions::ParagraphPropertyExceptions( unsigned char* bytes, int size, POLE::Stream* dataStream, int nWordVersion): 
				PropertyExceptions( ( bytes + 2 ), ( size - 2 ),  nWordVersion)
	{
		if (size < 1) return;

		if (nWordVersion > 1)
		{		
			istd =  bytes[0];

			ReadExceptions(bytes + 7, size - 7,  nWordVersion);

	//		RELEASEOBJECT( grpprl );
	//		grpprl = new std::list<SinglePropertyModifier>();
	//		
	//		MemoryStream oStream(bytes, size);
	//		int pos = 1;
	//		if (pos + 1 > size) return;
	//		unsigned char  jc	= oStream.ReadByte(); pos += 1;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPJc, 1, (BYTE*)&jc));

	//		if (pos + 1 > size) return;
	//		unsigned char  fSideBySide	= oStream.ReadByte(); pos += 1;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPFSideBySide, 1, (BYTE*)&fSideBySide));

	//		if (pos + 1 > size) return;
	//		unsigned char  fKeep	= oStream.ReadByte(); pos += 1;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPFKeep, 1, (BYTE*)&fKeep));

	//		if (pos + 1 > size) return;
	//		unsigned char  fKeepFollow	= oStream.ReadByte(); pos += 1;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPFKeepFollow, 1, (BYTE*)&fKeepFollow));

	//		if (pos + 1 > size) return;
	//		unsigned char  fPageBreakBefore	= oStream.ReadByte(); pos += 1;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPPageBreakBefore, 1, (BYTE*)&fPageBreakBefore));
	//		
	//		if (pos + 1 > size) return;
	//		unsigned char  flag	= oStream.ReadByte(); pos += 1;
	//		unsigned char  pcVert = GETBITS(flag, 4, 5);
	//		unsigned char  pcHorz = GETBITS(flag, 6, 7);
	//		//grpprl->push_back(SinglePropertyModifier(sprmOldPPageBreakBefore, 1, &fPageBreakBefore));
	//		//grpprl->push_back(SinglePropertyModifier(sprmOldPPageBreakBefore, 1, &fPageBreakBefore));

	//		if (pos + 1 > size) return;
	//		unsigned char  brcp	= oStream.ReadByte(); pos += 1;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPBrcp, 1, (BYTE*)&brcp));
	//		
	//		if (pos + 1 > size) return;
	//		unsigned char  brcl	= oStream.ReadByte(); pos += 1;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPBrcl, 1, (BYTE*)&brcl));

	//		if (pos + 1 > size) return;
	//		unsigned char  nfcSeqNumb	= oStream.ReadByte(); pos += 1;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPPageBreakBefore, 1, (BYTE*)&nfcSeqNumb));
	//		
	//		if (pos + 1 > size) return;
	//		unsigned char  nnSeqNumb	= oStream.ReadByte(); pos += 1;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPNLvlAnm, 1, (BYTE*)&nnSeqNumb));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  dxaRight	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPDxaRight, 2, (BYTE*)&dxaRight));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  dxaLeft	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPDxaLeft, 2, (BYTE*)&dxaLeft));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  dxaLeft1	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPDxaLeft1, 2, (BYTE*)&dxaLeft1));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  dyaLine	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPDyaLine, 2, (BYTE*)&dyaLine));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  dyaBefore	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPDyaBefore, 2, (BYTE*)&dyaBefore));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  dyaAfter	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPDyaAfter, 2, (BYTE*)&dyaAfter));
	////phe - 6		
	//		if (pos + 6 > size) return;
	//		
	//		//oStream.ReadUInt16(); 
	//		//oStream.ReadUInt16(); 
	//		//oStream.ReadUInt16(); 

	//		if (pos + 1 > size) return;
	//		unsigned char  fInTable	= oStream.ReadUInt16(); pos += 1;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPFInTable, 1, (BYTE*)&fInTable));
	//		
	//		if (pos + 1 > size) return;
	//		unsigned char  fTtp	= oStream.ReadUInt16(); pos += 1;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPTtp, 1, (BYTE*)&fTtp));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  ptap	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPPageBreakBefore, 2, (BYTE*)&ptap));

	//		if (pos + 2 > size) return;
	//		unsigned short  dxaAbs	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPDxaAbs, 2, (BYTE*)&dxaAbs));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  dyaAbs	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPDyaAbs, 2, (BYTE*)&dyaAbs));

	//		if (pos + 2 > size) return;
	//		unsigned short  dxaWidth	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPDxaWidth, 2, (BYTE*)&dxaWidth));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  brcTop	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPBrcTop10, 2, (BYTE*)&brcTop));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  brcLeft	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPBrcLeft10, 2, (BYTE*)&brcLeft));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  brcBottom	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPBrcBottom10, 2, (BYTE*)&brcBottom));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  brcRight	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPBrcRight10, 2, (BYTE*)&brcRight));

	//		if (pos + 2 > size) return;
	//		unsigned short  brcBetween	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPBrcBetween10, 2, (BYTE*)&brcBetween));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  brcBar	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPBrcBar10, 2, (BYTE*)&brcBar));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  dxaFromText	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPPageBreakBefore, 2, (BYTE*)&dxaFromText));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  dyaFromText	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPFromText10, 2, (BYTE*)&dyaFromText));
	//		
	//		if (pos + 1 > size) return;
	//		unsigned char  wr	= oStream.ReadByte(); pos += 1;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPWr, 1, &wr));
	//		
	//		if (pos + 1 > size) return;
	//		unsigned char  zz	= oStream.ReadByte(); pos += 1;
	//		
	//		if (pos + 1 > size) return;
	//		unsigned char  fTransparent	= oStream.ReadByte(); pos += 1;
	//		
	//		if (pos + 1 > size) return;
	//		unsigned char  fBiDi	= oStream.ReadByte(); pos += 1;
	//		grpprl->push_back(SinglePropertyModifier(sprmPFBiDi, 1, &fBiDi));
	//		
	//		if (pos + 1 > size) return;
	//		unsigned char  bSpare	= oStream.ReadByte(); pos += 1;
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short dyaHeight	= oStream.ReadUInt16(); pos += 2;
	//		unsigned char fMinHeight = GETBIT(dyaHeight, 15);
	//		dyaHeight = GETBITS(dyaHeight, 0, 14);
	//		grpprl->push_back(SinglePropertyModifier(sprmOldTDyaRowHeight, 2, (BYTE*)&dyaHeight));
	//		//grpprl->push_back(SinglePropertyModifier(sprmOldTDyaRowHeight, 1, &fMinHeight));
	//		
	//		if (pos + 2 > size) return;
	//		unsigned short  shd	= oStream.ReadUInt16(); pos += 2;
	//		grpprl->push_back(SinglePropertyModifier(sprmOldPShd, 2, (BYTE*)&shd));

	//		if (pos + 2 > size) return;
	//		unsigned short  itbdMac	= oStream.ReadUInt16(); pos += 2;
	//		if (itbdMac > 0)
	//		{
	//			short *tabs = new short[itbdMac + 1];
	//			for (unsigned short i = 0; i < itbdMac; i++)
	//			{
	//				tabs[i + 1]	= oStream.ReadUInt16(); pos += 2;
	//			}
	//			grpprl->push_back(SinglePropertyModifier(sprmPChgTabs, 2 * (itbdMac + 1), (BYTE*)tabs));
	//			delete []tabs;
	//		}
		}
		else 
		{
			istd = FormatUtils::BytesToUInt16( bytes, 0, size );
			VirtualStreamReader *reader = NULL;

			//There is a SPRM that points to an offset in the data stream, 
			//where a list of SPRM is saved.
			for ( std::list<SinglePropertyModifier>::iterator iter = grpprl->begin(); iter != grpprl->end(); iter++ )
			{
				SinglePropertyModifier sprm( *iter );

				if( ( sprm.OpCode == sprmPHugePapx ) || ( (int)sprm.OpCode == 0x6646 ) )
				{
					unsigned int fc = FormatUtils::BytesToUInt32( sprm.Arguments, 0, sprm.argumentsSize );
					reader = new VirtualStreamReader( dataStream, (int)fc, nWordVersion);

					//parse the size of the external grpprl
					unsigned char* sizebytes = reader->ReadBytes( 2, true );

					unsigned int grpprlsize = FormatUtils::BytesToUInt16( sizebytes, 0, 2 );

					RELEASEARRAYOBJECTS( sizebytes );

					//parse the external grpprl
					unsigned char* grpprlBytes = reader->ReadBytes( grpprlsize, true );

					PropertyExceptions externalPx( grpprlBytes, grpprlsize, nWordVersion );

					//assign the external grpprl
					RELEASEOBJECT( grpprl );
					grpprl = new std::list<SinglePropertyModifier>( *(externalPx.grpprl) );

					//remove the sprmPHugePapx
					grpprl->remove( sprm );

					RELEASEARRAYOBJECTS( grpprlBytes );
					RELEASEOBJECT( reader )

					break;
				}
			}
		}
    }
}
