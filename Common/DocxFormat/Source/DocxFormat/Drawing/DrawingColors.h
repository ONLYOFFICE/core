#pragma once
#ifndef OOX_LOGIC_DRAWING_COLORS_INCLUDE_H_
#define OOX_LOGIC_DRAWING_COLORS_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

#include "../WritingElement.h"
#include "../RId.h"

#include <algorithm>

namespace OOX
{
	namespace Drawing
	{
		namespace Colors
		{
			//-----------------------------------------------------------------------
			// CAlphaTransform 20.1.2.3.1
			//-----------------------------------------------------------------------
			class CAlphaTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CAlphaTransform)
				CAlphaTransform()
				{
				}
				virtual ~CAlphaTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:alpha val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_alpha;
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

				SimpleTypes::CPositiveFixedPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CAlphaModTransform 20.1.2.3.2
			//-----------------------------------------------------------------------
			class CAlphaModTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CAlphaModTransform)
				CAlphaModTransform()
				{
				}
				virtual ~CAlphaModTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:alphaMod val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_alphaMod;
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

				SimpleTypes::CPositivePercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CAlphaOffTransform 20.1.2.3.3
			//-----------------------------------------------------------------------
			class CAlphaOffTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CAlphaOffTransform)
				CAlphaOffTransform()
				{
				}
				virtual ~CAlphaOffTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:alphaOff val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_alphaOff;
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

				SimpleTypes::CFixedPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CBlueTransform 20.1.2.3.4
			//-----------------------------------------------------------------------
			class CBlueTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CBlueTransform)
				CBlueTransform()
				{
				}
				virtual ~CBlueTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:blue val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_blue;
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

				SimpleTypes::CPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CBlueModTransform 20.1.2.3.5
			//-----------------------------------------------------------------------
			class CBlueModTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CBlueModTransform)
				CBlueModTransform()
				{
				}
				virtual ~CBlueModTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:blueMod val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_blueMod;
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

				SimpleTypes::CPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CBlueOffTransform 20.1.2.3.6
			//-----------------------------------------------------------------------
			class CBlueOffTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CBlueOffTransform)
				CBlueOffTransform()
				{
				}
				virtual ~CBlueOffTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:blueOff val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_blueOff;
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

				SimpleTypes::CPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CComplementTransform 20.1.2.3.7
			//-----------------------------------------------------------------------
			class CComplementTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CComplementTransform)
				CComplementTransform()
				{
				}
				virtual ~CComplementTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					return _T("<a:comp/>");
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_comp;
				}

			};

			//-----------------------------------------------------------------------
			// CGammaTransform 20.1.2.3.8
			//-----------------------------------------------------------------------
			class CGammaTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CGammaTransform)
				CGammaTransform()
				{
				}
				virtual ~CGammaTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					return _T("<a:gamma/>");
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_gamma;
				}

			};

			//-----------------------------------------------------------------------
			// CGrayscaleTransform 20.1.2.3.9
			//-----------------------------------------------------------------------
			class CGrayscaleTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CGrayscaleTransform)
				CGrayscaleTransform()
				{
				}
				virtual ~CGrayscaleTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					return _T("<a:gray/>");
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_gray;
				}

			};

			//-----------------------------------------------------------------------
			// CGreenTransform 20.1.2.3.10
			//-----------------------------------------------------------------------
			class CGreenTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CGreenTransform)
				CGreenTransform()
				{
				}
				virtual ~CGreenTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:green val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_green;
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

				SimpleTypes::CPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CGreenModTransform 20.1.2.3.11
			//-----------------------------------------------------------------------
			class CGreenModTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CGreenModTransform)
				CGreenModTransform()
				{
				}
				virtual ~CGreenModTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:greenMod val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_greenMod;
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

				SimpleTypes::CPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CGreenOffTransform 20.1.2.3.12
			//-----------------------------------------------------------------------
			class CGreenOffTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CGreenOffTransform)
				CGreenOffTransform()
				{
				}
				virtual ~CGreenOffTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:greenOff val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_greenOff;
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

				SimpleTypes::CPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CHueTransform 20.1.2.3.14
			//-----------------------------------------------------------------------
			class CHueTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CHueTransform)
				CHueTransform()
				{
				}
				virtual ~CHueTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:hue val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_hue;
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

				SimpleTypes::CPositiveFixedAngle<> m_oVal;

			};

			//-----------------------------------------------------------------------
			// CHueModTransform 20.1.2.3.15
			//-----------------------------------------------------------------------
			class CHueModTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CHueModTransform)
				CHueModTransform()
				{
				}
				virtual ~CHueModTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:hueMod val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_hueMod;
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

				SimpleTypes::CPositivePercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CHueOffTransform 20.1.2.3.16
			//-----------------------------------------------------------------------
			class CHueOffTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CHueOffTransform)
				CHueOffTransform()
				{
				}
				virtual ~CHueOffTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:hueOff val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_hueOff;
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

				SimpleTypes::CAngle<> m_oVal;

			};

			//-----------------------------------------------------------------------
			// CInverseTransform 20.1.2.3.17
			//-----------------------------------------------------------------------
			class CInverseTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CInverseTransform)
				CInverseTransform()
				{
				}
				virtual ~CInverseTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					return _T("<a:inv/>");
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_inv;
				}

			};

			//-----------------------------------------------------------------------
			// CInverseGammaTransform 20.1.2.3.18
			//-----------------------------------------------------------------------
			class CInverseGammaTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CInverseGammaTransform)
				CInverseGammaTransform()
				{
				}
				virtual ~CInverseGammaTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					return _T("<a:invGamma/>");
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_invGamma;
				}

			};

			//-----------------------------------------------------------------------
			// CLumTransform 20.1.2.3.19
			//-----------------------------------------------------------------------
			class CLumTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CLumTransform)
				CLumTransform()
				{
				}
				virtual ~CLumTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:lum val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_lum;
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

				SimpleTypes::CPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CLumModTransform 20.1.2.3.20
			//-----------------------------------------------------------------------
			class CLumModTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CLumModTransform)
				CLumModTransform()
				{
				}
				virtual ~CLumModTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:lumMod val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_lumMod;
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

				SimpleTypes::CPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CLumOffTransform 20.1.2.3.21
			//-----------------------------------------------------------------------
			class CLumOffTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CLumOffTransform)
				CLumOffTransform()
				{
				}
				virtual ~CLumOffTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:lumOff val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_lumOff;
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

				SimpleTypes::CPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CRedTransform 20.1.2.3.23
			//-----------------------------------------------------------------------
			class CRedTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CRedTransform)
				CRedTransform()
				{
				}
				virtual ~CRedTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:red val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_red;
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

				SimpleTypes::CPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CRedModTransform 20.1.2.3.24
			//-----------------------------------------------------------------------
			class CRedModTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CRedModTransform)
				CRedModTransform()
				{
				}
				virtual ~CRedModTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:redMod val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_redMod;
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

				SimpleTypes::CPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CRedOffTransform 20.1.2.3.25
			//-----------------------------------------------------------------------
			class CRedOffTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CRedOffTransform)
				CRedOffTransform()
				{
				}
				virtual ~CRedOffTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:redOff val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_redOff;
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

				SimpleTypes::CPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CSatTransform 20.1.2.3.26
			//-----------------------------------------------------------------------
			class CSatTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CSatTransform)
				CSatTransform()
				{
				}
				virtual ~CSatTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:sat val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_sat;
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

				SimpleTypes::CPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CSatModTransform 20.1.2.3.27
			//-----------------------------------------------------------------------
			class CSatModTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CSatModTransform)
				CSatModTransform()
				{
				}
				virtual ~CSatModTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:satMod val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_satMod;
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

				SimpleTypes::CPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CSatOffTransform 20.1.2.3.28
			//-----------------------------------------------------------------------
			class CSatOffTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CSatOffTransform)
				CSatOffTransform()
				{
				}
				virtual ~CSatOffTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:satOff val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_satOff;
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

				SimpleTypes::CPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CShadeTransform 20.1.2.3.31
			//-----------------------------------------------------------------------
			class CShadeTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CShadeTransform)
				CShadeTransform()
				{
				}
				virtual ~CShadeTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:shade val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_shade;
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

				SimpleTypes::CPositiveFixedPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CTintTransform 20.1.2.3.34
			//-----------------------------------------------------------------------
			class CTintTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CTintTransform)
				CTintTransform()
				{
				}
				virtual ~CTintTransform()
				{
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					oNode.ReadAttributeBase( _T("val"), m_oVal );
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
						oReader.ReadTillEnd();
				}

				virtual CString      toXML() const
				{
					CString sResult = _T("<a:tint val=\"") + m_oVal.ToString() + _T("\"/>");

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_a_tint;
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

				SimpleTypes::CPositiveFixedPercentage m_oVal;

			};

			//-----------------------------------------------------------------------
			// CColorTransform
			//-----------------------------------------------------------------------

			const double c_d_1_6 = 1.0 / 6.0;
			const double c_d_1_3 = 1.0 / 3.0;
			const double c_d_2_3 = 2.0 / 3.0;

			class CColorTransform : public WritingElement
			{
			public:
				WritingElement_AdditionConstructors(CColorTransform)
				CColorTransform()
				{
					SetRGBA( 0, 0, 0, 255 );
				}
				virtual ~CColorTransform()
				{
                    for ( unsigned int nIndex = 0; nIndex < m_arrTransform.size(); nIndex++ )
						delete m_arrTransform[nIndex];
					m_arrTransform.clear();
				}

			public:

				virtual void         fromXML(XmlUtils::CXmlNode& oNode)
				{
					// TO DO: Реализовать
				}
				virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
				{
					if ( oReader.IsEmptyNode() )
						return;

					int nCurDepth = oReader.GetDepth();
					while ( oReader.ReadNextSiblingNode( nCurDepth ) )
					{
                        CString sName = oReader.GetName();

						WritingElement *pTransform = NULL;
						if ( _T("a:alpha") == sName )
							pTransform = new CAlphaTransform( oReader );
						else if ( _T("a:alphaMod") == sName )
							pTransform = new CAlphaModTransform( oReader );
						else if ( _T("a:alphaOff") == sName )
							pTransform = new CAlphaOffTransform( oReader );
						else if ( _T("a:blue") == sName )
							pTransform = new CBlueTransform( oReader );
						else if ( _T("a:blueMod") == sName )
							pTransform = new CBlueModTransform( oReader );
						else if ( _T("a:blueOff") == sName )
							pTransform = new CBlueOffTransform( oReader );
						else if ( _T("a:comp") == sName )
							pTransform = new CComplementTransform( oReader );
						else if ( _T("a:gamma") == sName )
							pTransform = new CGammaTransform( oReader );
						else if ( _T("a:gray") == sName )
							pTransform = new CGrayscaleTransform( oReader );
						else if ( _T("a:green") == sName )
							pTransform = new CGreenTransform( oReader );
						else if ( _T("a:greenMod") == sName )
							pTransform = new CGreenModTransform( oReader );
						else if ( _T("a:greenOff") == sName )
							pTransform = new CGreenOffTransform( oReader );
						else if ( _T("a:hue") == sName )
							pTransform = new CHueTransform( oReader );
						else if ( _T("a:hueMod") == sName )
							pTransform = new CHueModTransform( oReader );
						else if ( _T("a:hueOff") == sName )
							pTransform = new CHueOffTransform( oReader );
						else if ( _T("a:inv") == sName )
							pTransform = new CInverseTransform( oReader );
						else if ( _T("a:invGamma") == sName )
							pTransform = new CInverseGammaTransform( oReader );
						else if ( _T("a:lum") == sName )
							pTransform = new CLumTransform( oReader );
						else if ( _T("a:lumMod") == sName )
							pTransform = new CLumModTransform( oReader );
						else if ( _T("a:lumOff") == sName )
							pTransform = new CLumOffTransform( oReader );
						else if ( _T("a:red") == sName )
							pTransform = new CRedTransform( oReader );
						else if ( _T("a:redMod") == sName )
							pTransform = new CRedModTransform( oReader );
						else if ( _T("a:redOff") == sName )
							pTransform = new CRedOffTransform( oReader );
						else if ( _T("a:sat") == sName )
							pTransform = new CSatTransform( oReader );
						else if ( _T("a:satMod") == sName )
							pTransform = new CSatModTransform( oReader );
						else if ( _T("a:satOff") == sName )
							pTransform = new CSatOffTransform( oReader );
						else if ( _T("a:shade") == sName )
							pTransform = new CShadeTransform( oReader );
						else if ( _T("a:tint") == sName )
							pTransform = new CTintTransform( oReader );

						if ( pTransform )
                            m_arrTransform.push_back ( pTransform );
					}
				}

				virtual CString      toXML() const
				{
					CString sResult;
					
                    for ( unsigned int nIndex = 0; nIndex < m_arrTransform.size(); nIndex++ )
						sResult += m_arrTransform[nIndex]->toXML();

					return sResult;
				}
				virtual EElementType getType() const
				{
					return OOX::et_Unknown;
				}


			public:

				inline void SetRGBA   (unsigned char  unR, unsigned char  unG, unsigned char  unB, unsigned char  unA = 255)
				{
					m_unR = unR;
					m_unG = unG;
					m_unB = unB;
					m_unA = unA;

					ApplyTransform();
				}
				inline void GetRawRGBA(unsigned char& unR, unsigned char& unG, unsigned char& unB, unsigned char& unA) const
				{
					unR = m_unRawR;
					unG = m_unRawG;
					unB = m_unRawB;
					unA = m_unRawA;
				}
				inline void GetRGBA   (unsigned char& unR, unsigned char& unG, unsigned char& unB, unsigned char& unA) const
				{
					unR = m_unR;
					unG = m_unG;
					unB = m_unB;
					unA = m_unA;
				}

				inline void SetHSL    (double  dH, double  dS, double  dL)
				{
					HSL2RGB( dH, dS, dL, m_unR, m_unG, m_unB );
					ApplyTransform();
				}
				inline void GetHSL    (double& dH, double& dS, double& dL)
				{
					RGB2HSL( m_unR, m_unG, m_unB, dH, dS, dL );
				}

				inline void GetRawHSL (double& dH, double& dS, double& dL)
				{
					RGB2HSL( m_unRawR, m_unRawG, m_unRawB, dH, dS, dL );
				}

				void        ApplyTransform()
				{
					m_unRawR = m_unR;
					m_unRawG = m_unG;
					m_unRawB = m_unB;
					m_unRawA = m_unA;

					// Применяем преобразования к цвету

                    unsigned int nCount = m_arrTransform.size();
					for (unsigned  int nIndex = 0; nIndex < nCount; ++nIndex )
					{
						WritingElement* pTransform = m_arrTransform[nIndex];
						if ( !pTransform )
							continue;

						EElementType eType = pTransform->getType();
						switch( eType )
						{
						case et_a_alpha:
							{
								CAlphaTransform* pAlphaTr = (CAlphaTransform*)pTransform;

								double dVal = pAlphaTr->m_oVal.GetValue() / 100.0;

                                int nA = (int)std::max( 0.0, std::min( 255.0, 255 * dVal ) );
								m_unA = static_cast<unsigned char>(nA);

								break;
							}
						case et_a_alphaMod:
							{
								CAlphaModTransform* pAlphaTr = (CAlphaModTransform*)pTransform;

								double dVal = pAlphaTr->m_oVal.GetValue() / 100.0;

                                int nA = (int)std::max( 0.0, std::min( 255.0, m_unA * dVal ) );
								m_unA = static_cast<unsigned char>(nA);

								break;
							}
						case et_a_alphaOff:
							{
								CAlphaOffTransform* pAlphaTr = (CAlphaOffTransform*)pTransform;

								double dVal = pAlphaTr->m_oVal.GetValue() / 100.0;

                                int nA = (int)std::max( 0.0, std::min( 255.0, m_unA + 255 * dVal ) );
								m_unA = static_cast<unsigned char>(nA);

								break;
							}
						case et_a_blue:
							{
								CBlueTransform* pBlueTr = (CBlueTransform*)pTransform;

								double dVal = pBlueTr->m_oVal.GetValue() / 100.0;

                                int nB = (int)std::max( 0.0, std::min( 255.0, 255 * dVal ) );
								m_unB = static_cast<unsigned char>(nB);

								break;
							}
						case et_a_blueMod:
							{
								CBlueModTransform* pBlueTr = (CBlueModTransform*)pTransform;

								double dVal = pBlueTr->m_oVal.GetValue() / 100.0;

                                int nB = (int)std::max( 0.0, std::min( 255.0, m_unB * dVal ) );
								m_unB = static_cast<unsigned char>(nB);

								break;
							}
						case et_a_blueOff:
							{
								CBlueOffTransform* pBlueTr = (CBlueOffTransform*)pTransform;

								double dVal = pBlueTr->m_oVal.GetValue() / 100.0;

                                int nB = (int)std::max( 0.0, std::min( 255.0, m_unB + 255 * dVal ) );
								m_unB = static_cast<unsigned char>(nB);

								break;
							}
						case et_a_comp:
							{
								double dH, dS, dL;
								RGB2HSL( m_unR, m_unG, m_unB, dH, dS, dL );

								dH = dH + 0.5;
								if ( dH > 1.0 )
									dH -= 1.0;

								HSL2RGB( dH, dS, dL, m_unR, m_unG, m_unB );

								break;
							}
						case et_a_gamma:
							{
								// TO DO: Сделать
								break;
							}
						case et_a_gray:
							{
								// TO DO: Сделать
								break;
							}
						case et_a_green:
							{
								CGreenTransform* pGreenTr = (CGreenTransform*)pTransform;

								double dVal = pGreenTr->m_oVal.GetValue() / 100.0;

                                int nG = (int)std::max( 0.0, std::min( 255.0, 255 * dVal ) );
								m_unG = static_cast<unsigned char>(nG);

								break;
							}
						case et_a_greenMod:
							{
								CGreenModTransform* pGreenTr = (CGreenModTransform*)pTransform;

								double dVal = pGreenTr->m_oVal.GetValue() / 100.0;

                                int nG = (int)std::max( 0.0, std::min( 255.0, m_unG * dVal ) );
								m_unG = static_cast<unsigned char>(nG);

								break;
							}
						case et_a_greenOff:
							{
								CGreenModTransform* pGreenTr = (CGreenModTransform*)pTransform;

								double dVal = pGreenTr->m_oVal.GetValue() / 100.0;

                                int nG = (int)std::max( 0.0, std::min( 255.0, m_unG + 255 * dVal ) );
								m_unG = static_cast<unsigned char>(nG);

								break;
							}
						case et_a_hue:
							{
								CHueTransform* pHueTr = (CHueTransform*)pTransform;
								double dVal = pHueTr->m_oVal.GetAngle() / 360.0;

								double dH, dS, dL;
								RGB2HSL( m_unR, m_unG, m_unB, dH, dS, dL );

                                dH = std::max( 0.0, std::min( 1.0, dVal ) );

								HSL2RGB( dH, dS, dL, m_unR, m_unG, m_unB );

								break;
							}
						case et_a_hueMod:
							{
								CHueModTransform* pHueTr = (CHueModTransform*)pTransform;
								double dVal = pHueTr->m_oVal.GetValue() / 100.0;

								double dH, dS, dL;
								RGB2HSL( m_unR, m_unG, m_unB, dH, dS, dL );

                                dH = std::max( 0.0, std::min( 1.0, dH * dVal ) );

								HSL2RGB( dH, dS, dL, m_unR, m_unG, m_unB );

								break;
							}
						case et_a_hueOff:
							{
								CHueOffTransform* pHueTr = (CHueOffTransform*)pTransform;
								double dVal = pHueTr->m_oVal.GetAngle() / 360.0;

								double dH, dS, dL;
								RGB2HSL( m_unR, m_unG, m_unB, dH, dS, dL );

                                dH = std::max( 0.0, std::min( 1.0, dH + dVal ) );

								HSL2RGB( dH, dS, dL, m_unR, m_unG, m_unB );

								break;
							}
						case et_a_inv:
							{
								m_unR = m_unR ^ 0xFF;
								m_unG = m_unG ^ 0xFF;
								m_unB = m_unB ^ 0xFF;
								break;
							}
						case et_a_invGamma:
							{
								// TO DO: сделать
								break;
							}
						case et_a_lum:
							{
								CLumTransform* pLumTr = (CLumTransform*)pTransform;
								double dVal = pLumTr->m_oVal.GetValue() / 100.0;

								double dH, dS, dL;
								RGB2HSL( m_unR, m_unG, m_unB, dH, dS, dL );

                                dL = std::max( 0.0, std::min( 1.0, dVal ) );

								HSL2RGB( dH, dS, dL, m_unR, m_unG, m_unB );

								break;
							}
						case et_a_lumMod:
							{
								CLumModTransform* pLumTr = (CLumModTransform*)pTransform;
								double dVal = pLumTr->m_oVal.GetValue() / 100.0;

								double dH, dS, dL;
								RGB2HSL( m_unR, m_unG, m_unB, dH, dS, dL );

                                dL = std::max( 0.0, std::min( 1.0, dL * dVal ) );

								HSL2RGB( dH, dS, dL, m_unR, m_unG, m_unB );

								break;
							}
						case et_a_lumOff:
							{
								CLumOffTransform* pLumTr = (CLumOffTransform*)pTransform;
								double dVal = pLumTr->m_oVal.GetValue() / 100.0;

								double dH, dS, dL;
								RGB2HSL( m_unR, m_unG, m_unB, dH, dS, dL );

                                dL = std::max( 0.0, std::min( 1.0, dL + dVal ) );

								HSL2RGB( dH, dS, dL, m_unR, m_unG, m_unB );

								break;
							}
						case et_a_red:
							{
								CRedTransform* pRedTr = (CRedTransform*)pTransform;

								double dVal = pRedTr->m_oVal.GetValue() / 100.0;

                                int nR = (int)std::max( 0.0, std::min( 255.0, 255 * dVal ) );
								m_unR = static_cast<unsigned char>(nR);

								break;
							}
						case et_a_redMod:
							{
								CRedModTransform* pRedTr = (CRedModTransform*)pTransform;

								double dVal = pRedTr->m_oVal.GetValue() / 100.0;

                                int nR = (int)std::max( 0.0, std::min( 255.0, m_unR * dVal ) );
								m_unR = static_cast<unsigned char>(nR);

								break;
							}
						case et_a_redOff:
							{
								CRedOffTransform* pRedTr = (CRedOffTransform*)pTransform;

								double dVal = pRedTr->m_oVal.GetValue() / 100.0;

                                int nR = (int)std::max( 0.0, std::min( 255.0, m_unR + 255 * dVal ) );
								m_unR = static_cast<unsigned char>(nR);

								break;
							}
						case et_a_sat:
							{
								CSatTransform* pSatTr = (CSatTransform*)pTransform;
								double dVal = pSatTr->m_oVal.GetValue() / 100.0;

								double dH, dS, dL;
								RGB2HSL( m_unR, m_unG, m_unB, dH, dS, dL );

                                dS = std::max( 0.0, std::min( 1.0, dVal ) );

								HSL2RGB( dH, dS, dL, m_unR, m_unG, m_unB );

								break;
							}
						case et_a_satMod:
							{
								CSatModTransform* pSatTr = (CSatModTransform*)pTransform;
								double dVal = pSatTr->m_oVal.GetValue() / 100.0;

								double dH, dS, dL;
								RGB2HSL( m_unR, m_unG, m_unB, dH, dS, dL );

                                dS = std::max( 0.0, std::min( 1.0, dS * dVal ) );

								HSL2RGB( dH, dS, dL, m_unR, m_unG, m_unB );

								break;
							}
						case et_a_satOff:
							{
								CSatOffTransform* pSatTr = (CSatOffTransform*)pTransform;
								double dVal = pSatTr->m_oVal.GetValue() / 100.0;

								double dH, dS, dL;
								RGB2HSL( m_unR, m_unG, m_unB, dH, dS, dL );

                                dS = std::max( 0.0, std::min( 1.0, dS + dVal ) );

								HSL2RGB( dH, dS, dL, m_unR, m_unG, m_unB );

								break;
							}
						case et_a_shade:
							{
								CShadeTransform* pShadeTr = (CShadeTransform*)pTransform;

								double dVal = pShadeTr->m_oVal.GetValue() / 100.0;

								m_unR = static_cast<unsigned char>(m_unR * dVal);
								m_unG = static_cast<unsigned char>(m_unG * dVal);
								m_unB = static_cast<unsigned char>(m_unB * dVal);

								break;
							}
						case et_a_tint:
							{
								CTintTransform* pTintTr = (CTintTransform*)pTransform;

								double dVal = pTintTr->m_oVal.GetValue() / 100.0;

								m_unR = static_cast<unsigned char>( 255 - ( 255 - m_unR ) * dVal );
								m_unG = static_cast<unsigned char>( 255 - ( 255 - m_unG ) * dVal );
								m_unB = static_cast<unsigned char>( 255 - ( 255 - m_unB ) * dVal );

								break;
							}
						}
					}
				}
			private:
				inline void RGB2HSL(unsigned char unR, unsigned char unG, unsigned char unB, double& dH, double& dS, double& dL)
				{
                    int nmin   = std::min( unR, std::min( unG, unB ) );
                    int nmax   = std::max( unR, std::max( unG, unB ) );
                    int nDelta = nmax - nmin;
                    double dmax   = ( nmax + nmin ) / 255.0;
					double dDelta = nDelta / 255.0;

                    dL = dmax / 2.0;

					if ( 0 == nDelta ) //This is a gray, no chroma...
					{
						dH = 0.0;
						dS = 0.0;
					}
					else //Chromatic data...
					{
                        if ( dL < 0.5 ) dS = dDelta / dmax;
                        else            dS = dDelta / ( 2.0 - dmax );

						dDelta = dDelta * 1530.0;

                        double dR = ( nmax - unR ) / dDelta;
                        double dG = ( nmax - unG ) / dDelta;
                        double dB = ( nmax - unB ) / dDelta;

                        if      ( unR == nmax ) dH = dB - dG;
                        else if ( unG == nmax ) dH = c_d_1_3 + dR - dB;
                        else if ( unB == nmax ) dH = c_d_2_3 + dG - dR;

						if ( dH < 0.0 ) dH += 1.0;
						if ( dH > 1.0 ) dH -= 1.0;
					}
				}

				inline void HSL2RGB(double dH, double dS, double dL, unsigned char &unR, unsigned char& unG, unsigned char& unB)
				{
					if ( 0 == dS )
					{
						unR = static_cast<unsigned char>(255 * dL);
						unG = static_cast<unsigned char>(255 * dL);
						unB = static_cast<unsigned char>(255 * dL);
					}
					else
					{
						double dV2;
						if ( dL < 0.5 ) dV2 = dL * ( 1.0 + dS );
						else            dV2 = dL + dS - dS * dL;

						double dV1 = 2.0 * dL - dV2;

						unR	= static_cast<unsigned char>(255 * Hue2RGB( dV1, dV2, dH + c_d_1_3 ) );
						unG	= static_cast<unsigned char>(255 * Hue2RGB( dV1, dV2, dH ) );
						unB = static_cast<unsigned char>(255 * Hue2RGB( dV1, dV2, dH - c_d_1_3 ) );
					} 
				}

				inline double Hue2RGB(double dV1, double dV2, double dH)
				{
					if ( dH < 0.0 ) dH += 1.0;
					if ( dH > 1.0 ) dH -= 1.0;
					if ( dH < c_d_1_6 ) return dV1 + ( dV2 - dV1 ) * 6.0 * dH;
					if ( dH < 0.5     ) return dV2;
					if ( dH < c_d_2_3 ) return dV1 + ( dV2 - dV1 ) * ( c_d_2_3 - dH ) * 6.0;
					return dV1;
				}

			public:

				unsigned char m_unRawA; //
				unsigned char m_unRawR; // Исходный цвет 
				unsigned char m_unRawG; //
				unsigned char m_unRawB; //

				unsigned char m_unA; //
				unsigned char m_unR; // Цвет после преобразований
				unsigned char m_unG; //
				unsigned char m_unB; //

				// Childs
                std::vector<WritingElement*> m_arrTransform;

			};


		} // Colors
	} // Drawing
} // OOX

namespace OOX
{
	namespace Drawing
	{
        //-----------------------------------------------------------------------
        // CHslColor 20.1.2.3.13
        //-----------------------------------------------------------------------
		class CHslColor : public Colors::CColorTransform
		{
		public:
			WritingElement_AdditionConstructors(CHslColor)
			CHslColor()
			{
			}
			virtual ~CHslColor()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				CColorTransform::fromXML( oReader );

				double dH = m_oHue.GetAngle() / 360.0;
				double dL = m_oLum.GetValue() / 100.0;
				double dS = m_oSat.GetValue() / 100.0;

				SetHSL( dH, dS, dL );
			}

			virtual CString      toXML() const
			{
				CString sResult = _T("<a:hslClr hue=\"") + m_oHue.ToString() + _T("\" sat=\"") + m_oSat.ToString() + _T("\" lum=\"") + m_oLum.ToString() + _T("\">");

				sResult += CColorTransform::toXML();

				sResult += _T("</a:hslClr>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_hslClr;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("hue"), m_oHue )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("lum"), m_oLum )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("sat"), m_oSat )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			SimpleTypes::CPositiveFixedAngle<0> m_oHue;
			SimpleTypes::CPercentage            m_oLum;
			SimpleTypes::CPercentage            m_oSat;

		};
		
        //-----------------------------------------------------------------------
        // CPresetColor 20.1.2.3.22
        //-----------------------------------------------------------------------
		class CPresetColor : public Colors::CColorTransform
		{
		public:
			WritingElement_AdditionConstructors(CPresetColor)
			CPresetColor()
			{
			}
			virtual ~CPresetColor()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				CColorTransform::fromXML( oReader );

				unsigned char unR = m_oVal.Get_R();
				unsigned char unG = m_oVal.Get_G();
				unsigned char unB = m_oVal.Get_B();

				SetRGBA( unR, unG, unB );
			}

			virtual CString      toXML() const
			{
				CString sResult = _T("<a:prstClr val=\"") + m_oVal.ToString() + _T("\">");

				sResult += CColorTransform::toXML();

				sResult += _T("</a:prstClr>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_prstClr;
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

			SimpleTypes::CPresetColorVal<> m_oVal;
		};
		
        //-----------------------------------------------------------------------
        // CSchemeColor 20.1.2.3.29
        //-----------------------------------------------------------------------
		class CSchemeColor : public Colors::CColorTransform
		{
		public:
			WritingElement_AdditionConstructors(CSchemeColor)
			CSchemeColor()
			{
			}
			virtual ~CSchemeColor()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				CColorTransform::fromXML( oReader );

				// TO DO: Тут надо как-то получить информацию о темах

				SetRGBA( 0, 0, 0 );
			}

			virtual CString      toXML() const
			{
				CString sResult = _T("<a:schemeClr val=\"") + m_oVal.ToString() + _T("\">");

				sResult += CColorTransform::toXML();

				sResult += _T("</a:schemeClr>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_schemeClr;
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

			SimpleTypes::CShemeColorVal<> m_oVal;
		};
		
        //-----------------------------------------------------------------------
        // CScRgbColor 20.1.2.3.30
        //-----------------------------------------------------------------------
		class CScRgbColor : public Colors::CColorTransform
		{
		public:
			WritingElement_AdditionConstructors(CScRgbColor)
			CScRgbColor()
			{
			}
			virtual ~CScRgbColor()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				CColorTransform::fromXML( oReader );

				double dR = m_oR.GetValue() / 100.0;
				double dG = m_oG.GetValue() / 100.0;
				double dB = m_oB.GetValue() / 100.0;

                unsigned char unR = (unsigned char)std::min( 255.0, std::max( 0.0, 255 * dR ) );
                unsigned char unG = (unsigned char)std::min( 255.0, std::max( 0.0, 255 * dG ) );
                unsigned char unB = (unsigned char)std::min( 255.0, std::max( 0.0, 255 * dB ) );

				SetRGBA( unR, unG, unB );
			}

			virtual CString      toXML() const
			{
				CString sResult = _T("<a:scrgbClr r=\"") + m_oR.ToString() + _T("\" g=\"") + m_oG.ToString() + _T("\" b=\"") + m_oB.ToString() + _T("\">");

				sResult += CColorTransform::toXML();

				sResult += _T("</a:scrgbClr>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_scrgbClr;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("b"), m_oB )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("g"), m_oG )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("r"), m_oR )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			SimpleTypes::CPercentage m_oB;
			SimpleTypes::CPercentage m_oG;
			SimpleTypes::CPercentage m_oR;
		};
		
        //-----------------------------------------------------------------------
        // CSRgbColor 20.1.2.3.32
        //-----------------------------------------------------------------------
		class CSRgbColor : public Colors::CColorTransform
		{
		public:
			WritingElement_AdditionConstructors(CSRgbColor)
			CSRgbColor()
			{
			}
			virtual ~CSRgbColor()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				CColorTransform::fromXML( oReader );

				unsigned char unR = m_oVal.Get_R();
				unsigned char unG = m_oVal.Get_G();
				unsigned char unB = m_oVal.Get_B();

				SetRGBA( unR, unG, unB );
			}

			virtual CString      toXML() const
			{
				CString sResult = _T("<a:srgbClr val=\"") + m_oVal.ToString() + _T("\">");

				sResult += CColorTransform::toXML();

				sResult += _T("</a:srgbClr>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_srgbClr;
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

			SimpleTypes::CHexColorRGB<> m_oVal;
		};
		
        //-----------------------------------------------------------------------
        // CSystemColor 20.1.2.3.33
        //-----------------------------------------------------------------------
		class CSystemColor : public Colors::CColorTransform
		{
		public:
			WritingElement_AdditionConstructors(CSystemColor)
			CSystemColor()
			{
			}
			virtual ~CSystemColor()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				CColorTransform::fromXML( oReader );

				unsigned char unR = m_oVal.Get_R();
				unsigned char unG = m_oVal.Get_G();
				unsigned char unB = m_oVal.Get_B();

				SetRGBA( unR, unG, unB );
			}

			virtual CString      toXML() const
			{
				CString sResult = _T("<a:sysClr val=\"") + m_oVal.ToString() + _T("\">");

				sResult += CColorTransform::toXML();

				sResult += _T("</a:sysClr>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_sysClr;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("lastClr"), m_oLastClr )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("val"),     m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			SimpleTypes::CHexColorRGB<>    m_oLastClr;
			SimpleTypes::CSystemColorVal<> m_oVal;
		};
		
        //-----------------------------------------------------------------------
        // CColor
        //-----------------------------------------------------------------------
		enum EColor
		{
			colorUnknown = -1,
			colorHsl     =  0,
			colorPrst    =  1,
			colorSheme   =  2,
			colorScRgb   =  3,
			colorSRgb    =  4,
			colorSys     =  5
		};
		class CColor : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CColor)
			CColor()
			{
			}
			virtual ~CColor()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType = colorUnknown;

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("a:hslClr") == sName )
					{
						m_oHslClr = oReader;
						m_eType   = colorHsl;
					}
					else if ( _T("a:prstClr") == sName )
					{
						m_oPrstClr = oReader;
						m_eType    = colorPrst;
					}
					else if ( _T("a:schemeClr") == sName )
					{
						m_oShemeClr = oReader;
						m_eType     = colorSheme;
					}
					else if ( _T("a:scrgbClr") == sName )
					{
						m_oScrgbClr = oReader;
						m_eType     = colorScRgb;
					}
					else if ( _T("a:srgbClr") == sName )
					{
						m_oSrgbClr = oReader;
						m_eType    = colorSRgb;
					}
					else if ( _T("a:sysClr") == sName )
					{
						m_oSysClr = oReader;
						m_eType   = colorSys;
					}
				}
			}

			virtual CString      toXML() const
			{
				CString sResult;
				
				switch( m_eType )
				{
				case colorHsl:   sResult = m_oHslClr.toXML();   break;
				case colorPrst:  sResult = m_oPrstClr.toXML();  break;
				case colorSheme: sResult = m_oShemeClr.toXML(); break;
				case colorScRgb: sResult = m_oScrgbClr.toXML(); break;
				case colorSRgb:  sResult = m_oSrgbClr.toXML();  break;
				case colorSys:   sResult = m_oSysClr.toXML();   break;
				}

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_Unknown;
			}
			bool tryGetRgb(unsigned char& unR, unsigned char& unG, unsigned char& unB, unsigned char& unA)
			{
				if(colorSys == m_eType)
				{
					m_oSysClr.GetRGBA(unR, unG, unB, unA);
					return true;
				}
				else if(colorSRgb == m_eType)
				{
					m_oSrgbClr.GetRGBA(unR, unG, unB, unA);
					return true;
				}
				return false;
			}

		public:

			// Type
			EColor                         m_eType;

			// Childs
			OOX::Drawing::CHslColor        m_oHslClr;
			OOX::Drawing::CPresetColor     m_oPrstClr;
			OOX::Drawing::CSchemeColor     m_oShemeClr;
			OOX::Drawing::CScRgbColor      m_oScrgbClr;
			OOX::Drawing::CSRgbColor       m_oSrgbClr;
			OOX::Drawing::CSystemColor     m_oSysClr;
		};
		
	} // Drawing

} // OOX

#endif // OOX_LOGIC_DRAWING_COLORS_INCLUDE_H_
