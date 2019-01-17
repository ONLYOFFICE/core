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

namespace ASCDocFileFormat
{
	namespace FIB_OFFSET
	{
		enum FIB
		{
			csw					=	32,
			cslw				=	62,
			cbRgFcLcb			=	152
		};

		enum FibBase
		{
			wIdent				=	0,
			nFib				=	2,
			lid					=	6,
			pnNext				=	8,
			nFibBack			=	12,
			lKey				=	14, 
			envr				=	18
		};

		enum FibRgLw97
		{
			cbMac				=	64,
			ccpText				=	76,
			ccpFtn				=	80,
			ccpHdd				=	84,
			ccpAtn				=	92,	
			ccpEdn				=	96,
			ccpTxbx 			=	100,
			ccpHdrTxbx 			=	104
		};

		enum FibRgFcLcb97			 //	FibRgFcLcb97
		{
			fcStshfOrig			=	154, //	This value is undefined and MUST be ignored. 
			lcbStshfOrig		=	158, //	This value is undefined and MUST be ignored. 
			fcStshf				=	162, //	specifies the style sheet for this document begins at this offset
			lcbStshf			=	166, //	An unsigned integer that specifies the size, in bytes, of the STSH that begins at offset fcStshf in the Table Stream. This MUST be a nonzero value
			fcPlcfSed			=	202,
			lcbPlcfSed			=	206,			
			fcSttbfFfn			=	274,
			lcbSttbfFfn			=	278,	
			fcDop				=	402, // An unsigned integer that specifies an offset in the Table Stream. A Dopbegins at this offset. 
			lcbDop				=	406, // An unsigned integer that specifies the size, in bytes, of the Dopat fcDop. This value MUST NOT be zero.
			fcClx				=	418, // An unsigned integer that specifies an offset in the Table Stream. A Clx begins at this offset. 
			lcbClx				=	422, // An unsigned integer that specifies the size, in bytes, of the Clx at offset fcClx in the Table Stream. This value MUST be greater than zero. 
			fcPlcSpaMom			=	474, //	An unsigned integer that specifies an offset in the Table Stream. A PlcfSpa begins at this offset. The PlcfSpa contains shape information for the Main Document
			lcbPlcSpaMom		=	478, //	An unsigned integer that specifies the size, in bytes, of the PlcfSpa at offset fcPlcSpaMom
			fcPlcSpaHdr			=	482, //	A PlcfSpa begins at this offset. The PlcfSpa contains shape information for the Header Document.
			lcbPlcSpaHdr		=	486, // An unsigned integer that specifies the size, in bytes, of the PlcfSpa at the offset fcPlcSpaHdr			
			fcDggInfo			=	554, //	An OfficeArtContent that contains information about the drawings in the document begins at this offset.  
			lcbDggInfo			=	558, //	An unsigned integer that specifies the size, in bytes, of the OfficeArtContent at the offset fcDggInfo. If lcbDggInfo is zero, there MUST NOT be any drawings in the document.
			
			fcPlcftxbxTxt		=	602,	//
			lcbPlcftxbxTxt		=	606,

			fcPlcfHdrtxbxTxt	=	618,
			lcbPlcfHdrtxbxTxt	=	622,

			fcPlcfTxbxBkd		=	754,	//	смещение в стриме для структур описывающих текст в автофигурах (main document)
			lcbPlcfTxbxBkd		=	758,
			fcPlcfTxbxHdrBkd	=	762,	//	смещение в стриме для структур описывающих текст в автофигурах (header)
			lcbPlcfTxbxHdrBkd	=	766

		};
	}
}