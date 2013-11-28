#pragma once

#include <string>

#include "IRunItem.h"
#include "PICFAndOfficeArtData.h"
#include "BinaryStorage.h"
#include "OfficeArt/BlipFactory.h"
#include "OfficeArt/OfficeArtFOPT.h"
#include "OfficeArt/OfficeArtClientAnchor.h"

#define RATIO	1000	// TODO : recalculate


namespace AVSDocFileFormat
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
					int dataStreamOffset	=	BinaryStorageSingleton::Instance()->PushData( (const byte*)m_oBinPictureInfo, m_oBinPictureInfo.Size());

					m_arProperties.push_back (Prl((short)DocFileFormat::sprmCPicLocation,	(byte*)(&dataStreamOffset)));
					m_arProperties.push_back (Prl((short)DocFileFormat::sprmCFSpec,			(byte*)(&CFSpec)));

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

		virtual operator byte*() const
		{
			return (byte*)(m_oBinPictureInfo);
		}

		virtual operator const byte*() const
		{
			return (const byte*)(m_oBinPictureInfo);
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

		inline BOOL IsValid()
		{
			return m_bOK;
		}

	private:
		
		static const byte CFSpec = 1;

		BOOL					m_bOK;
	
		PICFAndOfficeArtData	m_oBinPictureInfo;

		std::wstring			m_sTextType;
		std::list<Prl>			m_arProperties;
	};
}