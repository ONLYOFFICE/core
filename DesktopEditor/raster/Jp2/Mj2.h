#pragma once

#include "Types.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Данные функции предназначены для чтения Motion JPEG 2000 (MJ2)
	//-------------------------------------------------------------------------------------------------------------------------------

	static bool Mj2_ReadBoxHeader(Mj2_Box* pBox, CReader * pStream)
	{
		if (!pBox)
			return false;

		pBox->nInitPos =  pStream->Tell();
		pBox->nLength  = pStream->Read(4);
		pBox->nType    = pStream->Read(4);

		if (1 == pBox->nLength)
		{
			if (0 != pStream->Read(4))
			{
				Event_Message(EVT_ERROR, "Error: Cannot handle box sizes higher than 2^32\n");
				return false;
			}
			pBox->nLength = pStream->Read(4);
			if (0 == pBox->nLength)
				pBox->nLength = pStream->GetLeftSize() + 12;
		}
		else if (0 == pBox->nLength)
		{
			int nBytesLeft = pStream->GetLeftSize();
			if (0 == nBytesLeft)
				return false;

			pBox->nLength = nBytesLeft + 8;
		}
		return true;
	}
	static bool Mj2_ReadJP(CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);

		if (oBox.nType != MJ2_JP)
		{
			Event_Message(EVT_ERROR, "Error: Expected JP Marker\n");
			return false;
		}
		if (0x0d0a870a != pStream->Read(4)) // read the 0x0d0a870a required in a JP box
		{
			Event_Message(EVT_ERROR, "Error with JP Marker\n");
			return false;
		}
		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with JP Box size \n");
			return false;
		}
		return true;

	}

	static bool Mj2_ReadFTYP(Mj2_Movie* pMovie, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);

		if (MJ2_FTYP != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected FTYP Marker\n");
			return false;
		}

		pMovie->unBrand         = pStream->Read(4); // BR
		pMovie->unMinVersion    = pStream->Read(4); // MinV
		pMovie->nCompListLength = (oBox.nLength - 16) / 4;
		pMovie->pCompList       = (unsigned int*)Malloc(pMovie->nCompListLength * sizeof(unsigned int));

		for (int nIndex = pMovie->nCompListLength - 1; nIndex > -1; nIndex--)
		{
			pMovie->pCompList[nIndex] = pStream->Read(4); // CLi
		}

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with FTYP Box\n");
			return false;
		}

		return true;
	}
	static bool Mj2_ReadMDAT(Mj2_Movie* pMovie, CReader * pStream, Image** ppImage, Mj2_Box oBox)
	{
		if (MJ2_MDAT != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected FTYP Marker\n");
			return false;
		}

		int nStartPos =  pStream->Tell();


		//>>>>

		// Достаем первую картинку в потоке MDAT
		Jp2Box oTempBox;
		Jp2_ReadBoxHeader(pMovie->pCodecInfo, pStream, &oTempBox);
		do
		{
			if (JP2_JP2C != oTempBox.nType)
			{
				pStream->Skip(oTempBox.nLength - 8);

				if (pStream->GetLeftSize() < 0)
					return false;

				if (!Jp2_ReadBoxHeader(pMovie->pCodecInfo, pStream, &oTempBox))
					return false;
			}
		} while (JP2_JP2C != oTempBox.nType);

		int nJ2kCodestreamOffset =  pStream->Tell();
		int nJ2kCodestreamLength = oTempBox.nLength - 8;

		// Декодируем J2K
		*ppImage = J2k_Decode(pMovie->pJ2k, pStream);
		if (!*ppImage)
		{
			Event_Message(EVT_ERROR, "Failed to decode J2K image\n");
			return false;
		}

		//<<<<<<

		int nReaded =  pStream->Tell() - nStartPos;

		pStream->Skip(oBox.nLength - nReaded - 8);
		if (pStream->GetLeftSize() < 0)
			return false;


		return true;
	}
	static bool Mj2_ReadMVHD(Mj2_Movie* pMovie, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_MVHD != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected MVHD Marker\n");
			return false;
		}


		if (0 != pStream->Read(4)) // Version = 0, flags = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in MVHD box\n");
		}

		// TO DO: Здесь в зависимости от версии разное число байт должно читаться
		//        см. fcd15444-3.pdf стр.15

		pMovie->unCreationTime     = pStream->Read(4); // Creation Time
		pMovie->unModificationTime = pStream->Read(4);	// Modification Time
		pMovie->nTimescale         = pStream->Read(4); // Timescale
		pMovie->unDuration         = pStream->Read(4); // Duration

		pMovie->nRate              = pStream->Read(4); // Rate
		pMovie->nVolume            = pStream->Read(2); // Volume

		pStream->Skip(10);                             // const bit(16) reserved = 0 + const unsigned int(32)[2] reserved = 0

		pMovie->anTransMatrix[0]   = pStream->Read(4); // Transformation matrix
		pMovie->anTransMatrix[1]   = pStream->Read(4); //
		pMovie->anTransMatrix[2]   = pStream->Read(4); //
		pMovie->anTransMatrix[3]   = pStream->Read(4); //
		pMovie->anTransMatrix[4]   = pStream->Read(4); //
		pMovie->anTransMatrix[5]   = pStream->Read(4); //
		pMovie->anTransMatrix[6]   = pStream->Read(4); //
		pMovie->anTransMatrix[7]   = pStream->Read(4); //
		pMovie->anTransMatrix[8]   = pStream->Read(4); //

		pStream->Skip(24);                             // bit(32)[6] pre-defined = 0; 

		pMovie->nNextTrackId       = pStream->Read(4); // ID of Next track to be added

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with MVHD Box Size\n");
			return false;
		}
		return true;
	}


	static bool Mj2_ReadTKHD(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);

		if (MJ2_TKHD != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected TKHD Marker\n");
			return false;
		}

		// TO DO: Здесь в зависимости от версии разное число байт должно читаться
		//        см. fcd15444-3.pdf стр.16

		if (0 != pStream->Read(1)) // Version = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in TKHD box\n");
			return false;
		}

		int nFlag = pStream->Read(3);

		if (!(1 == nFlag || 2 == nFlag || 3 == nFlag || 4 == nFlag)) // nFlags = 1, 2, 3 или 4
		{
			Event_Message(EVT_ERROR, "Error with flag in TKHD box: Expected flag 1,2,3 or 4\n");
			return false;
		}

		pTrack->unCreationTime     = pStream->Read(4); // Creation Time
		pTrack->unModificationTime = pStream->Read(4); // Modification Time
		pTrack->nTrackID           = pStream->Read(4); // Track ID
		pStream->Skip(4);                              // const unsigned int(32) reserved = 0; 
		pTrack->nDuration          = pStream->Read(4); // Duration

		pStream->Skip(8);                              // const unsigned int(32)[2] reserved = 0; 

		pTrack->nLayer             = pStream->Read(2); // Layer
		pStream->Read(2);                              // int(16) pre-defined = 0; 
		pTrack->nVolume            = pStream->Read(2); // Volume
		pStream->Skip(2);                              // const unsigned int(16) reserved = 0; 

		pTrack->anTransMatrix[0]   = pStream->Read(4); // Transformation matrix for track */
		pTrack->anTransMatrix[1]   = pStream->Read(4); // 
		pTrack->anTransMatrix[2]   = pStream->Read(4); // 
		pTrack->anTransMatrix[3]   = pStream->Read(4); // 
		pTrack->anTransMatrix[4]   = pStream->Read(4); // 
		pTrack->anTransMatrix[5]   = pStream->Read(4); // 
		pTrack->anTransMatrix[6]   = pStream->Read(4); // 
		pTrack->anTransMatrix[7]   = pStream->Read(4); // 
		pTrack->anTransMatrix[8]   = pStream->Read(4); // 

		pTrack->nVisualWidth       = pStream->Read(4); // Image Visual Width
		pTrack->nVisualHeight      = pStream->Read(4); // Image Visual Height

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with TKHD Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadMDHD(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);

		if (!(MJ2_MHDR == oBox.nType || MJ2_MDHD == oBox.nType)) // Kakadu writes MHDR instead of MDHD
		{
			Event_Message(EVT_ERROR, "Error: Expected MDHD Marker\n");
			return 1;
		}

		// TO DO: Сделать поодержку Version = 1
		if (0 != pStream->Read(1)) // Version = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in MDHD box\n");
			return false;
		}

		if (0 != pStream->Read(3)) // Flags = 0
		{
			Event_Message(EVT_ERROR, "Error with flag in MDHD box. Expected flag 0\n");
			return false;
		}

		pTrack->unCreationTime     = pStream->Read(4); // Creation Time
		pTrack->unModificationTime = pStream->Read(4); // Modification Time
		pTrack->nTimescale         = pStream->Read(4); // Timescale
		pTrack->nDuration          = pStream->Read(4); // Duration

		pTrack->nLanguage          = pStream->Read(2); // Language
		pStream->Skip(2);                              // unsigned int(16) pre-defined = 0; 

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with MDHD Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadHDLR(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);

		if (MJ2_HDLR != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected HDLR Marker\n");
			return false;
		}

		if (0 != pStream->Read(1)) // Version = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in HDLR box\n");
			return false;
		}

		if (0 != pStream->Read(3)) // Flags = 0
		{
			Event_Message(EVT_ERROR, "Error with flag in HDLR box. Expected flag 0\n");
			return false;
		}

		pStream->Skip(4);                        // unsigned int(32) pre-defined = 0; 
		pTrack->nHandlerType = pStream->Read(4); // handler-type
		pStream->Skip(12);                       // const unsigned int(32)[3] reserved = 0; 

		pTrack->nNameSize = oBox.nLength - 32;
		pTrack->sName     = (char*)Malloc(pTrack->nNameSize * sizeof(char));

		for (int nIndex = 0; nIndex < pTrack->nNameSize; nIndex++)
		{
			pTrack->sName[nIndex] = pStream->Read(1); // Name
		}

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with HDLR Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadVMHD(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_VMHD != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected VMHD Marker\n");
			return false;
		}

		if (0 != pStream->Read(1))  // Version = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in VMHD box\n");
			return false;
		}

		if (1 != pStream->Read(3))  // Flags = 1
		{
			Event_Message(EVT_ERROR, "Error with flag in VMHD box. Expected flag 1\n");
			return false;
		}

		pTrack->nTrackType    = 0;
		pTrack->nGraphicsMode = pStream->Read(2); // graphicsmode
		pTrack->anOpColor[0]  = pStream->Read(2); // opcolor
		pTrack->anOpColor[1]  = pStream->Read(2); //
		pTrack->anOpColor[2]  = pStream->Read(2); //

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with VMHD Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadSMHD(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_SMHD != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected SMHD Marker\n");
			return false;
		}

		if (0 != pStream->Read(1)) // Version = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in SMHD box\n");
			return false;
		}

		if (0 != pStream->Read(3)) // Flags = 0
		{
			Event_Message(EVT_ERROR, "Error with flag in SMHD box. Expected flag 0\n");
			return false;
		}

		pTrack->nTrackType = 1;
		pTrack->nBalance   = pStream->Read(2);

		// Init variables to zero to avoid problems when freeeing memory
		// The values will possibly be overidded when decoding the track structure
		pTrack->nNumBr             = 0;
		pTrack->nNumUrl            = 0;
		pTrack->nNumUrn            = 0;
		pTrack->unNumChunks        = 0;
		pTrack->nNumTimeToSample   = 0;
		pTrack->nNumSamplesToChunk = 0;
		pTrack->unNumSamples       = 0;

		pStream->Skip(2); // Reserved

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with SMHD Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadHMHD(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);

		if (MJ2_HMHD != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected HMHD Marker\n");
			return false;
		}

		if (0 != pStream->Read(1)) // Version = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in HMHD box\n");
			return false;
		}

		if (0 != pStream->Read(3)) // Flags = 0
		{
			Event_Message(EVT_ERROR, "Error with flag in HMHD box. Expected flag 0\n");
			return false;
		}

		pTrack->nTrackType = 2;
		pTrack->nMaxPDUsize        = pStream->Read(2); // maxPDUsize
		pTrack->nAvgPDUsize        = pStream->Read(2); // avgPDUsize
		pTrack->nMaxBitrate        = pStream->Read(4); // maxbitrate
		pTrack->nAvgBitrate        = pStream->Read(4); // avgbitrate
		pTrack->nSlidingAvgBitrate = pStream->Read(4); // slidingavgbitrate

		// Init variables to zero to avoid problems when freeeing memory
		// The values will possibly be overidded when decoding the track structure
		pTrack->nNumBr             = 0;
		pTrack->nNumUrl            = 0;
		pTrack->nNumUrn            = 0;
		pTrack->unNumChunks        = 0;
		pTrack->nNumTimeToSample   = 0;
		pTrack->nNumSamplesToChunk = 0;
		pTrack->unNumSamples       = 0;


		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with HMHD Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadURL(Mj2_TrackParams* pTrack, CReader * pStream, int nUrlNum)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_URL != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected URL Marker\n");
			return false;
		}

		if (0 != pStream->Read(1)) // Version = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in URL box\n");
			return false;
		}

		if (1 != pStream->Read(3)) // Если flags = 1, то медиа данные в файле
		{
			// TO DO: Сделать нормальное чтение строк
			pTrack->pUrl[nUrlNum].anLocation[0] = pStream->Read(4);
			pTrack->pUrl[nUrlNum].anLocation[1] = pStream->Read(4);
			pTrack->pUrl[nUrlNum].anLocation[2] = pStream->Read(4);
			pTrack->pUrl[nUrlNum].anLocation[3] = pStream->Read(4);
		}
		else
		{
			pTrack->nNumUrl--;
		}


		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with URL Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadURN(Mj2_TrackParams* pTrack, CReader * pStream, int nUrnNum)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);

		if (MJ2_URN != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected URN Marker\n");
			return false;
		}

		if (0 != pStream->Read(1)) // Version = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in URN box\n");
			return false;
		}

		if (1 != pStream->Read(3)) // Если flags = 1, то медиа данные в файле
		{
			// TO DO: Сделать нормальное чтение строк
			pTrack->pUrn[nUrnNum].anName[0]     = pStream->Read(4);
			pTrack->pUrn[nUrnNum].anName[1]     = pStream->Read(4);
			pTrack->pUrn[nUrnNum].anName[2]     = pStream->Read(4);
			pTrack->pUrn[nUrnNum].anName[3]     = pStream->Read(4);
			pTrack->pUrn[nUrnNum].anLocation[0] = pStream->Read(4);
			pTrack->pUrn[nUrnNum].anLocation[1] = pStream->Read(4);
			pTrack->pUrn[nUrnNum].anLocation[2] = pStream->Read(4);
			pTrack->pUrn[nUrnNum].anLocation[3] = pStream->Read(4);
		}


		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with URN Box size\n");
			return false;
		}
		return true;
	}


	static bool Mj2_ReadDREF(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_DREF != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected DREF Marker\n");
			return false;
		}

		if (0 != pStream->Read(1)) // Version = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in DREF box\n");
			return false;
		}

		if (0 != pStream->Read(3)) // Flags = 0
		{
			Event_Message(EVT_ERROR, "Error with flag in DREF box. Expected flag 0\n");
			return 1;
		}

		int nEntryCount = pStream->Read(4); // entry-count

		pTrack->nNumUrl = 0;
		pTrack->nNumUrn = 0;

		for (int nIndex = 0; nIndex < nEntryCount; nIndex++)
		{
			pStream->Skip(4);
			int nMarker = pStream->Read(4);

			if (MJ2_URL == nMarker)
			{
				pStream->Skip(-8);
				pTrack->nNumUrl++;

				if (!Mj2_ReadURL(pTrack, pStream, pTrack->nNumUrl))
					return false;

			}
			else if (MJ2_URN == nMarker)
			{
				pStream->Skip(-8);
				pTrack->nNumUrn++;

				if (!Mj2_ReadURN(pTrack, pStream, pTrack->nNumUrn))
					return false;
			}
			else
			{
				Event_Message(EVT_ERROR, "Error with in DREF box. Expected URN or URL box\n");
				return false;
			}

		}


		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with DREF Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadDINF(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_DINF != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected DINF Marker\n");
			return false;
		}

		if (!Mj2_ReadDREF(pTrack, pStream))
			return false;

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with DINF Box size\n");
			return false;
		}
		return true;
	}

	static void Mj2_DecompatTTS(Mj2_TrackParams* pTrack)
	{
		pTrack->unNumSamples = 0;
		for (int nIndex = 0; nIndex < pTrack->nNumTimeToSample; nIndex++)
		{
			pTrack->unNumSamples += pTrack->pTimeToSample[nIndex].nSampleCount;
		}

		pTrack->pSample = (Mj2_Sample*)Malloc(pTrack->unNumSamples * sizeof(Mj2_Sample));
		for (int nIndexTTS = 0; nIndexTTS < pTrack->nNumTimeToSample; nIndexTTS++)
		{
			for (int nIndexSample = 0; nIndexSample < pTrack->pTimeToSample[nIndexTTS].nSampleCount; nIndexSample++)
			{
				pTrack->pSample[nIndexSample].unSampleDelta = pTrack->pTimeToSample[nIndexTTS].nSampleDelta;
			}
		}
	}
	static void Mj2_DecompatSTSC(Mj2_TrackParams* pTrack)
	{
		if (1 == pTrack->nNumSamplesToChunk)
		{
			pTrack->unNumChunks = (unsigned int)ceil((double)pTrack->unNumSamples / (double)pTrack->pSampleToChunk[0].nSamplesPerChunk);
			pTrack->pChunk      = (Mj2_Chunk*)Malloc(pTrack->unNumChunks * sizeof(Mj2_Chunk));

			for (unsigned int unIndex = 0; unIndex < pTrack->unNumChunks; unIndex++)
			{
				pTrack->pChunk[unIndex].nNumSamples = pTrack->pSampleToChunk[0].nSamplesPerChunk;
			}
		}
		else
		{
			pTrack->pChunk      = (Mj2_Chunk*)Malloc(pTrack->unNumChunks * sizeof(Mj2_Chunk));
			pTrack->unNumChunks = 0;

			int nSampleNum = 0;
			for (int nIndexSTC = 0; nIndexSTC < pTrack->nNumSamplesToChunk - 1; nIndexSTC++)
			{
				for (int nIndexChunk = pTrack->pSampleToChunk[nIndexSTC].nFirstChunk - 1; nIndexChunk < pTrack->pSampleToChunk[nIndexSTC + 1].nFirstChunk - 1; nIndexChunk++)
				{
					pTrack->pChunk[nIndexChunk].nNumSamples = pTrack->pSampleToChunk[nIndexSTC].nSamplesPerChunk;
					pTrack->unNumChunks++;
					nSampleNum += pTrack->pChunk[nIndexChunk].nNumSamples;
				}
			}

			long unNumChunksOld = pTrack->unNumChunks;
			pTrack->unNumChunks += (int)(pTrack->unNumSamples - nSampleNum) / pTrack->pSampleToChunk[pTrack->nNumSamplesToChunk - 1].nSamplesPerChunk;
			for (unsigned int unIndex = pTrack->pSampleToChunk[pTrack->nNumSamplesToChunk - 1].nFirstChunk - 1; unIndex < pTrack->unNumChunks; unIndex++)
			{
				pTrack->pChunk[unIndex].nNumSamples = pTrack->pSampleToChunk[pTrack->nNumSamplesToChunk - 1].nSamplesPerChunk;
			}
			Mj2_Chunk* pChunk_new =  (Mj2_Chunk*)Malloc(pTrack->unNumChunks * sizeof(Mj2_Chunk));
			if (pChunk_new)
			{
				memcpy(pChunk_new, pTrack->pChunk, unNumChunksOld * sizeof(Mj2_Chunk));
				Free(pTrack->pChunk);
				pTrack->pChunk = pChunk_new;
			}
			else
			{
			}
		}
	}
	static void Mj2_DecompatSTCO(Mj2_TrackParams* pTrack)
	{
		for (unsigned int unChunk = 0; unChunk < pTrack->unNumChunks; unChunk++)
		{
			int nIntraChunkOffset = 0;
			int nSample2 = 0;
			for (int nSample = 0; nSample < pTrack->pChunk[unChunk].nNumSamples; nSample++)
			{
				pTrack->pSample[nSample2].unOffset = nIntraChunkOffset + pTrack->pChunk[unChunk].nOffset;
				nIntraChunkOffset += pTrack->pSample[nSample2].unSampleSize;
				nSample2++;
			}
		}
	}

	static bool Mj2_ReadSTTS(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_STTS != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected STTS Marker\n");
			return false;
		}

		if (0 != pStream->Read(1)) // Version
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in STTS box\n");
			return false;
		}

		if (0 != pStream->Read(3)) // Flags = 0
		{
			Event_Message(EVT_ERROR, "Error with flag in STTS box. Expected flag 0\n");
			return false;
		}

		pTrack->nNumTimeToSample = pStream->Read(4); // entry-count
		pTrack->pTimeToSample    = (Mj2_TimeToSample*)Malloc(pTrack->nNumTimeToSample * sizeof(Mj2_TimeToSample));

		for (int nIndex = 0; nIndex < pTrack->nNumTimeToSample; nIndex++)
		{
			pTrack->pTimeToSample[nIndex].nSampleCount = pStream->Read(4); // sample-count
			pTrack->pTimeToSample[nIndex].nSampleDelta = pStream->Read(4); // sample-delta
		}

		Mj2_DecompatTTS(pTrack);

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with STTS Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadSTSZ(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_STSZ != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected STSZ Marker\n");
			return false;
		}

		if (0 != pStream->Read(1)) // Version = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in STSZ box\n ");
			return false;
		}

		if (0 != pStream->Read(3)) // Flags = 0
		{
			Event_Message(EVT_ERROR, "Error with flag in STSZ box. Expected flag 0\n ");
			return false;
		}

		int nSampleSize = pStream->Read(4); // SampleSize

		if (0 != nSampleSize) // У всех самплов одинаковый размер
		{
			pTrack->unSameSampleSize = 1;

			for (unsigned int unIndex = 0; unIndex < pTrack->unNumSamples; unIndex++)
			{
				pTrack->pSample[unIndex].unSampleSize = nSampleSize;
			}
			pStream->Skip(4); // sample-count
		}
		else
		{
			pTrack->unSameSampleSize = 0;

			if (pTrack->unNumSamples != pStream->Read(4)) // sample-count
			{
				Event_Message(EVT_ERROR, "Error in STSZ box. Expected that sample-count is number of samples in track\n ");
				return false;
			}
			for (unsigned int unIndex = 0; unIndex < pTrack->unNumSamples; unIndex++)
			{
				pTrack->pSample[unIndex].unSampleSize = pStream->Read(4);	// entry-size
			}
		}

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with STSZ Box size\n ");
			return false;
		}

		return true;

	}

	static bool Mj2_ReadSTSC(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_STSC != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected STSC Marker\n");
			return false;
		}

		if (0 != pStream->Read(1)) // Version = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in STSC box\n");
			return false;
		}

		if (0 != pStream->Read(3)) // Flags = 0
		{
			Event_Message(EVT_ERROR, "Error with flag in STSC box. Expected flag 0\n");
			return false;
		}

		pTrack->nNumSamplesToChunk = pStream->Read(4); // entry-count
		pTrack->pSampleToChunk     = (Mj2_SampleToChunk*)Malloc(pTrack->nNumSamplesToChunk * sizeof(Mj2_SampleToChunk));

		for (int nIndex = 0; nIndex < pTrack->nNumSamplesToChunk; nIndex++)
		{
			pTrack->pSampleToChunk[nIndex].nFirstChunk             = pStream->Read(4); // first-chunk
			pTrack->pSampleToChunk[nIndex].nSamplesPerChunk        = pStream->Read(4); // samples-per-chunk
			pTrack->pSampleToChunk[nIndex].nSampleDescriptionIndex = pStream->Read(4); // sample-description-index
		}

		Mj2_DecompatSTSC(pTrack); // decompact sample to chunk box

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with STSC Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadSTCO(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		// TO DO: Сделать чтение 'co64'
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_STCO != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected STCO Marker\n");
			return false;
		}

		if (0 != pStream->Read(1)) // Version = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in STCO box\n");
			return false;
		}

		if (0 != pStream->Read(3)) // Flags = 0
		{
			Event_Message(EVT_ERROR, "Error with flag in STCO box. Expected flag 0\n");
			return false;
		}

		if (pStream->Read(4) != pTrack->unNumChunks)  // entry-count
		{
			Event_Message(EVT_ERROR, "Error in STCO box: expecting same amount of entry-count as chunks \n");
			return false;
		}
		else
		{
			for (unsigned int unIndex = 0; unIndex < pTrack->unNumChunks; unIndex++)
			{
				pTrack->pChunk[unIndex].nOffset = pStream->Read(4); // chunk-offset
			}
		}

		Mj2_DecompatSTCO(pTrack);

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with STCO Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadFIEL(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_FIEL != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected FIEL Marker\n");
			return false;
		}

		pTrack->unFieldCount = pStream->Read(1);
		pTrack->unFieldOrder = pStream->Read(1);

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with FIEL Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadJP2P(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_JP2P != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected JP2P Marker\n");
			return false;
		}

		if (0 != pStream->Read(1)) // Version = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in JP2P box\n");
			return false;
		}

		if (0 != pStream->Read(3)) // Flags = 0
		{
			Event_Message(EVT_ERROR, "Error with flag in JP2P box. Expected flag 0\n");
			return false;
		}

		pTrack->nNumBr = (oBox.nLength - 12) / 4;
		pTrack->pBr    = (unsigned int*)Malloc(pTrack->nNumBr * sizeof(unsigned int));

		for (int nIndex = 0; nIndex < pTrack->nNumBr; nIndex++)
		{
			pTrack->pBr[nIndex] = pStream->Read(4);
		}

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with JP2P Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadJP2X(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_JP2X != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected JP2X Marker\n");
			return false;
		}

		pTrack->unNumJp2x = (oBox.nLength - 8);
		pTrack->pJp2xData = (unsigned char*)Malloc(pTrack->unNumJp2x * sizeof(unsigned char));

		for (unsigned int unIndex = 0; unIndex < pTrack->unNumJp2x; unIndex++)
		{
			pTrack->pJp2xData[unIndex] = pStream->Read(1);
		}

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with JP2X Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadJSUB(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_JSUB != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected JSUB Marker\n");
			return false;
		}

		pTrack->unHsub = pStream->Read(1);
		pTrack->unVsub = pStream->Read(1);
		pTrack->unHoff = pStream->Read(1);
		pTrack->unVoff = pStream->Read(1);

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with JSUB Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadORFO(Mj2_TrackParams* pTrack, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_ORFO != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected ORFO Marker\n");
			return false;
		}

		pTrack->unOr_FieldCount = pStream->Read(1);
		pTrack->unOr_FieldOrder = pStream->Read(1);

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with ORFO Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadSMJ2(Mj2_TrackParams* pTrack, Image* pImage, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_MJ2 != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error in SMJ2 box: Expected MJ2 Marker\n");
			return false;
		}

		if (0 != pStream->Read(1)) // Version = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in MJP2 box\n");
			return false;
		}

		if (0 != pStream->Read(3)) // Flags = 0
		{
			Event_Message(EVT_ERROR, "Error with flag in MJP2 box. Expected flag 0\n");
			return false;
		}

		pStream->Skip(4);
		pStream->Skip(2);                               // Pre-defined
		pStream->Skip(2);                               // Reserved
		pStream->Skip(4);                               // Pre-defined
		pStream->Skip(4);                               // Pre-defined
		pStream->Skip(4);                               // Pre-defined

		pTrack->nWidth         = pStream->Read(2);      // Width
		pTrack->nHeight        = pStream->Read(2);      // Height
		pTrack->nHorResolution = pStream->Read(4);      // Horizontal resolution
		pTrack->nVerResolution = pStream->Read(4);	     // Vertical resolution

		pStream->Skip(4);                               // Reserved
		pStream->Skip(2);                               // Pre-defined = 1

		pTrack->anCompressorName[0] = pStream->Read(4); // Compressor Name
		pTrack->anCompressorName[1] = pStream->Read(4); //
		pTrack->anCompressorName[2] = pStream->Read(4); //
		pTrack->anCompressorName[3] = pStream->Read(4); //
		pTrack->anCompressorName[4] = pStream->Read(4); //
		pTrack->anCompressorName[5] = pStream->Read(4); //
		pTrack->anCompressorName[6] = pStream->Read(4); //
		pTrack->anCompressorName[7] = pStream->Read(4); //

		pTrack->nDepth              = pStream->Read(2); // Depth

		pStream->Skip(2);                               // Pre-defined = -1

		pTrack->unNumJp2x       = 0;
		pTrack->unFieldCount    = 1;
		pTrack->unFieldOrder    = 0;
		pTrack->unOr_FieldCount = 1;
		pTrack->unOr_FieldOrder = 0;

		if (!Jp2_ReadJP2H(&pTrack->oJp2, pStream))
		{
			Event_Message(EVT_ERROR, "Error reading JP2H Box\n");
			return false;
		}

		pTrack->oJp2.pComponents = (Jp2Component*)Malloc(pTrack->oJp2.nComponentsCount * sizeof(Jp2Component));
		pTrack->oJp2.pCompList   = (unsigned int*)Malloc(sizeof(unsigned int));

		pTrack->nNumBr    = 0;
		pTrack->unNumJp2x = 0;

		for (int nIndex = 0; pStream->Tell() - oBox.nInitPos < oBox.nLength; nIndex++)
		{
			Mj2_Box oBox2;
			Mj2_ReadBoxHeader(&oBox2, pStream);
			pStream->Seek(oBox2.nInitPos);

			switch (oBox2.nType)
			{
				case MJ2_FIEL:

				if (!Mj2_ReadFIEL(pTrack, pStream))
					return false;

				break;

				case MJ2_JP2P:

				if (!Mj2_ReadJP2P(pTrack, pStream))
					return false;

				break;

				case MJ2_JP2X:

				if (!Mj2_ReadJP2X(pTrack, pStream))
					return false;

				break;

				case MJ2_JSUB:

				if (!Mj2_ReadJSUB(pTrack, pStream))
					return false;

				break;

				case MJ2_ORFO:

				if (!Mj2_ReadORFO(pTrack, pStream))
					return false;

				break;

				default:

				Event_Message(EVT_ERROR, "Error with MJP2 Box size\n");
				return false;
			}
		}
		return true;
	}


	static bool Mj2_ReadSTSD(Mj2_TrackParams* pTrack, Image* pImage, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_STSD != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected STSD Marker\n");
			return false;
		}

		if (0 != pStream->Read(1)) // Version = 0
		{
			Event_Message(EVT_ERROR, "Error: Only Version 0 handled in STSD box\n");
			return false;
		}

		if (0 != pStream->Read(3)) // Flags = 0
		{
			Event_Message(EVT_ERROR, "Error with flag in STSD box. Expected flag 0\n");
			return false;
		}

		int nEntryCount = pStream->Read(4);

		if (0 == pTrack->nTrackType)
		{
			for (int nIndex = 0; nIndex < nEntryCount; nIndex++)
			{
				if (!Mj2_ReadSMJ2(pTrack, pImage, pStream))
					return false;
			}
		}
		else if (1 == pTrack->nTrackType)
		{
			// TO DO: Релизовать
			int nSkipLen = pStream->Read(4);
			pStream->Skip(nSkipLen - 4);
		}
		else if (2 == pTrack->nTrackType)
		{
			// TO DO: Реализовать
			int nSkipLen = pStream->Read(4);
			pStream->Skip(nSkipLen - 4);
		}


		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with STSD Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadSTBL(Mj2_TrackParams* pTrack, Image* pImage, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_STBL != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected STBL Marker\n");
			return false;
		}

		if (!Mj2_ReadSTSD(pTrack, pImage, pStream))
			return false;
		if (!Mj2_ReadSTTS(pTrack, pStream))
			return false;
		if (!Mj2_ReadSTSC(pTrack, pStream))
			return false;
		if (!Mj2_ReadSTSZ(pTrack, pStream))
			return false;
		if (!Mj2_ReadSTCO(pTrack, pStream))
			return false;

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with STBL Box size\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadMINF(Mj2_TrackParams* pTrack, Image* pImage, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);

		if (MJ2_MINF != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected MINF Marker\n");
			return false;
		}

		pStream->Skip(4);

		unsigned int unBoxType = pStream->Read(4);
		pStream->Skip(-8);

		if (MJ2_VMHD == unBoxType)
		{
			if (!Mj2_ReadVMHD(pTrack, pStream))
				return false;
		}
		else if (MJ2_SMHD == unBoxType)
		{
			if (!Mj2_ReadSMHD(pTrack, pStream))
				return false;
		}
		else if (MJ2_HMHD == unBoxType)
		{
			if (!Mj2_ReadHMHD(pTrack, pStream))
				return false;
		}
		else
		{
			Event_Message(EVT_ERROR, "Error in MINF box expected vmhd, smhd or hmhd\n");
			return false;
		}

		if (!Mj2_ReadDINF(pTrack, pStream))
			return false;

		if (!Mj2_ReadSTBL(pTrack, pImage, pStream))
			return false;

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with MINF Box size\n");
			return false;
		}
		return true;
	}
	static bool Mj2_ReadMDIA(Mj2_TrackParams* pTrack, Image* pImage, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);

		if (MJ2_MDIA != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected MDIA Marker\n");
			return false;
		}

		if (!Mj2_ReadMDHD(pTrack, pStream))
			return false;

		if (!Mj2_ReadHDLR(pTrack, pStream))
			return false;

		if (!Mj2_ReadMINF(pTrack, pImage, pStream))
			return false;

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with MDIA Box size\n");
			return false;
		}
		return true;
	}
	static bool Mj2_ReadTRAK(Mj2_TrackParams* pTrack, Image* pImage, CReader * pStream)
	{
		Mj2_Box oBox;
		Mj2_ReadBoxHeader(&oBox, pStream);
		if (MJ2_TRAK != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected TRAK Marker\n");
			return false;
		}

		if (!Mj2_ReadTKHD(pTrack, pStream))
			return false;

		if (!Mj2_ReadMDIA(pTrack, pImage, pStream))
			return false;

		if (pStream->Tell() - oBox.nInitPos != oBox.nLength)
		{
			Event_Message(EVT_ERROR, "Error with TRAK Box\n");
			return false;
		}
		return true;
	}

	static bool Mj2_ReadMOOV(Mj2_Movie* pMovie, CReader * pStream, Image*  pImage, Mj2_Box oBox)
	{
		if (MJ2_MOOV != oBox.nType)
		{
			Event_Message(EVT_ERROR, "Error: Expected FTYP Marker\n");
			return false;
		}

		if (!Mj2_ReadMVHD(pMovie, pStream))
			return false;

		pMovie->pTrack = (Mj2_TrackParams*)Malloc((pMovie->nNextTrackId - 1) * sizeof(Mj2_TrackParams));

		Mj2_Box oBox2;

		for (unsigned int unIndex = 0; pStream->Tell() - oBox.nInitPos < oBox.nLength; unIndex++)
		{
			Mj2_TrackParams* pTrack = &pMovie->pTrack[unIndex];
			pTrack->pCodecInfo = pMovie->pCodecInfo;

			Mj2_ReadBoxHeader(&oBox2, pStream);

			if (MJ2_TRAK == oBox2.nType)
			{
				pStream->Seek(oBox2.nInitPos);
				if (!Mj2_ReadTRAK(pTrack, pImage, pStream))
					return false;

				if (0 == pTrack->nTrackType)
					pMovie->nNumVtk++;
				else if (1 == pTrack->nTrackType)
					pMovie->nNumStk++;
				else if (2 == pTrack->nTrackType)
					pMovie->nNumHtk++;

			}
			else if (MJ2_MVEX == oBox2.nType)
			{
				pStream->Seek(oBox2.nInitPos);
				pStream->Skip(oBox2.nLength);
				unIndex--;
			}
			else
			{
				Event_Message(EVT_ERROR, "Error with MOOV Box: Expected TRAK or MVEX box\n");
				return false;
			}
		}
		return true;
	}
	static bool Mj2_ReadStruct(Mj2_Movie* pMovie, CReader * pStream, Image **ppImage)
	{
		if (!Mj2_ReadJP(pStream))
			return false;

		if (!Mj2_ReadFTYP(pMovie, pStream))
			return false;

		Mj2_Box oBox;
		oBox.nType = 0;
		Mj2_ReadBoxHeader(&oBox, pStream);

		while (MJ2_MOOV != oBox.nType)
		{
			switch (oBox.nType)
			{
				case MJ2_MDAT:

				// TO DO: Связать данные в MDAT с данными MOOV
				if (!Mj2_ReadMDAT(pMovie, pStream, ppImage, oBox))
					return false;

				break;

				case MJ2_MOOF:
				case MJ2_FREE:
				case MJ2_SKIP:

				pStream->Skip(oBox.nLength);
				if (pStream->GetLeftSize() < 0)
					return false;

				break;

				default:

				Event_Message(EVT_ERROR, "Unknown box in MJ2 stream\n");
				pStream->Skip(oBox.nLength);
				if (pStream->GetLeftSize() < 0)
					return false;
				break;
			}

			if (!Mj2_ReadBoxHeader(&oBox, pStream))
				return false;
		}

		Image oImage;
		if (!Mj2_ReadMOOV(pMovie, pStream, &oImage, oBox))
			return false;

		return true;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Декодирование потока Mj2
	//-------------------------------------------------------------------------------------------------------------------------------
	void       Mj2_DestroyDecompress(Mj2_Movie* pMovie)
	{
		if (pMovie)
		{
			Mj2_TrackParams* pTrack = NULL;

			if (pMovie->pCodecInfo->pJ2k)
				J2k_DestroyCompress(pMovie->pJ2k);

			//if ( pMovie->nCompListLength != 0 )
			Free(pMovie->pCompList);

			for (int nIndex = 0; nIndex < pMovie->nNumVtk + pMovie->nNumStk + pMovie->nNumHtk; nIndex++)
			{
				pTrack = &pMovie->pTrack[nIndex];

				//if ( pTrack->nNameSize != 0 )
				Free(pTrack->sName);
				if (pTrack->nTrackType == 0)
				{
					Free(pTrack->oJp2.pComponents);
					Free(pTrack->oJp2.pCompList);
					Free(pTrack->pJp2xData);
				}
				//if ( pTrack->nNumUrl != 0 )
				Free(pTrack->pUrl);
				//if ( pTrack->nNumUrn != 0 )
				Free(pTrack->pUrn);
				//if ( pTrack->nNumBr != 0 )
				Free(pTrack->pBr);
				//if ( pTrack->nNumTimeToSample != 0 )
				Free(pTrack->pTimeToSample);
				//if ( pTrack->unNumChunks != 0 )
				Free(pTrack->pChunk);
				//if ( pTrack->nNumSamplesToChunk != 0 )
				Free(pTrack->pSampleToChunk);
				//if ( pTrack->unNumSamples != 0 )
				Free(pTrack->pSample);

				Free(pTrack);
			}

			Free(pMovie);
		}
	}


	Mj2_Movie* Mj2_CreateDecompress(PCommon pCodecInfo)
	{
		Mj2_Movie* pMj2 = (Mj2_Movie*)Malloc(sizeof(Mj2_Movie));
		if (pMj2)
		{
			pMj2->pCodecInfo = (PCommon)pCodecInfo;
			pMj2->pJ2k = J2k_CreateDecompress(pCodecInfo);

			if (NULL == pMj2->pJ2k)
			{
				pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
				Mj2_DestroyDecompress(pMj2);
				return NULL;
			}
		}
		else
		{
			pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
		}

		return pMj2;
	}

	void       Mj2_SetupDecoder(Mj2_Movie* pMovie, DecoderParams *pParameters)
	{
		pMovie->nNumVtk = 0;
		pMovie->nNumStk = 0;
		pMovie->nNumHtk = 0;

		J2k_SetupDecoder((J2kCodestream*)pMovie->pCodecInfo->pJ2k, pParameters);
	}

	Image*     Mj2_Decode(Mj2_Movie* pMovie, CReader * pStream)
	{
		if (!pMovie || !pStream)
		{
			return NULL;
		}

		PCommon pCodecInfo = pMovie->pCodecInfo;

		Image *pImage = NULL;
		// Декодируем JP2
		if (!Mj2_ReadStruct(pMovie, pStream, &pImage))
		{
			Event_Message(EVT_ERROR, "Failed to decode jp2 structure\n");
			return NULL;
		}

		return pImage;
	}
}