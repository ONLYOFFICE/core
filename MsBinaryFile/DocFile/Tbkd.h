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

		short				fReusable;		//	set to 0 because one TbRef is bound to one element
	
		int					itxbxsDest;		//	This field MUST be ignored.
		int					lid;			
		int					txidUndo;		//	This value MUST be zero and MUST be ignored.
	};
}
