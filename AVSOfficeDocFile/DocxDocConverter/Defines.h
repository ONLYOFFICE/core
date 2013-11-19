#pragma once

namespace AVSDocFileFormat
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