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

#include <string>

#include "IRunItem.h"
#include "PICFAndOfficeArtData.h"
#include "BinaryStorage.h"
#include "OfficeArt/BlipFactory.h"
#include "OfficeArt/OfficeArtFOPT.h"
#include "OfficeArt/OfficeArtClientAnchor.h"

#define RATIO	1000	// TODO : recalculate

#include "../Common/TextMark.h"

namespace ASCDocFileFormat
{
	class Picture : public IRunItem, public IOperand
	{
	public:

		Picture () : m_oBinPictureInfo(), m_sTextType (std::wstring (&TextMark::Picture)), m_bOK(FALSE)
		{

		}

		Picture (std::wstring strFileName, 
			short iWidth,
			short iHeight,
			unsigned short iRatioX	=	RATIO, 
			unsigned short iRatioY	=	RATIO,
			Brc80 brcTop80			=	Brc80(2, 0x00, 0x00, 0, false, false),
			Brc80 brcLeft80			=	Brc80(2, 0x00, 0x00, 0, false, false),
			Brc80 brcBottom80		=	Brc80(2, 0x00, 0x00, 0, false, false),
			Brc80 brcRight80		=	Brc80(2, 0x00, 0x00, 0, false, false)) : m_oBinPictureInfo(), m_sTextType (std::wstring (&TextMark::Picture)), m_bOK(FALSE)
		{
			OfficeArt::BlipFactory oBlipFactory (strFileName);

			OfficeArt::OfficeArtBlip* blip = oBlipFactory.GetOfficeArtBlip();
			if (blip)
			{
				OfficeArt::OfficeArtSpContainer shape;
				OfficeArt::OfficeArtFSP shapeProp (OfficeArt::Enumerations::msosptPictureFrame);
				shape.PushBack (shapeProp);

				OfficeArt::OfficeArtRGFOPTE oTable;
				OfficeArt::OfficeArtFOPTE oEntry (OfficeArt::OfficeArtFOPTEOPID (OfficeArt::Enumerations::pib, true, false), 1);
				oTable.PushComplexProperty (OfficeArt::ComplexOfficeArtProperty (oEntry, NULL));

				OfficeArt::OfficeArtFOPT fopt (oTable);
				shape.PushBack (fopt);

				OfficeArt::OfficeArtInlineSpContainer oPicture (shape);
				OfficeArt::OfficeArtFBSE oBlipStoreEntry (TRUE, oBlipFactory.GetBlipType(), oBlipFactory.GetBlipType(), blip->Size() );
				oPicture.PushBack(oBlipStoreEntry);
				oPicture.PushBack(*blip);

				PICMID oBorders (iWidth, iHeight, iRatioX, iRatioY, brcTop80, brcLeft80, brcBottom80, brcRight80);
				PICF oPictureInfo (oPicture.Size(), oBorders);

				m_oBinPictureInfo = PICFAndOfficeArtData (oPictureInfo, oPicture);

				if (BinaryStorageSingleton::Instance())
				{
					int dataStreamOffset	=	BinaryStorageSingleton::Instance()->PushData( (const unsigned char*)m_oBinPictureInfo, m_oBinPictureInfo.Size());

					m_arProperties.push_back (Prl((short)DocFileFormat::sprmCPicLocation,	(unsigned char*)(&dataStreamOffset)));
					m_arProperties.push_back (Prl((short)DocFileFormat::sprmCFSpec,			(unsigned char*)(&CFSpec)));

					m_bOK					=	TRUE;
				}

				RELEASEOBJECT(blip);
			}
		}

		Picture (const Picture& oPicture) : m_oBinPictureInfo(oPicture.m_oBinPictureInfo), m_sTextType(oPicture.m_sTextType), m_arProperties(oPicture.m_arProperties), m_bOK(oPicture.m_bOK)
		{

		}

		virtual ~Picture()
		{
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(m_oBinPictureInfo);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)(m_oBinPictureInfo);
		}

		virtual unsigned int Size() const
		{
			return m_oBinPictureInfo.Size();
		}

		virtual IVirtualConstructor* New() const
		{
			return new Picture();
		}

		virtual IVirtualConstructor* Clone() const
		{
			return new Picture(*this);
		}

		virtual unsigned long GetTextSize() const
		{
			return m_sTextType.size();
		}

		virtual std::wstring GetAllText() const
		{
			return m_sTextType;
		}

		virtual std::vector<Prl> GetRunProperties() const
		{
			std::vector<Prl> prls;

			for (std::list<Prl>::const_iterator iter = m_arProperties.begin(); iter != m_arProperties.end(); ++iter)
				prls.push_back(*iter);

			return prls;
		}

		virtual unsigned int PrlSize() const
		{
			return (unsigned int)m_arProperties.size();
		}

		inline bool IsValid()
		{
			return m_bOK;
		}

	private:
		
		static const unsigned char CFSpec = 1;

		bool					m_bOK;
	
		PICFAndOfficeArtData	m_oBinPictureInfo;

		std::wstring			m_sTextType;
		std::list<Prl>			m_arProperties;
	};
}