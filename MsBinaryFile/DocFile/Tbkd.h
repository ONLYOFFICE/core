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

#include "ByteStructure.h"

namespace DocFileFormat
{
	// BreaK Descriptor (BKD)
	class Tbkd : public ByteStructure
	{
	public:
		friend class TextboxMapping;

		static const int STRUCTURE_SIZE = 6;

		Tbkd();
		virtual ~Tbkd();

		virtual ByteStructure* ConstructObject (VirtualStreamReader* reader, int length);

	private:
		bool bUsed = false;
		/// Except in textbox BKD, index to PGD in plfpgd that describes the page this break is on
		short ipgd;
		/// In textbox BKD
		short itxbxs;
		/// Number of cp's considered for this break; note that the CP's described by cpDepend in this break reside in the next BKD
		short dcpDepend;
		unsigned short icol;
		/// When true, this indicates that this is a table break.
		bool fTableBreak;
		/// When true, this indicates that this is a column break.
		bool fColumnBreak;
		/// Used temporarily while Word is running.
		bool fMarked;
		/// In textbox BKD, when true indicates cpLim of this textbox is not valid
		bool fUnk;
		/// In textbox BKD, when true indicates that text overflows the end of this textbox
		bool fTextOverflow;
	};

	class FTXBXS : public ByteStructure
	{
		friend class TextboxMapping;

	public:
		static const int STRUCTURE_SIZE		= 22;
		
		struct FTXBXSReusable
		{
			int	iNextReuse;
			int cReusable;
		};

		struct FTXBXNonReusable
		{
			int cTxbx;
			int cTxbxEdit;				//	This value MUST be zero and MUST be ignored.
		};

		FTXBXS();
		virtual ~FTXBXS();
	
		virtual ByteStructure* ConstructObject (VirtualStreamReader* reader, int length);

	private:
		int					reusable01;
		int					reusable02;

		short				fReusable;		//	ставим в 0 потому что один TbRef привязан к одному элементу
	
		int					itxbxsDest;		//	This field MUST be ignored.
		int					lid;			
		int					txidUndo;		//	This value MUST be zero and MUST be ignored.
	};
}
