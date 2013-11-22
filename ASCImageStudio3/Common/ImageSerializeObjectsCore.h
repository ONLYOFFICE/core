#pragma once

namespace ImageStudio
{
	namespace Serialize
	{
		class SerializedObject
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
			virtual void InternalToXmlNodes(XmlUtils::CXmlWriter& oXmlWriter)
			{
			}
			virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
			{
			}
			
			virtual void InternalClear()
			{
			}
				
			virtual void Initialize(int nID, const CString& strNames)
			{
				ID = nID;

				int nPos1 = 0;

				int nPos2 = strNames.Find(_T('|'));

				while (nPos2 > nPos1)
				{
					CString strName = strNames;

					strName = strName.Mid(nPos1, nPos2 - nPos1);
					strName.Trim();

					if (strName.GetLength() >= 0)
						Names.Add(strName);

					nPos1 = nPos2 + 1;

					nPos2 = strNames.Find(_T('|'), nPos1);
				}

				nPos2 = strNames.GetLength();

				if (nPos2 > nPos1)
				{
					CString strName = strNames;

					strName = strName.Mid(nPos1, nPos2 - nPos1);
					strName.Trim();

					if (strName.GetLength() >= 0)
						Names.Add(strName);
				}
			}
			virtual void Initialize(const CString& strIDNames)
			{
				int nPos1 = 0;
				int nPos2 = strIDNames.Find(_T('|'));

				if (nPos2 < 0 || nPos2 >= strIDNames.GetLength())
					return;

				CString strID = strIDNames.Mid(nPos1, nPos2 - nPos1);
				strID.Trim();
				int nID = -1;
				if (1 != _stscanf(strID, _T(" %d"), &nID))
					return;

				CString strNames = strIDNames.Mid(nPos2 + 1, strIDNames.GetLength() - nPos2 - 1);

				Initialize(nID, strNames);
			}
			
		public:
			
			SerializedObject()
			{
				bUseVideoCard = FALSE;

				ID = c_nSerializeNone;

				Clear();
			}
			virtual ~SerializedObject()
			{
				Clear();
			}
			
			virtual int GetRequiredStreamSize()
			{
				return sizeof(long) + GetName().GetLength() + 1 + InternalGetRequiredStreamSize();
			}
			virtual BOOL ToStream(Streams::IStream* pStream)
			{
				if (!pStream)
					return FALSE;

				try
				{
					InternalToStream(pStream);

					return TRUE;
				}
				catch (...)
				{
				}

				return FALSE;
			}
			virtual BOOL FromStream(Streams::IStream* pStream)
			{
				Clear();

				if (!pStream)
					return FALSE;

				try
				{
					InternalFromStream(pStream);

					return TRUE;
				}
				catch (...)
				{
				}

				return FALSE;
			}
			
			virtual CString ToXmlString()
			{
				XmlUtils::CXmlWriter oXmlWriter;

				try
				{
					oXmlWriter.WriteNodeBegin(GetName(), TRUE);
					
					InternalToXmlAttributes(oXmlWriter);

					oXmlWriter.WriteString(_T(">"));

					InternalToXmlNodes(oXmlWriter);

					oXmlWriter.WriteNodeEnd(GetName());
				}
				catch (...)
				{
				}

				return oXmlWriter.GetXmlString();
			}
			virtual BOOL FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
			{
				Clear();
				
				try
				{
					InternalFromXmlNode(oXmlNode);

					return TRUE;
				}
				catch (...)
				{
				}

				return FALSE;
			}
			virtual BOOL FromXmlString(const CString& strXml)
			{
				Clear();

				XmlUtils::CXmlNode oXmlNode;

				try
				{
					if (!oXmlNode.FromXmlString(strXml))
						return FALSE;

					CString strName = oXmlNode.GetName();

					if (!IsNameValid(strName))
						return FALSE;

					InternalFromXmlNode(oXmlNode);

					return TRUE;
				}
				catch (...)
				{
				}

				return FALSE;
			}
			
			virtual void Clear()
			{
				try
				{
					CreateSession();

					InternalClear();
				}
				catch (...)
				{
				}
			}
			
			virtual CString GetName()
			{
				if (Names.GetSize() < 1)
					return _T("");

				return Names[0];
			}
			virtual BOOL IsNameValid(const CString& strName)
			{
				for (int index = 0; index < Names.GetSize(); ++index)
				{
					if (Names[index] == strName)
						return TRUE;
				}

				return FALSE;
			}
			
			int GetSessionID() const
			{
				return m_nSessionID;
			}
			void SetSessionID(int nID)
			{
				m_nSessionID = nID;
			}
			void CreateSession()
			{
				static int g_nSessionCounter = 1;

				m_nSessionID = g_nSessionCounter;

				g_nSessionCounter++;
			}
			
		public:
			
			int ID; // идентификатор эффекта (это значение должно быть выставлено в конструкторе объекта)
			
			CSimpleArray<CString> Names; // names of the object and the name of the XML item (this value shoud be set in child constructor)

			CString SourceXml;

			BOOL bUseVideoCard;

		protected:
			
			int m_nSessionID; // уникальный идентификатор эффекта - GetTickCount() (нужен для связи эффекта и кэша его параметров)
		};
		class SerializedGroup : public SerializedObject
		{
		protected:
						
			virtual int InternalGetRequiredStreamSize()
			{
				return sizeof(int);
			}
			virtual void InternalToStream(Streams::IStream* pStream)
			{
				pStream->WriteLong(GroupIndex);
			}
			virtual void InternalFromStream(Streams::IStream* pStream)
			{
				GroupIndex = pStream->ReadLong();
			}
			
			virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
			{
				oXmlWriter.WriteAttribute(_T("index"), GroupIndex);
			}
			virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
			{
				GroupIndex = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("index"), _T("0")));
			}
			
			virtual void InternalClear()
			{
				GroupIndex = -1;
			}
			
		public:
			
			SerializedGroup()
			{
				Initialize(c_nSerializeGroup, c_strSerializeGroup);

				Clear();
			}
				
		public:
			
			int GroupIndex; // index of the group in external group list
		};
		class SerializedBufferClear : public SerializedObject
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
			
			SerializedBufferClear()
			{
				Initialize(c_nSerializeBufferClear, c_strSerializeBufferClear);

				Clear();
			}
				
		public:
			
			int Index; // индекс буффера который надо очистить
		};
		class SerializedBufferCopy : public SerializedObject
		{
		protected:
						
			virtual int InternalGetRequiredStreamSize()
			{
				return 2*sizeof(int);
			}
			virtual void InternalToStream(Streams::IStream* pStream)
			{
				pStream->WriteLong(IndexFrom);
				pStream->WriteLong(IndexTo);
			}
			virtual void InternalFromStream(Streams::IStream* pStream)
			{
				IndexFrom = pStream->ReadLong();
				IndexTo = pStream->ReadLong();
			}
			
			virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
			{
				oXmlWriter.WriteAttribute(_T("index-from"), IndexFrom);
				oXmlWriter.WriteAttribute(_T("index-to"), IndexTo);
			}
			virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
			{
				IndexFrom = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("index-from"), _T("-1")));
				IndexTo = Strings::ToInteger(oXmlNode.GetAttributeOrValue(_T("index-to"), _T("-1")));
			}
			
			virtual void InternalClear()
			{
				IndexFrom = -1;
				IndexTo = -1;
			}
			
		public:
			
			SerializedBufferCopy()
			{
				Initialize(c_nSerializeBufferCopy, c_strSerializeBufferCopy);

				Clear();
			}
				
		public:
			
			int IndexFrom; // индекс буффера из которого скопировать интерфейс
			int IndexTo; // индекс буффера в который скопировать интерфейс
		};

		class SerializedSetParamsUseVideoCard : public SerializedObject
		{
		protected:
			virtual void InternalToXmlAttributes(XmlUtils::CXmlWriter& oXmlWriter)
			{
				oXmlWriter.WriteAttribute(_T("usetype"), nUseType);
				oXmlWriter.WriteAttribute(_T("interpolation"), nInterpolation);
			}
			virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode)
			{
				nUseType = Strings::ToInteger(oXmlNode.GetAttribute(_T("usetype"), _T("2")));
				nInterpolation = Strings::ToInteger(oXmlNode.GetAttribute(_T("interpolation"), _T("2")));
			}
			
			virtual void InternalClear()
			{
				nUseType   = 2; // всегда использовать видеокарту
				nInterpolation = 2; // билинейная
			}
			
		public:
			
			SerializedSetParamsUseVideoCard()
			{
				Initialize(c_nSerializeSetParamsUseVideoCard, c_strSerializeSetParamsUseVideoCard);

				Clear();

				bUseVideoCard = FALSE;
			}
				
		public:
			
			int nUseType; // 0 - не использовать, 1 - совместное использование, 2 - монопольное использование
			int nInterpolation; // тип интерполяции по умолчанию для эффектов: 0 - максимально возможная
		};
		
		namespace Core
		{
			class EffectTimeline
			{
			public:

				const static int c_nTypeApplyAlways = 1;			// тип, при котором эффект применяется всегда, когда completeness > 0.0001
				const static int c_nTypeApplyOnceWaiting = 1001;	// тип, при котором эффект применяется только один раз, когда completeness > 0.0001, после этого, статус эффекта меняется на c_nTypeApplyOnceApplied
				const static int c_nTypeApplyOnceApplied = 1002;
				
			protected:

				struct STimeRange
				{
					// параметр, отвечающий, как часто надо вызывать эффект (см. c_nTypeApply* const): 
					//      1 - эффект надо применять постоянно при значении completeness > 0 
					//   1001 - означает, что эффект должен быть вызван один раз при достижении значения completeness > 0
					//   1002 - означает, что эффект уже один раз был вызван, и теперь его не надо вызывать (при достижении completeness 0 - опять сбросить)
					int Type;

					double Begin; // time of the beginning of the effect
					double End;	// time of the end of the effect
					double FadeIn; // time of the fade in
					double FadeOut; // time of the fade out
					double Completeness; // maximal completeness for the range. the value is computed for each frame separately: [0..1] - valid range
				};
				
				CSimpleArray<STimeRange> m_arrRanges;
				
			public:
				
				EffectTimeline()
				{
					Clear();
				}
				virtual ~EffectTimeline()
				{
					Clear();
				}
				
				virtual int GetRequiredStreamSize()
				{
					return m_arrRanges.GetSize()*sizeof(STimeRange);
				}
				virtual BOOL ToStream(Streams::IStream* pStream)
				{
					if (!pStream)
						return FALSE;

					pStream->WriteLong(m_arrRanges.GetSize());

					for (int index = 0; index < m_arrRanges.GetSize(); ++index)
					{
						pStream->WriteLong(m_arrRanges[index].Type);
						pStream->WriteDouble(m_arrRanges[index].Begin);
						pStream->WriteDouble(m_arrRanges[index].End);
						pStream->WriteDouble(m_arrRanges[index].FadeIn);
						pStream->WriteDouble(m_arrRanges[index].FadeOut);
						pStream->WriteDouble(m_arrRanges[index].Completeness);
					}

					return TRUE;
				}
				virtual BOOL FromStream(Streams::IStream* pStream)
				{
					Clear();

					if (!pStream)
						return FALSE;

					// зачитываем все timeline зоны..
					int nCount = pStream->ReadLong();

					for (int index = 0; index < nCount; ++index)
					{
						STimeRange oRange;

						oRange.Type = pStream->ReadLong();
						oRange.Begin = pStream->ReadDouble();
						oRange.End = pStream->ReadDouble();
						oRange.FadeIn = pStream->ReadDouble();
						oRange.FadeOut = pStream->ReadDouble();
						oRange.Completeness = pStream->ReadDouble();

						m_arrRanges.Add(oRange);
					}

					return TRUE;
				}
				
				virtual CString ToXmlString()
				{
					XmlUtils::CXmlWriter oXmlWriter;

					for (int index = 0; index < m_arrRanges.GetSize(); ++index)
					{
						oXmlWriter.WriteNodeBegin(_T("timeline"), TRUE);
						oXmlWriter.WriteNode(_T("type"), m_arrRanges[index].Type);
						oXmlWriter.WriteNode(_T("begin"), m_arrRanges[index].Begin);
						oXmlWriter.WriteNode(_T("end"), m_arrRanges[index].End);
						oXmlWriter.WriteNode(_T("fadein"), m_arrRanges[index].FadeIn);
						oXmlWriter.WriteNode(_T("fadeout"), m_arrRanges[index].FadeOut);
						oXmlWriter.WriteNode(_T("completeness"), m_arrRanges[index].Completeness);
						oXmlWriter.WriteNodeEnd(_T("timeline"), TRUE);
					}

					return oXmlWriter.GetXmlString();
				}
				virtual BOOL FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Clear();
					
					try
					{
						XmlUtils::CXmlNodes oXmlNodes;
						if (!oXmlNode.GetNodes(_T("timeline"), oXmlNodes))
							return FALSE;

						int nTimelines = oXmlNodes.GetCount();
						for (int nTimeline = 0; nTimeline < nTimelines; ++nTimeline)
						{
							XmlUtils::CXmlNode oXmlNodeItem;
							if (oXmlNodes.GetAt(nTimeline, oXmlNodeItem))
							{
								STimeRange oRange;

								oRange.Type = Strings::ToInteger(oXmlNodeItem.GetAttributeOrValue(_T("type"), _T("1")));
								oRange.Begin = Strings::ToDouble(oXmlNodeItem.GetAttributeOrValue(_T("begin"), _T("-1")));
								oRange.End = Strings::ToDouble(oXmlNodeItem.GetAttributeOrValue(_T("end"), _T("-1")));
								oRange.FadeIn = Strings::ToDouble(oXmlNodeItem.GetAttributeOrValue(_T("fadein"), _T("0")));
								oRange.FadeOut = Strings::ToDouble(oXmlNodeItem.GetAttributeOrValue(_T("fadeout"), _T("0")));
								oRange.Completeness = Strings::ToDouble(oXmlNodeItem.GetAttributeOrValue(_T("completeness"), _T("1")));

								m_arrRanges.Add(oRange);
							}
						}

						return TRUE;
					}
					catch (...)
					{
					}

					return FALSE;
				}
				virtual BOOL FromXmlString(const CString& strXml)
				{
					Clear();

					if (strXml.GetLength() < 1)
						return TRUE;

					XmlUtils::CXmlNode oXmlNode;

					if (!oXmlNode.FromXmlString(strXml))
						return FALSE;

					return FromXmlNode(oXmlNode);
				}
				
				virtual void Clear()
				{
					m_arrRanges.RemoveAll();
				}
				virtual BOOL GetCompleteness(double dTime, double& dResultCompleteness)
				{
					// если таймлайна нет - то эффект применяется всегда
					if (m_arrRanges.GetSize() < 1)
					{
						dResultCompleteness = 1.0;
						return TRUE;
					}

					// если таймлайн есть, то надо проверить - не попадает ли текущее время в какой-либо интервал
					BOOL bTypeUpdated = FALSE;

					for (int index = 0; index < m_arrRanges.GetSize(); ++index)
					{
						if (dTime < m_arrRanges[index].Begin || dTime > m_arrRanges[index].End)
						{
							if (m_arrRanges[index].Type == c_nTypeApplyOnceApplied)
							{
								m_arrRanges[index].Type = c_nTypeApplyOnceWaiting;
								bTypeUpdated = TRUE;
							}

							continue;
						}

						if (m_arrRanges[index].Completeness < 0.0001)
							dResultCompleteness = 0.0;
						else
						{
							double dTime0 = m_arrRanges[index].Begin;
							double dTime3 = m_arrRanges[index].End;
							double dTime1 = dTime0 + m_arrRanges[index].FadeIn;
							double dTime2 = dTime3 - m_arrRanges[index].FadeOut;

							double dCompleteness = 0.0;

							if (dTime >= dTime1 && dTime <= dTime2)
								dCompleteness = 1.0;
							else if (dTime >= dTime0 && dTime <= dTime1)
								dCompleteness = (dTime - dTime0)/max(0.0001, dTime1 - dTime0);
							else if (dTime > dTime2 && dTime <= dTime3)
								dCompleteness = (dTime3 - dTime)/max(0.0001, dTime3 - dTime2);

							dResultCompleteness = m_arrRanges[index].Completeness*dCompleteness;
						}

						// попали в интервал - надо обновить статус у сложного интервала.. (если он, конечно, таковой)
						if (dResultCompleteness >= 0.0001)
						{
							if (m_arrRanges[index].Type == c_nTypeApplyOnceWaiting)
							{
								m_arrRanges[index].Type = c_nTypeApplyOnceApplied;
								dResultCompleteness = 1.0;
								bTypeUpdated = TRUE;
							}
							else if (m_arrRanges[index].Type == c_nTypeApplyOnceApplied)
							{
								dResultCompleteness = 0.0;
							}
						}

						return bTypeUpdated;
					}

					// похоже ни в один интервал не попало - ну и ладно
					dResultCompleteness = 0.0;

					return bTypeUpdated;
				}
				virtual BOOL GetCompletenessStage(double dTime, double& dResultCompleteness)
				{
					// возвращает в dResultCompleteness параметр завершенности с учетом стадии эффекта, т.е. значение от 0 до 3
					// 0..1 - начало эффекта
					// 1..2 - эффект идет
					// 2..3 - конец эффекта

					// если таймлайна нет - то эффект применяется всегда
					if (m_arrRanges.GetSize() < 1)
					{
						dResultCompleteness = 1.5;
						return TRUE;
					}

					// если таймлайн есть, то надо проверить - не попадает ли текущее время в какой-либо интервал
					for (int index = 0; index < m_arrRanges.GetSize(); ++index)
					{
						if (dTime < m_arrRanges[index].Begin || dTime > m_arrRanges[index].End)
							continue;

						if (m_arrRanges[index].Completeness < 0.0001)
							dResultCompleteness = 0.0;
						else
						{
							double dTime0 = m_arrRanges[index].Begin;
							double dTime3 = m_arrRanges[index].End;
							double dTime1 = dTime0 + m_arrRanges[index].FadeIn;
							double dTime2 = dTime3 - m_arrRanges[index].FadeOut;

							double dCompleteness = 0.0;

							if (dTime >= dTime0 && dTime < dTime1)
								dCompleteness = 0.0 + (dTime - dTime0)/max(0.0001, dTime1 - dTime0);
							else if (dTime >= dTime1 && dTime < dTime2)
								dCompleteness = 1.0 + (dTime - dTime1)/max(0.0001, dTime2 - dTime1);
							else if (dTime >= dTime2 && dTime <= dTime3)
								dCompleteness = 2.0 + (dTime - dTime2)/max(0.0001, dTime3 - dTime2);

							dResultCompleteness = m_arrRanges[index].Completeness*dCompleteness;

							return TRUE;
						}
					}

					// похоже ни в один интервал не попало - ну и ладно
					dResultCompleteness = 0.0;

					return TRUE;
				}
				virtual BOOL GetRelativeTime( double dTime, double& dResultTime )
				{
					// возвращает в dResultTime время относительно начала эффекта, 
					// если время выходит за границы эффекта, то возвращается краевое значение

					// если таймлайна нет - то эффект применяется всегда
					if (m_arrRanges.GetSize() < 1)
					{
						dResultTime = 0;
						return TRUE;
					}

					// если таймлайн есть, то надо проверить - не попадает ли текущее время в какой-либо интервал
					for (int index = 0; index < m_arrRanges.GetSize(); ++index)
					{
						if (dTime < m_arrRanges[index].Begin || dTime > m_arrRanges[index].End)
							continue;

						if (m_arrRanges[index].Completeness > 0)
						{
							double dTime0 = m_arrRanges[index].Begin;
							double dTime3 = m_arrRanges[index].End;

							if( dTime < dTime0 )
								dResultTime = 0;
							else if( dTime > dTime3 )
								dResultTime = dTime3 - dTime0;
							else
								dResultTime = dTime - dTime0;

							return TRUE;
						}
					}

					// похоже ни в один интервал не попало - ну и ладно
					dResultTime = 0;

					return TRUE;
				}

				virtual BOOL GetTimeLength( double dTime, double& dResultTime )
				{
					// возвращает в dResultTime общее время эффекта,

					if (m_arrRanges.GetSize() < 1)
					{
						dResultTime = 0;
						return TRUE;
					}

					// если таймлайн есть, то надо проверить - не попадает ли текущее время в какой-либо интервал
					for (int index = 0; index < m_arrRanges.GetSize(); ++index)
					{
						if (dTime < m_arrRanges[index].Begin || dTime > m_arrRanges[index].End)
							continue;

						if (m_arrRanges[index].Completeness > 0)
						{
							dResultTime = m_arrRanges[index].End - m_arrRanges[index].Begin;

							return TRUE;
						}
					}

					// похоже ни в один интервал не попало - ну и ладно
					dResultTime = 0;

					return TRUE;
				}
			};

			class Effect : public SerializedObject
			{
			public:
				
				Effect()
				{
					ID = c_nSerializeNone;
					SourceId = 0;
					BaseRandom = -1;

					Clear();
				}
				
				virtual int GetRequiredStreamSize()
				{
					return sizeof(long) + GetName().GetLength() + 1 + TimeLine.GetRequiredStreamSize() + InternalGetRequiredStreamSize();
				}
				virtual BOOL ToStream(Streams::IStream* pStream)
				{
					if (!pStream)
						return FALSE;

					try
					{
						if (!TimeLine.ToStream(pStream))
							return FALSE;

						InternalToStream(pStream);

						return TRUE;
					}
					catch (...)
					{
					}

					return FALSE;
				}
				virtual BOOL FromStream(Streams::IStream* pStream)
				{
					Clear();

					if (!pStream)
						return FALSE;

					try
					{
						if (!TimeLine.FromStream(pStream))
							return FALSE;

						InternalFromStream(pStream);

						return TRUE;
					}
					catch (...)
					{
					}

					return FALSE;
				}
				
				virtual CString ToXmlString()
				{
					XmlUtils::CXmlWriter oXmlWriter;

					try
					{
						oXmlWriter.WriteNodeBegin(GetName(), TRUE);

						oXmlWriter.WriteAttribute(_T("sourceid"), SourceId);
						oXmlWriter.WriteAttribute(_T("random"), BaseRandom);

						InternalToXmlAttributes(oXmlWriter);

						oXmlWriter.WriteString(_T(">"));

						InternalToXmlNodes(oXmlWriter);

						oXmlWriter.WriteString(TimeLine.ToXmlString());
						
						oXmlWriter.WriteNodeEnd(GetName());
					}
					catch (...)
					{
					}

					return oXmlWriter.GetXmlString();
				}
				virtual BOOL FromXmlNode(XmlUtils::CXmlNode& oXmlNode)
				{
					Clear();
					
					try
					{
						if (!TimeLine.FromXmlNode(oXmlNode))
							return FALSE;

						SourceId = Strings::ToInteger(oXmlNode.GetAttribute(_T("sourceid"), _T("0")));
						BaseRandom = Strings::ToInteger(oXmlNode.GetAttribute(_T("randomnumber"), _T("-1")));

						InternalFromXmlNode(oXmlNode);

						SourceXml = oXmlNode.GetXml();

						return TRUE;
					}
					catch (...)
					{
					}

					return FALSE;
				}
				virtual BOOL FromXmlString(const CString& strXml)
				{
					Clear();

					XmlUtils::CXmlNode oXmlNode;

					try
					{
						if (!oXmlNode.FromXmlString(strXml))
							return FALSE;

						CString strName = oXmlNode.GetName();

						if (!IsNameValid(strName))
							return FALSE;

						if (!TimeLine.FromXmlNode(oXmlNode))
							return FALSE;

						InternalFromXmlNode(oXmlNode);

						SourceXml = strXml;

						return TRUE;
					}
					catch (...)
					{
					}

					return FALSE;
				}
				
				virtual void Clear()
				{
					try
					{
						CreateSession();

						SourceXml = _T("");

						TimeLine.Clear();

						SourceId = 0;
						BaseRandom = -1;
						
						InternalClear();
					}
					catch (...)
					{
					}
				}
				virtual double GetCompleteness(double dTime)
				{
					double dCompleteness = 0.0;

					if (TimeLine.GetCompleteness(dTime, dCompleteness))
					{
						// TODO: timeline сам обновил свой статус... если что-то надо делать в этот момент, то код вставлять прямо сюда
					}

					return dCompleteness;
				}
				virtual double GetCompletenessStage(double dTime)
				{
					double dCompleteness = 0.0;

					TimeLine.GetCompletenessStage(dTime, dCompleteness);

					return dCompleteness;
				}
				
				virtual double GetRelativeTime(double dTime)
				{
					double dResultTime = 0;
					
					TimeLine.GetRelativeTime( dTime, dResultTime );

					return dResultTime;
				}
				virtual double GetTimeLength( double dTime )
				{
					double dResultTime = 0;

					TimeLine.GetTimeLength( dTime, dResultTime );
					
					return dResultTime;
				}
				
			public:
				
				EffectTimeline TimeLine;

				int SourceId;
				int BaseRandom;
			};

			class AdditionalParams : public SerializedObject
			{
			public:

				AdditionalParams()
				{
					Initialize(c_nSerializeSetAdditionalParams, c_strSerializeSetAdditionalParams);

					Clear();
				}
			};
		}
	}
}