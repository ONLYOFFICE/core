#pragma once
#include <registration.h>

#define USE_ATL_CSTRING
#include <StringUtils.h>
#include <ImageSerializeConstants.h>
#include <ImageSerializeConstants2.h>
#include <ImageSerializeObjectsCore.h>

namespace ImageStudio
{
	namespace Serialize
	{
		namespace Effects
		{
			class ImageClear : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Index);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Index = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("index"), Index);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Index = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("index"), _T("-1")));
				}
				
				virtual void InternalClear()
				{
					Index = -1;
				}
				
			public:
				
				ImageClear()
				{
					Initialize(c_nSerializeImageClear, c_strSerializeImageClear);

					Clear();
				}
					
			public:
				
				int Index;
				
			};
			class ImageCopy : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 5*sizeof(int) + MaskPoints.GetSize()*sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(IndexFrom);
					pStream->WriteLong(IndexTo);
					pStream->WriteLong(MaskType);
					pStream->WriteLong(Metric);
					pStream->WriteLong(MaskPoints.GetSize());
					for (int index = 0; index < MaskPoints.GetSize(); ++index)
						pStream->WriteDouble(MaskPoints[index]);

				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					IndexFrom = pStream->ReadLong();
					IndexTo = pStream->ReadLong();
					MaskType = pStream->ReadLong();
					Metric = pStream->ReadLong();

					MaskPoints.RemoveAll();

					int nCount = pStream->ReadLong();

					for (int index = 0; index < nCount; ++index)
						MaskPoints.Add(pStream->ReadDouble());
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("index-from"), IndexFrom);
					oXmlWriter.WriteAttribute(_T("index-to"), IndexTo);
					oXmlWriter.WriteAttribute(_T("mask-type"), MaskType);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);

					_bstr_t strPointsData = Strings::FromBinary((BYTE*)MaskPoints.GetData(), MaskPoints.GetSize()*sizeof(double));

					oXmlWriter.WriteAttribute(_T("mask-data"), (TCHAR*)(strPointsData));

				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					IndexFrom = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("index-from"), _T("-1")));
					IndexTo = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("index-to"), _T("-1")));
					MaskType = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("mask-type"), _T("0")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));

					MaskPoints.RemoveAll();

					// try to load points from string like " 1 2 3 4 5 6" (string contains x,y pairs)
					CString strPoints = oXmlNode.GetAttributeOrValue(_T("mask-points"), _T(""));
					if (strPoints.GetLength() > 0)
					{
						Templates::CArray<CString> arrSubstrings;
						int nCoordinates = Strings::Split(strPoints, _T(" "), arrSubstrings);
						
						if ((nCoordinates % 2) == 0)
						{
							for (int index = 0; index < arrSubstrings.GetCount(); ++index)
								MaskPoints.Add(Strings::ToDouble(arrSubstrings[index]));
						}
					}

					// try to load points from base64 encoded string
					_bstr_t strPointsData = oXmlNode.GetAttributeOrValue(_T("mask-data"), _T(""));
					if (strPointsData.length() > 0)

					{
						BYTE* pData = 0;
						int nSizeAllocated = 0;
						int nSizeArray = 0;

						if (Strings::ToBinary(CString((char*)(strPointsData)), pData, nSizeAllocated, nSizeArray))
						{
							double* pPointsData = (double*)pData;
							int nPointsCount = nSizeArray/sizeof(double);

							if ((nSizeArray % sizeof(double)) == 0)
							{
								for (int index = 0; index < nPointsCount; ++index, ++pPointsData)
									MaskPoints.Add(*pPointsData);
							}

							delete[] pData;
						}
					}
				}
				
				virtual void InternalClear()
				{
					IndexFrom = -1;
					IndexTo = -1;

					MaskType = c_nMaskAllPixels;
					Metric = c_nMetricPixels;
					MaskPoints.RemoveAll();
				}
				
			public:
				
				ImageCopy()
				{
					Initialize(c_nSerializeImageCopy, c_strSerializeImageCopy);

					Clear();
				}
				
				void UpdateMask(int nWidth, int nHeight)
				{
					if (MaskType == c_nMaskNone || MaskType == c_nMaskAllPixels)
						return;
					if ((MaskType == c_nMaskRectangle || MaskType == c_nMaskInverseRectangle || MaskType == c_nMaskEllipse || MaskType == c_nMaskInverseEllipse) && MaskPoints.GetSize() != 4)
						return;
					if ((MaskType == c_nMaskPolygon || MaskType == c_nMaskInversePolygon) && MaskPoints.GetSize() < 6)
						return;

					// проверяем, может маска то уже создана ранее ?
					if (m_oMask.GetWidth() == nWidth && m_oMask.GetHeight() == nHeight)
						return;

					m_oMask.Create(nWidth, nHeight);

					// подсчитываем цвета заливки маски
					int nValueFill = 0;

					if (MaskType == c_nMaskRectangle)
						nValueFill = 0;
					else if (MaskType == c_nMaskInverseRectangle)
						nValueFill = 255;
					else if (MaskType == c_nMaskEllipse)
						nValueFill = 0;
					else if (MaskType == c_nMaskInverseEllipse)
						nValueFill = 255;
					else if (MaskType == c_nMaskPolygon)
						nValueFill = 0;
					else if (MaskType == c_nMaskInversePolygon)
						nValueFill = 255;

					// устанавливаем параметры в соответствии с метрикой
					double dMetricX, dMetricY;

					switch( Metric )
					{
					case ImageStudio::Serialize::c_nMetricPercents:
						dMetricX = nWidth * 0.01;
						dMetricY = nHeight * 0.01;
						break;
					case ImageStudio::Serialize::c_nMetricNormalizedCoordinates:
						dMetricX = nWidth;
						dMetricY = nHeight;
						break;
					case ImageStudio::Serialize::c_nMetricAlternativeNormalizedCoordinates:
						dMetricX = nWidth;
						dMetricY = nHeight;
						break;
					default:
						dMetricX = 1;
						dMetricY = 1;
					}

					if (MaskType == c_nMaskRectangle || MaskType == c_nMaskInverseRectangle || MaskType == c_nMaskEllipse || MaskType == c_nMaskInverseEllipse)
					{
						double dLeft   = MaskPoints[0] * dMetricX;
						double dTop    = MaskPoints[1] * dMetricY;
						double dRight  = MaskPoints[2] * dMetricX;
						double dBottom = MaskPoints[3] * dMetricY;

						m_oMask.Fill(nValueFill);

						if (MaskType == c_nMaskRectangle || MaskType == c_nMaskInverseRectangle)
							m_oMask.FillRectangle(dLeft, dTop, dRight, dBottom, 255 - nValueFill);
						else if (MaskType == c_nMaskEllipse || MaskType == c_nMaskInverseEllipse)
							m_oMask.FillEllipse(dLeft, dTop, dRight, dBottom, 255 - nValueFill);
					}
					else if (MaskType == c_nMaskPolygon || MaskType == c_nMaskInversePolygon)
					{
						int nPointsCount = MaskPoints.GetSize()/2;
						Gdiplus::PointF* pPoints = new Gdiplus::PointF[nPointsCount];
						if (!pPoints)
							return;

						for (int index = 0; index < nPointsCount; ++index)
						{
							pPoints[index].X = float(MaskPoints[2*index + 0] * dMetricX);
							pPoints[index].Y = float(MaskPoints[2*index + 1] * dMetricY);
						}

						m_oMask.Fill(nValueFill);
						m_oMask.FillPolygon(pPoints, nPointsCount, 255 - nValueFill);

						delete[] pPoints;
					}
				}
				//BYTE GetMaskValue(int nX, int nY, BYTE nDefaultValue)
				//{
				//	if (MaskType == c_nMaskNone)
				//		return 0;
				//	else if (MaskType == c_nMaskAllPixels)
				//		return 255;

				//	return m_oMask.GetPixel(nX, nY, nDefaultValue);
				//}

			public:
				ImageStudio::Core::Mask m_oMask;

				int IndexFrom;
				int IndexTo;

				int MaskType;
				int Metric;
				CSimpleArray<double> MaskPoints;
	
				/* @desc:
				ImageStudio-Serialize-Effects-ImageCopy (c_nSerializeImageCopy, c_strSerializeImageCopy)
				{
					int IndexFrom - 1;					// xml имя - index-from; индекс буффера из которого скопировать интерфейс
					int IndexTo = -1;					// xml имя - index-to; индекс буффера в который скопировать интерфейс

					// настройки маски для копирования
					int MaskType;						// xml имя - mask-type; тип маски, см. c_nMask...
					int Metric = c_nMetricPixels;		// xml имя - metriс; тип метрики, см. c_nMetric...
					CSimpleArray<double> MaskPoints;	// xml имя - mask-data или mask-points; координаты точек маски (в пикселях картинки или в другой какой-то метрике)
				}
				*/
			};
			
			class AdjustBrightness : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Level);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Level = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), Level);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Level = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("level"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Level = 0;
				}
				
			public:
				
				AdjustBrightness()
				{
					Initialize(c_nSerializeAdjustBrightness, c_strSerializeAdjustBrightness);

					Clear();
				}
					
			public:
				
				int Level; // -255..255
			};
			class AdjustBrightnessEx : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 3*sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(LevelRed);
					pStream->WriteLong(LevelGreen);
					pStream->WriteLong(LevelBlue);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					LevelRed = pStream->ReadLong();
					LevelGreen = pStream->ReadLong();
					LevelBlue = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("levelred"), LevelRed);
					oXmlWriter.WriteAttribute(_T("levelgreen"), LevelGreen);
					oXmlWriter.WriteAttribute(_T("levelblue"), LevelBlue);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					LevelRed = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("levelred"), _T("0")));
					LevelGreen = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("levelgreen"), _T("0")));
					LevelBlue = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("levelblue"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					LevelRed = 0;
					LevelGreen = 0;
					LevelBlue = 0;
				}
				
			public:
				
				AdjustBrightnessEx()
				{
					Initialize(c_nSerializeAdjustBrightnessEx, c_strSerializeAdjustBrightnessEx);

					Clear();
				}
								
			public:
				
				int LevelRed; // -255..255
				int LevelGreen; // -255..255
				int LevelBlue; // -255..255
			};
			class AdjustContrast : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Level);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Level = pStream->ReadDouble();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), Level);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Level = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("level"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Level = 0;
				}
				
			public:
				
				AdjustContrast()
				{
					Initialize(c_nSerializeAdjustContrast, c_strSerializeAdjustContrast);

					Clear();
				}
				
			public:
				
				double Level; // -100..100
			};
			class AdjustGamma : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Level);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Level = pStream->ReadDouble();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), Level);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Level = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("level"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Level = 0;
				}
				
			public:
				
				AdjustGamma()
				{
					Initialize(c_nSerializeAdjustGamma, c_strSerializeAdjustGamma);

					Clear();
				}
				
			public:
				
				double Level; // -100..100
			};
			class AdjustEqualize : public Core::Effect
			{
			public:
				
				AdjustEqualize()
				{
					Initialize(c_nSerializeAdjustEqualize, c_strSerializeAdjustEqualize);

					Clear();
				}
			};
			class AdjustGrayscale : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) + sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Desaturate);
					pStream->WriteDouble(Intensity);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Desaturate = pStream->ReadLong();
					Intensity  = pStream->ReadDouble();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("desaturate"), Desaturate);
					oXmlWriter.WriteAttribute(_T("intensity"), Intensity);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Desaturate = Strings::ToInteger(oXmlNode.GetAttribute(_T("desaturate"), _T("0"))) != 0 ? TRUE : FALSE;
					Intensity  = Strings::ToDouble(oXmlNode.GetAttribute(_T("intensity"), _T("100")));
				}
				
				virtual void InternalClear()
				{
					Desaturate = FALSE;
					Intensity = 100;
				}
				
			public:
				
				AdjustGrayscale()
				{
					Initialize(c_nSerializeAdjustGrayscale, c_strSerializeAdjustGrayscale);

					Clear();
				}
					
			public:
				
				BOOL Desaturate; // Intensity or Desaturate algorithm
				double Intensity;
				
			};
			class AdjustInvert : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) + sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(bOverGray);
					pStream->WriteDouble(Intensity);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					bOverGray = pStream->ReadLong();
					Intensity = pStream->ReadDouble();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("overgray"), bOverGray);
					oXmlWriter.WriteAttribute(_T("intensity"), Intensity);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					bOverGray = Strings::ToInteger(oXmlNode.GetAttribute(_T("overgray"), _T("1"))) != 0 ? TRUE : FALSE;
					Intensity = Strings::ToDouble(oXmlNode.GetAttribute(_T("intensity"), _T("100")));
				}
				
				virtual void InternalClear()
				{
					bOverGray = TRUE;
					Intensity = 100;
				}

			public:
				
				AdjustInvert()
				{
					Initialize(c_nSerializeAdjustInvert, c_strSerializeAdjustInvert);

					Clear();
				}
			
			public:
				BOOL bOverGray;
				double Intensity;
			};
			class AdjustAutobrightness : public Core::Effect
			{
			public:
				
				AdjustAutobrightness()
				{
					Initialize(c_nSerializeAdjustAutobrightness, c_strSerializeAdjustAutobrightness);

					Clear();
				}
			};
			class AdjustAutocontrast : public Core::Effect
			{
			public:
				
				AdjustAutocontrast()
				{
					Initialize(c_nSerializeAdjustAutocontrast, c_strSerializeAdjustAutocontrast);

					Clear();
				}
			};
			class AdjustAutolevels : public Core::Effect
			{
			public:
				
				AdjustAutolevels()
				{
					Initialize(c_nSerializeAdjustAutolevels, c_strSerializeAdjustAutolevels);

					Clear();
				}
			};
			class AdjustHue : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Angle);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Angle = pStream->ReadDouble();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("angle"), Angle);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Angle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Angle = 0;
				}
				
			public:
				
				AdjustHue()
				{
					Initialize(c_nSerializeAdjustHue, c_strSerializeAdjustHue);

					Clear();

					bUseVideoCard = FALSE; //TRUE;
				}
				
			public:
				
				double Angle; // -360..360
			};
			class AdjustSaturation : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Level);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Level = pStream->ReadDouble();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), Level);
					oXmlWriter.WriteAttribute(_T("factor"), Factor);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Level  = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("level"), _T("0")));
					Factor = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("factor"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Level = 0;
					Factor = 1;
				}
				
			public:
				
				AdjustSaturation()
				{
					Initialize(c_nSerializeAdjustSaturation, c_strSerializeAdjustSaturation);

					Clear();

					bUseVideoCard = FALSE; //TRUE;
				}
				
			public:
				
				double Level; // -100..100
				double Factor; // 1..oo
			};
			class AdjustPosterize : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Levels);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Levels = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("levels"), Levels);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Levels = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("levels"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Levels = 8;
				}
				
			public:
				
				AdjustPosterize()
				{
					Initialize(c_nSerializeAdjustPosterize, c_strSerializeAdjustPosterize);

					Clear();
				}
					
			public:
				
				int Levels; // 2..255
			};
			class AdjustTemperature : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Level);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Level = pStream->ReadDouble();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), Level);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Level = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("level"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Level = 0;
				}
				
			public:
				
				AdjustTemperature()
				{
					Initialize(c_nSerializeAdjustTemperature, c_strSerializeAdjustTemperature);

					Clear();
				}
				
			public:
				
				double Level; // -100..100
			};
			class AdjustColorize : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 4*sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(LevelRed);
					pStream->WriteLong(LevelGreen);
					pStream->WriteLong(LevelBlue);
					pStream->WriteLong(Type);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					LevelRed   = pStream->ReadLong();
					LevelGreen = pStream->ReadLong();
					LevelBlue  = pStream->ReadLong();
					Type       = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("levelred"), LevelRed);
					oXmlWriter.WriteAttribute(_T("levelgreen"), LevelGreen);
					oXmlWriter.WriteAttribute(_T("levelblue"), LevelBlue);
					oXmlWriter.WriteAttribute(_T("type"), Type);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					LevelRed   = Strings::ToInteger(oXmlNode.GetAttribute(_T("levelred"), _T("0")));
					LevelGreen = Strings::ToInteger(oXmlNode.GetAttribute(_T("levelgreen"), _T("0")));
					LevelBlue  = Strings::ToInteger(oXmlNode.GetAttribute(_T("levelblue"), _T("0")));
					Type       = Strings::ToInteger(oXmlNode.GetAttribute(_T("type"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					LevelRed   = 0;
					LevelGreen = 0;
					LevelBlue  = 0;
					Type       = 0;
				}
				
			public:
				
				AdjustColorize()
				{
					Initialize(c_nSerializeAdjustColorize, c_strSerializeAdjustColorize);

					Clear();
				}
								
			public:
				
				int LevelRed;   // 0..255
				int LevelGreen; // 0..255
				int LevelBlue;  // 0..255
				int Type;       // 0 - standart, 1 - invert standart, 2, 3
			};
			class AdjustThreshold : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 2*sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Level);
					pStream->WriteLong((int)Grayscale);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Level = pStream->ReadLong();
					Grayscale = (BOOL)pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), Level);
					oXmlWriter.WriteAttribute(_T("grayscale"), (int)Grayscale);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Level = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("level"), _T("128")));
					Grayscale = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("grayscale"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Level = 128;
					Grayscale = FALSE;
				}
				
			public:
				
				AdjustThreshold()
				{
					Initialize(c_nSerializeAdjustThreshold, c_strSerializeAdjustThreshold);

					Clear();
				}
					
			public:
				
				int Level; // 0..255
				BOOL Grayscale; // Grayscale imae or apply in 3 channels separately
				
			};
			class AdjustSepia : public Core::Effect
			{
			protected:
				virtual int InternalGetRequiredStreamSize()
				{
					return 5 * sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Level);
					pStream->WriteLong(Darkness);
					pStream->WriteLong(RedLevel);
					pStream->WriteLong(GreenLevel);
					pStream->WriteLong(BlueLevel);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Level      = pStream->ReadLong();
					Darkness   = pStream->ReadLong();
					RedLevel   = pStream->ReadLong();
					GreenLevel = pStream->ReadLong();
					BlueLevel  = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), Level);
					oXmlWriter.WriteAttribute(_T("darkness"), Darkness);
					oXmlWriter.WriteAttribute(_T("redlevel"), RedLevel);
					oXmlWriter.WriteAttribute(_T("greenlevel"), GreenLevel);
					oXmlWriter.WriteAttribute(_T("bluelevel"), BlueLevel);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Level      = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("level"), _T("100")));
					Darkness   = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("darkness"), _T("20")));
					RedLevel   = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("redlevel"), _T("100")));
					GreenLevel = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("greenlevel"), _T("90")));
					BlueLevel  = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("bluelevel"), _T("85")));
				}
				
				virtual void InternalClear()
				{
					Level      = 0;
					Darkness   = 0;
					RedLevel   = 0;
					GreenLevel = 0;
					BlueLevel  = 0;
				}
			public:
				AdjustSepia()
				{
					Initialize(c_nSerializeAdjustSepia, c_strSerializeAdjustSepia);

					Clear();
				}

			public:
				int Level;      // 0..100 %
				int Darkness;   // 0..100 %
				int RedLevel;   // 0..100 %
				int GreenLevel; // 0..100 %
				int BlueLevel;  // 0..100 %
			};

			class AdjustWhiteBalance : public Core::Effect
			{
			protected:
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("red"), nRed);
					oXmlWriter.WriteAttribute(_T("green"), nGreen);
					oXmlWriter.WriteAttribute(_T("blue"), nBlue);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nRed   = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("red"), _T("255")));
					nGreen = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("green"), _T("255")));
					nBlue  = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("blue"), _T("255")));
				}
				
				virtual void InternalClear()
				{
					nRed   = 255;
					nGreen = 255;
					nBlue  = 255;
				}
			public:
				AdjustWhiteBalance()
				{
					Initialize(c_nSerializeAdjustWhiteBalance, c_strSerializeAdjustWhiteBalance);

					Clear();
				}

			public:
				int nRed;   // 0..255
				int nGreen; // 0..255
				int nBlue;  // 0..255
			};
			class AdjustAutoWhiteBalance : public Core::Effect
			{
			protected:
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
				}
				
				virtual void InternalClear()
				{
				}

			public:
				AdjustAutoWhiteBalance()
				{
					Initialize(c_nSerializeAdjustAutoWhiteBalance, c_strSerializeAdjustAutoWhiteBalance);

					Clear();
				}

			public:
				
			};
			class AdjustClarity : public Core::Effect
			{
			protected:
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), Level);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Level = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("level"), _T("0")));
					Radius = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("radius"), _T("0")));
					Threshold = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("threshold"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Level = 0;
					Radius = 0;
					Threshold = 0;
				}

			public:
				AdjustClarity()
				{
					Initialize(c_nSerializeAdjustClarity, c_strSerializeAdjustClarity);

					Clear();
				}


			public:
				double Level; // -100..100
				double Radius;
				double Threshold;
			};
			class AdjustVibrance : public Core::Effect
			{
			protected:
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), Level);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Level = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("level"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Level = 0;
				}

			public:
				AdjustVibrance()
				{
					Initialize(c_nSerializeAdjustVibrance, c_strSerializeAdjustVibrance);

					Clear();
				}

			public:
				double Level; // -100..100
			};


			
			class AdjustRecovery : public Core::Effect
			{
			protected:
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(nLevel);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					nLevel = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), nLevel);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nLevel = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("level"), _T("100")));
				}
				
				virtual void InternalClear()
				{
					nLevel = 100;
				}
			public:
				AdjustRecovery()
				{
					Initialize(c_nSerializeAdjustRecovery, c_strSerializeAdjustRecovery);

					Clear();
				}

			public:
				int nLevel;
			};
			class AdjustFillLight : public Core::Effect
			{
			protected:
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(nLevel);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					nLevel = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), nLevel);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nLevel = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("level"), _T("100")));
				}
				
				virtual void InternalClear()
				{
					nLevel = 100;
				}
			public:
				AdjustFillLight()
				{
					Initialize(c_nSerializeAdjustFillLight, c_strSerializeAdjustFillLight);

					Clear();
				}

			public:
				int nLevel;
			};
			class AdjustExposure : public Core::Effect
			{
			protected:
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(nLevel);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					nLevel = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), nLevel);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nLevel = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("level"), _T("100")));
				}
				
				virtual void InternalClear()
				{
					nLevel = 100;
				}
			public:
				AdjustExposure()
				{
					Initialize(c_nSerializeAdjustExposure, c_strSerializeAdjustExposure);

					Clear();
				}

			public:
				int nLevel;
			};
			class AdjustBlacks : public Core::Effect
			{
			protected:
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(nLevel);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					nLevel = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), nLevel);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nLevel = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("level"), _T("100")));
				}
				
				virtual void InternalClear()
				{
					nLevel = 100;
				}
			public:
				AdjustBlacks()
				{
					Initialize(c_nSerializeAdjustBlacks, c_strSerializeAdjustBlacks);

					Clear();
				}

			public:
				int nLevel;
			};
			class AdjustHighLight : public Core::Effect
			{
			protected:
				virtual int InternalGetRequiredStreamSize()
				{
					return 2 * sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(nFillLight);
					pStream->WriteLong(nExposure);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					nFillLight = pStream->ReadLong();
					nExposure = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("filllight"), nFillLight);
					oXmlWriter.WriteAttribute(_T("exposure"), nExposure);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nFillLight = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("filllight"), _T("100")));
					nExposure = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("exposure"), _T("100")));
				}
				
				virtual void InternalClear()
				{
					nFillLight = 100;
					nExposure = 100;
				}
			public:
				AdjustHighLight()
				{
					Initialize(c_nSerializeAdjustHighLight, c_strSerializeAdjustHighLight);

					Clear();
				}

			public:
				int nFillLight;
				int nExposure;
			};
			class AdjustShadows : public Core::Effect
			{
			protected:
				virtual int InternalGetRequiredStreamSize()
				{
					return 2 * sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(nBlacks);
					pStream->WriteLong(nRecovery);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					nBlacks = pStream->ReadLong();
					nRecovery = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("blacks"), nBlacks);
					oXmlWriter.WriteAttribute(_T("recovery"), nRecovery);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nBlacks = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("blacks"), _T("100")));
					nRecovery = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("recovery"), _T("100")));
				}
				
				virtual void InternalClear()
				{
					nBlacks = 100;
					nRecovery = 0;
				}
			public:
				AdjustShadows()
				{
					Initialize(c_nSerializeAdjustShadows, c_strSerializeAdjustShadows);

					Clear();
				}

			public:
				int nBlacks;
				int nRecovery;
			};
			
			class EffectBlur : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double) + sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Level);
					pStream->WriteLong(Metric);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Level = pStream->ReadDouble();
					Metric = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), Level);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Level = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("level"), _T("0")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Level = 0;
					Metric = 0;
				}
				
			public:
				
				EffectBlur()
				{
					Initialize(c_nSerializeEffectBlur, c_strSerializeEffectBlur);

					Clear();

					bUseVideoCard = TRUE;
				}
				
			public:
				
				double Level; // 0..100
				int Metric;   // 0, 1, 2
			};
			class EffectSharpen : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double) + sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Level);
					pStream->WriteLong(Metric);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Level = pStream->ReadDouble();
					Metric = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), Level);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Level = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("level"), _T("0")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Level = 0;
					Metric = 0;
				}
				
			public:
				
				EffectSharpen()
				{
					Initialize(c_nSerializeEffectSharpen, c_strSerializeEffectSharpen);

					Clear();

					bUseVideoCard = TRUE;
				}
				
			public:
				
				double Level; // 0..100
				int Metric;
			};
			class EffectMosaic : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double) + 2*sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Size);
					pStream->WriteLong((int)Simple);
					pStream->WriteLong(Metric);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Size = pStream->ReadDouble();
					Simple = (BOOL)pStream->ReadLong();
					Metric = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("size"), Size);
					oXmlWriter.WriteAttribute(_T("simple"), (int)Simple);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Size = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("size"), _T("0")));
					Simple = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("simple"), _T("0")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Size = 0;
					Simple = FALSE;
					Metric = 0;
				}
				
			public:
				
				EffectMosaic()
				{
					Initialize(c_nSerializeEffectMosaic, c_strSerializeEffectMosaic);

					Clear();
				}
					
			public:
				
				double Size; // 1..255, 0..100, 0..1
				BOOL Simple; // simply get top-left-pixel color or compute average color for the quad
				int Metric;
				
			};
			class EffectGaussianBlur : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double) + sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Size);
					pStream->WriteLong(Metric);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Size = pStream->ReadDouble();
					Metric = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("size"), Size);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Size = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("size"), _T("0")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Size = 0;
					Metric = 0;
				}
				
			public:
				
				EffectGaussianBlur()
				{
					Initialize(c_nSerializeEffectGaussianBlur, c_strSerializeEffectGaussianBlur);

					Clear();

					bUseVideoCard = TRUE;
				}
				
			public:
				
				double Size; // 1..255, 0..100, 0..1
				int Metric;
			};
			class EffectNoise : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double) + 2*sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Level);
					pStream->WriteLong((int)Monochrome);
					pStream->WriteLong(Metric);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Level = pStream->ReadDouble();
					Monochrome = (BOOL)pStream->ReadLong();
					Metric = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), Level);
					oXmlWriter.WriteAttribute(_T("monochrome"), (int)Monochrome);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Level = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("level"), _T("0")));
					Monochrome = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("monochrome"), _T("0")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Level = 0;
					Monochrome = FALSE;
					Metric = 0;
				}
				
			public:
				
				EffectNoise()
				{
					Initialize(c_nSerializeEffectNoise, c_strSerializeEffectNoise);

					Clear();
				}
					
			public:
				
				double Level; // 1..1000, 0..100, 0..1
				BOOL Monochrome; // grayscale (1) or colored noise (0)
				int Metric;
			};
			class EffectDiffuse : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double) + sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Distance);
					pStream->WriteLong(Metric);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Distance = pStream->ReadDouble();
					Metric = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("distance"), Distance);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{

					Distance = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("distance"), _T("0")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Distance = 0;
					Metric = 0;
				}
				
			public:
				
				EffectDiffuse()
				{
					Initialize(c_nSerializeEffectDiffuse, c_strSerializeEffectDiffuse);

					Clear();
				}
					
			public:
				
				double Distance; // 1..255, 0..100, 0..1
				int Metric;
			};
			class EffectMotionBlur : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 2*sizeof(double) + sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Angle);
					pStream->WriteDouble(Distance);
					pStream->WriteLong(Metric);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Angle = pStream->ReadDouble();
					Distance = pStream->ReadDouble();
					Metric = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("angle"), Angle);
					oXmlWriter.WriteAttribute(_T("distance"), Distance);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Angle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));
					Distance = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("distance"), _T("0")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Angle = 0;
					Distance = 0;
					Metric = 0;
				}
				
			public:
				
				EffectMotionBlur()
				{
					Initialize(c_nSerializeEffectMotionBlur, c_strSerializeEffectMotionBlur);

					Clear();

					bUseVideoCard = TRUE;
				}
					
			public:
				
				double Angle; // -360..360	
				double Distance; // 1..255, 0..100, 0..1
				int Metric;
			};
			class EffectEmboss : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 3*sizeof(double) + sizeof(int)*2;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Angle);
					pStream->WriteDouble(Distance);
					pStream->WriteDouble(Amount);
					pStream->WriteLong(Metric);
					pStream->WriteLong(bGrayScale);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Angle = pStream->ReadDouble();
					Distance = pStream->ReadDouble();
					Amount = pStream->ReadDouble();
					Metric = pStream->ReadLong();
					bGrayScale = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("angle"), Angle);
					oXmlWriter.WriteAttribute(_T("distance"), Distance);
					oXmlWriter.WriteAttribute(_T("amount"), Amount);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
					oXmlWriter.WriteAttribute(_T("grayscale"), bGrayScale);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{

					Angle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));
					Distance = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("distance"), _T("0")));
					Amount = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("amount"), _T("0")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					bGrayScale = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("grayscale"), _T("1")));

				}
				
				virtual void InternalClear()
				{
					Angle = 0;
					Distance = 0;
					Amount = 0;
					Metric = 0;
					bGrayScale = FALSE;
				}
				
			public:
				
				EffectEmboss()
				{
					Initialize(c_nSerializeEffectEmboss, c_strSerializeEffectEmboss);

					Clear();
				}
					
			public:
				
				double Angle;    // -360..360	
				double Distance; // -oo..+oo, ..0..100.., ..0..1.. max = sqrt( cos(a)*cos(a)*w*w + sin(a)*sin(a)*h*h )
				double Amount;   // 1..100
				int  Metric;
				BOOL bGrayScale;
			};
			class EffectMinimal : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double) + sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Size);
					pStream->WriteLong(Metric);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Size = pStream->ReadDouble();
					Metric = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("size"), Size);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Size = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("size"), _T("0")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Size = 0;
					Metric = 0;
				}
				
			public:
				
				EffectMinimal()
				{
					Initialize(c_nSerializeEffectMinimal, c_strSerializeEffectMinimal);

					Clear();

					bUseVideoCard = TRUE;
				}
					
			public:
				
				double Size; // 1..255, 0..100, 0..1
				int Metric;
			};
			
			class EffectKenBurns : public Core::Effect
			{
			protected:

				struct CurveCubicBezier
				{
					struct Point2D
					{
						Point2D () : m_nX ( 0.0 ), m_nY ( 0.0 )
						{ 

						}

						Point2D ( double nX, double nY ) : m_nX ( nX ), m_nY ( nY )
						{ 

						}

						double m_nX;
						double m_nY;
					};

					inline Point2D GetValue ( double fU )
					{
						Point2D point;

						//	
						//	Add up all the blending functions multiplied with the control points
						//
						float fBlend = 0.0;
						float f1subu = 1.0f - (float)fU;

						//	
						//	First blending function (1-u)^3
						//	
						fBlend = f1subu * f1subu * f1subu;
						point.m_nX = fBlend * m_Points[0].m_nX;
						point.m_nY = fBlend * m_Points[0].m_nY;

						//	
						//	Second blending function 3u(1-u)^2
						//
						fBlend = (float)(3 * fU * f1subu * f1subu);
						point.m_nX += fBlend * m_Points[1].m_nX;
						point.m_nY += fBlend * m_Points[1].m_nY;

						//	
						//	Third blending function 3u^2 * (1-u)
						//
						fBlend = (float)(3 * fU * fU * f1subu);
						point.m_nX += fBlend * m_Points[2].m_nX;
						point.m_nY += fBlend * m_Points[2].m_nY;

						//	
						//	Fourth blending function u^3
						//	
						fBlend = (float)(fU * fU * fU);
						point.m_nX += fBlend * m_Points[3].m_nX;
						point.m_nY += fBlend * m_Points[3].m_nY;

						return point;
					}


					Point2D	m_Points[4];
				};

			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 0;
				}
				
				virtual void InternalToStream(Streams::IStream* pStream)
				{
				
				}
				
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
				
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
				
				}
				
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Metric			=	Strings::ToInteger ( oXmlNode.GetAttributeOrValue ( _T("metric"), _T("0") ) );
					
					m_nSlowFC		=	(double)_tstof ( oXmlNode.GetAttributeOrValue ( _T("slowfc"),	_T("0.0") ) );

					m_oTimeCurve.m_Points[0].m_nX	=	0.0;
					m_oTimeCurve.m_Points[0].m_nY	=	0.0;
					
					m_oTimeCurve.m_Points[1].m_nX	=	m_nSlowFC;
					m_oTimeCurve.m_Points[1].m_nY	=	0.0;

					m_oTimeCurve.m_Points[2].m_nX	=	1.0 - m_nSlowFC;
					m_oTimeCurve.m_Points[2].m_nY	=	1.0;
					
					m_oTimeCurve.m_Points[3].m_nX	=	1.0;
					m_oTimeCurve.m_Points[3].m_nY	=	1.0;


					// add begin in rect point
					m_Regions.Add ( Gdiplus::RectF ( 0.0f, 0.0f, 1.0f, 1.0f ) );

					Gdiplus::RectF region;
					
					// начало
					region.X		=	(float)_tstof ( oXmlNode.GetAttributeOrValue ( _T("left1"),		_T("0.0") ) );
					region.Y		=	(float)_tstof ( oXmlNode.GetAttributeOrValue ( _T("top1"),		_T("0.0") ) );
					region.Width	=	(float)_tstof ( oXmlNode.GetAttributeOrValue ( _T("right1"),	_T("0.0") ) ) - region.X;
					region.Height	=	(float)_tstof ( oXmlNode.GetAttributeOrValue ( _T("bottom1"),	_T("0.0") ) ) - region.Y;

					m_Regions.Add ( region );
					
					// конец
					region.X		=	(float)_tstof ( oXmlNode.GetAttributeOrValue ( _T("left2"),		_T("0.0") ) );
					region.Y		=	(float)_tstof ( oXmlNode.GetAttributeOrValue ( _T("top2"),		_T("0.0") ) );
					region.Width	=	(float)_tstof ( oXmlNode.GetAttributeOrValue ( _T("right2"),	_T("0.0") ) ) - region.X;
					region.Height	=	(float)_tstof ( oXmlNode.GetAttributeOrValue ( _T("bottom2"),	_T("0.0") ) ) - region.Y;
					
					m_Regions.Add ( region );
					
					// add end in rect point
					m_Regions.Add ( Gdiplus::RectF ( 0.0f, 0.0f, 1.0f, 1.0f ) );
				}

				virtual void InternalClear()
				{
					Metric = 0;
					m_Regions.RemoveAll ();
				}

			public:
				
				EffectKenBurns()
				{
					Initialize(c_nSerializeEffectKenBurns, c_strSerializeEffectKenBurns);

					Clear();
				}
				
				inline BOOL GetTransform ( double Stage, Gdiplus::RectF& TransformRegion )
				{
					if ( m_Regions.GetSize () < 4 || Stage < 0.0 || Stage > 3.0 )
					{
						return FALSE;
					}
					
					if ( 0.0 == Stage )
					{
						TransformRegion			=	m_Regions [ 0 ];
						
						return TRUE;
					}

					if ( 1.0 == Stage )
					{
						TransformRegion			=	m_Regions [ 1 ];
						
						return TRUE;
					}
				
					if ( 2.0 == Stage )
					{
						TransformRegion			=	m_Regions [ 2 ];
						
						return TRUE;
					}
					
					if ( 3.0 == Stage )
					{
						TransformRegion			=	m_Regions [ 3 ];
						
						return TRUE;
					}

					if ( 0.0 < Stage && Stage < 1.0 )
					{
						TransformRegion.X		=	(float)( m_Regions [ 0 ].X		* ( 1.0 - Stage )	+ m_Regions [ 1 ].X			* Stage );
						TransformRegion.Y		=	(float)( m_Regions [ 0 ].Y		* ( 1.0 - Stage )	+ m_Regions [ 1 ].Y			* Stage );
						TransformRegion.Width	=	(float)( m_Regions [ 0 ].Width	* ( 1.0 - Stage )	+ m_Regions [ 1 ].Width		* Stage );
						TransformRegion.Height	=	(float)( m_Regions [ 0 ].Height * ( 1.0 - Stage )	+ m_Regions [ 1 ].Height	* Stage );
					
						return TRUE;
					}

					if ( 1.0 < Stage && Stage < 2.0 )
					{
						Stage					=	Stage - 1.0;

						if ( 0.0 != m_nSlowFC )
						{
							Stage					=	m_oTimeCurve.GetValue ( Stage ).m_nY;
						}

						TransformRegion.X		=	(float)( m_Regions [ 1 ].X		* ( 1.0 - Stage )	+ m_Regions [ 2 ].X			* Stage );
						TransformRegion.Y		=	(float)( m_Regions [ 1 ].Y		* ( 1.0 - Stage )	+ m_Regions [ 2 ].Y			* Stage );
						TransformRegion.Width	=	(float)( m_Regions [ 1 ].Width	* ( 1.0 - Stage )	+ m_Regions [ 2 ].Width		* Stage );
						TransformRegion.Height	=	(float)( m_Regions [ 1 ].Height * ( 1.0 - Stage )	+ m_Regions [ 2 ].Height	* Stage );


						return TRUE;
					}


					if ( 2.0 < Stage && Stage < 3.0 )
					{
						Stage					=	Stage - 2.0;

						TransformRegion.X		=	(float)( m_Regions [ 2 ].X		* ( 1.0 - Stage )	+ m_Regions [ 3 ].X			* Stage );
						TransformRegion.Y		=	(float)( m_Regions [ 2 ].Y		* ( 1.0 - Stage )	+ m_Regions [ 3 ].Y			* Stage );
						TransformRegion.Width	=	(float)( m_Regions [ 2 ].Width	* ( 1.0 - Stage )	+ m_Regions [ 3 ].Width		* Stage );
						TransformRegion.Height	=	(float)( m_Regions [ 2 ].Height * ( 1.0 - Stage )	+ m_Regions [ 3 ].Height	* Stage );
					
						return TRUE;
					}
					
					return FALSE;
				}
					
			public:
				
				int								Metric;

				double							m_nSlowFC;		//	

				CSimpleArray < Gdiplus::RectF >	m_Regions;

				CurveCubicBezier				m_oTimeCurve;
			};

			class EffectMaximal : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double) + sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Size);
					pStream->WriteLong(Metric);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Size = pStream->ReadDouble();
					Metric = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("size"), Size);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Size = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("size"), _T("0")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Size = 0;
					Metric = 0;
				}
				
			public:
				
				EffectMaximal()
				{
					Initialize(c_nSerializeEffectMaximal, c_strSerializeEffectMaximal);

					Clear();

					bUseVideoCard = TRUE;
				}
					
			public:
				
				double Size; // 1..255, 0..100, 0..1
				int Metric;
			};
			class EffectDeinterlace : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 2*sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong((int)Even);
					pStream->WriteLong((int)Interpolate);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Even = (BOOL)pStream->ReadLong();
					Interpolate = (BOOL)pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("even"), (int)Even);
					oXmlWriter.WriteAttribute(_T("interpolate"), (int)Interpolate);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Even = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("even"), _T("1")));
					Interpolate = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("interpolate"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Even = TRUE;
					Interpolate = FALSE;
				}
				
			public:
				
				EffectDeinterlace()
				{
					Initialize(c_nSerializeEffectDeinterlace, c_strSerializeEffectDeinterlace);

					Clear();
				}
					
			public:
				
				BOOL Even; // correct even (1) or odd (0) fields in the image
				BOOL Interpolate; // interpolate (1) pixels or copy (0) from previous line
				
			};
			class EffectMedian : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Type);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Type = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("type"), Type);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Type = 1;
				}
				
			public:
				
				EffectMedian()
				{
					Initialize(c_nSerializeEffectMedian, c_strSerializeEffectMedian);

					Clear();
				}
				
			public:
				
				int Type; // c_nMedian* values or 1...N (where N is the size of the median square)
			};
			class EffectChromoKeyRange : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 7*sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(m_nIndex);
					pStream->WriteLong(m_nColor1);
					pStream->WriteLong(m_nThreshold1);
					pStream->WriteLong(m_nColor2);
					pStream->WriteLong(m_nThreshold2);
					pStream->WriteLong(m_nComposeAlpha);
					pStream->WriteLong(m_nEdgesSmooth);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					m_nIndex = pStream->ReadLong();
					m_nColor1 = pStream->ReadLong();
					m_nThreshold1 = pStream->ReadLong();
					m_nColor2 = pStream->ReadLong();
					m_nThreshold2 = pStream->ReadLong();
					m_nComposeAlpha = pStream->ReadLong();
					m_nEdgesSmooth = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("index"), m_nIndex);
					oXmlWriter.WriteAttribute(_T("color1"), m_nColor1);
					oXmlWriter.WriteAttribute(_T("threshold1"), m_nThreshold1);
					oXmlWriter.WriteAttribute(_T("color2"), m_nColor2);
					oXmlWriter.WriteAttribute(_T("threshold2"), m_nThreshold2);
					oXmlWriter.WriteAttribute(_T("composealpha"), m_nComposeAlpha);
					oXmlWriter.WriteAttribute(_T("edgessmooth"), m_nEdgesSmooth);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					m_nIndex = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("index"), _T("0")));
					m_nColor1 = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("color1"), _T("0")));
					m_nThreshold1 = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("threshold1"), _T("16")));
					m_nColor2 = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("color2"), _T("0")));
					m_nThreshold2 = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("threshold2"), _T("16")));
					m_nComposeAlpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("composealpha"), _T("0")));
					m_nEdgesSmooth = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("edgessmooth"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					m_nIndex = 0;
					m_nColor1 = 0;
					m_nThreshold1 = 16;
					m_nColor2 = 0;
					m_nThreshold2 = 16;
					m_nComposeAlpha = 0;
					m_nEdgesSmooth	=	0;
				}
				
			public:
				
				EffectChromoKeyRange()
				{
					Initialize(c_nSerializeEffectChromoKeyRange, c_strSerializeEffectChromoKeyRange);

					Clear();
				}
				
				void SetBufferIndex(int nIndex)
				{
					m_nIndex = nIndex;
				}
				int GetBufferIndex() const
				{
					return m_nIndex;
				}
				void SetColor1(int nColor)
				{
					m_nColor1 = nColor;
				}
				int GetColor1() const
				{
					return m_nColor1;
				}
				void SetColor2(int nColor)
				{
					m_nColor2 = nColor;
				}
				int GetColor2() const
				{
					return m_nColor2;
				}
				void SetThreshold1(int nThreshold)
				{
					m_nThreshold1 = nThreshold;
				}
				int GetThreshold1() const
				{
					return m_nThreshold1;
				}
				void SetThreshold2(int nThreshold)
				{
					m_nThreshold2 = nThreshold;
				}
				int GetThreshold2() const
				{
					return m_nThreshold2;
				}
				void SetComposeAlpha(int nComposeAlpha)
				{
					m_nComposeAlpha = nComposeAlpha;
				}
				int GetComposeAlpha() const
				{
					return m_nComposeAlpha;
				}
				int GetEdgesSmooth () const
				{
					return m_nEdgesSmooth;
				}
				
			protected:
				
				int m_nIndex;
				int m_nColor1;
				int m_nThreshold1;
				int m_nColor2;
				int m_nThreshold2;
				int m_nComposeAlpha;
				int m_nEdgesSmooth;
			};
			class EffectChromoKeyLevel : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 5*sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(m_nIndex);
					pStream->WriteLong(m_nColor);
					pStream->WriteLong(m_nThreshold);
					pStream->WriteLong(m_nComposeAlpha);
					pStream->WriteLong(m_nEdgesSmooth);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					m_nIndex = pStream->ReadLong();
					m_nColor = pStream->ReadLong();
					m_nThreshold = pStream->ReadLong();
					m_nComposeAlpha = pStream->ReadLong();
					m_nEdgesSmooth = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("index"), m_nIndex);
					oXmlWriter.WriteAttribute(_T("color"), m_nColor);
					oXmlWriter.WriteAttribute(_T("threshold"), m_nThreshold);
					oXmlWriter.WriteAttribute(_T("composealpha"), m_nComposeAlpha);
					oXmlWriter.WriteAttribute(_T("edgessmooth"), m_nEdgesSmooth);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					m_nIndex = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("index"), _T("0")));
					m_nColor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("color"), _T("0")));
					m_nThreshold = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("threshold"), _T("0")));
					m_nComposeAlpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("composealpha"), _T("0")));
					m_nEdgesSmooth = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("edgessmooth"), _T("0")));

					
					
					arrColors.RemoveAll();

					CString sData = oXmlNode.GetAttributeOrValue(_T("color"), _T("0"));

					TCHAR* pText = sData.LockBuffer();
					TCHAR* pStart = pText;
					BOOL bBadPtr = FALSE;

					int nStrLen = sData.GetLength();
					for( int i = 0; i < nStrLen; ++i )
					{
						if( pText[i] != _T(' ') )
						{
							if( bBadPtr )
							{
								pStart = &pText[i];
								bBadPtr = FALSE;
							}
							continue;
						}

						pText[i] = _T('\0');

						if( bBadPtr )
							continue;

						arrColors.Add( ::_ttoi( pStart ) );

						bBadPtr = TRUE;
					}

					if( !bBadPtr )
					{
						arrColors.Add( ::_ttoi( pStart ) );
					}

					sData.UnlockBuffer();
				}
				
				virtual void InternalClear()
				{
					m_nIndex = 0;
					m_nColor = 0;
					m_nThreshold = 0;
					m_nComposeAlpha = 0;
					m_nEdgesSmooth	=	0;

					arrColors.RemoveAll();
				}
				
			public:
				
				EffectChromoKeyLevel()
				{
					Initialize(c_nSerializeEffectChromoKeyLevel, c_strSerializeEffectChromoKeyLevel);

					Clear();
				}
				
				void SetBufferIndex(int nIndex)
				{
					m_nIndex = nIndex;
				}
				int GetBufferIndex() const
				{
					return m_nIndex;
				}
				void SetColor(int nColor)
				{
					m_nColor = nColor;
				}
				int GetColor() const
				{
					return m_nColor;
				}
				void SetThreshold(int nThreshold)
				{
					m_nThreshold = nThreshold;
				}
				int GetThreshold() const
				{
					return m_nThreshold;
				}
				void SetComposeAlpha(int nComposeAlpha)
				{
					m_nComposeAlpha = nComposeAlpha;
				}
				int GetComposeAlpha() const
				{
					return m_nComposeAlpha;
				}
				int GetEdgesSmooth () const
				{
					return m_nEdgesSmooth;
				}

				CSimpleArray<int> arrColors;

			protected:
				
				int m_nIndex;
				int m_nColor;
				int m_nThreshold; // threshold of the chromo keying (0..100)
				int m_nComposeAlpha;
				int m_nEdgesSmooth;
			};
			class EffectCinema : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 0; //sizeof(BOOL)*2 + sizeof(int)*20;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					//pStream->WriteLong( (int)bGrayscale );

					//pStream->WriteLong( nNoiseLevel );
					//pStream->WriteLong( (int)bNoiseMonochrome );
					//
					//pStream->WriteLong( nMinShowLines );
					//pStream->WriteLong( nMaxShowLines );
					//pStream->WriteLong( nNumFramesForShowLines );
					//pStream->WriteLong( nMinLineSizeInPercent );
					//pStream->WriteLong( nMaxLineSizeInPercent );
					//pStream->WriteLong( nMinLineWidthInPixels );
					//pStream->WriteLong( nMaxLineWidthInPixels );
					//pStream->WriteLong( nMinLineAlpha );
					//pStream->WriteLong( nMaxLineAlpha );
					//pStream->WriteLong( nLineColor );

					//pStream->WriteLong( nMinShowArtifacts );
					//pStream->WriteLong( nMaxShowArtifacts );
					//pStream->WriteLong( nNumFramesForShowArtifacts );
					//pStream->WriteLong( nArtifactSizeInPixels );
					//pStream->WriteLong( nMinArtifactAlpha );
					//pStream->WriteLong( nMaxArtifactAlpha );
					//pStream->WriteLong( nArtifactColor );

					//pStream->WriteLong( nNumFramesForShowShift );
					//pStream->WriteLong( nMaxShiftSize );
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					//bGrayscale             = (BOOL)pStream->ReadLong();

					//nNoiseLevel            = pStream->ReadLong();
					//bNoiseMonochrome       = (BOOL)pStream->ReadLong();
				
					//nMinShowLines          = pStream->ReadLong();
					//nMaxShowLines          = pStream->ReadLong();
					//nNumFramesForShowLines = pStream->ReadLong();
					//nMinLineSizeInPercent  = pStream->ReadLong();
					//nMaxLineSizeInPercent  = pStream->ReadLong();
					//nMinLineWidthInPixels  = pStream->ReadLong();
					//nMaxLineWidthInPixels  = pStream->ReadLong();
					//nMinLineAlpha          = pStream->ReadLong();
					//nMaxLineAlpha          = pStream->ReadLong();
					//nLineColor             = pStream->ReadLong();

					//nMinShowArtifacts      = pStream->ReadLong();
					//nMaxShowArtifacts      = pStream->ReadLong();
					//nNumFramesForShowArtifacts = pStream->ReadLong();
					//nArtifactSizeInPixels  = pStream->ReadLong();
					//nMinArtifactAlpha      = pStream->ReadLong();
					//nMaxArtifactAlpha      = pStream->ReadLong();
					//nArtifactColor         = pStream->ReadLong();

					//nNumFramesForShowShift = pStream->ReadLong();
					//nMaxShiftSize          = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					//oXmlWriter.WriteAttribute( _T("grayscale"), (int)bGrayscale );

					//oXmlWriter.WriteAttribute( _T("noise-level"), nNoiseLevel );
					//oXmlWriter.WriteAttribute( _T("noise-monochrome"), (int)bNoiseMonochrome );
				
					oXmlWriter.WriteAttribute( _T("lines-minshow"), m_nMinShowLines );
					oXmlWriter.WriteAttribute( _T("lines-maxshow"), m_nMaxShowLines );
					oXmlWriter.WriteAttribute( _T("lines-framesforshow"), m_nNumFramesForShowLines );
					
					//oXmlWriter.WriteAttribute( _T("lines-minsizeinpercent"), nMinLineSizeInPercent );
					//oXmlWriter.WriteAttribute( _T("lines-maxsizeinpercent"), nMaxLineSizeInPercent );
					//oXmlWriter.WriteAttribute( _T("lines-minwidthinpixels"), nMinLineWidthInPixels );
					//oXmlWriter.WriteAttribute( _T("lines-maxwidthinpixels"), nMaxLineWidthInPixels );
					//oXmlWriter.WriteAttribute( _T("lines-minalpha"), nMinLineAlpha );
					//oXmlWriter.WriteAttribute( _T("lines-maxalpha"), nMaxLineAlpha );
					//oXmlWriter.WriteAttribute( _T("lines-color"), nLineColor );

					oXmlWriter.WriteAttribute( _T("artifacts-minshow"), m_nMinShowArtifacts );
					oXmlWriter.WriteAttribute( _T("artifacts-maxshow"), m_nMaxShowArtifacts );
					
					//oXmlWriter.WriteAttribute( _T("artifacts-framesforshow"), nNumFramesForShowArtifacts );
					//oXmlWriter.WriteAttribute( _T("artifacts-sizeinpixels"), nArtifactSizeInPixels );
					//oXmlWriter.WriteAttribute( _T("artifacts-minalpha"), nMinArtifactAlpha );
					//oXmlWriter.WriteAttribute( _T("artifacts-maxalpha"), nMaxArtifactAlpha );
					//oXmlWriter.WriteAttribute( _T("artifacts-color"), nArtifactColor );

					oXmlWriter.WriteAttribute( _T("shift-offset-maxx"),  m_dShiftMaxOffsetX);
					oXmlWriter.WriteAttribute( _T("shift-offset-maxy"),  m_dShiftMaxOffsetY);
					oXmlWriter.WriteAttribute( _T("shift-offset-factorx"),  m_dShiftFactorOffsetX);
					oXmlWriter.WriteAttribute( _T("shift-offset-factory"),  m_dShiftFactorOffsetY);
					//oXmlWriter.WriteAttribute( _T("shift-framesforshow"), nNumFramesForShowShift );
					//oXmlWriter.WriteAttribute( _T("shift-sizeinpixels"), nMaxShiftSize );

					//oXmlWriter.WriteAttribute( _T("artifacts-file"), sArtefactFile );

					oXmlWriter.WriteAttribute(_T("brightness"), m_nBrightness);
					oXmlWriter.WriteAttribute(_T("contrast"), m_nContrast);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					//nMetric  = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("metric"), _T("2")) );


					//bGrayscale             = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("grayscale"), _T("1")) );

					//nNoiseLevel            = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("noise-level"), _T("15")) );
					//bNoiseMonochrome       = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("noise-monochrome"), _T("1")) );
				
					m_nMinShowLines          = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("lines-minshow"), _T("1")) );
					m_nMaxShowLines          = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("lines-maxshow"), _T("3")) );
					m_nNumFramesForShowLines = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("lines-framesforshow"), _T("20")) );
					//nMinLineSizeInPercent  = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("lines-minsizeinpercent"), _T("10")) );
					//nMaxLineSizeInPercent  = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("lines-maxsizeinpercent"), _T("120")) );
					//nMinLineWidthInPixels  = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("lines-minwidthinpixels"), _T("1")) );
					//nMaxLineWidthInPixels  = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("lines-maxwidthinpixels"), _T("2")) );
					//nMinLineAlpha          = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("lines-minalpha"), _T("50")) );
					//nMaxLineAlpha          = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("lines-maxalpha"), _T("180")) );
					//nLineColor             = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("lines-color"), _T("8421504")) );

					m_nMinShowArtifacts      = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("artifacts-minshow"), _T("3")) );
					m_nMaxShowArtifacts      = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("artifacts-maxshow"), _T("5")) );
					//nNumFramesForShowArtifacts = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("artifacts-framesforshow"), _T("10")) );
					//nArtifactSizeInPixels  = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("artifacts-sizeinpixels"), _T("5")) );
					//nMinArtifactAlpha      = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("artifacts-minalpha"), _T("50")) );
					//nMaxArtifactAlpha      = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("artifacts-maxalpha"), _T("255")) );
					//nArtifactColor         = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("artifacts-color"), _T("8421504")) );

					//dShiftTimePeriod = Strings::ToDouble( oXmlNode.GetAttribute( _T("shift-timeinterval"), _T("6")) );
					m_dShiftMaxOffsetX = Strings::ToDouble( oXmlNode.GetAttribute( _T("shift-offset-maxx"), _T("5")) );
					m_dShiftMaxOffsetY = Strings::ToDouble( oXmlNode.GetAttribute( _T("shift-offset-maxy"), _T("5")) );
					m_dShiftFactorOffsetX = Strings::ToDouble( oXmlNode.GetAttribute( _T("shift-offset-factorx"), _T("1.0")) );
					m_dShiftFactorOffsetY = Strings::ToDouble( oXmlNode.GetAttribute( _T("shift-offset-factory"), _T("1.5")) );

					//sArtefactFile = oXmlNode.GetAttributeOrValue( _T("artifacts-file"), _T(""));

					m_nBrightness = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("brightness"), _T("90")));
					m_nContrast   = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("contrast"), _T("10")));

				}
				
				virtual void InternalClear()
				{
					//bGrayscale = TRUE;

					//nNoiseLevel = 15;
					//bNoiseMonochrome = TRUE;
				
					m_nMinShowLines = 1;
					m_nMaxShowLines = 3;
					m_nNumFramesForShowLines = 20;
					//nMinLineSizeInPercent = 10;
					//nMaxLineSizeInPercent = 100;
					//nMinLineWidthInPixels = 1;
					//nMaxLineWidthInPixels = 2;
					//nMinLineAlpha = 0;
					//nMaxLineAlpha = 180;
					//nLineColor = Color::Gray;

					m_nMinShowArtifacts = 1;
					m_nMaxShowArtifacts = 3;
					//nNumFramesForShowArtifacts = 12;
					//nArtifactSizeInPixels = 5;
					//nMinArtifactAlpha = 0;
					//nMaxArtifactAlpha = 255;
					//nArtifactColor = Color::Gray;

					//dShiftTimePeriod = 0;
					m_dShiftMaxOffsetX = 0;
					m_dShiftMaxOffsetY = 0;
					m_dShiftFactorOffsetX = 1.0;
					m_dShiftFactorOffsetY = 1.5;

					//sArtefactFile = "";

					m_nBrightness = 0;
					m_nContrast = 0;
				}

			public:
				
				EffectCinema()
				{
					Initialize(c_nSerializeEffectCinema, c_strSerializeEffectCinema);

					Clear();
				}
					
			public:
				//int    nMetric;
				
				BOOL   bGrayscale;

				int    nNoiseLevel; // 0..1000
				BOOL   bNoiseMonochrome; // grayscale (1) or colored noise (0)
				
				int    m_nMinShowLines;
				int    m_nMaxShowLines;
				int    m_nNumFramesForShowLines;
				//int    nMinLineSizeInPercent;
				//int    nMaxLineSizeInPercent;
				//int    nMinLineWidthInPixels;
				//int    nMaxLineWidthInPixels;
				//int    nMinLineAlpha;
				//int    nMaxLineAlpha;
				//int    nLineColor;

				int    m_nMinShowArtifacts;
				int    m_nMaxShowArtifacts;
				//int    nNumFramesForShowArtifacts;
				//int    nArtifactSizeInPixels;
				//int    nMinArtifactAlpha;
				//int    nMaxArtifactAlpha;
				//int    nArtifactColor;

				//double dShiftTimePeriod;
				double m_dShiftMaxOffsetX;
				double m_dShiftMaxOffsetY;
				double m_dShiftFactorOffsetX;
				double m_dShiftFactorOffsetY;

				//CString sArtefactFile;

				int    m_nBrightness; // -100..100
				int    m_nContrast;   // -100..100
			};
			class EffectGlass : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Radius);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Radius = pStream->ReadLong();
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("radius"), Radius);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Radius = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("radius"), _T("0")));
				}

				virtual void InternalClear()
				{
					Radius = 0;
				}

			public:

				EffectGlass()
				{
					Initialize(c_nSerializeEffectGlass, c_strSerializeEffectGlass);

					Clear();
				}

			public:
				int Radius;
			};
			class EffectGlassMosaic : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double)*7 + sizeof(int)*2;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Metric);
					pStream->WriteLong(Quality);
					pStream->WriteDouble(Angle);
					pStream->WriteDouble(SizeX);
					pStream->WriteDouble(SizeY);
					pStream->WriteDouble(Curvature);
					pStream->WriteDouble(Smooth);
					pStream->WriteDouble(OffsetX);
					pStream->WriteDouble(OffsetY);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Metric = pStream->ReadLong();
					Quality = pStream->ReadLong();
					Angle = pStream->ReadDouble();
					SizeX = pStream->ReadDouble();
					SizeY = pStream->ReadDouble();
					Curvature = pStream->ReadDouble();
					Smooth = pStream->ReadDouble();
					OffsetX = pStream->ReadDouble();
					OffsetY = pStream->ReadDouble();
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
					oXmlWriter.WriteAttribute(_T("quality"), Quality);
					oXmlWriter.WriteAttribute(_T("angle"), Angle);
					oXmlWriter.WriteAttribute(_T("width"), SizeX);
					oXmlWriter.WriteAttribute(_T("height"), SizeY);
					oXmlWriter.WriteAttribute(_T("curvature"), Curvature);
					oXmlWriter.WriteAttribute(_T("smooth"), Smooth);
					oXmlWriter.WriteAttribute(_T("offsetx"), OffsetX);
					oXmlWriter.WriteAttribute(_T("offsety"), OffsetY);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Metric    = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					Quality   = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("quality"), _T("2")));
					Angle     = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));
					SizeX     = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("width"), _T("10")));
					SizeY     = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("height"), _T("10")));
					Curvature = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("curvature"), _T("0")));
					Smooth    = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("smooth"), _T("15")));
					OffsetX   = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("offsetx"), _T("0")));
					OffsetY   = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("offsety"), _T("0")));
					Scale     = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("scale"), _T("1")));
					MaxFactor = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("maxfactor"), _T("4")));
					
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}

				virtual void InternalClear()
				{
					Metric    = 0;
					Quality   = 2;
					Angle     = 0; 
					SizeX     = 10;   
					SizeY     = 10;
					Curvature = 0;
					Smooth    = 15; // 0..100
					OffsetX   = 0;
					OffsetY   = 0;
					Scale     = 1;
					MaxFactor = 4;
				}

			public:

				EffectGlassMosaic()
				{
					Initialize(c_nSerializeEffectGlassMosaic, c_strSerializeEffectGlassMosaic);

					Clear();

					bUseVideoCard = TRUE;
				}

			public:

				int Metric;
				int Quality;
				
				double Angle; 
				double SizeX;
				double SizeY;
				double Curvature;
				double Smooth;
				double OffsetX;
				double OffsetY;
				double Scale;
				double MaxFactor;
			};
			class EffectPencilSketch : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return 3 * sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Level);
					pStream->WriteLong(m_nBrightness);
					pStream->WriteLong(m_nContrast);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Level         = pStream->ReadLong();
					m_nBrightness = pStream->ReadLong();
					m_nContrast   = pStream->ReadLong();
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), Level);
					oXmlWriter.WriteAttribute(_T("brightness"), m_nBrightness);
					oXmlWriter.WriteAttribute(_T("contrast"), m_nContrast);
					oXmlWriter.WriteAttribute(_T("lengthfactor"), m_nLengthFactor);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Level = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("level"), _T("100")));
					m_nBrightness = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("brightness"), _T("10")));
					m_nContrast   = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("contrast"), _T("8")));
					m_nLengthFactor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("lengthfactor"), _T("25")));
				}

				virtual void InternalClear()
				{
					Level = 0;
					m_nBrightness = 0;
					m_nContrast = 0;
					m_nLengthFactor = 0;
				}

			public:

				EffectPencilSketch()
				{
					Initialize(c_nSerializeEffectPencilSketch, c_strSerializeEffectPencilSketch);

					Clear();
				}

			public:

				int Level; // 0..100%
				int m_nBrightness; // -100..100
				int m_nContrast;   // -100..100
				int m_nLengthFactor; // 0..100%
			};
			class EffectWaterColor : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) + sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(BrushSize);
					pStream->WriteLong(Coarseness);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					BrushSize = pStream->ReadLong();
					Coarseness = pStream->ReadLong();
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("size"), BrushSize);
					oXmlWriter.WriteAttribute(_T("coarseness"), Coarseness);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					BrushSize = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("size"), _T("3")));
					Coarseness = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("coarseness"), _T("10")));
				}

				virtual void InternalClear()
				{
					BrushSize = 3;
					Coarseness = 10;
				}

			public:

				EffectWaterColor()
				{
					Initialize(c_nSerializeEffectWaterColor, c_strSerializeEffectWaterColor);

					Clear();
				}

			public:
				int BrushSize; 
				int Coarseness;   
			};
			class EffectGlow : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Size);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Size = pStream->ReadLong();
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), Size);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Size = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("level"), _T("80")));
				}

				virtual void InternalClear()
				{
					Size = 0;
				}

			public:

				EffectGlow()
				{
					Initialize(c_nSerializeEffectGlow, c_strSerializeEffectGlow);

					Clear();
				}

			public:

				int Size; // 0..100
			};
			class EffectTVSimulation : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return 0;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					//pStream->WriteLong(Size);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					//Size = pStream->ReadLong();
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					//oXmlWriter.WriteAttribute(_T("size"), Size);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Metric           = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					nInterline       = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("interline"), _T("0")));
					dSpeedSynVer     = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("synchronize-ver-speed"), _T("0")));
					dOffsetSynVer    = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("synchronize-ver-offset"), _T("0")));
					dInterframeSize  = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("interframe-size"), _T("0")));
					dAmplitudeSynHor = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("synchronize-hor-amplitude"), _T("0")));
					dOffsetSynHor    = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("synchronize-hor-offset"), _T("0")));
					nAmplFactor      = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("synchronize-hor-amplfactor"), _T("0")));
					nAmplOffset      = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("synchronize-hor-amploffset"), _T("0")));
					nPhaseSpeed      = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("synchronize-hor-speed"), _T("0")));
					dInterframeXSize = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("interframe-x-size"), _T("0")));
					nSpeedFactorVer  = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("synchronize-ver-speed-factor"), _T("1")));

				}

				virtual void InternalClear()
				{
					Metric = 0;
					nInterline = 0;
					dSpeedSynVer = 0;
					dOffsetSynVer = 0;
					dInterframeSize = 0;
					dInterframeXSize = 0;
					dAmplitudeSynHor = 0;
					dOffsetSynHor = 0;
					nAmplFactor = 0;
					nAmplOffset = 0;
					nPhaseSpeed = 0;
					nSpeedFactorVer = 0;
				}

			public:

				EffectTVSimulation()
				{
					Initialize(c_nSerializeEffectTVSimulation, c_strSerializeEffectTVSimulation);

					Clear();
				}

			public:

				int Metric;
				int nInterline;

				double dSpeedSynVer;
				int    nSpeedFactorVer;
				double dOffsetSynVer;

				double dInterframeSize;
				double dInterframeXSize;

				double dAmplitudeSynHor;
				double dOffsetSynHor;
				
				int nAmplFactor;
				int nAmplOffset;
				int nPhaseSpeed;
			};
			class EffectGrayOut : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return 7*sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Color);
					pStream->WriteLong(Filter);
					pStream->WriteLong(LevelRed);
					pStream->WriteLong(LevelGreen);
					pStream->WriteLong(LevelBlue);

					pStream->WriteLong(Type);
					pStream->WriteLong(Threshold);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Color      = pStream->ReadLong();
					Filter     = pStream->ReadLong();
					LevelRed   = pStream->ReadLong();
					LevelGreen = pStream->ReadLong();
					LevelBlue  = pStream->ReadLong();

					Type       = pStream->ReadLong();
					Threshold  = pStream->ReadLong();
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("color"),      Color);
					oXmlWriter.WriteAttribute(_T("filter"),     Filter);
					oXmlWriter.WriteAttribute(_T("levelred"),   LevelRed);
					oXmlWriter.WriteAttribute(_T("levelgreen"), LevelGreen);
					oXmlWriter.WriteAttribute(_T("levelblue"),  LevelBlue);

					oXmlWriter.WriteAttribute(_T("type"),       Type);
					oXmlWriter.WriteAttribute(_T("threshold"),  Threshold);

				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Color      = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("color"), _T("255")));
					Filter     = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("filter"), _T("0")));
					LevelRed   = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("levelred"), _T("100")));
					LevelGreen = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("levelgreen"), _T("10")));
					LevelBlue  = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("levelblue"), _T("10")));

					Type       = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
					Threshold  = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("threshold"), _T("100")));
				}

				virtual void InternalClear()
				{
					Color      = 255;
					LevelRed   = 100;
					LevelGreen = 10;
					LevelBlue  = 10;
					Filter     = 0;

					Type = 0;
					Threshold = 100;
				}

			public:

				EffectGrayOut()
				{
					Initialize(c_nSerializeEffectGrayOut, c_strSerializeEffectGrayOut);

					Clear();
				}

			public:
				int Color;
				int LevelRed;   // 0..100%
				int LevelGreen; // 0..100%
				int LevelBlue;  // 0..100%
				int Filter;
				
				int Type;
				int Threshold; // 0..100%
			};
			class EffectWind : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) * 2 + sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(nMetric);
					pStream->WriteDouble(dLength);
					pStream->WriteLong(nDensity);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					nMetric  = pStream->ReadLong();
					dLength  = pStream->ReadDouble();
					nDensity = pStream->ReadLong();
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("metric"), nMetric);
					oXmlWriter.WriteAttribute(_T("length"), dLength);
					oXmlWriter.WriteAttribute(_T("density"), nDensity);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nMetric  = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					nDensity = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("density"), _T("0")));
					dLength  = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("length"), _T("0")));
				}

				virtual void InternalClear()
				{
					nMetric  = 0;
					nDensity = 16;
					dLength  = 16;
				}

			public:

				EffectWind()
				{
					Initialize(c_nSerializeEffectWind, c_strSerializeEffectWind);

					Clear();
				}

			public:
				int nMetric;
				int nDensity;
				double dLength;
			};
			class EffectNewsprint : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) * 2;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Level);
					pStream->WriteLong(Filter);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Level = pStream->ReadLong();
					Filter = pStream->ReadLong();
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("level"), Level);
					oXmlWriter.WriteAttribute(_T("filter"), Filter);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Level  = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("level"), _T("0")));
					Filter = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("filter"), _T("1")));

					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}

				virtual void InternalClear()
				{
					Level = 0;
					Filter = 1;
				}

			public:

				EffectNewsprint()
				{
					Initialize(c_nSerializeEffectNewsprint, c_strSerializeEffectNewsprint);

					Clear();

					bUseVideoCard = TRUE;
				}

			public:
				int Level;
				int Filter;
			};
			class EffectSnow : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return 7 * sizeof(int) + 3 * sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					//pStream->WriteLong(Level);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					//Level = pStream->ReadLong();
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("size"), nSize);
					oXmlWriter.WriteAttribute(_T("angle-dir"), nAngleDir);
					oXmlWriter.WriteAttribute(_T("angle-offset"), nAngleOffset);
					oXmlWriter.WriteAttribute(_T("count"), nCount);
					oXmlWriter.WriteAttribute(_T("droptime-min"), dMinTime);
					oXmlWriter.WriteAttribute(_T("droptime-max"), dMaxTime);
					oXmlWriter.WriteAttribute(_T("color"), nColor);
					oXmlWriter.WriteAttribute(_T("alpha"), nAlpha);
					oXmlWriter.WriteAttribute(_T("type"), nType);
					oXmlWriter.WriteAttribute(_T("quality"), nQuality);
					oXmlWriter.WriteAttribute(_T("snowdrift"), m_bSnowDrift);
					oXmlWriter.WriteAttribute(_T("snowdriftspeedfactor"), m_dSnowDriftSpeedFactor);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nSize        = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("size"), _T("0")));
					nAngleDir    = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("angle-dir"), _T("0")));
					nAngleOffset = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("angle-offset"), _T("0")));
					nCount       = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("count"), _T("0")));
					dMinTime     = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("droptime-min"), _T("0")));
					dMaxTime     = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("droptime-max"), _T("0")));
					nColor       = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("color"), _T("16777215")));
					nAlpha       = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("alpha"), _T("192")));
					nType        = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("192")));
					nQuality     = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("quality"), _T("192")));
					m_bSnowDrift = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("snowdrift"), _T("0")));
					m_dSnowDriftSpeedFactor = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("snowdriftspeedfactor"), _T("5")));
				}

				virtual void InternalClear()
				{
					nSize = 0;
					nAngleDir = 0;
					nAngleOffset = 0;

					nCount = 0;
					dMinTime = 0;
					dMaxTime = 0;

					nColor = 16777215;
					nAlpha = 192;
					nType  = 0;
					nQuality = 0;

					m_bSnowDrift = FALSE;
					m_dSnowDriftSpeedFactor = 1;
				}

			public:

				EffectSnow()
				{
					Initialize(c_nSerializeEffectSnow, c_strSerializeEffectSnow);

					Clear();
				}

			public:
				
				int nSize;        // 0..100%
				int nAngleDir;    // -45..45
				int nAngleOffset; // 0..45

				int    nCount;    // 0..1000
				double dMinTime;
				double dMaxTime;

				int nColor;
				int nAlpha;
				int nType;
				int nQuality;

				BOOL   m_bSnowDrift;
				double m_dSnowDriftSpeedFactor; // 0..10
			};

			class EffectJpegCompression : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong( m_nQuality );
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					m_nQuality = pStream->ReadLong();
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("quality"), m_nQuality);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					m_nQuality = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("quality"), _T("100")));
				}

				virtual void InternalClear()
				{
					m_nQuality = 100;
				}

			public:

				EffectJpegCompression()
				{
					Initialize(c_nSerializeEffectJpegCompression, c_strSerializeEffectJpegCompression);

					Clear();
				}

			public:

				int m_nQuality; // 0..100%
			};

			class EffectSoftFocus : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) * 3 + sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble( BlurSize );
					pStream->WriteLong( Metric );
					pStream->WriteLong( Intensity );
					pStream->WriteLong( Type );
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					BlurSize  = pStream->ReadDouble();
					Metric    = pStream->ReadLong();
					Intensity = pStream->ReadLong();
					Type      = pStream->ReadLong();
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("blursize"), BlurSize);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
					oXmlWriter.WriteAttribute(_T("intensity"), Intensity);
					oXmlWriter.WriteAttribute(_T("type"), Type);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					BlurSize  = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("blursize"), _T("5")));
					Metric    = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					Intensity = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("intensity"), _T("50")));
					Type      = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
				}

				virtual void InternalClear()
				{
					BlurSize  = 5;
					Metric    = 0;
					Intensity = 50;
					Type      = 0;
				}

			public:

				EffectSoftFocus()
				{
					Initialize(c_nSerializeEffectSoftFocus, c_strSerializeEffectSoftFocus);

					Clear();
				}

			public:

				double BlurSize; // 1..255, 0..100, 0..1
				int Metric;
				int Intensity; // 0..100%
				int Type;
			};

			class EffectOldPhoto : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int)+sizeof(int)+sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong( m_nMask );
					pStream->WriteLong( m_nClarity );
					pStream->WriteLong( m_WhiteBlack );
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					m_nMask = pStream->ReadLong();
					m_nClarity = pStream->ReadLong();
					m_WhiteBlack = pStream->ReadLong();
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("mask"), m_nMask);
					oXmlWriter.WriteAttribute(_T("clarity"), m_nClarity);
					oXmlWriter.WriteAttribute(_T("whiteblack"), m_WhiteBlack);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					m_nMask = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("mask"), _T("0")));
					m_nClarity = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("clarity"), _T("100")));
					m_WhiteBlack = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("whiteblack"), _T("1")));
				}

				virtual void InternalClear()
				{
					m_nMask = 0;
					m_nClarity = 100;
					m_WhiteBlack = 1;
				}

			public:

				EffectOldPhoto()
				{
					Initialize(c_nSerializeEffectOldPhoto, c_strSerializeEffectOldPhoto);

					Clear();
				}

			public:

				int m_nMask;  
				int m_nClarity; // 0..100%
				int m_WhiteBlack; // 0..100%
			};
			class EffectVignette : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return 0;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("metric"), nMetric);
					oXmlWriter.WriteAttribute(_T("centerx"), dCenterX);
					oXmlWriter.WriteAttribute(_T("centery"), dCenterY);
					oXmlWriter.WriteAttribute(_T("radius1x"), dRadius1X);
					oXmlWriter.WriteAttribute(_T("radius1y"), dRadius1Y);
					oXmlWriter.WriteAttribute(_T("radius2x"), dRadius2X);
					oXmlWriter.WriteAttribute(_T("radius2y"), dRadius2Y);
					oXmlWriter.WriteAttribute(_T("angle"), dAngle);
					oXmlWriter.WriteAttribute(_T("intensity"), dIntensity);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nMetric    = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					dCenterX   = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("centerx"), _T("0")));
					dCenterY   = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("centery"), _T("0")));
					dRadius1X  = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("radius1x"), _T("0")));
					dRadius1Y  = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("radius1y"), _T("0")));
					dRadius2X  = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("radius2x"), _T("0")));
					dRadius2Y  = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("radius2y"), _T("0")));
					dAngle     = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));
					dIntensity = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("intensity"), _T("0")));
				}

				virtual void InternalClear()
				{
					nMetric    = 0;
					dCenterX   = 0;
					dCenterY   = 0;
					dRadius1X  = 0;
					dRadius1Y  = 0;
					dRadius2X  = 0;
					dRadius2Y  = 0;
					dAngle     = 0;
					dIntensity = 0;
				}

			public:

				EffectVignette()
				{
					Initialize(c_nSerializeEffectVignette, c_strSerializeEffectVignette);

					Clear();
				}

			public:

				int nMetric;
				double dCenterX;
				double dCenterY;
				double dRadius1X;
				double dRadius1Y;
				double dRadius2X;
				double dRadius2Y;
				double dAngle;
				double dIntensity;
			};
			class EffectPostCrop : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return 0;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("amount"), dAmount);
					oXmlWriter.WriteAttribute(_T("midpoint"), dMidpoint);
					oXmlWriter.WriteAttribute(_T("roundness"), dRoundness);
					oXmlWriter.WriteAttribute(_T("feather"), dFeather);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					dAmount    = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("amount"), _T("0")));
					dMidpoint  = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("midpoint"), _T("0")));
					dRoundness = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("roundness"), _T("0")));
					dFeather   = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("feather"), _T("0")));
				}

				virtual void InternalClear()
				{
					dAmount    = 0;
					dMidpoint  = 0;
					dRoundness = 0;
					dFeather   = 0;
				}

			public:

				EffectPostCrop()
				{
					Initialize(c_nSerializeEffectPostCrop, c_strSerializeEffectPostCrop);

					Clear();
				}

			public:
				double dAmount;
				double dMidpoint;
				double dRoundness;
				double dFeather;
			};
			class EffectSetAlphaChannel : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return 0;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nMetric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					dX      = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("dst-x"), _T("0")));
					dY      = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("dst-y"), _T("0")));
					dWidth  = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("dst-width"), _T("0")));
					dHeight = Strings::ToDouble (oXmlNode.GetAttributeOrValue(_T("dst-height"), _T("0")));
					nType   = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
					nSrcBuffer = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("mask-buffer"), _T("-1")));
					sFilePath  = oXmlNode.GetAttributeOrValue(_T("mask-path"), _T("0"));
				}

				virtual void InternalClear()
				{
					nMetric = 0;
					dX = 0;
					dY = 0;
					dWidth = 0;
					dHeight = 0;
					nType = 0;
					sFilePath = _T("");
					nSrcBuffer = -1;
				}

			public:

				EffectSetAlphaChannel()
				{
					Initialize(c_nSerializeEffectSetAlphaChannel, c_strSerializeEffectSetAlphaChannel);

					Clear();
				}

			public:
				int nMetric;
				double dX;
				double dY;
				double dWidth;
				double dHeight;
				int nType;

				CString sFilePath;
				int nSrcBuffer;
			};
			class EffectZoomingSphere : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return 0;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Power = Strings::ToDouble( oXmlNode.GetAttributeOrValue(_T("power"), _T("1")));
				}

				virtual void InternalClear()
				{
					Power = 1;
				}

			public:

				EffectZoomingSphere()
				{
					Initialize(c_nSerializeEffectZoomingSphere, c_strSerializeEffectZoomingSphere);

					Clear();
				}

			public:
				double Power;
			};
			class EffectWaterMirrow : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return 0;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					MaskType = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("mask-type"), _T("0")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					X1 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("x1"), _T("0")));
					Y1 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("y1"), _T("0")));
					X2 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("x2"), _T("0")));
					Y2 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("y2"), _T("0")));
					AlphaMin = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("alphamin"), _T("0")));
					AlphaMax = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("alphamax"), _T("100")));

					MaskPoints.RemoveAll();

					// try to load points from string like " 1 2 3 4 5 6" (string contains x,y pairs)
					CString strPoints = oXmlNode.GetAttributeOrValue(_T("mask-points"), _T(""));
					if (strPoints.GetLength() > 0)
					{
						Templates::CArray<CString> arrSubstrings;
						int nCoordinates = Strings::Split(strPoints, _T(" "), arrSubstrings);
						
						if ((nCoordinates % 2) == 0)
						{
							for (int index = 0; index < arrSubstrings.GetCount(); ++index)
								MaskPoints.Add(Strings::ToDouble(arrSubstrings[index]));
						}
					}

					// try to load points from base64 encoded string
					_bstr_t strPointsData = oXmlNode.GetAttributeOrValue(_T("mask-data"), _T(""));
					if (strPointsData.length() > 0)

					{
						BYTE* pData = 0;
						int nSizeAllocated = 0;
						int nSizeArray = 0;

						if (Strings::ToBinary(CString((char*)(strPointsData)), pData, nSizeAllocated, nSizeArray))
						{
							double* pPointsData = (double*)pData;
							int nPointsCount = nSizeArray/sizeof(double);

							if ((nSizeArray % sizeof(double)) == 0)
							{
								for (int index = 0; index < nPointsCount; ++index, ++pPointsData)
									MaskPoints.Add(*pPointsData);
							}

							delete[] pData;
						}
					}
				}

				virtual void InternalClear()
				{
					MaskType = c_nMaskAllPixels;
					Metric = c_nMetricPixels;
					X1 = 0;
					Y1 = 0;
					X2 = 0;
					Y2 = 0;
					AlphaMin = 0;
					AlphaMax = 100;
					MaskPoints.RemoveAll();

					for( int i = 0; i < 9; ++i )
					{
						transform[i] = 0;
					}
				}

			public:
				BOOL UpdateMask( int nWidth, int nHeight, double m[9] )
				{
					for( int i = 0; i < 9; ++i )
					{
						m[i] = transform[i];
					}

					// проверяем, может маска то уже создана ранее ?
					if (m_oMask.GetWidth() == nWidth && m_oMask.GetHeight() == nHeight)
						return TRUE;

					if( MaskType == c_nMaskNone )
						return FALSE;

					if( !m_oMask.Create( nWidth, nHeight ) )
						return FALSE;

					if( MaskType == c_nMaskAllPixels )
					{
						m_oMask.Fill( 255 );
						return TRUE;
					}

					if ((MaskType == c_nMaskRectangle || MaskType == c_nMaskInverseRectangle || MaskType == c_nMaskEllipse || MaskType == c_nMaskInverseEllipse) && MaskPoints.GetSize() != 4)
						return FALSE;
					
					if ((MaskType == c_nMaskPolygon || MaskType == c_nMaskInversePolygon) && MaskPoints.GetSize() < 6)
						return FALSE;

					// подсчитываем цвета заливки маски
					int nValueFill = 0;

					if (MaskType == c_nMaskRectangle)
						nValueFill = 0;
					else if (MaskType == c_nMaskInverseRectangle)
						nValueFill = 255;
					else if (MaskType == c_nMaskEllipse)
						nValueFill = 0;
					else if (MaskType == c_nMaskInverseEllipse)
						nValueFill = 255;
					else if (MaskType == c_nMaskPolygon)
						nValueFill = 0;
					else if (MaskType == c_nMaskInversePolygon)
						nValueFill = 255;


					// устанавливаем параметры в соответствии с метрикой
					double dMetricX, dMetricY;

					switch( Metric )
					{
					case ImageStudio::Serialize::c_nMetricPercents:
						dMetricX = nWidth * 0.01;
						dMetricY = nHeight * 0.01;
						break;
					case ImageStudio::Serialize::c_nMetricNormalizedCoordinates:
						dMetricX = nWidth;
						dMetricY = nHeight;
						break;
					case ImageStudio::Serialize::c_nMetricAlternativeNormalizedCoordinates:
						dMetricX = nWidth;
						dMetricY = nHeight;
						break;
					default:
						dMetricX = 1;
						dMetricY = 1;
					}

					double dA = Y1 * dMetricY - Y2 * dMetricY;
					double dB = X2 * dMetricX - X1 * dMetricX;
					double dC = X1 * dMetricX * Y2 * dMetricY - X2 * dMetricX * Y1 * dMetricY;

					if( abs(dA) < IPP_EPS23 && abs(dB) < IPP_EPS23 )
					{
						return FALSE;
					}
					
					double dD = dA * dA + dB * dB;

					m[0] = (dB * dB - dA * dA) / dD;
					m[1] = -2 * dA * dB / dD;
					m[2] = -2 * dA * dB / dD;
					m[3] = (dA * dA - dB * dB) / dD;
					m[4] = -2 * dA * dC / dD;
					m[5] = -2 * dB * dC / dD;

					dD = sqrt( dD );

					m[6] = dA / dD;
					m[7] = dB / dD;
					m[8] = dC / dD;

					for( int i = 0; i < 9; ++i )
					{
						transform[i] = m[i];
					}

					Gdiplus::Matrix matrix( (float)m[0], (float)m[1], (float)m[2], (float)m[3], (float)m[4], (float)m[5] );

					if (MaskType == c_nMaskRectangle || MaskType == c_nMaskInverseRectangle || MaskType == c_nMaskEllipse || MaskType == c_nMaskInverseEllipse)
					{
						double dLeft   = MaskPoints[0] * dMetricX;
						double dTop    = MaskPoints[1] * dMetricY;
						double dRight  = MaskPoints[2] * dMetricX;
						double dBottom = MaskPoints[3] * dMetricY;

						m_oMask.Fill(nValueFill);

						if (MaskType == c_nMaskRectangle || MaskType == c_nMaskInverseRectangle)
							m_oMask.FillRectangle(dLeft, dTop, dRight, dBottom, 255 - nValueFill, &matrix);
						else if (MaskType == c_nMaskEllipse || MaskType == c_nMaskInverseEllipse)
							m_oMask.FillEllipse(dLeft, dTop, dRight, dBottom, 255 - nValueFill, &matrix);
					}
					else if (MaskType == c_nMaskPolygon || MaskType == c_nMaskInversePolygon)
					{
						int nPointsCount = MaskPoints.GetSize()/2;
						Gdiplus::PointF* pPoints = new Gdiplus::PointF[nPointsCount];
						if (!pPoints)
							return FALSE;

						for (int index = 0; index < nPointsCount; ++index)
						{
							double x = MaskPoints[2*index + 0] * dMetricX;
							double y = MaskPoints[2*index + 1] * dMetricY;

							pPoints[index].X = float(x);
							pPoints[index].Y = float(y);
						}

						m_oMask.Fill(nValueFill);
						m_oMask.FillPolygon(pPoints, nPointsCount, 255 - nValueFill, &matrix);

						delete[] pPoints;
					}

					return TRUE;
				}

				EffectWaterMirrow()
				{
					Initialize(c_nSerializeEffectWaterMirrow, c_strSerializeEffectWaterMirrow);

					Clear();
				}

			public:

				ImageStudio::Core::Mask m_oMask;

				int MaskType;
				int Metric;
				int AlphaMin, AlphaMax;
				double X1, Y1, X2, Y2;
				CSimpleArray<double> MaskPoints;

				double transform[9];
			};
			class EffectShapeMosaic : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return 0;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nMetric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("2")));
					dMosaicSize = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("size"), _T("0.05")));
				}

				virtual void InternalClear()
				{
					nMetric = 2;
					dMosaicSize = 0.05;
				}

			public:

				EffectShapeMosaic()
				{
					Initialize(c_nSerializeEffectShapeMosaic, c_strSerializeEffectShapeMosaic);

					Clear();
				}

			public:
				int nMetric;
				double dMosaicSize;
			};
			class EffectParticles : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return 0;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					sFilePath = oXmlNode.GetAttributeOrValue(_T("filepath"), _T(""));
					nTransparency = Strings::ToInteger(oXmlNode.GetAttribute(_T("transparency"), _T("100")));
					nMaxCountParticles = Strings::ToInteger(oXmlNode.GetAttribute(_T("maxcountparticles"), _T("10")));
					nMetric = Strings::ToInteger(oXmlNode.GetAttribute(_T("metric"), _T("0")));

					dMinSize = Strings::ToDouble(oXmlNode.GetAttribute(_T("minsize"), _T("0")));
					dMaxSize = Strings::ToDouble(oXmlNode.GetAttribute(_T("maxsize"), _T("0")));

					dBaseDirAngle = Strings::ToDouble(oXmlNode.GetAttribute(_T("basedirangle"), _T("0")));
					dDispDirAngle = Strings::ToDouble(oXmlNode.GetAttribute(_T("dispdirangle"), _T("0")));

					dMinLifeTime = Strings::ToDouble(oXmlNode.GetAttribute(_T("minlifetime"), _T("1000")));
					dMaxLifeTime = Strings::ToDouble(oXmlNode.GetAttribute(_T("maxlifetime"), _T("1000")));

					nRotType = Strings::ToInteger(oXmlNode.GetAttribute(_T("rottype"), _T("0")));

					dMinRotTime = Strings::ToDouble(oXmlNode.GetAttribute(_T("minrottime"), _T("0")));
					dMaxRotTime = Strings::ToDouble(oXmlNode.GetAttribute(_T("maxrottime"), _T("0")));

					dMinRotAngle = Strings::ToDouble(oXmlNode.GetAttribute(_T("minbaseangle"), _T("0")));
					dMaxRotAngle = Strings::ToDouble(oXmlNode.GetAttribute(_T("maxbaseangle"), _T("0")));

					bAbsRot = Strings::ToInteger(oXmlNode.GetAttribute(_T("absrot"), _T("0")));

					nQuality = Strings::ToInteger(oXmlNode.GetAttribute(_T("quality"), _T("0")));
				}

				virtual void InternalClear()
				{
					sFilePath = _T("");

					nMaxCountParticles = 100;
					nTransparency = 100;
					nMetric = 0;

					dMinSize = 0;
					dMaxSize = 0;

					dBaseDirAngle = 0;
					dDispDirAngle = 0;

					dMinLifeTime = 0;
					dMaxLifeTime = 0;

					dMinRotTime = 0;
					dMaxRotTime = 0;

					dMinRotAngle = 0;
					dMaxRotAngle = 0;

					nRotType = 0;
					bAbsRot = 0;

					nQuality = 0;
				}

			public:

				EffectParticles()
				{
					Initialize(c_nSerializeEffectParticles, c_strSerializeEffectParticles);

					Clear();
				}

			public:
				CString sFilePath;
				
				int nMaxCountParticles;
				int nTransparency; // прозрачность ресурса от 0 до 100
				int nMetric;

				double dMinSize;
				double dMaxSize;

				double dBaseDirAngle;
				double dDispDirAngle;

				double dMinLifeTime; // время жизни в миллисекундах
				double dMaxLifeTime;

				double dMinRotTime;
				double dMaxRotTime;

				double dMinRotAngle;
				double dMaxRotAngle;

				int nRotType;
				BOOL bAbsRot;

				int nQuality;
			};
			class EffectCanvas : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return 0;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					if( BaseRandom <= 0 )
						BaseRandom = 1;

					Alpha = Strings::ToDouble(oXmlNode.GetAttribute(_T("alpha"), _T("100")));
				}

				virtual void InternalClear()
				{
					Alpha = 100;
				}

			public:

				EffectCanvas()
				{
					Initialize(c_nSerializeEffectCanvas, c_strSerializeEffectCanvas);

					Clear();
				}

			public:
				double Alpha;
			};
			class EffectGlow2 : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return 0;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					Size = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("size"), _T("5")));
				}

				virtual void InternalClear()
				{
					Metric = 0;
					Size = 5;
				}

			public:

				EffectGlow2()
				{
					Initialize(c_nSerializeEffectGlow2, c_strSerializeEffectGlow2);

					Clear();
				}

			public:
				int Metric;
				double Size;
			};
			class EffectColorExchange : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return 0;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Color1 = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("color1"), _T("-1")));
					Color2 = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("color2"), _T("-1")));
				}

				virtual void InternalClear()
				{
					Color1 = -1;
					Color2 = -1;
				}

			public:

				EffectColorExchange()
				{
					Initialize(c_nSerializeEffectColorExchange, c_strSerializeEffectColorExchange);

					Clear();
				}

			public:
				int Color1;
				int Color2;
			};
			class EffectAnaglyph3d : public Core::Effect
			{
			protected:

				virtual int InternalGetRequiredStreamSize()
				{
					return 0;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
				}

				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nDepth = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("depth"), _T("36")));
					nType  = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
				}

				virtual void InternalClear()
				{
					nDepth = 36;
					nType = 0;
				}

			public:

				EffectAnaglyph3d()
				{
					Initialize(c_nSerializeEffectAnaglyph3d, c_strSerializeEffectAnaglyph3d);

					Clear();
				}

			public:
				int nDepth;
				int nType;
			};
			
			class TransformFlip : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 2*sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong((int)Vertical);
					pStream->WriteLong(BackColor);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Vertical = (BOOL)pStream->ReadLong();
					BackColor = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("vertical"), (int)Vertical);
					oXmlWriter.WriteAttribute(_T("backcolor"), BackColor);
					oXmlWriter.WriteAttribute(_T("backalpha"), BackAlpha);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Vertical = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("vertical"), _T("0")));
					BackColor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backcolor"), _T("0")));
					BackAlpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backalpha"), _T("255")));
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Vertical = FALSE;
					BackColor = 0;
					BackAlpha = 255;
				}
				
			public:
				
				TransformFlip()
				{
					Initialize(c_nSerializeTransformFlip, c_strSerializeTransformFlip);

					Clear();

					bUseVideoCard = TRUE;
				}
					
			public:
				
				BOOL Vertical; // flip image vertically (1) or horizontally (0)
				int BackColor; // background color for the transform
				int BackAlpha;
			};
			class TransformFlip2 : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 0;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Dir = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("dir"), _T("0")));
					BackColor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backcolor"), _T("0")));
					BackAlpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backalpha"), _T("255")));
				}
				
				virtual void InternalClear()
				{
					Dir = 0;
					BackColor = 0;
					BackAlpha = 255;
				}
				
			public:
				
				TransformFlip2()
				{
					Initialize(c_nSerializeTransformFlip2, c_strSerializeTransformFlip2);

					Clear();
				}
					
			public:
				
				int Dir; // (1, 2, 3, 4, 5, 6, 7, 8) 2 - вверх, 4 - вправо, 6 - вниз, 8 - влево
				int BackColor; // background color for the transform
				int BackAlpha;
			};
			class TransformPerspective : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 2*sizeof(int) + sizeof(double)*2;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong((int)Vertical);
					pStream->WriteDouble(Angle);
					pStream->WriteDouble(Factor);
					pStream->WriteLong(BackColor);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Vertical = (BOOL)pStream->ReadLong();
					Angle = pStream->ReadDouble();
					Factor = pStream->ReadDouble();
					BackColor = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("vertical"), (int)Vertical);
					oXmlWriter.WriteAttribute(_T("angle"), Angle);
					oXmlWriter.WriteAttribute(_T("factor"), Factor);
					oXmlWriter.WriteAttribute(_T("backcolor"), BackColor);
					oXmlWriter.WriteAttribute(_T("backalpha"), BackAlpha);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Vertical = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("vertical"), _T("0")));
					Angle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));
					Factor = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("factor"), _T("0.25")));
					BackColor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backcolor"), _T("0")));
					BackAlpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backalpha"), _T("255")));

					Alternative = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("alternative"), _T("0")));
					X1 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("x1"), _T("0")));
					Y1 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("y1"), _T("0")));
					X2 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("x2"), _T("1")));
					Y2 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("y2"), _T("0")));
					X3 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("x3"), _T("1")));
					Y3 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("y3"), _T("1")));
					X4 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("x4"), _T("0")));
					Y4 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("y4"), _T("1")));

					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Vertical = 0;
					Angle = 0;
					Factor = 0.25;
					BackColor = 0;
					BackAlpha = 255;

					Alternative;
					X1 = 0;
					Y1 = 0;
					X2 = 1;
					Y2 = 0;
					X3 = 1;
					Y3 = 1;
					X4 = 0;
					Y4 = 1;
				}
				
			public:
				
				TransformPerspective()
				{
					Initialize(c_nSerializeTransformPerspective, c_strSerializeTransformPerspective);

					Clear();

					bUseVideoCard = TRUE;
				}
					
			public:
				
				BOOL Vertical; // apply vertical (1) transform or horizontal (0)
				double Angle; // -90..90
				double Factor; // 0..1
				int BackColor; // background color for the transform
				int BackAlpha;

				BOOL Alternative;
				double X1;
				double Y1;
				double X2;
				double Y2;
				double X3;
				double Y3;
				double X4;
				double Y4;
			};
			class TransformSkew : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 2*sizeof(int) + sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong((int)Vertical);
					pStream->WriteDouble(Angle);
					pStream->WriteLong(BackColor);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Vertical = (BOOL)pStream->ReadLong();
					Angle = pStream->ReadDouble();
					BackColor = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("vertical"), (int)Vertical);
					oXmlWriter.WriteAttribute(_T("angle"), Angle);
					oXmlWriter.WriteAttribute(_T("backcolor"), BackColor);
					oXmlWriter.WriteAttribute(_T("backalpha"), BackAlpha);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Vertical = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("vertical"), _T("0")));
					Angle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));
					BackColor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backcolor"), _T("0")));
					BackAlpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backalpha"), _T("255")));
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Vertical = 0;
					Angle = 0;
					BackColor = 0;
					BackAlpha = 255;
				}
				
			public:
				
				TransformSkew()
				{
					Initialize(c_nSerializeTransformSkew, c_strSerializeTransformSkew);

					Clear();

					bUseVideoCard = TRUE;
				}
					
			public:
				
				BOOL Vertical; // apply vertical (1) transform or horizontal (0)
				double Angle; // -90..90
				int BackColor; // background color for the transform
				int BackAlpha;
			};
			class TransformShift : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 2*sizeof(int) + 2*sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Metric);
					pStream->WriteDouble(OffsetX);
					pStream->WriteDouble(OffsetY);
					pStream->WriteLong(BackColor);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Metric = pStream->ReadLong();
					OffsetX = pStream->ReadDouble();
					OffsetY = pStream->ReadDouble();
					BackColor = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
					oXmlWriter.WriteAttribute(_T("offsetx"), OffsetX);
					oXmlWriter.WriteAttribute(_T("offsety"), OffsetY);
					oXmlWriter.WriteAttribute(_T("backcolor"), BackColor);
					oXmlWriter.WriteAttribute(_T("backalpha"), BackAlpha);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					OffsetX = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("offsetx"), _T("0")));
					OffsetY = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("offsety"), _T("0")));
					BackColor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backcolor"), _T("0")));
					BackAlpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backalpha"), _T("255")));

					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Metric = c_nMetricPixels;

					OffsetX = 0;
					OffsetY = 0;
					BackColor = 0;
					BackAlpha = 255;
				}
				
			public:
				
				TransformShift()
				{
					Initialize(c_nSerializeTransformShift, c_strSerializeTransformShift);

					Clear();

					bUseVideoCard = TRUE;
				}
					
			public:
				
				int Metric;
				double OffsetX;
				double OffsetY; 
				int BackColor;
				int BackAlpha;

				/* @desc:
				ImageStudio-Serialize-Effects-TransformShift (c_nSerializeTransformShift, c_strSerializeTransformShift)
				{
					int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...

					// сдвиг картинки относительно самой себя (в пикселях картинки или в другой какой-то метрике)
					double OffsetX = 0;				// xml имя - offsetx; сдвиг по x
					double OffsetY = 0;				// xml имя - offsety; сдвиг по y

					int BackColor = 0;				// xml имя - backcolor; цвет, который будет использоваться для background'а при преобразовании
				}
				*/
			};
			class TransformRotate : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 2*sizeof(int) + sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Size);
					pStream->WriteDouble(Angle);
					pStream->WriteLong(BackColor);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Size = pStream->ReadLong();
					Angle = pStream->ReadDouble();
					BackColor = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("size"), Size);
					oXmlWriter.WriteAttribute(_T("angle"), Angle);
					oXmlWriter.WriteAttribute(_T("backcolor"), BackColor);
					oXmlWriter.WriteAttribute(_T("backalpha"), BackAlpha);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Size = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("size"), _T("0")));
					Angle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));
					BackColor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backcolor"), _T("0")));
					BackAlpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backalpha"), _T("255")));

					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Size = 0;
					Angle = 0;
					BackColor = 0;
					BackAlpha = 255;
				}
				
			public:
				
				TransformRotate()
				{
					Initialize(c_nSerializeTransformRotate, c_strSerializeTransformRotate);

					Clear();

					bUseVideoCard = TRUE;
				}
					
			public:
				
				int Size;
				double Angle;
				int BackColor;
				int BackAlpha;

				/* @desc:
				ImageStudio-Serialize-Effects-TransformRotate (c_nSerializeTransformRotate, c_strSerializeTransformRotate)
				{
					int Size = 0;						// xml имя - size; параметр, отвечающий за то, сохранять оригинальный размер картинки - (1) или нет - (0)

					double Angle = 0;					// xml имя - angle; угол поворота в градусах -360..360

					int BackColor = 0;					// xml имя - backcolor; цвет, который будет использоваться для background'а при преобразовании
				}
				*/
			};
			class TransformRotateAdvanced : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 5*sizeof(int) + 4*sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(IndexFrom);
					pStream->WriteLong(IndexTo);
					pStream->WriteLong(Metric);
					pStream->WriteDouble(CenterX);
					pStream->WriteDouble(CenterY);
					pStream->WriteDouble(Angle);
					pStream->WriteDouble(Scale);
					pStream->WriteLong(Interpolate);
					pStream->WriteLong(Alpha);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					IndexFrom = pStream->ReadLong();
					IndexTo = pStream->ReadLong();
					Metric = pStream->ReadLong();
					CenterX = pStream->ReadDouble();
					CenterY = pStream->ReadDouble();
					Angle = pStream->ReadDouble();
					Scale = pStream->ReadDouble();
					Interpolate = pStream->ReadLong();
					Alpha = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("index-from"), IndexFrom);
					oXmlWriter.WriteAttribute(_T("index-to"), IndexTo);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
					oXmlWriter.WriteAttribute(_T("centerx"), CenterX);
					oXmlWriter.WriteAttribute(_T("centery"), CenterY);
					oXmlWriter.WriteAttribute(_T("angle"), Angle);
					oXmlWriter.WriteAttribute(_T("scale"), Scale);
					oXmlWriter.WriteAttribute(_T("interpolate"), Interpolate);
					oXmlWriter.WriteAttribute(_T("alpha"), Alpha);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					IndexFrom = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("index-from"), _T("-1")));
					IndexTo = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("index-to"), _T("-1")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					CenterX = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("centerx"), _T("0")));
					CenterY = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("centery"), _T("0")));
					Angle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));
					Scale = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("scale"), _T("1")));
					Interpolate = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("interpolate"), _T("0")));
					Alpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("alpha"), _T("255")));
				}
				
				virtual void InternalClear()
				{
					IndexFrom = -1;
					IndexTo = -1;
					Metric = 0;
					CenterX = 0;
					CenterY = 0;
					Angle = 0;
					Scale = 1;
					Interpolate = 0;
					Alpha = 255;
				}
				
			public:
				
				TransformRotateAdvanced()
				{
					Initialize(c_nSerializeTransformRotateAdvanced, c_strSerializeTransformRotateAdvanced);

					Clear();
				}
					
			public:
				
				int IndexFrom;
				int IndexTo;

				int Metric;
				double CenterX;
				double CenterY;
				double Angle;
				double Scale;

				int Interpolate;
				int Alpha;

				/* @desc:
				ImageStudio-Serialize-Effects-TransformRotateAdvanced (c_nSerializeTransformRotateAdvanced, c_strSerializeTransformRotateAdvanced)
				{
					int IndexFrom - 1;					// xml имя - index-from; индекс буффера из которого скопировать интерфейс
					int IndexTo = -1;					// xml имя - index-to; индекс буффера в который скопировать интерфейс

					int Metric = c_nMetricPixels;		// xml имя - metriс; тип метрики, см. c_nMetric...

					double CenterX = 0;					// xml имя - centerx; x-координата центра, относительно которого будет производится поворот (с учетом метрики)
					double CenterY = 0;					// xml имя - centery; y-координата центра, относительно которого будет производится поворот (с учетом метрики)
					double Angle = 0;					// xml имя - angle; угол поворота в градусах -360..360
					double Scale = 0;					// xml имя - scale; коэффициент сжатия/растяжения from-картинки
					int Interpolate = 0;				// xml имя - interpolate; параметр, отвечающий за то - интерполировать ли (1) цвета пикселов при преобрзовании поворота биллинейно, или нет (0)
				}
				*/
			};
			class TransformRotateSimple : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Type);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Type = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("type"), Type);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Type = 0;
				}
				
			public:
				
				TransformRotateSimple()
				{
					Initialize(c_nSerializeTransformRotateSimple, c_strSerializeTransformRotateSimple);

					Clear();
				}
					
			public:
				
				int Type;
			};

			class TransformResample : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Times);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Times = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("times"), Times);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Times = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("times"), _T("1")));
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Times = 1;
				}
				
			public:
				
				TransformResample()
				{
					Initialize(c_nSerializeTransformResample, c_strSerializeTransformResample);

					Clear();

					bUseVideoCard = TRUE;
				}
					
			public:
				
				int Times;

				/* @desc:
				ImageStudio-Serialize-Effects-TransformResample (c_nSerializeTransformResample, c_strSerializeTransformResample)
				{
					int Times = 1;						// xml имя - times; количество повторений картинки 1..min(Width,Height)
				}
				*/
			};
			class TransformZoom : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 3*sizeof(int) + 2*sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Zoom);
					pStream->WriteLong(BackColor);
					pStream->WriteDouble(dx);
					pStream->WriteDouble(dy);
					pStream->WriteLong(Metric);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Zoom = pStream->ReadLong();
					BackColor = pStream->ReadLong();
					dx = pStream->ReadDouble();
					dy = pStream->ReadDouble();
					Metric = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("zoom"), Zoom);
					oXmlWriter.WriteAttribute(_T("backcolor"), BackColor);
					oXmlWriter.WriteAttribute(_T("backalpha"), BackAlpha);
					oXmlWriter.WriteAttribute(_T("dx"), dx);
					oXmlWriter.WriteAttribute(_T("dy"), dy);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Zoom = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("zoom"), _T("0")));
					BackColor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backcolor"), _T("0")));
					BackAlpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backalpha"), _T("255")));
					dx = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("dx"), _T("0")));
					dy = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("dy"), _T("0")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Zoom = 0;
					BackColor = 0;
					BackAlpha = 255;
					dx = 0;
					dy = 0;
					Metric = 0;
				}
				
			public:
				
				TransformZoom()
				{
					Initialize(c_nSerializeTransformZoom, c_strSerializeTransformZoom);

					Clear();

					bUseVideoCard = TRUE;
				}
					
			public:
				
				int Zoom; // magnification value: -100..100
				int BackColor; // background color for the transform
				int BackAlpha;
				double dx;
				double dy;
				int Metric;
			};
			class TransformZoomLinear : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 2*sizeof(int) + 4*sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(dZoomX);
					pStream->WriteDouble(dZoomY);
					pStream->WriteLong(BackColor);
					pStream->WriteDouble(dx);
					pStream->WriteDouble(dy);
					pStream->WriteLong(Metric);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					dZoomX = pStream->ReadDouble();
					dZoomY = pStream->ReadDouble();
					BackColor = pStream->ReadLong();
					dx = pStream->ReadDouble();
					dy = pStream->ReadDouble();
					Metric = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("zoomX"), dZoomX);
					oXmlWriter.WriteAttribute(_T("zoomY"), dZoomY);
					oXmlWriter.WriteAttribute(_T("backcolor"), BackColor);
					oXmlWriter.WriteAttribute(_T("backalpha"), BackAlpha);
					oXmlWriter.WriteAttribute(_T("dx"), dx);
					oXmlWriter.WriteAttribute(_T("dy"), dy);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					dZoomX = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("zoomX"), _T("0")));
					dZoomY = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("zoomY"), _T("0")));
					BackColor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backcolor"), _T("0")));
					BackAlpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backalpha"), _T("255")));
					dx = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("dx"), _T("0")));
					dy = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("dy"), _T("0")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					dZoomX = 1;
					dZoomY = 1;
					BackColor = 0;
					BackAlpha = 255;
					dx = 0;
					dy = 0;
					Metric = 0;
				}
				
			public:
				
				TransformZoomLinear()
				{
					Initialize(c_nSerializeTransformZoomLinear, c_strSerializeTransformZoomLinear);

					Clear();

					bUseVideoCard = TRUE;
				}
					
			public:
				
				double dZoomX;
				double dZoomY;
				int BackColor; // background color for the transform
				int BackAlpha;
				double dx;
				double dy;
				int Metric;
			};
			class TransformMirror : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 2*sizeof(int) + sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Type);
					pStream->WriteDouble(Level);
					pStream->WriteLong(BackColor);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Type = pStream->ReadLong();
					Level = pStream->ReadDouble();
					BackColor = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("type"), Type);
					oXmlWriter.WriteAttribute(_T("level"), Level);
					oXmlWriter.WriteAttribute(_T("backcolor"), BackColor);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
					Level = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("level"), _T("0")));
					BackColor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backcolor"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Type = 0;
					Level = 0;
					BackColor = 0;
				}
				
			public:
				
				TransformMirror()
				{
					Initialize(c_nSerializeTransformMirror, c_strSerializeTransformMirror);

					Clear();
				}
					
			public:
				
				int Type; // type of the mirror (see c_nMirror* constants)
				double Level; // y coordinate of the mirror in percents from the height of the image: 0..100
				int BackColor; // background color for the transform
			};
			class TransformResize : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 4*sizeof(int) + 3*sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Type);
					pStream->WriteDouble(Width);
					pStream->WriteDouble(Height);
					pStream->WriteLong(BackColor);
					pStream->WriteLong(BackAlpha);
					pStream->WriteLong(Metric);
					pStream->WriteDouble(CropZoom);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Type = pStream->ReadLong();
					Width = pStream->ReadDouble();
					Height = pStream->ReadDouble();
					BackColor = pStream->ReadLong();
					BackAlpha = pStream->ReadLong();
					Metric = pStream->ReadLong();
					CropZoom = pStream->ReadDouble();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("type"), Type);
					oXmlWriter.WriteAttribute(_T("width"), Width);
					oXmlWriter.WriteAttribute(_T("height"), Height);
					oXmlWriter.WriteAttribute(_T("backcolor"), BackColor);
					oXmlWriter.WriteAttribute(_T("backalpha"), BackAlpha);
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
					oXmlWriter.WriteAttribute(_T("cropzoom"), CropZoom);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
					Width = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("width"), _T("0")));
					Height = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("height"), _T("0")));
					SrcX = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("src-x"), _T("0")));
					SrcY = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("src-y"), _T("0")));
					SrcWidth = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("src-width"), _T("0")));
					SrcHeight = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("src-height"), _T("0")));
					SrcMetric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("src-metric"), _T("-1")));
					BackColor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backcolor"), _T("0")));
					BackAlpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backalpha"), _T("255")));
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					CropZoom = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("cropzoom"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Type = 0;
					Width = 0;
					Height = 0;
					SrcX = 0;
					SrcY = 0;
					SrcWidth = 0;
					SrcHeight = 0;
					SrcMetric = -1;
					BackColor = 0;
					BackAlpha = 255;
					Metric = 0;
					CropZoom = 1;
				}
				
			public:
				
				TransformResize()
				{
					Initialize(c_nSerializeTransformResize, c_strSerializeTransformResize);

					Clear();
				}
					
			public:
				
				int Type; // type of resizing (see c_nResize* constants)
				double Width; // new width of the image in pixels
				double Height; // new height of the image in pixels

				double SrcX;
				double SrcY;
				double SrcWidth;
				double SrcHeight;
				int SrcMetric;

				int BackColor; // background color for the transform
				int BackAlpha; // background alpha for the transform
				int Metric;
				double CropZoom;
			};
			class TransformTwirl : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) + sizeof(double) * 3;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Degree);
					pStream->WriteDouble(Angle);
					pStream->WriteDouble(Zoom);
					pStream->WriteLong(BackColor);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Degree = pStream->ReadDouble();
					Angle = pStream->ReadDouble();
					Zoom = pStream->ReadDouble();
					BackColor = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("degree"), Degree);
					oXmlWriter.WriteAttribute(_T("angle"), Angle);
					oXmlWriter.WriteAttribute(_T("zoom"), Angle);
					oXmlWriter.WriteAttribute(_T("backcolor"), BackColor);
					oXmlWriter.WriteAttribute(_T("backalpha"), BackAlpha);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Degree    = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("degree"), _T("1")));
					Angle     = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));
					Zoom      = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("zoom"), _T("1")));
					BackColor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backcolor"), _T("0")));
					BackAlpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backalpha"), _T("255")));
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Degree = 0;
					Angle = 0;
					Zoom = 1;
					BackColor = 0;
					BackAlpha = 255;
				}
				
			public:
				
				TransformTwirl()
				{
					Initialize(c_nSerializeTransformTwirl, c_strSerializeTransformTwirl);

					Clear();

					bUseVideoCard = TRUE;
				}
					
			public:
				
				double Degree; // degree of the effect function (x^degree is the main value of the function which defines curvature of the effect): 1..
				double Angle; // -360..360
				double Zoom;
				int BackColor; // background color for the transform
				int BackAlpha;
			};
			class TransformSphere : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 2*sizeof(int) + 5*sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Metric);
					pStream->WriteDouble(CenterX);
					pStream->WriteDouble(CenterY);
					pStream->WriteDouble(RadiusX);
					pStream->WriteDouble(RadiusY);
					pStream->WriteDouble(Degree);
					pStream->WriteLong(BackColor);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Metric = pStream->ReadLong();
					CenterX = pStream->ReadDouble();
					CenterY = pStream->ReadDouble();
					RadiusX = pStream->ReadDouble();
					RadiusY = pStream->ReadDouble();
					Degree = pStream->ReadDouble();
					BackColor = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
					oXmlWriter.WriteAttribute(_T("centerx"), CenterX);
					oXmlWriter.WriteAttribute(_T("centery"), CenterY);
					oXmlWriter.WriteAttribute(_T("radiusx"), RadiusX);
					oXmlWriter.WriteAttribute(_T("radiusy"), RadiusY);
					oXmlWriter.WriteAttribute(_T("degree"), Degree);
					oXmlWriter.WriteAttribute(_T("backcolor"), BackColor);
					oXmlWriter.WriteAttribute(_T("backalpha"), BackAlpha);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					CenterX = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("centerx"), _T("0")));
					CenterY = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("centery"), _T("0")));
					RadiusX = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("radiusx"), _T("0")));
					RadiusY = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("radiusy"), _T("0")));
					Degree = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("degree"), _T("0")));
					BackColor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backcolor"), _T("0")));
					BackAlpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backalpha"), _T("255")));
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Metric = c_nMetricPixels;

					CenterX = 0;
					CenterY = 0;
					RadiusX = 0;
					RadiusY = 0;
					Degree = 0;
					BackColor = 0;
					BackAlpha = 255;
				}
				
			public:
				
				TransformSphere()
				{
					Initialize(c_nSerializeTransformSphere, c_strSerializeTransformSphere);

					Clear();

					bUseVideoCard = TRUE;
				}
					
			public:

				int Metric;
				double CenterX, CenterY;
				double RadiusX, RadiusY;
				double Degree;
				int BackColor;
				int BackAlpha;

				/* @desc:
				ImageStudio-Serialize-Effects-TransformSphere (c_nSerializeTransformSphere, c_strSerializeTransformSphere)
				{
					int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...

					// координаты центра преобразования (в пикселях картинки или в другой какой-то метрике)
					double CenterX = 0;				// xml имя - centerx;
					double CenterY = 0;				// xml имя - centery;

					// размеры преобразования (в пикселях картинки или в другой какой-то метрике)
					double RadiusX = 0;				// xml имя - radiusx;
					double RadiusY = 0;				// xml имя - radiusy;

					double Degree = 0;				// xml имя - degree; степень преобразование (x^degree), изменяется в пределах -1..10
					int BackColor = 0;				// xml имя - backcolor; цвет, который будет использоваться для background'а при преобразовании
				}
				*/
			};
			class TransformCylinder : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 3*sizeof(int) + 3*sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Metric);
					pStream->WriteDouble(Center);
					pStream->WriteDouble(Radius);
					pStream->WriteLong(Type);
					pStream->WriteDouble(Degree);
					pStream->WriteLong(BackColor);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Metric = pStream->ReadLong();
					Center = pStream->ReadDouble();
					Radius = pStream->ReadDouble();
					Type = pStream->ReadLong();
					Degree = pStream->ReadDouble();
					BackColor = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("metric"), Metric);
					oXmlWriter.WriteAttribute(_T("center"), Center);
					oXmlWriter.WriteAttribute(_T("radius"), Radius);
					oXmlWriter.WriteAttribute(_T("type"), Type);
					oXmlWriter.WriteAttribute(_T("degree"), Degree);
					oXmlWriter.WriteAttribute(_T("backcolor"), BackColor);
					oXmlWriter.WriteAttribute(_T("backalpha"), BackAlpha);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					Center = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("center"), _T("0")));
					Radius = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("radius"), _T("0")));
					Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
					Degree = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("degree"), _T("0")));
					BackColor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backcolor"), _T("0")));
					BackAlpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backalpha"), _T("0")));
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					Metric = c_nMetricPixels;

					Center = 0;
					Radius = 0;
					Type = 0;
					Degree = 0;
					BackColor = 0;
					BackAlpha = 255;
				}
				
			public:
				
				TransformCylinder()
				{
					Initialize(c_nSerializeTransformCylinder, c_strSerializeTransformCylinder);

					Clear();

					bUseVideoCard = TRUE;
				}
					
			public:
				
				int Metric;
				double Center;
				double Radius;
				int Type;
				double Degree;
				int BackColor;
				int BackAlpha;

				/* @desc:
				ImageStudio-Serialize-Effects-TransformCylinder (c_nSerializeTransformCylinder, c_strSerializeTransformCylinder)
				{
					int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...

					double Center = 0;				// xml имя - center; x или y координата центра преобразования (в пикселях картинки или в другой какой-то метрике)
					double Radius = 0;				// xml имя - radius; размер преобразования (в пикселях картинки или в другой какой-то метрике)
					int Type = 0;					// xml имя - type; тип преобразования, см. c_nTransformCylinder...
					double Degree = 0;				// xml имя - degree; степень преобразование (x^degree), изменяется в пределах -1..10
					int BackColor = 0;				// xml имя - backcolor; цвет, который будет использоваться для background'а при преобразовании
				}
				*/
			};
			class TransformWave : public Core::Effect
			{
			protected:
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double)*4 + sizeof(int)*3;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble( dNumPeaks );
					pStream->WriteDouble( dPeakOffset );
					pStream->WriteDouble( dAmplitude );
					pStream->WriteDouble( dSpeed );
					pStream->WriteLong( bVertical );
					pStream->WriteLong( nType );
					pStream->WriteLong( nMetric );
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					dNumPeaks   = pStream->ReadDouble();
					dPeakOffset = pStream->ReadDouble();
					dAmplitude  = pStream->ReadDouble();
					dSpeed      = pStream->ReadDouble();
					bVertical   = pStream->ReadLong();
					nType       = pStream->ReadLong();
					nMetric     = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute( _T("peaks"), dNumPeaks );
					oXmlWriter.WriteAttribute( _T("peakoffset"), dPeakOffset );
					oXmlWriter.WriteAttribute( _T("amplitude"), dAmplitude );
					oXmlWriter.WriteAttribute( _T("speed"), dSpeed );
					oXmlWriter.WriteAttribute( _T("vertical"), bVertical );
					oXmlWriter.WriteAttribute( _T("type"), nType );
					oXmlWriter.WriteAttribute( _T("metric"), nMetric );
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					dNumPeaks   = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("peaks"), _T("0")) );
					dPeakOffset = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("peakoffset"), _T("0")) );
					dAmplitude  = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("amplitude"), _T("0")) );
					dSpeed      = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("speed"), _T("0")) );
					bVertical   = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("vertical"), _T("0")) );
					nType       = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("type"), _T("0")) );
					nMetric     = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("metric"), _T("0")) );
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					dNumPeaks   = 0;
					dPeakOffset = 0;
					dAmplitude  = 0;
					dSpeed      = 0;
					bVertical   = 0;
					nType       = 0;
					nMetric     = 0;
				}

			public:
				TransformWave()
				{
					Initialize(c_nSerializeTransformWave, c_strSerializeTransformWave);

					Clear();

					bUseVideoCard = TRUE;
				}
			public:
				double dNumPeaks;
				double dPeakOffset;
				double dAmplitude;
				double dSpeed;
				int    bVertical;
				int    nType;
				int    nMetric;
			};			
			class TransformRipple : public Core::Effect
			{
			protected:
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int)*2 + sizeof(double)*3;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong( nMetric );
					pStream->WriteLong( bRevers );
					pStream->WriteDouble( dOmega );
					pStream->WriteDouble( dOffset );
					pStream->WriteDouble( dSpeed );
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					nMetric = pStream->ReadLong();
					bRevers = pStream->ReadLong();
					dOmega  = pStream->ReadDouble();
					dOffset = pStream->ReadDouble();
					dSpeed  = pStream->ReadDouble();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute( _T("metric"), nMetric );
					oXmlWriter.WriteAttribute( _T("out"), bRevers );
					oXmlWriter.WriteAttribute( _T("omega"), dOmega );
					oXmlWriter.WriteAttribute( _T("offset"), dOffset );
					oXmlWriter.WriteAttribute( _T("speed"), dSpeed );
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nMetric = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("metric"), _T("0")) );
					bRevers = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("out"), _T("0")) );
					dOmega  = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("omega"), _T("50")) );
					dOffset = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("offset"), _T("10")) );
					dSpeed  = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("speed"), _T("0")) );
					bUseVideoCard = Strings::ToInteger(oXmlNode.GetAttribute(_T("enableGPU"), _T("1")));
				}
				
				virtual void InternalClear()
				{
					nMetric = 0;
					bRevers = 0;
					dOmega  = 0;
					dOffset = 0;
					dSpeed  = 0;
				}

			public:
				TransformRipple()
				{
					Initialize(c_nSerializeTransformRipple, c_strSerializeTransformRipple);

					Clear();

					bUseVideoCard = TRUE;
				}
			public:
				int nMetric;
				int bRevers;
				double dOmega;
				double dOffset;
				double dSpeed;
			};			

			class TransformRotateSpecial : public Core::Effect
			{
			protected:
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble( dAngle );
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					dAngle = pStream->ReadDouble();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute( _T("angle"), dAngle );
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					dAngle  = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("angle"), _T("0")) );
					dScale  = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("scale"), _T("0")) );
					dLeft   = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("CropLeft"), _T("0")) );
					dTop    = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("CropTop"), _T("0")) );
					dWidth  = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("CropWidth"), _T("0")) );
					dHeight = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("CropHeight"), _T("0")) );
					nMetric = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("metric"), _T("0")) );
				}
				
				virtual void InternalClear()
				{
					dAngle  = 0;
					dScale  = 0;
					dLeft   = 0;
					dTop    = 0;
					dWidth  = 0;
					dHeight = 0;
					nMetric = 0;
				}

			public:
				TransformRotateSpecial()
				{
					Initialize(c_nSerializeTransformRotateSpecial, c_strSerializeTransformRotateSpecial);

					Clear();
				}
			public:
				double dAngle;
				double dScale;
				
				double dLeft;
				double dTop;
				double dWidth;
				double dHeight;

				int nMetric;
			};			

			class TransformLinear : public Core::Effect
			{
			protected:
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nMetric = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("metric"), _T("0")) );
					dAngle  = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("angle"), _T("0")) );
					dScaleX = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("scalex"), _T("1")) );
					dScaleY = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("scaley"), _T("1")) );
					dShiftX = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("shiftx"), _T("0")) );
					dShiftY = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("shifty"), _T("0")) );
					nBackColor = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("backcolor"), _T("0")) );
					nBackAlpha = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("backalpha"), _T("0")) );
				}
				
				virtual void InternalClear()
				{
					nMetric = 0;

					dAngle = 0;
					dScaleX = 1;
					dScaleY = 1;
					dShiftX = 0;
					dShiftY = 0;

					nBackColor = 0;
					nBackAlpha = 0;
				}

			public:
				TransformLinear()
				{
					Initialize(c_nSerializeTransformLinear, c_strSerializeTransformLinear);

					Clear();
				}
			public:
				int nMetric;

				double dAngle;
				double dScaleX;
				double dScaleY;
				double dShiftX;
				double dShiftY;

				int nBackColor;
				int nBackAlpha;
			};			

			class TransformGlassTile : public Core::Effect
			{
			protected:
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nMetric = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("metric"), _T("0")) );
					dSize = Strings::ToDouble( oXmlNode.GetAttributeOrValue( _T("size"), _T("10")) );
				}
				
				virtual void InternalClear()
				{
					nMetric = 0;
					dSize = 10;
				}

			public:
				TransformGlassTile()
				{
					Initialize(c_nSerializeTransformGlassTile, c_strSerializeTransformGlassTile);

					Clear();
				}
			public:
				int nMetric;
				double dSize;
			};

			class TransformBrokenGlass : public Core::Effect
			{
			protected:
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					nDebris = Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("debris"), _T("15")) );
				}
				
				virtual void InternalClear()
				{
					nDebris = 15;
				}

			public:
				TransformBrokenGlass()
				{
					Initialize(c_nSerializeTransformBrokenGlass, c_strSerializeTransformBrokenGlass);

					Clear();
				}
			public:
				int nDebris;
			};			

			
			class FilterPrewitt : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Type);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Type = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("type"), Type);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Type = 0;
				}
				
			public:
				
				FilterPrewitt()
				{
					Initialize(c_nSerializeFilterPrewitt, c_strSerializeFilterPrewitt);

					Clear();
				}
					
			public:
				
				int Type;

				/* @desc:
				ImageStudio-Serialize-Effects-FilterPrewitt (c_nSerializeFilterPrewitt, c_strSerializeFilterPrewitt)
				{
					int Type = 0;						// xml имя - type; тип фильтра (см. c_nFilter* )
				}
				*/
			};
			class FilterScharr : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Type);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Type = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("type"), Type);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Type = 0;
				}
				
			public:
				
				FilterScharr()
				{
					Initialize(c_nSerializeFilterScharr, c_strSerializeFilterScharr);

					Clear();
				}
					
			public:
				
				int Type;

				/* @desc:
				ImageStudio-Serialize-Effects-FilterScharr (c_nSerializeFilterScharr, c_strSerializeFilterScharr)
				{
					int Type = 0;						// xml имя - type; тип фильтра (см. c_nFilter* )
				}
				*/
			};
			class FilterSobel : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 2*sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong((int)Extended);
					pStream->WriteLong(Type);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Extended = (BOOL)pStream->ReadLong();
					Type = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("extended"), (int)Extended);
					oXmlWriter.WriteAttribute(_T("type"), Type);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Extended = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("extended"), _T("0")));
					Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Extended = FALSE;
					Type = 0;
				}
				
			public:
				
				FilterSobel()
				{
					Initialize(c_nSerializeFilterSobel, c_strSerializeFilterSobel);

					Clear();
				}
					
			public:
				
				BOOL Extended;
				int Type;

				/* @desc:
				ImageStudio-Serialize-Effects-FilterSobel (c_nSerializeFilterSobel, c_strSerializeFilterSobel)
				{
					int Type = 0;						// xml имя - type; тип фильтра (см. c_nFilter* )
					BOOL Extended = FALSE;				// xml имя - extended; параметр, отвечающий за то, какой фильтр применять (1)-расширенный или (0)-упрощенный
				}
				*/
			};
			class FilterRoberts : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Type);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Type = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("type"), Type);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Type = 0;
				}
				
			public:
				
				FilterRoberts()
				{
					Initialize(c_nSerializeFilterRoberts, c_strSerializeFilterRoberts);

					Clear();
				}
					
			public:
				
				int Type;
				
				/* @desc:
				ImageStudio-Serialize-Effects-FilterRoberts (c_nSerializeFilterRoberts, c_strSerializeFilterRoberts)
				{
					int Type = 0;						// xml имя - type; тип фильтра (см. c_nFilter* )
				}
				*/
			};
			class FilterLaplace : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Type);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Type = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("type"), Type);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Type = 0;
				}
				
			public:
				
				FilterLaplace()
				{
					Initialize(c_nSerializeFilterLaplace, c_strSerializeFilterLaplace);

					Clear();
				}
					
			public:
				
				int Type;

				/* @desc:
				ImageStudio-Serialize-Effects-FilterLaplace (c_nSerializeFilterLaplace, c_strSerializeFilterLaplace)
				{
					int Type = 0;						// xml имя - type; тип фильтра (см. c_nFilter* )
				}
				*/
			};
			class FilterHipass : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Type);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Type = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("type"), Type);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Type = 0;
				}
				
			public:
				
				FilterHipass()
				{
					Initialize(c_nSerializeFilterHipass, c_strSerializeFilterHipass);

					Clear();
				}
					
			public:
				
				int Type;

				/* @desc:
				ImageStudio-Serialize-Effects-FilterHipass (c_nSerializeFilterHipass, c_strSerializeFilterHipass)
				{
					int Type = 0;						// xml имя - type; тип фильтра (см. c_nFilter* )
				}
				*/
			};
			class FilterLowpass : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Type);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Type = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("type"), Type);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Type = 0;
				}
				
			public:
				
				FilterLowpass()
				{
					Initialize(c_nSerializeFilterLowpass, c_strSerializeFilterLowpass);

					Clear();
				}
					
			public:
				
				int Type;

				/* @desc:
				ImageStudio-Serialize-Effects-FilterLowpass (c_nSerializeFilterLowpass, c_strSerializeFilterLowpass)
				{
					int Type = 0;						// xml имя - type; тип фильтра (см. c_nFilter* )
				}
				*/
			};
			class FilterBlur : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteLong(Type);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Type = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("type"), Type);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Type = 0;
				}
				
			public:
				
				FilterBlur()
				{
					Initialize(c_nSerializeFilterBlur, c_strSerializeFilterBlur);

					Clear();
				}
					
			public:
				
				int Type;

				/* @desc:
				ImageStudio-Serialize-Effects-FilterBlur (c_nSerializeFilterBlur, c_strSerializeFilterBlur)
				{
					int Type = 0;						// xml имя - type; тип фильтра (см. c_nFilter* )
				}
				*/
			};
			class FilterCanny : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 2*sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(LevelLow);
					pStream->WriteDouble(LevelHigh);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					LevelLow = pStream->ReadDouble();
					LevelHigh = pStream->ReadDouble();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("levellow"), LevelLow);
					oXmlWriter.WriteAttribute(_T("levelhigh"), LevelHigh);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					LevelLow = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("levellow"), _T("0")));
					LevelHigh = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("levelhigh"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					LevelLow = 0;
					LevelHigh = 0;
				}
				
			public:
				
				FilterCanny()
				{
					Initialize(c_nSerializeFilterCanny, c_strSerializeFilterCanny);

					Clear();
				}
					
			public:
				
				double LevelLow;
				double LevelHigh;

				/* @desc:
				ImageStudio-Serialize-Effects-FilterCanny (c_nSerializeFilterCanny, c_strSerializeFilterCanny)
				{
					double LevelLow = 0;				// xml имя - levellow; малый порог отсечения границ (изменяется в пределах 0..1000)
					double LevelHigh = 0;				// xml имя - levelhigh; большой порог отсечения границ (изменяется в пределах 0..1000)
				}
				*/
			};
		}
		namespace File
		{
			class FileLoadImage : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return SourcePath.GetLength() + 1 + sizeof(int);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteString(SourcePath);
					pStream->WriteLong(bVerFlip);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					SourcePath = pStream->ReadString();
					bVerFlip   = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("sourcepath"), SourcePath);
					oXmlWriter.WriteAttribute(_T("verflip"), bVerFlip);
					oXmlWriter.WriteAttribute(_T("filetype"), nFileType);
					oXmlWriter.WriteAttribute(_T("width"), nWidth);
					oXmlWriter.WriteAttribute(_T("height"), nHeight);
					oXmlWriter.WriteAttribute(_T("aspectx"), nAspectX);
					oXmlWriter.WriteAttribute(_T("aspecty"), nAspectY);
					oXmlWriter.WriteAttribute(_T("resizemode"), nResizeMode);
					oXmlWriter.WriteAttribute(_T("backcolor"), nBackColor);
					oXmlWriter.WriteAttribute(_T("backalpha"), nBackAlpha);
					oXmlWriter.WriteAttribute(_T("frame"), nFrame);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					SourcePath  = oXmlNode.GetAttributeOrValue(_T("sourcepath"), _T(""));
					bVerFlip    = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("verflip"), _T("0")) );
					nFileType   = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("filetype"), _T("0")) );
					nWidth      = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("width"), _T("0")) );
					nHeight     = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("height"), _T("0")) );
					nAspectX    = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("aspectx"), _T("0")) );
					nAspectY    = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("aspecty"), _T("0")) );
					nResizeMode = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("resizemode"), _T("0")) );
					nBackColor  = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("backcolor"), _T("0")) );
					nBackAlpha  = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("backalpha"), _T("255")) );
					nFrame      = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("frame"), _T("0")) );
				}
				
				virtual void InternalClear()
				{
					SourcePath  = _T("");
					bVerFlip    = FALSE;
					nFileType   = 0;
					nWidth      = 0;
					nHeight     = 0;
					nAspectX    = 0;
					nAspectY    = 0;
					nResizeMode = 0;
					nBackColor  = 0;
					nBackAlpha  = 0;
					nFrame      = 0;
				}
				
			public:
				
				FileLoadImage()
				{
					Initialize(c_nSerializeFileLoadImage, c_strSerializeFileLoadImage);

					Clear();
				}
					
			public:
				
				CString SourcePath; // путь к файлу
				
				BOOL bVerFlip;   // переворачивать ли файл при загрузке
				int nFileType;   // ожидаемый тип файла (для ускорения загрузки) (nFileType <=0 - любой тип)
				int nWidth;      // требуемая ширина (nWidth <= 0 - оригинальная ширина)
				int nHeight;     // требуемая высота (nHeight <= 0 - оригинальная высота)
				int nAspectX;    // требуемый аспект по X (nAspectX <= 0 - оригинальный аспект)
				int nAspectY;    // требуемый аспект по Y (nAspectY <= 0 - оригинальный аспект)
				int nResizeMode; // режим ресайзинга
				int nBackColor;  // цвет для полей 
				int nBackAlpha;  // альфа для полей
				int nFrame;
			};
			class FileLoadScreenshot : public Core::Effect
			{
			public:
				
				FileLoadScreenshot()
				{
					Initialize(c_nSerializeFileLoadScreenshot, c_strSerializeFileLoadScreenshot);

					Clear();
				}

				/* @desc:
				ImageStudio-Serialize-File-FileLoadScreenshot (c_nSerializeFileLoadScreenshot, c_strSerializeFileLoadScreenshot)
				{
				}
				*/
			};
			class FileLoadRaw : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 0;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("sourcepath"), SourcePath);
					oXmlWriter.WriteAttribute(_T("verflip"), bVerFlip);
					oXmlWriter.WriteAttribute(_T("Version"), Version);
					oXmlWriter.WriteAttribute(_T("UseCameraWhiteBalance"), UseCameraWhiteBalance);
					oXmlWriter.WriteAttribute(_T("UseAutoWhiteBalance"), UseAutoWhiteBalance);
					oXmlWriter.WriteAttribute(_T("UseAverageGrayBox"),  UseAverageGrayBox);
					oXmlWriter.WriteAttribute(_T("GrayBox_x"), AverageGrayBox.x);        
					oXmlWriter.WriteAttribute(_T("GrayBox_max_x"), AverageGrayBox.max_x);
					oXmlWriter.WriteAttribute(_T("GrayBox_y"), AverageGrayBox.y);        
					oXmlWriter.WriteAttribute(_T("GrayBox_max_y"), AverageGrayBox.max_y);
					oXmlWriter.WriteAttribute(_T("GrayBox_w"), AverageGrayBox.w);        
					oXmlWriter.WriteAttribute(_T("GrayBox_max_w"), AverageGrayBox.max_w);
					oXmlWriter.WriteAttribute(_T("GrayBox_h"), AverageGrayBox.h);        
					oXmlWriter.WriteAttribute(_T("GrayBox_max_h"), AverageGrayBox.max_h);
					oXmlWriter.WriteAttribute(_T("UseCustomWhiteBalance"), UseCustomWhiteBalance);
					oXmlWriter.WriteAttribute(_T("WhiteBalance_b"), CustomWhiteBalance.b);           
					oXmlWriter.WriteAttribute(_T("WhiteBalance_max_b"), CustomWhiteBalance.max_b);   
					oXmlWriter.WriteAttribute(_T("WhiteBalance_min_b"), CustomWhiteBalance.min_b);   
					oXmlWriter.WriteAttribute(_T("WhiteBalance_g1"), CustomWhiteBalance.g1);        
					oXmlWriter.WriteAttribute(_T("WhiteBalance_min_g1"), CustomWhiteBalance.max_g1);
					oXmlWriter.WriteAttribute(_T("WhiteBalance_min_g1"), CustomWhiteBalance.min_g1);
					oXmlWriter.WriteAttribute(_T("WhiteBalance_g2"), CustomWhiteBalance.g2);        
					oXmlWriter.WriteAttribute(_T("WhiteBalance_max_g2"), CustomWhiteBalance.max_g2);
					oXmlWriter.WriteAttribute(_T("WhiteBalance_min_g2"), CustomWhiteBalance.min_g2);
					oXmlWriter.WriteAttribute(_T("WhiteBalance_r"), CustomWhiteBalance.r);          
					oXmlWriter.WriteAttribute(_T("WhiteBalance_max_r"), CustomWhiteBalance.max_r);  
					oXmlWriter.WriteAttribute(_T("WhiteBalance_min_r"), CustomWhiteBalance.min_r);  
					oXmlWriter.WriteAttribute(_T("UsePresetWhiteBalance"), UsePresetWhiteBalance);
					oXmlWriter.WriteAttribute(_T("PresetWhiteBalance"), PresetWhiteBalance);
					oXmlWriter.WriteAttribute(_T("max_PresetWhiteBalance"), max_PresetWhiteBalance);
					oXmlWriter.WriteAttribute(_T("UseTemperature"), UseTemperature);
					oXmlWriter.WriteAttribute(_T("Temperature_green"), Temperature.green);                 
					oXmlWriter.WriteAttribute(_T("Temperature_max_green"), Temperature.max_green);         
					oXmlWriter.WriteAttribute(_T("Temperature_min_green"), Temperature.min_green);         
					oXmlWriter.WriteAttribute(_T("Temperature_temperature"), Temperature.temperature);     
					oXmlWriter.WriteAttribute(_T("Temperature_max_temperature"), Temperature.max_temperature); 
					oXmlWriter.WriteAttribute(_T("Temperature_min_temperature"), Temperature.min_temperature); 
					oXmlWriter.WriteAttribute(_T("UseCameraMatrix"), UseCameraMatrix);             
					oXmlWriter.WriteAttribute(_T("UseCorrectChromaticAberration"), UseCorrectChromaticAberration);    
					oXmlWriter.WriteAttribute(_T("ChromaticAberration_b"), CorrectChromaticAberration.b);      
					oXmlWriter.WriteAttribute(_T("ChromaticAberration_max_b"), CorrectChromaticAberration.max_b);
					oXmlWriter.WriteAttribute(_T("ChromaticAberration_min_b"), CorrectChromaticAberration.min_b);
					oXmlWriter.WriteAttribute(_T("ChromaticAberration_r"), CorrectChromaticAberration.r);     
					oXmlWriter.WriteAttribute(_T("ChromaticAberration_max_r"), CorrectChromaticAberration.max_r); 
					oXmlWriter.WriteAttribute(_T("ChromaticAberration_min_r"), CorrectChromaticAberration.min_r); 
					oXmlWriter.WriteAttribute(_T("UseDarknessLevel"), UseDarknessLevel);            
					oXmlWriter.WriteAttribute(_T("max_DarknessLevel"), max_DarknessLevel);          
					oXmlWriter.WriteAttribute(_T("min_DarknessLevel"), min_DarknessLevel);          
					oXmlWriter.WriteAttribute(_T("DarknessLevel"), DarknessLevel );                 
					oXmlWriter.WriteAttribute(_T("UseSaturationLevel"), UseSaturationLevel);        
					oXmlWriter.WriteAttribute(_T("SaturationLevel"), SaturationLevel);              
					oXmlWriter.WriteAttribute(_T("max_SaturationLevel"), max_SaturationLevel);      
					oXmlWriter.WriteAttribute(_T("min_SaturationLevel"), min_SaturationLevel);      
					oXmlWriter.WriteAttribute(_T("UseDenoisingThreshold"), UseDenoisingThreshold); 
					oXmlWriter.WriteAttribute(_T("max_DenoisingThreshold"), max_DenoisingThreshold);
					oXmlWriter.WriteAttribute(_T("DenoisingThreshold"), DenoisingThreshold );       
					oXmlWriter.WriteAttribute(_T("UseHighlightMode"), UseHighlightMode);            
					oXmlWriter.WriteAttribute(_T("max_HighlightMode"), max_HighlightMode);            
					oXmlWriter.WriteAttribute(_T("min_HighlightMode"), min_HighlightMode);            
					oXmlWriter.WriteAttribute(_T("HighlightMode"), HighlightMode);                    
					oXmlWriter.WriteAttribute(_T("NoAutoBrighten"), NoAutoBrighten);                  
					oXmlWriter.WriteAttribute(_T("UseAdjustBrightness"), UseAdjustBrightness);        
					oXmlWriter.WriteAttribute(_T("AdjustBrightness"), AdjustBrightness);                 
					oXmlWriter.WriteAttribute(_T("max_AdjustBrightness"), max_AdjustBrightness);          
					oXmlWriter.WriteAttribute(_T("min_AdjustBrightness"), min_AdjustBrightness);          
					oXmlWriter.WriteAttribute(_T("UseInterpolationQuality"), UseInterpolationQuality);    
					oXmlWriter.WriteAttribute(_T("InterpolationQuality"), InterpolationQuality);          
					oXmlWriter.WriteAttribute(_T("max_InterpolationQuality"), max_InterpolationQuality ); 
					oXmlWriter.WriteAttribute(_T("UseMedianFilterPasses"), UseMedianFilterPasses);        
					oXmlWriter.WriteAttribute(_T("MedianFilterPasses"), MedianFilterPasses);         
					oXmlWriter.WriteAttribute(_T("max_MedianFilterPasses"), max_MedianFilterPasses );
					//oXmlWriter.WriteNodeBegin(_T("ImageRaw3"), true);
					//oXmlWriter.WriteAttribute(_T("filetype"), nFileType);
					//oXmlWriter.WriteAttribute(_T("width"), nWidth);
					//oXmlWriter.WriteAttribute(_T("height"), nHeight);
					//oXmlWriter.WriteAttribute(_T("aspectx"), nAspectX);
					//oXmlWriter.WriteAttribute(_T("aspecty"), nAspectY);
					//oXmlWriter.WriteAttribute(_T("resizemode"), nResizeMode);
					//oXmlWriter.WriteAttribute(_T("backcolor"), nBackColor);
					//oXmlWriter.WriteAttribute(_T("backalpha"), nBackAlpha);

				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					SourcePath  = oXmlNode.GetAttributeOrValue(_T("sourcepath"), _T(""));
					bVerFlip    = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("verflip"), _T("0")) );
					Version                           = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("Version"), _T("1")) );
					UseCameraWhiteBalance             = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("UseCameraWhiteBalance"), _T("0")) ) != 0 ? true : false;
					UseAutoWhiteBalance               = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("UseAutoWhiteBalance"), _T("0")) ) != 0 ? true : false;
					UseAverageGrayBox                 = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("UseAverageGrayBox"), _T("0")) ) != 0 ? true : false;
					AverageGrayBox.x                  = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("GrayBox_x"), _T("0")) );
					AverageGrayBox.max_x              = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("GrayBox_max_x"), _T("0")) );
					AverageGrayBox.y                  = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("GrayBox_y"), _T("0")) );
					AverageGrayBox.max_y              = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("GrayBox_max_y"), _T("0")) );
					AverageGrayBox.w                  = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("GrayBox_w"), _T("0")) );
					AverageGrayBox.max_w              = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("GrayBox_max_w"), _T("0")) );
					AverageGrayBox.h                  = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("GrayBox_h"), _T("0")) );
					AverageGrayBox.max_h              = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("GrayBox_max_h"), _T("0")) );
					UseCustomWhiteBalance             = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("UseCustomWhiteBalance"), _T("0")) ) != 0 ? true : false;
					CustomWhiteBalance.b              = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("WhiteBalance_b"), _T("0")) ) );
					CustomWhiteBalance.max_b          = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("WhiteBalance_max_b"), _T("0")) ) );
					CustomWhiteBalance.min_b          = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("WhiteBalance_min_b"), _T("0")) ) );
					CustomWhiteBalance.g1             = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("WhiteBalance_g1"), _T("0")) ) );
					CustomWhiteBalance.max_g1         = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("WhiteBalance_max_g1"), _T("0")) ) );
					CustomWhiteBalance.min_g1         = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("WhiteBalance_min_g1"), _T("0")) ) );
					CustomWhiteBalance.g2             = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("WhiteBalance_g2"), _T("0")) ) );
					CustomWhiteBalance.max_g2         = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("WhiteBalance_max_g2"), _T("0")) ) );
					CustomWhiteBalance.min_g2         = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("WhiteBalance_min_g2"), _T("0")) ) );
					CustomWhiteBalance.r              = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("WhiteBalance_r"), _T("0")) ) );
					CustomWhiteBalance.max_r          = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("WhiteBalance_max_r"), _T("0")) ) );
					CustomWhiteBalance.min_r          = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("WhiteBalance_min_r"), _T("0")) ) );
					UsePresetWhiteBalance             = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("UsePresetWhiteBalance"), _T("0")) ) != 0 ? true : false;
					PresetWhiteBalance                = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("PresetWhiteBalance"), _T("0")) );
					max_PresetWhiteBalance            = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("max_PresetWhiteBalance"), _T("0")) );
					UseTemperature                    = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("UseTemperature"), _T("0")) ) != 0 ? true : false;
					Temperature.green                 = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("Temperature_green"), _T("0")) ) );
					Temperature.max_green             = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("Temperature_max_green"), _T("0")) ) );
					Temperature.min_green             = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("Temperature_min_green"), _T("0")) ) );
					Temperature.temperature           = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("Temperature_temperature"), _T("0")) ) );
					Temperature.max_temperature       = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("Temperature_max_temperature"), _T("0")) ) );
					Temperature.min_temperature       = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("Temperature_min_temperature"), _T("0")) ) );
					UseCameraMatrix                   = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("UseCameraMatrix"), _T("0") ) ) != 0 ? true : false;
					UseCorrectChromaticAberration     = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("UseCorrectChromaticAberration"), _T("0")) ) != 0 ? true : false;
					CorrectChromaticAberration.b      = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("ChromaticAberration_b"), _T("0")) ) );
					CorrectChromaticAberration.max_b  = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("ChromaticAberration_max_b"), _T("0")) ) );
					CorrectChromaticAberration.min_b  = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("ChromaticAberration_min_b"), _T("0")) ) );
					CorrectChromaticAberration.r      = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("ChromaticAberration_r"), _T("0")) ) );
					CorrectChromaticAberration.max_r  = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("ChromaticAberration_max_r"), _T("0")) ) );
					CorrectChromaticAberration.min_r  = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("ChromaticAberration_min_r"), _T("0")) ) );
					UseDarknessLevel                  = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("UseDarknessLevel"), _T("0")) ) != 0 ? true : false;
					max_DarknessLevel                 = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("max_DarknessLevel"), _T("0")) );
					min_DarknessLevel                 = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("min_DarknessLevel"), _T("0")) );
					DarknessLevel                     = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("DarknessLevel"), _T("-1")) );
					UseSaturationLevel                = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("UseSaturationLevel"), _T("0")) ) != 0 ? true : false;
					SaturationLevel                   = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("SaturationLevel"), _T("-1")) );
					max_SaturationLevel               = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("max_SaturationLevel"), _T("-1")) );
					min_SaturationLevel               = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("min_SaturationLevel"), _T("-1")) );
					UseDenoisingThreshold             = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("UseDenoisingThreshold"), _T("0")) ) != 0 ? true : false;
					max_DenoisingThreshold            = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("max_DenoisingThreshold"), _T("0.0")) ) );
					DenoisingThreshold                = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("DenoisingThreshold"), _T("0.0")) ) );
					UseHighlightMode                  = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("UseHighlightMode"), _T("0")) ) != 0 ? true : false;
					max_HighlightMode                 = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("max_HighlightMode"), _T("0")) );
					min_HighlightMode                 = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("min_HighlightMode"), _T("0")) );
					HighlightMode                     = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("HighlightMode"), _T("0")) );
					NoAutoBrighten                    = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("NoAutoBrighten"), _T("0")) ) != 0 ? true : false;
					UseAdjustBrightness               = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("UseAdjustBrightness"), _T("0")) ) != 0 ? true : false;
					AdjustBrightness                  = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("AdjustBrightness"), _T("1.0")) ) );
					max_AdjustBrightness              = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("max_AdjustBrightness"), _T("0.0")) ) );
					min_AdjustBrightness              = float(XmlUtils::GetDouble( oXmlNode.GetAttribute(_T("min_AdjustBrightness"), _T("0.0")) ) );
					UseInterpolationQuality           = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("UseInterpolationQuality"), _T("0")) ) != 0 ? true : false;
					InterpolationQuality              = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("InterpolationQuality"), _T("3")) );
					max_InterpolationQuality          = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("max_InterpolationQuality"), _T("3")) );
					UseMedianFilterPasses             = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("UseMedianFilterPasses"), _T("0")) ) != 0 ? true : false;
					MedianFilterPasses                = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("MedianFilterPasses"), _T("0")) );
					max_MedianFilterPasses            = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("max_MedianFilterPasses"), _T("0")) );
					//nFileType   = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("filetype"), _T("0")) );
					//nWidth      = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("width"), _T("0")) );
					//nHeight     = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("height"), _T("0")) );
					//nAspectX    = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("aspectx"), _T("0")) );
					//nAspectY    = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("aspecty"), _T("0")) );
					//nResizeMode = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("resizemode"), _T("0")) );
					//nBackColor  = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("backcolor"), _T("0")) );
					//nBackAlpha  = XmlUtils::GetInteger( oXmlNode.GetAttribute(_T("backalpha"), _T("255")) );
				}
				
				virtual void InternalClear()
				{
					SourcePath  = _T("");
					bVerFlip    = FALSE;
					Version = 1;
					UseCameraWhiteBalance = false;
					UseAutoWhiteBalance = false;
					UseAverageGrayBox = false;
					AverageGrayBox.Reset();
					UseCustomWhiteBalance = false;
					CustomWhiteBalance.Reset();
					UsePresetWhiteBalance = false;
					PresetWhiteBalance = 0;
					max_PresetWhiteBalance = 0;
					UseTemperature = false;
					Temperature.Reset();
					UseCameraMatrix = false;
					UseCorrectChromaticAberration = false;
					CorrectChromaticAberration.Reset();
					UseDarknessLevel = false;
					max_DarknessLevel = 0;
					min_DarknessLevel = 0;
					DarknessLevel = -1;
					UseSaturationLevel = false;
					SaturationLevel = -1;
					max_SaturationLevel = 0;
					min_SaturationLevel = 0;
					UseDenoisingThreshold = false;
					max_DenoisingThreshold = 0.0;
					DenoisingThreshold = 0.0;
					UseHighlightMode = false;
					max_HighlightMode = 0;
					max_HighlightMode = 0;
					HighlightMode = 0;
					NoAutoBrighten = false;
					UseAdjustBrightness = false;
					AdjustBrightness = 1.0;
					max_AdjustBrightness = 0.0;
					min_AdjustBrightness = 0.0;
					UseInterpolationQuality = false;
					InterpolationQuality = 3;
					UseMedianFilterPasses = false;
					MedianFilterPasses = 0;

					strSettingXML = _T("");
				}


				void WriteString(CString strValue)
				{
					strSettingXML += strValue;
				}
				void WriteInteger(int Value)
				{
					TCHAR str[33];
					
					_itot(Value, str, 10);

					strSettingXML += str;
				}
				void WriteDouble(double Value)
				{
					CString str;
					
					str.Format(_T("%lf"), Value);

					strSettingXML += str;
				}
				void WriteAttribute(CString strAttributeName, CString strAttributeValue)
				{
					strSettingXML += _T(" ") + strAttributeName + _T("=\"") + strAttributeValue + _T("\"");
				}


			protected:

				struct SCustomWhiteBalance
				{
					float r,g1,b,g2;
					float max_r,max_g1,max_b,max_g2;
					float min_r,min_g1,min_b,min_g2;
					SCustomWhiteBalance(float r_=0.0,float g1_=0.0,float b_=0.0,float g2_=0.0):r(r_),g1(g1_),b(b_),g2(g2_)
					{
						max_r = 9.0; max_g1 = 9.0; max_g2 = 9.0; max_b = 9.0;
						min_r = 1.0; min_g1 = 1.0; min_g2 = 1.0; min_b = 1.0;
					};
					void Reset()
					{
						max_r = 9.0; max_g1 = 9.0; max_g2 = 9.0; max_b = 9.0;
						min_r = 1.0; min_g1 = 1.0; min_g2 = 1.0; min_b = 1.0;
						r = g1 = b = g2 = 0.0;
					}
					
				};

				struct SAverageGrayBox
				{
					int x,y,w,h;
					int max_x,max_y,max_w,max_h;
					SAverageGrayBox(int x_ =0, int max_x_ =0
									,int y_=0, int max_y_ =0
									,int w_=0, int max_w_ =0
									,int h_=0, int max_h_ =0)
													:x(x_),y(y_),w(w_),h(h_),max_x(max_x_),max_y(max_y_),max_w(max_w_),max_h(max_h_)
					{
					};

					void Reset()
					{
						x = y = w = h = 0;
						max_x = max_y = max_w = max_h = 0;
					}
					
				};

				struct SCorrectChromaticAberration
				{
					float r,b;
					float min_r,min_b;
					float max_r,max_b;
					SCorrectChromaticAberration(float r_=0.0,float min_r_ =0.0,float max_r_ =0.0, float b_=0.0, float min_b_ = 0.0, float max_b_ = 0.0):
					r(r_),b(b_),max_r(max_r_),max_b(max_b_),min_r(min_r_),min_b(min_b_)
					{
					}

					void Reset()
					{
						r = b = 0.0;
						min_r = 0.0;
						max_r = 0.0;
						min_b = 0.0;
						max_b = 0.0;
					}
					
				};

				struct STemperature
				{
					float temperature;
					float green;
					float max_temperature,min_temperature;
					float max_green,min_green;
					//STemperature(float temperature_,float green_):temperature(temperature_),green(green_)
					//{
					//	max_temperature = 15000;
					//	min_temperature = 2000;
					//	max_green = 2.5;
					//	min_green = 0.2;
					//};
					void Reset()
					{
						temperature = 0;
						green = 0.0;
						max_temperature = 15000;
						min_temperature = 2000;
						max_green = float(2.5);
						min_green = float(0.2);
					}
				};


			public:
				
				FileLoadRaw()
				{
					Initialize(c_nSerializeFileLoadRaw, c_strSerializeFileLoadRaw);

					Clear();
				}

				BOOL XMLToString()
				{
					strSettingXML = _T("");
					strSettingXML += _T("<ImageRaw3 mode=\"0\">");
					
					strSettingXML += _T("<SimpleMode ver=\"");
					WriteInteger(Version);
					strSettingXML += _T("\">");
					
					strSettingXML += _T("<CameraWhiteBalance use=\"");
					WriteInteger(int(UseCameraWhiteBalance));
					strSettingXML += _T("\"/>");

					strSettingXML += _T("<AutoWhiteBalance use=\"");
					WriteInteger(int(UseAutoWhiteBalance));
					strSettingXML += _T("\"/>");

					//AverageGrayBox

					strSettingXML += _T("<AverageGrayBox use=\"");
					WriteInteger(int(UseAverageGrayBox));
					strSettingXML += _T("\">");
	
					strSettingXML += _T("<X max=\"");
					WriteInteger(AverageGrayBox.max_x);
					strSettingXML += _T("\">");
					WriteInteger(AverageGrayBox.x);
					strSettingXML += _T("</X>");

					strSettingXML += _T("<Y max=\"");
					WriteInteger(AverageGrayBox.max_y);
					strSettingXML += _T("\">");
					WriteInteger(AverageGrayBox.y);
					strSettingXML += _T("</Y>");

					strSettingXML += _T("<W max=\"");
					WriteInteger(AverageGrayBox.max_w);
					strSettingXML += _T("\">");
					WriteInteger(AverageGrayBox.w);
					strSettingXML += _T("</W>");

					strSettingXML += _T("<H max=\"");
					WriteInteger(AverageGrayBox.max_h);
					strSettingXML += _T("\">");
					WriteInteger(AverageGrayBox.h);
					strSettingXML += _T("</H>");

					strSettingXML += _T("</AverageGrayBox>");

					//CustomWhiteBalance

					strSettingXML += _T("<CustomWhiteBalance use=\"");
					WriteInteger(int(UseCustomWhiteBalance));
					strSettingXML += _T("\">");
	
					strSettingXML += _T("<R min=\"");
					WriteDouble(CustomWhiteBalance.min_r);
					strSettingXML += _T("\" max=\"");
					WriteDouble(CustomWhiteBalance.max_r);
					strSettingXML += _T("\">");
					WriteDouble(CustomWhiteBalance.r);
					strSettingXML += _T("</R>");

					strSettingXML += _T("<G1 min=\"");
					WriteDouble(CustomWhiteBalance.min_g1);
					strSettingXML += _T("\" max=\"");
					WriteDouble(CustomWhiteBalance.max_g1);
					strSettingXML += _T("\">");
					WriteDouble(CustomWhiteBalance.g1);
					strSettingXML += _T("</G1>");

					strSettingXML += _T("<B min=\"");
					WriteDouble(CustomWhiteBalance.min_b);
					strSettingXML += _T("\" max=\"");
					WriteDouble(CustomWhiteBalance.max_b);
					strSettingXML += _T("\">");
					WriteDouble(CustomWhiteBalance.b);
					strSettingXML += _T("</B>");

					strSettingXML += _T("<G2 min=\"");
					WriteDouble(CustomWhiteBalance.min_g2);
					strSettingXML += _T("\" max=\"");
					WriteDouble(CustomWhiteBalance.max_g2);
					strSettingXML += _T("\">");
					WriteDouble(CustomWhiteBalance.g2);
					strSettingXML += _T("</G2>");

					strSettingXML += _T("</CustomWhiteBalance>");

					//PresetWhiteBalance 

					strSettingXML += _T("<PresetWhiteBalance use=\"");
					WriteInteger(int(UsePresetWhiteBalance));
					strSettingXML += _T("\" max=\"");
					WriteInteger(max_PresetWhiteBalance);
					strSettingXML += _T("\">");
					WriteInteger(PresetWhiteBalance);
					strSettingXML += _T("</PresetWhiteBalance>");

 					//Temperature 

					strSettingXML += _T("<Temperature use=\"");
					WriteInteger(int(UseTemperature));
					strSettingXML += _T("\">");
	
					strSettingXML += _T("<TemperatureValue min=\"");
					WriteDouble(Temperature.min_temperature);
					strSettingXML += _T("\" max=\"");
					WriteDouble(Temperature.max_temperature);
					strSettingXML += _T("\">");
					WriteDouble(Temperature.temperature);
					strSettingXML += _T("</TemperatureValue>");

					strSettingXML += _T("<Green min=\"");
					WriteDouble(Temperature.min_green);
					strSettingXML += _T("\" max=\"");
					WriteDouble(Temperature.max_green);
					strSettingXML += _T("\">");
					WriteDouble(Temperature.green);
					strSettingXML += _T("</Green>");

					strSettingXML += _T("</Temperature>");

					//CameraMatrix

					strSettingXML += _T("<CameraMatrix use=\"");
					WriteInteger(int(UseCameraMatrix));
					strSettingXML += _T("\"/>");

 					//CorrectChromaticAberration

					strSettingXML += _T("<CorrectChromaticAberration use=\"");
					WriteInteger(int(UseCorrectChromaticAberration));
					strSettingXML += _T("\">");
	
					strSettingXML += _T("<R min=\"");
					WriteDouble(CorrectChromaticAberration.min_r);
					strSettingXML += _T("\" max=\"");
					WriteDouble(CorrectChromaticAberration.max_r);
					strSettingXML += _T("\">");
					WriteDouble(CorrectChromaticAberration.r);
					strSettingXML += _T("</R>");

					strSettingXML += _T("<B min=\"");
					WriteDouble(CorrectChromaticAberration.min_b);
					strSettingXML += _T("\" max=\"");
					WriteDouble(CorrectChromaticAberration.max_b);
					strSettingXML += _T("\">");
					WriteDouble(CorrectChromaticAberration.b);
					strSettingXML += _T("</B>");

					strSettingXML += _T("</CorrectChromaticAberration>");
					
					//DarknessLevel

					strSettingXML += _T("<DarknessLevel use=\"");
					WriteInteger(UseDarknessLevel);
					strSettingXML += _T("\" min=\"");
					WriteInteger(min_DarknessLevel);
					strSettingXML += _T("\" max=\"");
					WriteInteger(max_DarknessLevel);
					strSettingXML += _T("\">");
					WriteInteger(DarknessLevel);
					strSettingXML += _T("</DarknessLevel>");

					//SaturationLevel

					strSettingXML += _T("<SaturationLevel use=\"");
					WriteInteger(UseSaturationLevel);
					strSettingXML += _T("\" min=\"");
					WriteInteger(min_SaturationLevel);
					strSettingXML += _T("\" max=\"");
					WriteInteger(max_SaturationLevel);
					strSettingXML += _T("\">");
					WriteInteger(SaturationLevel);
					strSettingXML += _T("</SaturationLevel>");

					//DenoisingThreshold

					strSettingXML += _T("<DenoisingThreshold use=\"");
					WriteInteger(UseDenoisingThreshold);
					strSettingXML += _T("\" max=\"");
					WriteDouble(max_DenoisingThreshold);
					strSettingXML += _T("\">");
					WriteDouble(DenoisingThreshold);
					strSettingXML += _T("</DenoisingThreshold>");

					//HighlightMode

					strSettingXML += _T("<HighlightMode use=\"");
					WriteInteger(UseHighlightMode);
					strSettingXML += _T("\" max=\"");
					WriteInteger(max_HighlightMode);
					strSettingXML += _T("\">");
					WriteInteger(HighlightMode);
					strSettingXML += _T("</HighlightMode>");

					//NoAutoBrighten

					strSettingXML += _T("<NoAutoBrighten use=\"");
					WriteInteger(int(NoAutoBrighten));
					strSettingXML += _T("\"/>");

					//AdjustBrightness

					strSettingXML += _T("<AdjustBrightness use=\"");
					WriteInteger(UseAdjustBrightness);
					strSettingXML += _T("\" min=\"");
					WriteDouble(min_AdjustBrightness);
					strSettingXML += _T("\" max=\"");
					WriteDouble(max_AdjustBrightness);
					strSettingXML += _T("\">");
					WriteDouble(AdjustBrightness);
					strSettingXML += _T("</AdjustBrightness>");

					//InterpolationQuality

					strSettingXML += _T("<InterpolationQuality use=\"");
					WriteInteger(UseHighlightMode);
					strSettingXML += _T("\" max=\"");
					WriteInteger(max_InterpolationQuality);
					strSettingXML += _T("\">");
					WriteInteger(InterpolationQuality);
					strSettingXML += _T("</InterpolationQuality>");

					//MedianFilterPasses

					strSettingXML += _T("<MedianFilterPasses use=\"");
					WriteInteger(UseMedianFilterPasses);
					strSettingXML += _T("\" max=\"");
					WriteInteger(max_MedianFilterPasses);
					strSettingXML += _T("\">");
					WriteInteger(MedianFilterPasses);
					strSettingXML += _T("</MedianFilterPasses>");

					strSettingXML += _T("</SimpleMode>");
					strSettingXML += _T("</ImageRaw3>");

					return TRUE;	
				}
					
			public:
				
				CString SourcePath; // путь к файлу
				
				BOOL bVerFlip;   // переворачивать ли файл при загрузке
				//int nWidth;      // требуемая ширина (nWidth <= 0 - оригинальная ширина)
				//int nHeight;     // требуемая высота (nHeight <= 0 - оригинальная высота)
				//int nAspectX;    // требуемый аспект по X (nAspectX <= 0 - оригинальный аспект)
				//int nAspectY;    // требуемый аспект по Y (nAspectY <= 0 - оригинальный аспект)
				//int nResizeMode; // режим ресайзинга
				//int nBackColor;  // цвет для полей 
				//int nBackAlpha;  // альфа для полей

				int							Version;				//!<	Версия настроек
				bool						UseCameraWhiteBalance;	//!<	Баланс белого камеры 
				bool						UseAutoWhiteBalance;	//!<	Автоматический баланс белого
				
				bool						UseAverageGrayBox;		//!<	Использования серой области для установки баланса белого
				SAverageGrayBox				AverageGrayBox;			//!<	Серая область для установки баланса белого
				
				bool						UseCustomWhiteBalance;	//!<	Использование пользовательских значения для установки баланса белого
				SCustomWhiteBalance			CustomWhiteBalance;		//!<	Пользовательские значения баланса белого
				
				bool						UsePresetWhiteBalance;	//!<	Использование предустановленных настроек баланса белого
				int							PresetWhiteBalance;		//!<	Предустановленные настройки баланса белого
				int							max_PresetWhiteBalance;	//!<	Максимальное значение параметра PresetWhiteBalance
				
				bool						UseTemperature;			//!<	Использование пользовательских значений цветовой температуры и уровня зеленого
				STemperature				Temperature;			//!<	Пользовательские значения цветовой температуры и уровня зеленого
				
				bool						UseCameraMatrix;		//!<	use camera matrix
				
				bool						UseCorrectChromaticAberration;	//!<	Коррекция хроматических аберраций
				SCorrectChromaticAberration	CorrectChromaticAberration;		//!<	Значения для коррекции хроматических аберраций

				bool						UseDarknessLevel;		//!<	Установка уровня черного
				int							DarknessLevel;			//!<	Значение уровня черного
				int							max_DarknessLevel;		//!<	Максимальное значение параметра
				int							min_DarknessLevel;		//!<	Минимальное значение параметра

				bool						UseSaturationLevel;		//!<	Установка уровня насыщенности
				int							SaturationLevel;		//!<	Уровень насыщенности
				int							max_SaturationLevel;	//!<	Максимальное значение параметра \a SaturationLevel
				int							min_SaturationLevel;	//!<	Минимальное значение параметра \a SaturationLevel

				bool						UseDenoisingThreshold;	//!<	Использование подавления шумов
				float						DenoisingThreshold;		//!<	Значение порога для подавления шумов
				float						max_DenoisingThreshold;	//!<	Максимальное значение параметра \a DenoisingThreshold

				bool						UseHighlightMode;		//!<	Обработка пересвеченных областей
				int							HighlightMode;			//!<	Режим обработки пересвеченных областей
				int							min_HighlightMode;		//!<	Минимальное	значение параметра HighlightMode
				int							max_HighlightMode;		//!<	Максимальное значение параметра HighlightMode

				bool						NoAutoBrighten;			//!<	no auto brighten

				bool						UseAdjustBrightness;	//!<	Установка параметра яркости
				float						AdjustBrightness;		//!<	Значение параметра яркости
				float						min_AdjustBrightness;	//!<	Минимальное значение параметра \a AdjustBrightness
				float						max_AdjustBrightness;	//!<	Максимальное значение параметра \a AdjustBrightness

				bool						UseInterpolationQuality;	//!<	Использование параметра режима интерполяции
				int							InterpolationQuality;		//!<	Режим интерполяции
				int							max_InterpolationQuality;	//!<	Максимальное значение параметра \a InterpolationQuality

				bool						UseMedianFilterPasses;	//!<	Использование медианного фильтра
				int							MedianFilterPasses;		//!<	Число проходов медианного фильтра
				int							max_MedianFilterPasses;	//!<	Максимальное значение параметра \a MedianFilterPasses

				CString                     strSettingXML;


			};
			class FileSaveAsBmp : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) + DestinationPath.GetLength() + 1;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteString(DestinationPath);
					pStream->WriteLong(Format);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					DestinationPath = pStream->ReadString();
					Format = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("destinationpath"), DestinationPath);
					oXmlWriter.WriteAttribute(_T("format"), Format);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					DestinationPath = oXmlNode.GetAttributeOrValue(_T("destinationpath"), _T(""));
					Format = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("format"), _T("888")));
					dpi = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("dpi"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					DestinationPath = _T("");
					Format = ImageStudio::IO::c_nFormat_Uncompressed_24bpp;
					dpi = 0;
				}
				
			public:
				
				FileSaveAsBmp()
				{
					Initialize(c_nSerializeFileSaveAsBmp, c_strSerializeFileSaveAsBmp);

					Clear();
				}
					
			public:
				
				CString DestinationPath;
				int Format;
				int dpi;

				/* @desc:
				ImageStudio-Serialize-File-FileSaveAsBmp (c_nSerializeFileSaveAsBmp, c_strSerializeFileSaveAsBmp)
				{
					CString DestinationPath = "";						// xml имя - destinationpath; путь к файлу, в который будет сохранена картинка
					int Format = c_nFormat_Uncompressed_24bpp;			// xml имя - format; формат выходного файла, см. константы в ImageStudio::IO
				}
				*/
			};
			class FileSaveAsGif : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) + DestinationPath.GetLength() + 1;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteString(DestinationPath);
					pStream->WriteLong(Format);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					DestinationPath = pStream->ReadString();
					Format = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("destinationpath"), DestinationPath);
					oXmlWriter.WriteAttribute(_T("format"), Format);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					DestinationPath = oXmlNode.GetAttributeOrValue(_T("destinationpath"), _T(""));
					Format = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("format"), _T("888")));
					dpi = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("dpi"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					DestinationPath = _T("");
					Format = ImageStudio::IO::c_nFormat_Uncompressed_24bpp;
					dpi = 0;
				}
				
			public:
				
				FileSaveAsGif()
				{
					Initialize(c_nSerializeFileSaveAsGif, c_strSerializeFileSaveAsGif);

					Clear();
				}
					
			public:
				
				CString DestinationPath;
				int Format;
				int dpi;

				/* @desc:
				ImageStudio-Serialize-File-FileSaveAsGif (c_nSerializeFileSaveAsGif, c_strSerializeFileSaveAsGif)
				{
					CString DestinationPath = "";						// xml имя - destinationpath; путь к файлу, в который будет сохранена картинка
					int Format = c_nFormat_Uncompressed_24bpp;			// xml имя - format; формат выходного файла, см. константы в ImageStudio::IO
				}
				*/
			};
			class FileSaveAsJpeg : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 2*sizeof(int) + DestinationPath.GetLength() + 1;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteString(DestinationPath);
					pStream->WriteLong(Format);
					pStream->WriteLong(Quality);
					pStream->WriteString(MetaDataXML);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					DestinationPath = pStream->ReadString();
					Format = pStream->ReadLong();
					Quality = pStream->ReadLong();
					MetaDataXML=pStream->ReadString();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("destinationpath"), DestinationPath);
					oXmlWriter.WriteAttribute(_T("format"), Format);
					oXmlWriter.WriteAttribute(_T("quality"), Quality);
					oXmlWriter.WriteString(MetaDataXML);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					DestinationPath = oXmlNode.GetAttributeOrValue(_T("destinationpath"), _T(""));
					Format = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("format"), _T("888")));
					Quality = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("quality"), _T("90")));
					dpi = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("dpi"), _T("0")));

					XmlUtils::CXmlNode nodeMetaData;
					oXmlNode.GetNode(_T("MetaData"),nodeMetaData);

					if (nodeMetaData.IsValid())
					{
						MetaDataXML = nodeMetaData.GetXml(_T(""));

					}
				}
				
				virtual void InternalClear()
				{
					DestinationPath = _T("");
					Format = ImageStudio::IO::c_nFormat_Uncompressed_24bpp;
					Quality = 90;
					dpi = 0;
					MetaDataXML=_T("");
				}
				
			public:
				
				FileSaveAsJpeg()
				{
					Initialize(c_nSerializeFileSaveAsJpeg, c_strSerializeFileSaveAsJpeg);

					Clear();
				}
					
			public:
				CString	MetaDataXML;
				CString DestinationPath;
				int Format;
				int Quality;
				int dpi;

				/* @desc:
				ImageStudio-Serialize-File-FileSaveAsJpeg (c_nSerializeFileSaveAsJpeg, c_strSerializeFileSaveAsJpeg)
				{
					CString DestinationPath = "";						// xml имя - destinationpath; путь к файлу, в который будет сохранена картинка
					int Format = c_nFormat_Uncompressed_24bpp;			// xml имя - format; формат выходного файла, см. константы в ImageStudio::IO
					int Quality = 90;									// xml имя - quality; качество сжатия для jpeg компрессии, измеряется в процентах 0..100
				}
				*/
			};
			class FileSaveAsPng : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) + DestinationPath.GetLength() + 1;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteString(DestinationPath);
					pStream->WriteLong(Format);
					pStream->WriteString(MetaDataXML);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					DestinationPath = pStream->ReadString();
					Format = pStream->ReadLong();
					MetaDataXML=pStream->ReadString();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("destinationpath"), DestinationPath);
					oXmlWriter.WriteAttribute(_T("format"), Format);
					oXmlWriter.WriteString(MetaDataXML);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					DestinationPath = oXmlNode.GetAttributeOrValue(_T("destinationpath"), _T(""));
					Format = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("format"), _T("888")));
					dpi = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("dpi"), _T("0")));
				
					XmlUtils::CXmlNode nodeMetaData;
					oXmlNode.GetNode(_T("MetaData"),nodeMetaData);

					if (nodeMetaData.IsValid())
					{
						MetaDataXML = nodeMetaData.GetXml(_T(""));

					}				
				}
				
				virtual void InternalClear()
				{
					DestinationPath = _T("");
					Format = ImageStudio::IO::c_nFormat_Uncompressed_24bpp;
					dpi = 0;
					MetaDataXML=_T("");
				}
				
			public:
				
				FileSaveAsPng()
				{
					Initialize(c_nSerializeFileSaveAsPng, c_strSerializeFileSaveAsPng);

					Clear();
				}
					
			public:
				CString	MetaDataXML;
				CString DestinationPath;
				int Format;
				int dpi;

				/* @desc:
				ImageStudio-Serialize-File-FileSaveAsPng (c_nSerializeFileSaveAsPng, c_strSerializeFileSaveAsPng)
				{
					CString DestinationPath = "";						// xml имя - destinationpath; путь к файлу, в который будет сохранена картинка
					int Format = c_nFormat_Uncompressed_24bpp;			// xml имя - format; формат выходного файла, см. константы в ImageStudio::IO
				}
				*/
			};
			class FileSaveAsTiff : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 2*sizeof(int) + DestinationPath.GetLength() + 1;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteString(DestinationPath);
					pStream->WriteLong(Format);
					pStream->WriteLong(Compression);
					pStream->WriteString(MetaDataXML);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					DestinationPath = pStream->ReadString();
					Format = pStream->ReadLong();
					Compression = pStream->ReadLong();
					MetaDataXML=pStream->ReadString();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("destinationpath"), DestinationPath);
					oXmlWriter.WriteAttribute(_T("format"), Format);
					oXmlWriter.WriteAttribute(_T("compression"), Compression);
					oXmlWriter.WriteString(MetaDataXML);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					DestinationPath = oXmlNode.GetAttributeOrValue(_T("destinationpath"), _T(""));
					Format = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("format"), _T("888")));
					Compression = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("compression"), _T("0")));
					dpi = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("dpi"), _T("0")));
					
					XmlUtils::CXmlNode nodeMetaData;
					oXmlNode.GetNode(_T("MetaData"),nodeMetaData);

					if (nodeMetaData.IsValid())
					{
						MetaDataXML = nodeMetaData.GetXml(_T(""));

					}				
				}				
				virtual void InternalClear()
				{
					DestinationPath = _T("");
					Format = ImageStudio::IO::c_nFormat_Uncompressed_24bpp;
					Compression = ImageStudio::IO::c_nFormat_Tiff_CompressionNone;
					dpi = 0;
					MetaDataXML=_T("");
				}
				
			public:
				
				FileSaveAsTiff()
				{
					Initialize(c_nSerializeFileSaveAsTiff, c_strSerializeFileSaveAsTiff);

					Clear();
				}
					
			public:
				CString	MetaDataXML;				
				CString DestinationPath;
				int Format;
				int Compression;
				int dpi;

				/* @desc:
				ImageStudio-Serialize-File-FileSaveAsTiff (c_nSerializeFileSaveAsTiff, c_strSerializeFileSaveAsTiff)
				{
					CString DestinationPath = "";						// xml имя - destinationpath; путь к файлу, в который будет сохранена картинка
					int Format = c_nFormat_Uncompressed_24bpp;			// xml имя - format; формат выходного файла, см. константы в ImageStudio::IO
					int Compression = c_nFormat_Tiff_CompressionNone;	// xml имя - compression; тип tiff компрессии, см. константы в ImageStudio::IO
				}
				*/
			};
			class FileSaveAsWmf : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) + DestinationPath.GetLength() + 1;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteString(DestinationPath);
					pStream->WriteLong(Format);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					DestinationPath = pStream->ReadString();
					Format = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("destinationpath"), DestinationPath);
					oXmlWriter.WriteAttribute(_T("format"), Format);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					DestinationPath = oXmlNode.GetAttributeOrValue(_T("destinationpath"), _T(""));
					Format = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("format"), _T("888")));
					dpi = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("dpi"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					DestinationPath = _T("");
					Format = ImageStudio::IO::c_nFormat_Uncompressed_24bpp;
					dpi = 0;
				}
				
			public:
				
				FileSaveAsWmf()
				{
					Initialize(c_nSerializeFileSaveAsWmf, c_strSerializeFileSaveAsWmf);

					Clear();
				}
					
			public:
				
				CString DestinationPath;
				int Format;
				int dpi;

				/* @desc:
				ImageStudio-Serialize-File-FileSaveAsWmf (c_nSerializeFileSaveAsWmf, c_strSerializeFileSaveAsWmf)
				{
					CString DestinationPath = "";				// xml имя - destinationpath; путь к файлу, в который будет сохранена картинка
					int Format = c_nFormat_Uncompressed_24bpp;	// xml имя - format; формат выходного файла, см. константы в ImageStudio::IO
				}
				*/
			};
			class FileSaveAsEmf : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) + DestinationPath.GetLength() + 1;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteString(DestinationPath);
					pStream->WriteLong(Format);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					DestinationPath = pStream->ReadString();
					Format = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("destinationpath"), DestinationPath);
					oXmlWriter.WriteAttribute(_T("format"), Format);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					DestinationPath = oXmlNode.GetAttributeOrValue(_T("destinationpath"), _T(""));
					Format = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("format"), _T("888")));
					dpi = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("dpi"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					DestinationPath = _T("");
					Format = ImageStudio::IO::c_nFormat_Uncompressed_24bpp;
					dpi = 0;
				}
				
			public:
				
				FileSaveAsEmf()
				{
					Initialize(c_nSerializeFileSaveAsEmf, c_strSerializeFileSaveAsEmf);

					Clear();
				}
					
			public:
				
				CString DestinationPath;
				int Format;
				int dpi;

				/* @desc:
				ImageStudio-Serialize-File-FileSaveAsEmf (c_nSerializeFileSaveAsEmf, c_strSerializeFileSaveAsEmf)
				{
					CString DestinationPath = "";				// xml имя - destinationpath; путь к файлу, в который будет сохранена картинка
					int Format = c_nFormat_Uncompressed_24bpp;	// xml имя - format; формат выходного файла, см. константы в ImageStudio::IO
				}
				*/
			};
			class FileSaveAsPcx : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) + DestinationPath.GetLength() + 1;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteString(DestinationPath);
					pStream->WriteLong(Format);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					DestinationPath = pStream->ReadString();
					Format = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("destinationpath"), DestinationPath);
					oXmlWriter.WriteAttribute(_T("format"), Format);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					DestinationPath = oXmlNode.GetAttributeOrValue(_T("destinationpath"), _T(""));
					Format = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("format"), _T("888")));
				}
				
				virtual void InternalClear()
				{
					DestinationPath = _T("");
					Format = ImageStudio::IO::c_nFormat_Uncompressed_24bpp;
				}
				
			public:
				
				FileSaveAsPcx()
				{
					Initialize(c_nSerializeFileSaveAsPcx, c_strSerializeFileSaveAsPcx);

					Clear();
				}
					
			public:
				
				CString DestinationPath;
				int Format;

				/* @desc:
				ImageStudio-Serialize-File-FileSaveAsPcx (c_nSerializeFileSaveAsPcx, c_strSerializeFileSaveAsPcx)
				{
					CString DestinationPath = "";				// xml имя - destinationpath; путь к файлу, в который будет сохранена картинка
					int Format = c_nFormat_Uncompressed_24bpp;	// xml имя - format; формат выходного файла, см. константы в ImageStudio::IO
				}
				*/
			};
			class FileSaveAsTga : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) + DestinationPath.GetLength() + 1;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteString(DestinationPath);
					pStream->WriteLong(Format);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					DestinationPath = pStream->ReadString();
					Format = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("destinationpath"), DestinationPath);
					oXmlWriter.WriteAttribute(_T("format"), Format);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					DestinationPath = oXmlNode.GetAttributeOrValue(_T("destinationpath"), _T(""));
					Format = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("format"), _T("888")));
				}
				
				virtual void InternalClear()
				{
					DestinationPath = _T("");
					Format = ImageStudio::IO::c_nFormat_Uncompressed_24bpp;
				}
				
			public:
				
				FileSaveAsTga()
				{
					Initialize(c_nSerializeFileSaveAsTga, c_strSerializeFileSaveAsTga);

					Clear();
				}
					
			public:
				
				CString DestinationPath;
				int Format;

				/* @desc:
				ImageStudio-Serialize-File-FileSaveAsTga (c_nSerializeFileSaveAsTga, c_strSerializeFileSaveAsTga)
				{
					CString DestinationPath = "";				// xml имя - destinationpath; путь к файлу, в который будет сохранена картинка
					int Format = c_nFormat_Uncompressed_24bpp;	// xml имя - format; формат выходного файла, см. константы в ImageStudio::IO
				}
				*/
			};
			class FileSaveAsRas : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) + DestinationPath.GetLength() + 1;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteString(DestinationPath);
					pStream->WriteLong(Format);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					DestinationPath = pStream->ReadString();
					Format = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("destinationpath"), DestinationPath);
					oXmlWriter.WriteAttribute(_T("format"), Format);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					DestinationPath = oXmlNode.GetAttributeOrValue(_T("destinationpath"), _T(""));
					Format = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("format"), _T("888")));
				}
				
				virtual void InternalClear()
				{
					DestinationPath = _T("");
					Format = ImageStudio::IO::c_nFormat_Uncompressed_24bpp;
				}
				
			public:
				
				FileSaveAsRas()
				{
					Initialize(c_nSerializeFileSaveAsRas, c_strSerializeFileSaveAsRas);

					Clear();
				}
					
			public:
				
				CString DestinationPath;
				int Format;

				/* @desc:
				ImageStudio-Serialize-File-FileSaveAsRas (c_nSerializeFileSaveAsRas, c_strSerializeFileSaveAsRas)
				{
					CString DestinationPath = "";				// xml имя - destinationpath; путь к файлу, в который будет сохранена картинка
					int Format = c_nFormat_Uncompressed_24bpp;	// xml имя - format; формат выходного файла, см. константы в ImageStudio::IO
				}
				*/
			};
			class FileSaveAsIPod : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) + DestinationPath.GetLength() + 1;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteString(DestinationPath);
					pStream->WriteLong(Format);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					DestinationPath = pStream->ReadString();
					Format = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("destinationpath"), DestinationPath);
					oXmlWriter.WriteAttribute(_T("format"), Format);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					DestinationPath = oXmlNode.GetAttributeOrValue(_T("destinationpath"), _T(""));
					Format = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("format"), _T("80000")));
				}
				
				virtual void InternalClear()
				{
					DestinationPath = _T("");
					Format = ImageStudio::IO::c_nFormat_IPod_80000;
				}
				
			public:
				
				FileSaveAsIPod()
				{
					Initialize(c_nSerializeFileSaveAsIPod, c_strSerializeFileSaveAsIPod);

					Clear();
				}
					
			public:
				
				CString DestinationPath;
				int Format;

				/* @desc:
				ImageStudio-Serialize-File-FileSaveAsIPod (c_nSerializeFileSaveAsIPod, c_strSerializeFileSaveAsIPod)
				{
					CString DestinationPath = "";		// xml имя - destinationpath; путь к файлу, в который будет сохранена картинка
					int Format = c_nFormat_IPod_80000;	// xml имя - format; формат выходного файла, см. константы в ImageStudio::IO
				}
				*/
			};
			class FileSaveAsPsd : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return sizeof(int) + DestinationPath.GetLength() + 1;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteString(DestinationPath);
					pStream->WriteLong(Format);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					DestinationPath = pStream->ReadString();
					Format = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("destinationpath"), DestinationPath);
					oXmlWriter.WriteAttribute(_T("format"), Format);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					DestinationPath = oXmlNode.GetAttributeOrValue(_T("destinationpath"), _T(""));
					Format = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("format"), _T("8888")));
				}
				
				virtual void InternalClear()
				{
					DestinationPath = _T("");
					Format = ImageStudio::IO::c_nFormat_Uncompressed_32bpp;
				}
				
			public:
				
				FileSaveAsPsd()
				{
					Initialize(c_nSerializeFileSaveAsPsd, c_strSerializeFileSaveAsPsd);

					Clear();
				}
					
			public:
				
				CString DestinationPath;
				int Format;

				/* @desc:
				ImageStudio-Serialize-File-FileSaveAsPsd (c_nSerializeFileSaveAsPsd, c_strSerializeFileSaveAsPsd)
				{
					CString DestinationPath = "";				// xml имя - destinationpath; путь к файлу, в который будет сохранена картинка
					int Format = c_nFormat_Uncompressed_32bpp;	// xml имя - format; формат выходного файла, см. константы в ImageStudio::IO
				}
				*/
			};
			class FileSaveAsJp2 : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 3 * sizeof(int) + 2 * sizeof(BOOL) + sDestinationPath.GetLength() + 1;
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteString( sDestinationPath );
					pStream->WriteLong( nFormat );
					pStream->WriteLong( (int)bJp2 );
					pStream->WriteLong( (int)bLossLess );
					pStream->WriteLong( nQuality );
					pStream->WriteLong( nFileSize );
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					sDestinationPath = pStream->ReadString();
					nFormat          = pStream->ReadLong();
					bJp2             = pStream->ReadLong();
					bLossLess        = pStream->ReadLong();
					nQuality         = pStream->ReadLong();
					nFileSize        = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute( _T("destinationpath"), sDestinationPath );
					oXmlWriter.WriteAttribute( _T("format"),          nFormat );
					oXmlWriter.WriteAttribute( _T("jp2"),             (int)bJp2 );
					oXmlWriter.WriteAttribute( _T("lossless"),        (int)bLossLess );
					oXmlWriter.WriteAttribute( _T("quality"),         nQuality );
					oXmlWriter.WriteAttribute( _T("filesize"),        nFileSize );
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					sDestinationPath = oXmlNode.GetAttributeOrValue( _T("destinationpath"), _T("") );
					nFormat          =       Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("format"),   _T("888") ) );
					bJp2             = (BOOL)Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("jp2"),      _T("0")   ) );
					bLossLess        = (BOOL)Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("lossless"), _T("0")   ) );
					nQuality         =       Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("quality"),  _T("-1")  ) );
					nFileSize        =       Strings::ToInteger( oXmlNode.GetAttributeOrValue( _T("filesize"), _T("-1")  ) );
				}
				
				virtual void InternalClear()
				{
					sDestinationPath = _T("");
					nFormat          = ImageStudio::IO::c_nFormat_Uncompressed_24bpp;
					bJp2             = FALSE;
					bLossLess        = FALSE;
					nQuality         = 90;
					nFileSize        = 0;
				}
				
			public:
				
				FileSaveAsJp2()
				{
					Initialize(c_nSerializeFileSaveAsJp2, c_strSerializeFileSaveAsJp2);

					Clear();
				}
					
			public:
				
				CString sDestinationPath;
				int     nFormat;

				BOOL    bJp2;      // FALSE - пишем чистый Jpeg2000CodeStream, TRUE - пишем в формате Jp2 (враппер к Jpeg2000CodeStream)
				BOOL    bLossLess; // TRUE - компрессия без потерь
				int     nQuality;  // Параметр качества
				int     nFileSize; // Применяем компрессию, чтобы добиться нужного размера файла

				/* @desc:
				ImageStudio-Serialize-File-FileSaveAsJp2 (c_nSerializeFileSaveAsJp2, c_strSerializeFileSaveAsJp2)
				{
					CString sDestinationPath = "";						// xml имя - destinationpath; путь к файлу, в который будет сохранена картинка
					int nFormat = c_nFormat_Uncompressed_24bpp;			// xml имя - format; формат выходного файла, см. константы в ImageStudio::IO
					int nQuality = 90;									// xml имя - quality; качество сжатия для jpeg компрессии, измеряется в процентах 0..100
				}
				*/
			};
		}
		namespace Paint
		{
			namespace Structures
			{
				class Rect
				{
				public:
					Rect()
					{
						Clear();
					}

					void Clear()
					{
						m_dX1 = 0;
						m_dY1 = 0;
						m_dX2 = 0;
						m_dY2 = 0;
					}
					
					void ToXmlNode( XmlUtils::CXmlWriter& oXmlWriter, const CString& sName ) const
					{
						oXmlWriter.WriteAttribute( sName + _T("left"),   m_dX1 );
						oXmlWriter.WriteAttribute( sName + _T("top"),    m_dY1 );
						oXmlWriter.WriteAttribute( sName + _T("right"),  m_dX2 );
						oXmlWriter.WriteAttribute( sName + _T("bottom"), m_dY2 );
					}

					void FromXmlNode( XmlUtils::CXmlNode& oXmlNode, const CString& sName )
					{
						m_dX1 = XmlUtils::GetDouble( oXmlNode.GetAttribute( sName + _T("left") ));
						m_dY1 = XmlUtils::GetDouble( oXmlNode.GetAttribute( sName + _T("top") ));
						m_dX2 = XmlUtils::GetDouble( oXmlNode.GetAttribute( sName + _T("right") ));
						m_dY2 = XmlUtils::GetDouble( oXmlNode.GetAttribute( sName + _T("bottom") ));
					}
					
					void FromGdiplusRect( const Gdiplus::RectF& rect )
					{
						m_dX1 = rect.X;
						m_dY1 = rect.Y;
						m_dX2 = rect.X + rect.Width;
						m_dY2 = rect.Y + rect.Height;
					}
					void ToGdiplusRect( Gdiplus::RectF& rect ) const
					{
						if( m_dX1 < m_dX2 )
						{
							rect.X     = float( m_dX1 );
							rect.Width = float( m_dX2 - m_dX1 );
						}
						else
						{
							rect.X     = float( m_dX2 );
							rect.Width = float( m_dX1 - m_dX2 );
						}
						
						if( m_dY1 < m_dY2 )
						{
							rect.Y      = float( m_dY1 );
							rect.Height = float( m_dY2 - m_dY1 );
						}
						else
						{
							rect.Y      = float( m_dY2 );
							rect.Height = float( m_dY1 - m_dY2 );
						}
					}

				
				public:
					double m_dX1;
					double m_dY1;
					double m_dX2;
					double m_dY2;
				};
				
				class Pen
				{
				public:

					Pen()
					{
						Clear();
					}
					
					void Clear()
					{
						m_nColor = -1000001;
						m_nAlpha = -1000001;
						m_dSize = -1000001;
						m_nAntiAliasPen = -1000001;
						m_bUseMetric = FALSE;
						m_nAlign = 0;
					}
					void ToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("pen-color"), m_nColor);
						oXmlWriter.WriteAttribute(_T("pen-alpha"), m_nAlpha);
						oXmlWriter.WriteAttribute(_T("pen-size"), m_dSize);
						oXmlWriter.WriteAttribute(_T("pen-antialiaspen"), m_nAntiAliasPen);
						oXmlWriter.WriteAttribute(_T("pen-usemetric"), m_bUseMetric);
						oXmlWriter.WriteAttribute(_T("pen-alignment"), m_nAlign);
					}
					void FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						m_nColor = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-color"), _T("-1000001")));
						m_nAlpha = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-alpha"), _T("-1000001")));
						m_dSize = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("pen-size"), _T("-1000001")));
						m_nAntiAliasPen = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-antialiaspen"), _T("-1000001")));
						m_bUseMetric = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-usemetric"), _T("0")));
						m_nAlign = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-alignment"), _T("0")));
					}
					
					BOOL IsValidColor() const
					{
						return (m_nColor > -1000001);
					}
					BOOL IsValidAlpha() const
					{
						return (m_nAlpha > -1000001);
					}
					BOOL IsValidSize() const
					{
						return (m_dSize > -1000001);
					}
					BOOL IsValidAntiAliasPen() const
					{
						return (m_nAntiAliasPen > -1000001);
					}
					
					int GetColor() const
					{
						return m_nColor;
					}
					int GetAlpha() const
					{
						return m_nAlpha;
					}
					double GetSize() const
					{
						return m_dSize;
					}
					BOOL GetAntiAliasPen() const
					{
						return m_nAntiAliasPen;
					}
					BOOL IsUseMetric() const
					{
						return m_bUseMetric;
					}
					int GetAlignment() const
					{
						return m_nAlign;
					}

				private:
					
					int m_nColor;
					int m_nAlpha;
					double m_dSize;
					int m_nAntiAliasPen;
					int m_bUseMetric;
					int m_nAlign;
				};
				class Brush
				{
				public:

					Brush()
					{
						Clear();
					}
					
					void Clear()
					{
						m_nType = -1000001;
					
						m_nColor1 = -1000001;
						m_nColor2 = -1000001;
						m_nAlpha1 = -1000001;
						m_nAlpha2 = -1000001;

						m_nGradientAngle = -1000001;
					
						m_strTexturePath = _T("");
						m_nTextureAlpha = -1000001;
						m_nTextureMode = -1000001;
					
						m_nRectable = -1000001;

						m_rect.X = -1000001;
						m_rect.Y = -1000001;
						m_rect.Width = 0;
						m_rect.Height = 0;

						m_nAntiAliasBrush = -1000001;
					}
					void ToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("brush-type"), m_nType);
						oXmlWriter.WriteAttribute(_T("brush-color1"), m_nColor1);
						oXmlWriter.WriteAttribute(_T("brush-color2"), m_nColor2);
						oXmlWriter.WriteAttribute(_T("brush-alpha1"), m_nAlpha1);
						oXmlWriter.WriteAttribute(_T("brush-alpha2"), m_nAlpha2);
						oXmlWriter.WriteAttribute(_T("brush-gradient-angle"), m_nGradientAngle);
						oXmlWriter.WriteAttribute(_T("brush-texturepath"), m_strTexturePath);
						oXmlWriter.WriteAttribute(_T("brush-texturealpha"), m_nTextureAlpha);
						oXmlWriter.WriteAttribute(_T("brush-texturemode"), m_nTextureMode);
						oXmlWriter.WriteAttribute(_T("brush-rectable"), m_nRectable);

						oXmlWriter.WriteAttribute(_T("brush-rect-left"), m_rect.X);
						oXmlWriter.WriteAttribute(_T("brush-rect-top"), m_rect.Y);
						oXmlWriter.WriteAttribute(_T("brush-rect-width"), m_rect.Width);
						oXmlWriter.WriteAttribute(_T("brush-rect-height"), m_rect.Height);

						oXmlWriter.WriteAttribute(_T("brush-antialiasbrush"), m_nAntiAliasBrush);
					}
					void FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						m_nType = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-type"), _T("-1000001")));

						m_nColor1 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-color1"), _T("-1000001")));
						m_nColor2 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-color2"), _T("-1000001")));
						m_nAlpha1 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-alpha1"), _T("-1000001")));
						m_nAlpha2 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-alpha2"), _T("-1000001")));
						m_nGradientAngle = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-gradient-angle"), _T("-1000001")));

						m_strTexturePath = oXmlNode.GetAttributeOrValue(_T("brush-texturepath"), _T(""));
						m_nTextureAlpha = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-texturealpha"), _T("-1000001")));
						m_nTextureMode = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-texturemode"), _T("-1000001")));

						m_nRectable = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-rectable"), _T("-1000001")));

						m_rect.X = (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("brush-rect-left"), _T("-1000001")));
						m_rect.Y = (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("brush-rect-top"), _T("-1000001")));
						m_rect.Width = (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("brush-rect-width"), _T("0")));
						m_rect.Height = (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("brush-rect-height"), _T("0")));

						m_nAntiAliasBrush = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-antialiasbrush"), _T("-1000001")));
					}
					void FromXmlNode2(XmlUtils::CXmlNode& oXmlNode)
					{
						Brush oBrush;
						oBrush.FromXmlNode(oXmlNode);

						if (oBrush.IsValidType()) m_nType = oBrush.GetType();

						if (oBrush.IsValidColor1()) m_nColor1 = oBrush.GetColor1();
						if (oBrush.IsValidColor2()) m_nColor2 = oBrush.GetColor2();
						if (oBrush.IsValidAlpha1()) m_nAlpha1 = oBrush.GetAlpha1();
						if (oBrush.IsValidAlpha2()) m_nAlpha2 = oBrush.GetAlpha2();
						if (oBrush.IsValidGradientAngle()) m_nGradientAngle = oBrush.GetGradientAngle();

						if (oBrush.IsValidTexturePath()) m_strTexturePath = oBrush.GetTexturePath();
						if (oBrush.IsValidTextureAlpha()) m_nTextureAlpha = oBrush.GetTextureAlpha();
						if (oBrush.IsValidTextureMode()) m_nTextureMode = oBrush.GetTextureMode();

						if (oBrush.IsValidRectable()) m_nRectable = oBrush.GetRectable();
						
						if (oBrush.IsValidRect()) m_rect = oBrush.GetRect();

						if (oBrush.IsValidAntiAliasBrush()) m_nAntiAliasBrush = oBrush.GetAntiAliasBrush();
					}
					
					BOOL IsValidType() const
					{
						return (m_nType > -1000001);
					}
					BOOL IsValidColor1() const
					{
						return (m_nColor1 > -1000001);
					}
					BOOL IsValidColor2() const
					{
						return (m_nColor2 > -1000001);
					}
					BOOL IsValidAlpha1() const
					{
						return (m_nAlpha1 > -1000001);
					}
					BOOL IsValidAlpha2() const
					{
						return (m_nAlpha2 > -1000001);
					}
					BOOL IsValidGradientAngle() const
					{
						return (m_nGradientAngle > -1000001);
					}
					BOOL IsValidTexturePath() const
					{
						return (m_strTexturePath.GetLength() > 0);
					}
					BOOL IsValidTextureMode() const
					{
						return (m_nTextureMode > -1000001);
					}
					BOOL IsValidTextureAlpha() const
					{
						return (m_nTextureAlpha > -1000001);
					}
					BOOL IsValidRectable() const
					{
						return (m_nRectable > -1000001);
					}
					BOOL IsValidRect() const
					{
						return (m_rect.X > -1000001 && m_rect.Y > -1000001);
					}
					BOOL IsValidAntiAliasBrush() const
					{
						return (m_nAntiAliasBrush > -1000001);
					}
					
					int GetType() const
					{
						if (IsValidType())
							return Painter::ConstantCompatible(m_nType);

						return m_nType;
					}
					int GetColor1() const
					{
						return m_nColor1;
					}
					int GetColor2() const
					{
						return m_nColor2;
					}
					int GetAlpha1() const
					{
						return m_nAlpha1;
					}
					int GetAlpha2() const
					{
						return m_nAlpha2;
					}
					int GetGradientAngle() const
					{
						return m_nGradientAngle;
					}
					CString GetTexturePath() const
					{
						return m_strTexturePath;
					}
					int GetTextureMode() const
					{
						return m_nTextureMode;
					}
					int GetTextureAlpha() const
					{
						return m_nTextureAlpha;
					}
					BOOL GetRectable() const
					{
						return (BOOL)m_nRectable;
					}
					RectF GetRect() const
					{
						return m_rect;
					}
					BOOL GetAntiAliasBrush() const
					{
						return m_nAntiAliasBrush;
					}
					
				private:
					
					int m_nType;
					
					int m_nColor1;
					int m_nColor2;
					int m_nAlpha1;
					int m_nAlpha2;

					int m_nGradientAngle;
					
					CString m_strTexturePath;
					int m_nTextureAlpha;
					int m_nTextureMode;
					
					int m_nRectable;
					RectF m_rect;

					int m_nAntiAliasBrush;
				};
				class Font
				{
				public:

					Font()
					{
						Clear();
					}
					
					void Clear()
					{
						m_strName = _T("");
						m_dSize = -1000001;
						m_nBold = -1000001;
						m_nItalic = -1000001;
						m_nUnderline = -1000001;
						m_nStrikeout = -1000001;
						m_nAntiAliasText = -1000001;
						m_nStringAlignmentVertical = -1000001;
						m_nStringAlignmentHorizontal = -1000001;
						m_nAngle = -1000001;
					}
					void ToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("font-name"), m_strName);
						oXmlWriter.WriteAttribute(_T("font-size"), m_dSize);
						oXmlWriter.WriteAttribute(_T("font-bold"), m_nBold);
						oXmlWriter.WriteAttribute(_T("font-italic"), m_nItalic);
						oXmlWriter.WriteAttribute(_T("font-underline"), m_nUnderline);
						oXmlWriter.WriteAttribute(_T("font-strikeout"), m_nStrikeout);
						oXmlWriter.WriteAttribute(_T("font-antialiastext"), m_nAntiAliasText);
						oXmlWriter.WriteAttribute(_T("font-stringalignmentvertical"), m_nStringAlignmentVertical);
						oXmlWriter.WriteAttribute(_T("font-stringalignmenthorizontal"), m_nStringAlignmentHorizontal);
						oXmlWriter.WriteAttribute(_T("font-angle"), m_nAngle);
					}
					void FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						m_strName = oXmlNode.GetAttributeOrValue(_T("font-name"), _T(""));
						m_dSize = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("font-size"), _T("-1000001")));
						m_nBold = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-bold"), _T("-1000001")));
						m_nItalic = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-italic"), _T("-1000001")));
						m_nUnderline = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-underline"), _T("-1000001")));
						m_nStrikeout = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-strikeout"), _T("-1000001")));
						m_nAntiAliasText = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-antialiastext"), _T("-1000001")));
						m_nStringAlignmentVertical = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-stringalignmentvertical"), _T("-1000001")));
						m_nStringAlignmentHorizontal = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-stringalignmenthorizontal"), _T("-1000001")));
						m_nAngle = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("font-angle"), _T("-1000001")));
					}
					void FromXmlNode2(XmlUtils::CXmlNode& oXmlNode)
					{
						Font oFont;
						oFont.FromXmlNode(oXmlNode);

						if (oFont.IsValidName()) m_strName = oFont.GetName();
						if (oFont.IsValidSize()) m_dSize = oFont.GetSize();
						if (oFont.IsValidBold()) m_nBold = oFont.GetBold();
						if (oFont.IsValidItalic()) m_nItalic = oFont.GetItalic();
						if (oFont.IsValidUnderline()) m_nUnderline = oFont.GetUnderline();
						if (oFont.IsValidStrikeout()) m_nStrikeout = oFont.GetStrikeout();
						if (oFont.IsValidAntiAliasText()) m_nAntiAliasText = oFont.GetAntiAliasText();
						if (oFont.IsValidStringAlignmentVertical()) m_nStringAlignmentVertical = oFont.GetStringAlignmentVertical();
						if (oFont.IsValidStringAlignmentHorizontal()) m_nStringAlignmentHorizontal = oFont.GetStringAlignmentHorizontal();
						if (oFont.IsValidAngle()) m_nAngle = oFont.GetAngle();
					}
					
					BOOL IsValidName() const
					{
						return (m_strName.GetLength() > 0);
					}
					BOOL IsValidSize() const
					{
						return (m_dSize > -1000001);
					}
					BOOL IsValidBold() const
					{
						return (m_nBold > -1000001);
					}
					BOOL IsValidItalic() const
					{
						return (m_nItalic > -1000001);
					}
					BOOL IsValidUnderline() const
					{
						return (m_nUnderline > -1000001);
					}
					BOOL IsValidStrikeout() const
					{
						return (m_nStrikeout > -1000001);
					}
					BOOL IsValidAntiAliasText() const
					{
						return (m_nAntiAliasText > -1000001);
					}
					BOOL IsValidStringAlignmentVertical() const
					{
						return (m_nStringAlignmentVertical > -1000001);
					}
					BOOL IsValidStringAlignmentHorizontal() const
					{
						return (m_nStringAlignmentHorizontal > -1000001);
					}
					BOOL IsValidAngle() const
					{
						return (m_nAngle > -1000001);
					}
					
					CString GetName() const
					{
						return m_strName;
					}
					double GetSize() const
					{
						return m_dSize;
					}
					BOOL GetBold() const
					{
						return (BOOL)m_nBold;
					}
					BOOL GetItalic() const
					{
						return (BOOL)m_nItalic;
					}
					BOOL GetUnderline() const
					{
						return (BOOL)m_nUnderline;
					}
					BOOL GetStrikeout() const
					{
						return (BOOL)m_nStrikeout;
					}
					BOOL GetAntiAliasText() const
					{
						return (BOOL)m_nAntiAliasText;
					}
					int GetStringAlignmentVertical() const
					{
						return m_nStringAlignmentVertical;
					}
					int GetStringAlignmentHorizontal() const
					{
						return m_nStringAlignmentHorizontal;
					}
					double GetAngle() const
					{
						return m_nAngle;
					}
					
				private:
					
					CString m_strName;
					double m_dSize;
					int m_nBold;
					int m_nItalic;
					int m_nUnderline;
					int m_nStrikeout;
					int m_nAntiAliasText;
					int m_nStringAlignmentVertical;
					int m_nStringAlignmentHorizontal;
					double m_nAngle; // rotation angle for the text (in degrees: -360..360)
				};
				class Shadow
				{
				public:

					Shadow()
					{
						Clear();
					}
					
					void Clear()
					{
						m_nVisible = -1000001;
						m_nDistanceX = -1000001;
						m_nDistanceY = -1000001;
						m_nBlurSize = -1000001;
						m_nColor = -1000001;
						m_nAlpha = -1000001;
					}
					void ToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("shadow-visible"), m_nVisible);
						oXmlWriter.WriteAttribute(_T("shadow-distancex"), m_nDistanceX);
						oXmlWriter.WriteAttribute(_T("shadow-distancey"), m_nDistanceY);
						oXmlWriter.WriteAttribute(_T("shadow-blursize"), m_nBlurSize);
						oXmlWriter.WriteAttribute(_T("shadow-color"), m_nColor);
						oXmlWriter.WriteAttribute(_T("shadow-alpha"), m_nAlpha);
					}
					void FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						m_nVisible = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("shadow-visible"), _T("-1000001")));
						m_nDistanceX = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("shadow-distancex"), _T("-1000001")));
						m_nDistanceY = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("shadow-distancey"), _T("-1000001")));
						m_nBlurSize = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("shadow-blursize"), _T("-1000001")));
						m_nColor = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("shadow-color"), _T("-1000001")));
						m_nAlpha = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("shadow-alpha"), _T("-1000001")));
					}
					void FromXmlNode2(XmlUtils::CXmlNode& oXmlNode)
					{
						Shadow oShadow;
						oShadow.FromXmlNode(oXmlNode);

						if (oShadow.IsValidVisible()) m_nVisible = oShadow.GetVisible();
						if (oShadow.IsValidDistanceX()) m_nDistanceX = oShadow.GetDistanceX();
						if (oShadow.IsValidDistanceY()) m_nDistanceY = oShadow.GetDistanceY();
						if (oShadow.IsValidBlurSize()) m_nBlurSize = oShadow.GetBlurSize();
						if (oShadow.IsValidColor()) m_nColor = (int)oShadow.GetBlurSize();
						if (oShadow.IsValidAlpha()) m_nAlpha = oShadow.GetAlpha();
					}
					
					BOOL IsValidVisible() const
					{
						return (m_nVisible > -1000001);
					}
					BOOL IsValidDistanceX() const
					{
						return (m_nDistanceX > -1000001);
					}
					BOOL IsValidDistanceY() const
					{
						return (m_nDistanceY > -1000001);
					}
					BOOL IsValidBlurSize() const
					{
						return (m_nBlurSize > -1000001);
					}
					BOOL IsValidColor() const
					{
						return (m_nColor > -1000001);
					}
					BOOL IsValidAlpha() const
					{
						return (m_nAlpha > -1000001);
					}
					
					BOOL GetVisible() const
					{
						return (BOOL)m_nVisible;
					}
					double GetDistanceX() const
					{
						return m_nDistanceX;
					}
					double GetDistanceY() const
					{
						return m_nDistanceY;
					}
					double GetBlurSize() const
					{
						return m_nBlurSize;
					}
					int GetColor() const
					{
						return m_nColor;
					}
					int GetAlpha() const
					{
						return m_nAlpha;
					}
					
				private:
					
					int m_nVisible;
					double m_nDistanceX;
					double m_nDistanceY;
					double m_nBlurSize;
					int m_nColor;
					int m_nAlpha;
				};
				class Edge
				{
				public:

					Edge()
					{
						Clear();
					}
					
					void Clear()
					{
						m_nVisible = -1000001;;
						m_nDist = -1000001;;
						m_nColor = -1000001;;
						m_nAlpha = -1000001;;
					}
					void ToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("edge-visible"), m_nVisible);
						oXmlWriter.WriteAttribute(_T("edge-dist"), m_nDist);
						oXmlWriter.WriteAttribute(_T("edge-color"), m_nColor);
						oXmlWriter.WriteAttribute(_T("edge-alpha"), m_nAlpha);
					}
					void FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						m_nVisible = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("edge-visible"), _T("-1000001")));
						m_nDist = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("edge-dist"), _T("-1000001")));
						m_nColor = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("edge-color"), _T("-1000001")));
						m_nAlpha = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("edge-alpha"), _T("-1000001")));
					}
					void FromXmlNode2(XmlUtils::CXmlNode& oXmlNode)
					{
						Edge oEdge;
						oEdge.FromXmlNode(oXmlNode);

						if (oEdge.IsValidVisible()) m_nVisible = oEdge.GetVisible();
						if (oEdge.IsValidDist()) m_nDist = oEdge.GetDist();
						if (oEdge.IsValidColor()) m_nColor = oEdge.GetColor();
						if (oEdge.IsValidAlpha()) m_nAlpha = oEdge.GetAlpha();
					}
					
					BOOL IsValidVisible() const
					{
						return (m_nVisible > -1000001);
					}
					BOOL IsValidDist() const
					{
						return (m_nDist > -1000001);
					}
					BOOL IsValidColor() const
					{
						return (m_nColor > -1000001);
					}
					BOOL IsValidAlpha() const
					{
						return (m_nAlpha > -1000001);
					}
					
					BOOL GetVisible() const
					{
						return (BOOL)m_nVisible;
					}
					double GetDist() const
					{
						return m_nDist;
					}
					int GetColor() const
					{
						return m_nColor;
					}
					int GetAlpha() const
					{
						return m_nAlpha;
					}
					
				private:
					
					int m_nVisible;
					double m_nDist;
					int m_nColor;
					int m_nAlpha;
				};
			}
			namespace Common
			{
				class SetPen : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return Pen.GetSize();
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						Pen.ToStream(pStream);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Pen.FromStream(pStream);
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						Pen.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Pen.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Pen.Clear();
					}
					
				public:
					
					SetPen()
					{
						Initialize(c_nSerializePaintSetPen, c_strSerializePaintSetPen);

						Clear();
					}
						
				public:
					
					Painter::CPen Pen;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-SetPen (c_nSerializePaintSetPen, c_strSerializePaintSetPen)
					{
						Painter::CPen Pen;
					}
					*/
				};
				class SetBrush : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return Brush.GetSize();
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						Brush.ToStream(pStream);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Brush.FromStream(pStream);
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						Brush.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Brush.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Brush.Clear();
					}
					
				public:
					
					SetBrush()
					{
						Initialize(c_nSerializePaintSetBrush, c_strSerializePaintSetBrush);

						Clear();
					}
						
				public:
					
					Painter::CBrush Brush;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-SetBrush (c_nSerializePaintSetBrush, c_strSerializePaintSetBrush)
					{
						Painter::CBrush Brush;
					}
					*/
				};
				class SetFont : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return Font.GetSize();
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						Font.ToStream(pStream);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Font.FromStream(pStream);
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						Font.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Font.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Font.Clear();
					}
					
				public:
					
					SetFont()
					{
						Initialize(c_nSerializePaintSetFont, c_strSerializePaintSetFont);

						Clear();
					}
						
				public:
					
					Painter::CFont Font;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-SetFont (c_nSerializePaintSetFont, c_strSerializePaintSetFont)
					{
						Painter::CFont Font;
					}
					*/
				};
				class SetFormat : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return Format.GetSize();
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						Format.ToStream(pStream);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Format.FromStream(pStream);
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						Format.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Format.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Format.Clear();
					}
					
				public:
					
					SetFormat()
					{
						Initialize(c_nSerializePaintSetFormat, c_strSerializePaintSetFormat);

						Clear();
					}
						
				public:
					
					Painter::CFormat Format;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-SetFormat (c_nSerializePaintSetFormat, c_strSerializePaintSetFormat)
					{
						Painter::CFormat Format;
					}
					*/
				};
			
				class DrawLine : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 4*sizeof(double) + sizeof(long);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(X1);
						pStream->WriteDouble(Y1);
						pStream->WriteDouble(X2);
						pStream->WriteDouble(Y2);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						X1 = pStream->ReadDouble();
						Y1 = pStream->ReadDouble();
						X2 = pStream->ReadDouble();
						Y2 = pStream->ReadDouble();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("x1"), X1);
						oXmlWriter.WriteAttribute(_T("y1"), X1);
						oXmlWriter.WriteAttribute(_T("x2"), X1);
						oXmlWriter.WriteAttribute(_T("y2"), X1);

						Pen.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						X1 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("x1"), _T("0")));
						Y1 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("y1"), _T("0")));
						X2 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("x2"), _T("0")));
						Y2 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("y2"), _T("0")));

						Pen.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						X1 = 0;
						Y1 = 0;
						X2 = 0;
						Y2 = 0;

						Pen.Clear();
					}
					
				public:
					
					DrawLine()
					{
						Initialize(c_nSerializePaintDrawLine, c_strSerializePaintDrawLine);

						Clear();
					}
						
				public:
					
					int Metric;
					double X1, Y1, X2, Y2;

					ImageStudio::Serialize::Paint::Structures::Pen Pen;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawLine (c_nSerializePaintDrawLine, c_strSerializePaintDrawLine)
					{
						int Metric = c_nMetricPixels;		// xml имя - metriс; тип метрики, см. c_nMetric...

						// координаты отрезка, соединяющего точки A(X1, Y1) and B(X2, Y2). координаты зависят от метрики
						double X1 = 0;						// xml имя - x1; 
						double Y1 = 0;						// xml имя - y1; 
						double X2 = 0;						// xml имя - x2; 
						double Y2 = 0;						// xml имя - y2; 

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Pen Pen;			// xml имя - pen-...
					}
					*/
				};
				class DrawRectangle : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 4*sizeof(double) + 2*sizeof(int);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(Left);
						pStream->WriteDouble(Top);
						pStream->WriteDouble(Right);
						pStream->WriteDouble(Bottom);
						pStream->WriteLong((int)Solid);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Left = pStream->ReadDouble();
						Top = pStream->ReadDouble();
						Right = pStream->ReadDouble();
						Bottom = pStream->ReadDouble();
						Solid = (BOOL)pStream->ReadLong();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("left"), Left);
						oXmlWriter.WriteAttribute(_T("top"), Top);
						oXmlWriter.WriteAttribute(_T("right"), Right);
						oXmlWriter.WriteAttribute(_T("bottom"), Bottom);
						oXmlWriter.WriteAttribute(_T("solid"), (int)Solid);

						Pen.ToXmlAttributes(oXmlWriter);
						Brush.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						Left = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("left"), _T("0")));
						Top = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("top"), _T("0")));
						Right = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("right"), _T("0")));
						Bottom = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("bottom"), _T("0")));
						Solid = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("solid"), _T("0")));

						Pen.FromXmlNode(oXmlNode);
						Brush.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Left = 0;
						Top = 0;
						Right = 0;
						Bottom = 0;
						Solid = TRUE;

						Pen.Clear();
						Brush.Clear();
					}
					
				public:
					
					DrawRectangle()
					{
						Initialize(c_nSerializePaintDrawRectangle, c_strSerializePaintDrawRectangle);

						Clear();
					}
						
				public:
					
					int Metric;
					BOOL Solid;
					double Left, Top, Right, Bottom;

					ImageStudio::Serialize::Paint::Structures::Pen Pen;
					ImageStudio::Serialize::Paint::Structures::Brush Brush;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawRectangle (c_nSerializePaintDrawRectangle, c_strSerializePaintDrawRectangle)
					{
						int Metric = c_nMetricPixels;		// xml имя - metriс; тип метрики, см. c_nMetric...
						BOOL Solid = TRUE;					// xml имя - solid; заливать объект brush'ем (1) или рисовать только его границу pen'ом (0)

						// координаты прямоугольника (в пикселях картинки или в другой какой-то метрике)
						double Left = 0;					// xml имя - left; 
						double Top = 0;						// xml имя - top; 
						double Right = 0;					// xml имя - right; 
						double Bottom = 0;					// xml имя - bottom; 

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Pen Pen;			// xml имя - pen-...
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
					}
					*/
				};
				class DrawEllipse : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 4*sizeof(double) + 2*sizeof(int);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(Left);
						pStream->WriteDouble(Top);
						pStream->WriteDouble(Right);
						pStream->WriteDouble(Bottom);
						pStream->WriteLong((int)Solid);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Left = pStream->ReadDouble();
						Top = pStream->ReadDouble();
						Right = pStream->ReadDouble();
						Bottom = pStream->ReadDouble();
						Solid = (BOOL)pStream->ReadLong();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("left"), Left);
						oXmlWriter.WriteAttribute(_T("top"), Top);
						oXmlWriter.WriteAttribute(_T("right"), Right);
						oXmlWriter.WriteAttribute(_T("bottom"), Bottom);
						oXmlWriter.WriteAttribute(_T("solid"), (int)Solid);

						Pen.ToXmlAttributes(oXmlWriter);
						Brush.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						Left = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("left"), _T("0")));
						Top = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("top"), _T("0")));
						Right = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("right"), _T("0")));
						Bottom = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("bottom"), _T("0")));
						Solid = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("solid"), _T("0")));

						Pen.FromXmlNode(oXmlNode);
						Brush.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Left = 0;
						Top = 0;
						Right = 0;
						Bottom = 0;
						Solid = TRUE;

						Pen.Clear();
						Brush.Clear();
					}
					
				public:
					
					DrawEllipse()
					{
						Initialize(c_nSerializePaintDrawEllipse, c_strSerializePaintDrawEllipse);

						Clear();
					}
						
				public:
					
					int Metric;
					BOOL Solid;
					double Left, Top, Right, Bottom;

					ImageStudio::Serialize::Paint::Structures::Pen Pen;
					ImageStudio::Serialize::Paint::Structures::Brush Brush;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawEllipse (c_nSerializePaintDrawEllipse, c_strSerializePaintDrawEllipse)
					{
						int Metric = c_nMetricPixels;		// xml имя - metriс; тип метрики, см. c_nMetric...
						BOOL Solid = TRUE;					// xml имя - solid; заливать эллипс brush'ем (1) или рисовать только его границу pen'ом (0)

						// координаты прямоугольника, описанного около эллипса (в пикселях картинки или в другой какой-то метрике)
						double Left = 0;					// xml имя - left; 
						double Top = 0;						// xml имя - top; 
						double Right = 0;					// xml имя - right; 
						double Bottom = 0;					// xml имя - bottom; 

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Pen Pen;			// xml имя - pen-...
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
					}
					*/
				};
				class DrawInvRectangle : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 4*sizeof(double) + sizeof(int);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(Left);
						pStream->WriteDouble(Top);
						pStream->WriteDouble(Right);
						pStream->WriteDouble(Bottom);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Left = pStream->ReadDouble();
						Top = pStream->ReadDouble();
						Right = pStream->ReadDouble();
						Bottom = pStream->ReadDouble();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("left"), Left);
						oXmlWriter.WriteAttribute(_T("top"), Top);
						oXmlWriter.WriteAttribute(_T("right"), Right);
						oXmlWriter.WriteAttribute(_T("bottom"), Bottom);

						Brush.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						Left = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("left"), _T("0")));
						Top = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("top"), _T("0")));
						Right = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("right"), _T("0")));
						Bottom = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("bottom"), _T("0")));

						Brush.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Left = 0;
						Top = 0;
						Right = 0;
						Bottom = 0;

						Brush.Clear();
					}
					
				public:
					
					DrawInvRectangle()
					{
						Initialize(c_nSerializePaintDrawInvRectangle, c_strSerializePaintDrawInvRectangle);

						Clear();
					}
						
				public:
					
					int Metric;
					double Left, Top, Right, Bottom;

					ImageStudio::Serialize::Paint::Structures::Brush Brush;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawInvRectangle (c_nSerializePaintDrawInvRectangle, c_strSerializePaintDrawInvRectangle)
					{
						int Metric = c_nMetricPixels;		// xml имя - metriс; тип метрики, см. c_nMetric...

						// координаты прямоугольника (в пикселях картинки или в другой какой-то метрике)
						double Left = 0;					// xml имя - left; 
						double Top = 0;						// xml имя - top; 
						double Right = 0;					// xml имя - right; 
						double Bottom = 0;					// xml имя - bottom; 

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
					}
					*/
				};
				class DrawInvEllipse : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 4*sizeof(double) + sizeof(int);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(Left);
						pStream->WriteDouble(Top);
						pStream->WriteDouble(Right);
						pStream->WriteDouble(Bottom);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Left = pStream->ReadDouble();
						Top = pStream->ReadDouble();
						Right = pStream->ReadDouble();
						Bottom = pStream->ReadDouble();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("left"), Left);
						oXmlWriter.WriteAttribute(_T("top"), Top);
						oXmlWriter.WriteAttribute(_T("right"), Right);
						oXmlWriter.WriteAttribute(_T("bottom"), Bottom);

						Brush.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						Left = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("left"), _T("0")));
						Top = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("top"), _T("0")));
						Right = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("right"), _T("0")));
						Bottom = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("bottom"), _T("0")));

						Brush.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Left = 0;
						Top = 0;
						Right = 0;
						Bottom = 0;

						Brush.Clear();
					}
					
				public:
					
					DrawInvEllipse()
					{
						Initialize(c_nSerializePaintDrawInvEllipse, c_strSerializePaintDrawInvEllipse);

						Clear();
					}
						
				public:
					
					int Metric;
					double Left, Top, Right, Bottom;

					ImageStudio::Serialize::Paint::Structures::Brush Brush;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawInvEllipse (c_nSerializePaintDrawInvEllipse, c_strSerializePaintDrawInvEllipse)
					{
						int Metric = c_nMetricPixels;		// xml имя - metriс; тип метрики, см. c_nMetric...

						// координаты прямоугольника, описанного около эллипса (в пикселях картинки или в другой какой-то метрике)
						double Left = 0;					// xml имя - left; 
						double Top = 0;						// xml имя - top; 
						double Right = 0;					// xml имя - right; 
						double Bottom = 0;					// xml имя - bottom; 

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
					}
					*/
				};
				class DrawPie : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 6*sizeof(double) + 2*sizeof(int);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(Left);
						pStream->WriteDouble(Top);
						pStream->WriteDouble(Right);
						pStream->WriteDouble(Bottom);
						pStream->WriteDouble(StartAngle);
						pStream->WriteDouble(SweepAngle);
						pStream->WriteLong((int)Solid);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Left = pStream->ReadDouble();
						Top = pStream->ReadDouble();
						Right = pStream->ReadDouble();
						Bottom = pStream->ReadDouble();
						StartAngle = pStream->ReadDouble();
						SweepAngle = pStream->ReadDouble();
						Solid = (BOOL)pStream->ReadLong();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("left"), Left);
						oXmlWriter.WriteAttribute(_T("top"), Top);
						oXmlWriter.WriteAttribute(_T("right"), Right);
						oXmlWriter.WriteAttribute(_T("bottom"), Bottom);
						oXmlWriter.WriteAttribute(_T("startangle"), StartAngle);
						oXmlWriter.WriteAttribute(_T("sweepangle"), SweepAngle);
						oXmlWriter.WriteAttribute(_T("solid"), (int)Solid);

						Pen.ToXmlAttributes(oXmlWriter);
						Brush.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						Left = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("left"), _T("0")));
						Top = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("top"), _T("0")));
						Right = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("right"), _T("0")));
						Bottom = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("bottom"), _T("0")));
						StartAngle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("startangle"), _T("0")));
						SweepAngle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("sweepangle"), _T("0")));
						Solid = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("solid"), _T("0")));

						Pen.FromXmlNode(oXmlNode);
						Brush.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Left = 0;
						Top = 0;
						Right = 0;
						Bottom = 0;
						StartAngle = 0;
						SweepAngle = 0;
						Solid = TRUE;

						Pen.Clear();
						Brush.Clear();
					}
					
				public:
					
					DrawPie()
					{
						Initialize(c_nSerializePaintDrawPie, c_strSerializePaintDrawPie);

						Clear();
					}
						
				public:
					
					int Metric;
					BOOL Solid;
					double Left, Top, Right, Bottom;
					double StartAngle, SweepAngle;

					ImageStudio::Serialize::Paint::Structures::Pen Pen;
					ImageStudio::Serialize::Paint::Structures::Brush Brush;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawPie (c_nSerializePaintDrawPie, c_strSerializePaintDrawPie)
					{
						int Metric = c_nMetricPixels;		// xml имя - metriс; тип метрики, см. c_nMetric...
						BOOL Solid = TRUE;					// xml имя - solid; заливать объект brush'ем (1) или рисовать только его границу pen'ом

						// координаты прямоугольника описанного около эллипса, из которого вырезан pie (в пикселях картинки или в другой какой-то метрике)
						double Left = 0;					// xml имя - left; 
						double Top = 0;						// xml имя - top; 
						double Right = 0;					// xml имя - right; 
						double Bottom = 0;					// xml имя - bottom; 

						// углы, определяющие, какой pie вырезать из эллипса (в градусах, -360..360)
						double StartAngle = 0;				// xml имя - startangle; 
						double SweepAngle = 0;				// xml имя - sweepangle; 

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Pen Pen;			// xml имя - pen-...
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
					}
					*/
				};
				class DrawSector : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 7*sizeof(double) + 2*sizeof(int);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(Left);
						pStream->WriteDouble(Top);
						pStream->WriteDouble(Right);
						pStream->WriteDouble(Bottom);
						pStream->WriteDouble(StartAngle);
						pStream->WriteDouble(SweepAngle);
						pStream->WriteDouble(RadiusRatio);
						pStream->WriteLong((int)Solid);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Left = pStream->ReadDouble();
						Top = pStream->ReadDouble();
						Right = pStream->ReadDouble();
						Bottom = pStream->ReadDouble();
						StartAngle = pStream->ReadDouble();
						SweepAngle = pStream->ReadDouble();
						RadiusRatio = pStream->ReadDouble();
						Solid = (BOOL)pStream->ReadLong();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("left"), Left);
						oXmlWriter.WriteAttribute(_T("top"), Top);
						oXmlWriter.WriteAttribute(_T("right"), Right);
						oXmlWriter.WriteAttribute(_T("bottom"), Bottom);
						oXmlWriter.WriteAttribute(_T("startangle"), StartAngle);
						oXmlWriter.WriteAttribute(_T("sweepangle"), SweepAngle);
						oXmlWriter.WriteAttribute(_T("radiusratio"), RadiusRatio);
						oXmlWriter.WriteAttribute(_T("solid"), (int)Solid);

						Pen.ToXmlAttributes(oXmlWriter);
						Brush.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						Left = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("left"), _T("0")));
						Top = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("top"), _T("0")));
						Right = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("right"), _T("0")));
						Bottom = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("bottom"), _T("0")));
						StartAngle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("startangle"), _T("0")));
						SweepAngle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("sweepangle"), _T("0")));
						RadiusRatio = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("radiusratio"), _T("0")));
						Solid = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("solid"), _T("0")));

						Pen.FromXmlNode(oXmlNode);
						Brush.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Left = 0;
						Top = 0;
						Right = 0;
						Bottom = 0;
						StartAngle = 0;
						SweepAngle = 0;
						RadiusRatio = 0;
						Solid = TRUE;

						Pen.Clear();
						Brush.Clear();
					}
					
				public:
					
					DrawSector()
					{
						Initialize(c_nSerializePaintDrawSector, c_strSerializePaintDrawSector);

						Clear();
					}
						
				public:
					
					int Metric;
					BOOL Solid;
					double Left, Top, Right, Bottom;
					double StartAngle, SweepAngle;
					double RadiusRatio;

					ImageStudio::Serialize::Paint::Structures::Pen Pen;
					ImageStudio::Serialize::Paint::Structures::Brush Brush;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawSector (c_nSerializePaintDrawSector, c_strSerializePaintDrawSector)
					{
						int Metric = c_nMetricPixels;		// xml имя - metriс; тип метрики, см. c_nMetric...
						BOOL Solid = TRUE;					// xml имя - solid; заливать объект brush'ем (1) или рисовать только его границу pen'ом

						// коэффициент 0..1..1000, для подсчета второго радиуса для отсечения сектора (этот коэффициент умножается на радиус, заданный основным прямоугольником)
						double RadiusRatio = 0;				// xml имя - radiusratio ; 

						// координаты прямоугольника описанного около эллипса, из которого вырезан sector (в пикселях картинки или в другой какой-то метрике)
						double Left = 0;					// xml имя - left; 
						double Top = 0;						// xml имя - top; 
						double Right = 0;					// xml имя - right; 
						double Bottom = 0;					// xml имя - bottom; 

						// углы, определяющие, какой сектор вырезать из эллипса (в градусах, -360..360)
						double StartAngle = 0;				// xml имя - startangle; 
						double SweepAngle = 0;				// xml имя - sweepangle; 

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Pen Pen;			// xml имя - pen-...
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
					}
					*/
				};
				class DrawSimplePie : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 5*sizeof(double) + 2*sizeof(int);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(CenterX);
						pStream->WriteDouble(CenterY);
						pStream->WriteDouble(Radius);
						pStream->WriteDouble(StartAngle);
						pStream->WriteDouble(SweepAngle);
						pStream->WriteLong((int)Solid);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						CenterX = pStream->ReadDouble();
						CenterY = pStream->ReadDouble();
						Radius = pStream->ReadDouble();
						StartAngle = pStream->ReadDouble();
						SweepAngle = pStream->ReadDouble();
						Solid = (BOOL)pStream->ReadLong();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("centerx"), CenterX);
						oXmlWriter.WriteAttribute(_T("centery"), CenterY);
						oXmlWriter.WriteAttribute(_T("radius"), Radius);
						oXmlWriter.WriteAttribute(_T("startangle"), StartAngle);
						oXmlWriter.WriteAttribute(_T("sweepangle"), SweepAngle);
						oXmlWriter.WriteAttribute(_T("solid"), (int)Solid);

						Pen.ToXmlAttributes(oXmlWriter);
						Brush.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						CenterX = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("centerx"), _T("0")));
						CenterY = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("centery"), _T("0")));
						Radius = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("radius"), _T("0")));
						StartAngle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("startangle"), _T("0")));
						SweepAngle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("sweepangle"), _T("0")));
						Solid = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("solid"), _T("0")));

						Pen.FromXmlNode(oXmlNode);
						Brush.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						CenterX = 0;
						CenterY = 0;
						Radius = 0;
						StartAngle = 0;
						SweepAngle = 0;
						Solid = TRUE;

						Pen.Clear();
						Brush.Clear();
					}
					
				public:
					
					DrawSimplePie()
					{
						Initialize(c_nSerializePaintDrawSimplePie, c_strSerializePaintDrawSimplePie);

						Clear();
					}
						
				public:
					
					int Metric;
					BOOL Solid;
					double CenterX, CenterY;
					double Radius;
					double StartAngle, SweepAngle;

					ImageStudio::Serialize::Paint::Structures::Pen Pen;
					ImageStudio::Serialize::Paint::Structures::Brush Brush;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawSimplePie (c_nSerializePaintDrawSimplePie, c_strSerializePaintDrawSimplePie)
					{
						int Metric = c_nMetricPixels;		// xml имя - metriс; тип метрики, см. c_nMetric...
						BOOL Solid = TRUE;					// xml имя - solid; заливать объект brush'ем (1) или рисовать только его границу pen'ом

						// координаты центра окружности, из которой вырезан pie (в пикселях картинки или в другой какой-то метрике)
						double CenterX = 0;					// xml имя - centerx; 
						double CenterY = 0;					// xml имя - centery; 

						// радиус окружности, из которой вырезан pie (в пикселях картинки или в другой какой-то метрике, расчитанной относительно диагонали картинки)
						double Radius1 = 0;					// xml имя - radius1; 
						double Radius2 = 0;					// xml имя - radius2; 

						// углы, определяющие, какой сектор вырезать из окружности (в градусах, -360..360)
						double StartAngle = 0;				// xml имя - startangle; 
						double SweepAngle = 0;				// xml имя - sweepangle; 

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Pen Pen;			// xml имя - pen-...
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
					}
					*/
				};
				class DrawSimpleSector : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 6*sizeof(double) + 2*sizeof(int);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(CenterX);
						pStream->WriteDouble(CenterY);
						pStream->WriteDouble(Radius1);
						pStream->WriteDouble(Radius2);
						pStream->WriteDouble(StartAngle);
						pStream->WriteDouble(SweepAngle);
						pStream->WriteLong((int)Solid);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						CenterX = pStream->ReadDouble();
						CenterY = pStream->ReadDouble();
						Radius1 = pStream->ReadDouble();
						Radius2 = pStream->ReadDouble();
						StartAngle = pStream->ReadDouble();
						SweepAngle = pStream->ReadDouble();
						Solid = (BOOL)pStream->ReadLong();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("centerx"), CenterX);
						oXmlWriter.WriteAttribute(_T("centery"), CenterY);
						oXmlWriter.WriteAttribute(_T("radius1"), Radius1);
						oXmlWriter.WriteAttribute(_T("radius2"), Radius2);
						oXmlWriter.WriteAttribute(_T("startangle"), StartAngle);
						oXmlWriter.WriteAttribute(_T("sweepangle"), SweepAngle);
						oXmlWriter.WriteAttribute(_T("solid"), (int)Solid);

						Pen.ToXmlAttributes(oXmlWriter);
						Brush.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						CenterX = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("centerx"), _T("0")));
						CenterY = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("centery"), _T("0")));
						Radius1 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("radius1"), _T("0")));
						Radius2 = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("radius2"), _T("0")));
						StartAngle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("startangle"), _T("0")));
						SweepAngle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("sweepangle"), _T("0")));
						Solid = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("solid"), _T("0")));

						Pen.FromXmlNode(oXmlNode);
						Brush.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						CenterX = 0;
						CenterY = 0;
						Radius1 = 0;
						Radius2 = 0;
						StartAngle = 0;
						SweepAngle = 0;
						Solid = TRUE;

						Pen.Clear();
						Brush.Clear();
					}
					
				public:
					
					DrawSimpleSector()
					{
						Initialize(c_nSerializePaintDrawSimpleSector, c_strSerializePaintDrawSimpleSector);

						Clear();
					}
						
				public:
					
					int Metric;
					BOOL Solid;
					double CenterX, CenterY;
					double Radius1, Radius2;
					double StartAngle, SweepAngle;

					ImageStudio::Serialize::Paint::Structures::Pen Pen;
					ImageStudio::Serialize::Paint::Structures::Brush Brush;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawSimpleSector (c_nSerializePaintDrawSimpleSector, c_strSerializePaintDrawSimpleSector)
					{
						int Metric = c_nMetricPixels;		// xml имя - metriс; тип метрики, см. c_nMetric...
						BOOL Solid = TRUE;					// xml имя - solid; заливать объект brush'ем (1) или рисовать только его границу pen'ом

						// координаты центра окружности, из которой вырезан сектор (в пикселях картинки или в другой какой-то метрике)
						double CenterX = 0;					// xml имя - centerx; 
						double CenterY = 0;					// xml имя - centery; 

						// радиусы окружностей, которыми ограничен сектор (в пикселях картинки или в другой какой-то метрике, расчитанной относительно диагонали картинки)
						double Radius1 = 0;					// xml имя - radius1; 
						double Radius2 = 0;					// xml имя - radius2; 

						// углы, определяющие, какой сектор вырезать из окружности (в градусах, -360..360)
						double StartAngle = 0;				// xml имя - startangle; 
						double SweepAngle = 0;				// xml имя - sweepangle; 

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Pen Pen;			// xml имя - pen-...
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
					}
					*/
				};
				class DrawPolyline : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 2*sizeof(int) + Points.GetSize()*sizeof(double);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteLong(Points.GetSize());
						for (int index = 0; index < Points.GetSize(); ++index)
							pStream->WriteDouble(Points[index]);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();

						Points.RemoveAll();

						int nCount = pStream->ReadLong();

						for (int index = 0; index < nCount; ++index)
							Points.Add(pStream->ReadDouble());
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);

						_bstr_t strPointsData = Strings::FromBinary((BYTE*)Points.GetData(), Points.GetSize()*sizeof(double));

						oXmlWriter.WriteAttribute(_T("points-data"), (TCHAR*)strPointsData);

						Pen.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Points.RemoveAll();

						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));

						// try to load points from string like " 1 2 3 4 5 6" (string contains x,y pairs)
						CString strPoints = oXmlNode.GetAttributeOrValue(_T("points"), _T(""));
						if (strPoints.GetLength() > 0)
						{
							Templates::CArray<CString> arrSubstrings;
							int nCoordinates = Strings::Split(strPoints, _T(" "), arrSubstrings);
							
							if ((nCoordinates % 2) == 0)
							{
								for (int index = 0; index < arrSubstrings.GetCount(); ++index)
									Points.Add(Strings::ToDouble(arrSubstrings[index]));
							}
						}

						// try to load points from base64 encoded string

						_bstr_t strPointsData = oXmlNode.GetAttributeOrValue(_T("points-data"), _T(""));
						if (strPointsData.length() > 0)

						{
							BYTE* pData = 0;
							int nSizeAllocated = 0;
							int nSizeArray = 0;

							if (Strings::ToBinary(CString((char*)strPointsData), pData, nSizeAllocated, nSizeArray))
							{
								double* pPointsData = (double*)pData;
								int nPointsCount = nSizeArray/sizeof(double);

								if ((nSizeArray % sizeof(double)) == 0)
								{
									for (int index = 0; index < nPointsCount; ++index, ++pPointsData)
										Points.Add(*pPointsData);
								}

								delete[] pData;
							}
						}

						Pen.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Points.RemoveAll();

						Pen.Clear();
					}
					
				public:
				
					DrawPolyline()
					{
						Initialize(c_nSerializePaintDrawPolyline, c_strSerializePaintDrawPolyline);

						Clear();
					}
						
				public:
					
					int Metric;
					CSimpleArray<double> Points;

					ImageStudio::Serialize::Paint::Structures::Pen Pen;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawPolyline (c_nSerializePaintDrawPolyline, c_strSerializePaintDrawPolyline)
					{
						int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...

						// координаты точек polyline в пикселях картинки или в другой какой-то метрике (пары x-y)
						CSimpleArray<double> Points;	// xml имя - points-data; пример - 0 0 1 1 2 2

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Pen Pen;			// xml имя - pen-...
					}
					*/
				};
				class DrawPolygon : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 3*sizeof(int) + Points.GetSize()*sizeof(double);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteLong((int)Solid);
						pStream->WriteLong(Points.GetSize());
						for (int index = 0; index < Points.GetSize(); ++index)
							pStream->WriteDouble(Points[index]);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();

						Points.RemoveAll();

						Solid = (BOOL)pStream->ReadLong();

						int nCount = pStream->ReadLong();

						for (int index = 0; index < nCount; ++index)
							Points.Add(pStream->ReadDouble());
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);

						_bstr_t strPointsData = Strings::FromBinary((BYTE*)Points.GetData(), Points.GetSize()*sizeof(double));

						oXmlWriter.WriteAttribute(_T("points-data"), (TCHAR*)strPointsData);
						oXmlWriter.WriteAttribute(_T("solid"), (int)Solid);

						Pen.ToXmlAttributes(oXmlWriter);
						Brush.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Points.RemoveAll();

						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						Solid = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("solid"), _T("0")));

						// try to load points from string like " 1 2 3 4 5 6" (string contains x,y pairs)
						CString strPoints = oXmlNode.GetAttributeOrValue(_T("points"), _T(""));
						if (strPoints.GetLength() > 0)
						{
							Templates::CArray<CString> arrSubstrings;
							int nCoordinates = Strings::Split(strPoints, _T(" "), arrSubstrings);
							
							if ((nCoordinates % 2) == 0)
							{
								for (int index = 0; index < arrSubstrings.GetCount(); ++index)
									Points.Add(Strings::ToDouble(arrSubstrings[index]));
							}
						}

						// try to load points from base64 encoded string

						_bstr_t strPointsData = oXmlNode.GetAttributeOrValue(_T("points-data"), _T(""));
						if (strPointsData.length() > 0)

						{
							BYTE* pData = 0;
							int nSizeAllocated = 0;
							int nSizeArray = 0;

							if (Strings::ToBinary(CString((char*)strPointsData), pData, nSizeAllocated, nSizeArray))
							{
								double* pPointsData = (double*)pData;
								int nPointsCount = nSizeArray/sizeof(double);

								if ((nSizeArray % sizeof(double)) == 0)
								{
									for (int index = 0; index < nPointsCount; ++index, ++pPointsData)
										Points.Add(*pPointsData);
								}

								delete[] pData;
							}
						}

						Pen.FromXmlNode(oXmlNode);
						Brush.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Points.RemoveAll();

						Solid = TRUE;

						Pen.Clear();
						Brush.Clear();
					}
					
				public:
					
					DrawPolygon()
					{
						Initialize(c_nSerializePaintDrawPolygon, c_strSerializePaintDrawPolygon);

						Clear();
					}
						
				public:
					
					int Metric;
					BOOL Solid;
					CSimpleArray<double> Points;

					ImageStudio::Serialize::Paint::Structures::Pen Pen;
					ImageStudio::Serialize::Paint::Structures::Brush Brush;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawPolygon (c_nSerializePaintDrawPolygon, c_strSerializePaintDrawPolygon)
					{
						int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...
						BOOL Solid = TRUE;				// xml имя - solid; заливать объект brush'ем (1) или рисовать только его границу pen'ом (0)

						// координаты точек polygon в пикселях картинки или в другой какой-то метрике (пары x-y)
						CSimpleArray<double> Points;	// xml имя - points-data; пример - 0 0 1 1 2 2

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Pen Pen;			// xml имя - pen-...
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
					}
					*/
				};
				class DrawBorder : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 2*sizeof(int) + sizeof(double);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteLong(Type);
						pStream->WriteDouble(Size);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Type = pStream->ReadLong();
						Size = pStream->ReadDouble();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("type"), Type);
						oXmlWriter.WriteAttribute(_T("size"), Size);

						Brush.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("10100")));
						Size = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("size"), _T("0")));

						Brush.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Type = 10100;
						Size = 0;

						Brush.Clear();
					}
					
				public:
					
					DrawBorder()
					{
						Initialize(c_nSerializePaintDrawBorder, c_strSerializePaintDrawBorder);

						Clear();
					}
						
				public:
					
					int Metric;
					int Type;
					double Size;

					ImageStudio::Serialize::Paint::Structures::Brush Brush;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawBorder (c_nSerializePaintDrawBorder, c_strSerializePaintDrawBorder)
					{
						int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...
						int Type = 10100;				// xml имя - type; тип border'а, см. c_nBorder... значение 10100 соответствует типу "solid all"
						double Size = 0;				// xml имя - size; размер бордера в пикселях картинки или в другой какой-то метрике, расчитанной относительно диагонали картинки

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
					}
					*/
				};
				class DrawMovObject : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 4*sizeof(int);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Type);
						pStream->WriteLong(Divisions);
						pStream->WriteLong(Color);
						pStream->WriteLong(Alpha);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Type = pStream->ReadLong();
						Divisions = pStream->ReadLong();
						Color = pStream->ReadLong();
						Alpha = pStream->ReadLong();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("type"), Type);
						oXmlWriter.WriteAttribute(_T("divisions"), Divisions);
						oXmlWriter.WriteAttribute(_T("color"), Color);
						oXmlWriter.WriteAttribute(_T("alpha"), Alpha);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("1100")));
						Divisions = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("divisions"), _T("1")));
						Color = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("color"), _T("0")));
						Alpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("alpha"), _T("255")));
					}
					
					virtual void InternalClear()
					{
						Type = 1100;
						Divisions = 1;
						Color = 0;
						Alpha = 255;
					}
					
				public:
					
					DrawMovObject()
					{
						Initialize(c_nSerializePaintDrawMovObject, c_strSerializePaintDrawMovObject);

						Clear();
					}
						
				public:
					
					int Type;
					int Divisions;
					int Color;
					int Alpha;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawMovObject (c_nSerializePaintDrawMovObject, c_strSerializePaintDrawMovObject)
					{
						int Type = 1100;				// xml имя - type; тип анимации цветовой заливки, см. константы в TransformConstants.h; значение 1100 соответствует "c_nWipeDoorVerticalIn"
						int Divisions = 1;				// xml имя - divisions; количество разбиений цветовой заливки (tile'ов) по ширине/высоте (значение изменяется от 1 до min(width/4, height/4))
						int Color = 0;					// xml имя - color; цвет заливки : colorref
						int Alpha = 255;				// xml имя - alpha; прозрачность цветовой заливки: 0..255
					}
					*/
				};
				class DrawCredits : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 2*sizeof(int) + Text.GetLength() + 1;
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteLong(Type);
						pStream->WriteString(Text);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Type = pStream->ReadLong();
						Text = pStream->ReadString();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("type"), Type);
						oXmlWriter.WriteAttribute(_T("text"), Text);

						Brush.ToXmlAttributes(oXmlWriter);
						Font.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						Type = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("5100")));
						Text = oXmlNode.GetAttributeOrValue(_T("text"), _T(""));

						Brush.FromXmlNode(oXmlNode);
						Font.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Type = 5100;
						Text = _T("");

						Brush.Clear();
						Font.Clear();
					}
					
				public:
					
					DrawCredits()
					{
						Initialize(c_nSerializePaintDrawCredits, c_strSerializePaintDrawCredits);

						Clear();
					}
						
				public:
					
					int Metric;
					int Type;
					CString Text;

					ImageStudio::Serialize::Paint::Structures::Brush Brush;
					ImageStudio::Serialize::Paint::Structures::Font Font;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawCredits (c_nSerializePaintDrawCredits, c_strSerializePaintDrawCredits)
					{
						int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...

						int Type = 5100;				// xml имя - type; тип анимации титров, см. c_nCredits...
						CString Text = "";				// xml имя - text; текст для отрисовки

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						// TODO: в данный момент не используются, поскольку не используются низкоуровневой функцией
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
						ImageStudio::Serialize::Paint::Structures::Font Font;		// xml имя - font-...
					}
					*/
				};
				class DrawTextSimple : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 2*sizeof(double) + Text.GetLength() + 1 + sizeof(int);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(Left);
						pStream->WriteDouble(Top);
						pStream->WriteString(Text);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Left = pStream->ReadDouble();
						Top = pStream->ReadDouble();
						Text = pStream->ReadString();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("left"), Left);
						oXmlWriter.WriteAttribute(_T("top"), Top);
						oXmlWriter.WriteAttribute(_T("text"), Text);

						Brush.ToXmlAttributes(oXmlWriter);
						Font.ToXmlAttributes(oXmlWriter);

						oXmlWriter.WriteAttribute(_T("aligntype"), nAlignType);
						oXmlWriter.WriteAttribute(_T("margin"), nMargin);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						bAlign = true;

						CString sLeft = oXmlNode.GetAttributeOrValue(_T("left"));
						CString sTop  = oXmlNode.GetAttributeOrValue(_T("top"));

						if ( sLeft.GetLength() + sTop.GetLength() > 0 )
							bAlign = false;
						
						Metric     = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						Left       = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("left"), _T("0")));
						Top        = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("top"), _T("0")));
						Text       = oXmlNode.GetAttributeOrValue(_T("text"), _T(""));

						Brush.FromXmlNode(oXmlNode);
						Font.FromXmlNode(oXmlNode);

						nAlignType = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("aligntype"), _T("4")));
						nMargin    = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("margin"), _T("10")));
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Left = 0;
						Top = 0;
						Text = _T("");
						nAlignType = 4;
						nMargin = 10;
						bAlign = false;

						Brush.Clear();
						Font.Clear();
					}
					
				public:
					
					DrawTextSimple()
					{
						Initialize(c_nSerializePaintDrawTextSimple, c_strSerializePaintDrawTextSimple);

						Clear();
					}
						
				public:
					
					int Metric;
					CString Text;
					double Left, Top;

					int nAlignType; // 0..8, default = 4 (по центру экрана)
					int nMargin;    // отступ от краев картинки, в случае bAlign = true

					bool bAlign; 
					//  true: выравниваем текст в соответствии с nAlignType
					// false: выравниваем текст в соответствии с Left, Top
					 
					
					ImageStudio::Serialize::Paint::Structures::Brush Brush;
					ImageStudio::Serialize::Paint::Structures::Font Font;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawTextSimple (c_nSerializePaintDrawTextSimple, c_strSerializePaintDrawTextSimple)
					{
						int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...
						CString Text = "";				// xml имя - text; текст для отрисовки

						// координаты левого верхнего угла текста (в пикселях картинки или в другой какой-то метрике)
						double Left = 0;				// xml имя - left
						double Top = 0;					// xml имя - top

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
						ImageStudio::Serialize::Paint::Structures::Font Font;		// xml имя - font-...
					}
					*/
				};
				class DrawTextInRect : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 4*sizeof(double) + 2*sizeof(int) + Text.GetLength() + 1;
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(Left);
						pStream->WriteDouble(Top);
						pStream->WriteDouble(Right);
						pStream->WriteDouble(Bottom);
						pStream->WriteLong(Scale);
						pStream->WriteString(Text);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Left = pStream->ReadDouble();
						Top = pStream->ReadDouble();
						Right = pStream->ReadDouble();
						Bottom = pStream->ReadDouble();
						Scale = pStream->ReadLong();
						Text = pStream->ReadString();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("left"), Left);
						oXmlWriter.WriteAttribute(_T("top"), Top);
						oXmlWriter.WriteAttribute(_T("right"), Right);
						oXmlWriter.WriteAttribute(_T("bottom"), Bottom);
						oXmlWriter.WriteAttribute(_T("scale"), Scale);
						oXmlWriter.WriteAttribute(_T("text"), Text);

						Brush.ToXmlAttributes(oXmlWriter);
						Font.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						Left = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("left"), _T("0")));
						Top = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("top"), _T("0")));
						Right = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("right"), _T("0")));
						Bottom = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("bottom"), _T("0")));
						Scale = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("scale"), _T("0")));
						Text = oXmlNode.GetAttributeOrValue(_T("text"), _T(""));

						Brush.FromXmlNode(oXmlNode);
						Font.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Left = 0;
						Top = 0;
						Right = 0;
						Bottom = 0;
						Scale = 0;
						Text = _T("");

						Brush.Clear();
						Font.Clear();
					}
					
				public:
					
					DrawTextInRect()
					{
						Initialize(c_nSerializePaintDrawTextInRect, c_strSerializePaintDrawTextInRect);

						Clear();
					}
						
				public:
					
					int Metric;
					CString Text;
					int Scale;
					double Left, Top, Right, Bottom;

					ImageStudio::Serialize::Paint::Structures::Brush Brush;
					ImageStudio::Serialize::Paint::Structures::Font Font;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawTextInRect (c_nSerializePaintDrawTextInRect, c_strSerializePaintDrawTextInRect)
					{
						int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...
						CString Text = "";				// xml имя - text; текст для отрисовки
						int Scale = 0;					// xml имя - scale; значение, отвечающее за автоматическое растяжение текста в указанный прямоугольник: 1 - растягивать, 0 - не растягивать

						// прямоугольник, описанный около текста (в пикселях картинки или в другой какой-то метрике)
						double Left = 0;				// xml имя - left
						double Top = 0;					// xml имя - top
						double Right = 0;				// xml имя - right
						double Bottom = 0;				// xml имя - bottom

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
						ImageStudio::Serialize::Paint::Structures::Font Font;		// xml имя - font-...
					}
					*/
				};
				class DrawImageFromFile : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 5*sizeof(double) + 6*sizeof(int) + FilePath.GetLength() + 1;
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(Left);
						pStream->WriteDouble(Top);
						pStream->WriteDouble(Right);
						pStream->WriteDouble(Bottom);
						pStream->WriteDouble(Angle);
						pStream->WriteString(FilePath);
						pStream->WriteLong(BackColor);
						pStream->WriteLong(Alpha);
						pStream->WriteLong(nDrawImageScaleType);
						pStream->WriteLong(nDrawImageScaleBackColor);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Left = pStream->ReadDouble();
						Top = pStream->ReadDouble();
						Right = pStream->ReadDouble();
						Bottom = pStream->ReadDouble();
						Angle = pStream->ReadDouble();
						FilePath = pStream->ReadString();
						BackColor = pStream->ReadLong();
						Alpha = pStream->ReadLong();
						nDrawImageScaleType = pStream->ReadLong();
						nDrawImageScaleBackColor = pStream->ReadLong();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("left"), Left);
						oXmlWriter.WriteAttribute(_T("top"), Top);
						oXmlWriter.WriteAttribute(_T("angle"), Angle);
						oXmlWriter.WriteAttribute(_T("filepath"), FilePath);
						oXmlWriter.WriteAttribute(_T("backcolor"), BackColor);
						oXmlWriter.WriteAttribute(_T("alpha"), Alpha);
						oXmlWriter.WriteAttribute(_T("scaletype"), nDrawImageScaleType);
						oXmlWriter.WriteAttribute(_T("scalecolor"), nDrawImageScaleBackColor);

						if ( bAlign )
						{
							oXmlWriter.WriteAttribute(_T("aligntype"), nAlignType);
							oXmlWriter.WriteAttribute(_T("marginhorizontal"), nMarginX);
							oXmlWriter.WriteAttribute(_T("marginvertical"),   nMarginX);
						}
						switch(nSizeType)
						{
						case 0: 
							oXmlWriter.WriteAttribute(_T("bottom"), Bottom);
							oXmlWriter.WriteAttribute(_T("right"), Right);
							break;
						case 1:
							oXmlWriter.WriteAttribute(_T("width"), nWidth);
							oXmlWriter.WriteAttribute(_T("height"), nHeight);
							break;
						case 2:
							oXmlWriter.WriteAttribute(_T("width"), nWidth);
							break;
						case 3:
							oXmlWriter.WriteAttribute(_T("height"), nHeight);
							break;
						}

						oXmlWriter.WriteAttribute(_T("widthmetric"), dWidthMetric);
						oXmlWriter.WriteAttribute(_T("heightmetric"), dHeightMetric);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttribute(_T("metric"), _T("0")));
						Left = Strings::ToDouble(oXmlNode.GetAttribute(_T("left"), _T("0")));
						Top = Strings::ToDouble(oXmlNode.GetAttribute(_T("top"), _T("0")));
						Right = Strings::ToDouble(oXmlNode.GetAttribute(_T("right"), _T("0")));
						Bottom = Strings::ToDouble(oXmlNode.GetAttribute(_T("bottom"), _T("0")));
						Angle = Strings::ToDouble(oXmlNode.GetAttribute(_T("angle"), _T("0")));
						FilePath = oXmlNode.GetAttributeOrValue(_T("filepath"), _T(""));
						BackColor = Strings::ToInteger(oXmlNode.GetAttribute(_T("backcolor"), _T("-1")));
						Alpha = Strings::ToInteger(oXmlNode.GetAttribute(_T("alpha"), _T("255")));
						nDrawImageScaleType = Strings::ToInteger(oXmlNode.GetAttribute(_T("scaletype"), _T("-1")));
						nDrawImageScaleBackColor = Strings::ToInteger(oXmlNode.GetAttribute(_T("scalecolor"), _T("-1")));

						dWidthMetric = Strings::ToDouble(oXmlNode.GetAttribute(_T("widthmetric"), _T("0")));
						dHeightMetric = Strings::ToDouble(oXmlNode.GetAttribute(_T("heightmetric"), _T("0")));

						CString sAlign = oXmlNode.GetAttribute(_T("aligntype"));
						bAlign = FALSE;
						if ( sAlign.GetLength() > 0 )
							bAlign = TRUE;

						nAlignType = Strings::ToInteger(oXmlNode.GetAttribute(_T("aligntype"), _T("4")));
						nMarginX   = Strings::ToInteger(oXmlNode.GetAttribute(_T("marginhorizontal"), _T("10")));
						nMarginY   = Strings::ToInteger(oXmlNode.GetAttribute(_T("marginvertical"),   _T("10")));
						nWidth     = Strings::ToInteger(oXmlNode.GetAttribute(_T("width"),  _T("0")));
						nHeight    = Strings::ToInteger(oXmlNode.GetAttribute(_T("height"), _T("0")));

						CString sTemp = oXmlNode.GetAttribute(_T("right"));
						if ( sTemp.GetLength() < 1 )
							nSizeType = 1;

						sTemp = oXmlNode.GetAttribute(_T("bottom"));
						if ( sTemp.GetLength() < 1 )
							nSizeType = 1;

						if ( 1 == nSizeType )
						{
							sTemp = oXmlNode.GetAttribute(_T("height"));
							if ( sTemp.GetLength() < 1 )
								nSizeType = 2;

							sTemp = oXmlNode.GetAttribute(_T("width"));
							if ( 2!= nSizeType && sTemp.GetLength() < 1)
								nSizeType = 3;

							if ( 2 == nSizeType && sTemp.GetLength() < 1)
								nSizeType = 0;

						}

					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Left = 0;
						Top = 0;
						Right = 0;
						Bottom = 0;
						Angle = 0;
						FilePath = _T("");

						nAlignType = 4;
						nSizeType = 0;
						bAlign = FALSE;
						nWidth = 0 ;
						nHeight = 0;

						nMarginX = 10;
						nMarginY = 10;

						BackColor = -1;
						Alpha = 255;

						nDrawImageScaleType = -1;
						nDrawImageScaleBackColor = -1;

						dWidthMetric = 0.0;
						dHeightMetric = 0.0;
					}
					
				public:
					
					DrawImageFromFile()
					{
						Initialize(c_nSerializePaintDrawImageFromFile, c_strSerializePaintDrawImageFromFile);

						Clear();
					}
						
				public:
					
					int Metric;
					
					double Left, Top, Right, Bottom;
					double Angle;
					int BackColor;
					int Alpha;

					BOOL   bAlign;
					int    nAlignType;
					int    nSizeType; 
					// 0 - если заданы: left, top, right, bottom
					// 1 - если заданы: left, top, width, height
					// 2 - если заданы: left, top, width 
					// 3 - если заданы: left, top, height 
					
					int    nMarginX;
					int    nMarginY;
					int    nWidth;
					int    nHeight;
					
					CString FilePath;

					int nDrawImageScaleType; 
					int nDrawImageScaleBackColor;

					// для дополнительной метрики (используется в презентациях)
					double dWidthMetric;
					double dHeightMetric;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawImageFromFile (c_nSerializePaintDrawImageFromFile, c_strSerializePaintDrawImageFromFile)
					{
						int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...
						CString FilePath = "";			// xml имя - filepath; путь к файлу с картинкой для отрисовки

						// прямоугольник, описанный около картинки (в пикселях картинки или в другой какой-то метрике)
						double Left = 0;				// xml имя - left
						double Top = 0;					// xml имя - top
						double Right = 0;				// xml имя - right
						double Bottom = 0;				// xml имя - bottom

						// цвет, который будет "удален" из картинки (т.е. в некотором смысле - color key)
						int BackColor = -1;				// xml имя - backcolor

						int nDrawImageScaleType;         // stretch, shrink, crop (см. TransformConstants)
						int nDrawImageScaleBackColor;   // как и BackColor в Resize
					}
					*/
				};
				class DrawImageFromFileEx : public Core::Effect
				{
				protected:
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("src-metric"), nSrcMetric);
						oXmlWriter.WriteAttribute(_T("src-x"), dSrcX);
						oXmlWriter.WriteAttribute(_T("src-y"), dSrcY);
						oXmlWriter.WriteAttribute(_T("src-width"), dSrcWidth);
						oXmlWriter.WriteAttribute(_T("src-height"), dSrcHeight);
						oXmlWriter.WriteAttribute(_T("src-colorkey"), nSrcColorKey);
						oXmlWriter.WriteAttribute(_T("src-buffer"), nSrcBuffer);
						
						oXmlWriter.WriteAttribute(_T("dst-metric"), nDstMetric);
						oXmlWriter.WriteAttribute(_T("dst-x"), dDstX);
						oXmlWriter.WriteAttribute(_T("dst-y"), dDstY);
						oXmlWriter.WriteAttribute(_T("dst-width"), dDstWidth);
						oXmlWriter.WriteAttribute(_T("dst-height"), dDstHeight);

						oXmlWriter.WriteAttribute(_T("angle"), dAngle);
						oXmlWriter.WriteAttribute(_T("alpha"), dAlpha);
						oXmlWriter.WriteAttribute(_T("cropscale"), dCropScale);
						oXmlWriter.WriteAttribute(_T("scaletype"), nScaleType);

						oXmlWriter.WriteAttribute(_T("filepath"), sFilePath);
						oXmlWriter.WriteAttribute(_T("objectid"), sObjectId);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						nSrcMetric = Strings::ToInteger(oXmlNode.GetAttribute(_T("src-metric"), _T("0")));
						dSrcX      = Strings::ToDouble(oXmlNode.GetAttribute(_T("src-x"), _T("0")));
						dSrcY      = Strings::ToDouble(oXmlNode.GetAttribute(_T("src-y"), _T("0")));
						dSrcWidth  = Strings::ToDouble(oXmlNode.GetAttribute(_T("src-width"), _T("0")));
						dSrcHeight = Strings::ToDouble(oXmlNode.GetAttribute(_T("src-height"), _T("0")));
						nSrcColorKey = Strings::ToInteger(oXmlNode.GetAttribute(_T("src-colorkey"), _T("-1")));
						nSrcBuffer = Strings::ToInteger(oXmlNode.GetAttribute(_T("src-buffer"), _T("-1")));

						nDstMetric = Strings::ToInteger(oXmlNode.GetAttribute(_T("dst-metric"), _T("0")));
						dDstX      = Strings::ToDouble(oXmlNode.GetAttribute(_T("dst-x"), _T("0")));
						dDstY      = Strings::ToDouble(oXmlNode.GetAttribute(_T("dst-y"), _T("0")));
						dDstWidth  = Strings::ToDouble(oXmlNode.GetAttribute(_T("dst-width"), _T("0")));
						dDstHeight = Strings::ToDouble(oXmlNode.GetAttribute(_T("dst-height"), _T("0")));

						dAngle     = Strings::ToDouble(oXmlNode.GetAttribute(_T("angle"), _T("0")));
						dAlpha     = Strings::ToDouble(oXmlNode.GetAttribute(_T("alpha"), _T("255")));
						dCropScale = Strings::ToDouble(oXmlNode.GetAttribute(_T("cropscale"), _T("1")));
						nScaleType = Strings::ToInteger(oXmlNode.GetAttribute(_T("scaletype"), _T("0")));
						sFilePath  = oXmlNode.GetAttributeOrValue(_T("filepath"), _T(""));
						sObjectId  = oXmlNode.GetAttributeOrValue(_T("objectid"), _T(""));
					}
					
					virtual void InternalClear()
					{
						nSrcMetric = 0;
						dSrcX = 0;
						dSrcY = 0;
						dSrcWidth = 0;
						dSrcHeight = 0;
						nSrcColorKey = -1;
					
						nDstMetric = 0;
						dDstX = 0;
						dDstY = 0;
						dDstWidth = 0;
						dDstHeight = 0;
					
						dAngle = 0;
						dAlpha = 255;
						dCropScale = 1;
						nScaleType = 0; // тип масштабирования и интерполяции

						sFilePath = _T("");
						sObjectId = _T("");
						nSrcBuffer = -1;
					}
					
				public:
					
					DrawImageFromFileEx()
					{
						Initialize(c_nSerializePaintDrawImageFromFileEx, c_strSerializePaintDrawImageFromFileEx);

						Clear();
					}
						
				public:
					
					int nSrcMetric;
					double dSrcX, dSrcY, dSrcWidth, dSrcHeight;
					int nSrcColorKey;

					int nDstMetric;
					double dDstX, dDstY, dDstWidth, dDstHeight;
					
					double dAngle;
					double dAlpha;
					double dCropScale;

					int nScaleType; // тип масштабирования и интерполяции

					CString sFilePath;
					CString sObjectId;
					int nSrcBuffer;
				};
				class DrawImageFromStorage : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 5*sizeof(double) + 5*sizeof(int) + ObjectId.GetLength() + 1;
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(Left);
						pStream->WriteDouble(Top);
						pStream->WriteDouble(Right);
						pStream->WriteDouble(Bottom);
						pStream->WriteDouble(Angle);
						pStream->WriteString(ObjectId);
						pStream->WriteLong(BackColor);
						pStream->WriteLong(Alpha);
						pStream->WriteLong(nDrawImageScaleType);
						pStream->WriteLong(nDrawImageScaleBackColor);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Left = pStream->ReadDouble();
						Top = pStream->ReadDouble();
						Right = pStream->ReadDouble();
						Bottom = pStream->ReadDouble();
						Angle = pStream->ReadDouble();
						ObjectId = pStream->ReadString();
						BackColor = pStream->ReadLong();
						Alpha = pStream->ReadLong();
						nDrawImageScaleType = pStream->ReadLong();
						nDrawImageScaleBackColor = pStream->ReadLong();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("left"), Left);
						oXmlWriter.WriteAttribute(_T("top"), Top);
						oXmlWriter.WriteAttribute(_T("angle"), Angle);
						oXmlWriter.WriteAttribute(_T("objectid"), ObjectId);
						oXmlWriter.WriteAttribute(_T("backcolor"), BackColor);
						oXmlWriter.WriteAttribute(_T("alpha"), Alpha);
						oXmlWriter.WriteAttribute(_T("scaletype"), nDrawImageScaleType);
						oXmlWriter.WriteAttribute(_T("scalecolor"), nDrawImageScaleBackColor);
						if ( bAlign )
						{
							oXmlWriter.WriteAttribute(_T("aligntype"), nAlignType);
							oXmlWriter.WriteAttribute(_T("marginhorizontal"), nMarginX);
							oXmlWriter.WriteAttribute(_T("marginvertical"),   nMarginX);
						}
						switch(nSizeType)
						{
						case 0: 
							oXmlWriter.WriteAttribute(_T("bottom"), Bottom);
							oXmlWriter.WriteAttribute(_T("right"), Right);
							break;
						case 1:
							oXmlWriter.WriteAttribute(_T("width"), nWidth);
							oXmlWriter.WriteAttribute(_T("height"), nHeight);
							break;
						case 2:
							oXmlWriter.WriteAttribute(_T("width"), nWidth);
							break;
						case 3:
							oXmlWriter.WriteAttribute(_T("height"), nHeight);
							break;
						}
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttribute(_T("metric"), _T("0")));
						Left = Strings::ToDouble(oXmlNode.GetAttribute(_T("left"), _T("0")));
						Top = Strings::ToDouble(oXmlNode.GetAttribute(_T("top"), _T("0")));
						Right = Strings::ToDouble(oXmlNode.GetAttribute(_T("right"), _T("0")));
						Bottom = Strings::ToDouble(oXmlNode.GetAttribute(_T("bottom"), _T("0")));
						Angle = Strings::ToDouble(oXmlNode.GetAttribute(_T("angle"), _T("0")));
						ObjectId = oXmlNode.GetAttributeOrValue(_T("objectid"), _T(""));
						BackColor = Strings::ToInteger(oXmlNode.GetAttribute(_T("backcolor"), _T("-1")));
						Alpha = Strings::ToInteger(oXmlNode.GetAttribute(_T("alpha"), _T("255")));
						nDrawImageScaleType = Strings::ToInteger(oXmlNode.GetAttribute(_T("scaletype"), _T("-1")));
						nDrawImageScaleBackColor = Strings::ToInteger(oXmlNode.GetAttribute(_T("scalecolor"), _T("-1")));
						CString sAlign = oXmlNode.GetAttribute(_T("aligntype"));
						bAlign = FALSE;
						if ( sAlign.GetLength() > 0 )
							bAlign = TRUE;

						nAlignType = Strings::ToInteger(oXmlNode.GetAttribute(_T("aligntype"), _T("4")));
						nMarginX   = Strings::ToInteger(oXmlNode.GetAttribute(_T("marginhorizontal"), _T("10")));
						nMarginY   = Strings::ToInteger(oXmlNode.GetAttribute(_T("marginvertical"),   _T("10")));
						nWidth     = Strings::ToInteger(oXmlNode.GetAttribute(_T("width"),  _T("0")));
						nHeight    = Strings::ToInteger(oXmlNode.GetAttribute(_T("height"), _T("0")));

						CString sTemp = oXmlNode.GetAttribute(_T("right"));
						if ( sTemp.GetLength() < 1 )
							nSizeType = 1;

						sTemp = oXmlNode.GetAttribute(_T("bottom"));
						if ( sTemp.GetLength() < 1 )
							nSizeType = 1;

						if ( 1 == nSizeType )
						{
							sTemp = oXmlNode.GetAttribute(_T("height"));
							if ( sTemp.GetLength() < 1 )
								nSizeType = 2;

							sTemp = oXmlNode.GetAttribute(_T("width"));
							if ( 2!= nSizeType && sTemp.GetLength() < 1)
								nSizeType = 3;

							if ( 2 == nSizeType && sTemp.GetLength() < 1)
								nSizeType = 0;

						}
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Left = 0;
						Top = 0;
						Right = 0;
						Bottom = 0;
						Angle = 0;
						ObjectId = _T("");

						nAlignType = 4;
						nSizeType = 0;
						bAlign = FALSE;
						nWidth = 0 ;
						nHeight = 0;

						nMarginX = 10;
						nMarginY = 10;

						BackColor = -1;
						Alpha = 255;

						nDrawImageScaleType = -1;
						nDrawImageScaleBackColor = -1;
					}
					
				public:
					
					DrawImageFromStorage()
					{
						Initialize(c_nSerializePaintDrawImageFromStorage, c_strSerializePaintDrawImageFromStorage);

						Clear();
					}
						
				public:
					
					int Metric;
					
					double Left, Top, Right, Bottom;
					double Angle;
					int BackColor;
					int Alpha;

					BOOL   bAlign;
					int    nAlignType;
					int    nSizeType; 
					// 0 - если заданы: left, top, right, bottom
					// 1 - если заданы: left, top, width, height
					// 2 - если заданы: left, top, width 
					// 3 - если заданы: left, top, height 
					
					int    nMarginX;
					int    nMarginY;
					int    nWidth;
					int    nHeight;

					CString ObjectId;

					int nDrawImageScaleType; 
					int nDrawImageScaleBackColor;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawImageFromFile (c_nSerializePaintDrawImageFromFile, c_strSerializePaintDrawImageFromFile)
					{
						int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...
						CString FilePath = "";			// xml имя - filepath; путь к файлу с картинкой для отрисовки

						// прямоугольник, описанный около картинки (в пикселях картинки или в другой какой-то метрике)
						double Left = 0;				// xml имя - left
						double Top = 0;					// xml имя - top
						double Right = 0;				// xml имя - right
						double Bottom = 0;				// xml имя - bottom

						// цвет, который будет "удален" из картинки (т.е. в некотором смысле - color key)
						int BackColor = -1;				// xml имя - backcolor

						int nDrawImageScaleType;         // stretch, shrink, crop (см. TransformConstants)
						int nDrawImageScaleBackColor;   // как и BackColor в Resize
					}
					*/
				};
				class DrawImageFromBuffer : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 5*sizeof(double) + 5*sizeof(int);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(Left);
						pStream->WriteDouble(Top);
						pStream->WriteDouble(Right);
						pStream->WriteDouble(Bottom);
						pStream->WriteDouble(Angle);
						pStream->WriteLong(Buffer);
						pStream->WriteLong(Alpha);
						pStream->WriteLong(nDrawImageScaleType);
						pStream->WriteLong(nDrawImageScaleBackColor);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Left = pStream->ReadDouble();
						Top = pStream->ReadDouble();
						Right = pStream->ReadDouble();
						Bottom = pStream->ReadDouble();
						Angle = pStream->ReadDouble();
						Buffer = pStream->ReadLong();
						Alpha = pStream->ReadLong();
						nDrawImageScaleType = pStream->ReadLong();
						nDrawImageScaleBackColor = pStream->ReadLong();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("left"), Left);
						oXmlWriter.WriteAttribute(_T("top"), Top);
						oXmlWriter.WriteAttribute(_T("angle"), Angle);
						oXmlWriter.WriteAttribute(_T("buffer"), Buffer);
						oXmlWriter.WriteAttribute(_T("alpha"), Alpha);
						oXmlWriter.WriteAttribute(_T("scaletype"), nDrawImageScaleType);
						oXmlWriter.WriteAttribute(_T("scalecolor"), nDrawImageScaleBackColor);
						if ( bAlign )
						{
							oXmlWriter.WriteAttribute(_T("aligntype"), nAlignType);
							oXmlWriter.WriteAttribute(_T("marginhorizontal"), nMarginX);
							oXmlWriter.WriteAttribute(_T("marginvertical"),   nMarginX);
						}
						switch(nSizeType)
						{
						case 0: 
							oXmlWriter.WriteAttribute(_T("bottom"), Bottom);
							oXmlWriter.WriteAttribute(_T("right"), Right);
							break;
						case 1:
							oXmlWriter.WriteAttribute(_T("width"), nWidth);
							oXmlWriter.WriteAttribute(_T("height"), nHeight);
							break;
						case 2:
							oXmlWriter.WriteAttribute(_T("width"), nWidth);
							break;
						case 3:
							oXmlWriter.WriteAttribute(_T("height"), nHeight);
							break;
						}
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttribute(_T("metric"), _T("0")));
						Left = Strings::ToDouble(oXmlNode.GetAttribute(_T("left"), _T("0")));
						Top = Strings::ToDouble(oXmlNode.GetAttribute(_T("top"), _T("0")));
						Right = Strings::ToDouble(oXmlNode.GetAttribute(_T("right"), _T("0")));
						Bottom = Strings::ToDouble(oXmlNode.GetAttribute(_T("bottom"), _T("0")));
						Angle = Strings::ToDouble(oXmlNode.GetAttribute(_T("angle"), _T("0")));
						Buffer = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("buffer"), _T("-1")));
						Alpha = Strings::ToInteger(oXmlNode.GetAttribute(_T("alpha"), _T("255")));
						nDrawImageScaleType = Strings::ToInteger(oXmlNode.GetAttribute(_T("scaletype"), _T("-1")));
						nDrawImageScaleBackColor = Strings::ToInteger(oXmlNode.GetAttribute(_T("scalecolor"), _T("-1")));
						CString sAlign = oXmlNode.GetAttribute(_T("aligntype"));
						bAlign = FALSE;
						if ( sAlign.GetLength() > 0 )
							bAlign = TRUE;

						nAlignType = Strings::ToInteger(oXmlNode.GetAttribute(_T("aligntype"), _T("4")));
						nMarginX   = Strings::ToInteger(oXmlNode.GetAttribute(_T("marginhorizontal"), _T("10")));
						nMarginY   = Strings::ToInteger(oXmlNode.GetAttribute(_T("marginvertical"),   _T("10")));
						nWidth     = Strings::ToInteger(oXmlNode.GetAttribute(_T("width"),  _T("0")));
						nHeight    = Strings::ToInteger(oXmlNode.GetAttribute(_T("height"), _T("0")));

						CString sTemp = oXmlNode.GetAttribute(_T("right"));
						if ( sTemp.GetLength() < 1 )
							nSizeType = 1;

						sTemp = oXmlNode.GetAttribute(_T("bottom"));
						if ( sTemp.GetLength() < 1 )
							nSizeType = 1;

						if ( 1 == nSizeType )
						{
							sTemp = oXmlNode.GetAttribute(_T("height"));
							if ( sTemp.GetLength() < 1 )
								nSizeType = 2;

							sTemp = oXmlNode.GetAttribute(_T("width"));
							if ( 2!= nSizeType && sTemp.GetLength() < 1)
								nSizeType = 3;

							if ( 2 == nSizeType && sTemp.GetLength() < 1)
								nSizeType = 0;

						}
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Left = 0;
						Top = 0;
						Right = 0;
						Bottom = 0;
						Angle = 0;
						Buffer = -1;
						Alpha = 255;
						
						nAlignType = 4;
						nSizeType = 0;
						bAlign = FALSE;
						nWidth = 0 ;
						nHeight = 0;

						nMarginX = 10;
						nMarginY = 10;

						nDrawImageScaleType = -1;
						nDrawImageScaleBackColor = -1;
					}
					
				public:
					
					DrawImageFromBuffer()
					{
						Initialize(c_nSerializePaintDrawImageFromBuffer, c_strSerializePaintDrawImageFromBuffer);

						Clear();
					}
						
				public:
					
					int Metric;
					double Left, Top, Right, Bottom;
					double Angle;
					int Buffer;
					int Alpha;

					BOOL   bAlign;
					int    nAlignType;
					int    nSizeType; 
					// 0 - если заданы: left, top, right, bottom
					// 1 - если заданы: left, top, width, height
					// 2 - если заданы: left, top, width 
					// 3 - если заданы: left, top, height 
					
					int    nMarginX;
					int    nMarginY;
					int    nWidth;
					int    nHeight;

					int nDrawImageScaleType; 
					int nDrawImageScaleBackColor;

					/* @desc:
					ImageStudio-Serialize-Paint-Common-DrawImageFromBuffer (c_nSerializePaintDrawImageFromBuffer, c_strSerializePaintDrawImageFromBuffer)
					{
						int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...
						int Buffer = -1;				// xml имя - buffer; индекс буффера с картинкой, которую надо отрисовать в картинке в 0-м буффере

						// прямоугольник, описанный около картинки (в пикселях картинки или в другой какой-то метрике)
						double Left = 0;				// xml имя - left
						double Top = 0;					// xml имя - top
						double Right = 0;				// xml имя - right
						double Bottom = 0;				// xml имя - bottom

						int nDrawImageScaleType;         // stretch, shrink, crop (см. TransformConstants)
						int nDrawImageScaleBackColor;   // как и BackColor в Resize
					}
					*/
				};
				class DrawTimer : public Core::Effect
				{
				protected:
					class PartTime
					{
					public:
						PartTime()
						{
							Clear();
						}
						
						void Clear()
						{
							m_sPrefix = _T("");
							m_sSuffix = _T("");

							m_nStart  = 0;
							m_nEnd    = 0;
							m_nDigits = 0;

							m_bVisible = TRUE;
						}
						void ToXmlNode( XmlUtils::CXmlWriter& oXmlWriter, const CString& sName )
						{
							if( sName.IsEmpty() )
								return;

							oXmlWriter.WriteAttribute( sName + _T("prefix"),  m_sPrefix );
							oXmlWriter.WriteAttribute( sName + _T("suffix"),  m_sSuffix );
							oXmlWriter.WriteAttribute( sName + _T("start"),   m_nStart );
							oXmlWriter.WriteAttribute( sName + _T("end"),     m_nEnd );
							oXmlWriter.WriteAttribute( sName + _T("digits"),  m_nDigits );
							oXmlWriter.WriteAttribute( sName + _T("visible"), m_bVisible );
						}
						void FromXmlNode( XmlUtils::CXmlNode& oXmlNode, const CString& sName )
						{

							m_sPrefix  = oXmlNode.GetAttribute( sName + _T("prefix") );
							m_sSuffix  = oXmlNode.GetAttribute( sName + _T("suffix") );
							m_nStart   = XmlUtils::GetInteger( oXmlNode.GetAttribute( sName + _T("start") ) );
							m_nEnd     = XmlUtils::GetInteger( oXmlNode.GetAttribute( sName + _T("end") ) );
							m_nDigits  = XmlUtils::GetInteger( oXmlNode.GetAttribute( sName + _T("digits") ) );
							m_bVisible = XmlUtils::GetInteger( oXmlNode.GetAttribute( sName + _T("visible"), _T("1") ) );

						}

						void AddToString( CString& str, int value ) const
						{
							if( !m_bVisible )
								return;

							str += m_sPrefix;

							TCHAR sBuffer[16];
							::_itot( value, sBuffer, 10 );

							if( m_nDigits > 0 )
							{
								int digits = m_nDigits > 15 ? 15 : m_nDigits;
								int index  = 0;
								
								while( sBuffer[index] )
								{
									++index;
								}
								while( index < digits )
								{
									str += _T("0");
									++index;
								}
								sBuffer[digits] = 0;
							}

							str += sBuffer;
							str += m_sSuffix;
						}
						void Clipping( const CString& token )
						{
							int nTokenLength = token.GetLength();
							if( nTokenLength > 0 )
							{
								CString sPath;

								int nTextLength = m_sPrefix.GetLength();
								if( nTextLength )
								{
									sPath = m_sPrefix.Left( nTokenLength );
									if( sPath == token )
									{
										m_sPrefix.Delete( 0, nTokenLength - 1 );
										m_sPrefix.SetAt( 0, 30 );
										nTextLength -= nTokenLength - 1;
									}
								
									sPath = m_sPrefix.Right( nTokenLength );
									if( sPath == token )
									{
										m_sPrefix.Delete( nTextLength - nTokenLength + 1, nTokenLength - 1 );
										m_sPrefix.SetAt( nTextLength - nTokenLength, 30 );
									}
								}
								
								nTextLength = m_sSuffix.GetLength();
								if( nTextLength )
								{
									sPath = m_sSuffix.Left( nTokenLength );
									if( sPath == token )
									{
										m_sSuffix.Delete( 0, nTokenLength - 1 );
										m_sSuffix.SetAt( 0, 30 );
										nTextLength -= nTokenLength - 1;
									}
								
									sPath = m_sSuffix.Right( nTokenLength );
									if( sPath == token )
									{
										m_sSuffix.Delete( nTextLength - nTokenLength + 1, nTokenLength - 1 );
										m_sSuffix.SetAt( nTextLength - nTokenLength, 30 );
									}
								}
							}
						}
					
					public:
						CString m_sPrefix;
						CString m_sSuffix;

						long m_nStart;  // начальное значение
						long m_nEnd;    // конечное  значение
						long m_nDigits; // количество видимых цифр

						BOOL m_bVisible;
					};

				
				protected:
								
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						m_oHours       .FromXmlNode( oXmlNode, _T("hours-") );
						m_oMinutes     .FromXmlNode( oXmlNode, _T("minutes-") );
						m_oSeconds     .FromXmlNode( oXmlNode, _T("seconds-") );
						m_oMilliseconds.FromXmlNode( oXmlNode, _T("milliseconds-") );
						m_oBrush       .FromXmlNode( oXmlNode );
						m_oEdge        .FromXmlNode( oXmlNode );
						m_oFont        .FromXmlNode( oXmlNode );
						m_oShadow      .FromXmlNode( oXmlNode );

						m_nMetric = Strings::ToInteger( oXmlNode.GetAttribute( _T("metric") ) );
						m_sToken  = oXmlNode.GetAttribute( _T("token") );
						m_nTimerType = Strings::ToInteger( oXmlNode.GetAttribute( _T("type") ) );
						m_bUnlimited = Strings::ToInteger( oXmlNode.GetAttribute( _T("unlimited") ) );

						m_oHours       .Clipping( m_sToken );
						m_oMinutes     .Clipping( m_sToken );
						m_oSeconds     .Clipping( m_sToken );
						m_oMilliseconds.Clipping( m_sToken );

						
						m_dLeft   = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("rect-left"), _T("0")));
						m_dTop    = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("rect-top"), _T("0")));
						m_dRight  = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("rect-right"), _T("100")));
						m_dBottom = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("rect-bottom"), _T("100")));
						
						m_nFillMode    = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("text-fillmode"), _T("0")));
						m_nTypeEffects = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("text-typeeffects"), _T("0")));

						m_nAnimationID[0] = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("animation-fadein"), _T("-1")));
						m_nAnimationID[1] = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("animation-state"), _T("-1")));
						m_nAnimationID[2] = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("animation-fadeout"), _T("-1")));
					}
					virtual void InternalClear()
					{
						m_oHours       .Clear();
						m_oMinutes     .Clear();
						m_oSeconds     .Clear();
						m_oMilliseconds.Clear();
						m_oBrush       .Clear();
						m_oEdge        .Clear();
						m_oFont        .Clear();
						m_oShadow      .Clear();
						
						m_nMetric = c_nMetricPixels;
						m_sToken  = _T("");
					}
				public:

					DrawTimer()
					{
						Initialize(c_nSerializePaintDrawTimer, c_strSerializePaintDrawTimer);

						Clear();
					}

				
				public:
					PartTime m_oHours;
					PartTime m_oMinutes;
					PartTime m_oSeconds;
					PartTime m_oMilliseconds;

					ImageStudio::Serialize::Paint::Structures::Brush  m_oBrush;
					ImageStudio::Serialize::Paint::Structures::Edge   m_oEdge;
					ImageStudio::Serialize::Paint::Structures::Font   m_oFont;
					ImageStudio::Serialize::Paint::Structures::Shadow m_oShadow;

					int     m_nAnimationID[3];
					int     m_nMetric;
					double  m_dLeft, m_dTop, m_dRight, m_dBottom;

					CString m_sToken;

					int m_nFillMode;
					int m_nTypeEffects;
					int m_nTimerType;
					int m_bUnlimited;
				};
				class DrawFilm : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 10 * sizeof(int) + 2 * sizeof(double);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong((int)m_nSpeed);
						pStream->WriteDouble(m_dWideFactor);
						pStream->WriteLong((int)m_bVertical);
						pStream->WriteLong((int)m_bDrawBorder);
						pStream->WriteLong(m_nBkColor);
						pStream->WriteLong(m_nFilmMidColor);
						pStream->WriteLong(m_nFilmBorderColor);
						pStream->WriteLong(m_nBlendcolor);
						pStream->WriteLong((int)m_bZoom);
						pStream->WriteLong(m_nAlpha);
						pStream->WriteLong((int)m_bMoveDivider);
						pStream->WriteDouble(m_dSpeedFactor);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						m_nSpeed           = pStream->ReadLong();
						m_dWideFactor      = pStream->ReadDouble();
						m_bVertical        = (BOOL)pStream->ReadLong();
						m_bDrawBorder      = (BOOL)pStream->ReadLong();
						m_nBkColor         = pStream->ReadLong();
					    m_nFilmMidColor    = pStream->ReadLong();
					    m_nFilmBorderColor = pStream->ReadLong();
					    m_nBlendcolor      = pStream->ReadLong();
						m_bZoom            = (BOOL)pStream->ReadLong();
						m_nAlpha           = pStream->ReadLong();
						m_bMoveDivider     = (BOOL)pStream->ReadLong();
						m_dSpeedFactor     = pStream->ReadDouble();
					}					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("speed"), m_nSpeed);
						oXmlWriter.WriteAttribute(_T("widefactor"), m_dWideFactor);
						oXmlWriter.WriteAttribute(_T("vertical"), (int)m_bVertical);
						oXmlWriter.WriteAttribute(_T("drawborder"), (int)m_bDrawBorder);
						oXmlWriter.WriteAttribute(_T("backcolor"), m_nBkColor);
						oXmlWriter.WriteAttribute(_T("filmsmidcolor"), m_nFilmMidColor);
						oXmlWriter.WriteAttribute(_T("filmsbordercolor"), m_nFilmBorderColor);
						oXmlWriter.WriteAttribute(_T("blendcolor"), m_nBlendcolor);
						oXmlWriter.WriteAttribute(_T("zoom"), (int)m_bZoom);
						oXmlWriter.WriteAttribute(_T("alpha"), m_nAlpha);
						oXmlWriter.WriteAttribute(_T("movedivider"), m_bMoveDivider);
						oXmlWriter.WriteAttribute(_T("speedfactor"), m_dSpeedFactor);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						m_nSpeed           = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("speed"), _T("0")));
						m_dWideFactor      = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("widefactor"), _T("24")));
						m_bVertical        = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("vertical"), _T("0")));
						m_bDrawBorder      = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("drawborder"), _T("1")));
						m_nBkColor         = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("backcolor"), _T("0")));        
					    m_nFilmMidColor    = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("filmsmidcolor"), _T("2105376")));   
					    m_nFilmBorderColor = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("filmsbordercolor"), _T("6316128")));
					    m_nBlendcolor      = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("blendcolor"), _T("8421504")));    
						m_bZoom            = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("zoom"), _T("0")));
						m_nAlpha           = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("alpha"), _T("100")));
						m_bMoveDivider     = (BOOL)Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("movedivider"), _T("0")));
						m_dSpeedFactor     = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("speedfactor"), _T("1")));
					}					
					virtual void InternalClear()
					{
						m_nSpeed = 0;
						m_dWideFactor = 24;
						m_bVertical = FALSE;
						m_bDrawBorder = TRUE;
					    m_nBkColor = 0;         
					    m_nFilmMidColor = 2105376;   
					    m_nFilmBorderColor = 6316128;
					    m_nBlendcolor = 8421504; 
						m_bZoom = FALSE;
						m_nAlpha = 100;
						m_bMoveDivider = FALSE;
						m_dSpeedFactor = 1;
					}


				public:

					DrawFilm()
					{
						Initialize(c_nSerializePaintDrawFilm, c_strSerializePaintDrawFilm);

						Clear();
					}


				public:

					int    m_nSpeed;		   //скорость прокуртки 
					double m_dWideFactor;	   //ширина края ленты от 0 до 100, стандартное значение 24
					BOOL   m_bVertical;		   //направление ленты: 0 - горизонтальное, 1 - вертикальное
					BOOL   m_bDrawBorder;      //прорисовка края ленты: 1 - рисуем, 0 - перфорации(дырки) рисуются прямо на картинке
					BOOL   m_bZoom;            //выполняется ли зуммирование
					int    m_nBkColor;         //цвет перфорации
					int    m_nFilmMidColor;    //цвет пленки в середине
					int    m_nFilmBorderColor; //цвет пленки скраю
					int    m_nBlendcolor;      //цвет линии отсвета на перфорации
					int    m_nAlpha;           //общая альфа
					BOOL   m_bMoveDivider;     //двигается или нет полоска разделитель
					double m_dSpeedFactor;     //коэффициент скорости


				};
				class DrawPuzzle : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 4 * sizeof(int) + sizeof(double);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(m_nRow);
						pStream->WriteLong(m_nCol);
						pStream->WriteDouble(m_dSideFactor);
						pStream->WriteLong(m_nType);
						pStream->WriteLong(m_nMetric);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						m_nRow        = pStream->ReadLong();
						m_nCol        = pStream->ReadLong();
						m_dSideFactor = pStream->ReadDouble();
						m_nType       = pStream->ReadLong();
						m_nMetric     = pStream->ReadLong();
					}					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("rows"), m_nRow);
						oXmlWriter.WriteAttribute(_T("columns"), m_nCol);
						oXmlWriter.WriteAttribute(_T("sidefactor"), m_dSideFactor);
						oXmlWriter.WriteAttribute(_T("type"), m_nType);
						oXmlWriter.WriteAttribute(_T("mertic"), m_nMetric);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						m_nRow        = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("rows"), _T("10")));
						m_nCol        = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("columns"), _T("10")));
						m_dSideFactor = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("sidefactor"), _T("1.0")));
						m_nType       = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("type"), _T("0")));
						m_nMetric     = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
					}					
					virtual void InternalClear()
					{
						m_nRow = 10;
						m_nCol = 10;
						m_dSideFactor = 1.0;
						m_nType = 0;
						m_nMetric = 0;
					}


				public:

					DrawPuzzle()
					{
						Initialize(c_nSerializePaintDrawPuzzle, c_strSerializePaintDrawPuzzle);

						Clear();
					}


				public:

					int    m_nRow;  // количество кусочков по вертикали
					int    m_nCol;  // количество кусочков по горизонтали		

					double m_dSideFactor;

					int    m_nType; 

					// 0 - обычная плитка(таблица)
					// 1 - пазл
					// 2 - шестиугольники
					// 3 - ромбы
					// 4 - кирпичи
					// 5 - треугольники
					// 6 - наклонные кирпичи(45 градусов угол наклона)

					int    m_nMetric; // 0, 1, 2

				};
				class DrawFog : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 2 * sizeof(int);
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Color);
						pStream->WriteLong(Alpha);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Color = pStream->ReadLong();
						Alpha = pStream->ReadLong();
					}					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("color"), Color);
						oXmlWriter.WriteAttribute(_T("alpha"), Alpha);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Color = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("color"), _T("16777215")));
						Alpha = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("alpha"), _T("255")));
					}					
					virtual void InternalClear()
					{
						Color = 16777215;
						Alpha = 255;
					}

				
				public:
					DrawFog()
					{
						Initialize(c_nSerializeEffectFog, c_strSerializeEffectFog);

						Clear();
					}

				
				public:
					int Color;
					int Alpha;
				};
				class DrawGraphicPath : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 0;
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						return;
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						return;
					}					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						return;
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						RELEASEINTERFACE(m_pPainter);
						CoCreateInstance(AVSGraphics::CLSID_CAVSVMLPainter, NULL, CLSCTX_ALL, AVSGraphics::IID_IAVSVMLPainter, (void**)&m_pPainter);

						if (NULL != m_pPainter)
						{
							BSTR bsXml = oXmlNode.GetXml().AllocSysString();
							m_pPainter->OpenXml(bsXml);
							RELEASESYSSTRING(bsXml);
						}
					}					
					virtual void InternalClear()
					{
						RELEASEINTERFACE(m_pPainter);
					}

				public:

					DrawGraphicPath()
					{
						Initialize(c_nSerializePaintDrawPath, c_strSerializePaintDrawPath);

						m_pPainter = NULL;
						Clear();
					}

					BOOL Draw(IUnknown* punkSource)
					{
						if (NULL != m_pPainter)
						{
							m_pPainter->DrawOnMediaData(punkSource, 1.0);
							return TRUE;
						}
						return FALSE;
					}

				private:
					AVSGraphics::IAVSVMLPainter* m_pPainter;
				};

				class DrawAutoShape : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 0;
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						return;
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						return;
					}					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						return;
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						//CString str = oXmlNode.GetXml();

						//BSTR bsXml = str.AllocSysString();
						//m_pShape->SetXml(bsXml);

						//SysFreeString(bsXml);
					}					
					virtual void InternalClear()
					{
						//m_pShape = NULL;

						//::CoCreateInstance( __uuidof(COOXMLShape), NULL, CLSCTX_INPROC, __uuidof(IOOXMLShape), (void**)&m_pShape );
					}

				public:

					DrawAutoShape()
					{
						Initialize(c_nSerializePaintDrawAutoShape, c_strSerializePaintDrawAutoShape);

						Clear();
					}
					~DrawAutoShape()
					{
						//RELEASEINTERFACE(m_pShape);
					}

					BOOL Draw(IUnknown* pRenderer, double dFrame)
					{
						return TRUE;
						
						//if (NULL == m_pShape || NULL == pRenderer)
						//	return;
						
						// вставить, когда все будет работать через AVSGraphics
						
						//BSTR bsXml = NULL;
						//HRESULT hr = m_pShape->GetXml2(lWidth, lHeight, &bsXml);

						//if (S_OK == hr)
						//{
						//	CString strXml = (CString)bsXml;
						//	SysFreeString(bsXml);

						//	strXml = _T("<node>") + strXml + _T("</node>");

						//	XmlUtils::CXmlNode oNode;

						//	if (oNode.FromXmlString(strXml))
						//	{						
						//		XmlUtils::CXmlNode oNodePath;

						//		if (oNode.GetNode(_T("ImagePaint-DrawGraphicPath"), oNodePath))
						//		{
						//			CDrawGraphicPath oPath;
						//			oPath.InternalFromXmlNode(oNodePath);

						//			oPath.Draw(pRenderer);
						//		}
						//	}
						//}
					}

				private:
					//IOOXMLShape* m_pShape;
				};

				class DrawTextEx : public Core::Effect
				{
				public:
					virtual int InternalGetRequiredStreamSize()
					{
						return 0;
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
					}					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						return;
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						RELEASEINTERFACE(m_pPainter);
						CoCreateInstance(AVSGraphics::CLSID_CAVSTextPainter, NULL, CLSCTX_ALL, AVSGraphics::IID_IAVSTextPainter, (void**)&m_pPainter);

						if (NULL != m_pPainter)
						{
							BSTR bsXml = oXmlNode.GetXml().AllocSysString();
							m_pPainter->OpenXml(bsXml);
							RELEASESYSSTRING(bsXml);
						}
					}

				public:
					DrawTextEx()
					{
						Initialize(c_nSerializePaintDrawTextEx, c_strSerializePaintDrawTextEx);

						m_pPainter = NULL;
						Clear();
					}
					BOOL Draw(IUnknown* punkSource, double dCompleteness)
					{
						if (NULL != m_pPainter)
						{
							m_pPainter->DrawOnMediaData(punkSource, dCompleteness);
							return TRUE;
						}
						return FALSE;
					}

				private:
					AVSGraphics::IAVSTextPainter* m_pPainter;
				};

				class DrawCountdown : public Core::Effect
				{
				protected:
								
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						m_oBrush       .FromXmlNode( oXmlNode );
						m_oEdge        .FromXmlNode( oXmlNode );
						m_oFont        .FromXmlNode( oXmlNode );
						m_oShadow      .FromXmlNode( oXmlNode );

						m_nMetric = Strings::ToInteger( oXmlNode.GetAttribute( _T("metric"), _T("0") ) );
						m_nStartNumber = Strings::ToInteger( oXmlNode.GetAttribute( _T("startnumber"), _T("10") ) );
						m_nFinishNumber = Strings::ToInteger( oXmlNode.GetAttribute( _T("finishnumber"), _T("0") ) );

						m_dLeft   = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("rect-left"), _T("0")));
						m_dTop    = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("rect-top"), _T("0")));
						m_dRight  = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("rect-right"), _T("100")));
						m_dBottom = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("rect-bottom"), _T("100")));
						
						m_nFillMode    = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("text-fillmode"), _T("0")));
						m_nTypeEffects = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("text-typeeffects"), _T("0")));

						m_nAnimationID[0] = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("animation-fadein"), _T("-1")));
						m_nAnimationID[1] = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("animation-state"), _T("-1")));
						m_nAnimationID[2] = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("animation-fadeout"), _T("-1")));
					}
					virtual void InternalClear()
					{
						m_oBrush       .Clear();
						m_oEdge        .Clear();
						m_oFont        .Clear();
						m_oShadow      .Clear();
						
						m_nMetric = c_nMetricPixels;

						m_nFillMode = 0;
						m_nTypeEffects = 0;
						m_nStartNumber = 10;
						m_nFinishNumber = 0;
					}
				public:

					DrawCountdown()
					{
						Initialize(c_nSerializePaintDrawCountdown, c_strSerializePaintDrawCountdown);

						Clear();
					}

				
				public:
					ImageStudio::Serialize::Paint::Structures::Brush  m_oBrush;
					ImageStudio::Serialize::Paint::Structures::Edge   m_oEdge;
					ImageStudio::Serialize::Paint::Structures::Font   m_oFont;
					ImageStudio::Serialize::Paint::Structures::Shadow m_oShadow;

					int     m_nAnimationID[3];
					int     m_nMetric;
					double  m_dLeft, m_dTop, m_dRight, m_dBottom;

					int m_nFillMode;
					int m_nTypeEffects;

					int m_nStartNumber;
					int m_nFinishNumber;
				};
			}
			namespace TextStatic
			{
				class SetBrush : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return Brush.GetSize();
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						Brush.ToStream(pStream);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Brush.FromStream(pStream);
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						Brush.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Brush.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Brush.Clear();
					}
					
				public:
					
					SetBrush()
					{
						Initialize(c_nSerializePaintTextStaticSetBrush, c_strSerializePaintTextStaticSetBrush);

						Clear();
					}
						
				public:
					
					Painter::CBrush Brush;

					/* @desc:
					ImageStudio-Serialize-Paint-TextStatic-SetBrush (c_nSerializePaintTextStaticSetBrush, c_strSerializePaintTextStaticSetBrush)
					{
						Painter::CBrush Brush;
					}
					*/
				};
				class SetFont : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return Font.GetSize();
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						Font.ToStream(pStream);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Font.FromStream(pStream);
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						Font.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Font.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Font.Clear();
					}
					
				public:
					
					SetFont()
					{
						Initialize(c_nSerializePaintTextStaticSetFont, c_strSerializePaintTextStaticSetFont);

						Clear();
					}
						
				public:
					
					Painter::CFont Font;

					/* @desc:
					ImageStudio-Serialize-Paint-TextStatic-SetFont (c_nSerializePaintTextStaticSetFont, c_strSerializePaintTextStaticSetFont)
					{
						Painter::CFont Font;
					}
					*/
				};
				class SetFormat : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return sizeof(long) + Format.GetSize();
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						Format.ToStream(pStream);
						pStream->WriteLong(Angle);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Format.FromStream(pStream);
						Angle = pStream->ReadLong();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						Format.ToXmlAttributes(oXmlWriter);
						oXmlWriter.WriteAttribute(_T("angle"), Angle);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Format.FromXmlNode(oXmlNode);
						Angle = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));
					}
					
					virtual void InternalClear()
					{
						Format.Clear();

						Angle = 0;
					}
					
				public:
					
					SetFormat()
					{
						Initialize(c_nSerializePaintTextStaticSetFormat, c_strSerializePaintTextStaticSetFormat);

						Clear();
					}
						
				public:
					
					Painter::CFormat Format;
					int Angle;

					/* @desc:
					ImageStudio-Serialize-Paint-TextStatic-SetFormat (c_nSerializePaintTextStaticSetFormat, c_strSerializePaintTextStaticSetFormat)
					{
						Painter::CFormat Format;
						
						int Angle = 0;				// xml имя - angle; угол вращения текста (измеряется в градусах: -360..360)
					}
					*/
				};
				class SetShadow : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return Shadow.GetSize();
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						Shadow.ToStream(pStream);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Shadow.FromStream(pStream);
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						Shadow.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Shadow.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Shadow.Clear();
					}
					
				public:
					
					SetShadow()
					{
						Initialize(c_nSerializePaintTextStaticSetShadow, c_strSerializePaintTextStaticSetShadow);

						Clear();
					}
						
				public:
					
					Painter::CShadow Shadow;

					/* @desc:
					ImageStudio-Serialize-Paint-TextStatic-SetShadow (c_nSerializePaintTextStaticSetShadow, c_strSerializePaintTextStaticSetShadow)
					{
						Painter::CShadow Shadow;
					}
					*/
				};
				class SetEdge : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return Edge.GetSize();
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						Edge.ToStream(pStream);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Edge.FromStream(pStream);
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						Edge.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Edge.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Edge.Clear();
					}
					
				public:
					
					SetEdge()
					{
						Initialize(c_nSerializePaintTextStaticSetEdge, c_strSerializePaintTextStaticSetEdge);

						Clear();
					}
						
				public:
					
					Painter::CEdgeText Edge;

					/* @desc:
					ImageStudio-Serialize-Paint-TextStatic-SetEdge (c_nSerializePaintTextStaticSetEdge, c_strSerializePaintTextStaticSetEdge)
					{
						Painter::CEdgeText Edge;
					}
					*/
				};
				
				class DrawInRect : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 4*sizeof(double) + sizeof(int) + Text.GetLength() + 1;
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(Left);
						pStream->WriteDouble(Top);
						pStream->WriteDouble(Right);
						pStream->WriteDouble(Bottom);
						pStream->WriteString(Text);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Left = pStream->ReadDouble();
						Top = pStream->ReadDouble();
						Right = pStream->ReadDouble();
						Bottom = pStream->ReadDouble();
						Text = pStream->ReadString();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("left"), Left);
						oXmlWriter.WriteAttribute(_T("top"), Top);
						oXmlWriter.WriteAttribute(_T("right"), Right);
						oXmlWriter.WriteAttribute(_T("bottom"), Bottom);
						oXmlWriter.WriteAttribute(_T("text"), Text);

						Brush.ToXmlAttributes(oXmlWriter);
						Font.ToXmlAttributes(oXmlWriter);
						Shadow.ToXmlAttributes(oXmlWriter);
						Edge.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						Left = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("left"), _T("0")));
						Top = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("top"), _T("0")));
						Right = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("right"), _T("0")));
						Bottom = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("bottom"), _T("0")));
						Text = oXmlNode.GetAttributeOrValue(_T("text"), _T(""));

						Brush.FromXmlNode(oXmlNode);
						Font.FromXmlNode(oXmlNode);
						Shadow.FromXmlNode(oXmlNode);
						Edge.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Left = 0;
						Top = 0;
						Right = 0;
						Bottom = 0;
						Text = _T("");

						Brush.Clear();
						Font.Clear();
						Shadow.Clear();
						Edge.Clear();
					}
					
				public:
					
					DrawInRect()
					{
						Initialize(c_nSerializePaintTextStaticDrawInRect, c_strSerializePaintTextStaticDrawInRect);

						Clear();
					}
						
				public:
					
					int Metric;
					double Left, Top, Right, Bottom;
					CString Text;

					ImageStudio::Serialize::Paint::Structures::Brush Brush;
					ImageStudio::Serialize::Paint::Structures::Font Font;
					ImageStudio::Serialize::Paint::Structures::Shadow Shadow;
					ImageStudio::Serialize::Paint::Structures::Edge Edge;

					/* @desc:
					ImageStudio-Serialize-Paint-TextStatic-DrawInRect (c_nSerializePaintTextStaticDrawInRect, c_strSerializePaintTextStaticDrawInRect)
					{
						int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...
						CString Text = "";				// xml имя - text; текст для отрисовки

						// прямоугольник, описанный около текста (в пикселях картинки или в другой какой-то метрике)
						double Left = 0;				// xml имя - left
						double Top = 0;					// xml имя - top
						double Right = 0;				// xml имя - right
						double Bottom = 0;				// xml имя - bottom

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
						ImageStudio::Serialize::Paint::Structures::Font Font;		// xml имя - font-...
						ImageStudio::Serialize::Paint::Structures::Shadow Shadow;	// xml имя - shadow-...
						ImageStudio::Serialize::Paint::Structures::Edge Edge;		// xml имя - edge-...
					}
					*/
				};
				class DrawOnEllipse : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 6*sizeof(double) + sizeof(int) + Text.GetLength() + 1;
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteDouble(Left);
						pStream->WriteDouble(Top);
						pStream->WriteDouble(Right);
						pStream->WriteDouble(Bottom);
						pStream->WriteDouble(StartAngle);
						pStream->WriteDouble(SweepAngle);
						pStream->WriteString(Text);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Left = pStream->ReadDouble();
						Top = pStream->ReadDouble();
						Right = pStream->ReadDouble();
						Bottom = pStream->ReadDouble();
						StartAngle = pStream->ReadDouble();
						SweepAngle = pStream->ReadDouble();
						Text = pStream->ReadString();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("left"), Left);
						oXmlWriter.WriteAttribute(_T("top"), Top);
						oXmlWriter.WriteAttribute(_T("right"), Right);
						oXmlWriter.WriteAttribute(_T("bottom"), Bottom);
						oXmlWriter.WriteAttribute(_T("startangle"), StartAngle);
						oXmlWriter.WriteAttribute(_T("sweepangle"), SweepAngle);
						oXmlWriter.WriteAttribute(_T("text"), Text);

						Brush.ToXmlAttributes(oXmlWriter);
						Font.ToXmlAttributes(oXmlWriter);
						Shadow.ToXmlAttributes(oXmlWriter);
						Edge.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						Left = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("left"), _T("0")));
						Top = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("top"), _T("0")));
						Right = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("right"), _T("0")));
						Bottom = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("bottom"), _T("0")));
						StartAngle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("startangle"), _T("0")));
						SweepAngle = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("sweepangle"), _T("0")));
						Text = oXmlNode.GetAttributeOrValue(_T("text"), _T(""));

						Brush.FromXmlNode(oXmlNode);
						Font.FromXmlNode(oXmlNode);
						Shadow.FromXmlNode(oXmlNode);
						Edge.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Left = 0;
						Top = 0;
						Right = 0;
						Bottom = 0;
						StartAngle = 0;
						SweepAngle = 0;
						Text = _T("");

						Brush.Clear();
						Font.Clear();
						Shadow.Clear();
						Edge.Clear();
					}
					
				public:
					
					DrawOnEllipse()
					{
						Initialize(c_nSerializePaintTextStaticDrawOnEllipse, c_strSerializePaintTextStaticDrawOnEllipse);

						Clear();
					}
						
				public:
					
					int Metric;
					double Left, Top, Right, Bottom;
					double StartAngle, SweepAngle;
					CString Text;

					ImageStudio::Serialize::Paint::Structures::Brush Brush;
					ImageStudio::Serialize::Paint::Structures::Font Font;
					ImageStudio::Serialize::Paint::Structures::Shadow Shadow;
					ImageStudio::Serialize::Paint::Structures::Edge Edge;

					/* @desc:
					ImageStudio-Serialize-Paint-TextStatic-DrawOnEllipse (c_nSerializePaintTextStaticDrawOnEllipse, c_strSerializePaintTextStaticDrawOnEllipse)
					{
						int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...
						CString Text = "";				// xml имя - text; текст для отрисовки

						// прямоугольник, описанный около эллипса (в пикселях картинки или в другой какой-то метрике)
						double Left = 0;				// xml имя - left
						double Top = 0;					// xml имя - top
						double Right = 0;				// xml имя - right
						double Bottom = 0;				// xml имя - bottom

						// углы, определяющие, какой сектор вырезать из эллипса (в градусах, -360..360)
						double StartAngle = 0;			// xml имя - startangle
						double SweepAngle = 0;			// xml имя - sweepangle

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
						ImageStudio::Serialize::Paint::Structures::Font Font;		// xml имя - font-...
						ImageStudio::Serialize::Paint::Structures::Shadow Shadow;	// xml имя - shadow-...
						ImageStudio::Serialize::Paint::Structures::Edge Edge;		// xml имя - edge-...
					}
					*/
				};
				class DrawOnPolyline : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 2*sizeof(int) + Points.GetSize()*sizeof(double) + Text.GetLength() + 1;
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);

						pStream->WriteLong(Points.GetSize());
						for (int index = 0; index < Points.GetSize(); ++index)
							pStream->WriteDouble(Points[index]);
						pStream->WriteString(Text);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Points.RemoveAll();

						Metric = pStream->ReadLong();

						int nCount = pStream->ReadLong();

						for (int index = 0; index < nCount; ++index)
							Points.Add(pStream->ReadDouble());

						Text = pStream->ReadString();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						_bstr_t strPointsData = Strings::FromBinary((BYTE*)Points.GetData(), Points.GetSize()*sizeof(double));


						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("points-data"), (TCHAR*)strPointsData);
						oXmlWriter.WriteAttribute(_T("text"), Text);

						Brush.ToXmlAttributes(oXmlWriter);
						Font.ToXmlAttributes(oXmlWriter);
						Shadow.ToXmlAttributes(oXmlWriter);
						Edge.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Points.RemoveAll();

						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));

						Text = oXmlNode.GetAttributeOrValue(_T("text"), _T(""));

						// try to load points from string like " 1 2 3 4 5 6" (string contains x,y pairs)
						CString strPoints = oXmlNode.GetAttributeOrValue(_T("points"), _T(""));
						if (strPoints.GetLength() > 0)
						{
							Templates::CArray<CString> arrSubstrings;
							int nCoordinates = Strings::Split(strPoints, _T(" "), arrSubstrings);
							
							if ((nCoordinates % 2) == 0)
							{
								for (int index = 0; index < arrSubstrings.GetCount(); ++index)
									Points.Add(Strings::ToDouble(arrSubstrings[index]));
							}
						}

						// try to load points from base64 encoded string

						_bstr_t strPointsData = oXmlNode.GetAttributeOrValue(_T("points-data"), _T(""));
						if (strPointsData.length() > 0)

						{
							BYTE* pData = 0;
							int nSizeAllocated = 0;
							int nSizeArray = 0;

							if (Strings::ToBinary(CString((char*)strPointsData), pData, nSizeAllocated, nSizeArray))
							{
								double* pPointsData = (double*)pData;
								int nPointsCount = nSizeArray/sizeof(double);

								if ((nSizeArray % sizeof(double)) == 0)
								{
									for (int index = 0; index < nPointsCount; ++index, ++pPointsData)
										Points.Add(*pPointsData);
								}

								delete[] pData;
							}
						}

						Brush.FromXmlNode(oXmlNode);
						Font.FromXmlNode(oXmlNode);
						Shadow.FromXmlNode(oXmlNode);
						Edge.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Points.RemoveAll();

						Text = _T("");

						Brush.Clear();
						Font.Clear();
						Shadow.Clear();
						Edge.Clear();
					}
					
				public:
				
					DrawOnPolyline()
					{
						Initialize(c_nSerializePaintTextStaticDrawOnPolyline, c_strSerializePaintTextStaticDrawOnPolyline);

						Clear();
					}
						
				public:
					
					int Metric;
					CSimpleArray<double> Points;
					CString Text;

					ImageStudio::Serialize::Paint::Structures::Brush Brush;
					ImageStudio::Serialize::Paint::Structures::Font Font;
					ImageStudio::Serialize::Paint::Structures::Shadow Shadow;
					ImageStudio::Serialize::Paint::Structures::Edge Edge;

					/* @desc:
					ImageStudio-Serialize-Paint-TextStatic-DrawOnPolyline (c_nSerializePaintTextStaticDrawOnPolyline, c_strSerializePaintTextStaticDrawOnPolyline)
					{
						int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...
						CString Text = "";				// xml имя - text; текст для отрисовки

						// координаты точек polyline в пикселях картинки или в другой какой-то метрике (пары x-y) по которому будет отрисован текст
						CSimpleArray<double> Points;	// xml имя - points-data; пример - 0 0 1 1 2 2 3 3

						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
						ImageStudio::Serialize::Paint::Structures::Font Font;		// xml имя - font-...
						ImageStudio::Serialize::Paint::Structures::Shadow Shadow;	// xml имя - shadow-...
						ImageStudio::Serialize::Paint::Structures::Edge Edge;		// xml имя - edge-...
					}
					*/
				};
			}
			namespace TextDynamic
			{
				class SetBrush : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return Brush.GetSize();
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						Brush.ToStream(pStream);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Brush.FromStream(pStream);
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						Brush.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Brush.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Brush.Clear();
					}
					
				public:
					
					SetBrush()
					{
						Initialize(c_nSerializePaintTextDynamicSetBrush, c_strSerializePaintTextDynamicSetBrush);

						Clear();
					}
						
				public:
					
					Painter::CBrush Brush;

					/* @desc:
					ImageStudio-Serialize-Paint-TextDynamic-SetBrush (c_nSerializePaintTextDynamicSetBrush, c_strSerializePaintTextDynamicSetBrush)
					{
						Painter::CBrush Brush;
					}
					*/
				};
				class SetFont : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return Font.GetSize();
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						Font.ToStream(pStream);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Font.FromStream(pStream);
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						Font.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Font.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Font.Clear();
					}
					
				public:
					
					SetFont()
					{
						Initialize(c_nSerializePaintTextDynamicSetFont, c_strSerializePaintTextDynamicSetFont);

						Clear();
					}
						
				public:
					
					Painter::CFont Font;
					
					/* @desc:
					ImageStudio-Serialize-Paint-TextDynamic-SetFont (c_nSerializePaintTextDynamicSetFont, c_strSerializePaintTextDynamicSetFont)
					{
						Painter::CFont Font;
					}
					*/
				};
				class SetShadow : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return Shadow.GetSize();
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						Shadow.ToStream(pStream);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Shadow.FromStream(pStream);
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						Shadow.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Shadow.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Shadow.Clear();
					}
					
				public:
					
					SetShadow()
					{
						Initialize(c_nSerializePaintTextDynamicSetShadow, c_strSerializePaintTextDynamicSetShadow);

						Clear();
					}
						
				public:
					
					Painter::CShadow Shadow;

					/* @desc:
					ImageStudio-Serialize-Paint-TextDynamic-SetShadow (c_nSerializePaintTextDynamicSetShadow, c_strSerializePaintTextDynamicSetShadow)
					{
						Painter::CShadow Shadow;
					}
					*/
				};
				class SetEdge : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return Edge.GetSize();
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						Edge.ToStream(pStream);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Edge.FromStream(pStream);
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						Edge.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Edge.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Edge.Clear();
					}
					
				public:
					
					SetEdge()
					{
						Initialize(c_nSerializePaintTextDynamicSetEdge, c_strSerializePaintTextDynamicSetEdge);

						Clear();
					}
						
				public:
					
					Painter::CEdgeText Edge;

					/* @desc:
					ImageStudio-Serialize-Paint-TextDynamic-SetEdge (c_nSerializePaintTextDynamicSetEdge, c_strSerializePaintTextDynamicSetEdge)
					{
						Painter::CEdgeText Edge;
					}
					*/
				};
				
				class Start : public Core::Effect
				{
				protected:
								
					virtual int InternalGetRequiredStreamSize()
					{
						return 8*sizeof(int) + Text.GetLength() + 1;
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
						pStream->WriteLong(Metric);
						pStream->WriteString(Text);
						pStream->WriteLong(Category);
						pStream->WriteLong(Effect);
						pStream->WriteLong(EffectID);
						pStream->WriteLong(X);
						pStream->WriteLong(Y);
						pStream->WriteLong(Step);
						pStream->WriteLong(Angle);
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
						Metric = pStream->ReadLong();
						Text = pStream->ReadString();
						Category = pStream->ReadLong();
						Effect = pStream->ReadLong();
						EffectID = pStream->ReadLong();
						X = pStream->ReadLong();
						Y = pStream->ReadLong();
						Step = pStream->ReadLong();
						Angle = pStream->ReadLong();
					}
					
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), Metric);
						oXmlWriter.WriteAttribute(_T("text"), Text);
						oXmlWriter.WriteAttribute(_T("category"), Category);
						oXmlWriter.WriteAttribute(_T("effect"), Effect);
						oXmlWriter.WriteAttribute(_T("effectid"), EffectID);
						oXmlWriter.WriteAttribute(_T("x"), X);
						oXmlWriter.WriteAttribute(_T("y"), Y);
						oXmlWriter.WriteAttribute(_T("step"), Step);
						oXmlWriter.WriteAttribute(_T("angle"), Angle);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Metric = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("metric"), _T("0")));
						Text = oXmlNode.GetAttributeOrValue(_T("text"), _T(""));
						Category = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("category"), _T("-1")));
						Effect = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("effect"), _T("-1")));
						EffectID = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("effectid"), _T("-1")));
						X = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("x"), _T("0")));
						Y = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("y"), _T("0")));
						Step = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("step"), _T("1")));
						Angle = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("angle"), _T("0")));
					}
					
					virtual void InternalClear()
					{
						Metric = c_nMetricPixels;

						Text = _T("");
						Category = -1;
						Effect = -1;
						EffectID = -1;
						X = Y = 0;
						Step = 1;
						Angle = 0;
					}
					
				public:
					
					Start()
					{
						Initialize(c_nSerializePaintTextDynamicStart, c_strSerializePaintTextDynamicStart);

						Clear();
					}
						
				public:
					
					int Metric;
					CString Text;
					int Category, Effect, EffectID;
					int X, Y;
					int Step;
					int Angle;
					
					/* @desc:
					ImageStudio-Serialize-Paint-TextDynamic-Start (c_nSerializePaintTextDynamicStart, c_strSerializePaintTextDynamicStart)
					{
						int Metric = c_nMetricPixels;	// xml имя - metriс; тип метрики, см. c_nMetric...
						CString Text = "";				// xml имя - text; текст для отрисовки

						// параметры, описывающие, какой эффект применять (либо EffectID, либо Category-Effect)
						int Category = -1;				// xml имя - category; порядковый индекс категории
						int Effect = -1;				// xml имя - effect; порядковый индекс эффекта
						int EffectID = -1;				// xml имя - effectid; уникальный идентификатор эффекта

						// координаты левой-верхней точки, куда должен приехать текст (в пикселях картинки или в другой какой-то метрике)
						int X = 0;						// xml имя - x;
						int Y = 0;						// xml имя - y;
						int Step = 1;					// xml имя - step; сдвиг одной буквы, относительно предыдущей (для конечной позиции текста). измеряется в пикселях картинки или в другой какой-то метрике 
						int Angle = 0;					// xml имя - angle; угол поворота текста в градусах
					}
					*/
				};
				class Stop : public Core::Effect
				{
				public:
					
					Stop()
					{
						Initialize(c_nSerializePaintTextDynamicStop, c_strSerializePaintTextDynamicStop);

						Clear();
					}

					/* @desc:
					ImageStudio-Serialize-Paint-TextDynamic-Stop (c_nSerializePaintTextDynamicStop, c_strSerializePaintTextDynamicStop)
					{
					}
					*/
				};
				class Render : public Core::Effect
				{
				protected:
								
					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						Brush.ToXmlAttributes(oXmlWriter);
						Font.ToXmlAttributes(oXmlWriter);
						Shadow.ToXmlAttributes(oXmlWriter);
						Edge.ToXmlAttributes(oXmlWriter);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						Brush.FromXmlNode(oXmlNode);
						Font.FromXmlNode(oXmlNode);
						Shadow.FromXmlNode(oXmlNode);
						Edge.FromXmlNode(oXmlNode);
					}
					
					virtual void InternalClear()
					{
						Brush.Clear();
						Font.Clear();
						Shadow.Clear();
						Edge.Clear();
					}
					
				public:

					Render()
					{
						Initialize(c_nSerializePaintTextDynamicRender, c_strSerializePaintTextDynamicRender);

						Clear();
					}
					
				public:
					
					ImageStudio::Serialize::Paint::Structures::Brush Brush;
					ImageStudio::Serialize::Paint::Structures::Font Font;
					ImageStudio::Serialize::Paint::Structures::Shadow Shadow;
					ImageStudio::Serialize::Paint::Structures::Edge Edge;

					/* @desc:
					ImageStudio-Serialize-Paint-TextDynamic-Render (c_nSerializePaintTextDynamicRender, c_strSerializePaintTextDynamicRender)
					{
						// опциональные значения для настроек отрисовки (опциональные только в xml!)
						ImageStudio::Serialize::Paint::Structures::Brush Brush;		// xml имя - brush-...
						ImageStudio::Serialize::Paint::Structures::Font Font;		// xml имя - font-...
						ImageStudio::Serialize::Paint::Structures::Shadow Shadow;	// xml имя - shadow-...
						ImageStudio::Serialize::Paint::Structures::Edge Edge;		// xml имя - edge-...
					}
					*/
				};
			}

			namespace Text
			{
				static const int c_nAnimatedTextLine = 0;
				static const int c_nAnimatedTextEllipse = 1;
				static const int c_nAnimatedTextPolyline = 2;
				static const int c_nAnimatedTextRectangle = 3;
				
				class AnimatedTextEllipse
				{
				public:
								
					void ToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("ellipse-startangle"), m_dAngleStart);
					}
					void FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						m_dAngleStart = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("ellipse-startangle"), _T("0")));
					}
					void Clear()
					{
						m_dAngleStart = 0;
					}
					
					double GetAngleStart() const
					{
						return m_dAngleStart;
					}
					
				protected:
					
					double m_dAngleStart;
				};
				class AnimatedTextPolyline
				{
				public:

					void ToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						_bstr_t strPointsData = Strings::FromBinary((BYTE*)m_arrPoints.GetData(), m_arrPoints.GetSize()*sizeof(double));


						oXmlWriter.WriteAttribute(_T("polyline-points-data"), (TCHAR*)strPointsData);

					}
					void FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						m_arrPoints.RemoveAll();

						// try to load points from string like " 1 2 3 4 5 6" (string contains x,y pairs)
						CString strPoints = oXmlNode.GetAttributeOrValue(_T("polyline-points"), _T(""));
						if (strPoints.GetLength() > 0)
						{
							Templates::CArray<CString> arrSubstrings;
							int nCoordinates = Strings::Split(strPoints, _T(" "), arrSubstrings);

							if ((nCoordinates % 2) == 0)
							{
								for (int index = 0; index < arrSubstrings.GetCount(); ++index)
									m_arrPoints.Add(Strings::ToDouble(arrSubstrings[index]));
							}
						}

						// try to load points from base64 encoded string

						_bstr_t strPointsData = oXmlNode.GetAttributeOrValue(_T("polyline-points-data"), _T(""));
						if (strPointsData.length() > 0)

						{
							BYTE* pData = 0;
							int nSizeAllocated = 0;
							int nSizeArray = 0;

							if (Strings::ToBinary(CString((char*)strPointsData), pData, nSizeAllocated, nSizeArray))
							{
								double* pPointsData = (double*)pData;
								int nPointsCount = nSizeArray/sizeof(double);

								if ((nSizeArray % sizeof(double)) == 0)
								{
									for (int index = 0; index < nPointsCount; ++index, ++pPointsData)
										m_arrPoints.Add(*pPointsData);
								}

								delete[] pData;
							}
						}
					}
					void Clear()
					{
						m_arrPoints.RemoveAll();
					}

					CSimpleArray<double>& GetPoints()
					{
						return m_arrPoints;
					}

				protected:

					CSimpleArray<double> m_arrPoints;
				};

				class AnimatedText : public Core::Effect
				{
				protected:

					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
						oXmlWriter.WriteAttribute(_T("metric"), m_nMetric);
						oXmlWriter.WriteAttribute(_T("rect-left"), m_dLeft);
						oXmlWriter.WriteAttribute(_T("rect-top"), m_dTop);
						oXmlWriter.WriteAttribute(_T("rect-right"), m_dRight);
						oXmlWriter.WriteAttribute(_T("rect-bottom"), m_dBottom);
						oXmlWriter.WriteAttribute(_T("rect-clipping"), m_bTextClipped);

						oXmlWriter.WriteAttribute(_T("text"), m_strText);
						oXmlWriter.WriteAttribute(_T("token"), m_strToken);
						oXmlWriter.WriteAttribute(_T("animation-fadein"), m_nAnimationID[0]);
						oXmlWriter.WriteAttribute(_T("animation-state"), m_nAnimationID[1]);
						oXmlWriter.WriteAttribute(_T("animation-fadeout"), m_nAnimationID[2]);
						oXmlWriter.WriteAttribute(_T("text-fillmode"), m_nFillMode);
						oXmlWriter.WriteAttribute(_T("text-typeeffects"), m_nTypeEffects);

						if (m_nAnimationID[1] == c_nAnimatedTextEllipse)
							m_oSettingsEllipse.ToXmlAttributes(oXmlWriter);
						else if (m_nAnimationID[1] == c_nAnimatedTextPolyline)
							m_oSettingsPolyline.ToXmlAttributes(oXmlWriter);

						m_oBrush.ToXmlAttributes(oXmlWriter);
						m_oFont.ToXmlAttributes(oXmlWriter);
						m_oShadow.ToXmlAttributes(oXmlWriter);
						m_oEdge.ToXmlAttributes(oXmlWriter);

						if ( m_bAlign )
							oXmlWriter.WriteAttribute(_T("aligntype"), m_nAlignType);
						oXmlWriter.WriteAttribute(_T("marginhorizontal"), m_nMarginX);
						oXmlWriter.WriteAttribute(_T("marginvertical"), m_nMarginY);
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						m_nMetric = Strings::ToInteger(oXmlNode.GetAttribute(_T("metric"), _T("0")));
						m_dLeft   = Strings::ToDouble(oXmlNode.GetAttribute(_T("rect-left"), _T("0")));
						m_dTop    = Strings::ToDouble(oXmlNode.GetAttribute(_T("rect-top"), _T("0")));
						m_dRight  = Strings::ToDouble(oXmlNode.GetAttribute(_T("rect-right"), _T("100")));
						m_dBottom = Strings::ToDouble(oXmlNode.GetAttribute(_T("rect-bottom"), _T("100")));

						m_bTextClipped = Strings::ToInteger(oXmlNode.GetAttribute(_T("rect-clipping"), _T("0")));
						m_nFillMode    = Strings::ToInteger(oXmlNode.GetAttribute(_T("text-fillmode"), _T("0")));
						m_nTypeEffects = Strings::ToInteger(oXmlNode.GetAttribute(_T("text-typeeffects"), _T("0")));
						m_bSimpleLine  = Strings::ToInteger(oXmlNode.GetAttribute(_T("text-simpleline"), _T("0")));

						m_dTextDirX1 = Strings::ToDouble(oXmlNode.GetAttribute(_T("text-dir-x1"), _T("0")));
						m_dTextDirY1 = Strings::ToDouble(oXmlNode.GetAttribute(_T("text-dir-y1"), _T("0")));
						m_dTextDirX2 = Strings::ToDouble(oXmlNode.GetAttribute(_T("text-dir-x2"), _T("0")));
						m_dTextDirY2 = Strings::ToDouble(oXmlNode.GetAttribute(_T("text-dir-y2"), _T("0")));

						m_strText = oXmlNode.GetAttributeOrValue(_T("text"), _T(""));
						m_strToken = oXmlNode.GetAttribute(_T("token"), _T(""));

						int nTextLength  = m_strText.GetLength();
						int nTokenLength = m_strToken.GetLength();
						if( nTokenLength > 0 && nTextLength > 0 )
						{
							CString sPath;

							sPath = m_strText.Left( nTokenLength );
							if( sPath == m_strToken )
							{
								m_strText.Delete( 0, nTokenLength - 1 );
								m_strText.SetAt( 0, 30 );
								nTextLength -= nTokenLength - 1;
							}

							sPath = m_strText.Right( nTokenLength );
							if( sPath == m_strToken )
							{
								m_strText.Delete( nTextLength - nTokenLength + 1, nTokenLength - 1 );
								m_strText.SetAt( nTextLength - nTokenLength, 30 );
							}
						}

						m_nAnimationID[0] = Strings::ToInteger(oXmlNode.GetAttribute(_T("animation-fadein"), _T("-1")));
						m_nAnimationID[1] = Strings::ToInteger(oXmlNode.GetAttribute(_T("animation-state"), _T("-1")));
						m_nAnimationID[2] = Strings::ToInteger(oXmlNode.GetAttribute(_T("animation-fadeout"), _T("-1")));
						m_bUseFrameOnly = Strings::ToInteger(oXmlNode.GetAttribute(_T("animation-useframeonly"), _T("0")));

						if (m_nAnimationID[1] == c_nAnimatedTextEllipse)
							m_oSettingsEllipse.FromXmlNode(oXmlNode);
						else if (m_nAnimationID[1] == c_nAnimatedTextPolyline)
							m_oSettingsPolyline.FromXmlNode(oXmlNode);

						m_oBrush.FromXmlNode(oXmlNode);
						m_oFont.FromXmlNode(oXmlNode);
						m_oShadow.FromXmlNode(oXmlNode);
						m_oEdge.FromXmlNode(oXmlNode);

						CString sAlignType = oXmlNode.GetAttribute(_T("aligntype"));
						if ( sAlignType.GetLength() > 0 )
							m_bAlign = TRUE;
						else 
							m_bAlign = FALSE;

						m_nAlignType = Strings::ToInteger(oXmlNode.GetAttribute(_T("aligntype"), _T("4")));
						m_nMarginX   = Strings::ToInteger(oXmlNode.GetAttribute(_T("marginhorizontal"), _T("10")));
						m_nMarginY   = Strings::ToInteger(oXmlNode.GetAttribute(_T("marginvertical"), _T("10")));

						CString sTemp = oXmlNode.GetAttribute(_T("rect-left"));
						if ( sTemp.GetLength() < 1)
							m_bRect = TRUE;

						sTemp = oXmlNode.GetAttribute(_T("rect-top"));
						if ( sTemp.GetLength() < 1)
							m_bRect = TRUE;

						sTemp = oXmlNode.GetAttribute(_T("rect-right"));
						if ( sTemp.GetLength() < 1)
							m_bRect = TRUE;

						sTemp = oXmlNode.GetAttribute(_T("rect-bottom"));
						if ( sTemp.GetLength() < 1)
							m_bRect = TRUE;
					}
					virtual void InternalClear()
					{
						m_strText = _T("");
						m_strToken = _T("");
						m_nAnimationID[0] = -1;
						m_nAnimationID[1] = -1;
						m_nAnimationID[2] = -1;
						m_fAlphaFadeFactorMin = 0;
						m_fAlphaFadeFactorMax = 1;

						m_nMetric = 0;
						m_dLeft = 0;
						m_dTop = 0;
						m_dRight = 100;
						m_dBottom = 100;
						m_bTextClipped = FALSE;
						m_nFillMode = 0;
						m_nTypeEffects = 0;
						m_nAlignType = 4;
						m_nMarginX = 10;
						m_nMarginY = 10;
						m_bAlign = FALSE;
						m_bRect = FALSE;
						m_bSimpleLine = FALSE;
						m_bUseFrameOnly = FALSE;

						m_dTextDirX1 = 0;
						m_dTextDirY1 = 0;
						m_dTextDirX2 = 0;
						m_dTextDirY2 = 0;

						m_oSettingsEllipse.Clear();
						m_oSettingsPolyline.Clear();

						m_oBrush.Clear();
						m_oFont.Clear();
						m_oShadow.Clear();
						m_oEdge.Clear();
					}

				public:

					AnimatedText()
					{
						Initialize(c_nSerializePaintAnimatedText, c_strSerializePaintAnimatedText);

						Clear();
					}

					ImageStudio::Serialize::Paint::Structures::Brush& GetBrush()
					{
						return m_oBrush;
					}
					ImageStudio::Serialize::Paint::Structures::Font& GetFont()
					{
						return m_oFont;
					}
					ImageStudio::Serialize::Paint::Structures::Shadow& GetShadow()
					{
						return m_oShadow;
					}
					ImageStudio::Serialize::Paint::Structures::Edge& GetEdge()
					{
						return m_oEdge;
					}

					CString GetText()
					{
						return m_strText;
					}
					int GetAnimationID(int nIndex) const
					{
						return m_nAnimationID[nIndex];
					}

					AnimatedTextEllipse GetSettingsEllipse()
					{
						return m_oSettingsEllipse;
					}
					AnimatedTextPolyline GetSettingsPolyline()
					{
						return m_oSettingsPolyline;
					}


					int GetMetric() const
					{
						return m_nMetric;
					}
					double GetRectLeft() const
					{
						return m_dLeft;
					}
					double GetRectTop() const
					{
						return m_dTop;
					}
					double GetRectRight() const
					{
						return m_dRight;
					}
					double GetRectBottom() const
					{
						return m_dBottom;
					}
					double GetAlignType() const
					{
						if ( m_bAlign )
							return m_nAlignType;
						else 
							return -1;
					}
					double GetMarginX() const
					{
						return m_nMarginX;
					}
					double GetMarginY() const
					{
						return m_nMarginY;
					}
					float  GetAlphaFadeFactorMin() const
					{
						return m_fAlphaFadeFactorMin;
					}
					float  GetAlphaFadeFactorMax() const
					{
						return m_fAlphaFadeFactorMax;
					}
					BOOL   IsTextClipped()
					{
						return m_bTextClipped;
					}


					BOOL   IsEvaluateRect()
					{
						return m_bRect;
					}


					
					int  m_nFillMode;
					int  m_nTypeEffects;
					BOOL m_bSimpleLine;
					BOOL m_bUseFrameOnly;

					double m_dTextDirX1;
					double m_dTextDirY1;
					double m_dTextDirX2;
					double m_dTextDirY2;

				protected:
					CString m_strText;
					CString m_strToken;
					int     m_nAnimationID[3];
					float   m_fAlphaFadeFactorMin;
					float   m_fAlphaFadeFactorMax;
					int     m_nMetric;
					double  m_dLeft, m_dTop, m_dRight, m_dBottom;
					BOOL    m_bTextClipped;
					int     m_nAlignType;
					int     m_nMarginX;
					int     m_nMarginY;
					BOOL    m_bAlign;
					BOOL    m_bRect; // нужно ли самому определять размер ректа

					AnimatedTextEllipse  m_oSettingsEllipse;
					AnimatedTextPolyline m_oSettingsPolyline;

					ImageStudio::Serialize::Paint::Structures::Brush  m_oBrush;
					ImageStudio::Serialize::Paint::Structures::Font   m_oFont;
					ImageStudio::Serialize::Paint::Structures::Shadow m_oShadow;
					ImageStudio::Serialize::Paint::Structures::Edge   m_oEdge;

					/* @desc:
					ImageStudio-Serialize-Paint-Text-AnimatedText (c_strSerializePaintAnimatedText, c_nSerializePaintAnimatedText)
					{
					// опциональные значения для настроек отрисовки (опциональные только в xml!)
					ImageStudio::Serialize::Paint::Structures::Brush m_oBrush;		// xml имя - brush-...
					ImageStudio::Serialize::Paint::Structures::Font m_oFont;		// xml имя - font-...
					ImageStudio::Serialize::Paint::Structures::Shadow m_oShadow;	// xml имя - shadow-...
					ImageStudio::Serialize::Paint::Structures::Edge m_oEdge;		// xml имя - edge-...
					}
					*/
				};

				class EffectWritingText : public Core::Effect
				{
				protected:

					virtual int InternalGetRequiredStreamSize()
					{
						return 0;
					}
					virtual void InternalToStream(Streams::IStream* pStream)
					{
					}
					virtual void InternalFromStream(Streams::IStream* pStream)
					{
					}

					virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
					{
					}
					virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
					{
						m_nMetric = Strings::ToInteger(oXmlNode.GetAttribute(_T("metric"), _T("0")));
						m_dLeft   = Strings::ToDouble(oXmlNode.GetAttribute(_T("rect-left"), _T("0")));
						m_dTop    = Strings::ToDouble(oXmlNode.GetAttribute(_T("rect-top"), _T("0")));
						m_dRight  = Strings::ToDouble(oXmlNode.GetAttribute(_T("rect-right"), _T("100")));
						m_dBottom = Strings::ToDouble(oXmlNode.GetAttribute(_T("rect-bottom"), _T("100")));

						m_nType = Strings::ToInteger(oXmlNode.GetAttribute(_T("type"), _T("0")));

						m_strText = oXmlNode.GetAttributeOrValue(_T("text"), _T(""));
						m_strToken = oXmlNode.GetAttribute(_T("token"), _T(""));

						int nTextLength  = m_strText.GetLength();
						int nTokenLength = m_strToken.GetLength();
						if( nTokenLength > 0 && nTextLength > 0 )
						{
							CString sPath;

							sPath = m_strText.Left( nTokenLength );
							if( sPath == m_strToken )
							{
								m_strText.Delete( 0, nTokenLength - 1 );
								m_strText.SetAt( 0, 30 );
								nTextLength -= nTokenLength - 1;
							}

							sPath = m_strText.Right( nTokenLength );
							if( sPath == m_strToken )
							{
								m_strText.Delete( nTextLength - nTokenLength + 1, nTokenLength - 1 );
								m_strText.SetAt( nTextLength - nTokenLength, 30 );
							}
						}

						m_oFont.FromXmlNode(oXmlNode);
						m_oEdge.FromXmlNode(oXmlNode);
					}

					virtual void InternalClear()
					{
						m_strText = _T("");
						m_strToken = _T("");

						m_nMetric = 0;
						m_dLeft = 0;
						m_dTop = 0;
						m_dRight = 0;
						m_dBottom = 0;

						m_oFont.Clear();
						m_oEdge.Clear();

						m_nType = 0;
					}

				public:

					EffectWritingText()
					{
						Initialize(c_nSerializeEffectWritingText, c_strSerializeEffectWritingText);

						Clear();
					}

				public:
					ImageStudio::Serialize::Paint::Structures::Font& GetFont()
					{
						return m_oFont;
					}
					ImageStudio::Serialize::Paint::Structures::Edge& GetEdge()
					{
						return m_oEdge;
					}

					CString GetText()
					{
						return m_strText;
					}
					int GetMetric() const
					{
						return m_nMetric;
					}
					double GetRectLeft() const
					{
						return m_dLeft;
					}
					double GetRectTop() const
					{
						return m_dTop;
					}
					double GetRectRight() const
					{
						return m_dRight;
					}
					double GetRectBottom() const
					{
						return m_dBottom;
					}

					int GetType() const
					{
						return m_nType;
					}
				protected:
					CString m_strText;
					CString m_strToken;
					int     m_nMetric;
					double  m_dLeft, m_dTop, m_dRight, m_dBottom;

					int m_nType;

					ImageStudio::Serialize::Paint::Structures::Font   m_oFont;
					ImageStudio::Serialize::Paint::Structures::Edge   m_oEdge;
				};

			}

			namespace Animations
			{
				struct EffectAnimation
				{
				public:
					EffectAnimation ()
					{
						m_nIndex		=	0;

						m_nID			=	-1;
						m_nType			=	0;
						m_nDirection	=	0;

						m_TimeBegin		=	0.0;
						m_TimeEnd		=	0.0;
						
						m_nPlaneWidth	=	0.0;
						m_nPlaneHeight	=	0.0;
					}


					inline virtual bool FromXmlNode ( XmlUtils::CXmlNode& oXmlNode )
					{
						m_nID			=	_tstol ( oXmlNode.GetAttributeOrValue ( _T("effect_id"),		_T("-1") ) );
						m_nType			=	_tstol ( oXmlNode.GetAttributeOrValue ( _T("effect_type"),	_T("-1") ) );
						m_nDirection	=	_tstol ( oXmlNode.GetAttributeOrValue ( _T("effect_dir"),		_T("-1") ) );

						m_TimeBegin		=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("time_delay"),		_T("0.0") ) );
						m_TimeEnd		=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("time_duration"),	_T("0.0") ) );

						m_TimeEnd		=	m_TimeBegin	+ m_TimeEnd;

						m_nPlaneWidth	=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("plale_width"),	_T("0.0") ) );
						m_nPlaneHeight	=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("plane_height"),	_T("0.0") ) );

						return true;
					}

					inline bool IsContains ( double Time )
					{
						if ( Time >= m_TimeBegin && Time <= m_TimeEnd )
						{
							return true;
						}

						return false;
					}
					inline bool IsBetter ( double Time )
					{
						if ( Time > m_TimeEnd )
							return true;

						return false;
					}


					
				public:

					int		m_nIndex;

					int		m_nID;
					int		m_nType;
					int		m_nDirection;

					double	m_TimeBegin;
					double	m_TimeEnd;

					double	m_nPlaneWidth;
					double	m_nPlaneHeight;
				};

				class CMotionPath : public EffectAnimation
				{
				protected:

					struct ActionMove
					{
						double	X[3];
						double	Y[3];
						WCHAR	TYPE;	// L = Line, C = CubicBezier 		
					};

				public:

					CMotionPath ()
					{
						m_X			=	0.0;
						m_Y			=	0.0;

						m_TimeBegin	=	0.0;
						m_TimeEnd	=	0.0;
					}

					inline bool FromXmlNode ( XmlUtils::CXmlNode& oNode )
					{
						EffectAnimation::FromXmlNode ( oNode );

						XmlUtils::CXmlNode oXmlNode;
						if ( oNode.GetNode ( _T("motion_path"), oXmlNode ) )
						{
							m_Points.RemoveAll ();

							m_X		=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("x"),	_T("0.0") ) );
							m_Y		=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("y"),	_T("0.0") ) );

							XmlUtils::CXmlNodes oXmlNodes;
							if ( oXmlNode.GetNodes ( _T("move"), oXmlNodes ) )
							{
								for ( int i = 0; i < oXmlNodes.GetCount(); ++i )
								{
									XmlUtils::CXmlNode MoveNode;
									if ( oXmlNodes.GetAt ( i, MoveNode ) )
									{
										XmlUtils::CXmlNodes PointNodes;
										if ( MoveNode.GetNodes ( _T("point"), PointNodes ) )
										{
											CMotionPath::ActionMove action;

											if ( 2 == PointNodes.GetCount () )
												action.TYPE		=	L'L';
											if ( 3 == PointNodes.GetCount () )
												action.TYPE		=	L'C';

											for ( int j = 0; j < PointNodes.GetCount(); ++j )
											{
												XmlUtils::CXmlNode PointNode;
												if ( PointNodes.GetAt ( j, PointNode ) )
												{
													action.X[j]	=	_tstof ( PointNode.GetAttributeOrValue ( _T("x"),	_T("0.0") ) );
													action.Y[j]	=	_tstof ( PointNode.GetAttributeOrValue ( _T("y"),	_T("0.0") ) );
												}
											}

											m_Points.Add ( action );
										}
									}
								}
							}
						}

						return ( m_Points.GetSize () >= 1 );
					}

					inline void SetTimes ( double TimeBegin, double TimeEnd )
					{
						m_TimeBegin		=	TimeBegin;
						m_TimeEnd		=	TimeEnd;
					}

					inline bool GetValue ( const double& Time, double& X, double& Y ) const
					{
						if ( Time < m_TimeBegin || Time > m_TimeEnd )
							return false;

						double TimeFactor	=	( Time - m_TimeBegin ) / ( m_TimeEnd - m_TimeBegin );

						double dSelectMove	=	( (double)m_Points.GetSize () ) * TimeFactor;
						int Ind				=	(int)dSelectMove;
						double T			=	dSelectMove - (double)Ind;

						if ( Ind >= 0 && m_Points.GetSize () > Ind )
						{						
							if ( L'L' == m_Points [ Ind ].TYPE )
							{
								X			=	m_Points [ Ind ].X [ 0 ] + ( m_Points [ Ind ].X [ 1 ] - m_Points [ Ind ].X [ 0 ] ) * T;
								Y			=	m_Points [ Ind ].Y [ 0 ] + ( m_Points [ Ind ].Y [ 1 ] - m_Points [ Ind ].Y [ 0 ] ) * T;

								return true;
							}

							if ( L'C' == m_Points [ Ind ].TYPE )
							{
								if ( 0 == Ind )
								{
									GetCubicBezierValue ( m_Points [ Ind ], m_X, m_Y, T, X, Y );
									return true;
								}

								if ( L'C' == m_Points [ Ind - 1 ].TYPE  )
								{
									GetCubicBezierValue ( m_Points [ Ind ], m_Points [ Ind - 1 ].X [ 2 ], m_Points [ Ind - 1 ].Y [ 2 ], T, X, Y );
									return true;
								}

								if ( L'L' == m_Points [ Ind - 1 ].TYPE )
								{
									GetCubicBezierValue ( m_Points [ Ind ], m_Points [ Ind - 1 ].X [ 1 ], m_Points [ Ind - 1 ].Y [ 1 ], T, X, Y );
									return true;
								}

								return true;
							}
						}

						return  false;
					}

					inline bool GetEndValue ( double& X, double& Y )
					{
						int Ind		=	m_Points.GetSize () - 1;

						if ( L'L' == m_Points [ Ind ].TYPE )
						{
							X			=	m_Points [ Ind ].X [ 1 ];
							Y			=	m_Points [ Ind ].Y [ 1 ];

							return true;
						}

						if ( L'C' == m_Points [ Ind ].TYPE )
						{
							X			=	m_Points [ Ind ].X [ 2 ];
							Y			=	m_Points [ Ind ].Y [ 2 ];

							return true;
						}

						return true;
					}
					inline const double& GetBeginTime ()
					{
						return m_TimeBegin;
					}
					inline const double& GetEndTime ()
					{
						return m_TimeEnd;
					}
				protected:

					inline void GetCubicBezierValue ( const ActionMove& pMove, const double& X0, const  double& Y0, const double& Factor, double& X, double& Y ) const
					{
						double mum1		=	0.0;
						double mum13	=	0.0;
						double mu3		=	0.0;

						mum1			=	1.0 - Factor;
						mum13			=	mum1 * mum1 * mum1;
						mu3				=	Factor * Factor * Factor;

						X				=	mum13 * X0 + 3.0 * Factor * mum1 * mum1 * pMove.X [ 0 ] + 3.0 * Factor * Factor * mum1 * pMove.X [ 1 ] + mu3 * pMove.X [ 2 ];
						Y				=	mum13 * Y0 + 3.0 * Factor * mum1 * mum1 * pMove.Y [ 0 ] + 3.0 * Factor * Factor * mum1 * pMove.Y [ 1 ] + mu3 * pMove.Y [ 2 ];
					}

				protected:

					CSimpleArray < ActionMove >	m_Points;

					// начальное положение
					double						m_X;
					double						m_Y;
				};

				class CAnimationContainer
				{
				public:

					CAnimationContainer ()
					{
						m_nIndex			=	0;

						m_nEffectID			=	-1;
						m_nEffectDirection	=	-1;
						m_nEffectType		=	-1;
						m_nTimeDelay		=	0.0;
						m_nTimeDuration		=	0.0;
					}

					virtual ~CAnimationContainer ()
					{
						Clear ();
					}

					inline virtual bool FromXmlNode ( XmlUtils::CXmlNode& oXmlNode )
					{
						Clear ();

						// ! если получилось что несколько motion path анимаций, и больше ни каких анимаций,
						// ! классы отрисовщиков применят трансформации перемещения и все нормально отрисуется

						XmlUtils::CXmlNodes oXmlNodes;
						if ( oXmlNode.GetNodes ( _T("animation_effect"), oXmlNodes ) )
						{
							for ( int i = 0; i < oXmlNodes.GetCount(); ++i )
							{
								XmlUtils::CXmlNode NodeEffect;
								if ( oXmlNodes.GetAt ( i, NodeEffect ) )
								{
									long EffectType = _tstol ( NodeEffect.GetAttributeOrValue ( _T("effect_type"),		_T("-1") ) );

									if ( 0x00000001 == EffectType )	//	Entrance
									{
										EffectAnimation*	pEffect	=	new EffectAnimation ();
										if ( NULL != pEffect )
										{
											pEffect->FromXmlNode ( NodeEffect );
											m_Effects.Add ( pEffect );

											m_EntranceIndexes.Add ( m_Effects.GetSize () - 1 );
										}
									}
									
									if ( 0x00000002 == EffectType )	//	Exit
									{
										EffectAnimation*	pEffect	=	new EffectAnimation ();
										if ( NULL != pEffect )
										{
											pEffect->FromXmlNode ( NodeEffect );
											m_Effects.Add ( pEffect );

											m_ExitIndexes.Add ( m_Effects.GetSize () - 1 );
										}
									}

									if ( 0x00000003 == EffectType )								//	Emphasis
									{
										EffectAnimation*	pEffect	=	new EffectAnimation ();
										if ( NULL != pEffect )
										{
											pEffect->FromXmlNode ( NodeEffect );
											m_Effects.Add ( pEffect );
										}
									}

									if ( 0x00000004	== EffectType )								//	Motion path
									{
										EffectAnimation*	pEffect	=	new CMotionPath ();
										if ( NULL != pEffect )
										{
											pEffect->FromXmlNode ( NodeEffect );
											m_Effects.Add ( pEffect );

											m_MPIndexes.Add ( m_Effects.GetSize () - 1 );
										}
									}

									//EffectAnimation	effect;
									
									//if ( effect.FromXmlNode ( NodeEffect ) )
									//{
									//	effect.m_nIndex		=	m_nIndex;
									//	++m_nIndex;

									//	m_nEffectID			=	atol ( NodeEffect.GetAttributeOrValue ( _T("effect_id"),		_T("-1") ) );
									//	m_nEffectType		=	atol ( NodeEffect.GetAttributeOrValue ( _T("effect_type"),		_T("-1") ) );
									//	m_nEffectDirection	=	atol ( NodeEffect.GetAttributeOrValue ( _T("effect_dir"),		_T("-1") ) );

									//	m_nTimeDelay		=	atol ( NodeEffect.GetAttributeOrValue ( _T("time_delay"),		_T("0.0") ) );
									//	m_nTimeDuration		=	atol ( NodeEffect.GetAttributeOrValue ( _T("time_duration"),	_T("0.0") ) );

									//	if ( 0x00000001	== effect.m_nType || 0x00000002	== effect.m_nType )	//	Entrance Exit
									//	{
									//		m_GoEffects.Add ( effect );
									//	}

									//	if ( 0x00000003	== effect.m_nType )	//	Emphasis
									//	{
									//		m_StressEffects.Add ( effect );
									//	}

									//	if ( 0x00000004	== effect.m_nType )	//	Motion path
									//	{
									//		XmlUtils::CXmlNode MotionEffect;
									//		if ( NodeEffect.GetNode ( _T("motion_path"), MotionEffect ) )
									//		{
									//			CMotionPath	path;
									//			if ( path.FromXmlNode ( MotionEffect ) )
									//			{
									//				path.m_nIndex		=	m_nIndex;

									//				path.SetTimes ( effect.m_TimeBegin, effect.m_TimeEnd );
									//				m_arrMotion.Add ( path );
									//			}
									//		}
									//	}
									//}
								}
							}
						}

						return true;
					}

					inline virtual void Clear ()
					{
						m_nIndex			=	0;

						m_nEffectID			=	-1;
						m_nEffectDirection	=	-1;
						m_nEffectType		=	-1;
						m_nTimeDelay		=	0.0;
						m_nTimeDuration		=	0.0;

						m_arrMotion.RemoveAll ();
						m_GoEffects.RemoveAll ();
						m_StressEffects.RemoveAll ();

						for ( int i = 0; i < m_Effects.GetSize (); ++i )
						{
							RELEASEOBJECT ( m_Effects [i] );
						}

						m_Effects.RemoveAll ();
					}

					inline bool GetMotionOffset ( const double& Time, double& RefX, double& RefY )
					{
						if ( 0 == m_arrMotion.GetSize () )
							return false;

						// ! на равнозначных участках берем значение на том отрезке, у которого EndTime больше для данного времени
						// ! если Time не входит не в один временной отрезок, значение наиболее близкое значение по времени от Time слева

						double MaxTimeForEnd	=	-1.0;
						// 
						double MinTimeForEnd	=	-1.0;

						int ZoneMax				=	-1;
						int ZoneMin				=	-1;

						for ( int i = 0; i < m_arrMotion.GetSize (); ++i )
						{
							if ( m_arrMotion[i].IsBetter ( Time ) )
							{
								if ( MinTimeForEnd < m_arrMotion[i].GetEndTime () )
								{
									MinTimeForEnd		=	m_arrMotion[i].GetEndTime ();
									ZoneMin				=	i;
								}
							}

							if ( m_arrMotion[i].IsContains ( Time ) )
							{
								if ( MaxTimeForEnd < m_arrMotion[i].GetEndTime () )
								{
									MaxTimeForEnd	=	m_arrMotion[i].GetEndTime ();
									ZoneMax			=	i;
								}
							}
						}

						double X = 0.0;
						double Y = 0.0;

						if ( -1 == ZoneMax && -1 != ZoneMin )
						{
							m_arrMotion [ ZoneMin ].GetEndValue ( X, Y );

							RefX	=	X;
							RefY	=	Y;

							return true;
						}

						if ( -1 == ZoneMax && -1 != ZoneMin || -1 == ZoneMax && -1 == ZoneMin )		// случай когда по текущему времени, все анимации дальше по времени
						{
							return false;
						}

						m_arrMotion [ ZoneMax ].GetValue ( Time, X, Y );

						RefX	=	X;
						RefY	=	Y;

						EffectAnimation effect;
						double TimeFactor	= 0.0;
						
						//if ( GetGoEffect ( Time, effect, TimeFactor ) )
						//{
						//	if ( m_arrMotion [ ZoneMax ].m_nIndex < effect.m_nIndex )
						//		return false;
						//}

						return true;
					}

					inline bool GetGoEffect ( const double& Time, EffectAnimation& effect, double& TimeFactor )
					{
						if ( 0 == m_GoEffects.GetSize () )
							return false;

						int ExitEffect		=	-1;
						double ExitTime		=	0.0;
						
						int ZoneEffectFirst		=	-1;		//	первый эффект по таймлайну типа - Entrance
						double TimeEffectFirst	=	m_GoEffects[0].m_TimeBegin;

						for ( int i = 0; i < m_GoEffects.GetSize (); ++i )
						{
							if ( m_GoEffects[i].m_TimeBegin <= TimeEffectFirst /*&& 2 == m_GoEffects[i].m_nType*/ )
							{
								ZoneEffectFirst	=	i;
								TimeEffectFirst	=	m_GoEffects[i].m_TimeBegin;
							}							

							// ! возможен вариант один эффект типа Exit на таймлайне
							if ( m_GoEffects[i].IsBetter ( Time ) && 2 == m_GoEffects[i].m_nType )
							{
								if ( ExitTime < m_GoEffects[i].m_TimeEnd )
								{
									ExitTime	=	m_GoEffects[i].m_TimeEnd;

									ExitEffect	=	i;
								}
							}

							if ( m_GoEffects[i].IsContains ( Time ) )
							{
								effect		=	m_GoEffects[i];

								TimeFactor	=	( Time - m_GoEffects[i].m_TimeBegin ) / ( m_GoEffects[i].m_TimeEnd - m_GoEffects[i].m_TimeBegin );

								return true;
							}
						}

						//if ( 2 == m_GoEffects [ 0 ].m_nType )
						//{
						//	return false;
						//}

						//if ( 1 == m_GoEffects [ ZoneEffectFirst ].m_nType )
						//{
						//	effect		=	m_GoEffects [ ZoneEffectFirst ];
						//	TimeFactor	=	0.0;

						//	return true;
						//}

						if ( -1 == ExitEffect && -1 != ZoneEffectFirst )
						{
							if ( 1 == m_GoEffects [ ZoneEffectFirst ].m_nType )
							{
								effect		=	m_GoEffects [ ZoneEffectFirst ];
								TimeFactor	=	0.0;

								return true;
							}
						}

						if ( -1 != ExitEffect )
						{
							effect		=	m_GoEffects [ ExitEffect ];
							TimeFactor	=	1.000001;

							return true;
						}

						return false;
					}

					inline bool GetStressEffect ( const double& Time, EffectAnimation& effect )
					{
						if ( 0 == m_StressEffects.GetSize () )
							return false;


						return false;
					}

					inline int GetEffectIndex ( const double& Time )
					{

					}

					
					inline bool GetEffect ( const double& Time, EffectAnimation** pComposeEffect, EffectAnimation** pMPEffect, double& TimeFactor, bool& EffectNone, bool& EffectExit )
					{
						// ! в спецификации не описан способ отображения последовательности анимаций.
						if ( m_Effects.GetSize () == 0 )
							return false;
						
						*pComposeEffect		=	NULL;

						int		Ind			=	-1;
						double	BeginTime	=	-1.0;

						EffectNone			=	true;
						EffectExit			=	false;

						for ( int i = 0; i < m_Effects.GetSize (); ++i )
						{
							if ( m_Effects[i]->IsBetter ( Time ) )
							{
								EffectNone	=	false;
							}

							if ( m_Effects[i]->IsContains ( Time ) )
							{
								if ( BeginTime < m_Effects[i]->m_TimeBegin )
								{
									Ind			=	i;
									BeginTime	=	m_Effects[i]->m_TimeBegin;
									EffectNone	=	false;
								}
							}
						}

						// все эффекты правее по данной временно метке
						if ( EffectNone && -1 == Ind )
							return true;
					
						int ExitEffect = GetExitEffect ( Time );
						int EntrEffect = GetEntranceEffect ( Time );

						// по данному времени нету эффектов
						if ( -1 == Ind )
						{
							if ( -1 != ExitEffect && -1 == EntrEffect )
							{
								EffectExit	=	true;	// находимся по времени, когда все эффекты слева и последний EXIT
								return true;
							}
							
							if ( -1 != ExitEffect && -1 != EntrEffect )
							{
								if ( m_Effects [ EntrEffect ]->IsContains ( m_Effects [ ExitEffect ]->m_TimeEnd ) )
								{
									//if ( )
									{
										//pEffect
									}

									EffectExit	=	true;
									// гхммм... получилось что имеется какой то эффект, но есть пересечение с EXIT эффектом, соотвественно, ничего рисовать не нужно
									return true;
								}
							}

							return false;
						}

						if ( -1 != ExitEffect )
						{
							if ( Ind != ExitEffect )
							{
								if ( m_Effects [ Ind ]->IsContains ( m_Effects [ ExitEffect ]->m_TimeEnd ) )
								{
									// гхммм... получилось что имеется какой то эффект, но есть пересечение с EXIT эффектом, соотвественно, ничего рисовать не нужно
									return true;
								}
							}
						}

						if ( 0x00000001 == m_Effects [ Ind ]->m_nType )
						{
							// в том случае если, имеется пересечение текуще эффекта с другим EXIT эффектом ранее примененным
							if ( IsContainsExitEffect ( m_Effects [ Ind ]->m_TimeBegin, Time ) )							
							{
								EffectExit	=	true;
								return true;
							}
						}

						int MPEffect = GetMPEffect ( Time );
						if ( -1 != MPEffect )
						{
							if ( MPEffect < Ind )
							{
								*pMPEffect = m_Effects [ m_MPIndexes [ MPEffect ] ];
							}
						}

						*pComposeEffect	=	m_Effects [ Ind ];
						TimeFactor		=	( Time -( *pComposeEffect )->m_TimeBegin ) / ( ( *pComposeEffect )->m_TimeEnd - ( *pComposeEffect )->m_TimeBegin );
						
						return true;
					}



					inline bool IsFirstEntrance ()
					{
						if ( m_Effects.GetSize () == 0 )
							return false;

						return ( 0x00000001 == m_Effects[0]->m_nType );
					}
					inline bool IsFirstEffectExit ()
					{
						if ( m_Effects.GetSize () == 0 )
							return false;

						return ( 0x00000002 == m_Effects[0]->m_nType );
					}


				private:

					inline int GetExitEffect ( const double& Time )
					{
						int		ExitEffect	=	-1;
						double	EffectTime	=	-1.0;

						for ( int i = 0; i < m_ExitIndexes.GetSize (); ++i )
						{
							if (  m_Effects [ m_ExitIndexes[i] ]->IsBetter ( Time ) )
							{
								if ( EffectTime < m_Effects [ m_ExitIndexes[i] ]->m_TimeEnd )
								{
									ExitEffect	=	i;
									EffectTime	=	m_Effects [ m_ExitIndexes[i] ]->m_TimeEnd;
								}
							}
						}

						return ExitEffect;
					}

					inline int GetEntranceEffect ( const double& Time )
					{
						int		Effect	=	-1;
						double	EffectTime	=	-1.0;

						for ( int i = 0; i < m_EntranceIndexes.GetSize (); ++i )
						{
							if (  m_Effects [ m_EntranceIndexes[i] ]->IsBetter ( Time ) )
							{
								if ( EffectTime < m_Effects [ m_EntranceIndexes[i] ]->m_TimeEnd )
								{
									Effect	=	i;
									EffectTime	=	m_Effects [ m_EntranceIndexes[i] ]->m_TimeEnd;
								}
							}
						}

						return Effect;
					}

					inline bool IsContainsExitEffect ( const double& TimeBegin, const double& TimeEnd )
					{
						for ( int i = 0; i < m_ExitIndexes.GetSize (); ++i )
						{
						//	EffectAnimation oEffect = *m_Effects [ m_ExitIndexes[i] ];

							if ( m_Effects [ m_ExitIndexes[i] ]->m_TimeEnd >= TimeBegin &&
								 m_Effects [ m_ExitIndexes[i] ]->m_TimeBegin <= TimeEnd ||
								 m_Effects [ m_ExitIndexes[i] ]->m_TimeEnd >= TimeBegin &&
								 m_Effects [ m_ExitIndexes[i] ]->m_TimeBegin <= TimeEnd )
							{
								return true;
							}
						}

						return false;
					}

					inline int GetMPEffect ( const double& Time )
					{
						int		Effect	=	-1;
						double	EffectTime	=	-1.0;

						for ( int i = 0; i < m_MPIndexes.GetSize (); ++i )
						{
							if ( m_Effects [ m_MPIndexes[i] ]->IsContains ( Time ) )
							{
								if ( EffectTime < m_Effects [ m_MPIndexes[i] ]->m_TimeEnd )
								{
									Effect	=	i;
									EffectTime	=	m_Effects [ m_MPIndexes[i] ]->m_TimeEnd;
								}
							}
						}

						return Effect;
					}

				public:

					int									m_nIndex;

					long								m_nEffectID;
					long								m_nEffectDirection;
					long								m_nEffectType;

					double								m_nTimeDelay;
					double								m_nTimeDuration;
				
					CSimpleArray < EffectAnimation* >	m_Effects;
					CSimpleArray < int >				m_ExitIndexes;
					CSimpleArray < int >				m_EntranceIndexes;
					CSimpleArray < int >				m_MPIndexes;

					CSimpleArray < EffectAnimation >	m_GoEffects;
					CSimpleArray < EffectAnimation >	m_StressEffects;

					CSimpleArray < CMotionPath >		m_arrMotion;		// траектории группируются и по ним выдается смещение
				};

				
				// временно отключена анимация. если она и будет, то в AVSGraphics

				class DrawImageFromFileAnimate : public Core::Effect// : public ImageStudio::Serialize::Paint::Common::DrawImageFromFile
				{
				protected:

					virtual void InternalFromXmlNode ( XmlUtils::CXmlNode& oXmlNode )
					{
						//ImageStudio::Serialize::Paint::Common::DrawImageFromFile::InternalFromXmlNode ( oXmlNode );

						m_Animation.FromXmlNode ( oXmlNode );
					}

					virtual void InternalClear ()
					{
						//ImageStudio::Serialize::Paint::Common::DrawImageFromFile::InternalClear();
						m_Animation.Clear ();
					}

				public:

					DrawImageFromFileAnimate ()
					{
						Initialize ( c_nSerializePaintDrawImageFromFileAnimate, c_strSerializePaintDrawImageFromFileAnimate );

						Clear();
					}

				public:

					CAnimationContainer	m_Animation;
				};

				class DrawGraphicPathAnimate : public Core::Effect// : public ImageStudio::Serialize::Paint::Common::DrawGraphicPath
				{
				protected:
								
					virtual void InternalFromXmlNode ( XmlUtils::CXmlNode& oXmlNode )
					{
						//ImageStudio::Serialize::Paint::Common::DrawGraphicPath::InternalFromXmlNode ( oXmlNode );
						m_Animation.FromXmlNode ( oXmlNode );
					}					
					
					virtual void InternalClear()
					{
						//ImageStudio::Serialize::Paint::Common::DrawGraphicPath::InternalClear ();
						m_Animation.Clear ();
					}

				public:

					DrawGraphicPathAnimate()
					{
						Initialize ( c_nSerializePaintDrawPathAnimate, c_strSerializePaintDrawPathAnimate );

						Clear();
					}


				public:
					CAnimationContainer	m_Animation;
				};
			}
		}
		namespace Compose
		{
			class Render : public Core::Effect
			{
			protected:
							
				virtual int InternalGetRequiredStreamSize()
				{
					return 5*sizeof(int) + sizeof(double);
				}
				virtual void InternalToStream(Streams::IStream* pStream)
				{
					pStream->WriteDouble(Time);
					pStream->WriteLong(Category);
					pStream->WriteLong(Effect);
					pStream->WriteLong(EffectID);
					pStream->WriteLong(TilesWidth);
					pStream->WriteLong(TilesHeight);
				}
				virtual void InternalFromStream(Streams::IStream* pStream)
				{
					Time = pStream->ReadDouble();
					Category = pStream->ReadLong();
					Effect = pStream->ReadLong();
					EffectID = pStream->ReadLong();
					TilesWidth = pStream->ReadLong();
					TilesHeight = pStream->ReadLong();
				}
				
				virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
				{
					oXmlWriter.WriteAttribute(_T("Time"), Time); // TODO: change to lowercase !
					oXmlWriter.WriteAttribute(_T("category"), Category);
					oXmlWriter.WriteAttribute(_T("effect"), Effect);
					oXmlWriter.WriteAttribute(_T("effectid"), EffectID);
					oXmlWriter.WriteAttribute(_T("tileswidth"), TilesWidth);
					oXmlWriter.WriteAttribute(_T("tilesheight"), TilesHeight);
				}
				virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Time = Strings::ToDouble(oXmlNode.GetAttributeOrValue(_T("Time"), _T("-1"))); // TODO: change to lowercase !
					Category = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("category"), _T("-1")));
					Effect = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("effect"), _T("-1")));
					EffectID = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("effectid"), _T("-1")));
					TilesWidth = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("tileswidth"), _T("0")));
					TilesHeight = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("tilesheight"), _T("0")));
				}
				
				virtual void InternalClear()
				{
					Time = -1;
					Category = -1;
					Effect = -1;
					EffectID = -1;
					TilesWidth = 0;
					TilesHeight = 0;
				}
				
			public:
				
				Render()
				{
					Initialize(c_nSerializeComposeRender, c_strSerializeComposeRender);

					Clear();
				}
					
			public:
				
				double Time;
				int TilesWidth, TilesHeight;
				int Category, Effect, EffectID;

				/* @desc:
				ImageStudio-Serialize-Compose-Render (c_nSerializeComposeRender, c_strSerializeComposeRender)
				{
					// длительность перехода. при Time > 0 эффект работает не по таймлайну, а по Time'у (при Time <= 0 - по таймлайнам)
					double Time = -1;		// xml имя - Time

					// размер тайлов по ширине и высоте в пикселях: 0 - означает эффект на всей картинке, 2.. 
					int TilesWidth = 0;		// xml имя - tileswidth
					int TilesHeight = 0;	// xml имя - tilesheight
					
					// параметры, описывающие, какой эффект применять. либо EffectID, либо Category-Effect
					int Category = -1;		// xml имя - category; порядковый индекс категории
					int Effect = -1;		// xml имя - effect; порядковый индекс эффекта
					int EffectID = -1;		// xml имя - effectid; уникальный идентификатор эффекта
				}
				*/
			};
		}
	}
	namespace Serialize
	{
		class SerializedObjectNamesManager
		{
			//CAtlMap<CString, int> m_mapEffects;
			
		private:
			
			//#define SerializedObjectNamesManager_AddObject_Typed(Type) \
			//	if (TRUE) \
			//	{ \
			//		##Type oObject; \
			//		\
			//		for (int index = 0; index < oObject.Names.GetSize(); ++index) \
			//			m_mapEffects.SetAt(oObject.Names[index], oObject.ID); \
			//	}

			static const TNamePair* FindPair( const TNamePair* pPair, unsigned int nCount, const TCHAR* pText )
			{
				// двоичный поиск в массиве, упорядоченном по возрастанию

				if( !nCount )
					return 0;

				pPair += nCount / 2;

				int nResult = ::_tcscmp( pText, pPair->m_str );
				if( !nResult )
					return pPair;

				if( nResult > 0 )
				{
					nCount = (nCount - 1) / 2;
					pPair += 1;
				}
				else
				{
					nCount = nCount / 2;
					pPair -= nCount;
				}

				return FindPair( pPair, nCount, pText );
			}

		public:
			
			SerializedObjectNamesManager()
			{
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::SerializedGroup);

				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::SerializedBufferClear);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::SerializedBufferCopy);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Core::AdditionalParams);

				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::ImageClear);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::ImageCopy);

				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustBrightness);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustBrightnessEx);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustContrast);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustGamma);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustEqualize);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustGrayscale);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustInvert);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustAutocontrast);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustAutolevels);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustHue);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustSaturation);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustPosterize);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustTemperature);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustColorize);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustThreshold);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::AdjustSepia);

				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectBlur);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectSharpen);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectMosaic);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectGaussianBlur);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectNoise);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectDiffuse);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectMotionBlur);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectEmboss);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectMinimal);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectMaximal);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectDeinterlace);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectMedian);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectChromoKeyRange);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectChromoKeyLevel);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectCinema);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectGlass);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectGlassMosaic);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectPencilSketch);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectWaterColor);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectGlow);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectTVSimulation);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectGrayOut);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectWind);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectNewsprint);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectSnow);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectJpegQuality);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::EffectOldPhoto);

				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformFlip);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformPerspective);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformSkew);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformShift);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformRotate);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformRotateAdvanced);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformRotateSimple);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformResample);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformZoom);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformZoomLinear);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformMirror);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformResize);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformTwirl);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformSphere);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformCylinder);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformWave);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformRipple);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::TransformRotateSpecial);

				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::FilterPrewitt);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::FilterScharr);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::FilterSobel);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::FilterRoberts);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::FilterLaplace);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::FilterHipass);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::FilterLowpass);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::FilterBlur);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Effects::FilterCanny);

				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileLoadImage);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileLoadScreenshot);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileLoadRaw);
				//
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileSaveAsBmp);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileSaveAsGif);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileSaveAsJpeg);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileSaveAsPng);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileSaveAsTiff);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileSaveAsWmf);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileSaveAsEmf);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileSaveAsPcx);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileSaveAsTga);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileSaveAsRas);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileSaveAsIPod);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileSaveAsPsd);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::File::FileSaveAsJp2);

				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::SetPen);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::SetBrush);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::SetFont);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::SetFormat);

				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawLine);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawRectangle);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawEllipse);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawInvRectangle);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawInvEllipse);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawPie);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawSector);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawSimplePie);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawSimpleSector);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawPolyline);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawPolygon);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawBorder);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawMovObject);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawCredits);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawTextSimple);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawTextInRect);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawImageFromFile);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawImageFromBuffer);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawTimer);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawImageFromStorage);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawFilm);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawPuzzle);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawGraphicPath);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Common::DrawTextEx);

				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::TextStatic::SetBrush);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::TextStatic::SetFont);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::TextStatic::SetFormat);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::TextStatic::SetShadow);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::TextStatic::SetEdge);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::TextStatic::DrawInRect);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::TextStatic::DrawOnEllipse);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::TextStatic::DrawOnPolyline);

				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::TextDynamic::SetBrush);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::TextDynamic::SetFont);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::TextDynamic::SetShadow);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::TextDynamic::SetEdge);

				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::TextDynamic::Start);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::TextDynamic::Stop);
				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::TextDynamic::Render);

				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Compose::Render);

				//SerializedObjectNamesManager_AddObject_Typed(ImageStudio::Serialize::Paint::Text::AnimatedText);
			}
			int GetEffectIDByName(const CString& strName)
			{
				//ATL::CAtlMap<CString, int>::CPair* pPair = m_mapEffects.Lookup(strName);

				//if (pPair != NULL)
				//	return pPair->m_value;

				const TNamePair* pPair = FindPair( c_aMapNames, sizeof(c_aMapNames)/sizeof(c_aMapNames[0]), strName );
				if( pPair )
					return pPair->m_id;

				return ImageStudio::Serialize::c_nSerializeNone;
			}
		};
		
		class SerializedObjectList
		{
			SerializedObjectNamesManager* m_pNamesManager; // менеджер имен трансформов (нужен для быстрого поиска типа трансформа по имени)

			CAtlArray<ImageStudio::Serialize::SerializedObject*> m_arrEffects; // список трансформов для применения
			
		private:
			
			CString NormalizeXml(const CString& strXml)
			{
				// TODO: надо сделать проверку - может мы вообще не имеем никаких эффектов в этой xml

				Templates::CArray<CString> arrGroupNames;

				CString strXmlTemp = strXml;
				CString strGroupName;
				
				Strings::Split(c_strSerializeGroup, _T("|"), arrGroupNames);
				
				for (int index = 0; index < arrGroupNames.GetCount(); ++index)
				{
					strGroupName = _T("<") + arrGroupNames[index];

					if (Strings::StartsWith(strXmlTemp, strGroupName))
						return strXml;
				}

				return _T("<") + arrGroupNames[0] + _T(">") + strXmlTemp + _T("</") + arrGroupNames[0] + _T(">");
			}

			#define SerializedObjectList_AddEffect_Typed(Constant, Class, ID, Node) \
			case ImageStudio::Serialize::##Constant: \
				{ \
					ImageStudio::Serialize::##Class* pObject = new ImageStudio::Serialize::##Class(); \
					if( pObject ) \
					{ \
						if( pObject->FromXmlNode(Node)) \
						{ \
							if( Add(pObject) ) \
								return; \
						} \
						delete pObject; \
					} \
				} break;
			
		public:
			
			SerializedObjectList()
			{
				m_pNamesManager = NULL;
			}
			virtual ~SerializedObjectList()
			{
				Clear(TRUE);
			}
			
			void SetNamesManager(ImageStudio::Serialize::SerializedObjectNamesManager* pNamesManager)
			{
				m_pNamesManager = pNamesManager;
			}
			ImageStudio::Serialize::SerializedObjectNamesManager* GetNamesManager()
			{
				return m_pNamesManager;
			}
			
			int GetCount()
			{
				return (int)m_arrEffects.GetCount();
			}
			ImageStudio::Serialize::SerializedObject* operator[](int nIndex)
			{
				return GetAt(nIndex);
			}
			ImageStudio::Serialize::SerializedObject* GetAt(int nIndex)
			{
				return m_arrEffects[nIndex];
			}
			void SetAt(int nIndex, ImageStudio::Serialize::SerializedObject* pObject)
			{
				m_arrEffects.SetAt(nIndex, pObject);
			}
			
			bool Add(ImageStudio::Serialize::SerializedObject* pObject)
			{
				try
				{
					return m_arrEffects.Add(pObject) < m_arrEffects.GetCount();
				}
				catch(...)
				{
					return false;
				}
			}
			void Add(int nTransformID, XmlUtils::CXmlNode& oXmlNode)
			{
				switch( nTransformID )
				{
					SerializedObjectList_AddEffect_Typed(c_nSerializeBufferClear,				SerializedBufferClear, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeBufferCopy,				SerializedBufferCopy,	nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeSetAdditionalParams,		Core::AdditionalParams, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeSetParamsUseVideoCard,		SerializedSetParamsUseVideoCard, nTransformID, oXmlNode);

					SerializedObjectList_AddEffect_Typed(c_nSerializeImageClear,				Effects::ImageClear, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeImageCopy,					Effects::ImageCopy, nTransformID, oXmlNode);

					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustBrightness,			Effects::AdjustBrightness, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustBrightnessEx,		Effects::AdjustBrightnessEx, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustContrast,			Effects::AdjustContrast, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustGamma,				Effects::AdjustGamma, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustEqualize,			Effects::AdjustEqualize, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustGrayscale,			Effects::AdjustGrayscale, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustInvert,				Effects::AdjustInvert, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustAutobrightness,		Effects::AdjustAutobrightness, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustAutocontrast,		Effects::AdjustAutocontrast, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustAutolevels,			Effects::AdjustAutolevels, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustHue,					Effects::AdjustHue, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustSaturation,			Effects::AdjustSaturation, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustPosterize,			Effects::AdjustPosterize, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustTemperature,			Effects::AdjustTemperature, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustColorize,			Effects::AdjustColorize, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustThreshold,			Effects::AdjustThreshold, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustSepia,   			Effects::AdjustSepia, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustWhiteBalance,		Effects::AdjustWhiteBalance, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustAutoWhiteBalance,	Effects::AdjustAutoWhiteBalance, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustClarity,				Effects::AdjustClarity, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustVibrance,			Effects::AdjustVibrance, nTransformID, oXmlNode);

					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustRecovery,   			Effects::AdjustRecovery, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustFillLight,   		Effects::AdjustFillLight, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustExposure,   			Effects::AdjustExposure, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustBlacks,   			Effects::AdjustBlacks, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustHighLight,   		Effects::AdjustHighLight, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeAdjustShadows,   			Effects::AdjustShadows, nTransformID, oXmlNode);

					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectBlur,				Effects::EffectBlur, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectSharpen,				Effects::EffectSharpen, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectMosaic,				Effects::EffectMosaic, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectGaussianBlur,		Effects::EffectGaussianBlur, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectNoise,				Effects::EffectNoise, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectDiffuse,				Effects::EffectDiffuse, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectMotionBlur,			Effects::EffectMotionBlur, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectEmboss,				Effects::EffectEmboss, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectKenBurns,			Effects::EffectKenBurns, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectMinimal,				Effects::EffectMinimal, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectMaximal,				Effects::EffectMaximal, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectDeinterlace,			Effects::EffectDeinterlace, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectMedian,				Effects::EffectMedian, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectChromoKeyRange,		Effects::EffectChromoKeyRange, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectChromoKeyLevel,		Effects::EffectChromoKeyLevel, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectCinema,				Effects::EffectCinema, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectGlass,				Effects::EffectGlass, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectGlassMosaic,			Effects::EffectGlassMosaic, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectPencilSketch,		Effects::EffectPencilSketch, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectWaterColor,			Effects::EffectWaterColor, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectGlow,				Effects::EffectGlow, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectTVSimulation,		Effects::EffectTVSimulation, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectGrayOut,				Effects::EffectGrayOut, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectWind,				Effects::EffectWind, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectNewsprint,			Effects::EffectNewsprint, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectSnow,				Effects::EffectSnow, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectJpegCompression,		Effects::EffectJpegCompression, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectSoftFocus,			Effects::EffectSoftFocus, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectOldPhoto,		    Effects::EffectOldPhoto, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectVignette,		    Effects::EffectVignette, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectPostCrop,		    Effects::EffectPostCrop, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectSetAlphaChannel,	    Effects::EffectSetAlphaChannel, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectZoomingSphere,	    Effects::EffectZoomingSphere, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectWaterMirrow,			Effects::EffectWaterMirrow, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectShapeMosaic,			Effects::EffectShapeMosaic, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectParticles,			Effects::EffectParticles, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectCanvas,				Effects::EffectCanvas, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectGlow2,				Effects::EffectGlow2, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectColorExchange,		Effects::EffectColorExchange, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectAnaglyph3d,			Effects::EffectAnaglyph3d, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectFog,	                Paint::Common::DrawFog, nTransformID, oXmlNode);

					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformFlip,				Effects::TransformFlip, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformFlip2,			Effects::TransformFlip2, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformPerspective,		Effects::TransformPerspective, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformSkew,				Effects::TransformSkew, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformShift,			Effects::TransformShift, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformRotate,			Effects::TransformRotate, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformRotateAdvanced,	Effects::TransformRotateAdvanced, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformRotateSimple,		Effects::TransformRotateSimple, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformResample,			Effects::TransformResample, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformZoom,				Effects::TransformZoom, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformZoomLinear,		Effects::TransformZoomLinear, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformMirror,			Effects::TransformMirror, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformResize,			Effects::TransformResize, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformTwirl,			Effects::TransformTwirl, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformSphere,			Effects::TransformSphere, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformCylinder,			Effects::TransformCylinder, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformWave,				Effects::TransformWave, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformRipple,			Effects::TransformRipple, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformRotateSpecial,	Effects::TransformRotateSpecial, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformLinear,			Effects::TransformLinear, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformGlassTile,		Effects::TransformGlassTile, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeTransformBrokenGlass,		Effects::TransformBrokenGlass, nTransformID, oXmlNode);

					SerializedObjectList_AddEffect_Typed(c_nSerializeFilterPrewitt,				Effects::FilterPrewitt, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFilterScharr,				Effects::FilterScharr, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFilterSobel,				Effects::FilterSobel, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFilterRoberts,				Effects::FilterRoberts, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFilterLaplace,				Effects::FilterLaplace, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFilterHipass,				Effects::FilterHipass, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFilterLowpass,				Effects::FilterLowpass, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFilterBlur,				Effects::FilterBlur, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFilterCanny,				Effects::FilterCanny, nTransformID, oXmlNode);

					SerializedObjectList_AddEffect_Typed(c_nSerializeFileLoadImage,				File::FileLoadImage, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFileLoadScreenshot,		File::FileLoadScreenshot, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFileLoadRaw,				File::FileLoadRaw, nTransformID, oXmlNode);
					
					SerializedObjectList_AddEffect_Typed(c_nSerializeFileSaveAsBmp,				File::FileSaveAsBmp, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFileSaveAsGif,				File::FileSaveAsGif, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFileSaveAsJpeg,			File::FileSaveAsJpeg, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFileSaveAsPng,				File::FileSaveAsPng, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFileSaveAsTiff,			File::FileSaveAsTiff, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFileSaveAsWmf,				File::FileSaveAsWmf, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFileSaveAsEmf,				File::FileSaveAsEmf, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFileSaveAsPcx,				File::FileSaveAsPcx, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFileSaveAsTga,				File::FileSaveAsTga, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFileSaveAsRas,				File::FileSaveAsRas, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFileSaveAsIPod,			File::FileSaveAsIPod, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFileSaveAsPsd,				File::FileSaveAsPsd, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeFileSaveAsJp2,             File::FileSaveAsJp2, nTransformID, oXmlNode);

					SerializedObjectList_AddEffect_Typed(c_nSerializePaintSetPen,					Paint::Common::SetPen, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintSetBrush,					Paint::Common::SetBrush, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintSetFont,					Paint::Common::SetFont, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintSetFormat,				Paint::Common::SetFormat, nTransformID, oXmlNode);

					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawLine,					Paint::Common::DrawLine, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawRectangle,			Paint::Common::DrawRectangle, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawEllipse,				Paint::Common::DrawEllipse, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawInvRectangle,			Paint::Common::DrawInvRectangle, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawInvEllipse,			Paint::Common::DrawInvEllipse, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawPie,					Paint::Common::DrawPie, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawSector,				Paint::Common::DrawSector, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawSimplePie,			Paint::Common::DrawSimplePie, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawSimpleSector,			Paint::Common::DrawSimpleSector, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawPolyline,				Paint::Common::DrawPolyline, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawPolygon,				Paint::Common::DrawPolygon, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawBorder,				Paint::Common::DrawBorder, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawMovObject,			Paint::Common::DrawMovObject, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawCredits,				Paint::Common::DrawCredits, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawTextSimple,			Paint::Common::DrawTextSimple, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawTextInRect,			Paint::Common::DrawTextInRect, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawImageFromFile,		Paint::Common::DrawImageFromFile, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawImageFromFileEx,		Paint::Common::DrawImageFromFileEx, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawImageFromBuffer,		Paint::Common::DrawImageFromBuffer, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawTimer,		        Paint::Common::DrawTimer, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawImageFromStorage,		Paint::Common::DrawImageFromStorage, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawFilm,                 Paint::Common::DrawFilm, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawPuzzle,               Paint::Common::DrawPuzzle, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawPath,				    Paint::Common::DrawGraphicPath, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawTextEx,			    Paint::Common::DrawTextEx, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawCountdown,	        Paint::Common::DrawCountdown, nTransformID, oXmlNode);

					SerializedObjectList_AddEffect_Typed(c_nSerializePaintTextStaticSetBrush,		Paint::TextStatic::SetBrush, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintTextStaticSetFont,		Paint::TextStatic::SetFont, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintTextStaticSetFormat,		Paint::TextStatic::SetFormat, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintTextStaticSetShadow,		Paint::TextStatic::SetShadow, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintTextStaticSetEdge,		Paint::TextStatic::SetEdge, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintTextStaticDrawInRect,		Paint::TextStatic::DrawInRect, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintTextStaticDrawOnEllipse,	Paint::TextStatic::DrawOnEllipse, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintTextStaticDrawOnPolyline,	Paint::TextStatic::DrawOnPolyline, nTransformID, oXmlNode);

					SerializedObjectList_AddEffect_Typed(c_nSerializePaintTextDynamicSetBrush,		Paint::TextDynamic::SetBrush, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintTextDynamicSetFont,		Paint::TextDynamic::SetFont, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintTextDynamicSetShadow,		Paint::TextDynamic::SetShadow, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintTextDynamicSetEdge,		Paint::TextDynamic::SetEdge, nTransformID, oXmlNode);

					SerializedObjectList_AddEffect_Typed(c_nSerializePaintTextDynamicStart,			Paint::TextDynamic::Start, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintTextDynamicStop,			Paint::TextDynamic::Stop, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintTextDynamicRender,		Paint::TextDynamic::Render, nTransformID, oXmlNode);

					SerializedObjectList_AddEffect_Typed(c_nSerializeComposeRender,					Compose::Render, nTransformID, oXmlNode);

					SerializedObjectList_AddEffect_Typed(c_nSerializePaintAnimatedText,				Paint::Text::AnimatedText, nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializeEffectWritingText,				Paint::Text::EffectWritingText, nTransformID, oXmlNode);

					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawImageFromFileAnimate,	Paint::Animations::DrawImageFromFileAnimate,	nTransformID, oXmlNode);
					SerializedObjectList_AddEffect_Typed(c_nSerializePaintDrawPathAnimate,			Paint::Animations::DrawGraphicPathAnimate,		nTransformID, oXmlNode);
				}
			}
			void Remove(int nIndex, BOOL bRemoveObject)
			{
				if (bRemoveObject)
				{
					if (m_arrEffects[nIndex] != NULL)
						delete m_arrEffects[nIndex];
				}

				m_arrEffects.RemoveAt(nIndex);
			}
			void Clear(BOOL bRemoveObjects)
			{
				if (bRemoveObjects)
				{
					for (int index = 0; index < GetCount(); ++index)
					{
						if (m_arrEffects[index] != NULL)
							delete m_arrEffects[index];
					}
				}

				m_arrEffects.RemoveAll();
			}
			void Append(SerializedObjectList& oList)
			{
				for (int index = 0; index < oList.GetCount(); ++index)
					Add(oList[index]);
			}
			
			BOOL FromXmlNode(XmlUtils::CXmlNode& oXmlNode, BOOL bMerge)
			{
				if (!m_pNamesManager)
					return FALSE;

				XmlUtils::CXmlNodes oXmlNodesTransforms;
				if (!oXmlNode.GetNodes(_T("*"), oXmlNodesTransforms))
				{
					Clear(TRUE);
					return FALSE;
				}

				// это список, который получится в результате подмены старых трансформов новыми
				SerializedObjectList oNewList; oNewList.SetNamesManager(m_pNamesManager);
				int nCountNewEffects = 0;

				int nTransformsCount = oXmlNodesTransforms.GetCount();
				for (int index1 = 0; index1 < nTransformsCount; ++index1)
				{
					// сейчас будем искать по-настоящему новые трансформы среди тех, которые пришли на вход
					XmlUtils::CXmlNode oXmlNodeTransform;
					if (!oXmlNodesTransforms.GetAt(index1, oXmlNodeTransform))
						continue;

					// для начала, надо понять, с каким трансформом мы имеем дело...
					CString strTransformName = oXmlNodeTransform.GetName();
					int nTransformID = m_pNamesManager->GetEffectIDByName(strTransformName);

					// группы обрабатываем без каких-либо проверок, т.е. вообще без кэширования
					if (nTransformID == ImageStudio::Serialize::c_nSerializeGroup) 
					{
						// обрабатываем группы: тэги групп у нас просто игнорируются - в результате получаетсяи линейный список эффектов
						SerializedObjectList oGroupList; oGroupList.SetNamesManager(m_pNamesManager);

						if (oGroupList.FromXmlNode(oXmlNodeTransform, FALSE))
						{
							oNewList.Append(oGroupList);

							// массив очищаем, но указатели не трогаем, иначе мы будем иметь invalid указатели
							oGroupList.Clear(FALSE);
						}

						continue;
					}

					// ok, похоже это не группа, а какой-то настоящий трансформ
					if (!bMerge)
					{
						// просто добавляем трансформ в список
						oNewList.Add(nTransformID, oXmlNodeTransform);
						++nCountNewEffects;
					}
					else
					{
						// берем этот новый трансформ и ищем - может мы уже ранее создавали абсолютно такой же трансформ ?
						// только надо учесть, что трансформ может входить в список только один раз.
						// нельзя допустить, чтобы один указатель входил в список дважды - иначе он дважды потом будет удален, 
						// что приведет к access violation'у

						CString strXml = _T("");
						BOOL bFoundPrevious = FALSE;

						// пытаемся найти такой же трансформ
						for (int index2 = 0; index2 < GetCount(); ++index2)
						{
							if (!GetAt(index2) || GetAt(index2)->ID != nTransformID)
								continue;

							if (strXml.GetLength() < 1)
								strXml = oXmlNodeTransform.GetXml();

							// быстро сравниваем два трансформа
							if (Strings::NotEquals(GetAt(index2)->SourceXml, strXml))
							{
								continue;
							}
							

							// похоже трансформы совпали - берем существующий, т.к. скопировать указатель быстрее, 
							// чем создавай новый трансформ и читать его настройки из xml

							oNewList.Add(GetAt(index2));

							// аккуратно зачищаем старый указатель (чтобы их не было 2х и болеее)
							SetAt(index2, NULL);

							bFoundPrevious = TRUE;
							break;
						}

						// похоже, что такого трансформа раньше не было, либо настройки у какого-то сцществующего трансформа сменились
						// придется создавать новый трансформ по входной xml
						if (!bFoundPrevious)
						{
							oNewList.Add(nTransformID, oXmlNodeTransform);
							++nCountNewEffects;
						}
					}
				}

				// проверка регистрации
				if( nCountNewEffects > 0 )
				{
					if (!CRegistratorClient::IsRegistered())
					{
						CRegistratorClient::Register();
					}
				}
				
				// теперь мы имеем готовый список с трансформами (часть из которых, вполне возможно, была просто скопирована)
				// осталось удалить ненужные трансформы и удалить временный список трансформов
				Clear(TRUE);
				Append(oNewList);
				oNewList.Clear(FALSE); // указатели не удалаем - мы их просто копируем

				return TRUE;
			}
			BOOL FromXmlString(const CString& strXml, BOOL bMerge)
			{
				if (strXml.GetLength() < 1)
				{
					Clear(TRUE);
					return TRUE;
				}

				CString strXmlCorrected = NormalizeXml(strXml);

				XmlUtils::CXmlNode oXmlNodeRoot;

				if (!oXmlNodeRoot.FromXmlString(strXmlCorrected))
					return FALSE;

				return FromXmlNode(oXmlNodeRoot, bMerge);
			}
		};
	}
}
