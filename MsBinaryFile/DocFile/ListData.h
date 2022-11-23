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
#pragma once

#include "ListLevel.h"
#include "ByteStructure.h"

namespace DocFileFormat
{
	class ListData;
	typedef class boost::shared_ptr<ListData> ListDataPtr;

	class ListData
	{
		friend class ListTable;
		friend class NumberingMapping;

	private:
		/// Unique List ID
		int lsid;
		/// Unique template code
		int tplc;
		/// Array of shorts containing the istd‘s linked to each level of the list, 
		/// or ISTD_NIL (4095) if no style is linked.
		std::vector<short> rgistd;
		/// True if this is a simple (one-level) list.
		/// False if this is a multilevel (nine-level) list.
		bool fSimpleList;
		/// Word 6.0 compatibility option:
		/// True if the list should start numbering over at the beginning of each section.
		bool fRestartHdn;
		/// To emulate Word 6.0 numbering:
		/// True if Auto numbering
		bool fAutoNum;
		/// When true, this list was there before we started reading RTF.
		bool fPreRTF;
		/// When true, list is a hybrid multilevel/simple (UI=simple, internal=multilevel)
		bool fHybrid;
		/// Array of ListLevel describing the several levels of the list.
		std::vector<ListLevel*>* rglvl;
		/// A grfhic that specifies HTML incompatibilities of the list.
		unsigned char grfhic;

	public:
		static const int		LSTF_LENGTH		= 28;
		static const short		ISTD_NIL		= 4095;
		static const int		VARIABLE_LENGTH = INT_MAX;

		virtual ~ListData();
		ListData( VirtualStreamReader* reader, int length );

		static ListDataPtr create(VirtualStreamReader* reader, int length);
	};

	class NumberingDescriptor;
	typedef class boost::shared_ptr<NumberingDescriptor> NumberingDescriptorPtr;
	
	class NumberingDescriptor : public IVisitable
	{
		friend class ListTable;
		friend class NumberingMapping;
	
	private:
		size_t			id;
		unsigned char	nfc;
		unsigned char	cbTextBefore;
		unsigned char	cbTextAfter;
		unsigned char	jc;

		bool			fPrev;
		bool			fHang;

		bool			fSetBold;
		bool			fSetItalic;
		bool			fSetSmallCaps;
		bool			fSetCaps;
		bool			fSetStrike;
		bool			fSetKul;

		bool			fPrevSpace;
		bool			fBold;
		bool			fItalic;
		bool			fSmallCaps;
		bool			fCaps;
		bool			fStrike;

		unsigned char	kul;
		unsigned char	ico;
		short			ftc;
		unsigned short	hps;
		unsigned short	iStartAt;
		unsigned short	dxaIndent;
		unsigned short	dxaSpace;

		unsigned char	fNumber1;
		unsigned char	fNumberAcross;
		unsigned char	fRestartHdn;
		unsigned char	fSpareX;

		std::wstring	xst; //32 chars ansi

	public:
		std::wstring rPr;

		bool operator == (const NumberingDescriptor & val) const;
		bool operator == (const NumberingDescriptorPtr & val) const;

		virtual ~NumberingDescriptor(){}
		// Parses the given StreamReader to retrieve a ANLD struct
		NumberingDescriptor( unsigned char * data, int length ); //cbANLD (count of bytes of ANLD) is 52 
		
		static NumberingDescriptorPtr create(unsigned char * data, int length);
	};

	
	class OutlineListDescriptor : public IVisitable, public ByteStructure 
	{
		friend class ListTable;
		friend class NumberingMapping;
	
	private:

        NumberingLevelDescriptor lvl[9];
		
		unsigned char	fRestartHdr;
		unsigned char	fSpareOlst2;
		unsigned char	fSpareOlst3;
		unsigned char	fSpareOlst4;

		std::wstring	xst; //64 chars ansi

		static const int STRUCTURE_SIZE = 212;
		static const int STRUCTURE_SIZE_OLD = 10;
    public:
		static const int GetSize(int nWordVersion)
		{
			return (nWordVersion == 2) ? STRUCTURE_SIZE_OLD : STRUCTURE_SIZE;
		}
        virtual ByteStructure* ConstructObject( VirtualStreamReader* reader, int length );
		
		virtual ~OutlineListDescriptor();
		// Parses the given StreamReader to retrieve a OLST struct
		OutlineListDescriptor( unsigned char * data, int length ); //cbOLST(count of bytes of OLST) is 212 
		OutlineListDescriptor() {}
	};
}
