
#pragma once

#include <XmlUtils.h>


#pragma pack(push, 1)
struct TRational
{
	int Fraction;
	int Denominator;
};
struct TRationalU
{
	unsigned int Fraction;
	unsigned int Denominator;
};
#pragma pack(pop)

		class CMetaDataSaver
		{

		public:

			CMetaDataSaver()
			{
				m_bBitmap = FALSE;

				Clear();
			}

			~CMetaDataSaver()
			{
				Clear();
			}


			BOOL SaveToBitmap(Bitmap* bitmap)
			{
				SetPropertyStr(bitmap, PropertyTagImageTitle,       m_sTitle);
				SetPropertyStr(bitmap, PropertyTagArtist,           m_sAuthor);
				SetPropertyStr(bitmap, PropertyTagImageDescription, m_sDescription);
				
				CString sDateTime = _T("");
				CString sTemp = _T("");
				char sValue[33];
				_itoa(m_oDateTimeCreation.nYear, sValue, 10);

				sTemp = sValue;
				while ( sTemp.GetLength() < 4 )
					sTemp = _T("0") + sTemp;

				sDateTime += sTemp + _T(":");

				_itoa(m_oDateTimeCreation.nMonth, sValue, 10);

				sTemp = sValue;
				while ( sTemp.GetLength() < 2 )
					sTemp = _T("0") + sTemp;

				sDateTime += sTemp + _T(":");

				_itoa(m_oDateTimeCreation.nDay, sValue, 10);

				sTemp = sValue;
				while ( sTemp.GetLength() < 2 )
					sTemp = _T("0") + sTemp;

				sDateTime += sTemp + _T(" ");

				_itoa(m_oDateTimeCreation.nHour, sValue, 10);

				sTemp = sValue;
				while ( sTemp.GetLength() < 2 )
					sTemp = _T("0") + sTemp;

				sDateTime += sTemp + _T(":");

				_itoa(m_oDateTimeCreation.nMinute, sValue, 10);

				sTemp = sValue;
				while ( sTemp.GetLength() < 2 )
					sTemp = _T("0") + sTemp;

				sDateTime += sTemp + _T(":");

				_itoa(m_oDateTimeCreation.nSecond, sValue, 10);

				sTemp = sValue;
				while ( sTemp.GetLength() < 2 )
					sTemp = _T("0") + sTemp;

				sDateTime += sTemp;

				SetPropertyStr(bitmap, PropertyTagDateTime,         sDateTime);
				SetPropertyStr(bitmap, PropertyTagExifUserComment,	m_sComment);
				SetPropertyStr(bitmap, PropertyTagCopyright,        m_sCopyright);
				SetPropertyStr(bitmap, PropertyTagHostComputer,     m_sHostComputer);
				SetPropertyStr(bitmap, PropertyTagEquipMake,        m_sEquipmentType);
				SetPropertyStr(bitmap, PropertyTagEquipModel,       m_sEquipmentModel);
				SetPropertyStr(bitmap, PropertyTagSoftwareUsed,     m_sSoftwareID);
				SetPropertyStr(bitmap, PropertyTagDocumentName,     m_sDocumentName);
				SetPropertyStr(bitmap, PropertyTagPageName,         m_sPageName);
				SetPropertyStr(bitmap, PropertyTagPageName,         m_sDisclaimer);

				short nPageNumber = BYTE(m_nPagesCount) << 8 | BYTE(m_nPageNumber);
				SetPropertyShort(bitmap, PropertyTagPageNumber, nPageNumber);
				
				//SetPropertyStr(bitmap, PropertyTagPageName,         m_sJobName);
				//SetPropertyUndefined(bitmap,PropertyTagExifVer,         m_sVersionLetter);
				//SetPropertyShort(bitmap, PropertyTagPageName,       m_nBitDepth);
				SetPropertyShort(bitmap, PropertyTagExifISOSpeed,   m_nISOSpeed);
				SetPropertyShort(bitmap, PropertyTagExifColorSpace, m_nColorSpace);

				SetPropertyShort(bitmap, PropertyTagOrientation,    m_nOrientation);
				SetPropertyShort(bitmap, PropertyTagExifExposureProg,m_nExposureProgram);
				SetPropertyShort(bitmap, PropertyTagExifFlash,       m_nFlashMode);
				SetPropertyShort(bitmap, PropertyTagExifMeteringMode, m_nMeteringMode);

				SetPropertyRational(bitmap,PropertyTagExifFNumber,m_oFNumber);
				SetPropertyRational(bitmap,PropertyTagExifExposureTime,m_oExposureTime);
				SetPropertyRational(bitmap,PropertyTagExifAperture,m_oLensAperture);
				SetPropertyRational(bitmap,PropertyTagExifMaxAperture,m_oMaxApertureValue);
				SetPropertySRational(bitmap,PropertyTagExifShutterSpeed,m_oShutterSpeed);
				SetPropertySRational(bitmap,PropertyTagExifExposureBias,m_oExposureCompensation);
				SetPropertySRational(bitmap,PropertyTagExifBrightness,m_oBrightness);
				SetPropertyRational(bitmap,PropertyTagExifFocalLength,m_oFocalLength);

				//exif 2.2 
				SetPropertyShort(bitmap,PropertyTagExifExposureMode,m_nExposureMode );
				SetPropertyShort(bitmap,PropertyTagExifWhiteBalance,m_nWhiteBalance);
				SetPropertyShort(bitmap,PropertyTagExifSensingMethod , m_nSensingMethod);
				SetPropertyShort(bitmap,PropertyTagExifFocalLengthIn35mmFilm,m_nFocalLengthIn35mmFilm);

				if (m_sGpsVersion[0]>0 || m_sGpsVersion[1] >0 || m_sGpsVersion[2]>0)
				{
					SetPropertyByte(bitmap, PropertyTagGpsVer,			m_sGpsVersion, 4);
				}
				if (m_sGpsLatitudeRef.GetLength()>0)
				{
					SetPropertyStr(bitmap, PropertyTagGpsLatitudeRef,	m_sGpsLatitudeRef);
					SetPropertyRational(bitmap, PropertyTagGpsLatitude,	m_sGpsLatitude,3);
				}
				if (m_sGpsLongitudeRef.GetLength()>0)
				{
					SetPropertyStr(bitmap, PropertyTagGpsLongitudeRef,	m_sGpsLongitudeRef);
					SetPropertyRational(bitmap, PropertyTagGpsLongitude,m_sGpsLongitude,3);
				}
				SetPropertyByte(bitmap, PropertyTagGpsAltitudeRef,	&m_sGpsAltitudeRef,1);
				SetPropertyRational(bitmap, PropertyTagGpsAltitude,	m_sGpsAltitude,3);
				SetPropertyRational(bitmap, PropertyTagGpsGpsTime,	m_sGpsTimeStamp,3);
				SetPropertyStr(bitmap, PropertyTagGpsDate,			m_sGpsDateStamp);

				if (bitmap->GetLastStatus() != Ok)
					return FALSE;

				//дополнительные данные

				return TRUE;
			}
			void Clear()
			{
				m_sAuthor      = _T("");
				m_sTitle       = _T("");
				m_sDescription = _T("");
				m_oDateTimeCreation.nYear    = 0;
				m_oDateTimeCreation.nMonth   = 0;
				m_oDateTimeCreation.nDay     = 0;
				m_oDateTimeCreation.nHour    = 0;
				m_oDateTimeCreation.nMinute  = 0;
				m_oDateTimeCreation.nSecond  = 0;
				m_oDateTimeCreation.sSummary = _T("");
				m_sCopyright      = _T("");
				m_sDisclaimer     = _T("");
				m_sComment        = _T("");
				m_sEquipmentType  = _T("");
				m_sEquipmentModel = _T("");
				m_oJobTime.nHour   = 0;
				m_oJobTime.nMinute = 0;
				m_oJobTime.nSecond = 0;
				m_sJobName       = _T("");
				m_sSoftwareID    = _T("");
				m_nVersionNumber = 0;
				m_sVersionLetter = _T("");
				m_sHostComputer  = _T("");
				m_sWarning       = _T("");
				m_sDocumentName  = _T("");
				m_sPageName      = _T("");
				m_nPageNumber    = 0;
				m_nPagesCount    = 0;

				// дополнительные данные для photoeditor

				m_nExposureProgram                  = 0;
				m_oExposureTime.Fraction            = 0;
				m_oExposureTime.Denominator         = 0;
				m_oExposureCompensation.Fraction    = 0;
				m_oExposureCompensation.Denominator = 0;
				m_oShutterSpeed.Fraction            = 0;
				m_oShutterSpeed.Denominator         = 0;
				m_oLensAperture.Fraction            = 0;
				m_oLensAperture.Denominator         = 0;
				m_oFocalLength.Fraction             = 0;
				m_oFocalLength.Denominator          = 0;
				m_oFNumber.Fraction                 = 0;
				m_oFNumber.Denominator              = 0;
				m_nISOSpeed                         = 0;
				m_nMeteringMode                     = 0;
				m_nFlashMode                        = 0;
				m_nColorSpace                       = 0;
		
				m_oMaxApertureValue.Fraction        = 0;
				m_oMaxApertureValue.Denominator     = 0;
				m_oBrightness.Fraction				= 0;
				m_oBrightness.Denominator			= 0;
				
				m_nSensingMethod                    = 0;
				m_nWhiteBalance                     = 0;
				m_nExposureMode                     = 0;
				m_nOrientation                      = 1;
				m_nFocalLengthIn35mmFilm			= 0;

				m_nOrientation                      = 1;
				
				m_nBitDepth                         = 1;

				m_sGpsLongitudeRef	= _T("");
				m_sGpsAltitudeRef	= 0;
				m_sGpsLatitudeRef	= _T("");
				m_sGpsDateStamp		= _T("");

				ZeroMemory(m_sGpsLatitude,	3*sizeof(TRationalU));
				ZeroMemory(m_sGpsLongitude,	3*sizeof(TRationalU));
				ZeroMemory(m_sGpsAltitude,	3*sizeof(TRationalU));
				ZeroMemory(m_sGpsTimeStamp,	3*sizeof(TRationalU));
				ZeroMemory(m_sGpsVersion,	4*sizeof(BYTE));


				m_sMetaDataXML = _T("");
			}
			
			CString GetXML()
			{
				MakeXML();
				return m_sMetaDataXML;
			}

			void SetXML(CString sXML)
			{
				ParseXML(sXML);
				return;
			}

			//short GetOrientation()
			//{
			//	return m_nOrientation;
			//}
		protected:

			//common functions
			BYTE  IntFromBytes1( const BYTE* pBytes)
			{
				return pBytes[0];
			}
			int  IntFromBytes2( const BYTE* pBytes, int nType)
			{
				int nValue = 0;

				if ( 0 == nType )
				{
					nValue = pBytes[0];
					nValue = ( nValue << 8 ) | pBytes[1];
				}
				else
				{
					nValue = pBytes[1];
					nValue = ( nValue << 8 ) | pBytes[0];
				}
				return nValue;
			}

			int  IntFromBytes4( const BYTE* pBytes, int nType)
			{
				int nValue = 0;

				if ( 0 == nType )
				{
					nValue = pBytes[0];
					nValue = ( nValue << 8 ) | pBytes[1];
					nValue = ( nValue << 8 ) | pBytes[2];
					nValue = ( nValue << 8 ) | pBytes[3];
				}
				else
				{
					nValue = pBytes[3];
					nValue = ( nValue << 8 ) | pBytes[2];
					nValue = ( nValue << 8 ) | pBytes[1];
					nValue = ( nValue << 8 ) | pBytes[0];
				}
				return nValue;
			}
			void IntToBytes4( int nNumber, BYTE* pBytes )
			{
				pBytes[3] = (BYTE)nNumber; nNumber >>= 8;
				pBytes[2] = (BYTE)nNumber; nNumber >>= 8;
				pBytes[1] = (BYTE)nNumber; nNumber >>= 8;
				pBytes[0] = (BYTE)nNumber;
			}


			CString ChunkType(const BYTE* pCurPoint)
			{
				CString sType = _T("");
				sType += pCurPoint[0];
				sType += pCurPoint[1];
				sType += pCurPoint[2];
				sType += pCurPoint[3];
				return sType;
			}

			void BytesFromString(CString sSource, BYTE *pResult, int nCount)
			{
				int nLen = min( (int)sSource.GetLength(), nCount);
				for (int nIndex = 0; nIndex < nLen; nIndex++ )
					pResult[nIndex] = sSource[nIndex];
			}

			// Bitmap functions
			CString GetPropertyStr(Bitmap*& pImage, PROPID nId)
			{
				// reset value
				CString sValue = _T("");

				// retrieve property size
				UINT nSize = pImage->GetPropertyItemSize(nId);

				// check for valid size
				if (nSize > 0)
				{
					// allocate memory for tag
					PropertyItem* pProp = (PropertyItem*)(malloc(nSize));
					if ( NULL == pProp )
						return sValue;

					// read tag to buffer
					pImage->GetPropertyItem(nId, nSize, pProp);

					// copy data to buffer
					if ( NULL != pProp->value )
						sValue = (const char*)pProp->value;
					
					// release buffer
					free(pProp);
				}
				return sValue;
			}

			int GetPropertyInt(Bitmap*& pImage, PROPID nId)
			{
				// reset value
				int nValue = 0;

				// retrieve property size
				UINT nSize = pImage->GetPropertyItemSize(nId);

				// check for valid size
				if (nSize > 0)
				{
					// allocate memory for tag
					PropertyItem* pProp = (PropertyItem*)(malloc(nSize));
					if ( NULL == pProp )
						return nValue;

					// read tag to buffer
					pImage->GetPropertyItem(nId, nSize, pProp);

					// copy data to buffer
					if ( NULL != pProp->value )
						nValue = *(int*)pProp->value;
					
					// release buffer
					free(pProp);
				}
				return nValue;
			}
			void GetPropertyByte(Bitmap*& pImage, PROPID nId, BYTE *Value, int size)
			{
				// retrieve property size
				UINT nSize = pImage->GetPropertyItemSize(nId);

				// check for valid size
				if (nSize > 0)
				{
					// allocate memory for tag
					PropertyItem* pProp = (PropertyItem*)(malloc(nSize));
					if ( NULL == pProp )
						return;

					// read tag to buffer
					pImage->GetPropertyItem(nId, nSize, pProp);

					// copy data to buffer
					if ( NULL != pProp->value )
						memcpy(Value,pProp->value,min (size,nSize));
					
					// release buffer
					free(pProp);
				}
			}

			BYTE GetPropertyByte(Bitmap*& pImage, PROPID nId)
			{
				// reset value
				int nValue = 0;

				// retrieve property size
				UINT nSize = pImage->GetPropertyItemSize(nId);

				// check for valid size
				if (nSize > 0)
				{
					// allocate memory for tag
					PropertyItem* pProp = (PropertyItem*)(malloc(nSize));
					if ( NULL == pProp )
						return nValue;

					// read tag to buffer
					pImage->GetPropertyItem(nId, nSize, pProp);

					// copy data to buffer
					if ( NULL != pProp->value )
						nValue = *(int*)pProp->value;
					
					// release buffer
					free(pProp);
				}
				return nValue;
			}
			void SetPropertyByte(Bitmap*& pImage, PROPID nId, BYTE* nValue, int size )
			{
				// create new property item
				PropertyItem* pProp = new PropertyItem;
				if ( NULL == pProp )
					return;

				// compose property item
				pProp->id = nId;
				pProp->length = size * sizeof(BYTE);
				pProp->type = PropertyTagTypeByte; 
				pProp->value = nValue;

				// sset property item
				pImage->SetPropertyItem(pProp);

				// delete property item
				delete pProp;
			}
			short GetPropertyShort(Bitmap*& pImage, PROPID nId)
			{
				// reset value
				short nValue = 0;

				// retrieve property size
				UINT nSize = pImage->GetPropertyItemSize(nId);

				// check for valid size
				if (nSize > 0)
				{
					// allocate memory for tag
					PropertyItem* pProp = (PropertyItem*)(malloc(nSize));
					if ( NULL == pProp )
						return nValue;

					// read tag to buffer
					pImage->GetPropertyItem(nId, nSize, pProp);

					// copy data to buffer
					if ( NULL != pProp->value )
						nValue = *(short*)pProp->value;
					
					// release buffer
					free(pProp);
				}
				return nValue;
			}


			double GetPropertyDouble(Bitmap*& pImage, PROPID nId)
			{
				// reset value
				double dValue = 0;

				// retrieve property size
				UINT nSize = pImage->GetPropertyItemSize(nId);

				// check for valid size
				if (nSize > 0)
				{
					// allocate memory for tag
					PropertyItem* pProp = (PropertyItem*)(malloc(nSize));
					if ( NULL == pProp )
						return dValue;

					// read tag to buffer
					pImage->GetPropertyItem(nId, nSize, pProp);

					// copy data to buffer
					if ( NULL != pProp->value )
						dValue = *(double*)pProp->value;
					
					// release buffer
					free(pProp);
				}
				return dValue;
			}

			void GetPropertyRationalU(Bitmap*& pImage, PROPID nId, TRationalU *pResult,int sizeResult = 1)
			{
				if (sizeResult <1)return;

				//rational - пара двух чисел (числитель и знаменатель)
				(*pResult).Denominator = 1;
				(*pResult).Fraction    = 0;
				// retrieve property size
				UINT nSize = pImage->GetPropertyItemSize(nId);

				// check for valid size
				if (nSize > 0)
				{
					// allocate memory for tag
					PropertyItem* pProp = (PropertyItem*)(malloc(nSize));
					if ( NULL == pProp )
						return;

					// read tag to buffer
					pImage->GetPropertyItem(nId, nSize, pProp);

					// copy data to buffer
					if ( NULL != pProp->value )
					{
						if (sizeResult >1)
						{
							memcpy(pResult,pProp->value,min(nSize-8,sizeResult * 2 * 4));
						}
						else
						{
						*pResult = *(TRationalU*)pProp->value;
						}
					}
					
					// release buffer
					free(pProp);
				}
			}



			void GetPropertyRational(Bitmap*& pImage, PROPID nId, TRational *pResult)
			{
				//rational - пара двух чисел (числитель и знаменатель)
				(*pResult).Denominator = 1;
				(*pResult).Fraction    = 0;
				// retrieve property size
				UINT nSize = pImage->GetPropertyItemSize(nId);

				// check for valid size
				if (nSize > 0)
				{
					// allocate memory for tag
					PropertyItem* pProp = (PropertyItem*)(malloc(nSize));
					if ( NULL == pProp )
						return;

					// read tag to buffer
					pImage->GetPropertyItem(nId, nSize, pProp);

					// copy data to buffer
					if ( NULL != pProp->value )
						*pResult = *(TRational*)pProp->value;
					
					// release buffer
					free(pProp);
				}
			}


			void SetPropertyStr(Bitmap*& pImage, PROPID nId, CString& sValue)
			{
				// check for valid string
				if (sValue.GetLength() < 1)
					return;

				// create new property item
				PropertyItem* pProp = new PropertyItem;
				if ( NULL == pProp )
					return;

				// compose property item
				pProp->id = nId;
				pProp->length = (nId == PropertyTagDateTime) ? 20 : sValue.GetLength() + 1;
				pProp->type = PropertyTagTypeASCII; 

				CT2A convert(sValue);
				pProp->value =convert;

				// sset property item
				pImage->SetPropertyItem(pProp);

				// delete property item
				delete pProp;
			}
			void SetPropertyShort(Bitmap*& pImage, PROPID nId, short& nValue)
			{
				// create new property item
				PropertyItem* pProp = new PropertyItem;
				if ( NULL == pProp )
					return;

				// compose property item
				pProp->id = nId;
				pProp->length = sizeof(short);
				pProp->type = PropertyTagTypeShort; 
				pProp->value = (short*)(&nValue);

				// sset property item
				pImage->SetPropertyItem(pProp);

				// delete property item
				delete pProp;
			}
			void SetPropertyRational(Bitmap*& pImage, PROPID nId, TRationalU& nValue)
			{//unsignes
				// create new property item
				PropertyItem* pProp = new PropertyItem;
				if ( NULL == pProp )
					return;

				// compose property item
				pProp->id = nId;
				pProp->length = 2 * sizeof(long);
				pProp->type = PropertyTagTypeRational; 
				pProp->value = (unsigned long*)(&nValue);

				// sset property item
				pImage->SetPropertyItem(pProp);

				// delete property item
				delete pProp;
			}
			void SetPropertyRational(Bitmap*& pImage, PROPID nId, TRationalU* nValue,int size=1)
			{//unsignes
				// create new property item
				PropertyItem* pProp = new PropertyItem;
				if ( NULL == pProp )
					return;

				// compose property item
				pProp->id = nId;
				pProp->length = 2 * sizeof(long) * size;
				pProp->type = PropertyTagTypeRational; 
				pProp->value = (unsigned long*)(nValue);

				// sset property item
				pImage->SetPropertyItem(pProp);

				// delete property item
				delete pProp;
			}
			void SetPropertySRational(Bitmap*& pImage, PROPID nId, TRational& nValue)
			{//signed
				// create new property item
				PropertyItem* pProp = new PropertyItem;
				if ( NULL == pProp )
					return;

				// compose property item
				pProp->id = nId;
				pProp->length = 2 * sizeof(long);
				pProp->type = PropertyTagTypeSRational; 
				pProp->value = (long*)(&nValue);

				// sset property item
				pImage->SetPropertyItem(pProp);

				// delete property item
				delete pProp;
			}
			CString RationalUToString(TRationalU oRat)
			{
				CString sResult = _T("");
				char str[32];
				_itoa(oRat.Fraction, str, 10);
				sResult += str;
				if ( 0 != oRat.Denominator )
				{
					sResult += _T("/");
					_itoa(oRat.Denominator, str, 10);
					sResult += str;
				}
				return sResult;
			}
			CString RationalUToString(TRationalU *oRat,int size =1)
			{
				CString sResult = _T("");
				for (long i=0;i<size;i++)
				{
					char str[32];
					_itoa(oRat[i].Fraction, str, 10);
					sResult += str;
					if ( 0 != oRat[i].Denominator )
					{
						sResult += _T("/");
						_itoa(oRat[i].Denominator, str, 10);
						sResult += str;
					}
					sResult += _T(" ");
				}
				return sResult;
			}
			CString RationalToString(TRational oRat)
			{
				CString sResult = _T("");
				char str[32];
				_itoa(oRat.Fraction, str, 10);
				sResult += str;
				if ( 0 != oRat.Denominator )
				{
					sResult += _T("/");
					_itoa(oRat.Denominator, str, 10);
					sResult += str;
				}
				return sResult;
			}
			BOOL LoadBitmapMetaData(Bitmap*& bitmap)
			{
				// retrieve info properties
				m_sTitle                     = GetPropertyStr(bitmap, PropertyTagImageTitle); 
				m_sAuthor                    = GetPropertyStr(bitmap, PropertyTagArtist);
				m_sDescription               = GetPropertyStr(bitmap, PropertyTagImageDescription);
				
				CString sDateTime            = GetPropertyStr(bitmap, PropertyTagDateTime);
				if ( 19 == sDateTime.GetLength() )
				{
					TCHAR *pDateTime = sDateTime.GetBuffer();
					m_oDateTimeCreation.nYear   = _ttoi( pDateTime );
					m_oDateTimeCreation.nMonth  = _ttoi( pDateTime + 5 );
					m_oDateTimeCreation.nDay    = _ttoi( pDateTime + 8 );
					m_oDateTimeCreation.nHour   = _ttoi( pDateTime + 11 );
					m_oDateTimeCreation.nMinute = _ttoi( pDateTime + 14 );
					m_oDateTimeCreation.nSecond = _ttoi( pDateTime + 17 );

				}
				else
					m_oDateTimeCreation.sSummary = sDateTime;
			
				m_sCopyright                 = GetPropertyStr(bitmap, PropertyTagCopyright);
				m_sHostComputer              = GetPropertyStr(bitmap, PropertyTagHostComputer);
				m_sEquipmentType             = GetPropertyStr(bitmap, PropertyTagEquipMake);
				m_sEquipmentModel            = GetPropertyStr(bitmap, PropertyTagEquipModel);
				m_sSoftwareID                = GetPropertyStr(bitmap, PropertyTagSoftwareUsed);
				m_sDocumentName              = GetPropertyStr(bitmap, PropertyTagDocumentName);
				m_sPageName                  = GetPropertyStr(bitmap, PropertyTagPageName);
				short nPageNumber = GetPropertyInt(bitmap, PropertyTagPageNumber);

				m_nPageNumber = (BYTE)nPageNumber; 
				nPageNumber   = nPageNumber >> 8;
				m_nPagesCount = (BYTE)nPageNumber;


				m_nExposureProgram      = GetPropertyInt(bitmap, PropertyTagExifExposureProg);
				GetPropertyRationalU(bitmap, PropertyTagExifExposureTime, &m_oExposureTime);
				GetPropertyRational(bitmap,  PropertyTagExifExposureBias, &m_oExposureCompensation);
				GetPropertyRational(bitmap,  PropertyTagExifShutterSpeed, &m_oShutterSpeed);
				GetPropertyRationalU(bitmap, PropertyTagExifAperture,     &m_oLensAperture);
				GetPropertyRationalU(bitmap, PropertyTagExifFocalLength,  &m_oFocalLength);
				GetPropertyRationalU(bitmap, PropertyTagExifFNumber,      &m_oFNumber);
				m_nISOSpeed             = GetPropertyInt(bitmap, PropertyTagExifISOSpeed);
				m_nMeteringMode         = GetPropertyInt(bitmap, PropertyTagExifMeteringMode);
				m_nFlashMode            = GetPropertyInt(bitmap, PropertyTagExifFlash);
				m_nColorSpace           = GetPropertyInt(bitmap, PropertyTagExifColorSpace);

				m_nOrientation          = GetPropertyShort(bitmap, PropertyTagOrientation);

				GetPropertyByte(bitmap, PropertyTagGpsVer, m_sGpsVersion, 4);				
				m_sGpsLatitudeRef	= GetPropertyStr(bitmap, PropertyTagGpsLatitudeRef);
				GetPropertyRationalU(bitmap, PropertyTagGpsLatitude, m_sGpsLatitude,3);
				m_sGpsLongitudeRef	= GetPropertyStr(bitmap, PropertyTagGpsLongitudeRef);
				GetPropertyRationalU(bitmap, PropertyTagGpsLongitude, m_sGpsLongitude,3);
				m_sGpsAltitudeRef	= GetPropertyByte(bitmap, PropertyTagGpsAltitudeRef);
				GetPropertyRationalU(bitmap, PropertyTagGpsAltitude, m_sGpsAltitude,3);
				GetPropertyRationalU(bitmap, PropertyTagGpsGpsTime, m_sGpsTimeStamp,3);
				m_sGpsDateStamp	= GetPropertyStr(bitmap, PropertyTagGpsDate);		
				
				if ( m_nBitDepth <= 1 )
					m_nBitDepth = (short)GetPixelFormatSize( bitmap->GetPixelFormat() );


				// Запрашиваем формат картинки
				GUID guidFormat;
				bitmap->GetRawFormat( &guidFormat );

				// check for format type
				if ( ImageFormatTIFF == guidFormat )
				{
					if( bitmap->GetFrameCount(&FrameDimensionPage) > 1 )
						m_bMultiPageImage = TRUE;
				}
				else if ( ImageFormatGIF == guidFormat )
				{
					if( bitmap->GetFrameCount(&FrameDimensionTime) > 1 )
						m_bMultiPageImage = TRUE;
				}


				// remove property load-save errors here
				if (bitmap->GetLastStatus() != Ok)
					return FALSE;

				return TRUE;
			}

			// Save MetaDatafunctions

			void WriteNode(XmlUtils::CXmlWriter &oXmlWriter, const CString &m_sName, const CString &m_sValue)
			{
				CString sValue = m_sValue;
				sValue.Replace(_T("&"), _T("&amp;"));
				sValue.Replace(_T("<"), _T("&lt;"));
				sValue.Replace(_T(">"), _T("&gt;"));
				sValue.Replace(_T("'"), _T("&apos;"));
				sValue.Replace(_T("\""), _T("&quot;"));
				oXmlWriter.WriteNode(m_sName, sValue.GetBuffer());
			}

			BOOL GetValueOfAttribute(CString sAttribute, CString *sValue)
			{
				int nPosition = m_sMetaDataXML.Find(sAttribute);

				if ( -1 == nPosition )
					return FALSE;

				nPosition += int(m_sMetaDataXML.GetLength());
				while ( ' ' != m_sMetaDataXML[nPosition] && '\\' != m_sMetaDataXML[nPosition] )
				{
					*sValue += m_sMetaDataXML[nPosition];
					nPosition++;
				}

				return TRUE;
			}

			void ReadNodeText(XmlUtils::CXmlReader &oReader, const CString &sName, CString *sValue)
			{
				oReader.ReadRootNode();
				oReader.ReadNode(sName);
				*sValue = oReader.ReadNodeText();
			}
			void ReadNodeText(XmlUtils::CXmlReader &oReader, const CString &sName, TRational *oValue)
			{
				CString sValue = _T("");
				oReader.ReadRootNode();
				oReader.ReadNode(sName);
				sValue = oReader.ReadNodeText();
				int pos_flash = sValue.Find(_T("/"));
				int pos_point = sValue.Find(_T("."));
				
				if (pos_flash >=0)
				{
					oValue->Fraction = _ttoi(sValue.Left(pos_flash));
					oValue->Denominator = _ttoi(sValue.Mid(pos_flash+1));
				}
				else
				{
					if (pos_point>=0)
					{
						float fValue = _tstof(sValue);
						oValue->Denominator = 100;
						oValue->Fraction = int(fValue*100);
					}
					else
					{
						oValue->Fraction = _ttoi(sValue);
						oValue->Denominator = 0;
					}
				}
			}			
			void ReadNodeText(XmlUtils::CXmlReader &oReader, const CString &sName, TRationalU *oValue,int max_size_read = 1)
			{
				CString sValue = _T("");
				oReader.ReadRootNode();
				oReader.ReadNode(sName);
				sValue = oReader.ReadNodeText();
				
				CString sValueOne = sValue;
				
				int i=0;
				while(true)
				{					
					int pos_space = sValue.Find(_T(" "));
					if (pos_space > 0)
					{
						sValueOne = sValue.Left(pos_space);
						sValue = sValue.Mid(pos_space +1);
					}else
					{
						sValueOne = sValue;
						sValue = L"";
					}

					int pos_flash = sValueOne.Find(_T("/"));
					int pos_point = sValueOne.Find(_T("."));
				
				if (pos_flash >=0)
				{
						oValue[i].Fraction = _ttoi(sValueOne.Left(pos_flash));
						oValue[i].Denominator = _ttoi(sValueOne.Mid(pos_flash+1));
				}
				else
				{
					if (pos_point>=0)
					{
							float fValue = _tstof(sValueOne);
							oValue[i].Denominator = 100;
							oValue[i].Fraction = int(fValue*100);
					}
					else
					{
							oValue[i].Fraction = _ttoi(sValueOne);
							oValue[i].Denominator = 0;
						}
					}
					if (sValue.GetLength()<1)break;
					i++;
					if (i>max_size_read)break;
				}
			}

			void ReadNodeText(XmlUtils::CXmlReader &oReader, const CString &sName, short *nValue)
			{
				CString sValue = _T("");
				oReader.ReadRootNode();
				oReader.ReadNode(sName);
				sValue = oReader.ReadNodeText();
				*nValue = _ttoi(sValue);
			}
			void ReadNodeText(XmlUtils::CXmlReader &oReader, const CString &sName, BYTE *nValue, int size=1)
			{
				CString sValue = _T("");
				oReader.ReadRootNode();
				oReader.ReadNode(sName);
				sValue = oReader.ReadNodeText();
				BytesFromString(sValue,nValue,size);
			}
			void ReadNodeText(XmlUtils::CXmlReader &oReader, const CString &sName, BOOL *nValue)
			{
				CString sValue = _T("");
				oReader.ReadRootNode();
				oReader.ReadNode(sName);
				sValue = oReader.ReadNodeText();
				*nValue = _ttoi(sValue);
			}
			void ReadSubNodeText(XmlUtils::CXmlReader &oReader, const CString &sNodeName, const CString &sSubNodeName, CString *sValue)
			{
				oReader.ReadRootNode();
				oReader.ReadNode(sNodeName);
				oReader.ReadNode(sSubNodeName);
				*sValue = oReader.ReadNodeText();
			}

			void ReadSubNodeText(XmlUtils::CXmlReader &oReader, const CString &sNodeName, const CString &sSubNodeName, short *nValue)
			{
				CString sValue = _T("");
				oReader.ReadRootNode();
				oReader.ReadNode(sNodeName);
				oReader.ReadNode(sSubNodeName);
				sValue = oReader.ReadNodeText();
				*nValue = _ttoi(sValue);
			}

			BOOL MakeXML()
			{
				XmlUtils::CXmlWriter oXmlWriter;

				oXmlWriter.WriteNodeBegin(_T("MetaData"));

				WriteNode(oXmlWriter, _T("Title"), m_sTitle);
				WriteNode(oXmlWriter, _T("Author"), m_sAuthor);
				WriteNode(oXmlWriter, _T("Description"), m_sDescription);

				oXmlWriter.WriteNodeBegin(_T("DateTime"));

				oXmlWriter.WriteNode(_T("Year"), m_oDateTimeCreation.nYear);
				oXmlWriter.WriteNode(_T("Month"), m_oDateTimeCreation.nMonth);
				oXmlWriter.WriteNode(_T("Day"), m_oDateTimeCreation.nDay);
				oXmlWriter.WriteNode(_T("Hour"), m_oDateTimeCreation.nHour);
				oXmlWriter.WriteNode(_T("Minute"), m_oDateTimeCreation.nMinute);
				oXmlWriter.WriteNode(_T("Second"), m_oDateTimeCreation.nSecond);
				WriteNode(oXmlWriter, _T("Summary"), m_oDateTimeCreation.sSummary);
				
				oXmlWriter.WriteNodeEnd(_T("DateTime"));

				WriteNode(oXmlWriter, _T("Copyright"), m_sCopyright);
				WriteNode(oXmlWriter, _T("Disclaimer"), m_sDisclaimer);
				WriteNode(oXmlWriter, _T("Comment"), m_sComment);
				WriteNode(oXmlWriter,_T("EquipmentType"), m_sEquipmentType);
				WriteNode(oXmlWriter, _T("EquipmentModel"), m_sEquipmentModel);

				oXmlWriter.WriteNodeBegin(_T("JobTime"));

				oXmlWriter.WriteNode(_T("Hour"), m_oJobTime.nHour);
				oXmlWriter.WriteNode(_T("Minute"), m_oJobTime.nMinute);
				oXmlWriter.WriteNode(_T("Second"), m_oJobTime.nSecond);

				oXmlWriter.WriteNodeEnd(_T("JobTime"));

				WriteNode(oXmlWriter, _T("JobName"), m_sJobName);
				WriteNode(oXmlWriter, _T("SoftwareID"), m_sSoftwareID);
				oXmlWriter.WriteNode(_T("VersionNumber"), m_nVersionNumber);
				WriteNode(oXmlWriter, _T("VersionLetter"), m_sVersionLetter);
				WriteNode(oXmlWriter, _T("HostComputer"), m_sHostComputer);
				WriteNode(oXmlWriter, _T("Warning"), m_sWarning);
				WriteNode(oXmlWriter, _T("DocumentName"), m_sDocumentName);
				WriteNode(oXmlWriter, _T("PageName"), m_sPageName);
				oXmlWriter.WriteNode(_T("PageNumber"), m_nPageNumber);
				oXmlWriter.WriteNode(_T("PagesCount"), m_nPagesCount);

				if ( 1 == m_oExposureTime.Fraction || ( 0 != m_oExposureTime.Fraction && 0 == fmod( (float)m_oExposureTime.Denominator, (float)m_oExposureTime.Fraction ) ) )
				{
					WriteNode(oXmlWriter, _T("ExposureTime"), RationalUToString(m_oExposureTime) );
				}
				else if ( 0 != m_oExposureTime.Denominator )
				{
					float fExposureTime = (float)m_oExposureTime.Fraction / (float)m_oExposureTime.Denominator;
					CString sExposureTime;
					sExposureTime.Format(_T("%.1f"), fExposureTime);
					WriteNode(oXmlWriter, _T("ExposureTime"), sExposureTime );
				}
				else WriteNode(oXmlWriter, _T("ExposureTime"), _T("0") );
			
				CString sExposureProgram = _T("");
				switch(m_nExposureProgram)
				{
				case 0: sExposureProgram = _T("Not Defined"); break;
				case 1: sExposureProgram = _T("Manual"); break;
				case 2: sExposureProgram = _T("Normal program"); break;
				case 3: sExposureProgram = _T("Aperture priority"); break;
				case 4: sExposureProgram = _T("Shutter priority"); break;
				case 5: sExposureProgram = _T("Creative program (Slow speed)");  break;
				case 6: sExposureProgram = _T("Action program (High speed)"); break;
				case 7: sExposureProgram = _T("Portrait mode"); break;
				case 8: sExposureProgram = _T("Landscape mode"); break;
				}
				WriteNode(oXmlWriter, _T("ExposureProgram"), sExposureProgram);

				// ShutterSpeed
				if ( 1 == m_oShutterSpeed.Fraction || ( 0 != m_oShutterSpeed.Fraction && 0 == fmod( (float)m_oShutterSpeed.Denominator, (float)m_oShutterSpeed.Fraction ) ) )
				{
					WriteNode(oXmlWriter, _T("ShutterSpeed"), RationalToString(m_oShutterSpeed) );
				}
				else if ( 0 != m_oShutterSpeed.Denominator )
				{
					float fShutterSpeed = (float)m_oShutterSpeed.Fraction / (float)m_oShutterSpeed.Denominator;
					CString sShutterSpeed;
					sShutterSpeed.Format(_T("%.2f"), fShutterSpeed);
					WriteNode(oXmlWriter, _T("ShutterSpeed"), sShutterSpeed );
				}
				else
				{
					oXmlWriter.WriteNode( _T("ShutterSpeed"), m_oShutterSpeed.Fraction );
				}

				WriteNode(oXmlWriter, _T("LensAperture"), RationalUToString(m_oLensAperture) );
				WriteNode(oXmlWriter, _T("MaxApertureValue"), RationalUToString(m_oMaxApertureValue) );
				WriteNode(oXmlWriter, _T("FocalLength"), RationalUToString(m_oFocalLength) );
				
				if ( 0 != m_oFNumber.Denominator )
				{
					float fFNumber = (float)m_oFNumber.Fraction / (float)m_oFNumber.Denominator;
					CString sFNumber;
					sFNumber.Format(_T("%.2f"), fFNumber);
					WriteNode(oXmlWriter, _T("FNumber"), sFNumber );
				}
				else oXmlWriter.WriteNode( _T("FNumber"), (int)m_oFNumber.Fraction);
			
				if ( 0 != m_oBrightness.Denominator )
				{
					float fFNumber = (float)m_oBrightness.Fraction / (float)m_oBrightness.Denominator;
					CString sFNumber;
					sFNumber.Format(_T("%.2f"), fFNumber);
					WriteNode(oXmlWriter, _T("Brightness"), sFNumber );
				}
				else oXmlWriter.WriteNode( _T("Brightness"), (int)m_oBrightness.Fraction);

				if ( 0 != m_oExposureCompensation.Denominator )
				{
					float fFNumber = (float)m_oExposureCompensation.Fraction / (float)m_oExposureCompensation.Denominator;
					CString sFNumber;
					sFNumber.Format(_T("%.2f"), fFNumber);
					WriteNode(oXmlWriter, _T("ExposureCompensation"), sFNumber );
				}
				else oXmlWriter.WriteNode( _T("ExposureCompensation"), (int)m_oExposureCompensation.Fraction);

				oXmlWriter.WriteNode(_T("ISOSpeed"), m_nISOSpeed);
				oXmlWriter.WriteNode(_T("SensingMethod"), m_nSensingMethod);
				oXmlWriter.WriteNode(_T("WhiteBalance"), m_nWhiteBalance);
				oXmlWriter.WriteNode(_T("ExposureMode"), m_nExposureMode);
				oXmlWriter.WriteNode(_T("FocalLengthIn35mmFilm"), m_nFocalLengthIn35mmFilm);

				CString sMeteringMode = _T("");
				switch(m_nMeteringMode)
				{
				case 0: sMeteringMode = _T("Unknown"); break;
				case 1: sMeteringMode = _T("Average"); break;
				case 2: sMeteringMode = _T("Center-weighted average"); break;
				case 3: sMeteringMode = _T("Spot"); break;
				case 4: sMeteringMode = _T("Multi-spot"); break;
				case 5: sMeteringMode = _T("Pattern");  break;
				case 6: sMeteringMode = _T("Partial"); break;
				case 255: sMeteringMode = _T(""); break;
				}
				WriteNode(oXmlWriter, _T("MeteringMode"), sMeteringMode);

				CString sFlashMode = _T("");
				switch(m_nFlashMode)
				{
				case 0x0: sFlashMode = _T("No Flash"); break;
				case 0x1: sFlashMode = _T("Fired"); break; 
				case 0x5: sFlashMode = _T("Fired, Return not detected"); break; 
				case 0x7: sFlashMode = _T("Fired, Return detected"); break; 
				case 0x8: sFlashMode = _T("On, Did not fire"); break; 
				case 0x9: sFlashMode = _T("On, Fired"); break; 
				case 0xd: sFlashMode = _T("On, Return not detected"); break; 
				case 0xf: sFlashMode = _T("On, Return detected"); break; 
				case 0x10: sFlashMode = _T("Off, Did not fire"); break; 
				case 0x14: sFlashMode = _T("Off, Did not fire, Return not detected"); break; 
				case 0x18: sFlashMode = _T("Auto, Did not fire"); break; 
				case 0x19: sFlashMode = _T("Auto, Fired"); break; 
				case 0x1d: sFlashMode = _T("Auto, Fired, Return not detected"); break; 
				case 0x1f: sFlashMode = _T("Auto, Fired, Return detected"); break; 
				case 0x20: sFlashMode = _T("No flash function"); break; 
				case 0x30: sFlashMode = _T("Off, No flash function"); break; 
				case 0x41: sFlashMode = _T("Fired, Red-eye reduction"); break; 
				case 0x45: sFlashMode = _T("Fired, Red-eye reduction, Return not detected"); break; 
				case 0x47: sFlashMode = _T("Fired, Red-eye reduction, Return detected"); break; 
				case 0x49: sFlashMode = _T("On, Red-eye reduction"); break; 
				case 0x4d: sFlashMode = _T("On, Red-eye reduction, Return not detected"); break; 
				case 0x4f: sFlashMode = _T("On, Red-eye reduction, Return detected"); break; 
				case 0x50: sFlashMode = _T("Off, Red-eye reduction"); break; 
				case 0x58: sFlashMode = _T("Auto, Did not fire, Red-eye reduction"); break; 
				case 0x59: sFlashMode = _T("Auto, Fired, Red-eye reduction"); break; 
				case 0x5d: sFlashMode = _T("Auto, Fired, Red-eye reduction, Return not detected"); break; 
				case 0x5f: sFlashMode = _T("Auto, Fired, Red-eye reduction, Return detected"); break;
				}
				WriteNode(oXmlWriter, _T("FlashMode"), sFlashMode);

				CString sColorSpace = _T("");
				switch(m_nColorSpace)
				{
					case 1: sColorSpace = _T("sRGB"); break;
					case 65535: sColorSpace = _T("Uncalibrated"); break;
				}
				WriteNode(oXmlWriter, _T("ColorSpace"), sColorSpace);

				WriteNode(oXmlWriter, _T("GpsVersion"),		CString(m_sGpsVersion));
				WriteNode(oXmlWriter, _T("GpsLatitudeRef"),	m_sGpsLatitudeRef);
				WriteNode(oXmlWriter, _T("GpsLatitude"),	RationalUToString(m_sGpsLatitude,3));
				WriteNode(oXmlWriter, _T("GpsLongitudeRef"),m_sGpsLongitudeRef);
				WriteNode(oXmlWriter, _T("GpsLongitude"),	RationalUToString(m_sGpsLongitude,3));
				oXmlWriter.WriteNode(_T("GpsAltitudeRef"),	m_sGpsAltitudeRef);
				WriteNode(oXmlWriter, _T("GpsAltitude"),	RationalUToString(m_sGpsAltitude,3));
				WriteNode(oXmlWriter, _T("GpsTimeStamp"),	RationalUToString(m_sGpsTimeStamp,3));
				WriteNode(oXmlWriter, _T("GpsDateStamp"),	m_sGpsDateStamp);

				oXmlWriter.WriteNode(_T("BitDepth"), m_nBitDepth);
				oXmlWriter.WriteNode(_T("MultiPageImage"), (int)m_bMultiPageImage );

				oXmlWriter.WriteNodeEnd(_T("MetaData"));
#ifdef _UNICODE
				oXmlWriter.EncodingUnicodeToUTF8();
#else
				oXmlWriter.EncodingASCIIToUTF8();
#endif
				
				m_sMetaDataXML = oXmlWriter.GetXmlString();

				return TRUE;
			}

			BOOL ParseXML(CString sXML)
			{
				XmlUtils::CXmlReader oReader;
				
				oReader.SetXmlString(sXML);

				ReadNodeText(oReader, _T("Title"),       &m_sTitle);
				ReadNodeText(oReader, _T("Author"),      &m_sAuthor);
				ReadNodeText(oReader, _T("Description"), &m_sDescription);

				ReadSubNodeText(oReader, _T("DateTime"), _T("Year"),    &m_oDateTimeCreation.nYear);
				ReadSubNodeText(oReader, _T("DateTime"), _T("Month"),   &m_oDateTimeCreation.nMonth);
				ReadSubNodeText(oReader, _T("DateTime"), _T("Day"),     &m_oDateTimeCreation.nDay);
				ReadSubNodeText(oReader, _T("DateTime"), _T("Hour"),    &m_oDateTimeCreation.nHour);
				ReadSubNodeText(oReader, _T("DateTime"), _T("Minute"),  &m_oDateTimeCreation.nMinute);
				ReadSubNodeText(oReader, _T("DateTime"), _T("Second"),  &m_oDateTimeCreation.nSecond);
				ReadSubNodeText(oReader, _T("DateTime"), _T("Summary"), &m_oDateTimeCreation.sSummary);

				ReadNodeText(oReader, _T("Copyright"),      &m_sCopyright);
				ReadNodeText(oReader, _T("Disclaimer"),     &m_sDisclaimer);
				ReadNodeText(oReader, _T("Comment"),        &m_sComment);
				ReadNodeText(oReader, _T("EquipmentType"),  &m_sEquipmentType);
				ReadNodeText(oReader, _T("EquipmentModel"), &m_sEquipmentModel);

				ReadSubNodeText(oReader, _T("JobTime"), _T("Hour"),   &m_oJobTime.nHour);
				ReadSubNodeText(oReader, _T("JobTime"), _T("Minute"), &m_oJobTime.nMinute);
				ReadSubNodeText(oReader, _T("JobTime"), _T("Second"), &m_oJobTime.nSecond);

				ReadNodeText(oReader, _T("JobName"),       &m_sJobName);
				ReadNodeText(oReader, _T("SoftwareID"),    &m_sSoftwareID);
				ReadNodeText(oReader, _T("VersionNumber"), &m_nVersionNumber);
				ReadNodeText(oReader, _T("VersionLetter"), &m_sVersionLetter);
				ReadNodeText(oReader, _T("HostComputer"),  &m_sHostComputer);
				ReadNodeText(oReader, _T("Warning"),       &m_sWarning);
				ReadNodeText(oReader, _T("DocumentName"),  &m_sDocumentName);
				ReadNodeText(oReader, _T("PageName"),      &m_sPageName);
				ReadNodeText(oReader, _T("PageNumber"),    &m_nPageNumber);
				ReadNodeText(oReader, _T("PagesCount"),    &m_nPagesCount);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// ExposureTime
				ReadNodeText(oReader, _T("ExposureTime"),&m_oExposureTime);
				ReadNodeText(oReader, _T("ExposureCompensation"),&m_oExposureCompensation);
				ReadNodeText(oReader, _T("Brightness"),&m_oBrightness);
				
				CString strExposureProgram;
				ReadNodeText(oReader, _T("ExposureProgram"),&strExposureProgram);
				if(strExposureProgram == _T("Not Defined"))m_nExposureProgram =0;
				if(strExposureProgram == _T("Manual"))m_nExposureProgram =1;
				if(strExposureProgram == _T("Normal program"))m_nExposureProgram =2;
				if(strExposureProgram == _T("Aperture priority"))m_nExposureProgram =3;
				if(strExposureProgram == _T("Shutter priority"))m_nExposureProgram =4;
				if(strExposureProgram == _T("Creative program (Slow speed)"))m_nExposureProgram =5;
				if(strExposureProgram == _T("Action program (High speed)"))m_nExposureProgram =6;
				if(strExposureProgram == _T("Portrait mode"))m_nExposureProgram =7;
				if(strExposureProgram == _T("Landscape mode"))m_nExposureProgram =8;

				ReadNodeText(oReader, _T("LensAperture"),&m_oLensAperture);
				ReadNodeText(oReader, _T("MaxApertureValue"),&m_oMaxApertureValue);
				ReadNodeText(oReader, _T("FocalLength"),&m_oFocalLength);
				ReadNodeText(oReader, _T("FNumber"),&m_oFNumber);

				ReadNodeText(oReader, _T("ISOSpeed"), &m_nISOSpeed);
				ReadNodeText(oReader, _T("SensingMethod"), &m_nSensingMethod);
				ReadNodeText(oReader, _T("WhiteBalance"), &m_nWhiteBalance);
				ReadNodeText(oReader, _T("ExposureMode"), &m_nExposureMode);
				ReadNodeText(oReader, _T("FocalLengthIn35mmFilm"), &m_nFocalLengthIn35mmFilm);
				
				CString strMeteringMode ;
				ReadNodeText(oReader, _T("MeteringMode"), &strMeteringMode);
				if(strMeteringMode == _T("Unknown"))m_nMeteringMode =0;
				if(strMeteringMode == _T("Average"))m_nMeteringMode =1;
				if(strMeteringMode == _T("Center-weighted average"))m_nMeteringMode =2;
				if(strMeteringMode == _T("Spot"))m_nMeteringMode =3;
				if(strMeteringMode == _T("Multi-spot"))m_nMeteringMode =4;
				if(strMeteringMode == _T("Pattern"))m_nMeteringMode =5;
				if(strMeteringMode == _T("Partial"))m_nMeteringMode =6;
				if(strMeteringMode == _T(""))m_nMeteringMode =255;

				CString strFlashMode;
				ReadNodeText(oReader, _T("FlashMode"), &strFlashMode);
				if (strFlashMode == _T("No Flash")) m_nFlashMode =0x0;
				if (strFlashMode == _T("Fired")) m_nFlashMode = 0x1;
				if (strFlashMode == _T("Fired, Return not detected")) m_nFlashMode = 0x5;
				if (strFlashMode == _T("Fired, Return detected")) m_nFlashMode = 0x7;
				if (strFlashMode == _T("On, Did not fire")) m_nFlashMode = 0x8;
				if (strFlashMode == _T("On, Fired")) m_nFlashMode = 0x9;
				if (strFlashMode == _T("On, Return not detected")) m_nFlashMode = 0xd;
				if (strFlashMode == _T("On, Return detected")) m_nFlashMode = 0xf;
				if (strFlashMode == _T("Off, Did not fire")) m_nFlashMode = 0x10;
				if (strFlashMode == _T("Off, Did not fire, Return not detected")) m_nFlashMode = 0x14;
				if (strFlashMode == _T("Auto, Did not fire")) m_nFlashMode = 0x18;
				if (strFlashMode == _T("Auto, Fired")) m_nFlashMode = 0x19;
				if (strFlashMode == _T("Auto, Fired, Return not detected")) m_nFlashMode = 0x1d;
				if (strFlashMode == _T("Auto, Fired, Return detected")) m_nFlashMode = 0x1f;
				if (strFlashMode == _T("No flash function")) m_nFlashMode = 0x20;
				if (strFlashMode == _T("Off, No flash function")) m_nFlashMode = 0x30;
				if (strFlashMode == _T("Fired, Red-eye reduction")) m_nFlashMode = 0x41;
				if (strFlashMode == _T("Fired, Red-eye reduction, Return not detected")) m_nFlashMode = 0x45;
				if (strFlashMode == _T("Fired, Red-eye reduction, Return detected")) m_nFlashMode = 0x47;
				if (strFlashMode == _T("On, Red-eye reduction")) m_nFlashMode = 0x49;
				if (strFlashMode == _T("On, Red-eye reduction, Return not detected")) m_nFlashMode = 0x4d;
				if (strFlashMode == _T("On, Red-eye reduction, Return detected")) m_nFlashMode = 0x4f;
				if (strFlashMode == _T("Off, Red-eye reduction")) m_nFlashMode = 0x50;
				if (strFlashMode == _T("Auto, Did not fire, Red-eye reduction")) m_nFlashMode = 0x58;
				if (strFlashMode == _T("Auto, Fired, Red-eye reduction")) m_nFlashMode = 0x59;
				if (strFlashMode == _T("Auto, Fired, Red-eye reduction, Return not detected")) m_nFlashMode = 0x5d;
				if (strFlashMode == _T("Auto, Fired, Red-eye reduction, Return detected")) m_nFlashMode =0x5f;

				CString strColorSpace;
				ReadNodeText(oReader, _T("ColorSpace"), &strColorSpace);
				if (strColorSpace == _T("sRGB"))m_nColorSpace =1;
				if (strColorSpace == _T("Uncalibrated"))m_nColorSpace =65535;

				ReadNodeText(oReader, _T("BitDepth"),&m_nBitDepth);
				ReadNodeText(oReader, _T("MultiPageImage"), &m_bMultiPageImage );
			
				ReadNodeText(oReader, _T("GpsVersion"),		m_sGpsVersion, 4);
				ReadNodeText(oReader, _T("GpsLatitudeRef"),	&m_sGpsLatitudeRef);
				ReadNodeText(oReader, _T("GpsLatitude"),	m_sGpsLatitude, 3);
				ReadNodeText(oReader, _T("GpsLongitudeRef"),&m_sGpsLongitudeRef);
				ReadNodeText(oReader, _T("GpsLongitude"),	m_sGpsLongitude, 3);
				ReadNodeText(oReader, _T("GpsAltitudeRef"),	&m_sGpsAltitudeRef,1);
				ReadNodeText(oReader, _T("GpsAltitude"),	m_sGpsAltitude, 3);
				ReadNodeText(oReader, _T("GpsTimeStamp"),	m_sGpsTimeStamp, 3);
				ReadNodeText(oReader, _T("GpsDateStamp"),	&m_sGpsDateStamp);
				return TRUE;
			}
		private:

			struct TDateTime
			{
				short   nYear;
				short   nMonth;
				short   nDay;
				short   nHour;
				short   nMinute;
				short   nSecond;
				CString sSummary;
			};

			CString   m_sAuthor;
			CString   m_sTitle;
			CString   m_sDescription;
			TDateTime m_oDateTimeCreation;
			CString   m_sCopyright;
			CString   m_sDisclaimer;
			CString   m_sComment;
			CString   m_sEquipmentType;
			CString   m_sEquipmentModel;
			TDateTime m_oJobTime;
			CString   m_sJobName;
			CString   m_sSoftwareID;
			short     m_nVersionNumber;
			CString   m_sVersionLetter;
			CString   m_sHostComputer;
			CString   m_sWarning;
			CString   m_sDocumentName;
			CString   m_sPageName;
			short     m_nPageNumber;
			short     m_nPagesCount;

			BYTE		m_sGpsVersion[4];
			CString		m_sGpsLatitudeRef;//size = 2
			TRationalU	m_sGpsLatitude[3];
			CString		m_sGpsLongitudeRef;//size = 2
			TRationalU	m_sGpsLongitude[3];
			BYTE		m_sGpsAltitudeRef;
			TRationalU	m_sGpsAltitude[3];
			TRationalU	m_sGpsTimeStamp[3];
			CString		m_sGpsDateStamp;//size = 11

			short      m_nExposureProgram;
			TRationalU m_oExposureTime;
			TRational  m_oExposureCompensation;
			TRational  m_oBrightness;
			TRational  m_oShutterSpeed;
			TRationalU m_oLensAperture;
			TRationalU m_oFocalLength;
			TRationalU m_oFNumber;

			short     m_nISOSpeed;
			short     m_nMeteringMode; 
			short     m_nFlashMode;
			short     m_nColorSpace;
			short	  m_nSensingMethod;
			short	  m_nWhiteBalance;
			short	  m_nExposureMode ;
			short	  m_nFocalLengthIn35mmFilm;
			
			TRationalU m_oMaxApertureValue;

			short     m_nOrientation;

			short     m_nBitDepth;
			BOOL      m_bMultiPageImage;

			CString   m_sMetaDataXML;

			BOOL      m_bBitmap;

		protected:

#pragma pack(push,1)

			// Tiff  структуры
			struct TTiffFileHeader
			{
				BYTE nByteOrder[2];
				BYTE nSignature[2];
				BYTE nIDFoffset[4];
			};

			struct TDirectoryEntry
			{
				BYTE nTag[2];
				BYTE nType[2];
				BYTE nCount[4];
				BYTE nValueOffset[4];
			};



			// PNG структуры 
			struct TChunk
			{
				BYTE nLenght[4];
				BYTE nType[4];
			};
			struct TChunkTIME
			{
				BYTE  nYear[2];
				BYTE  nMonth;
				BYTE  nDay;
				BYTE  nHour;
				BYTE  nMinute;
				BYTE  nSecond;
			};
			struct TChunkACTL
			{
				BYTE unNumFrames[4];
				BYTE unNumPlays[4];
			};


			// TGA структуры
			struct TFileFooter
			{
				DWORD nExtensionAreaOffset;
				DWORD nDeveloperAreaOffset;
				BYTE arrSignature[18];
			};
			struct TExtensionArea
			{
				WORD  nExtensionSize;
				BYTE  sAuthorName[41];
				BYTE  sComments[324];
				short nDateTime[6];
				BYTE  sJobName[41];
				short nJobTime[3];
				BYTE  sSoftwareID[41];
				short nSoftwareVersionNumber;
				BYTE nSoftwareVersionLetter;
				long  nKeyColor;
				short nPixelAspectRatio[2];
				short nGammaValue[2];
				long  nColorCorrectionOffset;
				long  nPostageStampOffset;
				long  nScanLineOffset;
				BYTE  nAttributesType;  
			};


			// Raw струтуры
			// Raw-foveon
			struct TFoveonDirectorySectionHeader
			{
				BYTE  pSignature[4];
				short nVersion[2];
				DWORD nDirectoryEntriesCount;
			};

			struct TFoveonDirectoryEntries
			{
				DWORD nOffeset;
				DWORD nLenght;
				BYTE  pType[4];
			};

			struct TFoveonPROPHeader
			{
				BYTE  pSignature[4];
				DWORD nVersion;
				DWORD nEntriesCount;
				DWORD nFormat;
				BYTE  pRESERVED[4]; // не используется				
				DWORD nTotalLenght; 
				// total, потомучто тут сумма длины имени 
				// свойста и его значения
			};
			struct TFoveonPROPOffsets
			{
				DWORD nName;
				DWORD nValue;
			};

			// PDS структуры
			struct TPSDFileHeader
			{
				BYTE nSignature[4]; // always equal 8BPS, do not read file if not
				BYTE nVersion[2];   // always equal 1, do not read file if not
				BYTE nReserved[6];  // must be zero
				BYTE nChannels[2];  // numer of channels including any alpha channels, supported range 1 to 24
				BYTE nRows[4];      // height in PIXELS, supported range 1 to 30000
				BYTE nColumns[4];   // width in PIXELS, supported range 1 to 30000
				BYTE nDepth[2];     // number of bpp
				BYTE nMode[2];      // colour mode of the EColorMode
			};
			// PCX структуры
			struct TPCXFileHeader
			{
				BYTE nManufacturer;			// код производителя (редактора)
				BYTE nVersion;				// версия файла
				BYTE bEncoding;				// сжатие данных: 0 - нет, 1 - да
				BYTE nBitsPerPixelPerPlane;	// количество бит на пиксел в слое
				WORD nMinX, nMinY;			// координаты изображения 
				WORD nMaxX, nMaxY;			//
				WORD nScreenHorResolution;	// разрешение монитора по горизонтали
				WORD nScreenVerResolution;	// разрешение монитора по вертикали
				BYTE arrColorMap[48];		// палитра RGB на 16 цветов
				BYTE nReserved;				// номер видеорижима (сейчас не используеться)
				BYTE nPlanes;				// количество слоёв
				WORD nBytesPerLinePerPlane; // количество байт на строку в слое
				WORD bGrayscalePalette;		// итерпритация цветов палитры: 0 - оттенки серого, 1 - RGB
				WORD nScannerHorResolution;	// разрешение сканера по горизонтали
				WORD nScannerVerResolution;	// разрешение сканера по вертикали
				BYTE arrExtraData[54];		// дополнительная информация
			};
			// Ras структуры
			struct TRasFileHeader
			{
				DWORD nMagicNumber;     // Magic (identification) number 
				DWORD nWidth;           // Width of image in pixels 
				DWORD nHeight;          // Height of image in pixels 
				DWORD nDepth;           // Number of bits per pixel 
				DWORD nSize;            // Size of image data in bytes
				DWORD nType;            // Type of raster file 
				DWORD nPaletteType;     // Type of color map 
				DWORD nPaletteLength;   // Size of the color map in bytes 
			};


#pragma pack(pop)

			// класс для проверки чексуммы
			// каждого кусочка файлов формата PNG
			class crc32
			{
			
			protected:
	      
				unsigned m_Table[256];

			public:
	      
				unsigned m_crc32;      
				crc32()
				{
					const unsigned CRC_POLY = 0xEDB88320;
					unsigned i, j, r;
					for (i = 0; i < 256; i++)
					{
						for (r = i, j = 8; j; j--)
							r = r & 1? (r >> 1) ^ CRC_POLY: r >> 1;
						m_Table[i] = r;
					}
					m_crc32 = 0;
				}
				void ProcessCRC(void* pData, int nLen)
				{
					const unsigned CRC_MASK = 0xD202EF8D;
					register unsigned char* pdata = reinterpret_cast<unsigned char*>(pData);
					register unsigned crc = m_crc32;
					while (nLen--)
					{
						crc = m_Table[static_cast<unsigned char>(crc) ^ *pdata++] ^ crc >> 8;
						crc ^= CRC_MASK;
					}
					m_crc32 = crc;
				}
			};



		public:


		};




 
