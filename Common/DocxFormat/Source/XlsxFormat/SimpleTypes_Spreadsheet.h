﻿/*
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
#include "../Common/SimpleTypes_Base.h"

namespace SimpleTypes
{
	namespace Spreadsheet
	{	
		template<typename E, E DefValue = 0>
		class CSimpleType
		{
		public:
			CSimpleType()
			{
				m_eValue = DefValue;
			}
			CSimpleType(const E& val)
			{
				m_eValue = val;
			}
            virtual ~CSimpleType() {}

			virtual E GetValue() const
			{
				return m_eValue;
			}
			virtual void SetValue(E eValue)
			{
				m_eValue = eValue;
			}
            virtual E FromString(std::wstring &sValue) = 0;
			virtual std::wstring ToString() const = 0;
		protected:
			E m_eValue;
		};

		enum ETableType
		{
			typeQueryTable	=  0,
			typeWorksheet	=  1,
			typeXml			=  2,
		};
		template<ETableType eDefValue = typeWorksheet>
		class CTableType : public CSimpleType<ETableType, eDefValue>
		{
		public:
			CTableType() {}
			CTableType(const ETableType & val)	{ this->m_eValue = val; }

            virtual ETableType FromString(std::wstring &sValue)
			{
                if      ( L"queryTable"	== sValue ) this->m_eValue = typeQueryTable;
                else if ( L"worksheet"	== sValue ) this->m_eValue = typeWorksheet;
                else if ( L"xml"		== sValue ) this->m_eValue = typeXml;
                else this->m_eValue = eDefValue;

                return this->m_eValue;
			}

			virtual std::wstring ToString  () const 
			{
                switch(this->m_eValue)
				{
				case typeQueryTable :	return L"queryTable";
				case typeXml :			return L"xml";
				case typeWorksheet :
				default :				return L"worksheet";

				}
			}

			SimpleType_FromString     (ETableType)
			SimpleType_Operator_Equal (CTableType)
		};
		enum EDatabaseSourceType
		{
			typeODBCBasedSource = 1,
			typeDAOBasedSource = 2,
			typeFileBasedDatabaseSource = 3,
			typeWebQuery = 4,
			typeOLEDBBasedSource = 5,
			typeTextBasedSource = 6,
			typeADORecordSet = 7,
			typeDSP
		};

		enum EExternalConnectionType
		{
			extConnTypeGeneral	= 0,
			extConnTypeText		= 1,
			extConnTypeMDY		= 2,
			extConnTypeDMY		= 3,
			extConnTypeYMD		= 4,
			extConnTypeMYD		= 5,
			extConnTypeDYM		= 6,
			extConnTypeYDM		= 7,
			extConnTypeSkip		= 8,
			extConnTypeEMD		= 9
		};
		template<EExternalConnectionType eDefValue = extConnTypeGeneral>
		class CExternalConnectionType : public CSimpleType<EExternalConnectionType, eDefValue>
		{
		public:
			CExternalConnectionType() {}
			CExternalConnectionType(const EExternalConnectionType & val)	{ this->m_eValue = val; }

            virtual EExternalConnectionType FromString(std::wstring &sValue)
			{
                if      ( L"general"	== sValue ) this->m_eValue = extConnTypeGeneral;
                else if ( L"text"		== sValue ) this->m_eValue = extConnTypeText;
                else if ( L"MDY"		== sValue ) this->m_eValue = extConnTypeMDY;
                else if ( L"DMY"		== sValue ) this->m_eValue = extConnTypeDMY;
                else if ( L"YMD"		== sValue ) this->m_eValue = extConnTypeYMD;
                else if ( L"MYD"		== sValue ) this->m_eValue = extConnTypeMYD;
                else if ( L"DYM"		== sValue ) this->m_eValue = extConnTypeDYM;
                else if ( L"YDM"		== sValue ) this->m_eValue = extConnTypeYDM;
                else if ( L"skip"		== sValue ) this->m_eValue = extConnTypeSkip;
				else if ( L"EMD"		== sValue ) this->m_eValue = extConnTypeEMD;
				else								this->m_eValue = extConnTypeGeneral;

                return this->m_eValue;
			}
			virtual std::wstring ToString  () const 
			{
                switch(this->m_eValue)
				{
				case extConnTypeText:		return L"text";	
				case extConnTypeMDY:		return L"MDY";	
				case extConnTypeDMY:		return L"DMY";	
				case extConnTypeYMD:		return L"YMD";	
				case extConnTypeMYD:		return L"MYD";	
				case extConnTypeDYM:		return L"DYM";	
				case extConnTypeYDM:		return L"YDM";
				case extConnTypeSkip:		return L"stored";
				case extConnTypeGeneral:
				default :					return L"general";

				}
			}
			SimpleType_FromString     (EExternalConnectionType)
			SimpleType_Operator_Equal (CExternalConnectionType)
		};	
		enum ECredMethod
		{
			integrated_method	= 0,
			none_method			= 1,
			prompt_method		= 2,
			stored_method		= 3
		};
		template<ECredMethod eDefValue = integrated_method>
		class CCredMethod : public CSimpleType<ECredMethod, eDefValue>
		{
		public:
			CCredMethod() {}
			CCredMethod(const ECredMethod & val)	{ this->m_eValue = val; }

            virtual ECredMethod FromString(std::wstring &sValue)
			{
                if      ( L"integrated"	== sValue ) this->m_eValue = integrated_method;
                else if ( L"none"		== sValue ) this->m_eValue = none_method;
                else if ( L"prompt"		== sValue ) this->m_eValue = prompt_method;
                else if ( L"stored"		== sValue ) this->m_eValue = stored_method;
                else this->m_eValue = integrated_method;

                return this->m_eValue;
			}
			virtual std::wstring ToString  () const 
			{
                switch(this->m_eValue)
				{
				case none_method:		return L"none";	
				case prompt_method:		return L"prompt";
				case stored_method:		return L"stored";
				case integrated_method:
				default :				return L"integrated";

				}
			}
			SimpleType_FromString     (ECredMethod)
			SimpleType_Operator_Equal (CCredMethod)
		};		

		enum EVisibleType
		{
			visibleHidden		=  0,
			visibleVeryHidden	=  1,
			visibleVisible		=  2,
		};
		template<EVisibleType eDefValue = visibleVisible>
		class CVisibleType : public CSimpleType<EVisibleType, eDefValue>
		{
		public:
			CVisibleType() {}
			CVisibleType(const EVisibleType & val) 
			{
				this->m_eValue = val;
			}

            virtual EVisibleType FromString(std::wstring &sValue)
			{
                if      ( L"hidden"      == sValue ) this->m_eValue = visibleHidden;
                else if ( L"veryHidden"  == sValue ) this->m_eValue = visibleVeryHidden;
                else if ( L"visible"     == sValue ) this->m_eValue = visibleVisible;
                else this->m_eValue = eDefValue;

                return this->m_eValue;
			}
			virtual std::wstring ToString  () const 
			{
                switch(this->m_eValue)
				{
				case visibleHidden :		return L"hidden";	
				case visibleVeryHidden :	return L"veryHidden";
				case visibleVisible :		
				default :					return L"visible";
				}
			}
			SimpleType_FromString     (EVisibleType)
			SimpleType_Operator_Equal (CVisibleType)
		};
		enum EHtmlFormat
		{
			htmlNone =  0,
			htmlRtf	=  1,
			htmlAll	=  2
		};
		template<EHtmlFormat eDefValue = htmlNone>
		class CHtmlFormat : public CSimpleType<EHtmlFormat, eDefValue>
		{
		public:
			CHtmlFormat() {}
			CHtmlFormat(const EHtmlFormat & val)	{ this->m_eValue = val; }

            virtual EHtmlFormat FromString(std::wstring &sValue)
			{
                if      ( L"none"		== sValue )	this->m_eValue = htmlNone;
                else if ( L"veryHidden" == sValue )	this->m_eValue = htmlRtf;
                else if ( L"visible"	== sValue ) this->m_eValue = htmlAll;
                else this->m_eValue = eDefValue;

                return this->m_eValue;
			}
			virtual std::wstring ToString  () const 
			{
                switch(this->m_eValue)
				{
				case htmlNone : return L"none";
				case htmlRtf :	return L"rtf";
				case htmlAll :	return L"all";
				default :		return L"none";
				}
			}
			SimpleType_FromString     (EHtmlFormat)
			SimpleType_Operator_Equal (CHtmlFormat)
		};
		enum EParameterType
		{
			parameterPrompt =  0,
			parameterValue	=  1,
			parameterCell	=  2
		};
		template<EParameterType eDefValue = parameterPrompt>
		class CParameterType : public CSimpleType<EParameterType, eDefValue>
		{
		public:
			CParameterType() {}
			CParameterType(const EParameterType & val)	{ this->m_eValue = val; }

            virtual EParameterType FromString(std::wstring &sValue)
			{
                if      ( L"promt"	== sValue )	this->m_eValue = parameterPrompt;
                else if ( L"value"	== sValue )	this->m_eValue = parameterValue;
                else if ( L"cell"	== sValue ) this->m_eValue = parameterCell;
                else                            this->m_eValue = eDefValue;

                return this->m_eValue;
				}
			virtual std::wstring ToString  () const 
			{
                switch(this->m_eValue)
				{
				case parameterValue :	return L"value";
				case parameterCell :	return L"cell";
				case parameterPrompt : 
				default :				return L"promt";
			}
			}
			SimpleType_FromString     (EParameterType)
			SimpleType_Operator_Equal (CParameterType)
		};
		enum EQualifier
		{
			doubleQuote =  0,
			singleQuote	=  1,
			noneQuote	=  2
		};
		template<EQualifier eDefValue = doubleQuote>
		class CQualifier : public CSimpleType<EQualifier, eDefValue>
		{
		public:
			CQualifier() {}
			CQualifier(const EQualifier & val)	{ this->m_eValue = val; }

            virtual EQualifier FromString(std::wstring &sValue)
			{
                if      ( L"doubleQuote"	== sValue )	this->m_eValue = doubleQuote;
                else if ( L"singleQuote"	== sValue )	this->m_eValue = singleQuote;
                else if ( L"none"			== sValue ) this->m_eValue = noneQuote;
                else									this->m_eValue = eDefValue;

                return this->m_eValue;
			}
			virtual std::wstring ToString  () const 
			{
                switch(this->m_eValue)
				{
				case noneQuote :	return L"none";
				case singleQuote :	return L"singleQuote";
				case doubleQuote : 
				default :			return L"doubleQuote";
				}
			}
			SimpleType_FromString     (EQualifier)
			SimpleType_Operator_Equal (CQualifier)
		};
		enum EFileType
		{
			fileTypeMac		=  0,
			fileTypeWin		=  1,
			fileTypeDos		=  2,
			fileTypeLin		=  3,
			fileTypeOther	=  4
		};
		template<EFileType eDefValue = fileTypeWin>
		class CFileType : public CSimpleType<EFileType, eDefValue>
		{
		public:
			CFileType() {}
			CFileType(const EFileType & val)	{ this->m_eValue = val; }

            virtual EFileType FromString(std::wstring &sValue)
			{
                if      ( L"win"	== sValue )	this->m_eValue = fileTypeWin;
                else if ( L"mac"	== sValue )	this->m_eValue = fileTypeMac;
                else if ( L"lin"	== sValue )	this->m_eValue = fileTypeLin;
				else if ( L"dos"	== sValue )	this->m_eValue = fileTypeDos;
				else if ( L"other"	== sValue ) this->m_eValue = fileTypeOther;
                else                            this->m_eValue = eDefValue;

                return this->m_eValue;
			}
			virtual std::wstring ToString  () const 
			{
                switch(this->m_eValue)
				{
				case fileTypeMac :		return L"mac";
				case fileTypeDos :		return L"dos";
				case fileTypeLin :		return L"lin";
				case fileTypeOther :	return L"other";
				case fileTypeWin : 
				default :				return L"win";
				}
			}
			SimpleType_FromString     (EFileType)
			SimpleType_Operator_Equal (CFileType)
		};		
		enum EPhoneticAlignmentType
		{
			phoneticalignmentCenter			=  0,
			phoneticalignmentDistributed	=  1,
			phoneticalignmentLeft			=  2,
			phoneticalignmentNoControl 		=  3,
		};

		template<EPhoneticAlignmentType eDefValue = phoneticalignmentNoControl>
		class CPhoneticAlignment : public CSimpleType<EPhoneticAlignmentType, eDefValue>
		{
		public:
			CPhoneticAlignment() {}
			CPhoneticAlignment(const EPhoneticAlignmentType & val)	{ this->m_eValue = val; }

            virtual EPhoneticAlignmentType FromString(std::wstring &sValue)
			{
                if      ( L"center"		== sValue ) this->m_eValue = phoneticalignmentCenter;
                else if ( L"distributed"== sValue ) this->m_eValue = phoneticalignmentDistributed;
                else if ( L"left"		== sValue ) this->m_eValue = phoneticalignmentLeft;
                else if ( L"noControl"	== sValue ) this->m_eValue = phoneticalignmentNoControl;
                else                                             this->m_eValue = eDefValue;

                return this->m_eValue;
			}

			virtual std::wstring       ToString  () const 
			{
                switch(this->m_eValue)
				{
				case phoneticalignmentCenter :		return L"center";
				case phoneticalignmentDistributed : return L"distributed";
				case phoneticalignmentLeft :		return L"left";		
				case phoneticalignmentNoControl : 
				default :							return L"noControl";
}
			}

			SimpleType_FromString     (EPhoneticAlignmentType)
			SimpleType_Operator_Equal (CPhoneticAlignment)
		};

		enum EPhoneticTypeType
		{
			phonetictypeFullwidthKatakana			=  0,
			phonetictypeHalfwidthKatakana			=  1,
			phonetictypeHiragana					=  2,
			phonetictypeNoConversion 				=  3,
		};

		template<EPhoneticTypeType eDefValue = phonetictypeNoConversion>
		class CPhoneticType : public CSimpleType<EPhoneticTypeType, eDefValue>
		{
		public:
			CPhoneticType() {}
			CPhoneticType(const EPhoneticTypeType & val)	{ this->m_eValue = val; }

            virtual EPhoneticTypeType FromString(std::wstring &sValue)
			{
                if      ( L"fullwidthKatakana"	== sValue ) this->m_eValue = phonetictypeFullwidthKatakana;
                else if ( L"halfwidthKatakana"	== sValue ) this->m_eValue = phonetictypeHalfwidthKatakana;
                else if ( L"hiragana"			== sValue ) this->m_eValue = phonetictypeHiragana;
                else if ( L"noConversion"		== sValue ) this->m_eValue = phonetictypeNoConversion;
                else                                             this->m_eValue = eDefValue;

                return this->m_eValue;
			}

			virtual std::wstring       ToString  () const 
			{
				switch(this->m_eValue)
				{
				case phonetictypeFullwidthKatakana :	return L"fullwidthKatakana";
				case phonetictypeHalfwidthKatakana :	return L"halfwidthKatakana";
				case phonetictypeHiragana :				return L"hiragana";	
				case phonetictypeNoConversion : 
				default :								return L"noConversion";
				}
			}

			SimpleType_FromString     (EPhoneticTypeType)
			SimpleType_Operator_Equal (CPhoneticType)
		};

		enum EFontCharset
		{
			fontcharsetANSI        =   0, // 00
			fontcharsetDefault     =   1, // 01
			fontcharsetSymbol      =   2, // 02
			fontcharsetMacintosh   =  77, // 4D
			fontcharsetShitJIS     = 128, // 80
			fontcharsetHangeul     = 129, // 81
			fontcharsetJohab       = 130, // 82
			fontcharsetGB2313      = 134, // 86
			fontcharsetChineseBig5 = 136, // 88
			fontcharsetGreek       = 161, // A1
			fontcharsetTurkish     = 162, // A2
			fontcharsetVietnamese  = 163, // A3
			fontcharsetHebrew      = 177, // B1
			fontcharsetArabic      = 178, // B2
			fontcharsetBaltic      = 186, // BA
			fontcharsetRussian     = 204, // CC
			fontcharsetThai        = 222, // DE
			fontcharsetEastEurope  = 238, // EE
			fontcharsetOEM         = 255, // FF	
		};

		template<EFontCharset eDefValue = fontcharsetANSI>
		class CFontCharset : public CSimpleType<EFontCharset, eDefValue>
		{
		public:
			CFontCharset() {}
			CFontCharset(const EFontCharset & val)	{ this->m_eValue = val; }

            virtual EFontCharset FromString(std::wstring &sValue)
			{
                int nCharset = _wtoi(sValue.c_str());
				switch(nCharset)
				{
                case 0: this->m_eValue = fontcharsetANSI;break;
                case 1: this->m_eValue = fontcharsetDefault;break;
                case 2: this->m_eValue = fontcharsetSymbol;break;
                case 77: this->m_eValue = fontcharsetMacintosh;break;
                case 128: this->m_eValue = fontcharsetShitJIS;break;
                case 129: this->m_eValue = fontcharsetHangeul;break;
                case 130: this->m_eValue = fontcharsetJohab;break;
                case 134: this->m_eValue = fontcharsetGB2313;break;
                case 136: this->m_eValue = fontcharsetChineseBig5;break;
                case 161: this->m_eValue = fontcharsetGreek;break;
                case 162: this->m_eValue = fontcharsetTurkish;break;
                case 163: this->m_eValue = fontcharsetVietnamese;break;
                case 177: this->m_eValue = fontcharsetHebrew;break;
                case 178: this->m_eValue = fontcharsetArabic;break;
                case 186: this->m_eValue = fontcharsetBaltic;break;
                case 204: this->m_eValue = fontcharsetRussian;break;
                case 222: this->m_eValue = fontcharsetThai;break;
                case 238: this->m_eValue = fontcharsetEastEurope;break;
                case 255: this->m_eValue = fontcharsetOEM;break;
                default:this->m_eValue = eDefValue;
				}

                return this->m_eValue;
			}

			virtual std::wstring       ToString  () const 
			{
                return std::to_wstring( this->m_eValue);
			}
			virtual std::wstring       ToHexString  () const 
			{
                std::wstring sRes;
                switch(this->m_eValue)
				{
				case fontcharsetANSI: sRes = L"00";break;
				case fontcharsetDefault: sRes = L"01";break;
				case fontcharsetSymbol: sRes = L"02";break;
				case fontcharsetMacintosh: sRes = L"4D";break;
				case fontcharsetShitJIS: sRes = L"80";break;
				case fontcharsetHangeul: sRes = L"81";break;
				case fontcharsetJohab: sRes = L"82";break;
				case fontcharsetGB2313: sRes = L"86";break;
				case fontcharsetChineseBig5: sRes = L"88";break;
				case fontcharsetGreek: sRes = L"A1";break;
				case fontcharsetTurkish: sRes = L"A2";break;
				case fontcharsetVietnamese: sRes = L"A3";break;
				case fontcharsetHebrew: sRes = L"B1";break;
				case fontcharsetArabic: sRes = L"B2";break;
				case fontcharsetBaltic: sRes = L"BA";break;
				case fontcharsetRussian: sRes = L"CC";break;
				case fontcharsetThai: sRes = L"DE";break;
				case fontcharsetEastEurope: sRes = L"EE";break;
				case fontcharsetOEM: sRes = L"FF";break;
				}
				return sRes;
			}

			SimpleType_FromString     (EFontCharset)
			SimpleType_Operator_Equal (CFontCharset)
		};

		enum EThemeColor
		{
			themecolorLight1            =  0,
			themecolorDark1				=  1,
			themecolorLight2            =  2,
			themecolorDark2				=  3,
			themecolorAccent1           =  4,
			themecolorAccent2           =  5,
			themecolorAccent3           =  6,
			themecolorAccent4           =  7,
			themecolorAccent5           =  8,
			themecolorAccent6           =  9,
			themecolorHyperlink         = 10,
			themecolorFollowedHyperlink = 11
		};

		template<EThemeColor eDefValue = themecolorDark1>
		class CThemeColor : public CSimpleType<EThemeColor, eDefValue>
		{
		public:
			CThemeColor() {}
			CThemeColor(const EThemeColor & val)	{ this->m_eValue = val; }

            virtual EThemeColor FromString(std::wstring &sValue)
			{
                int nThemeColor = _wtoi(sValue.c_str());
				switch(nThemeColor)
				{
                case 0:this->m_eValue = themecolorLight1;break;
                case 1:this->m_eValue = themecolorDark1;break;
                case 2:this->m_eValue = themecolorLight2;break;
                case 3:this->m_eValue = themecolorDark2;break;
                case 4:this->m_eValue = themecolorAccent1;break;
                case 5:this->m_eValue = themecolorAccent2;break;
                case 6:this->m_eValue = themecolorAccent3;break;
                case 7:this->m_eValue = themecolorAccent4;break;
                case 8:this->m_eValue = themecolorAccent5;break;
                case 9:this->m_eValue = themecolorAccent6;break;
                case 10:this->m_eValue = themecolorHyperlink;break;
                case 11:this->m_eValue = themecolorFollowedHyperlink;break;
                default:this->m_eValue = eDefValue;
				}
                return this->m_eValue;
			}
			virtual std::wstring ToString  () const 
			{
				switch(this->m_eValue)
				{
				case themecolorLight1:		return L"lt1";
                case themecolorDark1:		return L"dk1";
                case themecolorLight2:		return L"lt2";
                case themecolorDark2:		return L"dk2";
                case themecolorAccent1:		return L"accent1";
                case themecolorAccent2:		return L"accent2";
                case themecolorAccent3:		return L"accent3";
                case themecolorAccent4:		return L"accent4";
                case themecolorAccent5:		return L"accent5";
                case themecolorAccent6:		return L"accent6";
                case themecolorHyperlink:	return L"hlink";
                case themecolorFollowedHyperlink:	return L"folHlink";
				default:
					return L"";
				}
				return L"";
			}

			SimpleType_FromString     (EThemeColor)
			SimpleType_Operator_Equal (CThemeColor)
		};

		class CHexColor
		{
		public:
			CHexColor() 
			{
				m_unA = 255;
				m_unR = 0;
				m_unG = 0;
				m_unB = 0;
			}
			CHexColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) 
			{
				m_unA = a;
				m_unR = r;
				m_unG = g;
				m_unB = b;
			}
            CHexColor(std::wstring & cwsValue)
            {
                FromString( cwsValue );
            }

            virtual void FromString(std::wstring &sValue)
			{
				Parse(sValue);
			}
            virtual void FromString(const std::wstring &sValue)
            {
                Parse(sValue);
            }
            CHexColor(const std::wstring& wsStr)
            {
                FromString( wsStr);
            }
            const CHexColor &operator =(std::wstring &sValue)
            {
                FromString( sValue );
                return *this;
            }
            const CHexColor &operator =(const std::wstring& wsStr)
            {
                FromString( wsStr);
                return *this;
            }
			virtual std::wstring   ToString  () const 
			{
                std::wstring sResult;
                sResult += XmlUtils::IntToString(m_unA, L"%02X");
                sResult += XmlUtils::IntToString(m_unR, L"%02X");
                sResult += XmlUtils::IntToString(m_unG, L"%02X");
                sResult += XmlUtils::IntToString(m_unB, L"%02X");

                return sResult;
			}
			void Set_R(unsigned char R)
			{
				m_unR = R;
			}
			void Set_G(unsigned char G)
			{
				m_unG = G;
			}
			void Set_B(unsigned char B)
			{
				m_unB = B;
			}
			void Set_A(unsigned char A)
			{
				m_unA = A;
			}
			unsigned char Get_R() const
			{
				return m_unR;
			}
			unsigned char Get_G() const
			{
				return m_unG;
			}

			unsigned char Get_B() const
			{
				return m_unB;
			}
			unsigned char Get_A() const
			{
				return 255;
			}
			int ToInt() const
			{
				int nRes = m_unB;
				nRes += m_unG << 8;
				nRes += m_unR << 16;
				nRes += m_unA << 24;
				return nRes;
			}
			void FromInt(int nColor)
			{
				m_unB = static_cast<unsigned char>(nColor & 0xFF);
				m_unG = static_cast<unsigned char>((nColor & 0xFF00)>>8);
				m_unR = static_cast<unsigned char>((nColor & 0xFF0000)>>16);
				m_unA = 255;
			}
		private:

            void Parse(std::wstring sValue)
			{
				if (0 == sValue.find(L"#"))
				{
					sValue = sValue.substr(1);
				}
                int nValueLength = (int)sValue.length();

                if(3 == nValueLength)
				{
					int nTempR = HexToInt( (int)sValue[0] );
					int nTempG = HexToInt( (int)sValue[1] );
					int nTempB = HexToInt( (int)sValue[2] );

                    m_unR = nTempR +  (unsigned char)(nTempR << 4);
					m_unG = nTempG +  (unsigned char)(nTempG << 4);
					m_unB = nTempB +  (unsigned char)(nTempB << 4);
				}
				else if(6 == nValueLength)
				{
					m_unR = HexToInt( (int)sValue[1] ) + (unsigned char)(HexToInt( (int)sValue[0] ) << 4);
					m_unG = HexToInt( (int)sValue[3] ) + (unsigned char)(HexToInt( (int)sValue[2] ) << 4);
					m_unB = HexToInt( (int)sValue[5] ) + (unsigned char)(HexToInt( (int)sValue[4] ) << 4);
				}
				else if(8 == nValueLength)
				{
					m_unA = HexToInt( (int)sValue[1] ) + (unsigned char)(HexToInt( (int)sValue[0] ) << 4);
					m_unR = HexToInt( (int)sValue[3] ) + (unsigned char)(HexToInt( (int)sValue[2] ) << 4);
					m_unG = HexToInt( (int)sValue[5] ) + (unsigned char)(HexToInt( (int)sValue[4] ) << 4);
					m_unB = HexToInt( (int)sValue[7] ) + (unsigned char)(HexToInt( (int)sValue[6] ) << 4);
				}
			}

			int	HexToInt(int nHex)
			{
				if ( nHex >= '0' && nHex <= '9' ) return (nHex - '0');
				if ( nHex >= 'a' && nHex <= 'f' ) return (nHex - 'a' + 10);
				if ( nHex >= 'A' && nHex <= 'F' ) return (nHex - 'A' + 10);

				return 0;
			}
		private:
			unsigned char m_unA;
			unsigned char m_unR;
			unsigned char m_unG;
			unsigned char m_unB;		
		};


		enum EFontFamily
		{
			fontfamilyNotApplicable		=  0,
			fontfamilyRoman				=  1,
			fontfamilySwiss				=  2,
			fontfamilyModern			=  3,
			fontfamilyScript			=  4,
			fontfamilyDecorative		=  5,
		};
		template<EFontFamily eDefValue = fontfamilyNotApplicable>
		class CFontFamily : public CSimpleType<EFontFamily, eDefValue>
		{
		public:
			CFontFamily() {}
			CFontFamily(const EFontFamily & val)	{ this->m_eValue = val; }

            virtual EFontFamily FromString(std::wstring &sValue)
			{
                int nFontFamily = _wtoi(sValue.c_str());
				switch(nFontFamily)
				{
                case 0:this->m_eValue = fontfamilyNotApplicable;break;
                case 1:this->m_eValue = fontfamilyRoman;break;
                case 2:this->m_eValue = fontfamilySwiss;break;
                case 3:this->m_eValue = fontfamilyModern;break;
                case 4:this->m_eValue = fontfamilyScript;break;
                case 5:this->m_eValue = fontfamilyDecorative;break;
                default:this->m_eValue = eDefValue;
				}
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                return std::to_wstring(this->m_eValue);
			}
            std::wstring ToStringWord() const
			{
                std::wstring sRes;
                switch(this->m_eValue)
				{
				case fontfamilyNotApplicable:sRes = L"auto";break;
				case fontfamilyRoman:sRes = L"roman";break;
				case fontfamilySwiss:sRes = L"swiss";break;
				case fontfamilyModern:sRes = L"modern";break;
				case fontfamilyScript:sRes = L"script";break;
				case fontfamilyDecorative:sRes = L"decorative";break;
				default:sRes = L"auto";
				}
				return sRes;
			}

			SimpleType_FromString     (EFontFamily)
			SimpleType_Operator_Equal (CFontFamily)
		};

		enum EFontScheme
		{
			fontschemeMajor		=  0,
			fontschemeMinor		=  1,
			fontschemeNone		=  2
		};
		template<EFontScheme eDefValue = fontschemeNone>
		class CFontScheme : public CSimpleType<EFontScheme, eDefValue>
		{
		public:
			CFontScheme() {}
			CFontScheme(const EFontScheme & val)	{ this->m_eValue = val; }

            virtual EFontScheme FromString(std::wstring &sValue)
			{
				if(L"major" == sValue)
                    this->m_eValue = fontschemeMajor;
				else if(L"minor" == sValue)
                    this->m_eValue = fontschemeMinor;
				else if(L"none" == sValue)
                    this->m_eValue = fontschemeNone;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                std::wstring sRes;
                switch(this->m_eValue)
				{
				case fontschemeMajor: sRes = L"major";break;
				case fontschemeMinor: sRes = L"minor";break;
				case fontschemeNone: sRes = L"none";break;
				}
				return sRes;
			}

			SimpleType_FromString     (EFontScheme)
				SimpleType_Operator_Equal (CFontScheme)
		};
		enum EUnderline
		{
			underlineDouble				=  0,
			underlineDoubleAccounting	=  1,
			underlineNone				=  2,
			underlineSingle				=  3,
			underlineSingleAccounting	=  4
		};
		template<EUnderline eDefValue = underlineSingle>
		class CUnderline : public CSimpleType<EUnderline, eDefValue>
		{
		public:
			CUnderline() {}
			CUnderline(const EUnderline & val)	{ this->m_eValue = val; }

            virtual EUnderline FromString(std::wstring &sValue)
			{
				if(L"double" == sValue)
                    this->m_eValue = underlineDouble;
				else if(L"doubleAccounting" == sValue)
                    this->m_eValue = underlineDoubleAccounting;
				else if(L"none" == sValue)
                    this->m_eValue = underlineNone;
				else if(L"single" == sValue)
                    this->m_eValue = underlineSingle;
				else if(L"singleAccounting" == sValue)
                    this->m_eValue = underlineSingleAccounting;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case underlineDouble: return L"double";break;
				case underlineDoubleAccounting: return L"doubleAccounting";break;
				case underlineNone: return L"none";break;
				case underlineSingle: return L"single";break;
				case underlineSingleAccounting: return L"singleAccounting";break;
				default: return L"none";
				}
			}

			SimpleType_FromString     (EUnderline)
			SimpleType_Operator_Equal (CUnderline)
		};

		enum EBorderStyle
		{
			borderstyleDashDot			=  0,
			borderstyleDashDotDot		=  1,
			borderstyleDashed			=  2,
			borderstyleDotted			=  3,
			borderstyleDouble			=  4,
			borderstyleHair				=  5,
			borderstyleMedium			=  6,
			borderstyleMediumDashDot	=  7,
			borderstyleMediumDashDotDot	=  8,
			borderstyleMediumDashed		=  9,
			borderstyleNone				= 10,
			borderstyleSlantDashDot		= 11,
			borderstyleThick			= 12,
			borderstyleThin				= 13
		};
		template<EBorderStyle eDefValue = borderstyleNone>
		class CBorderStyle : public CSimpleType<EBorderStyle, eDefValue>
		{
		public:
			CBorderStyle() {}
			CBorderStyle(const EBorderStyle & val)	{ this->m_eValue = val; }

            virtual EBorderStyle FromString(std::wstring &sValue)
			{
				if(L"dashDot" == sValue)
                    this->m_eValue = borderstyleDashDot;
				else if(L"dashDotDot" == sValue)
                    this->m_eValue = borderstyleDashDotDot;
				else if(L"dashed" == sValue)
                    this->m_eValue = borderstyleDashed;
				else if(L"dotted" == sValue)
                    this->m_eValue = borderstyleDotted;
				else if(L"double" == sValue)
                    this->m_eValue = borderstyleDouble;
				else if(L"hair" == sValue)
                    this->m_eValue = borderstyleHair;
				else if(L"medium" == sValue)
                    this->m_eValue = borderstyleMedium;
				else if(L"mediumDashDot" == sValue)
                    this->m_eValue = borderstyleMediumDashDot;
				else if(L"mediumDashDotDot" == sValue)
                    this->m_eValue = borderstyleMediumDashDotDot;
				else if(L"mediumDashed" == sValue)
                    this->m_eValue = borderstyleMediumDashed;
				else if(L"none" == sValue)
                    this->m_eValue = borderstyleNone;
				else if(L"slantDashDot" == sValue)
                    this->m_eValue = borderstyleSlantDashDot;
				else if(L"thick" == sValue)
                    this->m_eValue = borderstyleThick;
				else if(L"thin" == sValue)
                    this->m_eValue = borderstyleThin;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case borderstyleDashDot : return L"dashDot";break;
				case borderstyleDashDotDot : return L"dashDotDot";break;
				case borderstyleDashed : return L"dashed";break;
				case borderstyleDotted : return L"dotted";break;
				case borderstyleDouble : return L"double";break;
				case borderstyleHair : return L"hair";break;
				case borderstyleMedium : return L"medium";break;
				case borderstyleMediumDashDot : return L"mediumDashDot";break;
				case borderstyleMediumDashDotDot : return L"mediumDashDotDot";break;
				case borderstyleMediumDashed : return L"mediumDashed";break;
				case borderstyleNone : return L"none";break;
				case borderstyleSlantDashDot : return L"slantDashDot";break;
				case borderstyleThick : return L"thick";break;
				case borderstyleThin : return L"thin";break;
				default:return L"none";
				}
			}

			SimpleType_FromString     (EBorderStyle)
			SimpleType_Operator_Equal (CBorderStyle)
		};

		enum EHorizontalAlignment
		{
			horizontalalignmentCenter			=  0,
			horizontalalignmentContinuous		=  1,
			horizontalalignmentDistributed		=  2,
			horizontalalignmentFill				=  3,
			horizontalalignmentGeneral			=  4,
			horizontalalignmentJustify			=  5,
			horizontalalignmentLeft				=  6,
			horizontalalignmentRight			=  7,
			horizontalalignmentCenterContinuous	=  8
		};
		template<EHorizontalAlignment eDefValue = horizontalalignmentGeneral>
		class CHorizontalAlignment : public CSimpleType<EHorizontalAlignment, eDefValue>
		{
		public:
			CHorizontalAlignment() {}
			CHorizontalAlignment(const EHorizontalAlignment & val)	{ this->m_eValue = val; }

            virtual EHorizontalAlignment FromString(std::wstring &sValue)
			{
				// JustifyDistributed

				if(L"center" == sValue || L"Center" == sValue)
                    this->m_eValue = horizontalalignmentCenter;
				else if (L"centerContinuous" == sValue || L"CenterAcrossSelection" == sValue)
					this->m_eValue = horizontalalignmentCenterContinuous;
				else if(L"continuous" == sValue)
                    this->m_eValue = horizontalalignmentContinuous;
				else if(L"distributed" == sValue || L"Distributed" == sValue)
                    this->m_eValue = horizontalalignmentDistributed;
				else if(L"fill" == sValue || L"Fill" == sValue)
                    this->m_eValue = horizontalalignmentFill;
				else if(L"general" == sValue || L"Automatic" == sValue)
                    this->m_eValue = horizontalalignmentGeneral;
				else if(L"justify" == sValue || L"Justify" == sValue)
                    this->m_eValue = horizontalalignmentJustify;
				else if(L"left" == sValue || L"Left" == sValue)
                    this->m_eValue = horizontalalignmentLeft;
				else if(L"right" == sValue || L"Right" == sValue)
                    this->m_eValue = horizontalalignmentRight;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case horizontalalignmentCenter : return L"center"; break;
				case horizontalalignmentContinuous : return L"continuous"; break;
				case horizontalalignmentDistributed : return L"distributed"; break;
				case horizontalalignmentFill : return L"fill"; break;
				case horizontalalignmentGeneral : return L"general"; break;
				case horizontalalignmentJustify : return L"justify"; break;
				case horizontalalignmentLeft : return L"left"; break;
				case horizontalalignmentRight : return L"right"; break;
				case horizontalalignmentCenterContinuous: return L"centerContinuous"; break;
				default : return L"general";
				}
			}
			SimpleType_FromString     (EHorizontalAlignment)
			SimpleType_Operator_Equal (CHorizontalAlignment)
		};
		enum EVerticalAlignment
		{
			verticalalignmentBottom				=  0,
			verticalalignmentCenter				=  1,
			verticalalignmentDistributed		=  2,
			verticalalignmentJustify			=  3,
			verticalalignmentTop				=  4
		};
		template<EVerticalAlignment eDefValue = verticalalignmentBottom>
		class CVerticalAlignment : public CSimpleType<EVerticalAlignment, eDefValue>
		{
		public:
			CVerticalAlignment() {}
			CVerticalAlignment(const EVerticalAlignment & val)	{ this->m_eValue = val; }

            virtual EVerticalAlignment FromString(std::wstring &sValue)
			{
				//Automatic, JustifyDistributed
				if(L"bottom" == sValue || L"Bottom" == sValue)
                    this->m_eValue = verticalalignmentBottom;
				else if(L"center" == sValue || L"Center" == sValue)
                    this->m_eValue = verticalalignmentCenter;
				else if(L"distributed" == sValue || L"Distributed" == sValue)
                    this->m_eValue = verticalalignmentDistributed;
				else if(L"justify" == sValue || L"Justify" == sValue)
                    this->m_eValue = verticalalignmentJustify;
				else if(L"top" == sValue || L"Top" == sValue)
                    this->m_eValue = verticalalignmentTop;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}
			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case verticalalignmentBottom : return L"bottom"; break;
				case verticalalignmentCenter : return L"center"; break;
				case verticalalignmentDistributed : return L"distributed"; break;
				case verticalalignmentJustify : return L"justify"; break;
				case verticalalignmentTop : return L"top"; break;
				default:return L"bottom";
				}
			}
			SimpleType_FromString     (EVerticalAlignment)
			SimpleType_Operator_Equal (CVerticalAlignment)
		};
		enum EGradientType
		{
			gradienttypeLine				=  0,
			gradienttypePath				=  1
		};
		template<EGradientType eDefValue = gradienttypeLine>
		class CGradientType : public CSimpleType<EGradientType, eDefValue>
		{
		public:
			CGradientType() {}
			CGradientType(const EGradientType & val)	{ this->m_eValue = val; }

            virtual EGradientType FromString(std::wstring &sValue)
			{
				if(L"line" == sValue)
                    this->m_eValue = gradienttypeLine;
				else if(L"path" == sValue)
                    this->m_eValue = gradienttypePath;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
				switch(this->m_eValue)
				{
				case gradienttypeLine : return L"line"; break;
				case gradienttypePath : return L"path"; break;
				default:return L"line";
				}
			}

			SimpleType_FromString     (EGradientType)
			SimpleType_Operator_Equal (CGradientType)
		};
		enum EPatternType
		{
			patterntypeDarkDown				=  0,
			patterntypeDarkGray				=  1,
			patterntypeDarkGrid				=  2,
			patterntypeDarkHorizontal		=  3,
			patterntypeDarkTrellis			=  4,
			patterntypeDarkUp				=  5,
			patterntypeDarkVertical			=  6,
			patterntypeGray0625				=  7,
			patterntypeGray125				=  8,
			patterntypeLightDown			=  9,
			patterntypeLightGray			= 10,
			patterntypeLightGrid			= 11,
			patterntypeLightHorizontal		= 12,
			patterntypeLightTrellis			= 13,
			patterntypeLightUp			= 14,
			patterntypeLightVertical	= 15,
			patterntypeMediumGray		= 16,
			patterntypeNone					= 17,
			patterntypeSolid				= 18
		};
		template<EPatternType eDefValue = patterntypeNone>
		class CPatternType : public CSimpleType<EPatternType, eDefValue>
		{
		public:
			CPatternType() {}
			CPatternType(const EPatternType & val)	{ this->m_eValue = val; }

            virtual EPatternType FromString(std::wstring &sValue)
			{
				if(L"darkDown" == sValue)
                    this->m_eValue = patterntypeDarkDown;
				else if(L"darkGray" == sValue)
                    this->m_eValue = patterntypeDarkGray;
				else if(L"darkGrid" == sValue)
                    this->m_eValue = patterntypeDarkGrid;
				else if(L"darkHorizontal" == sValue)
                    this->m_eValue = patterntypeDarkHorizontal;
				else if(L"darkTrellis" == sValue)
                    this->m_eValue = patterntypeDarkTrellis;
				else if(L"darkUp" == sValue)
                    this->m_eValue = patterntypeDarkUp;
				else if(L"darkVertical" == sValue || L"VertStripe" == sValue)
                    this->m_eValue = patterntypeDarkVertical;
				else if(L"gray0625" == sValue || L"Gray0625" == sValue )
                    this->m_eValue = patterntypeGray0625;
				else if(L"gray125" == sValue || L"Gray125" == sValue)
                    this->m_eValue = patterntypeGray125;
				else if(L"lightDown" == sValue)
                    this->m_eValue = patterntypeLightDown;
				else if(L"lightGray" == sValue)
                    this->m_eValue = patterntypeLightGray;
				else if(L"lightGrid" == sValue || L"ThinHorzCross" == sValue)
                    this->m_eValue = patterntypeLightGrid;
				else if(L"lightHorizontal" == sValue || L"HorzStripe" == sValue)
                    this->m_eValue = patterntypeLightHorizontal;
				else if(L"lightTrellis" == sValue)
                    this->m_eValue = patterntypeLightTrellis;
				else if(L"lightUp" == sValue)
                    this->m_eValue = patterntypeLightUp;
				else if(L"lightVertical" == sValue)
                    this->m_eValue = patterntypeLightVertical;
				else if(L"mediumGray" == sValue || L"Gray50" == sValue)
                    this->m_eValue = patterntypeMediumGray;
				else if(L"none" == sValue || L"None" == sValue)
                    this->m_eValue = patterntypeNone;
				else if(L"solid" == sValue || L"Solid" == sValue)
                    this->m_eValue = patterntypeSolid;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring ToString  () const 
			{
                switch(this->m_eValue)
				{
					case patterntypeDarkDown:return L"darkDown";break;
					case patterntypeDarkGray:return L"darkGray";break;
					case patterntypeDarkGrid:return L"darkGrid";break;
					case patterntypeDarkHorizontal:return L"darkHorizontal";break;
					case patterntypeDarkTrellis:return L"darkTrellis";break;
					case patterntypeDarkUp:return L"darkUp";break;
					case patterntypeDarkVertical:return L"darkVertical";break;
					case patterntypeGray0625:return L"gray0625";break;
					case patterntypeGray125:return L"gray125";break;
					case patterntypeLightDown:return L"lightDown";break;
					case patterntypeLightGray:return L"lightGray";break;
					case patterntypeLightGrid:return L"lightGrid";break;
					case patterntypeLightHorizontal:return L"lightHorizontal";break;
					case patterntypeLightTrellis:return L"lightTrellis";break;
					case patterntypeLightUp:return L"lightUp";break;
					case patterntypeLightVertical:return L"lightVertical";break;
					case patterntypeMediumGray:return L"mediumGray";break;
					case patterntypeNone:return L"none";break;
				case patterntypeSolid:return L"solid";break;
				default: return L"none";
				}

				return L"none";
			}

			SimpleType_FromString     (EPatternType)
			SimpleType_Operator_Equal (CPatternType)
		};

		enum ETableStyleType
		{
			tablestyletypeBlankRow			=  0,
			tablestyletypeFirstColumn			=  1,
			tablestyletypeFirstColumnStripe			=  2,
			tablestyletypeFirstColumnSubheading			=  3,
			tablestyletypeFirstHeaderCell			=  4,
			tablestyletypeFirstRowStripe			=  5,
			tablestyletypeFirstRowSubheading			=  6,
			tablestyletypeFirstSubtotalColumn			=  7,
			tablestyletypeFirstSubtotalRow			=  8,
			tablestyletypeFirstTotalCell			=  9,
			tablestyletypeHeaderRow			=  10,
			tablestyletypeLastColumn			=  11,
			tablestyletypeLastHeaderCell			=  12,
			tablestyletypeLastTotalCell			=  13,
			tablestyletypePageFieldLabels			=  14,
			tablestyletypePageFieldValues			=  15,
			tablestyletypeSecondColumnStripe			=  16,
			tablestyletypeSecondColumnSubheading			=  17,
			tablestyletypeSecondRowStripe			=  18,
			tablestyletypeSecondRowSubheading			=  19,
			tablestyletypeSecondSubtotalColumn			=  20,
			tablestyletypeSecondSubtotalRow			=  21,
			tablestyletypeThirdColumnSubheading			=  22,
			tablestyletypeThirdRowSubheading			=  23,
			tablestyletypeThirdSubtotalColumn			=  24,
			tablestyletypeThirdSubtotalRow			=  25,
			tablestyletypeTotalRow			=  26,
			tablestyletypeWholeTable			=  27
		};
		template<ETableStyleType eDefValue = tablestyletypeBlankRow>
		class CTableStyleType : public CSimpleType<ETableStyleType, eDefValue>
		{
		public:
			CTableStyleType() {}
			CTableStyleType(const ETableStyleType & val)	{ this->m_eValue = val; }

            virtual ETableStyleType FromString(std::wstring &sValue)
			{
				if(L"blankRow" == sValue)
                    this->m_eValue = tablestyletypeBlankRow;
				else if(L"firstColumn" == sValue)
                    this->m_eValue = tablestyletypeFirstColumn;
				else if(L"firstColumnStripe" == sValue)
                    this->m_eValue = tablestyletypeFirstColumnStripe;
				else if(L"firstColumnSubheading" == sValue)
                    this->m_eValue = tablestyletypeFirstColumnSubheading;
				else if(L"firstHeaderCell" == sValue)
                    this->m_eValue = tablestyletypeFirstHeaderCell;
				else if(L"firstRowStripe" == sValue)
                    this->m_eValue = tablestyletypeFirstRowStripe;
				else if(L"firstRowSubheading" == sValue)
                    this->m_eValue = tablestyletypeFirstRowSubheading;
				else if(L"firstSubtotalColumn" == sValue)
                    this->m_eValue = tablestyletypeFirstSubtotalColumn;
				else if(L"firstSubtotalRow" == sValue)
                    this->m_eValue = tablestyletypeFirstSubtotalRow;
				else if(L"firstTotalCell" == sValue)
                    this->m_eValue = tablestyletypeFirstTotalCell;
				else if(L"headerRow" == sValue)
                    this->m_eValue = tablestyletypeHeaderRow;
				else if(L"lastColumn" == sValue)
                    this->m_eValue = tablestyletypeLastColumn;
				else if(L"lastHeaderCell" == sValue)
                    this->m_eValue = tablestyletypeLastHeaderCell;
				else if(L"lastTotalCell" == sValue)
                    this->m_eValue = tablestyletypeLastTotalCell;
				else if(L"pageFieldLabels" == sValue)
                    this->m_eValue = tablestyletypePageFieldLabels;
				else if(L"pageFieldValues" == sValue)
                    this->m_eValue = tablestyletypePageFieldValues;
				else if(L"secondColumnStripe" == sValue)
                    this->m_eValue = tablestyletypeSecondColumnStripe;
				else if(L"secondColumnSubheading" == sValue)
                    this->m_eValue = tablestyletypeSecondColumnSubheading;
				else if(L"secondRowStripe" == sValue)
                    this->m_eValue = tablestyletypeSecondRowStripe;
				else if(L"secondRowSubheading" == sValue)
                    this->m_eValue = tablestyletypeSecondRowSubheading;
				else if(L"secondSubtotalColumn" == sValue)
                    this->m_eValue = tablestyletypeSecondSubtotalColumn;
				else if(L"secondSubtotalRow" == sValue)
                    this->m_eValue = tablestyletypeSecondSubtotalRow;
				else if(L"thirdColumnSubheading" == sValue)
                    this->m_eValue = tablestyletypeThirdColumnSubheading;
				else if(L"thirdRowSubheading" == sValue)
                    this->m_eValue = tablestyletypeThirdRowSubheading;
				else if(L"thirdSubtotalColumn" == sValue)
                    this->m_eValue = tablestyletypeThirdSubtotalColumn;
				else if(L"thirdSubtotalRow" == sValue)
                    this->m_eValue = tablestyletypeThirdSubtotalRow;
				else if(L"totalRow" == sValue)
                    this->m_eValue = tablestyletypeTotalRow;
				else if(L"wholeTable" == sValue)
                    this->m_eValue = tablestyletypeWholeTable;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case tablestyletypeBlankRow: return L"blankRow";break;
				case tablestyletypeFirstColumn: return L"firstColumn";break;
				case tablestyletypeFirstColumnStripe: return L"firstColumnStripe";break;
				case tablestyletypeFirstColumnSubheading: return L"firstColumnSubheading";break;
				case tablestyletypeFirstHeaderCell: return L"firstHeaderCell";break;
				case tablestyletypeFirstRowStripe: return L"firstRowStripe";break;
				case tablestyletypeFirstRowSubheading: return L"firstRowSubheading";break;
				case tablestyletypeFirstSubtotalColumn: return L"firstSubtotalColumn";break;
				case tablestyletypeFirstSubtotalRow: return L"firstSubtotalRow";break;
				case tablestyletypeFirstTotalCell: return L"firstTotalCell";break;
				case tablestyletypeHeaderRow: return L"headerRow";break;
				case tablestyletypeLastColumn: return L"lastColumn";break;
				case tablestyletypeLastHeaderCell: return L"lastHeaderCell";break;
				case tablestyletypeLastTotalCell: return L"lastTotalCell";break;
				case tablestyletypePageFieldLabels: return L"pageFieldLabels";break;
				case tablestyletypePageFieldValues: return L"pageFieldValues";break;
				case tablestyletypeSecondColumnStripe: return L"secondColumnStripe";break;
				case tablestyletypeSecondColumnSubheading: return L"secondColumnSubheading";break;
				case tablestyletypeSecondRowStripe: return L"secondRowStripe";break;
				case tablestyletypeSecondRowSubheading: return L"secondRowSubheading";break;
				case tablestyletypeSecondSubtotalColumn: return L"secondSubtotalColumn";break;
				case tablestyletypeSecondSubtotalRow: return L"secondSubtotalRow";break;
				case tablestyletypeThirdColumnSubheading: return L"thirdColumnSubheading";break;
				case tablestyletypeThirdRowSubheading: return L"thirdRowSubheading";break;
				case tablestyletypeThirdSubtotalColumn: return L"thirdSubtotalColumn";break;
				case tablestyletypeThirdSubtotalRow: return L"thirdSubtotalRow";break;
				case tablestyletypeTotalRow: return L"totalRow";break;
				case tablestyletypeWholeTable: return L"wholeTable";break;
				default : return L"blankRow";break;
				}
				return L"blankRow";
			}

			SimpleType_FromString     (ETableStyleType)
			SimpleType_Operator_Equal (CTableStyleType)
		};
		enum ECellFormulaType
		{
			cellformulatypeArray			=  0,
			cellformulatypeDataTable		=  1,
			cellformulatypeNormal			=  2,
			cellformulatypeShared			=  3
		};
		template<ECellFormulaType eDefValue = cellformulatypeNormal>
		class CCellFormulaType : public CSimpleType<ECellFormulaType, eDefValue>
		{
		public:
			CCellFormulaType() {}
			CCellFormulaType(const ECellFormulaType & val)	{ this->m_eValue = val; }

            virtual ECellFormulaType FromString(std::wstring &sValue)
			{
				if(L"array" == sValue)
                    this->m_eValue = cellformulatypeArray;
				else if(L"dataTable" == sValue)
                    this->m_eValue = cellformulatypeDataTable;
				else if(L"normal" == sValue)
                    this->m_eValue = cellformulatypeNormal;
				else if(L"shared" == sValue)
                    this->m_eValue = cellformulatypeShared;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}
			ECellFormulaType FromStringA(const char* sValue)
			{
				if(strcmp("array", sValue) == 0)
					this->m_eValue = cellformulatypeArray;
				else if(strcmp("dataTable", sValue) == 0)
					this->m_eValue = cellformulatypeDataTable;
				else if(strcmp("normal", sValue) == 0)
					this->m_eValue = cellformulatypeNormal;
				else if(strcmp("shared", sValue) == 0)
					this->m_eValue = cellformulatypeShared;
				else
					this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case cellformulatypeArray : return L"array";break;
				case cellformulatypeDataTable : return L"dataTable";break;
				case cellformulatypeNormal : return L"normal";break;
				case cellformulatypeShared : return L"shared";break;
				default: return L"normal";
				}
			}

			SimpleType_FromString     (ECellFormulaType)
			SimpleType_Operator_Equal (CCellFormulaType)
		};
		enum EUpdateLinksType
		{
			updatelinksAlways			=  0,
			updatelinksNever			=  1,
			updatelinksUserSet			=  2
		};
		template<EUpdateLinksType eDefValue = updatelinksAlways>
		class CUpdateLinksType : public CSimpleType<EUpdateLinksType, eDefValue>
		{
		public:
			CUpdateLinksType() {}
			CUpdateLinksType(const EUpdateLinksType & val)	{ this->m_eValue = val; }

            virtual EUpdateLinksType FromString(std::wstring &sValue)
			{
				if(L"always" == sValue)
                    this->m_eValue = updatelinksAlways;
				else if(L"never" == sValue)
                    this->m_eValue = updatelinksNever;
				else if(L"userSet" == sValue)
                    this->m_eValue = updatelinksUserSet;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
				switch(this->m_eValue)
				{
				case updatelinksAlways: return L"always";break;
				case updatelinksNever: return L"never";break;
				case updatelinksUserSet: return L"userSet";break;
				default: return L"always";
				}
			}

			SimpleType_FromString     (EUpdateLinksType)
			SimpleType_Operator_Equal (CUpdateLinksType)
		};
		enum ECellTypeType
		{
			celltypeBool			=  0,
			celltypeDate			=  1,
			celltypeError			=  2,
			celltypeInlineStr		=  3,
			celltypeNumber			=  4,
			celltypeSharedString	=  5,
			celltypeStr				=  6,
//--------------- for our using
			celltypePercentage		= 7,
			celltypeScientific		= 8,
			celltypeFraction		= 9,
			celltypeTime			= 10,
			celltypeCurrency		= 11,
			celltypeDateTime		= 12
		};
		template<ECellTypeType eDefValue = celltypeNumber>
		class CCellTypeType : public CSimpleType<ECellTypeType, eDefValue>
		{
		public:
			CCellTypeType() {}
			CCellTypeType(const ECellTypeType & val)	{ this->m_eValue = val; }

            virtual ECellTypeType FromString(std::wstring &sValue)
			{
				if(L"b" == sValue)
                    this->m_eValue = celltypeBool;
				else if(L"d" == sValue || L"DateTime" == sValue)
                    this->m_eValue = celltypeDate;
				else if(L"e" == sValue)
                    this->m_eValue = celltypeError;
				else if(L"inlineStr" == sValue)
                    this->m_eValue = celltypeInlineStr;
				else if(L"n" == sValue || L"Number" == sValue)
                    this->m_eValue = celltypeNumber;
				else if(L"s" == sValue)
                    this->m_eValue = celltypeSharedString;
				else if(L"str" == sValue || L"String" == sValue)
                    this->m_eValue = celltypeStr;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}
			ECellTypeType FromStringA(const char* sValue)
			{
				if(strcmp("s", sValue) == 0)
					this->m_eValue = celltypeSharedString;
				else if(strcmp("str", sValue) == 0)
					this->m_eValue = celltypeStr;
				else if(strcmp("n", sValue) == 0)
					this->m_eValue = celltypeNumber;
				else if(strcmp("e", sValue) == 0)
					this->m_eValue = celltypeError;
				else if(strcmp("b", sValue) == 0)
					this->m_eValue = celltypeBool;
				else if(strcmp("inlineStr", sValue) == 0)
					this->m_eValue = celltypeInlineStr;
				else if(strcmp("d", sValue) == 0)
					this->m_eValue = celltypeDate;
				else
					this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case celltypeBool: return L"b";break;
				case celltypeDate: return L"d";break;
				case celltypeError: return L"e";break;
				case celltypeInlineStr: return L"inlineStr";break;
				case celltypeNumber: return L"n";break;
				case celltypeSharedString: return L"s";break;
				case celltypeStr: return L"str";break;
				default: return L"n";
				}
			}

			SimpleType_FromString     (ECellTypeType)
			SimpleType_Operator_Equal (CCellTypeType)
		};
		enum ECellAnchorType
		{
			cellanchorAbsolute			=  0,
			cellanchorOneCell			=  1,
			cellanchorTwoCell			=  2
		};
		template<ECellAnchorType eDefValue = cellanchorTwoCell>
		class CCellAnchorType : public CSimpleType<ECellAnchorType, eDefValue>
		{
		public:
			CCellAnchorType() {}
			CCellAnchorType(const ECellAnchorType & val)	{ this->m_eValue = val; }

            virtual ECellAnchorType FromString(std::wstring &sValue)
			{
				if(L"absolute" == sValue)
                    this->m_eValue = cellanchorAbsolute;
				else if(L"oneCell" == sValue)
                    this->m_eValue = cellanchorOneCell;
				else if(L"twoCell" == sValue)
                    this->m_eValue = cellanchorTwoCell;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case cellanchorAbsolute: return L"absolute"; break;
				case cellanchorOneCell: return L"oneCell"; break;
				case cellanchorTwoCell: return L"twoCell"; break;
				default: return L"twoCell";
				}
			}

			SimpleType_FromString     (ECellAnchorType)
			SimpleType_Operator_Equal (CCellAnchorType)
		};

		enum ESheetViewType
		{
			sheetviewNormal				=  0,
			sheetviewPageBreakPreview	=  1,
			sheetviewPageLayout			=  2
		};
		template<ESheetViewType eDefValue = sheetviewNormal>
		class CSheetViewType : public CSimpleType<ESheetViewType, eDefValue>
		{
		public:
			CSheetViewType() {}
			CSheetViewType(const ESheetViewType & val)	{ this->m_eValue = val; }

            virtual ESheetViewType FromString(std::wstring &sValue)
			{
				if(L"normal" == sValue)
                    this->m_eValue = sheetviewNormal;
				else if(L"pageBreakPreview" == sValue)
                    this->m_eValue = sheetviewPageBreakPreview;
				else if(L"pageLayout" == sValue)
                    this->m_eValue = sheetviewPageLayout;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case sheetviewNormal: return L"normal"; break;
				case sheetviewPageBreakPreview: return L"pageBreakPreview"; break;
				case sheetviewPageLayout: return L"pageLayout"; break;
				default: return L"normal";
				}
			}

			SimpleType_FromString     (ESheetViewType)
			SimpleType_Operator_Equal (CSheetViewType)
		};
		enum EChartLegendPos
		{
			chartlegendposLeft		=  0,
			chartlegendposTop		=  1,
			chartlegendposRight		=  2,
			chartlegendposBottom	=  3,
			chartlegendposRightTop	=  4
		};
		template<EChartLegendPos eDefValue = chartlegendposTop>
		class CChartLegendPos : public CSimpleType<EChartLegendPos, eDefValue>
		{
		public:
			CChartLegendPos() {}
			CChartLegendPos(const EChartLegendPos & val)	{ this->m_eValue = val; }

            virtual EChartLegendPos FromString(std::wstring &sValue)
			{
				if(L"l" == sValue)
                    this->m_eValue = chartlegendposLeft;
				else if(L"t" == sValue)
                    this->m_eValue = chartlegendposTop;
				else if(L"r" == sValue)
                    this->m_eValue = chartlegendposRight;
				else if(L"b" == sValue)
                    this->m_eValue = chartlegendposBottom;
				else if(L"tr" == sValue)
                    this->m_eValue = chartlegendposRightTop;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case chartlegendposLeft: return L"l"; break;
				case chartlegendposTop: return L"t"; break;
				case chartlegendposRight: return L"r"; break;
				case chartlegendposBottom: return L"b"; break;
				case chartlegendposRightTop: return L"tr"; break;
				default: return L"t";
				}
			}

			SimpleType_FromString     (EChartLegendPos)
			SimpleType_Operator_Equal (CChartLegendPos)
		};
		enum EChartHMode
		{
			charthmodeFactor	=  0,
			charthmodeEdge		=  1
		};
		template<EChartHMode eDefValue = charthmodeFactor>
		class CChartHMode : public CSimpleType<EChartHMode, eDefValue>
		{
		public:
			CChartHMode() {}
			CChartHMode(const EChartHMode & val)	{ this->m_eValue = val; }

            virtual EChartHMode FromString(std::wstring &sValue)
			{
				if(L"factor" == sValue)
                    this->m_eValue = charthmodeFactor;
				else if(L"edge" == sValue)
                    this->m_eValue = charthmodeEdge;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case charthmodeFactor: return L"factor"; break;
				case charthmodeEdge: return L"edge"; break;
				default: return L"factor";
				}
			}

			SimpleType_FromString     (EChartHMode)
			SimpleType_Operator_Equal (CChartHMode)
		};
		enum EChartLayoutTarget
		{
			chartlayouttargetInner	=  0,
			chartlayouttargetOuter	=  1
		};
		template<EChartLayoutTarget eDefValue = chartlayouttargetInner>
		class CChartLayoutTarget : public CSimpleType<EChartLayoutTarget, eDefValue>
		{
		public:
			CChartLayoutTarget() {}
			CChartLayoutTarget(const EChartLayoutTarget & val)	{ this->m_eValue = val; }

            virtual EChartLayoutTarget FromString(std::wstring &sValue)
			{
				if(L"inner" == sValue)
                    this->m_eValue = chartlayouttargetInner;
				else if(L"outer" == sValue)
                    this->m_eValue = chartlayouttargetOuter;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case chartlayouttargetInner: return L"inner"; break;
				case chartlayouttargetOuter: return L"outer"; break;
				default: return L"inner";
				}
			}

			SimpleType_FromString     (EChartLayoutTarget)
			SimpleType_Operator_Equal (CChartLayoutTarget)
		};
		enum EChartAxPos
		{
			chartaxposLeft	=  0,
			chartaxposTop	=  1,
			chartaxposRight	=  2,
			chartaxposBottom=  3
		};
		template<EChartAxPos eDefValue = chartaxposLeft>
		class CChartAxPos : public CSimpleType<EChartAxPos, eDefValue>
		{
		public:
			CChartAxPos() {}
			CChartAxPos(const EChartAxPos & val)	{ this->m_eValue = val; }

            virtual EChartAxPos FromString(std::wstring &sValue)
			{
				if(L"l" == sValue)
                    this->m_eValue = chartaxposLeft;
				else if(L"t" == sValue)
                    this->m_eValue = chartaxposTop;
				else if(L"r" == sValue)
                    this->m_eValue = chartaxposRight;
				else if(L"b" == sValue)
                    this->m_eValue = chartaxposBottom;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case chartaxposLeft: return L"l"; break;
				case chartaxposTop: return L"t"; break;
				case chartaxposRight: return L"r"; break;
				case chartaxposBottom: return L"b"; break;
				default: return L"l";
				}
			}

			SimpleType_FromString     (EChartAxPos)
				SimpleType_Operator_Equal (CChartAxPos)
		};
		enum EChartBarGrouping
		{
			chartbargroupingClustered	=  0,
			chartbargroupingPercentStacked	=  1,
			chartbargroupingStacked	=  2,
			chartbargroupingStandard=  3
		};
		template<EChartBarGrouping eDefValue = chartbargroupingClustered>
		class CChartBarGrouping : public CSimpleType<EChartBarGrouping, eDefValue>
		{
		public:
			CChartBarGrouping() {}
			CChartBarGrouping(const EChartBarGrouping & val)	{ this->m_eValue = val; }

            virtual EChartBarGrouping FromString(std::wstring &sValue)
			{
				if(L"clustered" == sValue)
                    this->m_eValue = chartbargroupingClustered;
				else if(L"percentStacked" == sValue)
                    this->m_eValue = chartbargroupingPercentStacked;
				else if(L"stacked" == sValue)
                    this->m_eValue = chartbargroupingStacked;
				else if(L"standard" == sValue)
                    this->m_eValue = chartbargroupingStandard;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case chartbargroupingClustered: return L"clustered"; break;
				case chartbargroupingPercentStacked: return L"percentStacked"; break;
				case chartbargroupingStacked: return L"stacked"; break;
				case chartbargroupingStandard: return L"standard"; break;
				default: return L"clustered";
				}
			}

			SimpleType_FromString     (EChartBarGrouping)
			SimpleType_Operator_Equal (CChartBarGrouping)
		};
		enum EChartBarDirection
		{
			chartbardirectionBar	=  0,
			chartbardirectionCol	=  1
		};
		template<EChartBarDirection eDefValue = chartbardirectionBar>
		class CChartBarDirection : public CSimpleType<EChartBarDirection, eDefValue>
		{
		public:
			CChartBarDirection() {}
			CChartBarDirection(const EChartBarDirection & val)	{ this->m_eValue = val; }

            virtual EChartBarDirection FromString(std::wstring &sValue)
			{
				if(L"bar" == sValue)
                    this->m_eValue = chartbardirectionBar;
				else if(L"col" == sValue)
                    this->m_eValue = chartbardirectionCol;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case chartbardirectionBar: return L"bar"; break;
				case chartbardirectionCol: return L"col"; break;
				default: return L"bar";
				}
			}

			SimpleType_FromString     (EChartBarDirection)
			SimpleType_Operator_Equal (CChartBarDirection)
		};
		enum EChartSymbol
		{
			chartsymbolCircle	=  0,
			chartsymbolDash	=  1,
			chartsymbolDiamond	=  2,
			chartsymbolDot	=  3,
			chartsymbolNone	=  4,
			chartsymbolPicture =  5,
			chartsymbolPlus	=  6,
			chartsymbolSquare	=  7,
			chartsymbolStare	=  8,
			chartsymbolStar	=  9,
			chartsymbolTriangle	=  10,
			chartsymbolX	=  11
		};
		template<EChartSymbol eDefValue = chartsymbolNone>
		class CChartSymbol : public CSimpleType<EChartSymbol, eDefValue>
		{
		public:
			CChartSymbol() {}
			CChartSymbol(const EChartSymbol & val)	{ this->m_eValue = val; }

            virtual EChartSymbol FromString(std::wstring &sValue)
			{
				if(L"circle" == sValue)
                    this->m_eValue = chartsymbolCircle;
				else if(L"dash" == sValue)
                    this->m_eValue = chartsymbolDash;
				else if(L"diamond" == sValue)
                    this->m_eValue = chartsymbolDiamond;
				else if(L"dot" == sValue)
                    this->m_eValue = chartsymbolDot;
				else if(L"none" == sValue)
                    this->m_eValue = chartsymbolNone;
				else if(L"picture" == sValue)
                    this->m_eValue = chartsymbolPicture;
				else if(L"plus" == sValue)
                    this->m_eValue = chartsymbolPlus;
				else if(L"square" == sValue)
                    this->m_eValue = chartsymbolSquare;
				else if(L"star" == sValue)
                    this->m_eValue = chartsymbolStare;
				else if(L"triangle" == sValue)
                    this->m_eValue = chartsymbolTriangle;
				else if(L"x" == sValue)
                    this->m_eValue = chartsymbolX;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
				case chartsymbolCircle: return L"circle"; break;
				case chartsymbolDash: return L"dash"; break;
				case chartsymbolDiamond: return L"diamond"; break;
				case chartsymbolDot: return L"dot"; break;
				case chartsymbolNone: return L"none"; break;
				case chartsymbolPicture: return L"picture"; break;
				case chartsymbolPlus: return L"plus"; break;
				case chartsymbolSquare: return L"square"; break;
				case chartsymbolStare: return L"star"; break;
				case chartsymbolTriangle: return L"triangle"; break;
				case chartsymbolX: return L"x"; break;
				default: return L"none";
				}
			}

			SimpleType_FromString     (EChartSymbol)
			SimpleType_Operator_Equal (CChartSymbol)
		};
		enum EPageSize
		{
			pagesizeLetterPaper	=  1,
			pagesizeLetterSmall	=  2,
			pagesizeTabloidPaper =  3,
			pagesizeLedgerPaper =  4,
			pagesizeLegalPaper =  5,
			pagesizeStatementPaper =  6,
			pagesizeExecutivePaper =  7,
			pagesizeA3Paper =  8,
			pagesizeA4Paper =  9,
			pagesizeA4SmallPaper =  10,
			pagesizeA5Paper =  11,
			pagesizeB4Paper =  12,
			pagesizeB5Paper =  13,
			pagesizeFolioPaper =  14,
			pagesizeQuartoPaper =  15,
			pagesizeStandardPaper1 =  16,
			pagesizeStandardPaper2 =  17,
			pagesizeNotePaper =  18,
			pagesize9Envelope =  19,
			pagesize10Envelope =  20,
			pagesize11Envelope =  21,
			pagesize12Envelope =  22,
			pagesize14Envelope =  23,
			pagesizeCPaper =  24,
			pagesizeDPaper =  25,
			pagesizeEPaper =  26,
			pagesizeDLEnvelope =  27,
			pagesizeC5Envelope =  28,
			pagesizeC3Envelope =  29,
			pagesizeC4Envelope =  30,
			pagesizeC6Envelope =  31,
			pagesizeC65Envelope =  32,
			pagesizeB4Envelope =  33,
			pagesizeB5Envelope =  34,
			pagesizeB6Envelope =  35,
			pagesizeItalyEnvelope =  36,
			pagesizeMonarchEnvelope =  37,
			pagesize6_3_4Envelope =  38,
			pagesizeUSStandardFanfold =  39,
			pagesizeGermanStandardFanfold =  40,
			pagesizeGermanLegalFanfold =  41,
			pagesizeISOB4 =  42,
			pagesizeJapaneseDoublePostcard =  43,
			pagesizeStandardPaper3 =  44,
			pagesizeStandardPaper4 =  45,
			pagesizeStandardPaper5 =  46,
			pagesizeInviteEnvelope =  47,
			pagesizeLetterExtraPaper =  50,
			pagesizeLegalExtraPaper =  51,
			pagesizeTabloidExtraPaper =  52,
			pagesizeA4ExtraPaper =  53,
			pagesizeLetterTransversePaper =  54,
			pagesizeA4TransversePaper =  55,
			pagesizeLetterExtraTransversePaper =  56,
			pagesizeSuperA_SuperA_A4Paper =  57,
			pagesizeSuperB_SuperB_A3Paper =  58,
			pagesizeLetterPlusPaper =  59,
			pagesizeA4PlusPaper =  60,
			pagesizeA5TransversePaper =  61,
			pagesizeJISB5TransversePaper =  62,
			pagesizeA3ExtraPaper =  63,
			pagesizeA5ExtraPaper =  64,
			pagesizeISOB5ExtraPaper =  65,
			pagesizeA2Paper =  66,
			pagesizeA3TransversePaper =  67,
			pagesizeA3ExtraTransversePaper =  68
		};
		template<EPageSize eDefValue = pagesizeA4Paper>
		class CPageSize : public CSimpleType<EPageSize, eDefValue>
		{
		public:
			CPageSize() {}
			CPageSize(const EPageSize & val)	{ this->m_eValue = val; }

            virtual EPageSize FromString(std::wstring &sValue)
			{
                int nCharset = _wtoi(sValue.c_str());
                this->m_eValue = (EPageSize)nCharset;
                return this->m_eValue;
			}

			virtual std::wstring ToString  () const 
			{
                return std::to_wstring(this->m_eValue );
			}

			SimpleType_FromString     (EPageSize)
			SimpleType_Operator_Equal (CPageSize)
		};
		
		enum EPageUnits
		{
			mm		= 0,
			cm		= 1,
			inch	= 2,
			pt		= 3,
			px		= 4,
			emu		= 5

		};
		template<EPageUnits eDefValue = mm>
		class CPageUnits : public CSimpleType<EPageUnits, eDefValue>
		{
		public:
			CPageUnits() {}
			CPageUnits(const EPageUnits & val)	{ this->m_eValue = val; }

            virtual EPageUnits FromString(std::wstring &sValue)
			{
					 if	(sValue == L"in")	this->m_eValue = inch;
				else if (sValue == L"mm")	this->m_eValue = mm;
				else if (sValue == L"cm")	this->m_eValue = cm;
				else if (sValue == L"pt")	this->m_eValue = pt;
				else if (sValue == L"px")	this->m_eValue = px;

                return this->m_eValue;
			}

			virtual std::wstring ToString  () const 
			{
                std::wstring sResult;
                switch(this->m_eValue)
				{
                case mm:	sResult = L"mm";break;
                case cm:	sResult = L"cm";break;
                case pt:	sResult = L"pt";break;
                case px:	sResult = L"px";break;
                case inch:	sResult = L"in";break;
				}
				return sResult;
			}

			SimpleType_FromString     (EPageUnits)
			SimpleType_Operator_Equal (CPageUnits)
		};

		enum ECellComments
		{
			cellcommentsNone = 0,
			cellcommentsAsDisplayed = 1,
			cellcommentsAtEnd = 2
		};

		template<ECellComments eDefValue = cellcommentsNone>
		class CCellComments : public CSimpleType<ECellComments, eDefValue>
		{
		public:
			CCellComments() {}
                        CCellComments(const ECellComments & val)	{ this->m_eValue = val; }
			virtual ECellComments FromString(std::wstring &sValue)
			{
				if      (L"asDisplayed" == sValue ) this->m_eValue = cellcommentsAsDisplayed;
				else if (L"atEnd" ==  sValue ) this->m_eValue = cellcommentsAtEnd;
				else if (L"none" ==  sValue ) this->m_eValue = cellcommentsNone;
				else this->m_eValue = eDefValue;

				return this->m_eValue;
			}

			virtual std::wstring          ToString  () const
			{
				switch(this->m_eValue)
				{
				case cellcommentsAsDisplayed : return L"asDisplayed";
				case cellcommentsAtEnd : return L"atEnd";
				case cellcommentsNone : return L"none";
				default : return L"none";
				}
			}

			SimpleType_FromString     (ECellComments)
			SimpleType_Operator_Equal (CCellComments)
		};

		enum EPrintError
		{
			printerrorDisplayed  = 0,
			printerrorBlank = 1,
			printerrorDash  = 2,
			printerrorNA  = 3
		};

		template<EPrintError eDefValue = printerrorDisplayed>
		class CPrintError : public CSimpleType<EPrintError, eDefValue>
		{
		public:
			CPrintError() {}
                        CPrintError(const EPrintError & val)	{ this->m_eValue = val; }
			virtual EPrintError FromString(std::wstring &sValue)
			{
				if      (L"blank" == sValue ) this->m_eValue = printerrorBlank;
				else if (L"dash" ==  sValue ) this->m_eValue = printerrorDash;
				else if (L"displayed" ==  sValue ) this->m_eValue = printerrorDisplayed;
				else if (L"NA" ==  sValue ) this->m_eValue = printerrorNA;
				else this->m_eValue = eDefValue;

				return this->m_eValue;
			}

			virtual std::wstring          ToString  () const
			{
				switch(this->m_eValue)
				{
				case printerrorBlank: return L"blank";
				case printerrorDash: return L"dash";
				case printerrorDisplayed: return L"displayed";
				case printerrorNA: return L"NA";
				default: return L"displayed";
				}
			}

			SimpleType_FromString     (EPrintError)
			SimpleType_Operator_Equal (CPrintError)
		};

		enum EPageOrder
		{
			pageorderDownThenOver = 0,
			pageorderOverThenDown  = 1
		};

		template<EPageOrder eDefValue = pageorderDownThenOver>
		class CPageOrder : public CSimpleType<EPageOrder, eDefValue>
		{
		public:
			CPageOrder() {}
                        CPageOrder(const EPageOrder & val)	{ this->m_eValue = val; }
			virtual EPageOrder FromString(std::wstring &sValue)
			{
				if      (L"downThenOver" == sValue ) this->m_eValue = pageorderDownThenOver;
				else if (L"overThenDown" ==  sValue ) this->m_eValue = pageorderOverThenDown;
				else this->m_eValue = eDefValue;

				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
				case pageorderDownThenOver : return L"downThenOver";
				case pageorderOverThenDown  : return L"overThenDown";
				default : return L"downThenOver";
				}
			}

			SimpleType_FromString     (EPageOrder)
			SimpleType_Operator_Equal (CPageOrder)
		};
		enum ETotalsRowFunction
		{
			totalrowfunctionAverage		=  1,
			totalrowfunctionCount		=  2,
			totalrowfunctionCountNums	=  3,
			totalrowfunctionCustom		=  4,
			totalrowfunctionMax			=  5,
			totalrowfunctionMin			=  6,
			totalrowfunctionNone		=  7,
			totalrowfunctionStdDev		=  8,
			totalrowfunctionSum			=  9,
			totalrowfunctionVar			=  10
		};

		template<ETotalsRowFunction eDefValue = totalrowfunctionNone>
		class CTotalsRowFunction : public CSimpleType<ETotalsRowFunction, eDefValue>
		{
		public:
			CTotalsRowFunction() {}
			CTotalsRowFunction(const ETotalsRowFunction & val)	{ this->m_eValue = val; }

            virtual ETotalsRowFunction FromString(std::wstring &sValue)
			{
				if(L"average" == sValue)
                    this->m_eValue = totalrowfunctionAverage;
				else if(L"count" == sValue)
                    this->m_eValue = totalrowfunctionCount;
				else if(L"countNums" == sValue)
                    this->m_eValue = totalrowfunctionCountNums;
				else if(L"custom" == sValue)
                    this->m_eValue = totalrowfunctionCustom;
				else if(L"max" == sValue)
                    this->m_eValue = totalrowfunctionMax;
				else if(L"min" == sValue)
                    this->m_eValue = totalrowfunctionMin;
				else if(L"none" == sValue)
                    this->m_eValue = totalrowfunctionNone;
				else if(L"stdDev" == sValue)
                    this->m_eValue = totalrowfunctionStdDev;
				else if(L"sum" == sValue)
                    this->m_eValue = totalrowfunctionSum;
				else if(L"var" == sValue)
                    this->m_eValue = totalrowfunctionVar;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                std::wstring sResult;
                switch(this->m_eValue)
				{
                case totalrowfunctionAverage: sResult = L"average";break;
                case totalrowfunctionCount: sResult = L"count";break;
                case totalrowfunctionCountNums: sResult = L"countNums";break;
                case totalrowfunctionCustom: sResult = L"custom";break;
                case totalrowfunctionMax: sResult = L"max";break;
                case totalrowfunctionMin: sResult = L"min";break;
                case totalrowfunctionNone: sResult = L"none";break;
                case totalrowfunctionStdDev: sResult = L"stdDev";break;
                case totalrowfunctionSum: sResult = L"sum";break;
                case totalrowfunctionVar: sResult = L"var";break;
				}
				return sResult;
			}

			SimpleType_FromString     (ETotalsRowFunction)
			SimpleType_Operator_Equal (CTotalsRowFunction)
		};
		enum ESortBy
		{
			sortbyCellColor	=  1,
			sortbyFontColor =  2,
			sortbyIcon =  3,
			sortbyValue =  4
		};
		template<ESortBy eDefValue = sortbyValue>
		class CSortBy : public CSimpleType<ESortBy, eDefValue>
		{
		public:
			CSortBy() {}
			CSortBy(const ESortBy & val)	{ this->m_eValue = val; }

            virtual ESortBy FromString(std::wstring &sValue)
			{
				if(L"cellColor" == sValue)
                    this->m_eValue = sortbyCellColor;
				else if(L"fontColor" == sValue)
                    this->m_eValue = sortbyFontColor;
				else if(L"icon" == sValue)
                    this->m_eValue = sortbyIcon;
				else if(L"value" == sValue)
                    this->m_eValue = sortbyValue;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                std::wstring sResult;
                switch(this->m_eValue)
				{
                case sortbyCellColor:   sResult = L"cellColor"; break;
                case sortbyFontColor:   sResult = L"fontColor"; break;
                case sortbyIcon:        sResult = L"icon";      break;
                case sortbyValue:       sResult = L"value";     break;
				}
				return sResult;
			}

			SimpleType_FromString     (ESortBy)
				SimpleType_Operator_Equal (CSortBy)
		};
		enum ECustomFilter
		{
			customfilterEqual	=  1,
			customfilterGreaterThan =  2,
			customfilterGreaterThanOrEqual =  3,
			customfilterLessThan =  4,
			customfilterLessThanOrEqual =  5,
			customfilterNotEqual =  6
		};
		template<ECustomFilter eDefValue = customfilterEqual>
		class CCustomFilter : public CSimpleType<ECustomFilter, eDefValue>
		{
		public:
			CCustomFilter() {}
			CCustomFilter(const ECustomFilter & val)	{ this->m_eValue = val; }

            virtual ECustomFilter FromString(std::wstring &sValue)
			{
				if(L"equal" == sValue)
                    this->m_eValue = customfilterEqual;
				else if(L"greaterThan" == sValue)
                    this->m_eValue = customfilterGreaterThan;
				else if(L"greaterThanOrEqual" == sValue)
                    this->m_eValue = customfilterGreaterThanOrEqual;
				else if(L"lessThan" == sValue)
                    this->m_eValue = customfilterLessThan;
				else if(L"lessThanOrEqual" == sValue)
                    this->m_eValue = customfilterLessThanOrEqual;
				else if(L"notEqual" == sValue)
                    this->m_eValue = customfilterNotEqual;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                std::wstring sResult;
                switch(this->m_eValue)
				{
                case customfilterEqual: sResult = L"equal";break;
                case customfilterGreaterThan: sResult = L"greaterThan";break;
                case customfilterGreaterThanOrEqual: sResult = L"greaterThanOrEqual";break;
                case customfilterLessThan: sResult = L"lessThan";break;
                case customfilterLessThanOrEqual: sResult = L"lessThanOrEqual";break;
                case customfilterNotEqual: sResult = L"notEqual";break;
				}
				return sResult;
			}

			SimpleType_FromString     (ECustomFilter)
			SimpleType_Operator_Equal (CCustomFilter)
		};
		enum EDateTimeGroup
		{
			datetimegroupDay	=  1,
			datetimegroupHour =  2,
			datetimegroupMinute =  3,
			datetimegroupMonth =  4,
			datetimegroupSecond =  5,
			datetimegroupYear =  6
		}; 
		template<EDateTimeGroup eDefValue = datetimegroupDay>
		class CDateTimeGroup : public CSimpleType<EDateTimeGroup, eDefValue>
		{
		public:
			CDateTimeGroup() {}
			CDateTimeGroup(const EDateTimeGroup & val)	{ this->m_eValue = val; }

            virtual EDateTimeGroup FromString(std::wstring &sValue)
			{
				if(L"day" == sValue)
                    this->m_eValue = datetimegroupDay;
				else if(L"hour" == sValue)
                    this->m_eValue = datetimegroupHour;
				else if(L"minute" == sValue)
                    this->m_eValue = datetimegroupMinute;
				else if(L"month" == sValue)
                    this->m_eValue = datetimegroupMonth;
				else if(L"second" == sValue)
                    this->m_eValue = datetimegroupSecond;
				else if(L"year" == sValue)
                    this->m_eValue = datetimegroupYear;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                std::wstring sResult;
                switch(this->m_eValue)
				{
                case datetimegroupDay:      sResult = L"day";       break;
                case datetimegroupHour:     sResult = L"hour";      break;
                case datetimegroupMinute:   sResult = L"minute";    break;
                case datetimegroupMonth:    sResult = L"month";     break;
                case datetimegroupSecond:   sResult = L"second";    break;
                case datetimegroupYear:     sResult = L"year";      break;
				}
				return sResult;
			}

			SimpleType_FromString     (EDateTimeGroup)
            SimpleType_Operator_Equal (CDateTimeGroup)
		};
		enum ESortMethod
		{
			sortmethodNone = 1,
			sortmethodPinYin = 2,
			sortmethodStroke = 3,
		};
		template<ESortMethod eDefValue = sortmethodNone>
		class CSortMethod : public CSimpleType<ESortMethod, eDefValue>
		{
		public:
			CSortMethod() {}
			CSortMethod(const ESortMethod & val)	{ this->m_eValue = val; }

			virtual ESortMethod FromString(std::wstring &sValue)
			{
				if(L"none" == sValue)
					this->m_eValue = sortmethodNone;
				else if(L"pinYin" == sValue)
					this->m_eValue = sortmethodPinYin;
				else if(L"stroke" == sValue)
					this->m_eValue = sortmethodStroke;
				else
					this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring     ToString  () const
			{
				std::wstring sResult;
				switch(this->m_eValue)
				{
				case sortmethodNone: sResult = L"none";break;
				case sortmethodPinYin: sResult = L"pinYin";break;
				case sortmethodStroke: sResult = L"stroke";break;
				}
				return sResult;
			}

			SimpleType_FromString     (ESortMethod)
			SimpleType_Operator_Equal (CSortMethod)
		};
		enum EDynamicFilterType
		{
			dynamicfiltertypeAboveAverage = 1,
			dynamicfiltertypeBelowAverage = 2,
			dynamicfiltertypeLastMonth = 3,
			dynamicfiltertypeLastQuarter = 4,
			dynamicfiltertypeLastWeek = 5,
			dynamicfiltertypeLastYear = 6,
			dynamicfiltertypeM1 = 7,
			dynamicfiltertypeM10 = 8,
			dynamicfiltertypeM11 = 9,
			dynamicfiltertypeM12 = 10,
			dynamicfiltertypeM2 = 11,
			dynamicfiltertypeM3 = 12,
			dynamicfiltertypeM4 = 13,
			dynamicfiltertypeM5 = 14,
			dynamicfiltertypeM6 = 15,
			dynamicfiltertypeM7 = 16,
			dynamicfiltertypeM8 = 17,
			dynamicfiltertypeM9 = 18,
			dynamicfiltertypeNextMonth = 19,
			dynamicfiltertypeNextQuarter = 20,
			dynamicfiltertypeNextWeek = 21,
			dynamicfiltertypeNextYear = 22,
			dynamicfiltertypeNull = 23,
			dynamicfiltertypeQ1 = 24,
			dynamicfiltertypeQ2 = 25,
			dynamicfiltertypeQ3 = 26,
			dynamicfiltertypeQ4 = 27,
			dynamicfiltertypeThisMonth = 28,
			dynamicfiltertypeThisQuarter = 29,
			dynamicfiltertypeThisWeek = 30,
			dynamicfiltertypeThisYear = 31,
			dynamicfiltertypeToday = 32,
			dynamicfiltertypeTomorrow = 33,
			dynamicfiltertypeYearToDate = 34,
			dynamicfiltertypeYesterday = 35
		}; 
		template<EDynamicFilterType eDefValue = dynamicfiltertypeNull>
		class CDynamicFilterType : public CSimpleType<EDynamicFilterType, eDefValue>
		{
		public:
			CDynamicFilterType() {}
			CDynamicFilterType(const EDynamicFilterType & val)	{ this->m_eValue = val; }

            virtual EDynamicFilterType FromString(std::wstring &sValue)
			{
				if(L"aboveAverage" == sValue)
                    this->m_eValue = dynamicfiltertypeAboveAverage;
				else if(L"belowAverage" == sValue)
                    this->m_eValue = dynamicfiltertypeBelowAverage;
				else if(L"lastMonth" == sValue)
                    this->m_eValue = dynamicfiltertypeLastMonth;
				else if(L"lastQuarter" == sValue)
                    this->m_eValue = dynamicfiltertypeLastQuarter;
				else if(L"lastWeek" == sValue)
                    this->m_eValue = dynamicfiltertypeLastWeek;
				else if(L"lastYear" == sValue)
                    this->m_eValue = dynamicfiltertypeLastYear;
				else if(L"M1" == sValue)
                    this->m_eValue = dynamicfiltertypeM1;
				else if(L"M10" == sValue)
                    this->m_eValue = dynamicfiltertypeM10;
				else if(L"M11" == sValue)
                    this->m_eValue = dynamicfiltertypeM11;
				else if(L"M12" == sValue)
                    this->m_eValue = dynamicfiltertypeM12;
				else if(L"M2" == sValue)
                    this->m_eValue = dynamicfiltertypeM2;
				else if(L"M3" == sValue)
                    this->m_eValue = dynamicfiltertypeM3;
				else if(L"M4" == sValue)
                    this->m_eValue = dynamicfiltertypeM4;
				else if(L"M5" == sValue)
                    this->m_eValue = dynamicfiltertypeM5;
				else if(L"M6" == sValue)
                    this->m_eValue = dynamicfiltertypeM6;
				else if(L"M7" == sValue)
                    this->m_eValue = dynamicfiltertypeM7;
				else if(L"M8" == sValue)
                    this->m_eValue = dynamicfiltertypeM8;
				else if(L"M9" == sValue)
                    this->m_eValue = dynamicfiltertypeM9;
				else if(L"nextMonth" == sValue)
                    this->m_eValue = dynamicfiltertypeNextMonth;
				else if(L"nextQuarter" == sValue)
                    this->m_eValue = dynamicfiltertypeNextQuarter;
				else if(L"nextWeek" == sValue)
                    this->m_eValue = dynamicfiltertypeNextWeek;
				else if(L"nextYear" == sValue)
                    this->m_eValue = dynamicfiltertypeNextYear;
				else if(L"null" == sValue)
                    this->m_eValue = dynamicfiltertypeNull;
				else if(L"Q1" == sValue)
                    this->m_eValue = dynamicfiltertypeQ1;
				else if(L"Q2" == sValue)
                    this->m_eValue = dynamicfiltertypeQ2;
				else if(L"Q3" == sValue)
                    this->m_eValue = dynamicfiltertypeQ3;
				else if(L"Q4" == sValue)
                    this->m_eValue = dynamicfiltertypeQ4;
				else if(L"thisMonth" == sValue)
                    this->m_eValue = dynamicfiltertypeThisMonth;
				else if(L"thisQuarter" == sValue)
                    this->m_eValue = dynamicfiltertypeThisQuarter;
				else if(L"thisWeek" == sValue)
                    this->m_eValue = dynamicfiltertypeThisWeek;
				else if(L"thisYear" == sValue)
                    this->m_eValue = dynamicfiltertypeThisYear;
				else if(L"today" == sValue)
                    this->m_eValue = dynamicfiltertypeToday;
				else if(L"tomorrow" == sValue)
                    this->m_eValue = dynamicfiltertypeTomorrow;
				else if(L"yearToDate" == sValue)
                    this->m_eValue = dynamicfiltertypeYearToDate;
				else if(L"yesterday" == sValue)
                    this->m_eValue = dynamicfiltertypeYesterday;
				else
                    this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                std::wstring sResult;
                switch(this->m_eValue)
				{
                case dynamicfiltertypeAboveAverage: sResult = L"aboveAverage";break;
                case dynamicfiltertypeBelowAverage: sResult = L"belowAverage";break;
                case dynamicfiltertypeLastMonth: sResult = L"lastMonth";break;
                case dynamicfiltertypeLastQuarter: sResult = L"lastQuarter";break;
                case dynamicfiltertypeLastWeek: sResult = L"lastWeek";break;
                case dynamicfiltertypeLastYear: sResult = L"lastYear";break;
                case dynamicfiltertypeM1: sResult = L"M1";break;
                case dynamicfiltertypeM10: sResult = L"M10";break;
                case dynamicfiltertypeM11: sResult = L"M11";break;
                case dynamicfiltertypeM12: sResult = L"M12";break;
                case dynamicfiltertypeM2: sResult = L"M2";break;
                case dynamicfiltertypeM3: sResult = L"M3";break;
                case dynamicfiltertypeM4: sResult = L"M4";break;
                case dynamicfiltertypeM5: sResult = L"M5";break;
                case dynamicfiltertypeM6: sResult = L"M6";break;
                case dynamicfiltertypeM7: sResult = L"M7";break;
                case dynamicfiltertypeM8: sResult = L"M8";break;
                case dynamicfiltertypeM9: sResult = L"M9";break;
                case dynamicfiltertypeNextMonth: sResult = L"nextMonth";break;
                case dynamicfiltertypeNextQuarter: sResult = L"nextQuarter";break;
                case dynamicfiltertypeNextWeek: sResult = L"nextWeek";break;
                case dynamicfiltertypeNextYear: sResult = L"nextYear";break;
                case dynamicfiltertypeNull: sResult = L"null";break;
                case dynamicfiltertypeQ1: sResult = L"Q1";break;
                case dynamicfiltertypeQ2: sResult = L"Q2";break;
                case dynamicfiltertypeQ3: sResult = L"Q3";break;
                case dynamicfiltertypeQ4: sResult = L"Q4";break;
                case dynamicfiltertypeThisMonth: sResult = L"thisMonth";break;
                case dynamicfiltertypeThisQuarter: sResult = L"thisQuarter";break;
                case dynamicfiltertypeThisWeek: sResult = L"thisWeek";break;
                case dynamicfiltertypeThisYear: sResult = L"thisYear";break;
                case dynamicfiltertypeToday: sResult = L"today";break;
                case dynamicfiltertypeTomorrow: sResult = L"tomorrow";break;
                case dynamicfiltertypeYearToDate: sResult = L"yearToDate";break;
                case dynamicfiltertypeYesterday: sResult = L"yesterday";break;
				}
				return sResult;
			}

			SimpleType_FromString     (EDynamicFilterType)
			SimpleType_Operator_Equal (CDynamicFilterType)
		};
	//--------------------------------------------------------
	//	ST_IconSetType (18.18.42)
	//--------------------------------------------------------
		enum EIconSetType
		{
			Arrows3				= 0,
			Arrows3Gray			= 1,
			Flags3				= 2,
			Signs3				= 3,
			Symbols3			= 4,
			Symbols3_2			= 5,
			Traffic3Lights1		= 6,
			Traffic3Lights2		= 7,
			Arrows4				= 8,
			Arrows4Gray			= 9,
			Rating4				= 10,
			RedToBlack4			= 11,
			Traffic4Lights		= 12,
			Arrows5				= 13,
			Arrows5Gray			= 14,
			Quarters5			= 15,
			Rating5				= 16,
			Triangles3			= 17,
			Stars3				= 18,
			Boxes5				= 19,
			NoIcons				= 20


		};
		template<EIconSetType eDefValue = Arrows3>
		class ST_IconSetType : public CSimpleType<EIconSetType, eDefValue>
		{
		public:
			ST_IconSetType() {}
			ST_IconSetType(const EIconSetType & val)	{ this->m_eValue = val; }

            virtual EIconSetType FromString(std::wstring &sValue)
			{
                if(L"3Arrows" == sValue)				this->m_eValue = Arrows3;
                else if(L"3ArrowsGray" == sValue)		this->m_eValue = Arrows3Gray;
                else if(L"3Flags" == sValue)			this->m_eValue = Flags3;
                else if(L"3Signs" == sValue)			this->m_eValue = Signs3;
                else if(L"3Symbols" == sValue)			this->m_eValue = Symbols3;
                else if(L"3Symbols2" == sValue)			this->m_eValue = Symbols3_2;
                else if(L"3TrafficLights1" == sValue)	this->m_eValue = Traffic3Lights1;
                else if(L"3TrafficLights2" == sValue)	this->m_eValue = Traffic3Lights2;
                else if(L"4Arrows" == sValue)			this->m_eValue = Arrows4;
                else if(L"4ArrowsGray" == sValue)		this->m_eValue = Arrows4Gray;
                else if(L"4Rating" == sValue)			this->m_eValue = Rating4;
                else if(L"4RedToBlack" == sValue)		this->m_eValue = RedToBlack4;
                else if(L"4TrafficLights" == sValue)	this->m_eValue = Traffic4Lights;
                else if(L"5Arrows" == sValue)			this->m_eValue = Arrows5;
                else if(L"5ArrowsGray" == sValue)		this->m_eValue = Arrows5Gray;
                else if(L"5Quarters" == sValue)			this->m_eValue = Quarters5;
		//..ext....
                else if(L"5Rating" == sValue)			this->m_eValue = Rating5;
                else if(L"3Triangles" == sValue)		this->m_eValue = Triangles3;
				else if(L"3Stars" == sValue)			this->m_eValue = Stars3;
				else if(L"5Boxes" == sValue)			this->m_eValue = Boxes5;
				else if(L"NoIcons" == sValue)			this->m_eValue = NoIcons;
                else 									this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
					case Arrows3		:	return L"3Arrows";			break;
					case Arrows3Gray	:	return L"3ArrowsGray";		break;
					case Flags3			:	return L"3Flags";			break;
					case Signs3			:	return L"3Signs";			break;
					case Symbols3		:	return L"3Symbols";			break;
					case Symbols3_2		:	return L"3Symbols2";		break;
					case Traffic3Lights1:	return L"3TrafficLights1";	break;
					case Traffic3Lights2:	return L"3TrafficLights2";	break;
					case Arrows4		:	return L"4Arrows";			break;
					case Arrows4Gray	:	return L"4ArrowsGray";		break;
					case Rating4		:	return L"4Rating";			break;
					case RedToBlack4	:	return L"4RedToBlack";		break;
					case Traffic4Lights	:	return L"4TrafficLights";	break;
					case Arrows5		:	return L"5Arrows";			break;
					case Arrows5Gray	:	return L"5ArrowsGray";		break;
					case Quarters5		:	return L"5Quarters";		break;
					case Rating5		:	return L"5Rating";			break;
					case Triangles3		:	return L"3Triangles";		break;
					case Stars3			:	return L"3Stars";			break;
					case Boxes5			:	return L"5Boxes";			break;
					case NoIcons		:	return L"NoIcons";			break;
					default				:	return L"3Arrows";
				}
			}

			SimpleType_FromString     (EIconSetType)
			SimpleType_Operator_Equal (ST_IconSetType)
		};	
		
	//--------------------------------------------------------
	//	ST_DataBarAxisPosition
	//--------------------------------------------------------
		enum EDataBarAxisPosition
		{
			automatic_pos	= 0,
			middle_pos		= 1,
			none_pos		= 2
		};
		template<EDataBarAxisPosition eDefValue = automatic_pos>
		class ST_DataBarAxisPosition : public CSimpleType<EDataBarAxisPosition, eDefValue>
		{
		public:
			ST_DataBarAxisPosition() {}
			ST_DataBarAxisPosition(const EDataBarAxisPosition & val)	{ this->m_eValue = val; }

            virtual EDataBarAxisPosition FromString(std::wstring &sValue)
			{
				if(L"automatic" == sValue)		this->m_eValue = automatic_pos;
                else if(L"middle" == sValue)	this->m_eValue = middle_pos;
                else if(L"none" == sValue)		this->m_eValue = none_pos;
				else 								this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring ToString  () const 
			{
                switch(this->m_eValue)
				{
					case automatic_pos	:	return L"automatic";break;
					case middle_pos		:	return L"middle";	break;
					case none_pos		:	return L"none";		break;
					default				:	return L"automatic";
				}
			}

			SimpleType_FromString     (EDataBarAxisPosition)
			SimpleType_Operator_Equal (ST_DataBarAxisPosition)
		};

	//--------------------------------------------------------
	//	ST_DataBarDirection 
	//--------------------------------------------------------
		enum EDataBarDirection
		{
			context_direction	= 0,
			leftToRight	= 1,
			rightToLeft	= 2
		};
		template<EDataBarDirection eDefValue = context_direction>
		class ST_DataBarDirection : public CSimpleType<EDataBarDirection, eDefValue>
		{
		public:
			ST_DataBarDirection() {}
			ST_DataBarDirection(const EDataBarDirection & val)	{ this->m_eValue = val; }

            virtual EDataBarDirection FromString(std::wstring &sValue)
			{
					 if(L"context"		== sValue)	this->m_eValue = context_direction;
                else if(L"leftToRight"	== sValue)	this->m_eValue = leftToRight;
                else if(L"rightToLeft"	== sValue)	this->m_eValue = rightToLeft;
				else 									this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring ToString  () const 
			{
                switch(this->m_eValue)
				{
					case context_direction	:	return L"context";
					case leftToRight		:	return L"leftToRight";
					case rightToLeft		:	return L"rightToLeft";
					default					:	return L"automatic";
				}
			}

			SimpleType_FromString     (EDataBarDirection)
			SimpleType_Operator_Equal (ST_DataBarDirection)
		};
	//----------------------------------------------------
	//	18.18.15 ST_CfOperator (Conditional Format Operators)
	//----------------------------------------------------
		enum ECfOperator
		{
			Operator_beginsWith			= 0,
			Operator_between			= 1,
			Operator_containsText		= 2,
			Operator_endsWith			= 3,
			Operator_equal				= 4,
			Operator_greaterThan		= 5,
			Operator_greaterThanOrEqual	= 6,
			Operator_lessThan			= 7,
			Operator_lessThanOrEqual	= 8,
			Operator_notBetween			= 9,
			Operator_notContains		= 10,
			Operator_notEqual			= 11
		};
		template<ECfOperator eDefValue = Operator_equal>
		class ST_CfOperator : public CSimpleType<ECfOperator, eDefValue>
		{
		public:
			ST_CfOperator() {}
			ST_CfOperator(const ECfOperator & val)	{ this->m_eValue = val; }

            virtual ECfOperator FromString(std::wstring &sValue)
			{
                if(L"beginsWith" == sValue)				this->m_eValue = Operator_beginsWith;
                else if(L"between" == sValue)			this->m_eValue = Operator_between;
                else if(L"containsText" == sValue)		this->m_eValue = Operator_containsText;
                else if(L"endsWith" == sValue)			this->m_eValue = Operator_endsWith;
                else if(L"equal" == sValue)				this->m_eValue = Operator_equal;
                else if(L"greaterThan" == sValue)		this->m_eValue = Operator_greaterThan;
                else if(L"greaterThanOrEqual" == sValue)	this->m_eValue = Operator_greaterThanOrEqual;
                else if(L"lessThan" == sValue)			this->m_eValue = Operator_lessThan;
                else if(L"lessThanOrEqual" == sValue)	this->m_eValue = Operator_lessThanOrEqual;
                else if(L"notBetween" == sValue)			this->m_eValue = Operator_notBetween;
                else if(L"notContains" == sValue)		this->m_eValue = Operator_notContains;
                else if(L"notEqual" == sValue)			this->m_eValue = Operator_notEqual;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
					case Operator_beginsWith		:	return L"beginsWith";		break;			
					case Operator_between			:	return L"between";			break;
					case Operator_containsText		:	return L"containsText";		break;
					case Operator_endsWith			:	return L"endsWith";			break;
					case Operator_equal				:	return L"equal";				break;
					case Operator_greaterThan		:	return L"greaterThan";		break;
					case Operator_greaterThanOrEqual:	return L"greaterThanOrEqual";break;
					case Operator_lessThan			:	return L"lessThan";			break;
					case Operator_lessThanOrEqual	:	return L"lessThanOrEqual";	break;
					case Operator_notBetween		:	return L"notBetween";		break;
					case Operator_notContains		:	return L"notContains";		break;
					case Operator_notEqual			:	return L"notEqual";			break;
					default							:	return L"equal";
				}
			}

			SimpleType_FromString     (ECfOperator)
			SimpleType_Operator_Equal (ST_CfOperator)
		};

                //----------------------------------------------------
                //	18.18.82 ST_TimePeriod (Conditional Format Value Object Type)
                //----------------------------------------------------
                        enum ETimePeriod
                        {
                                last7Days					= 0,
                                lastMonth					= 1,
                                lastWeek					= 2,
                                nextMonth					= 3,
                                nextWeek					= 4,
                                thisMonth       				= 5,
                                thisWeek					= 6,
                                today                                           = 7,
                                tomorrow                                        = 8,
                                yesterday                                       = 9
                        };
                        template<ETimePeriod eDefValue = last7Days>
                        class ST_TimePeriod : public CSimpleType<ETimePeriod, eDefValue>
                        {
                        public:
                                ST_TimePeriod() {}
                                ST_TimePeriod(const ETimePeriod & val)	{ this->m_eValue = val; }

                    virtual ETimePeriod FromString(std::wstring &sValue)
                                {
                        if(_T("last7Days") == sValue)			this->m_eValue = last7Days;
                        else if(_T("lastMonth") == sValue)		this->m_eValue = lastMonth;
                        else if(_T("lastWeek") == sValue)		this->m_eValue = lastWeek;
                        else if(_T("nextMonth") == sValue)		this->m_eValue = nextMonth;
                        else if(_T("nextWeek") == sValue)               this->m_eValue = nextWeek;
                        else if(_T("thisMonth") == sValue)              this->m_eValue = thisMonth;
                        else if(_T("thisWeek") == sValue)               this->m_eValue = thisWeek;
                        else if(_T("today") == sValue)                  this->m_eValue = today;
                        else if(_T("tomorrow") == sValue)               this->m_eValue = tomorrow;
                        else if(_T("yesterday") == sValue)              this->m_eValue = yesterday;
                        else 								this->m_eValue = eDefValue;
                        return this->m_eValue;
                                }

                                virtual std::wstring     ToString  () const
                                {
                        switch(this->m_eValue)
                                        {
                                                case last7Days:	return _T("last7Days");
                                                case lastMonth:	return _T("lastMonth");
                                                case lastWeek:	return _T("lastWeek");
                                                case nextMonth:	return _T("nextMonth");
                                                case nextWeek:	return _T("nextWeek");
                                                case thisMonth: return _T("thisMonth");
                                                case thisWeek:	return _T("thisWeek");
                                                case today:	return _T("today");
                                                case tomorrow:	return _T("tomorrow");
                                                case yesterday:	return _T("yesterday");
                                                default		:	return _T("num");
                                        }
                                }

                                SimpleType_FromString     (ETimePeriod)
                                SimpleType_Operator_Equal (ST_TimePeriod)
                        };

	//----------------------------------------------------
	//	18.18.12 ST_CfType (Conditional Format Type)
	//----------------------------------------------------
		enum ECfType
		{
			aboveAverage			= 0,
			beginsWith				= 1,
			cellIs					= 2,
			colorScale				= 3,
			containsBlanks			= 4,
			containsErrors			= 5,
			containsText			= 6,
			dataBar					= 7,
			duplicateValues			= 8,
			expression				= 9,
			iconSet					= 10,
			notContainsBlanks		= 11,
			notContainsErrors		= 12,
			notContainsText			= 13,
			timePeriod				= 14,
			top10					= 15,
			uniqueValues			= 16,
			endsWith				= 17
		};
		template<ECfType eDefValue = dataBar>
		class ST_CfType : public CSimpleType<ECfType, eDefValue>
		{
		public:
			ST_CfType() {}
			ST_CfType(const ECfType & val)	{ this->m_eValue = val; }

            virtual ECfType FromString(std::wstring &sValue)
			{
                if(L"aboveAverage" == sValue)			this->m_eValue = aboveAverage;
                else if(L"beginsWith" == sValue)		this->m_eValue = beginsWith;
                else if(L"cellIs" == sValue)			this->m_eValue = cellIs;
                else if(L"colorScale" == sValue)		this->m_eValue = colorScale;
                else if(L"containsBlanks" == sValue)	this->m_eValue = containsBlanks;
                else if(L"containsText" == sValue)		this->m_eValue = containsText;
                else if(L"containsErrors" == sValue)	this->m_eValue = containsErrors;
                else if(L"dataBar" == sValue)			this->m_eValue = dataBar;
                else if(L"duplicateValues" == sValue)	this->m_eValue = duplicateValues;
                else if(L"expression" == sValue)		this->m_eValue = expression;
                else if(L"iconSet" == sValue)			this->m_eValue = iconSet;
                else if(L"notContainsBlanks" == sValue)	this->m_eValue = notContainsBlanks;
                else if(L"notContainsErrors" == sValue)	this->m_eValue = notContainsErrors;
                else if(L"notContainsText" == sValue)	this->m_eValue = notContainsText;
                else if(L"timePeriod" == sValue)		this->m_eValue = timePeriod;
                else if(L"top10" == sValue)				this->m_eValue = top10;
                else if(L"uniqueValues" == sValue)		this->m_eValue = uniqueValues;
                else if(L"endsWith" == sValue)			this->m_eValue = endsWith;
                else 									this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
					case aboveAverage		:	return L"aboveAverage";		break;			
					case beginsWith			:	return L"beginsWith";		break;
					case cellIs				:	return L"cellIs";			break;
					case colorScale			:	return L"colorScale";		break;
					case containsBlanks		:	return L"containsBlanks";	break;
					case containsErrors		:	return L"containsErrors";	break;
					case containsText		:	return L"containsText";		break;
					case dataBar			:	return L"dataBar";			break;
					case duplicateValues	:	return L"duplicateValues";	break;
					case expression			:	return L"expression";		break;
					case iconSet			:	return L"iconSet";			break;
					case notContainsBlanks	:	return L"notContainsBlanks";break;
					case notContainsErrors	:	return L"notContainsErrors";break;
					case notContainsText	:	return L"notContainsText";	break;
					case timePeriod			:	return L"timePeriod";		break;
					case top10				:	return L"top10";			break;
					case uniqueValues		:	return L"uniqueValues";		break;
					case endsWith			:	return L"endsWith";			break;
					default					:	return L"dataBar";
				}
			}

			SimpleType_FromString     (ECfType)
			SimpleType_Operator_Equal (ST_CfType)
		};

	//----------------------------------------------------
	//	18.18.13 ST_CfvoType (Conditional Format Value Object Type)
	//----------------------------------------------------
		enum ECfvoType
		{
			Formula					= 0,
			Maximum					= 1,
			Minimum					= 2,
			Number					= 3,
			Percent					= 4,
			Percentile				= 5,
			autoMin					= 6,
			autoMax					= 7
		};
		template<ECfvoType eDefValue = Number>
		class ST_CfvoType : public CSimpleType<ECfvoType, eDefValue>
		{
		public:
			ST_CfvoType() {}
			ST_CfvoType(const ECfvoType & val)	{ this->m_eValue = val; }

            virtual ECfvoType FromString(std::wstring &sValue)
			{
                if(L"formula" == sValue)		this->m_eValue = Formula;
                else if(L"max" == sValue)		this->m_eValue = Maximum;
                else if(L"min" == sValue)		this->m_eValue = Minimum;
                else if(L"num" == sValue)		this->m_eValue = Number;
                else if(L"percent" == sValue)	this->m_eValue = Percent;
                else if(L"percentile" == sValue)this->m_eValue = Percentile;
				else if(L"autoMin" == sValue)	this->m_eValue = autoMin;
                else if(L"autoMax" == sValue)	this->m_eValue = autoMax;
               else 							this->m_eValue = eDefValue;
                return this->m_eValue;
			}

			virtual std::wstring     ToString  () const 
			{
                switch(this->m_eValue)
				{
					case Formula:	return L"formula";
					case Maximum:	return L"max";		
					case Minimum:	return L"min";
					case Number:	return L"num";	
					case Percent:	return L"percent";
					case Percentile:return L"percentile";
					case autoMin:	return L"autoMin";
					case autoMax:	return L"autoMax";
					default		:	return L"num";
				}
			}

			SimpleType_FromString     (ECfvoType)
			SimpleType_Operator_Equal (ST_CfvoType)
		};

        enum ESparklineType
        {
            Line					= 0,
            Column					= 1,
            Stacked					= 2
        };
        template<ESparklineType eDefValue = Line>
        class ST_SparklineType : public CSimpleType<ESparklineType, eDefValue>
        {
        public:
			ST_SparklineType() {}
			ST_SparklineType(const ESparklineType & val)	{ this->m_eValue = val; }

            virtual ESparklineType FromString(std::wstring &sValue)
            {
                if(L"line" == sValue)			this->m_eValue = Line;
                else if(L"column" == sValue)	this->m_eValue = Column;
                else if(L"stacked" == sValue)	this->m_eValue = Stacked;
                else 							this->m_eValue = eDefValue;
                return this->m_eValue;
            }

            virtual std::wstring     ToString  () const
            {
                switch(this->m_eValue)
                {
                    case Line:		return L"line";		break;
                    case Column:	return L"column";	break;
                    case Stacked:	return L"stacked";	break;
                    default		:	return L"line";
                }
            }

            SimpleType_FromString     (ESparklineType)
            SimpleType_Operator_Equal (ST_SparklineType)
        };

        enum ESparklineAxisMinMax
        {
            Individual				= 0,
            Group					= 1,
            Custom					= 2
        };
        template<ESparklineAxisMinMax eDefValue = Individual>
        class ST_SparklineAxisMinMax : public CSimpleType<ESparklineAxisMinMax, eDefValue>
        {
        public:
			ST_SparklineAxisMinMax() {}
            ST_SparklineAxisMinMax(const ESparklineAxisMinMax & val)	{ this->m_eValue = val; }

            virtual ESparklineAxisMinMax FromString(std::wstring &sValue)
            {
                if(L"individual" == sValue)			this->m_eValue = Individual;
                else if(L"group" == sValue)		this->m_eValue = Group;
                else if(L"custom" == sValue)	this->m_eValue = Custom;
                else 								this->m_eValue = eDefValue;
                return this->m_eValue;
            }

            virtual std::wstring     ToString  () const
            {
                switch(this->m_eValue)
                {
                    case Individual: return L"individual";	break;
                    case Group:	return L"group";			break;
                    case Custom: return L"custom";			break;
                    default: return L"individual";
                }
            }

            SimpleType_FromString     (ESparklineAxisMinMax)
            SimpleType_Operator_Equal (ST_SparklineAxisMinMax)
        };
		enum EDvAspect
		{
			Content					= 0,
			Icon					= 1
		};
		template<EDvAspect eDefValue = Icon>
		class ST_DvAspect : public CSimpleType<EDvAspect, eDefValue>
		{
		public:
			ST_DvAspect() {}
			ST_DvAspect(const EDvAspect & val)	{ this->m_eValue = val; }

            virtual EDvAspect FromString(std::wstring &sValue)
			{
				if(L"DVASPECT_CONTENT" == sValue)	this->m_eValue = Content;
				else if(L"DVASPECT_ICON" == sValue)	this->m_eValue = Icon;
				else 								this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring	 ToString  () const
			{
				switch(this->m_eValue)
				{
					case Content: return L"DVASPECT_CONTENT";		break;
					case Icon:	return L"DVASPECT_ICON";			break;
					default: return L"DVASPECT_CONTENT";
				}
			}

			SimpleType_FromString	 (EDvAspect)
			SimpleType_Operator_Equal (ST_DvAspect)
		};
		enum EOleUpdate
		{
			Always = 0,
			OnCall = 1
		};
		template<EOleUpdate eDefValue = Always>
		class ST_OleUpdate : public CSimpleType<EOleUpdate, eDefValue>
		{
		public:
			ST_OleUpdate() {}
			ST_OleUpdate(const EOleUpdate & val)	{ this->m_eValue = val; }

            virtual EOleUpdate FromString(std::wstring &sValue)
			{
				if(L"OLEUPDATE_ALWAYS" == sValue) this->m_eValue = Always;
				else if(L"OLEUPDATE_ONCALL" == sValue) this->m_eValue = OnCall;
				else this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring	 ToString  () const
			{
				switch(this->m_eValue)
				{
					case Always: return L"OLEUPDATE_ALWAYS"; break;
					case OnCall: return L"OLEUPDATE_ONCALL"; break;
					default: return L"OLEUPDATE_ALWAYS";
				}
			}

			SimpleType_FromString	 (EOleUpdate)
			SimpleType_Operator_Equal (ST_OleUpdate)
		};

		enum EActivePane
		{
			activepaneBottomLeft = 0,
			activepaneBottomRight  = 1,
			activepaneTopLeft = 2,
			activepaneTopRight = 3
		};

		template<EActivePane eDefValue = activepaneTopLeft>
		class CActivePane : public CSimpleType<EActivePane, eDefValue>
		{
		public:
			CActivePane() {}
			CActivePane(const EActivePane & val)	{ this->m_eValue = val; }

            virtual EActivePane FromString(std::wstring &sValue)
			{
				if      ( L"bottomLeft" == sValue ) this->m_eValue = activepaneBottomLeft;
				else if ( L"bottomRight" ==  sValue ) this->m_eValue = activepaneBottomRight;
				else if ( L"topLeft" ==  sValue ) this->m_eValue = activepaneTopLeft;
				else if ( L"topRight" ==  sValue ) this->m_eValue = activepaneTopRight;
				else this->m_eValue = eDefValue;

				return this->m_eValue;
			}

			virtual std::wstring          ToString  () const
			{
				switch(this->m_eValue)
				{
				case activepaneBottomLeft : return L"bottomLeft";
				case activepaneBottomRight  : return L"bottomRight";
				case activepaneTopLeft  : return L"topLeft";
				case activepaneTopRight  : return L"topRight";
				default : return L"topLeft";
				}
			}

			SimpleType_FromString     (EActivePane)
			SimpleType_Operator_Equal (CActivePane)
		};

		enum EPaneState
		{
			panestateFrozen = 0,
			panestateFrozenSplit  = 1,
			panestateSplit = 2
		};

        template<EPaneState eDefValue = panestateFrozen>
        class CPaneState : public CSimpleType<EPaneState, eDefValue>
        {
        public:
			CPaneState() {}
			CPaneState(const EPaneState & val)	{ this->m_eValue = val; }

            virtual EPaneState FromString(std::wstring &sValue)
			{
				if      ( L"frozen" == sValue ) this->m_eValue = panestateFrozen;
				else if ( L"frozenSplit" ==  sValue ) this->m_eValue = panestateFrozenSplit;
				else if ( L"split" ==  sValue ) this->m_eValue = panestateSplit;
				else                                  this->m_eValue = eDefValue;

				return this->m_eValue;
			}

			virtual std::wstring          ToString  () const
			{
				switch(this->m_eValue)
				{
				case panestateFrozen : return L"frozen";
				case panestateFrozenSplit  : return L"frozenSplit";
				case panestateSplit  : return L"split";
				default : return L"frozen";
				}
			}

            SimpleType_FromString     (EPaneState)
            SimpleType_Operator_Equal (CPaneState)
		};

		enum EDdeValueType
		{
			ddevaluetypeNil = 0,
			ddevaluetypeB = 1,
			ddevaluetypeN = 2,
			ddevaluetypeE = 3,
			ddevaluetypeStr = 4
		};

		template<EDdeValueType eDefValue = ddevaluetypeNil>
		class CDdeValueType : public CSimpleType<EDdeValueType, eDefValue>
		{
		public:
			CDdeValueType() {}
			CDdeValueType(const EDdeValueType & val)	{ this->m_eValue = val; }

			virtual EDdeValueType FromString(std::wstring &sValue)
			{
				if      ( L"nil" == sValue ) this->m_eValue = ddevaluetypeNil;
				else if ( L"b" ==  sValue ) this->m_eValue = ddevaluetypeB;
				else if ( L"n" ==  sValue ) this->m_eValue = ddevaluetypeN;
				else if ( L"e" ==  sValue ) this->m_eValue = ddevaluetypeE;
				else if ( L"str" ==  sValue ) this->m_eValue = ddevaluetypeStr;
				else this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring          ToString  () const
			{
				switch(this->m_eValue)
				{
				case ddevaluetypeNil : return L"nil";
				case ddevaluetypeB  : return L"b";
				case ddevaluetypeN  : return L"n";
				case ddevaluetypeE  : return L"e";
				case ddevaluetypeStr  : return L"str";
				default : return L"nil";
				}
			}

			SimpleType_FromString     (EDdeValueType)
			SimpleType_Operator_Equal (CDdeValueType)
		};

		enum ECalcMode
		{
			calcmodeAuto			=  0,
			calcmodeAutoNoTable	=  1,
			calcmodeManual		=  2,
		};

		template<ECalcMode eDefValue = calcmodeAuto>
		class CCalcMode : public CSimpleType<ECalcMode, eDefValue>
		{
		public:
			CCalcMode() {}
			CCalcMode(const ECalcMode & val)	{ this->m_eValue = val; }

			virtual ECalcMode FromString(std::wstring &sValue)
			{
				if      ( L"auto"      == sValue ) this->m_eValue = calcmodeAuto;
				else if ( L"autoNoTable" ==  sValue ) this->m_eValue = calcmodeAutoNoTable;
				else if ( L"manual"     == sValue ) this->m_eValue = calcmodeManual;
				else this->m_eValue = eDefValue;

				return this->m_eValue;
			}

			virtual std::wstring  ToString  () const
			{
				switch(this->m_eValue)
				{
				case calcmodeAuto : return L"auto";break;
				case calcmodeAutoNoTable : return L"autoNoTable";break;
				case calcmodeManual : return L"manual";break;
				default : return L"auto";

				}
			}

			SimpleType_FromString     (ECalcMode)
			SimpleType_Operator_Equal (CCalcMode)
		};

		enum ERefMode
		{
			refmodeA1	=  0,
			refmodeR1C1	=  1
		};

		template<ERefMode eDefValue = refmodeA1>
		class CRefMode : public CSimpleType<ERefMode, eDefValue>
		{
		public:
			CRefMode() {}
			CRefMode(const ERefMode & val)	{ this->m_eValue = val; }

			virtual ERefMode FromString(std::wstring &sValue)
			{
				if      ( L"A1"  == sValue ) this->m_eValue = refmodeA1;
				else if ( L"R1C1" ==  sValue ) this->m_eValue = refmodeR1C1;
				else this->m_eValue = eDefValue;

				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
				case refmodeA1 : return L"A1";break;
				case refmodeR1C1 : return L"R1C1";break;
				default : return L"A1";

				}
			}

			SimpleType_FromString     (ERefMode)
			SimpleType_Operator_Equal (CRefMode)
		};

		enum EDoubleOrAutomatic 
		{
			typeAuto		=  0,
			typeDouble		=  1
		};
		template<EDoubleOrAutomatic eDefValue = typeAuto>
		class CDoubleOrAutomatic : public CSimpleType<EDoubleOrAutomatic, eDefValue>
		{
		public:
			double m_dValue;
			CDoubleOrAutomatic() : m_dValue(0){}
			
            CDoubleOrAutomatic(const double & val) { m_dValue = val; this->m_eValue = typeDouble; }
            CDoubleOrAutomatic(const EDoubleOrAutomatic & val) { this->m_eValue = val; m_dValue = 0; }

            virtual EDoubleOrAutomatic FromString(std::wstring &sValue)
			{
				if(L"auto" == sValue)
				{
                    this->m_eValue = typeAuto;
					m_dValue = 0;
				}
				else
				{
                    this->m_eValue = typeDouble;
					m_dValue = XmlUtils::GetDouble(sValue);
				}
                return this->m_eValue;
			}
			double GetDoubleValue() const
			{
				return m_dValue;
			}
			virtual std::wstring ToString  () const 
			{
                switch(this->m_eValue)
				{
					case typeAuto : return L"auto";break;
					case typeDouble :
					default: 
					{
						return XmlUtils::DoubleToString(m_dValue);
					}break;
				}
			}

			SimpleType_FromString     (EDoubleOrAutomatic)
			SimpleType_Operator_Equal (CDoubleOrAutomatic)
		};
		
		enum ESeriesLayout 
		{
			typeSeriesLayoutBoxWhisker		=  0,
			typeSeriesLayoutClusteredColumn	=  1,
			typeSeriesLayoutFunnel			=  2,
			typeSeriesLayoutParetoLine		=  3,
			typeSeriesLayoutRegionMap		=  4,
			typeSeriesLayoutSunburst		=  5,
			typeSeriesLayoutTreemap			=  6,
			typeSeriesLayoutWaterfall		=  7,
		};
		template<ESeriesLayout eDefValue = typeSeriesLayoutBoxWhisker>
		class CSeriesLayout : public CSimpleType<ESeriesLayout, eDefValue>
		{
		public:
			CSeriesLayout() {}
			CSeriesLayout(const ESeriesLayout & val)	{ this->m_eValue = val; }

			virtual ESeriesLayout FromString(std::wstring &sValue)
			{
				if      ( L"boxWhisker" ==  sValue ) this->m_eValue = typeSeriesLayoutBoxWhisker;
				else if ( L"clusteredColumn" == sValue ) this->m_eValue = typeSeriesLayoutClusteredColumn;
				else if ( L"funnel" ==  sValue ) this->m_eValue = typeSeriesLayoutFunnel;
				else if ( L"paretoLine" ==  sValue ) this->m_eValue = typeSeriesLayoutParetoLine;
				else if ( L"regionMap" ==  sValue ) this->m_eValue = typeSeriesLayoutRegionMap;
				else if ( L"sunburst" ==  sValue ) this->m_eValue = typeSeriesLayoutSunburst;
				else if ( L"treemap" ==  sValue ) this->m_eValue = typeSeriesLayoutTreemap;
				else if ( L"waterfall" ==  sValue ) this->m_eValue = typeSeriesLayoutWaterfall;
				else this->m_eValue = eDefValue;

				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case typeSeriesLayoutBoxWhisker :		return L"boxWhisker";break;
					case typeSeriesLayoutClusteredColumn :	return L"clusteredColumn";break;
					case typeSeriesLayoutFunnel:			return L"funnel";	break;
					case typeSeriesLayoutParetoLine :		return L"paretoLine";break;
					case typeSeriesLayoutRegionMap :		return L"regionMap";	break;
					case typeSeriesLayoutSunburst :			return L"sunburst";	break;
					case typeSeriesLayoutTreemap :			return L"treemap";	break;
					case typeSeriesLayoutWaterfall :		return L"waterfall";	break;
					default : return L"auto";
				}
			}

			SimpleType_FromString     (ESeriesLayout)
			SimpleType_Operator_Equal (CSeriesLayout)
		};
		
		enum EAxisUnit 
		{
			unitHundreds			=  0,
			unitThousands			=  1,
			unitTenThousands		=  2,
			unitHundredThousands	=  3,
			unitMillions			=  4,
			unitTenMillions			=  5,
			unitHundredMillions		=  6,
			unitBillions			=  7,
			unitTrillions			=  8,
			unitPercentage			=  9

		};
		template<EAxisUnit eDefValue = unitPercentage>
		class CAxisUnit : public CSimpleType<EAxisUnit, eDefValue>
		{
		public:
			CAxisUnit() {}
			CAxisUnit(const EAxisUnit & val)	{ this->m_eValue = val; }

			virtual EAxisUnit FromString(std::wstring &sValue)
			{
				if      ( L"hundreds" ==  sValue )	this->m_eValue = unitHundreds;
				else if ( L"thousands" ==  sValue ) this->m_eValue = unitThousands;
				else if ( L"tenThousands" == sValue ) this->m_eValue = unitTenThousands;
				else if ( L"hundredThousands" ==  sValue ) this->m_eValue = unitHundredThousands;
				else if ( L"millions" ==  sValue ) this->m_eValue = unitMillions;
				else if ( L"tenMillions" ==  sValue ) this->m_eValue = unitTenMillions;
				else if ( L"hundredMillions" == sValue ) this->m_eValue = unitHundredMillions;
				else if ( L"billions" ==  sValue ) this->m_eValue = unitBillions;
				else if ( L"trillions" ==  sValue ) this->m_eValue = unitTrillions;
				else if ( L"percentage" ==  sValue ) this->m_eValue = unitPercentage;
				else this->m_eValue = eDefValue;

				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case unitHundreds :			return L"hundreds";		break;
					case unitThousands :		return L"thousands";		break;
					case unitTenThousands:		return L"tenThousands";	break;
					case unitHundredThousands :	return L"hundredThousands";break;
					case unitMillions :			return L"millions";		break;
					case unitTenMillions :		return L"tenMillions";	break;
					case unitHundredMillions :	return L"hundredMillions";break;
					case unitBillions :			return L"billions";		break;
					case unitTrillions :		return L"trillions";		break;
					case unitPercentage :		return L"percentage";	break;
					default : return L"";
				}
			}

			SimpleType_FromString     (EAxisUnit)
			SimpleType_Operator_Equal (CAxisUnit)
		};
		enum EDimensionType 
		{
			typeDimensionVal		=  0,
			typeDimensionX			=  1,
			typeDimensionY			=  2,
			typeDimensionSize		=  3,
			typeDimensionColorVal	=  4,
			typeDimensionCat		=  10,
			typeDimensionColorStr	=  11
		};
		template<EDimensionType eDefValue = typeDimensionVal>
		class CDimensionType : public CSimpleType<EDimensionType, eDefValue>
		{
		public:
			CDimensionType() {}
			CDimensionType(const EDimensionType & val)	{ this->m_eValue = val; }

			virtual EDimensionType FromString(std::wstring &sValue)
			{
				if      ( L"val" ==  sValue )	this->m_eValue = typeDimensionVal;
				else if ( L"x" ==  sValue ) this->m_eValue = typeDimensionX;
				else if ( L"y" ==  sValue ) this->m_eValue = typeDimensionY;
				else if ( L"size" == sValue ) this->m_eValue = typeDimensionSize;
				else if ( L"colorVal" ==  sValue ) this->m_eValue = typeDimensionColorVal;
				else if ( L"cat" ==  sValue ) this->m_eValue = typeDimensionCat;
				else if ( L"colorStr" ==  sValue ) this->m_eValue = typeDimensionColorStr;
				else this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case typeDimensionVal :		return L"val";	
					case typeDimensionX :		return L"x";		
					case typeDimensionY:		return L"y";		
					case typeDimensionSize :	return L"size";	
					case typeDimensionColorVal :return L"colorVal";
					case typeDimensionCat :		return L"cat";
					case typeDimensionColorStr :return L"colorStr";
					default : return L"";
				}
			}

			SimpleType_FromString     (EDimensionType)
			SimpleType_Operator_Equal (CDimensionType)
		};
		enum EFormulaDirection 
		{
			directionCol		=  0,
			directionRow		=  1
		};
		template<EFormulaDirection eDefValue = directionCol>
		class CFormulaDirection : public CSimpleType<EFormulaDirection, eDefValue>
		{
		public:
			CFormulaDirection() {}
			CFormulaDirection(const EFormulaDirection & val)	{ this->m_eValue = val; }

            virtual EFormulaDirection FromString(std::wstring &sValue)
			{
                if(L"row" == sValue) this->m_eValue = directionRow;
                else                    this->m_eValue = directionCol;
                return this->m_eValue;
			}
			virtual std::wstring ToString  () const 
			{
                switch(this->m_eValue)
				{
					case directionRow : return L"row";
					case directionCol :
					default: 
					{
						return L"col";
					}break;
				}
			}

			SimpleType_FromString     (EFormulaDirection)
			SimpleType_Operator_Equal (CFormulaDirection)
		};
		enum ESidePos 
		{
			sideLeft		=  0,
			sideRight		=  1,
			sideTop			=  2,
			sideBottom		=  3
		};
		template<ESidePos eDefValue = sideTop>
		class CSidePos : public CSimpleType<ESidePos, eDefValue>
		{
		public:
			CSidePos() {}
			CSidePos(const ESidePos & val)	{ this->m_eValue = val; }

			virtual ESidePos FromString(std::wstring &sValue)
			{
				if      ( L"t" == sValue )	this->m_eValue = sideTop;
				else if ( L"l" == sValue ) this->m_eValue = sideLeft;
				else if ( L"r" == sValue ) this->m_eValue = sideRight;
				else if ( L"b" == sValue ) this->m_eValue = sideBottom;
				else							this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case sideLeft :		return L"l";	
					case sideRight:		return L"r";	
					case sideBottom :	return L"b";	
					case sideTop :		
					default : return L"t";
				}
			}

			SimpleType_FromString     (ESidePos)
			SimpleType_Operator_Equal (CSidePos)
		};
		enum EPosAlign 
		{
			posAlignMin		=  0,
			posAlignCtr		=  1,
			posAlignMax		=  2
		};
		template<EPosAlign eDefValue = posAlignCtr>
		class CPosAlign : public CSimpleType<EPosAlign, eDefValue>
		{
		public:
			CPosAlign() {}
			CPosAlign(const EPosAlign & val)	{ this->m_eValue = val; }

			virtual EPosAlign FromString(std::wstring &sValue)
			{
				if      ( L"min" == sValue )	this->m_eValue = posAlignMin;
				else if ( L"max" == sValue ) this->m_eValue = posAlignMax;
				else if ( L"ctr" == sValue ) this->m_eValue = posAlignCtr;
				else							this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case posAlignMax :	return L"max";
					case posAlignMin:	return L"min";
					case posAlignCtr :	
					default :			return L"ctr";
				}
			}

			SimpleType_FromString     (EPosAlign)
			SimpleType_Operator_Equal (CPosAlign)
		};
		enum ERegionLabelLayout 
		{
			layoutNone			=  0,
			layoutBestFitOnly	=  1,
			layoutShowAll		=  2
		};
		template<ERegionLabelLayout eDefValue = layoutNone>
		class CRegionLabelLayout : public CSimpleType<ERegionLabelLayout, eDefValue>
		{
		public:
			CRegionLabelLayout() {}
			CRegionLabelLayout(const ERegionLabelLayout & val)	{ this->m_eValue = val; }

			virtual ERegionLabelLayout FromString(std::wstring &sValue)
			{
				if      ( L"none" ==  sValue )	this->m_eValue = layoutNone;
				else if ( L"bestFitOnly" == sValue ) this->m_eValue = layoutBestFitOnly;
				else if ( L"showAll" ==  sValue ) this->m_eValue = layoutShowAll;
				else									this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case layoutNone :		return L"none";
					case layoutBestFitOnly:	return L"bestFitOnly";
					case layoutShowAll :	
					default :				return L"showAll";
				}
			}

			SimpleType_FromString     (ERegionLabelLayout)
			SimpleType_Operator_Equal (CRegionLabelLayout)
		};
		enum EParentLabelLayout 
		{
			layoutNone_			=  0,
			layoutBanner		=  1,
			layoutOverlapping	=  2
		};
		template<EParentLabelLayout eDefValue = layoutNone_>
		class CParentLabelLayout : public CSimpleType<EParentLabelLayout, eDefValue>
		{
		public:
			CParentLabelLayout() {}
			CParentLabelLayout(const EParentLabelLayout & val)	{ this->m_eValue = val; }

			virtual EParentLabelLayout FromString(std::wstring &sValue)
			{
				if      ( L"none" ==  sValue )	this->m_eValue = layoutNone_;
				else if ( L"banner" ==  sValue ) this->m_eValue = layoutBanner;
				else if ( L"overlapping" == sValue ) this->m_eValue = layoutOverlapping;
				else									this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case layoutNone_ :		return L"none";
					case layoutOverlapping:	return L"banner";
					case layoutBanner :	
					default :				return L"overlapping";
				}
			}

			SimpleType_FromString     (EParentLabelLayout)
			SimpleType_Operator_Equal (CParentLabelLayout)
		};
		
		enum EQuartileMethod 
		{
			methodInclusive		=  0,
			methodExclusive		=  1,
		};
		template<EQuartileMethod eDefValue = methodInclusive>
		class CQuartileMethod : public CSimpleType<EQuartileMethod, eDefValue>
		{
		public:
			CQuartileMethod() {}
			CQuartileMethod(const EQuartileMethod & val)	{ this->m_eValue = val; }

			virtual EQuartileMethod FromString(std::wstring &sValue)
			{
				if      ( L"inclusive" == sValue )	this->m_eValue = methodInclusive;
				else if ( L"exclusive" == sValue ) this->m_eValue = methodExclusive;
				else									this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case methodInclusive :	return L"inclusive";
					case methodExclusive:	
					default :				return L"exclusive";
				}
			}

			SimpleType_FromString     (EQuartileMethod)
			SimpleType_Operator_Equal (CQuartileMethod)
		};

		enum EDataLabelPos 
		{
			posBestFit	=  0,
			posBottom	=  1,
			posCtr		=  2,
			posInBase	=  3,
			posInEnd	=  4,
			posLeft		=  5,
			posOutEnd	=  6,
			posRight	=  7,
			posTop		=  8

		};
		template<EDataLabelPos eDefValue = posBestFit>
		class CDataLabelPos : public CSimpleType<EDataLabelPos, eDefValue>
		{
		public:
			CDataLabelPos() {}
			CDataLabelPos(const EDataLabelPos & val)	{ this->m_eValue = val; }

			virtual EDataLabelPos FromString(std::wstring &sValue)
			{
				if      ( L"bestFit" == sValue )	this->m_eValue = posBestFit;
				else if ( L"b" ==  sValue ) this->m_eValue = posBottom;
				else if ( L"ctr" ==  sValue ) this->m_eValue = posCtr;
				else if ( L"inBase" == sValue ) this->m_eValue = posInBase;
				else if ( L"inEnd" == sValue ) this->m_eValue = posInEnd;
				else if ( L"l" ==  sValue ) this->m_eValue = posLeft;
				else if ( L"outEnd" == sValue ) this->m_eValue = posOutEnd;
				else if ( L"r" ==  sValue ) this->m_eValue = posRight;
				else if ( L"t" ==  sValue ) this->m_eValue = posTop;
				else								this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case posBottom :	return L"b";
					case posCtr :		return L"ctr";
					case posInBase :	return L"inBase";
					case posInEnd :		return L"inEnd";
					case posLeft :		return L"l";
					case posOutEnd :	return L"outEnd";
					case posRight :		return L"r";
					case posTop :		return L"t";
					case posBestFit:	
					default :			return L"bestFit";
				}
			}

			SimpleType_FromString     (EDataLabelPos)
			SimpleType_Operator_Equal (CDataLabelPos)
		};

		template<ESidePos eDefValue = sideLeft>
		class CIntervalClosedSide : public CSimpleType<ESidePos, eDefValue>
		{
		public:
			CIntervalClosedSide() {}
			CIntervalClosedSide(const ESidePos & val)	{ this->m_eValue = val; }

			virtual ESidePos FromString(std::wstring &sValue)
			{
				if      ( L"l" == sValue )	this->m_eValue = sideLeft;
				else if ( L"r" == sValue ) this->m_eValue = sideRight;
				else							this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case sideRight:	return L"r";
					case sideLeft :	
					default :		return L"l";		
				}
			}

			SimpleType_FromString     (ESidePos)
			SimpleType_Operator_Equal (CIntervalClosedSide)
		};
		
		enum ETickMarksType 
		{
			marksTypeIn		=  0,
			marksTypeOut	=  1,
			marksTypeCross	=  2,
			marksTypeNone	=  3

		};
		template<ETickMarksType eDefValue = marksTypeNone>
		class CTickMarksType : public CSimpleType<ETickMarksType, eDefValue>
		{
		public:
			CTickMarksType() {}
			CTickMarksType(const ETickMarksType & val)	{ this->m_eValue = val; }

			virtual ETickMarksType FromString(std::wstring &sValue)
			{
				if      ( L"none" == sValue )	this->m_eValue = marksTypeNone;
				else if ( L"out" ==  sValue ) this->m_eValue = marksTypeOut;
				else if ( L"in" ==  sValue ) this->m_eValue = marksTypeIn;
				else if ( L"cross" == sValue ) this->m_eValue = marksTypeCross;
				else								this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case marksTypeIn	:	return L"in";
					case marksTypeOut	:	return L"out";
					case marksTypeCross :	return L"cross";
					case marksTypeNone:	
					default :			return L"none";
				}
			}

			SimpleType_FromString     (ETickMarksType)
			SimpleType_Operator_Equal (CTickMarksType)
		};
		enum ESourceCacheType
		{
			typeSourceConsolidation	=  0,
			typeSourceExternal		=  1,
			typeSourceScenario		=  2,
			typeSourceWorksheet		=  3,
		};
		template<ESourceCacheType eDefValue = typeSourceWorksheet>
		class CSourceCacheType : public CSimpleType<ESourceCacheType, eDefValue>
		{
		public:
			CSourceCacheType() {}
			CSourceCacheType(const ESourceCacheType & val)	{ this->m_eValue = val; }

			virtual ESourceCacheType FromString(std::wstring &sValue)
			{
				if      ( L"consolidation" == sValue )	this->m_eValue = typeSourceConsolidation;
				else if ( L"external" ==  sValue ) this->m_eValue = typeSourceExternal;
				else if ( L"scenario" ==  sValue ) this->m_eValue = typeSourceScenario;
				else if ( L"worksheet" ==  sValue ) this->m_eValue = typeSourceWorksheet;

				return this->m_eValue;
			}
			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case typeSourceConsolidation:	return L"consolidation";
					case typeSourceExternal:		return L"external";
					case typeSourceScenario:		return L"scenario";
					case typeSourceWorksheet:		return L"worksheet";
					default :						return L"worksheet";
				}
			}
			SimpleType_FromString     (ESourceCacheType)
			SimpleType_Operator_Equal (CSourceCacheType)
		};		
		enum EValuesGroupBy
		{
			groupByDays				=  0,
			groupByHours			=  1,
			groupByMinutes			=  2,
			groupByMonths			=  3,
			groupByQuarters			=  4,
			groupByNumericRanges	=  5,
			groupBySeconds			=  6,
			groupByYears			=  7
		};
		template<EValuesGroupBy eDefValue = groupByMonths>
		class CValuesGroupBy : public CSimpleType<EValuesGroupBy, eDefValue>
		{
		public:
			CValuesGroupBy() {}
			CValuesGroupBy(const EValuesGroupBy & val)	{ this->m_eValue = val; }

			virtual EValuesGroupBy FromString(std::wstring &sValue)
			{
				if      ( L"days" == sValue )	this->m_eValue = groupByDays;
				else if ( L"hours" == sValue ) this->m_eValue = groupByHours;
				else if ( L"minutes" == sValue ) this->m_eValue = groupByMinutes;
				else if ( L"months" == sValue ) this->m_eValue = groupByMonths;
				else if ( L"quarters" ==  sValue ) this->m_eValue = groupByQuarters;
				else if ( L"range" == sValue ) this->m_eValue = groupByNumericRanges;
				else if ( L"seconds" == sValue ) this->m_eValue = groupBySeconds;
				else if ( L"years" == sValue ) this->m_eValue = groupByYears;

				return this->m_eValue;
			}
			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case groupByDays:			return L"days";
					case groupByHours:			return L"hours";
					case groupByMinutes:		return L"minutes";
					case groupByMonths:			return L"months";
					case groupByQuarters:		return L"quarters";
					case groupByNumericRanges:	return L"range";
					case groupBySeconds:		return L"seconds";
					case groupByYears:			return L"years";
					default :					return L"";
				}
			}
			SimpleType_FromString     (EValuesGroupBy)
			SimpleType_Operator_Equal (CValuesGroupBy)
		};
		enum EPivotItemType
		{
			typeAverage	=  0,
			typeBlank	=  1,
			typeCount	=  2,
			typeCountA	=  3,
			typeData	=  4,
			typeDefault	=  5,
			typeGrandTotalt	=  6,
			typeMin		=  7,
			typeMax		=  8,
			typeProduct	=  9,
			typeStdDev	=  10,
			typeStdDevP	=  11,
			typeSum		=  12,
			typeVar		=  13,
			typeVarP	=  14
		};
		template<EPivotItemType eDefValue = typeSum>
		class CPivotItemType : public CSimpleType<EPivotItemType, eDefValue>
		{
		public:
			CPivotItemType() {}
			CPivotItemType(const EPivotItemType & val)	{ this->m_eValue = val; }

			virtual EPivotItemType FromString(std::wstring &sValue)
			{
				if      ( L"avg" ==  sValue )	this->m_eValue = typeAverage;
				else if ( L"blank" == sValue ) this->m_eValue = typeBlank;
				else if ( L"count" == sValue ) this->m_eValue = typeCount;
				else if ( L"countA" == sValue ) this->m_eValue = typeCountA;
				else if ( L"data" == sValue ) this->m_eValue = typeData;
				else if ( L"default" == sValue ) this->m_eValue = typeDefault;
				else if ( L"grand" == sValue ) this->m_eValue = typeGrandTotalt;
				else if ( L"min" ==  sValue ) this->m_eValue = typeMin;
				else if ( L"max" ==  sValue ) this->m_eValue = typeMax;
				else if ( L"product" == sValue ) this->m_eValue = typeProduct;
				else if ( L"stdDev" == sValue ) this->m_eValue = typeStdDev;
				else if ( L"stdDevP" == sValue ) this->m_eValue = typeStdDevP;
				else if ( L"sum" ==  sValue ) this->m_eValue = typeSum;
				else if ( L"var" ==  sValue ) this->m_eValue = typeVar;
				else if ( L"varP" == sValue ) this->m_eValue = typeVarP;

				return this->m_eValue;
			}
			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case typeAverage:	return L"avg";
					case typeBlank:		return L"blank";
					case typeCount:		return L"count";
					case typeCountA:	return L"countA";
					case typeData:		return L"data";
					case typeDefault:	return L"default";
					case typeGrandTotalt:return L"grand";
					case typeMin:		return L"min";
					case typeMax:		return L"max";
					case typeProduct:	return L"product";
					case typeStdDev:	return L"stdDev";
					case typeStdDevP:	return L"stdDevP";
					case typeSum:		return L"sum";
					case typeVar:		return L"var";
					case typeVarP:		return L"varP";
					default :			return L"";
				}
			}
			SimpleType_FromString     (EPivotItemType)
			SimpleType_Operator_Equal (CPivotItemType)
		};
		enum EFieldSortType
		{
			sortManual		=  0,
			sortAscending	=  1,
			sortDescending	=  2
		};
		template<EFieldSortType eDefValue = sortManual>
		class CFieldSortType : public CSimpleType<EFieldSortType, eDefValue>
		{
		public:
			CFieldSortType() {}
			CFieldSortType(const EFieldSortType & val)	{ this->m_eValue = val; }

			virtual EFieldSortType FromString(std::wstring &sValue)
			{
				if      ( L"manual" ==  sValue )	this->m_eValue = sortManual;
				else if ( L"ascending" == sValue ) this->m_eValue = sortAscending;
				else if ( L"descending" == sValue ) this->m_eValue = sortDescending;

				return this->m_eValue;
			}
			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case sortManual:		return L"manual";
					case sortAscending:		return L"ascending";
					case sortDescending:	return L"descending";
					default :				return L"";
				}
			}
			SimpleType_FromString     (EFieldSortType)
			SimpleType_Operator_Equal (CFieldSortType)
		};
		
		enum EPivotAreaType
		{
			areaAll			=  0,
			areaFieldButton =  1,
			areaData		=  2,	
			areaNone		=  3,
			areaNormal		=  4,
			areaOrigin		=  5,
			areaTopEnd		=  6
		};
		template<EPivotAreaType eDefValue = areaNone>
		class CPivotAreaType : public CSimpleType<EPivotAreaType, eDefValue>
		{
		public:
			CPivotAreaType() {}
			CPivotAreaType(const EPivotAreaType & val)	{ this->m_eValue = val; }

			virtual EPivotAreaType FromString(std::wstring &sValue)
			{
				if      ( L"all" ==  sValue )	this->m_eValue = areaAll;
				else if ( L"button" == sValue ) this->m_eValue = areaFieldButton;
				else if ( L"data" == sValue ) this->m_eValue = areaData;
				else if ( L"none" == sValue ) this->m_eValue = areaNone;
				else if ( L"normal" == sValue ) this->m_eValue = areaNormal;
				else if ( L"origin" == sValue ) this->m_eValue = areaOrigin;
				else if ( L"topEnd" == sValue ) this->m_eValue = areaTopEnd;

				return this->m_eValue;
			}
			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case areaAll:			return L"all";
					case areaFieldButton:	return L"button";
					case areaData:			return L"data";
					case areaNone:			return L"none";
					case areaNormal:		return L"normal";
					case areaOrigin:		return L"origin";
					case areaTopEnd:		return L"topEnd";
					default :				return L"";
				}
			}
			SimpleType_FromString     (EPivotAreaType)
			SimpleType_Operator_Equal (CPivotAreaType)
		};
		enum EPivotAxisType
		{
			axisCol		=  0,
			axisPage	=  1,
			axisRow		=  2,
			axisValues	=  3
		};
		template<EPivotAxisType eDefValue = axisCol>
		class CPivotAxisType : public CSimpleType<EPivotAxisType, eDefValue>
		{
		public:
			CPivotAxisType() {}
			CPivotAxisType(const EPivotAxisType & val)	{ this->m_eValue = val; }

			virtual EPivotAxisType FromString(std::wstring &sValue)
			{
				if      ( L"axisCol" ==  sValue )	this->m_eValue = axisCol;
				else if ( L"axisPage" == sValue ) this->m_eValue = axisPage;
				else if ( L"axisRow" ==  sValue ) this->m_eValue = axisRow;
				else if ( L"axisValues" == sValue ) this->m_eValue = axisValues;

				return this->m_eValue;
			}
			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case axisCol:		return L"axisCol";
					case axisPage:		return L"axisPage";
                                        case axisRow:		return L"axisRow";
                                        case axisValues:	return L"countNums";
					default :			return L"";
				}
			}
			SimpleType_FromString     (EPivotAxisType)
			SimpleType_Operator_Equal (CPivotAxisType)
		};
		enum EFormatAction
		{
			actionBlank		=  0,
			actionDrill		=  1,
			actionFormatting=  2,
			actionFormula	=  3
		};
		template<EFormatAction eDefValue = actionBlank>
		class CFormatAction : public CSimpleType<EFormatAction, eDefValue>
		{
		public:
			CFormatAction() {}
			CFormatAction(const EFormatAction & val)	{ this->m_eValue = val; }

			virtual EFormatAction FromString(std::wstring &sValue)
			{
				if      ( L"blank" ==  sValue )	this->m_eValue = actionBlank;
				else if ( L"drill" ==  sValue ) this->m_eValue = actionDrill;
				else if ( L"formatting" == sValue ) this->m_eValue = actionFormatting;
				else if ( L"formula" ==  sValue ) this->m_eValue = actionFormula;

				return this->m_eValue;
			}
			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case actionBlank:		return L"blank";
					case actionDrill:		return L"drill";
					case actionFormatting:	return L"formatting";
					case actionFormula:		return L"formula";
					default :				return L"";
				}
			}
			SimpleType_FromString     (EFormatAction)
			SimpleType_Operator_Equal (CFormatAction)
		};
		enum EShowDataAs
		{
			dataAsDifference	=  0,
			dataAsIndex			=  1,
			dataAsNormal		=  2,
			dataAsPercentOff	=  3,
			dataAsPercentDiff	=  4,
			dataAsPercentOfCol	=  5,
			dataAsPercentOfRow	=  6,
			dataAsPercentOfTotal =  7,
			dataAsRunTotal		=  8
		};
		template<EShowDataAs eDefValue = dataAsRunTotal>
		class CShowDataAs : public CSimpleType<EShowDataAs, eDefValue>
		{
		public:
			CShowDataAs() {}
			CShowDataAs(const EShowDataAs & val)	{ this->m_eValue = val; }

			virtual EShowDataAs FromString(std::wstring &sValue)
			{
				if      ( L"difference" ==  sValue )	this->m_eValue = dataAsDifference;
				else if ( L"index" ==  sValue ) this->m_eValue = dataAsIndex;
				else if ( L"normal" ==  sValue ) this->m_eValue = dataAsNormal;
				else if ( L"percent" ==  sValue ) this->m_eValue = dataAsPercentOff;
				else if ( L"percentDiff" ==  sValue ) this->m_eValue = dataAsPercentDiff;
				else if ( L"percentOfCol" == sValue ) this->m_eValue = dataAsPercentOfCol;
				else if ( L"percentOfRow" == sValue ) this->m_eValue = dataAsPercentOfRow;
				else if ( L"percentOfTotal" == sValue ) this->m_eValue = dataAsPercentOfTotal;
				else if ( L"runTotal" ==  sValue ) this->m_eValue = dataAsRunTotal;
				else										this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case dataAsDifference:		return L"difference";
					case dataAsIndex:			return L"index";
					case dataAsNormal:			return L"normal";
					case dataAsPercentOff:		return L"percent";
					case dataAsPercentDiff:		return L"percentDiff";
					case dataAsPercentOfCol:	return L"percentOfCol";
					case dataAsPercentOfRow:	return L"percentOfRow";
					case dataAsPercentOfTotal:	return L"percentOfTotal";
					case dataAsRunTotal:		return L"percentOfTotal";
					default :					return L"runTotal";
				}
			}

			SimpleType_FromString     (EShowDataAs)
			SimpleType_Operator_Equal (CShowDataAs)
		};
		enum EDataConsolidateFunction 
		{
			functionAverage		=  0,
			functionCount		=  1,
			functionCountNums	=  2,
			functionMaximum		=  3,
			functionMinimum		=  4,
			functionProduct		=  5,
			functionStdDev		=  6,
			functionStdDevP		=  7,
                        functionSum		=  8,
			functionVariance	=  9,
			functionVarP		=  10
		};
		template<EDataConsolidateFunction eDefValue = functionSum>
		class CDataConsolidateFunction : public CSimpleType<EDataConsolidateFunction, eDefValue>
		{
		public:
			CDataConsolidateFunction() {}
			CDataConsolidateFunction(const EDataConsolidateFunction & val)	{ this->m_eValue = val; }

			virtual EDataConsolidateFunction FromString(std::wstring &sValue)
			{
				if      ( L"average" ==  sValue )	this->m_eValue = functionAverage;
				else if ( L"count" ==  sValue ) this->m_eValue = functionCount;
				else if ( L"countNums" == sValue ) this->m_eValue = functionCountNums;
				else if ( L"max" ==  sValue ) this->m_eValue = functionMaximum;
				else if ( L"min" ==  sValue ) this->m_eValue = functionMinimum;
				else if ( L"product" ==  sValue ) this->m_eValue = functionProduct;
				else if ( L"stdDev" ==  sValue ) this->m_eValue = functionStdDev;
				else if ( L"stdDevp" ==  sValue ) this->m_eValue = functionStdDevP;
				else if ( L"sum" ==  sValue ) this->m_eValue = functionSum;
				else if ( L"var" ==  sValue ) this->m_eValue = functionVariance;
				else if ( L"varp" ==  sValue ) this->m_eValue = functionVarP;
				else									this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case functionAverage:	return L"average";
					case functionCount:		return L"count";
					case functionCountNums:	return L"countNums";
					case functionMaximum:	return L"max";
					case functionMinimum:	return L"min";
					case functionProduct:	return L"product";
					case functionStdDev:	return L"stdDev";
					case functionStdDevP:	return L"stdDevp";
					case functionSum:		return L"sum";
					case functionVariance:	return L"var";
					case functionVarP:		return L"varp";
					default :				return L"sum";
				}
			}

			SimpleType_FromString     (EDataConsolidateFunction)
			SimpleType_Operator_Equal (CDataConsolidateFunction)
		};
		enum EDataValidationType 
		{
			validationTypeNone		=  0,
			validationTypeCustom	=  1,
			validationTypeDate		=  2,
			validationTypeDecimal	=  3,
			validationTypeList		=  4,
			validationTypeTextLength=  5,
			validationTypeTime		=  6,
			validationTypeWhole		=  7
		};
		template<EDataValidationType eDefValue = validationTypeNone>
		class CDataValidationType : public CSimpleType<EDataValidationType, eDefValue>
		{
		public:
			CDataValidationType() {}
			CDataValidationType(const EDataValidationType & val)	{ this->m_eValue = val; }

			virtual EDataValidationType FromString(std::wstring &sValue)
			{
				if      ( L"custom"		== sValue )	this->m_eValue = validationTypeCustom;
				else if ( L"date"		== sValue ) this->m_eValue = validationTypeDate;
				else if ( L"decimal"	== sValue ) this->m_eValue = validationTypeDecimal;
				else if (	L"list"		== sValue ||
							L"List"	== sValue)		this->m_eValue = validationTypeList;
				else if ( L"none"		== sValue ) this->m_eValue = validationTypeNone;
				else if ( L"textLength"	== sValue ) this->m_eValue = validationTypeTextLength;
				else if ( L"time"		== sValue ) this->m_eValue = validationTypeTime;
				else if ( L"whole"		== sValue ) this->m_eValue = validationTypeWhole;
				else									this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case validationTypeCustom	:	return L"custom";
					case validationTypeDate	:		return L"date";
					case validationTypeDecimal :	return L"decimal";
					case validationTypeList :		return L"list";
					case validationTypeNone :		return L"none";
					case validationTypeTextLength :	return L"textLength";
					case validationTypeTime:		return L"time";
					case validationTypeWhole:		return L"whole";
					default :						return L"none";
				}
			}

			SimpleType_FromString     (EDataValidationType)
			SimpleType_Operator_Equal (CDataValidationType)
		};
		enum EDataValidationErrorStyle 
		{
			errorStyleStop			=  0,
			errorStyleWarning		=  1,
			errorStyleInformation	=  2
		};
		template<EDataValidationErrorStyle eDefValue = errorStyleStop>
		class CDataValidationErrorStyle : public CSimpleType<EDataValidationErrorStyle, eDefValue>
		{
		public:
			CDataValidationErrorStyle() {}
			CDataValidationErrorStyle(const EDataValidationErrorStyle & val)	{ this->m_eValue = val; }

			virtual EDataValidationErrorStyle FromString(std::wstring &sValue)
			{
				if      ( L"stop" ==  sValue )	this->m_eValue = errorStyleStop;
				else if ( L"warning" ==  sValue ) this->m_eValue = errorStyleWarning;
				else if ( L"information" == sValue ) this->m_eValue = errorStyleInformation;
				else									this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case errorStyleInformation	:	return L"information";
					case errorStyleWarning :		return L"warning";
					case errorStyleStop:	
					default :						return L"stop";
				}
			}

			SimpleType_FromString     (EDataValidationErrorStyle)
			SimpleType_Operator_Equal (CDataValidationErrorStyle)
		};
		enum EDataValidationOperator 
		{
			operatorBetween				=  0,
			operatorNotBetween			=  1,
			operatorEqual				=  2,
			operatorNotEqual			=  3,
			operatorLessThan			=  4,
			operatorLessThanOrEqual		=  5,
			operatorGreaterThan			=  6,
			operatorGreaterThanOrEqual	=  7
		};
		template<EDataValidationOperator eDefValue = operatorBetween>
		class CDataValidationOperator : public CSimpleType<EDataValidationOperator, eDefValue>
		{
		public:
			CDataValidationOperator() {}
			CDataValidationOperator(const EDataValidationOperator & val)	{ this->m_eValue = val; }

			virtual EDataValidationOperator FromString(std::wstring &sValue)
			{
				if      ( L"between" ==  sValue )	this->m_eValue = operatorBetween;
				else if ( L"notBetween" ==  sValue ) this->m_eValue = operatorNotBetween;
				else if ( L"equal" ==  sValue ) this->m_eValue = operatorEqual;
				else if ( L"notEqual" ==  sValue ) this->m_eValue = operatorNotEqual;
				else if ( L"lessThan" ==  sValue ) this->m_eValue = operatorLessThan;
				else if ( L"lessThanOrEqual" == sValue ) this->m_eValue = operatorLessThanOrEqual;
				else if ( L"greaterThan" ==  sValue ) this->m_eValue = operatorGreaterThan;
				else if ( L"greaterThanOrEqual" ==  sValue ) this->m_eValue = operatorGreaterThanOrEqual;
				else										this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case operatorBetween	:		return L"between";
					case operatorNotBetween	:		return L"notBetween";
					case operatorEqual :			return L"equal";
					case operatorNotEqual :			return L"notEqual";
					case operatorLessThan :			return L"lessThan";
					case operatorLessThanOrEqual :	return L"lessThanOrEqual";
					case operatorGreaterThan:		return L"greaterThan";
					case operatorGreaterThanOrEqual:return L"greaterThanOrEqual";
					default :						return L"between";
				}
			}

			SimpleType_FromString     (EDataValidationOperator)
			SimpleType_Operator_Equal (CDataValidationOperator)
		};
		enum EDataValidationImeMode 
		{
			imeModeNoControl		=  0,
			imeModeOff				=  1,
			imeModeOn				=  2,
			imeModeDisabled		=  3,
			imeModeHiragana		=  4,
			imeModeFullKatakana	=  5,
			imeModeHalfKatakana	=  6,
			imeModeFullAlpha		=  7,
			imeModeHalfAlpha		=  8,
			imeModeFullHangul		=  9,
			imeModeHalfHangul		=  10
		};
		template<EDataValidationImeMode eDefValue = imeModeNoControl>
		class CDataValidationImeMode : public CSimpleType<EDataValidationImeMode, eDefValue>
		{
		public:
			CDataValidationImeMode() {}
			CDataValidationImeMode(const EDataValidationImeMode & val)	{ this->m_eValue = val; }

			virtual EDataValidationImeMode FromString(std::wstring &sValue)
			{
				if      ( L"noControl" == sValue )	this->m_eValue = imeModeNoControl;
				else if ( L"off" ==  sValue ) this->m_eValue = imeModeOff;
				else if ( L"on" ==  sValue ) this->m_eValue = imeModeOn;
				else if ( L"disabled" == sValue ) this->m_eValue = imeModeDisabled;
				else if ( L"hiragana" == sValue ) this->m_eValue = imeModeHiragana;
				else if ( L"fullKatakana" ==  sValue ) this->m_eValue = imeModeFullKatakana;
				else if ( L"halfKatakana" ==  sValue ) this->m_eValue = imeModeHalfKatakana;
				else if ( L"fullAlpha" == sValue ) this->m_eValue = imeModeFullAlpha;
				else if ( L"halfAlpha" == sValue ) this->m_eValue = imeModeHalfAlpha;
				else if ( L"fullHangul" == sValue ) this->m_eValue = imeModeFullHangul;
				else if ( L"halfHangul" == sValue ) this->m_eValue = imeModeHalfHangul;
				else									this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case imeModeNoControl	:	return L"noControl";
					case imeModeOff	:			return L"off";
					case imeModeOn :			return L"on";
					case imeModeDisabled :		return L"disabled";
					case imeModeHiragana :		return L"hiragana";
					case imeModeFullKatakana :	return L"fullKatakana";
					case imeModeHalfKatakana:	return L"halfKatakana";
					case imeModeFullAlpha:		return L"fullAlpha";
					case imeModeHalfAlpha:		return L"halfAlpha";
					case imeModeFullHangul:		return L"fullHangul";
					case imeModeHalfHangul:		return L"halfHangul";
					default :					return L"noControl";
				}
			}

			SimpleType_FromString     (EDataValidationImeMode)
			SimpleType_Operator_Equal (CDataValidationImeMode)
		};
		enum EObjectType 
		{
			objectButton	= 0,
			objectCheckBox	= 1,
			objectDrop		= 2,
			objectGBox		= 3,
			objectLabel		= 4,
			objectList		= 5,
			objectRadio		= 6,
			objectScroll	= 7,
			objectSpin		= 8,
			objectEditBox	= 9,
			objectDialog	= 10
		};
		template<EObjectType eDefValue = objectButton>
		class CObjectType : public CSimpleType<EObjectType, eDefValue>
		{
		public:
			CObjectType() {}
			CObjectType(const EObjectType & val)	{ this->m_eValue = val; }

			virtual EObjectType FromString(std::wstring &sValue)
			{
				if      ( L"Button" == sValue )	this->m_eValue = objectButton;
				else if ( L"CheckBox" ==  sValue ) this->m_eValue = objectCheckBox;
				else if ( L"Drop" == sValue ) this->m_eValue = objectDrop;
				else if ( L"GBox" == sValue ) this->m_eValue = objectGBox;
				else if ( L"Label" == sValue ) this->m_eValue = objectLabel;
				else if ( L"List" == sValue ) this->m_eValue = objectList;
				else if ( L"Radio" == sValue ) this->m_eValue = objectRadio;
				else if ( L"Scroll" == sValue ) this->m_eValue = objectScroll;
				else if ( L"Spin" == sValue ) this->m_eValue = objectSpin;
				else if ( L"EditBox" == sValue ) this->m_eValue = objectEditBox;
				else if ( L"Dialog" == sValue ) this->m_eValue = objectDialog;
				else								this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case objectButton	:	return L"Button";
					case objectCheckBox	:	return L"CheckBox";
					case objectDrop :		return L"Drop";
					case objectGBox :		return L"GBox";
					case objectLabel :		return L"Label";
					case objectList :		return L"List";
					case objectRadio:		return L"Radio";
					case objectScroll:		return L"Scroll";
					case objectSpin:		return L"Spin";
					case objectEditBox:		return L"EditBox";
					case objectDialog:		return L"Dialog";
					default :				return L"Button";
				}
			}
			SimpleType_FromString     (EObjectType)
			SimpleType_Operator_Equal (CObjectType)
		};
		enum EChecked 
		{
			valUnchecked	=  0,
			valChecked		=  1,
			valMixed		=  2
		};
		template<EChecked eDefValue = valUnchecked>
		class CChecked : public CSimpleType<EChecked, eDefValue>
		{
		public:
			CChecked() {}
			CChecked(const EChecked & val)	{ this->m_eValue = val; }

			virtual EChecked FromString(std::wstring &sValue)
			{
				if      ( L"Mixed" ==  sValue )	this->m_eValue = valMixed;
				else if ( L"Checked" ==  sValue ) this->m_eValue = valChecked;
				else if ( L"Unchecked" == sValue ) this->m_eValue = valUnchecked;
				else if ( L"1" ==  sValue ) this->m_eValue = valChecked;
				else if ( L"0" ==  sValue ) this->m_eValue = valUnchecked;
				else									this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case valMixed	:	return L"Mixed";
					case valChecked :	return L"Checked";
					case valUnchecked:	
					default :			return L"Unchecked";
				}
			}

			SimpleType_FromString     (EChecked)
			SimpleType_Operator_Equal (CChecked)
		};
 		enum EDropStyle 
		{
			valCombo		=  0,
			valComboedit	=  1,
			valSimple		=  2
		};
		template<EDropStyle eDefValue = valSimple>
		class CDropStyle : public CSimpleType<EDropStyle, eDefValue>
		{
		public:
			CDropStyle() {}
			CDropStyle(const EDropStyle & val)	{ this->m_eValue = val; }

			virtual EDropStyle FromString(std::wstring &sValue)
			{
				if ( L"combo" ==  sValue )	this->m_eValue = valCombo;
				else if ( L"comboedit" == sValue ) this->m_eValue = valComboedit;
				else if ( L"simple" ==  sValue ) this->m_eValue = valSimple;
				else this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case valCombo	:	return L"combo";
					case valComboedit :	return L"comboedit";
					case valSimple:	
					default :			return L"simple";
				}
			}

			SimpleType_FromString     (EDropStyle)
			SimpleType_Operator_Equal (CDropStyle)
		};
  		enum ESelType 
		{
			valSingle	=  0,
			valMulti	=  1,
			valExtended	=  2
		};
		template<ESelType eDefValue = valSingle>
		class CSelType : public CSimpleType<ESelType, eDefValue>
		{
		public:
			CSelType() {}
			CSelType(const ESelType & val)	{ this->m_eValue = val; }

			virtual ESelType FromString(std::wstring &sValue)
			{
				if      ( L"extended" ==  sValue )	this->m_eValue = valExtended;
				else if ( L"multi" == sValue ) this->m_eValue = valMulti;
				else if ( L"single" == sValue ) this->m_eValue = valSingle;
				else								this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case valExtended:	return L"extended";
					case valMulti :		return L"multi";
					case valSingle:	
					default :			return L"single";
				}
			}

			SimpleType_FromString     (ESelType)
			SimpleType_Operator_Equal (CSelType)
		};
  		enum EEditValidation 
		{
			editText		=  0,
			editInteger		=  1,
			editNumber		=  2,
			editReference	=  3,
			editFormula		=  4
		};
		template<EEditValidation eDefValue = editText>
		class CEditValidation : public CSimpleType<EEditValidation, eDefValue>
		{
		public:
			CEditValidation() {}
			CEditValidation(const EEditValidation & val)	{ this->m_eValue = val; }

			virtual EEditValidation FromString(std::wstring &sValue)
			{
				if      ( L"text" ==  sValue )	this->m_eValue = editText;
				else if ( L"integer" ==  sValue ) this->m_eValue = editInteger;
				else if ( L"number" ==  sValue ) this->m_eValue = editNumber;
				else if ( L"reference" == sValue ) this->m_eValue = editReference;
				else if ( L"formula" ==  sValue ) this->m_eValue = editFormula;
				else									this->m_eValue = eDefValue;
				return this->m_eValue;
			}

			virtual std::wstring ToString  () const
			{
				switch(this->m_eValue)
				{
					case editInteger:		return L"integer";
					case editNumber :		return L"number";
					case editReference :	return L"reference";
					case editFormula :		return L"formula";
					case editText:	
					default :				return L"text";
				}
			}

			SimpleType_FromString     (EEditValidation)
			SimpleType_Operator_Equal (CEditValidation)
		};

		enum ETabularSlicerCacheSortOrder
		{
			ctabularslicercachesortorderAscending = 0,
			ctabularslicercachesortorderDescending = 1
		};

		template<ETabularSlicerCacheSortOrder eDefValue = ctabularslicercachesortorderAscending>
		class CTabularSlicerCacheSortOrder : public CSimpleType<ETabularSlicerCacheSortOrder, eDefValue>
		{
		public:
			CTabularSlicerCacheSortOrder(){}
                        CTabularSlicerCacheSortOrder(const ETabularSlicerCacheSortOrder & val)	{ this->m_eValue = val; }
			virtual ETabularSlicerCacheSortOrder FromString(std::wstring &sValue)
			{
				if(L"ascending" == sValue)
					this->m_eValue = ctabularslicercachesortorderAscending;
				else if(L"descending" == sValue)
					this->m_eValue = ctabularslicercachesortorderDescending;
				else
					this->m_eValue = eDefValue;
				return this->m_eValue;
			}
			ETabularSlicerCacheSortOrder FromStringA(const char* sValue)
			{
				if(strcmp("ascending", sValue) == 0)
					this->m_eValue = ctabularslicercachesortorderAscending;
				else if(strcmp("descending", sValue) == 0)
					this->m_eValue = ctabularslicercachesortorderDescending;
				else
					this->m_eValue = eDefValue;
				return this->m_eValue;
			}
			virtual std::wstring ToString() const
			{
				switch(this->m_eValue)
				{
				case ctabularslicercachesortorderAscending : return L"ascending";break;
				case ctabularslicercachesortorderDescending : return L"descending";break;
				}
				return L"ascending";
			}
			SimpleType_FromString(ETabularSlicerCacheSortOrder)
			SimpleType_Operator_Equal(CTabularSlicerCacheSortOrder)
		};
		enum ESlicerCacheCrossFilter
		{
			cslicercachecrossfilterNone = 0,
			cslicercachecrossfilterShowItemsWithDataAtTop = 1,
			cslicercachecrossfilterShowItemsWithNoData = 2
		};

		template<ESlicerCacheCrossFilter eDefValue = cslicercachecrossfilterNone>
		class CSlicerCacheCrossFilter : public CSimpleType<ESlicerCacheCrossFilter, eDefValue>
		{
		public:
			CSlicerCacheCrossFilter(){}
                        CSlicerCacheCrossFilter(const ESlicerCacheCrossFilter & val)	{ this->m_eValue = val; }
			virtual ESlicerCacheCrossFilter FromString(std::wstring &sValue)
			{
				if(L"none" == sValue)
					this->m_eValue = cslicercachecrossfilterNone;
				else if(L"showItemsWithDataAtTop" == sValue)
					this->m_eValue = cslicercachecrossfilterShowItemsWithDataAtTop;
				else if(L"showItemsWithNoData" == sValue)
					this->m_eValue = cslicercachecrossfilterShowItemsWithNoData;
				else
					this->m_eValue = eDefValue;
				return this->m_eValue;
			}
			ESlicerCacheCrossFilter FromStringA(const char* sValue)
			{
				if(strcmp("none", sValue) == 0)
					this->m_eValue = cslicercachecrossfilterNone;
				else if(strcmp("showItemsWithDataAtTop", sValue) == 0)
					this->m_eValue = cslicercachecrossfilterShowItemsWithDataAtTop;
				else if(strcmp("showItemsWithNoData", sValue) == 0)
					this->m_eValue = cslicercachecrossfilterShowItemsWithNoData;
				else
					this->m_eValue = eDefValue;
				return this->m_eValue;
			}
			virtual std::wstring ToString() const
			{
				switch(this->m_eValue)
				{
				case cslicercachecrossfilterNone : return L"none";break;
				case cslicercachecrossfilterShowItemsWithDataAtTop : return L"showItemsWithDataAtTop";break;
				case cslicercachecrossfilterShowItemsWithNoData : return L"showItemsWithNoData";break;
				}
				return L"none";
			}
			SimpleType_FromString(ESlicerCacheCrossFilter)
			SimpleType_Operator_Equal(CSlicerCacheCrossFilter)
		};
		enum EOlapSlicerCacheSortOrder
		{
			colapslicercachesortorderNatural = 0,
			colapslicercachesortorderAscending = 1,
			colapslicercachesortorderDescending = 2
		};

		template<EOlapSlicerCacheSortOrder eDefValue = colapslicercachesortorderNatural>
		class COlapSlicerCacheSortOrder : public CSimpleType<EOlapSlicerCacheSortOrder, eDefValue>
		{
		public:
			COlapSlicerCacheSortOrder(){}
                        COlapSlicerCacheSortOrder(const EOlapSlicerCacheSortOrder & val)	{ this->m_eValue = val; }
			virtual EOlapSlicerCacheSortOrder FromString(std::wstring &sValue)
			{
				if(L"natural" == sValue)
					this->m_eValue = colapslicercachesortorderNatural;
				else if(L"ascending" == sValue)
					this->m_eValue = colapslicercachesortorderAscending;
				else if(L"descending" == sValue)
					this->m_eValue = colapslicercachesortorderDescending;
				else
					this->m_eValue = eDefValue;
				return this->m_eValue;
			}
			EOlapSlicerCacheSortOrder FromStringA(const char* sValue)
			{
				if(strcmp("natural", sValue) == 0)
					this->m_eValue = colapslicercachesortorderNatural;
				else if(strcmp("ascending", sValue) == 0)
					this->m_eValue = colapslicercachesortorderAscending;
				else if(strcmp("descending", sValue) == 0)
					this->m_eValue = colapslicercachesortorderDescending;
				else
					this->m_eValue = eDefValue;
				return this->m_eValue;
			}
			virtual std::wstring ToString() const
			{
				switch(this->m_eValue)
				{
				case colapslicercachesortorderNatural : return L"natural";break;
				case colapslicercachesortorderAscending : return L"ascending";break;
				case colapslicercachesortorderDescending : return L"descending";break;
				}
				return L"natural";
			}
			SimpleType_FromString(EOlapSlicerCacheSortOrder)
			SimpleType_Operator_Equal(COlapSlicerCacheSortOrder)
		};

		enum ESlicerStyleType
		{
			cslicerstyletypeUnselectedItemWithData = 0,
			cslicerstyletypeSelectedItemWithData = 1,
			cslicerstyletypeUnselectedItemWithNoData = 2,
			cslicerstyletypeSelectedItemWithNoData = 3,
			cslicerstyletypeHoveredUnselectedItemWithData = 4,
			cslicerstyletypeHoveredSelectedItemWithData = 5,
			cslicerstyletypeHoveredUnselectedItemWithNoData = 6,
			cslicerstyletypeHoveredSelectedItemWithNoData = 7
		};

		template<ESlicerStyleType eDefValue = cslicerstyletypeUnselectedItemWithData>
		class CSlicerStyleType : public CSimpleType<ESlicerStyleType, eDefValue>
		{
		public:
			CSlicerStyleType(){}
			virtual ESlicerStyleType FromString(std::wstring &sValue)
			{
				if(L"unselectedItemWithData" == sValue)
					this->m_eValue = cslicerstyletypeUnselectedItemWithData;
				else if(L"selectedItemWithData" == sValue)
					this->m_eValue = cslicerstyletypeSelectedItemWithData;
				else if(L"unselectedItemWithNoData" == sValue)
					this->m_eValue = cslicerstyletypeUnselectedItemWithNoData;
				else if(L"selectedItemWithNoData" == sValue)
					this->m_eValue = cslicerstyletypeSelectedItemWithNoData;
				else if(L"hoveredUnselectedItemWithData" == sValue)
					this->m_eValue = cslicerstyletypeHoveredUnselectedItemWithData;
				else if(L"hoveredSelectedItemWithData" == sValue)
					this->m_eValue = cslicerstyletypeHoveredSelectedItemWithData;
				else if(L"hoveredUnselectedItemWithNoData" == sValue)
					this->m_eValue = cslicerstyletypeHoveredUnselectedItemWithNoData;
				else if(L"hoveredSelectedItemWithNoData" == sValue)
					this->m_eValue = cslicerstyletypeHoveredSelectedItemWithNoData;
				else
					this->m_eValue = eDefValue;
				return this->m_eValue;
			}
			ESlicerStyleType FromStringA(const char* sValue)
			{
				if(strcmp("unselectedItemWithData", sValue) == 0)
					this->m_eValue = cslicerstyletypeUnselectedItemWithData;
				else if(strcmp("selectedItemWithData", sValue) == 0)
					this->m_eValue = cslicerstyletypeSelectedItemWithData;
				else if(strcmp("unselectedItemWithNoData", sValue) == 0)
					this->m_eValue = cslicerstyletypeUnselectedItemWithNoData;
				else if(strcmp("selectedItemWithNoData", sValue) == 0)
					this->m_eValue = cslicerstyletypeSelectedItemWithNoData;
				else if(strcmp("hoveredUnselectedItemWithData", sValue) == 0)
					this->m_eValue = cslicerstyletypeHoveredUnselectedItemWithData;
				else if(strcmp("hoveredSelectedItemWithData", sValue) == 0)
					this->m_eValue = cslicerstyletypeHoveredSelectedItemWithData;
				else if(strcmp("hoveredUnselectedItemWithNoData", sValue) == 0)
					this->m_eValue = cslicerstyletypeHoveredUnselectedItemWithNoData;
				else if(strcmp("hoveredSelectedItemWithNoData", sValue) == 0)
					this->m_eValue = cslicerstyletypeHoveredSelectedItemWithNoData;
				else
					this->m_eValue = eDefValue;
				return this->m_eValue;
			}
			virtual std::wstring ToString() const
			{
				switch(this->m_eValue)
				{
				case cslicerstyletypeUnselectedItemWithData : return L"unselectedItemWithData";break;
				case cslicerstyletypeSelectedItemWithData : return L"selectedItemWithData";break;
				case cslicerstyletypeUnselectedItemWithNoData : return L"unselectedItemWithNoData";break;
				case cslicerstyletypeSelectedItemWithNoData : return L"selectedItemWithNoData";break;
				case cslicerstyletypeHoveredUnselectedItemWithData : return L"hoveredUnselectedItemWithData";break;
				case cslicerstyletypeHoveredSelectedItemWithData : return L"hoveredSelectedItemWithData";break;
				case cslicerstyletypeHoveredUnselectedItemWithNoData : return L"hoveredUnselectedItemWithNoData";break;
				case cslicerstyletypeHoveredSelectedItemWithNoData : return L"hoveredSelectedItemWithNoData";break;
				}
				return L"unselectedItemWithData";
			}
			SimpleType_FromString(ESlicerStyleType)
			SimpleType_Operator_Equal(CSlicerStyleType)
		};

	}// Spreadsheet
} // SimpleTypes
