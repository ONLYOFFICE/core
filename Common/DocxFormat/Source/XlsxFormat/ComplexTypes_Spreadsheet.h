#pragma once
#include "../Common/ComplexTypes.h"

#include "SimpleTypes_Spreadsheet.h"

namespace ComplexTypes
{
	namespace Spreadsheet
	{
			template<SimpleTypes::EOnOff eDefValue = SimpleTypes::onoffTrue>
			class COnOff2 : public ComplexType
			{
			public:
				ComplexTypes_AdditionConstructors(COnOff2)
					COnOff2()
				{
				}
				virtual ~COnOff2()
				{
				}

				virtual void    FromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					// Читаем атрибуты
					WritingElement_ReadAttributes_Start( oReader )
						WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
						WritingElement_ReadAttributes_End( oReader )
				}
				virtual CString ToString() const
				{
					CString sResult;

					sResult += "val=\"";
					sResult += m_oVal.ToString();
					sResult += "\" ";

					return sResult;
				}
				void FromBool(bool bVal)
				{
					m_oVal.FromBool(bVal);
				}
				
			public:

				SimpleTypes::COnOff<eDefValue> m_oVal;

			};

			class CDecimalNumber : public ComplexType
			{
			public:
				ComplexTypes_AdditionConstructors(CDecimalNumber)
				CDecimalNumber()
				{
				}
				virtual ~CDecimalNumber()
				{
				}

				virtual void    FromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes(oReader);

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}
				virtual CString ToString() const
				{
					CString sResult;

					if ( m_oVal.IsInit() )
					{
						sResult += "val=\"";
						sResult += m_oVal->ToString();
						sResult += "\" ";
					}

					return sResult;
				}
			private:

				void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
				{
					// Читаем атрибуты
					WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
					WritingElement_ReadAttributes_End( oReader )
				}
			public:

				nullable<SimpleTypes::CDecimalNumber<> > m_oVal;
			};
			class CString_ : public ComplexType
			{
			public:
				ComplexTypes_AdditionConstructors(CString_)
					CString_()
				{
				}
				virtual ~CString_()
				{
				}

				virtual void    FromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_sVal );
				}
				virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes(oReader);

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}
				virtual CString ToString() const
				{
					CString sResult;

					if ( m_sVal.IsInit() )
					{
						sResult += "val=\"";
						sResult += m_sVal->GetString();
						sResult += "\" ";
					}

					return sResult;
				}
				CString ToString2() const
				{
					CString sResult;

					if ( m_sVal.IsInit() )
						sResult += m_sVal.get();

					return sResult;
				}
			private:

				void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
				{
					// Читаем атрибуты
					WritingElement_ReadAttributes_Start( oReader )
						WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_sVal )
						WritingElement_ReadAttributes_End( oReader )
				}

			public:

				nullable<CString> m_sVal;
			};
			class CDouble : public ComplexType
			{
			public:
				ComplexTypes_AdditionConstructors(CDouble)
					CDouble()
				{
				}
				virtual ~CDouble()
				{
				}

				virtual void    FromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes(oReader);

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}
				virtual CString ToString() const
				{
					CString sResult;

					if ( m_oVal.IsInit() )
					{
						sResult += "val=\"";
						sResult += m_oVal->ToString();
						sResult += "\" ";
					}

					return sResult;
				}
			private:

				void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
				{
					// Читаем атрибуты
					WritingElement_ReadAttributes_Start( oReader )
						WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
						WritingElement_ReadAttributes_End( oReader )
				}

			public:

				nullable<SimpleTypes::CDouble > m_oVal;
			};
			class CPointMeasure : public ComplexType
			{
			public:
				ComplexTypes_AdditionConstructors(CPointMeasure)
					CPointMeasure()
				{
				}
				virtual ~CPointMeasure()
				{
				}

				virtual void    FromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes(oReader);

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}
				virtual CString ToString() const
				{
					CString sResult;

					if ( m_oVal.IsInit() )
					{
						sResult += "val=\"";
						sResult += m_oVal->ToString();
						sResult += "\" ";
					}

					return sResult;
				}
			private:

				void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
				{
					// Читаем атрибуты
					WritingElement_ReadAttributes_Start( oReader )
						WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
						WritingElement_ReadAttributes_End( oReader )
				}

			public:

				nullable<SimpleTypes::CPointMeasure<> > m_oVal;
			};
			class CChartLayoutTarget : public ComplexType
			{
			public:
				ComplexTypes_AdditionConstructors(CChartLayoutTarget)
					CChartLayoutTarget()
				{
				}
				virtual ~CChartLayoutTarget()
				{
				}

				virtual void    FromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes(oReader);

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}
				virtual CString ToString() const
				{
					CString sResult;

					if ( m_oVal.IsInit() )
					{
						sResult += "val=\"";
						sResult += m_oVal->ToString();
						sResult += "\" ";
					}

					return sResult;
				}
			private:

				void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
				{
					// Читаем атрибуты
					WritingElement_ReadAttributes_Start( oReader )
						WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
						WritingElement_ReadAttributes_End( oReader )
				}

			public:

				nullable<SimpleTypes::Spreadsheet::CChartLayoutTarget<> > m_oVal;
			};
			class CChartHMode : public ComplexType
			{
			public:
				ComplexTypes_AdditionConstructors(CChartHMode)
					CChartHMode()
				{
				}
				virtual ~CChartHMode()
				{
				}

				virtual void    FromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes(oReader);

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}
				virtual CString ToString() const
				{
					CString sResult;

					if ( m_oVal.IsInit() )
					{
						sResult += "val=\"";
						sResult += m_oVal->ToString();
						sResult += "\" ";
					}

					return sResult;
				}
			private:

				void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
				{
					// Читаем атрибуты
					WritingElement_ReadAttributes_Start( oReader )
						WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
						WritingElement_ReadAttributes_End( oReader )
				}

			public:

				nullable<SimpleTypes::Spreadsheet::CChartHMode<> > m_oVal;
			};
			class CChartAxPos : public ComplexType
			{
			public:
				ComplexTypes_AdditionConstructors(CChartAxPos)
					CChartAxPos()
				{
				}
				virtual ~CChartAxPos()
				{
				}

				virtual void    FromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes(oReader);

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}
				virtual CString ToString() const
				{
					CString sResult;

					if ( m_oVal.IsInit() )
					{
						sResult += "val=\"";
						sResult += m_oVal->ToString();
						sResult += "\" ";
					}

					return sResult;
				}
			private:

				void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
				{
					// Читаем атрибуты
					WritingElement_ReadAttributes_Start( oReader )
						WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
						WritingElement_ReadAttributes_End( oReader )
				}

			public:

				nullable<SimpleTypes::Spreadsheet::CChartAxPos<> > m_oVal;
			};
			class CChartBarGrouping : public ComplexType
			{
			public:
				ComplexTypes_AdditionConstructors(CChartBarGrouping)
					CChartBarGrouping()
				{
				}
				virtual ~CChartBarGrouping()
				{
				}

				virtual void    FromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes(oReader);

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}
				virtual CString ToString() const
				{
					CString sResult;

					if ( m_oVal.IsInit() )
					{
						sResult += "val=\"";
						sResult += m_oVal->ToString();
						sResult += "\" ";
					}

					return sResult;
				}
			private:

				void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
				{
					// Читаем атрибуты
					WritingElement_ReadAttributes_Start( oReader )
						WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
						WritingElement_ReadAttributes_End( oReader )
				}

			public:

				nullable<SimpleTypes::Spreadsheet::CChartBarGrouping<> > m_oVal;
			};
			class CChartBarDerection : public ComplexType
			{
			public:
				ComplexTypes_AdditionConstructors(CChartBarDerection)
					CChartBarDerection()
				{
				}
				virtual ~CChartBarDerection()
				{
				}

				virtual void    FromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes(oReader);

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}
				virtual CString ToString() const
				{
					CString sResult;

					if ( m_oVal.IsInit() )
					{
						sResult += "val=\"";
						sResult += m_oVal->ToString();
						sResult += "\" ";
					}

					return sResult;
				}
			private:

				void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
				{
					// Читаем атрибуты
					WritingElement_ReadAttributes_Start( oReader )
						WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
						WritingElement_ReadAttributes_End( oReader )
				}

			public:

				nullable<SimpleTypes::Spreadsheet::CChartBarDerection<> > m_oVal;
			};
			class CChartSymbol : public ComplexType
			{
			public:
				ComplexTypes_AdditionConstructors(CChartSymbol)
					CChartSymbol()
				{
				}
				virtual ~CChartSymbol()
				{
				}

				virtual void    FromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes(oReader);

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}
				virtual CString ToString() const
				{
					CString sResult;

					if ( m_oVal.IsInit() )
					{
						sResult += "val=\"";
						sResult += m_oVal->ToString();
						sResult += "\" ";
					}

					return sResult;
				}
			private:

				void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
				{
					// Читаем атрибуты
					WritingElement_ReadAttributes_Start( oReader )
						WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
						WritingElement_ReadAttributes_End( oReader )
				}

			public:

				nullable<SimpleTypes::Spreadsheet::CChartSymbol<> > m_oVal;
			};
	}
} // ComplexTypes
