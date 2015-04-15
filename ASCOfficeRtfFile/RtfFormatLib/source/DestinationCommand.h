#pragma once 

#include "RtfReader.h"
#include "RtfDocument.h"
#include "RtfChar.h"
#include "RtfField.h"
#include "RtfShape.h"
#include "RtfMath.h"
#include "RtfTable.h"
#include "RtfBookmark.h"
#include "RtfOle.h"

#include "Writer/OOXWriter.h"
#include "Writer/OOXRelsWriter.h"

#define COMMAND_RTF_BOOL( pattern, target, command, hasParameter, parameter )\
		else if( pattern == command )\
		{\
			if( true == hasParameter && 0 == parameter)\
				target = 0;\
			else\
				target = 1;\
		}
#define COMMAND_RTF_INT( pattern, target, command, hasParameter, parameter )\
		else if( pattern == command )\
		{\
			if( true == hasParameter )\
				target = parameter;\
		}
#define COMMAND_RTF_SPECIAL_CHAR( pattern, target, command, hasParameter, parameter )\
		else if( pattern == command  )\
		{\
			RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );\
			oNewChar->m_eType = parameter;\
			oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;\
			target->AddItem( oNewChar );\
		}
//Command не имеет состояний
#include "math.h"
class RtfBorderCommand
{
public:static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter,RtfBorder& oOutput)
		{
			if( _T("brdrs") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrs;
			else if( _T("brdrth") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrth;
			else if( _T("brdrsh") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrsh;
			else if( _T("brdrdb") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdb;
			else if( _T("brdrdot") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdot;
			else if( _T("brdrdash") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdash;
			else if( _T("brdrhair") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrhair;
			else if( _T("brdrdashsm") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdashsm;
			else if( _T("brdrdashd") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdashd;
			else if( _T("brdrdashdd") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdashdd;
			else if( _T("brdrdashdot") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdashdot;
			else if( _T("brdrdashdotdot") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdashdot;
			else if( _T("brdrtriple") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrtriple;
			else if( _T("brdrtnthsg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrtnthsg;
			else if( _T("brdrthtnsg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrthtnsg;
			else if( _T("brdrtnthtnsg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrtnthtnsg;
			else if( _T("brdrtnthmg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrtnthmg;
			else if( _T("brdrthtnmg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrthtnmg;
			else if( _T("brdrtnthtnmg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrtnthtnmg;
			else if( _T("brdrtnthlg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrtnthlg;
			else if( _T("brdrthtnlg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrthtnlg;
			else if( _T("brdrtnthtnlg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrtnthtnlg;
			else if( _T("brdrwavy") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrwavy;
			else if( _T("brdrwavydb") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrwavydb;
			else if( _T("brdrdashdotstr") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdashdotstr;
			else if( _T("brdremboss") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdremboss;
			else if( _T("brdrengrave") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrengrave;
			else if( _T("brdroutset") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdroutset;
			else if( _T("brdrinset") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrinset;

			else if( _T("brdrnone") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrnone;
			else if( _T("brdrw") == sCommand )
			{
				if( true == hasParameter )
				{
					oOutput.m_nWidth = parameter;
					if( RtfBorder::bt_none == oOutput.m_eType )
						oOutput.m_eType = RtfBorder::bt_brdrs;
				}
			}
			else if( _T("brsp") == sCommand )
			{
				if( true == hasParameter )
				{
					oOutput.m_nSpace = parameter;
					if( RtfBorder::bt_none == oOutput.m_eType )
						oOutput.m_eType = RtfBorder::bt_brdrs;
				}
			}
			else if( _T("brdrcf") == sCommand )
			{
				if( true == hasParameter )
				{
					oOutput.m_nColor = parameter;
					if( RtfBorder::bt_none == oOutput.m_eType )
						oOutput.m_eType = RtfBorder::bt_brdrs;
				}
			}
			else
				return false;
			return true;
		}
};
class RtfShadingCommand
{
public:
		static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput )
		{
			if( _T("bghoriz") == sCommand )
				oOutput.m_eType = RtfShading::st_chbghoriz;
			else if( _T("bgvert") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgvert;
			else if( _T("bgfdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgfdiag;
			else if( _T("bgbdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgbdiag;
			else if( _T("bgcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgcross;
			else if( _T("bgdcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdcross;
			else if( _T("bgdkhoriz") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkhoriz;
			else if( _T("bgdkvert") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkvert;
			else if( _T("bgdkfdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkfdiag;
			else if( _T("bgdkbdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkbdiag;
			else if( _T("bgdkcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkcross;
			else if( _T("bgdkdcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkdcross;
			else if( _T("cfpat") == sCommand )
			{
				if( true == hasParameter )
					oOutput.m_nForeColor = parameter;
			}
			else if( _T("cbpat") == sCommand )
			{
				if( true == hasParameter )
					oOutput.m_nBackColor = parameter;
			}
			else if( _T("shading") == sCommand )
			{
				if( true == hasParameter )
					oOutput.m_nValue = parameter;
			}
			else
				return false;
			return true;
		}
};
class RtfShadingCellCommand
{
public: 
		static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput)
		{
			//для свойст таблицы и для стилей таблицы
			if( _T("clshdrawnil") == sCommand )
				oOutput.m_eType = RtfShading::st_clshdrawnil;
			else if( _T("rawclbgvert") == sCommand || _T("clbgvert") == sCommand || _T("tsbgvert") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgvert;
			else if( _T("rawclbgfdiag") == sCommand || _T("clbgfdiag") == sCommand || _T("tsbgfdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgfdiag;
			else if( _T("rawclbgbdiag") == sCommand || _T("clbgbdiag") == sCommand || _T("tsbgbdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgbdiag;
			else if( _T("rawclbgcross") == sCommand || _T("clbgcross") == sCommand || _T("tsbgcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgcross;
			else if( _T("rawclbgdcross") == sCommand || _T("clbgdcross") == sCommand || _T("tsbgdcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdcross;
			else if( _T("rawclbgdkhor") == sCommand || _T("clbgdkhor") == sCommand || _T("tsbgdkhor") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkhoriz;
			else if( _T("rawclbgdkvert") == sCommand || _T("clbgdkvert") == sCommand || _T("tsbgdkvert") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkvert;
			else if( _T("rawclbgdkfdiag") == sCommand || _T("clbgdkfdiag") == sCommand || _T("tsbgdkfdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkfdiag;
			else if( _T("rawclbgdkbdiag") == sCommand || _T("clbgdkbdiag") == sCommand || _T("tsbgdkbdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkbdiag;
			else if( _T("rawclbgdkcross") == sCommand || _T("clbgdkcross") == sCommand || _T("tsbgdkcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkcross;
			else if( _T("rawclbgdkdcross") == sCommand || _T("clbgdkdcross") == sCommand || _T("tsbgdkdcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkdcross;
			COMMAND_RTF_INT( _T("clcfpat"), oOutput.m_nForeColor, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("clcbpat"), oOutput.m_nBackColor, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("clcfpatraw"), oOutput.m_nForeColor, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("clcbpatraw"), oOutput.m_nBackColor, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("tscellcfpat"), oOutput.m_nForeColor, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("tscellcbpat"), oOutput.m_nBackColor, sCommand, hasParameter, parameter )

			COMMAND_RTF_INT( _T("clshdng"), oOutput.m_nValue, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("clshdngraw"), oOutput.m_nValue, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("tscellpct"), oOutput.m_nValue, sCommand, hasParameter, parameter )
			else
				return false;
			return true;
		}
};

class RtfShadingCharCommand
{
public: 
		static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput )
		{
			if( _T("chbghoriz") == sCommand )
				oOutput.m_eType = RtfShading::st_chbghoriz;
			else if( _T("chbgvert") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgvert;
			else if( _T("chbgfdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgfdiag;
			else if( _T("chbgbdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgbdiag;
			else if( _T("chbgcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgcross;
			else if( _T("chbgdcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdcross;
			else if( _T("chbgdkhoriz") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkhoriz;
			else if( _T("chbgdkvert") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkvert;
			else if(  _T("chbgdkfdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkfdiag;
			else if( _T("chbgdkbdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkbdiag;
			else if(  _T("chbgdkcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkcross;
			else if( _T("chbgdkdcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkdcross;
			else if( _T("chcfpat") == sCommand )
			{
				if( true == hasParameter )
					oOutput.m_nForeColor = parameter;
			}
			else if( _T("chcbpat") == sCommand )
			{
				if( true == hasParameter )
					oOutput.m_nBackColor = parameter;
			}
			else if( _T("chshdng") == sCommand )
			{
				if( true == hasParameter )
					oOutput.m_nValue = parameter;
			}
			else
				return false;
			return true;
		}
};
class RtfShadingRowCommand
{
public: 
		static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput)
		{
			if( _T("trbghoriz") == sCommand )
				oOutput.m_eType = RtfShading::st_chbghoriz;
			else if( _T("trbgvert") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgvert;
			else if( _T("trbgfdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgfdiag;
			else if( _T("trbgbdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgbdiag;
			else if( _T("trbgcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgcross;
			else if( _T("trbgdcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdcross;
			else if( _T("trbgdkhor") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkhoriz;
			else if( _T("trbgdkvert") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkvert;
			else if( _T("trbgdkfdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkfdiag;
			else if( _T("trbgdkbdiag") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkbdiag;
			else if( _T("trbgdkcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkcross;
			else if( _T("trbgdkdcross") == sCommand )
				oOutput.m_eType = RtfShading::st_chbgdkdcross;
			else if( _T("trcfpat") == sCommand )
			{
				if( true == hasParameter )
					oOutput.m_nForeColor = parameter;
			}
			else if( _T("trcbpat") == sCommand )
			{
				if( true == hasParameter )
					oOutput.m_nBackColor = parameter;
			}
			else if( _T("trshdng") == sCommand )
			{
				if( true == hasParameter )
					oOutput.m_nValue = parameter;
			}
			else
				return false;
			return true;
		}
};
class RtfFrameCommand
{
public:static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfFrame& oOutput )
		{
			return true;
		}
};
//class RtfOldList;
class TextReader: public RtfAbstractReader
{
private: 
	CString& m_sName;
	bool m_bErease;
public:
	TextReader( CString& sName, bool bErease = true ):m_sName(sName),m_bErease(bErease)
	{
	}
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
	{
		if( true == m_bErease )
			sText.Replace(_T(";"),_T(""));
		m_sName += sText;
	}
};
class RtfCharPropCommand
{
//bool bLookOnBorder; Надо ли читать свойства border( актуалЃEЃEдля ParagraphReader )
public:
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, bool bLookOnBorder = true)
	{
		if( _T("plain") == sCommand )
			oReader.m_oState->m_oCharProp.SetDefaultRtf();
		COMMAND_RTF_INT( _T("animtext"), oReader.m_oState->m_oCharProp.m_nAnimated, sCommand, hasParameter, parameter)
		COMMAND_RTF_INT( _T("uc"), oReader.m_oState->m_nUD, sCommand, hasParameter, parameter)
		COMMAND_RTF_BOOL( _T("b"), oReader.m_oState->m_oCharProp.m_bBold, sCommand, hasParameter, parameter)
		COMMAND_RTF_BOOL( _T("caps"), oReader.m_oState->m_oCharProp.m_bCaps, sCommand, hasParameter, parameter)
		COMMAND_RTF_INT( _T("charscalex"), oReader.m_oState->m_oCharProp.m_nScalex, sCommand, hasParameter, parameter)
		COMMAND_RTF_INT( _T("cs"), oReader.m_oState->m_oCharProp.m_nCharStyle, sCommand, hasParameter, parameter)
		COMMAND_RTF_INT( _T("down"), oReader.m_oState->m_oCharProp.m_nDown, sCommand, hasParameter, parameter)
		COMMAND_RTF_BOOL( _T("embo"), oReader.m_oState->m_oCharProp.m_bEmbo, sCommand, hasParameter, parameter)
		COMMAND_RTF_INT( _T("expndtw"), oReader.m_oState->m_oCharProp.m_nCharacterSpacing, sCommand, hasParameter, parameter)
		else if( _T("expnd") == sCommand )
		{
			if( true == hasParameter )
				oReader.m_oState->m_oCharProp.m_nCharacterSpacing  = 5 * parameter; //quater -points
		}
		COMMAND_RTF_INT( _T("fittext"), oReader.m_oState->m_oCharProp.m_nFitText, sCommand, hasParameter, parameter)
		COMMAND_RTF_INT( _T("f"), oReader.m_oState->m_oCharProp.m_nFont, sCommand, hasParameter, parameter)
		COMMAND_RTF_INT( _T("fs"), oReader.m_oState->m_oCharProp.m_nFontSize, sCommand, hasParameter, parameter)
		COMMAND_RTF_BOOL( _T("i"), oReader.m_oState->m_oCharProp.m_bItalic, sCommand, hasParameter, parameter)
		COMMAND_RTF_BOOL( _T("impr"), oReader.m_oState->m_oCharProp.m_bImprint, sCommand, hasParameter, parameter)
		COMMAND_RTF_INT( _T("kerning"), oReader.m_oState->m_oCharProp.m_nKerning, sCommand, hasParameter, parameter)
		else if( _T("ltrch") == sCommand )
		{
			if( false == hasParameter || 0 != parameter ) 
				oReader.m_oState->m_oCharProp.m_bRightToLeft  = 0;
			else
				oReader.m_oState->m_oCharProp.m_bRightToLeft  = 1;
		}
		COMMAND_RTF_BOOL( _T("cs"), oReader.m_oState->m_oCharProp.m_nComplexScript, sCommand, hasParameter, parameter)
		COMMAND_RTF_BOOL( _T("outl"), oReader.m_oState->m_oCharProp.m_bOutline, sCommand, hasParameter, parameter)
		COMMAND_RTF_BOOL( _T("scaps"), oReader.m_oState->m_oCharProp.m_bScaps, sCommand, hasParameter, parameter)
		COMMAND_RTF_BOOL( _T("shad"), oReader.m_oState->m_oCharProp.m_bShadow, sCommand, hasParameter, parameter)
		COMMAND_RTF_BOOL( _T("strike"), oReader.m_oState->m_oCharProp.m_bStrike, sCommand, hasParameter, parameter)
		COMMAND_RTF_BOOL( _T("striked"), oReader.m_oState->m_oCharProp.m_nStriked, sCommand, hasParameter, parameter)
		COMMAND_RTF_BOOL( _T("sub"), oReader.m_oState->m_oCharProp.m_bSub, sCommand, hasParameter, parameter)
		COMMAND_RTF_BOOL( _T("super"), oReader.m_oState->m_oCharProp.m_bSuper, sCommand, hasParameter, parameter)
		COMMAND_RTF_INT( _T("highlight"), oReader.m_oState->m_oCharProp.m_nHightlited, sCommand, hasParameter, parameter)
		else if( _T("cf") == sCommand )
		{
			if( true == hasParameter )
				oReader.m_oState->m_oCharProp.m_nForeColor = parameter;
			else
				oReader.m_oState->m_oCharProp.m_nForeColor= PROP_DEF;
		}
		else if( _T("ul") == sCommand )
		{
			if( true == hasParameter && 0 == parameter)
				oReader.m_oState->m_oCharProp.m_eUnderStyle = RtfCharProperty::uls_none;
			else
				oReader.m_oState->m_oCharProp.m_eUnderStyle = RtfCharProperty::uls_Single;
		}
		//COMMAND_RTF_BOOL( _T("ul"), oReader.m_oState->m_oCharProp.m_bUnderline, sCommand, hasParameter, parameter)
		COMMAND_RTF_INT( _T("ulc"), oReader.m_oState->m_oCharProp.m_nUnderlineColor, sCommand, hasParameter, parameter)


		COMMAND_RTF_INT( _T("uld"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Dotted)
		COMMAND_RTF_INT( _T("uldash"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Dashed)
		COMMAND_RTF_INT( _T("uldashd"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Dash_dotted)
		COMMAND_RTF_INT( _T("uldashdd"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Dash_dot_dotted)
		COMMAND_RTF_INT( _T("uldb"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Double)
		COMMAND_RTF_INT( _T("ulhwave"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Heavy_wave)
		COMMAND_RTF_INT( _T("ulldash"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Long_dashe)
		COMMAND_RTF_INT( _T("ulnone"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_none)
		COMMAND_RTF_INT( _T("ulth"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Thick)
		COMMAND_RTF_INT( _T("ulthd"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Thick_dotted)
		COMMAND_RTF_INT( _T("ulthdash"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Thick_dashed)
		COMMAND_RTF_INT( _T("ulthdashd"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Thick_dash_dotted)
		COMMAND_RTF_INT( _T("ulthdashdd"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Thick_dash_dot_dotted)
		COMMAND_RTF_INT( _T("ulthldash"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Thick_long_dashed)
		COMMAND_RTF_INT( _T("ululdbwave"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Double_wave)
		COMMAND_RTF_INT( _T("ulw"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Word)
		COMMAND_RTF_INT( _T("ulwave"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Wave)
		COMMAND_RTF_INT( _T("up"), oReader.m_oState->m_oCharProp.m_nUp, sCommand, hasParameter, parameter)
		else if( _T("nosupersub") == sCommand )
		{
			oReader.m_oState->m_oCharProp.m_bSub  = 0;
			oReader.m_oState->m_oCharProp.m_bSuper  = 0;
		}
		else if( _T("nosectexpand") == sCommand )
		{
			oReader.m_oState->m_oCharProp.m_nCharacterSpacing  = PROP_DEF;
		}
		else
		{
			bool bResult ;
			bResult = RtfShadingCharCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCharProp.m_poShading);
			if( true == bResult )
					return true;
			if( true == bLookOnBorder )
			{
				bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCharProp.m_poBorder);
				if( true == bResult )
						return true;
			}
			return false;
		}
		return true;
	}
};
//Reader выражения в скобках
class RtfOldListReader : public RtfAbstractReader
{
private: 
	RtfOldList& m_oTarget;
public: 
	RtfOldListReader( RtfOldList& oTarget) : m_oTarget(oTarget)
	{
	}
	bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, CString sKey, bool bHasPar, int nPar )
	 {
		if( _T("pn") == sKey )
			return true;
		else if( _T("pnlvlblt") == sKey )
		{
			m_oTarget.m_eLevelType = RtfOldList::lt_blt;
			m_oTarget.m_nIlvl = 0;
		}
		COMMAND_RTF_INT( _T("pnf"), oReader.m_oState->m_oCharProp.m_nFont, sKey, bHasPar, nPar )
		COMMAND_RTF_INT( _T("pnfs"), oReader.m_oState->m_oCharProp.m_nFontSize, sKey, bHasPar, nPar )
		COMMAND_RTF_BOOL( _T("pnb"), oReader.m_oState->m_oCharProp.m_bBold, sKey, bHasPar, nPar )
		COMMAND_RTF_BOOL( _T("pni"), oReader.m_oState->m_oCharProp.m_bItalic, sKey, bHasPar, nPar )
		COMMAND_RTF_BOOL( _T("pncaps"), oReader.m_oState->m_oCharProp.m_bCaps, sKey, bHasPar, nPar )
		COMMAND_RTF_BOOL( _T("pnstrike"), oReader.m_oState->m_oCharProp.m_bStrike, sKey, bHasPar, nPar )
		COMMAND_RTF_INT( _T("pnul"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Single)
		COMMAND_RTF_INT( _T("pnuld"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Dashed)
		COMMAND_RTF_INT( _T("pnuldash"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Dash_dotted)
		COMMAND_RTF_INT( _T("pnuldashdd"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Dash_dot_dotted)
		COMMAND_RTF_INT( _T("pnulth"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Thick)
		COMMAND_RTF_INT( _T("pnulwave"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Wave)
		COMMAND_RTF_INT( _T("pnuldb"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Double)
		COMMAND_RTF_INT( _T("pnulnone"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_none)
		COMMAND_RTF_INT( _T("pnulnone"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Word)

		COMMAND_RTF_INT( _T("pnindent"), m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )
		COMMAND_RTF_INT( _T("pnsp"), m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )
		COMMAND_RTF_INT( _T("pnstart"), m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )
		COMMAND_RTF_INT( _T("pnhang"), m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )

		COMMAND_RTF_INT( _T("pnqc"), m_oTarget.m_eLevelJust, sKey, true, RtfOldList::lj_center )
		COMMAND_RTF_INT( _T("pnql"), m_oTarget.m_eLevelJust, sKey, true, RtfOldList::lj_left )
		COMMAND_RTF_INT( _T("pnqr"), m_oTarget.m_eLevelJust, sKey, true, RtfOldList::lj_right )

		else if( _T("pntxtb") == sKey )
		{
			CString sNumberingText;
			TextReader oTextReader( sNumberingText, false );
			StartSubReader( oTextReader, oDocument, oReader );
			RtfCharPtr oNewChar = RtfCharPtr( new RtfChar() );
			oNewChar->setText( sNumberingText );
			m_oTarget.m_oLevelText->AddItem( oNewChar );
		}
		else
			return false;
		return true;
	}

	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
	{
		m_oTarget.m_oLevelText->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
	}
};
//class RtfColorSchemeReader: public RtfAbstractReader
//{
//private: CString sSchemeXml;
//public: void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
//		{
//			sSchemeXml += sText;
//		}
//		void ExitReader(RtfDocument& oDocument, RtfReader& oReader)
//		{
//			CString sXml = RtfUtility::EncodeHex(sSchemeXml);
//			XmlUtils::CXmlLiteReader oXmlReader;
//			oXmlReader.OpenFromXmlString(sXml);
//			oXmlReader.ReadRootNode(_T("a:clrMap"));
//			CString sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("bg1"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.bg1 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("tx1"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.t1 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("bg2"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.bg2 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("tx2"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.t2 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("accent1"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.accent1 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("accent2"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.accent3 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("accent3"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.accent4 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("accent4"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.accent5 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("accent5"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.accent6 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("accent6"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.hyperlink = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("hlink"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.hyperlink = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("folHlink"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.followedHyperlink = sValue;
//		}
//};
//class RtfThemeDataReader: public RtfAbstractReader
//{
//public: void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText )
//		{
//			oDocument.m_sThemeData += sText;
//		}
//};
class RtfFontTableReader: public RtfAbstractReader
{
private: 
	RtfFont m_oFont;
	typedef enum { is_normal, is_panose, is_altname } InternalState;
	InternalState m_eInternalState;
public: 
	RtfFontTableReader()
	{
		m_eInternalState = is_normal;
		m_oFont.SetDefaultOOX();
	}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("fonttbl") == sCommand )
				;
			else if( _T("flomajor") == sCommand )
				m_oFont.m_eFontTheme = RtfFont::ft_flomajor;
			else if( _T("fhimajor") == sCommand )
				m_oFont.m_eFontTheme = RtfFont::ft_fhimajor;
			else if( _T("fdbmajor") == sCommand )
				m_oFont.m_eFontTheme = RtfFont::ft_fdbmajor;
			else if( _T("fbimajor") == sCommand )
				m_oFont.m_eFontTheme = RtfFont::ft_fbimajor;
			else if( _T("flominor") == sCommand )
				m_oFont.m_eFontTheme = RtfFont::ft_flominor;
			else if( _T("fhiminor") == sCommand )
				m_oFont.m_eFontTheme = RtfFont::ft_fhiminor;
			else if( _T("fdbminor") == sCommand )
				m_oFont.m_eFontTheme = RtfFont::ft_fdbminor;
			else if( _T("fbiminor") == sCommand )
				m_oFont.m_eFontTheme = RtfFont::ft_fbiminor;

			else if( _T("fnil") == sCommand )
				m_oFont.m_eFontFamily = RtfFont::ff_fnil;
			else if( _T("froman") == sCommand )
				m_oFont.m_eFontFamily = RtfFont::ff_froman;
			else if( _T("fswiss") == sCommand )
				m_oFont.m_eFontFamily = RtfFont::ff_fswiss;
			else if( _T("fmodern") == sCommand )
				m_oFont.m_eFontFamily = RtfFont::ff_fmodern;
			else if( _T("fscript") == sCommand )
				m_oFont.m_eFontFamily = RtfFont::ff_fscript;
			else if( _T("fdecor") == sCommand )
				m_oFont.m_eFontFamily = RtfFont::ff_fdecor;
			else if( _T("ftech") == sCommand )
				m_oFont.m_eFontFamily = RtfFont::ff_ftech;
			else if( _T("fbidi") == sCommand )
				m_oFont.m_eFontFamily = RtfFont::ff_fbidi;

			else if( _T("panose") == sCommand )
				m_eInternalState = is_panose;
			else if( _T("falt") == sCommand )
				m_eInternalState = is_altname;

			else if( _T("f") == sCommand )
			{
				if( true == hasParameter )
					m_oFont.m_nID = parameter;
			}
			else if( _T("fcharset") == sCommand )
			{
				if( true == hasParameter )
					m_oFont.m_nCharset = parameter;
			}
			else if( _T("cpg") == sCommand )
			{
				if( true == hasParameter )
					m_oFont.m_nCodePage = parameter;
			}
			else if( _T("fprq") == sCommand )
			{
				if( true == hasParameter )
					m_oFont.m_nPitch = parameter;
			}
			else
			{
				return false;
				//ATLASSERT( false );
			}
			return true;

		}
		void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
		{
			if( is_panose == m_eInternalState )
				m_oFont.m_sPanose += sText;
			else if( is_altname == m_eInternalState )
				m_oFont.m_sAltName += sText;
			else if( is_normal == m_eInternalState )
			{
				if( sText.Find(';') != -1 )
				{
					sText.Remove(';');
					m_oFont.m_sName += sText;
					oDocument.m_oFontTable.DirectAddItem( m_oFont );
					m_oFont.SetDefaultRtf();
				}
				else
				{
					m_oFont.m_sName += sText;
				}
			}
		}
		void PopState(RtfDocument& oDocument, RtfReader& oReader)
		{
			RtfAbstractReader::PopState( oDocument, oReader );
			if( is_panose == m_eInternalState )
				m_eInternalState = is_normal;
			if( is_altname == m_eInternalState )
				m_eInternalState = is_normal;
		}
};


class RtfColorTableReader : public RtfAbstractReader
{
private: RtfColor oCurColor;
		bool m_bIsSet;

public: RtfColorTableReader()
		{
			oCurColor.SetDefaultRtf();
			m_bIsSet = false;
		}
		bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, CString sKey, bool bHasPar, int nPar )
		 {
			if( _T("colortbl") == sKey )
				return true;
			else if( _T("cmaindarkone") == sKey )
				oCurColor.m_eTheme = RtfColor::cmaindarkone;
			else if( _T("cmainlightone") == sKey )
				oCurColor.m_eTheme = RtfColor::cmainlightone;
			else if( _T("cmaindarktwo") == sKey )
				oCurColor.m_eTheme = RtfColor::cmaindarktwo;
			else if( _T("cmainlighttwo") == sKey )
				oCurColor.m_eTheme = RtfColor::cmainlighttwo;
			else if( _T("caccentone") == sKey )
				oCurColor.m_eTheme = RtfColor::caccentone;
			else if( _T("caccenttwo") == sKey )
				oCurColor.m_eTheme = RtfColor::caccenttwo;
			else if( _T("caccentthree") == sKey )
				oCurColor.m_eTheme = RtfColor::caccentthree;
			else if( _T("caccentfour") == sKey )
				oCurColor.m_eTheme = RtfColor::caccentfour;
			else if( _T("caccentfive") == sKey )
				oCurColor.m_eTheme = RtfColor::caccentfive;
			else if( _T("caccentsix") == sKey )
				oCurColor.m_eTheme = RtfColor::caccentsix;
			else if( _T("chyperlink") == sKey )
				oCurColor.m_eTheme = RtfColor::chyperlink;
			else if( _T("cfollowedhyperlink") == sKey )
				oCurColor.m_eTheme = RtfColor::cfollowedhyperlink;
			else if( _T("cbackgroundone") == sKey )
				oCurColor.m_eTheme = RtfColor::cbackgroundone;
			else if( _T("ctextone") == sKey )
				oCurColor.m_eTheme = RtfColor::ctextone;
			else if( _T("cbackgroundtwo") == sKey )
				oCurColor.m_eTheme = RtfColor::cbackgroundtwo;
			else if( _T("ctexttwo") == sKey )
				oCurColor.m_eTheme = RtfColor::ctexttwo;
			else if( _T("ctint") == sKey && true == bHasPar )
				oCurColor.m_byteTint = nPar;
			else if( _T("cshade") == sKey && true == bHasPar )
				oCurColor.m_byteShade = nPar;
			else if( _T("red") == sKey && true == bHasPar )
				oCurColor.m_byteRed = nPar;
			else if( _T("green") == sKey && true == bHasPar )
				oCurColor.m_byteGreen = nPar;
			else if( _T("blue") == sKey && true == bHasPar )
				oCurColor.m_byteBlue = nPar;
			else
			{
				return false;
				//ATLASSERT( false );
			}	
			m_bIsSet = true;
			return true;
		 }
		void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, CString oText )
		{
			if( oText.Find(';') != -1)
			{
				if( true == m_bIsSet )
					oDocument.m_oColorTable.DirectAddItem( oCurColor );
				oCurColor.SetDefaultRtf();
				m_bIsSet = false;
			}
			else
				;//ATLASSERT(false);
		}
};
class RtfDefCharPropReader: public RtfAbstractReader
{
public: RtfCharProperty& m_oCharProp;
		RtfDefCharPropReader( RtfCharProperty& oOutput ):m_oCharProp(oOutput)
		{
		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("defchp") == sCommand )
				return true;
			else 
				return RtfCharPropCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter );
		}
		void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
		{
			m_oCharProp = oReader.m_oState->m_oCharProp;
		}
};

class RtfInfoReader: public RtfAbstractReader
{
private: typedef enum { is_normal, is_title, is_subject, is_author, is_manager, is_company, is_operator, is_category, is_keywords,
						is_comment, is_doccomm, is_hlinkbase, is_creatim, is_revtim, is_printim, is_buptim } InternalState;
		InternalState m_eInternalState;
public: RtfInfoReader()
		{
			m_eInternalState = is_normal;
		}

		bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("info") == sCommand )
				return true;
			else if( _T("title") == sCommand )
				m_eInternalState = is_title;
			else if( _T("subject") == sCommand )
				m_eInternalState = is_subject;
			else if( _T("author") == sCommand )
				m_eInternalState = is_author;
			else if( _T("manager") == sCommand )
				m_eInternalState = is_manager;
			else if( _T("company") == sCommand )
				m_eInternalState = is_company;
			else if( _T("operator") == sCommand )
				m_eInternalState = is_operator;
			else if( _T("category") == sCommand )
				m_eInternalState = is_category;
			else if( _T("keywords") == sCommand )
				m_eInternalState = is_keywords;
			else if( _T("comment") == sCommand )
				m_eInternalState = is_comment;
			else if( _T("doccomm") == sCommand )
				m_eInternalState = is_doccomm;
			else if( _T("hlinkbase") == sCommand )
				m_eInternalState = is_hlinkbase;
			else if( _T("version") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nVersion = parameter;
			}
			else if( _T("vern") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nInternalVersion = parameter;
			}
			else if( _T("edmins") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nEndingTime = parameter;
			}
			else if( _T("nofpages") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nNumberOfPages= parameter;
			}
			else if( _T("nofwords") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nNumberOfWords= parameter;
			}
			else if( _T("nofchars") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nNumberOfCharactersWithSpace= parameter;
			}
			else if( _T("nofcharsws") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nNumberOfCharactersWithoutSpace= parameter;
			}
			else if( _T("id") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nInternalId= parameter;
			}
			else if( _T("creatim") == sCommand )
				m_eInternalState = is_creatim;
			else if( _T("revtim") == sCommand )
				m_eInternalState = is_revtim;
			else if( _T("printim") == sCommand )
				m_eInternalState = is_printim;
			else if( _T("buptim") == sCommand )
				m_eInternalState = is_buptim;
			else if(  _T("yr") == sCommand  )
			{
				if( true == hasParameter )
				{
					if( is_creatim == m_eInternalState )
						oDocument.m_oInformation.m_oCreateTime.m_nYear = parameter;
					else if( is_revtim == m_eInternalState )
						oDocument.m_oInformation.m_oRevTime.m_nYear = parameter;
					else if( is_printim == m_eInternalState )
						oDocument.m_oInformation.m_oPrintTime.m_nYear = parameter;
					else if( is_buptim == m_eInternalState )
						oDocument.m_oInformation.m_oBackupTime.m_nYear = parameter;
				}
			}
			else if(  _T("mo") == sCommand  )
			{
				if( true == hasParameter )
				{
					if( is_creatim == m_eInternalState )
						oDocument.m_oInformation.m_oCreateTime.m_nMonth = parameter;
					else if( is_revtim == m_eInternalState )
						oDocument.m_oInformation.m_oRevTime.m_nMonth = parameter;
					else if( is_printim == m_eInternalState )
						oDocument.m_oInformation.m_oPrintTime.m_nMonth = parameter;
					else if( is_buptim == m_eInternalState )
						oDocument.m_oInformation.m_oBackupTime.m_nMonth = parameter;
				}
			}
			else if(  _T("dy") == sCommand  )
			{
				if( true == hasParameter )
				{
					if( is_creatim == m_eInternalState )
						oDocument.m_oInformation.m_oCreateTime.m_nDay = parameter;
					else if( is_revtim == m_eInternalState )
						oDocument.m_oInformation.m_oRevTime.m_nDay = parameter;
					else if( is_printim == m_eInternalState )
						oDocument.m_oInformation.m_oPrintTime.m_nDay = parameter;
					else if( is_buptim == m_eInternalState )
						oDocument.m_oInformation.m_oBackupTime.m_nDay = parameter;
				}
			}
			else if(  _T("hr") == sCommand  )
			{
				if( true == hasParameter )
				{
					if( is_creatim == m_eInternalState )
						oDocument.m_oInformation.m_oCreateTime.m_nHour = parameter;
					else if( is_revtim == m_eInternalState )
						oDocument.m_oInformation.m_oRevTime.m_nHour = parameter;
					else if( is_printim == m_eInternalState )
						oDocument.m_oInformation.m_oPrintTime.m_nHour = parameter;
					else if( is_buptim == m_eInternalState )
						oDocument.m_oInformation.m_oBackupTime.m_nHour = parameter;
				}
			}
			else if(  _T("min") == sCommand  )
			{
				if( true == hasParameter )
				{
					if( is_creatim == m_eInternalState )
						oDocument.m_oInformation.m_oCreateTime.m_nMin = parameter;
					else if( is_revtim == m_eInternalState )
						oDocument.m_oInformation.m_oRevTime.m_nMin = parameter;
					else if( is_printim == m_eInternalState )
						oDocument.m_oInformation.m_oPrintTime.m_nMin = parameter;
					else if( is_buptim == m_eInternalState )
						oDocument.m_oInformation.m_oBackupTime.m_nMin = parameter;
				}
			}
			else if(  _T("sec") == sCommand  )
			{
				if( true == hasParameter )
				{
					if( is_creatim == m_eInternalState )
						oDocument.m_oInformation.m_oCreateTime.m_nSecond = parameter;
					else if( is_revtim == m_eInternalState )
						oDocument.m_oInformation.m_oRevTime.m_nSecond = parameter;
					else if( is_printim == m_eInternalState )
						oDocument.m_oInformation.m_oPrintTime.m_nSecond = parameter;
					else if( is_buptim == m_eInternalState )
						oDocument.m_oInformation.m_oBackupTime.m_nSecond = parameter;
				}
			}
			else
			{
				return false;
			}
			return true;

		}
		void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, CString sText )
		{
			if( is_title == m_eInternalState )
				oDocument.m_oInformation.m_sTitle += sText;
			else if( is_subject == m_eInternalState )
				oDocument.m_oInformation.m_sSubject += sText;
			else if( is_author == m_eInternalState )
				oDocument.m_oInformation.m_sAuthor += sText;
			else if( is_manager == m_eInternalState )
				oDocument.m_oInformation.m_sManager += sText;
			else if( is_company == m_eInternalState )
				oDocument.m_oInformation.m_sCompany += sText;
			else if( is_operator == m_eInternalState )
				oDocument.m_oInformation.m_sOperator += sText;
			else if( is_category == m_eInternalState )
				oDocument.m_oInformation.m_sCategory += sText;
			else if( is_keywords == m_eInternalState )
				oDocument.m_oInformation.m_sKeywords += sText;
			else if( is_comment == m_eInternalState )
				oDocument.m_oInformation.m_sComment += sText;
			else if( is_doccomm == m_eInternalState )
				oDocument.m_oInformation.m_sDocCom += sText;
			else if( is_hlinkbase == m_eInternalState )
				oDocument.m_oInformation.m_sLinkBase += sText;
		}
};
class PictureReader:  public RtfAbstractReader
{
	class PLACEABLEMETAHEADER //заголовок для wmf из rtf ( в rtf wmf не содержит размеров картинки )
	{
	public:
		DWORD Key;           /* Magic number (always 9AC6CDD7h) */
		WORD  Handle;        /* Metafile HANDLE number (always 0) */
		SHORT Left;          /* Left coordinate in metafile units */
		SHORT Top;           /* Top coordinate in metafile units */
		SHORT Right;         /* Right coordinate in metafile units */
		SHORT Bottom;        /* Bottom coordinate in metafile units */
		WORD  Inch;          /* Number of metafile units per inch */
		DWORD Reserved;      /* Reserved (always 0) */
		WORD  Checksum;      /* Checksum value for previous 10 WORDs */
	public:
		PLACEABLEMETAHEADER()
		{
			Key = 0x9AC6CDD7;
			Handle = 0;
			Left = 0;
			Top = 0;
			Right = 0;
			Bottom = 0;
			Inch = 0x0902;
			Reserved = 0;
			Checksum = 0;
		}
		void CalculateChecksum()
		{
			Checksum = 0;
			Checksum ^= (Key & 0x0000FFFFUL);
			Checksum ^= ((Key & 0xFFFF0000UL) >> 16);
			Checksum ^= Handle; 
			Checksum ^= Left;
			Checksum ^= Top; 
			Checksum ^= Right;
			Checksum ^= Bottom; 
			Checksum ^= Inch;
			Checksum ^= (Reserved & 0x0000FFFFUL);
			Checksum ^= ((Reserved & 0xFFFF0000UL) >> 16);
		}
		CString ToString()
		{
			CString sResult;
			sResult.Append( ByteToString( (BYTE*)&Key, 4, true ) );
			sResult.Append( ByteToString( (BYTE*)&Handle, 2, true ) );
			sResult.Append( ByteToString( (BYTE*)&Left, 2, true ) );
			sResult.Append( ByteToString( (BYTE*)&Top, 2, true ) );
			sResult.Append( ByteToString( (BYTE*)&Right, 2, true ) );
			sResult.Append( ByteToString( (BYTE*)&Bottom, 2, true ) );
			sResult.Append( ByteToString( (BYTE*)&Inch, 2, true ) );
			sResult.Append( ByteToString( (BYTE*)&Reserved, 4, true ) );
			sResult.Append( ByteToString( (BYTE*)&Checksum, 2, true ) );
			return sResult;
		}
			CString ByteToString( BYTE* pbData, int nSize, bool bLittleEnd = true )
			 {
				CString sResult;
				if( true == bLittleEnd )
				{
					for( int i = 0; i < nSize; i++ )
					{
						BYTE byteVal = pbData[i];
						if( byteVal < 0x10 )
							sResult.AppendFormat( _T("0%x"), byteVal );
						else
							sResult.AppendFormat( _T("%x"), byteVal );
					}
				}
				else
				{
					for( int i = nSize - 1 ; i >= 0; i-- )
					{
						BYTE byteVal = pbData[i];
						if( byteVal < 0x10 )
							sResult.AppendFormat( _T("0%x"), byteVal );
						else
							sResult.AppendFormat( _T("%x"), byteVal );
					}
				}
				return sResult;
			 }
	};

private: RtfShape& m_oShape;
		CString m_sFile;
		CString m_sData;
		bool  m_bBin;
		byte* m_pbBin;
		int m_nBinLength;
public: PictureReader( RtfReader& oReader, RtfShape& oShape ):m_oShape(oShape)
		{
			m_bBin = false;
			m_pbBin = NULL;

			//m_oFileWriter = NULL;

			//m_sFile = Utils::CreateTempFile( oReader.m_sTempFolder );
			//try{
			//	RELEASEOBJECT( m_oFileWriter );
			//	BSTR bstrFilename = m_sFile.AllocSysString();
			//	m_oFileWriter = new NFileWriter::CBufferedFileWriter( bstrFilename );
			//	SysFreeString( bstrFilename );
			//}
			//catch(...)
			//	m_oFileWriter = NULL;
		}
		~PictureReader()
		{
			RELEASEARRAYOBJECTS(m_pbBin);
			//RELEASEOBJECT( m_oFileWriter );
		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter);
		void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
		{
			m_sData += sText;
		}
		void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
		{
			//if( NULL != m_oFileWriter )
			//{
			//	//есЃE ыD задаЃEтиЃEзадаем егЃEсаЃE
			//	if( RtfPicture::dt_none ==  m_oShape.m_oPicture->eDataType )
			//		RtfPicture::DataType eDataType = RtfPicture::GetPictureType( m_sFile );
			//	m_oShape.m_oPicture->m_bIsCopy = true;
			//	m_oShape.m_oPicture->m_sPicFilename = m_sFile;
			//}
			if( NULL != m_oShape.m_oPicture && RtfPicture::dt_wmf == m_oShape.m_oPicture->eDataType )
			{
				if( PROP_DEF != m_oShape.m_oPicture->m_nWidthGoal && PROP_DEF != (int)m_oShape.m_oPicture->m_dScaleX &&
					PROP_DEF != m_oShape.m_oPicture->m_nHeightGoal && PROP_DEF != (int)m_oShape.m_oPicture->m_dScaleY )
				{
					PLACEABLEMETAHEADER oPLACEABLEMETAHEADER;
					oPLACEABLEMETAHEADER.Right = long( (m_oShape.m_oPicture->m_dScaleX / 100.0) * m_oShape.m_oPicture->m_nWidthGoal * ( 96.0 / 1440 ) ); //to pixel
					oPLACEABLEMETAHEADER.Bottom = long( (m_oShape.m_oPicture->m_dScaleY / 100.0) * m_oShape.m_oPicture->m_nHeightGoal * ( 96.0 / 1440 ) );
					oPLACEABLEMETAHEADER.CalculateChecksum();
					m_sData.Insert( 0, oPLACEABLEMETAHEADER.ToString() );
				}
			}

			CString sTempFile = Utils::CreateTempFile( oReader.m_sTempFolder );
			if(m_bBin)
				RtfUtility::WriteDataToFileBinary( sTempFile, m_pbBin, m_nBinLength );
			else
				RtfUtility::WriteDataToFile( sTempFile, m_sData );
			if( RtfPicture::dt_none ==  m_oShape.m_oPicture->eDataType )
				RtfPicture::DataType eDataType = RtfPicture::GetPictureType( sTempFile );
			m_oShape.m_oPicture->m_bIsCopy = true;
			m_oShape.m_oPicture->m_sPicFilename = sTempFile;
		}
};
class OleReader:  public RtfAbstractReader
{
private: RtfOle& m_oOle;
public: OleReader(RtfOle& oOle):m_oOle(oOle)
		{
		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter);

};
class ShapeReader : public RtfAbstractReader
{
class ShapePropertyReader : public RtfAbstractReader
	{
	class ShapePropertyValueReader : public RtfAbstractReader
	{
		CString& m_sPropName;
		CString m_sPropValue;
		RtfShape& m_oShape;
	public: ShapePropertyValueReader(CString& sPropName, RtfShape& oShape):m_sPropName(sPropName),m_oShape(oShape)
			{
			}
			bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
			{
				if( _T("sv") == sCommand )
					return true;
				else if( _T("pict") == sCommand && _T("pib") == m_sPropName )
				{
					m_oShape.m_oPicture = RtfPicturePtr( new RtfPicture() );
					PictureReader oPictureReader( oReader, m_oShape );
					StartSubReader( oPictureReader, oDocument, oReader );
				}
				return true;
			}
			void PopState( RtfDocument& oDocument, RtfReader& oReader )
			{
				RtfAbstractReader::PopState( oDocument, oReader );
				if( _T("") == m_sPropValue ) 
					return;
				CString sValue = m_sPropValue;
				int nValue = Strings::ToInteger( sValue );

				if( _T("shapeType") == m_sPropName )
					m_oShape.m_nShapeType = nValue;
				//Position absolute
				else if( _T("posh") == m_sPropName )
					m_oShape.m_nPositionH = nValue;
				else if( _T("posrelh") == m_sPropName )
					m_oShape.m_nPositionHRelative = nValue;
				else if( _T("posv") == m_sPropName )
					m_oShape.m_nPositionV = nValue;
				else if( _T("posrelv") == m_sPropName )
					m_oShape.m_nPositionVRelative = nValue;
				else if( _T("fLayoutInCell") == m_sPropName )
					m_oShape.m_bLayoutInCell = nValue;
				else if( _T("fAllowOverlap") == m_sPropName )
					m_oShape.m_bAllowOverlap = nValue;
				//Position relative
				else if( _T("pctHorizPos") == m_sPropName )
					m_oShape.m_nPositionHPct = nValue;
				else if( _T("pctVertPos") == m_sPropName )
					m_oShape.m_nPositionVPct = nValue;
				else if( _T("pctHoriz") == m_sPropName )
					m_oShape.m_nPctWidth = nValue;
				else if( _T("pctVert") == m_sPropName )
					m_oShape.m_nPctHeight = nValue;
				else if( _T("sizerelh") == m_sPropName )
					m_oShape.m_nPctWidthRelative = nValue;
				else if( _T("sizerelv") == m_sPropName )
					m_oShape.m_nPctHeightRelative = nValue;
				else if( _T("colStart") == m_sPropName )
					m_oShape.m_nColStart = nValue;
				//Rehydration
				else if( _T("metroBlob") == m_sPropName )
					m_oShape.m_sMetroBlob = sValue;
				//Object Type
				else if( _T("fIsBullet") == m_sPropName )
					m_oShape.m_bIsBullet = nValue;
				else if( _T("rotation") == m_sPropName )
					m_oShape.m_nRotation = nValue;
				else if( _T("fFlipV") == m_sPropName )
					m_oShape.m_bFlipV = nValue;
				else if( _T("fFlipH") == m_sPropName )
					m_oShape.m_bFlipH = nValue;
				else if( _T("shapeType") == m_sPropName )
					m_oShape.m_nShapeType = nValue;
				else if( _T("pWrapPolygonVertices") == m_sPropName )
				{
					CString sWrapPair;
					int nPosition = 0;
					int nCount=0;
					while(true)
					{
						sWrapPair = sValue.Tokenize(_T(";"),nPosition);
						if( _T("") == sWrapPair )
							break;
						nCount++;
						if( nCount > 2 )
						{
							int nSubPos = 0;
							CString sWrapPoint = sWrapPair.Tokenize( _T(","), nSubPos);
							sWrapPoint.Remove('(');
							int nWrapPoint = Strings::ToInteger(sWrapPoint);
							m_oShape.m_aWrapPoints.push_back( nWrapPoint );
							sWrapPoint = sWrapPair.Tokenize( _T(","), nSubPos);
							sWrapPoint.Remove(')');
							nWrapPoint = Strings::ToInteger(sWrapPoint);
							m_oShape.m_aWrapPoints.push_back( nWrapPoint );
						}
					}

				}
				else if( _T("dxWrapDistLeft") == m_sPropName )
					m_oShape.m_nWrapDistLeft =  RtfUtility::Emu2Twips( nValue );
				else if( _T("dyWrapDistTop") == m_sPropName )
					m_oShape.m_nWrapDistTop = RtfUtility::Emu2Twips( nValue );
				else if( _T("dxWrapDistRight") == m_sPropName )
					m_oShape.m_nWrapDistRight = RtfUtility::Emu2Twips( nValue );
				else if( _T("dyWrapDistBottom") == m_sPropName )
					m_oShape.m_nWrapDistBottom = RtfUtility::Emu2Twips( nValue );
				else if( _T("fBehindDocument") == m_sPropName )
					m_oShape.m_nZOrderRelative = nValue;
				else if( _T("fHidden") == m_sPropName )
					m_oShape.m_bHidden = nValue;
				//Text box
				else if( _T("dxTextLeft") == m_sPropName )
					m_oShape.m_nTexpLeft = nValue;
				else if( _T("dyTextTop") == m_sPropName )
					m_oShape.m_nTexpTop = nValue;
				else if( _T("dxTextRight") == m_sPropName )
					m_oShape.m_nTexpRight = nValue;
				else if( _T("dyTextBottom") == m_sPropName )
					m_oShape.m_nTexpBottom = nValue;

				//Geometry
				else if( _T("adjustValue") == m_sPropName )
					m_oShape.m_nAdjustValue = nValue;
				else if( _T("adjust2Value") == m_sPropName )
					m_oShape.m_nAdjustValue2 = nValue;
				else if( _T("adjust3Value") == m_sPropName )
					m_oShape.m_nAdjustValue3 = nValue;
				else if( _T("adjust4Value") == m_sPropName )
					m_oShape.m_nAdjustValue4 = nValue;
				else if( _T("adjust5Value") == m_sPropName )
					m_oShape.m_nAdjustValue5 = nValue;
				else if( _T("adjust6Value") == m_sPropName )
					m_oShape.m_nAdjustValue6 = nValue;
				else if( _T("adjust7Value") == m_sPropName )
					m_oShape.m_nAdjustValue7 = nValue;
				else if( _T("adjust8Value") == m_sPropName )
					m_oShape.m_nAdjustValue8 = nValue;
				else if( _T("adjust9Value") == m_sPropName )
					m_oShape.m_nAdjustValue9 = nValue;
				else if( _T("adjust10Value") == m_sPropName )
					m_oShape.m_nAdjustValue10 = nValue;
				//WordArt Effects
				else if( _T("cropFromTop") == m_sPropName )
					m_oShape.m_nCropFromTop = nValue;
				else if( _T("cropFromBottom") == m_sPropName )
					m_oShape.m_nCropFromBottom = nValue;
				else if( _T("cropFromLeft") == m_sPropName )
					m_oShape.m_nCropFromRight = nValue;
				else if( _T("cropFromRight") == m_sPropName )
					m_oShape.m_nCropFromTop = nValue;
				//Grouped Shapes
				else if( _T("groupBottom") == m_sPropName )
					m_oShape.m_nGroupBottom = nValue;
				else if( _T("groupLeft") == m_sPropName )
					m_oShape.m_nGroupLeft = nValue;
				else if( _T("groupRight") == m_sPropName )
					m_oShape.m_nGroupRight = nValue;
				else if( _T("groupTop") == m_sPropName )
					m_oShape.m_nGroupTop = nValue;
				else if( _T("relBottom") == m_sPropName )
					m_oShape.m_nRelBottom = nValue;
				else if( _T("relLeft") == m_sPropName )
					m_oShape.m_nRelLeft = nValue;
				else if( _T("relRight") == m_sPropName )
					m_oShape.m_nRelRight = nValue;
				else if( _T("relTop") == m_sPropName )
					m_oShape.m_nRelTop = nValue;
				else if( _T("relRotation") == m_sPropName )
					m_oShape.m_nRelRotation = nValue;
				else if( _T("dhgt") == m_sPropName )
					m_oShape.m_nRelZOrder = nValue;
				//Fill
				else if( _T("fFilled") == m_sPropName )
				{
					if( 0 == nValue )
						m_oShape.m_bFilled = false;
					else
						m_oShape.m_bFilled = false;
				}
				//Line
				else if( _T("fLine") == m_sPropName )
				{
					if( 0 == nValue )
						m_oShape.m_bLine = false;
					else
						m_oShape.m_bLine = false;
				}
				else if( _T("lineStartArrowhead") == m_sPropName )
					m_oShape.m_nLineStartArrow = nValue;
				else if( _T("lineStartArrowWidth") == m_sPropName )
					m_oShape.m_nLineStartArrowWidth = nValue;
				else if( _T("lineStartArrowLength") == m_sPropName )
					m_oShape.m_nLineStartArrowLength = nValue;
				else if( _T("lineEndArrowhead") == m_sPropName )
					m_oShape.m_nLineEndArrow = nValue;
				else if( _T("lineEndArrowWidth") == m_sPropName )
					m_oShape.m_nLineEndArrowWidth = nValue;
				else if( _T("lineEndArrowLength") == m_sPropName )
					m_oShape.m_nLineEndArrowLength = nValue;
				else if( _T("lineWidth") == m_sPropName )
					m_oShape.m_nLineWidth = nValue;
			}
			void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, CString oText )
			{
				CString sValue;
				sValue = oText;
				m_sPropValue.Append( sValue );
			}
	};
	private: CString sPropName;
			RtfShape& m_oShape;
	public: ShapePropertyReader(RtfShape& oShape):m_oShape(oShape)
			{
			}
			bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
			{
				if( _T("sp") == sCommand )
					return true;
				else if( _T("sn") == sCommand )
				{
					TextReader oTextReader(sPropName);
					StartSubReader( oTextReader, oDocument, oReader );
				}
				else if( _T("sv") == sCommand )
				{
					ShapePropertyValueReader oShPropValReader(sPropName, m_oShape);
					StartSubReader( oShPropValReader, oDocument, oReader );
				}
				else
					return false;
				return true;
			}
	};
private: RtfShape& m_oShape;
public: ShapeReader( RtfShape& oShape ):m_oShape(oShape)
		{
		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter);
		void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
		{
			//если задан поворот, то надо повернуть и исходный rect, если угол от 45 до 135 и от 225 до 315
			if( (PROP_DEF != m_oShape.m_nRotation || PROP_DEF != m_oShape.m_nRelRotation) && 
					 (( PROP_DEF != m_oShape.m_nLeft && PROP_DEF != m_oShape.m_nTop &&
					PROP_DEF != m_oShape.m_nBottom && PROP_DEF != m_oShape.m_nRight ) ||
					( PROP_DEF != m_oShape.m_nRelLeft && PROP_DEF != m_oShape.m_nRelTop &&
					PROP_DEF != m_oShape.m_nRelRight && PROP_DEF != m_oShape.m_nRelBottom ) )
					)
			{
				int nAngel = 0; // в градусах
				if( PROP_DEF != m_oShape.m_nRotation )
					nAngel = m_oShape.m_nRotation / 65536;
				else
					nAngel = m_oShape.m_nRelRotation / 65536;
				int nSourceAngel = nAngel; // в градусах
				bool bRel = false;
				int nLeft;
				int nRight;
				int nTop;
				int nBottom;
				if( PROP_DEF != m_oShape.m_nLeft )
				{
					nLeft = m_oShape.m_nLeft;
					nRight = m_oShape.m_nRight;
					nTop = m_oShape.m_nTop;
					nBottom = m_oShape.m_nBottom;
				}
				else
				{
					bRel = true;
					nLeft = m_oShape.m_nRelLeft;
					nRight = m_oShape.m_nRelRight;
					nTop = m_oShape.m_nRelTop;
					nBottom = m_oShape.m_nRelBottom;
				}
				//поворачиваем на 45 градусов
				nAngel -= 45;
				//делаем угол от 0 до 360
				nAngel = nAngel % 360;
				if( nAngel < 0 )
					nAngel += 360;
				int nQuater = nAngel / 90; // определяем четверть
				if( 0 == nQuater || 2 == nQuater )
				{
					//поворачиваем относительно центра на 90 градусов обратно
					int nCenterX = ( nLeft + nRight ) / 2;
					int nCenterY = ( nTop + nBottom ) / 2;
					int nWidth = nRight - nLeft;
					int nHeight = nBottom - nTop;
					if( true == bRel )
					{
						m_oShape.m_nRelLeft = nCenterX - nHeight / 2;
						m_oShape.m_nRelRight = nCenterX + nHeight / 2;
						m_oShape.m_nRelTop = nCenterY - nWidth / 2;
						m_oShape.m_nRelBottom = nCenterY + nWidth / 2;
					}
					else
					{
						m_oShape.m_nLeft = nCenterX - nHeight / 2;
						m_oShape.m_nRight = nCenterX + nHeight / 2;
						m_oShape.m_nTop = nCenterY - nWidth / 2;
						m_oShape.m_nBottom = nCenterY + nWidth / 2;
					}
				}

			}
		}
};
class ShapeGroupReader : public ShapeReader
{
public: RtfShapeGroup& m_oShapeGroup;
		bool m_bHeader; //чтобы отличать заголовок от вложенных групп
		ShapeGroupReader( RtfShapeGroup& oShape ):ShapeReader(oShape),m_oShapeGroup(oShape)
		{
			m_bHeader = true;
		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("shpgrp") == sCommand )
			{
				if( true == m_bHeader )
					m_bHeader = false;
				else
				{
					RtfShapeGroupPtr oNewShape = RtfShapeGroupPtr( new RtfShapeGroup() );
					ShapeGroupReader oShapeGroupReader( *oNewShape );
					StartSubReader( oShapeGroupReader, oDocument, oReader );
					m_oShapeGroup.AddItem( oNewShape );
				}
			}
			else if( _T("shp") == sCommand )
			{
				RtfShapePtr oNewShape = RtfShapePtr( new RtfShape() );
				ShapeReader oShapeReader( *oNewShape );
				StartSubReader( oShapeReader, oDocument, oReader );
				m_oShapeGroup.AddItem( oNewShape );
			}
			else
				return ShapeReader::ExecuteCommand( oDocument,  oReader, sCommand, hasParameter, parameter);
			return true;
		}
};
class ShppictReader : public RtfAbstractReader
{
private: RtfShape& m_oShape;
public: ShppictReader( RtfShape& oShape ):m_oShape(oShape)
		{
		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("shppict") == sCommand )
				return true;
			else if( _T("pict") == sCommand )
			{
				m_oShape.m_eShapeType = RtfShape::st_inline;
				m_oShape.m_nShapeType = 75;
				m_oShape.m_nWrapType = 3; // none
				m_oShape.m_nPositionHRelative = 3;//TCHAR
				m_oShape.m_nPositionVRelative = 3;//line
				m_oShape.m_nPositionH = 0;//absolute
				m_oShape.m_nPositionV = 0;//absolute
				m_oShape.m_oPicture = RtfPicturePtr( new RtfPicture() );
				PictureReader oPictureReader( oReader, m_oShape);
				StartSubReader( oPictureReader, oDocument, oReader );
			}
			else
				return false;
			return true;
		}
};

class AllPictReader : public RtfAbstractReader {
private: RtfShape& m_oShape;
public: AllPictReader( RtfShape& oShape ):m_oShape(oShape)
		{
		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("shp") == sCommand )
			{
				ShapeReader oShapeReader(m_oShape);
				StartSubReader( oShapeReader, oDocument, oReader );
			}
			if( _T("shppict") == sCommand )
			{
				ShppictReader oShppictReader(m_oShape);
				StartSubReader( oShppictReader, oDocument, oReader );
			}
			else if( _T("pict") == sCommand )
			{
				m_oShape.m_eShapeType = RtfShape::st_inline;
				m_oShape.m_nShapeType = 75;
				m_oShape.m_nWrapType = 3; // none
				m_oShape.m_nPositionHRelative = 3;//TCHAR
				m_oShape.m_nPositionVRelative = 3;//line
				m_oShape.m_nPositionH = 0;//absolute
				m_oShape.m_nPositionV = 0;//absolute
				m_oShape.m_oPicture = RtfPicturePtr( new RtfPicture() );
				PictureReader oPictureReader( oReader, m_oShape);
				StartSubReader( oPictureReader, oDocument, oReader );
			}
			else
				return false;
			return true;
		}
};
class RtfMathPropReader : public RtfAbstractReader
{
public: RtfMathPropReader(  )
		{
		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("mmathPr") == sCommand )
				return true;
			else if( _T("mbrkBin") == sCommand )
			{
				if( true == hasParameter ) 
					oDocument.m_oMathProp.mbrkBin = parameter;
			}
			else if( _T("mbrkBinSub") == sCommand )
			{
				if( true == hasParameter ) 
					oDocument.m_oMathProp.mbrkBinSub = parameter;
			}
			else if( _T("mdefJc") == sCommand )
			{
				if( true == hasParameter ) 
					oDocument.m_oMathProp.mdefJc = parameter;
			}
			else if( _T("mdispDef") == sCommand )
			{
				if( true == hasParameter ) 
					oDocument.m_oMathProp.mdispDef = parameter;
			}
			else if( _T("minterSp") == sCommand )
			{
				if( true == hasParameter ) 
					oDocument.m_oMathProp.minterSp = parameter;
			}
			else if( _T("mintraSp") == sCommand )
			{
				if( true == hasParameter ) 
					oDocument.m_oMathProp.mintraSp = parameter;
			}
			else if( _T("mlMargin") == sCommand )
			{
				if( true == hasParameter ) 
					oDocument.m_oMathProp.mlMargin = parameter;
			}
			else if( _T("mmathFont") == sCommand )
			{
				if( true == hasParameter ) 
					oDocument.m_oMathProp.mmathFont = parameter;
			}
			else if( _T("mnaryLim") == sCommand )
			{
				if( true == hasParameter ) 
					oDocument.m_oMathProp.mnaryLim = parameter;
			}
			else if( _T("mpostSp") == sCommand )
			{
				if( true == hasParameter ) 
					oDocument.m_oMathProp.mpostSp = parameter;
			}
			else if( _T("mpreSp") == sCommand )
			{
				if( true == hasParameter ) 
					oDocument.m_oMathProp.mpreSp = parameter;
			}
			else if( _T("mrMargin") == sCommand )
			{
				if( true == hasParameter ) 
					oDocument.m_oMathProp.mrMargin = parameter;
			}
			else if( _T("msmallFrac") == sCommand )
			{
				if( true == hasParameter ) 
					oDocument.m_oMathProp.msmallFrac = parameter;
			}
			else if( _T("mwrapIndent") == sCommand )
			{
				if( true == hasParameter ) 
					oDocument.m_oMathProp.mwrapIndent = parameter;
			}
			else if( _T("mwrapRight") == sCommand )
			{
				if( true == hasParameter ) 
					oDocument.m_oMathProp.mwrapRight = parameter;
			}
			else
			{
				return false;
			}
			return true;
		}
};

class RtfMathReader: public RtfAbstractReader
{
private: RtfCharProperty m_oCharProp;
public: RtfMath& m_oMath;
		RtfParagraphProperty::ParagraphAlign m_eParAlign;
		RtfMathReader(RtfMath& oMath): m_oMath(oMath)
		{
			m_eParAlign = RtfParagraphProperty::pa_none;
			m_oCharProp.SetDefaultRtf();
		}

		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("mmath") == sCommand )
			{//ставим выравнивание параграфа по default свойствам Math
				if( PROP_DEF != oDocument.m_oMathProp.mdefJc )
				{
					switch( oDocument.m_oMathProp.mdefJc )
					{
						case 1: m_eParAlign = RtfParagraphProperty::pa_qc;break;
						case 2: m_eParAlign = RtfParagraphProperty::pa_qc;break;
						case 3: m_eParAlign = RtfParagraphProperty::pa_ql;break;
						case 4: m_eParAlign = RtfParagraphProperty::pa_qr;break;
					}
				}
			}
			else if( _T("mjc") == sCommand  )
			{
				if( true == hasParameter )
				{
					switch( parameter )
					{
						case 1: m_eParAlign = RtfParagraphProperty::pa_qc;break;
						case 2: m_eParAlign = RtfParagraphProperty::pa_qc;break;
						case 3: m_eParAlign = RtfParagraphProperty::pa_ql;break;
						case 4: m_eParAlign = RtfParagraphProperty::pa_qr;break;
					}
				}
			}
			else if( _T("mmathPict") == sCommand  )
				;
			else if( _T("shppict") == sCommand )
			{
				m_oMath.m_oPicture = RtfShapePtr( new RtfShape() );
				ShppictReader oShppictReader( *m_oMath.m_oPicture );
				StartSubReader( oShppictReader, oDocument, oReader );
			}
			else if( _T("nonshppict") == sCommand )
				Skip( oDocument, oReader );
			else 
			{
				if( RtfMath::IsRtfControlWord( sCommand ) == true )
				{
					if( m_oMath.IsEmpty() )
						m_oMath.SetRtfName( sCommand );
					else
					{
						RtfMathPtr oNewMath( new RtfMath() );
						oNewMath->SetRtfName( sCommand );
						RtfMathReader oSubMathReader( *oNewMath );
						StartSubReader( oSubMathReader, oDocument, oReader );
						if( RtfParagraphProperty::pa_none != oSubMathReader.m_eParAlign )
							m_eParAlign = oSubMathReader.m_eParAlign;
						if( oNewMath->IsValid() == true )
							m_oMath.AddItem( oNewMath );
					}
				}
				else if( RtfCharPropCommand::ExecuteCommand(oDocument, oReader, sCommand, hasParameter, parameter ) == true )
				{
					//MathItem * oNewItem = NULL;
					//m_oTempInformation.GetItemPointer(&oNewItem);
					//if( NULL != oNewItem )
					//{
					//	oNewItem->m_bCharChange = true;
					//}
				}
				else
					return false;

				return true;
			}

			return true;
		}
		void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
		{
			RtfCharPtr oNewChar( new RtfChar() );
			oNewChar->setText( sText ); 
			m_oMath.AddItem( oNewChar );
		}
		void ExitReader2( RtfDocument& oDocument, RtfReader& oReader )
		{
			if( RtfParagraphProperty::pa_none != m_eParAlign )
				oReader.m_oState->m_oParagraphProp.m_eAlign = m_eParAlign;
		}
};
class FieldReader: public RtfAbstractReader
{
private: typedef enum { is_normal, is_insert, is_datafield, is_formfield, is_result } InternalState;
		InternalState m_eInternalState;
		RtfField& m_oField;
public: FieldReader( RtfField& oField ):m_oField(oField)
		{
			m_eInternalState = is_normal;
		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter);
		void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
		{
			//если результат пустой пытаемся его сгенерировать
			if( true == IsEmptyText( oDocument ) )
				TryToPepairResult( oDocument, oReader );
		}
		bool IsEmptyText( RtfDocument& oDocument )
		{
			if( NULL != m_oField.m_oResult )
			{
				OOXWriter oTempWriter( oDocument, _T("") );
				OOXRelsWriter oTempRelsWriter( _T(""), oDocument );
				RenderParameter oRenderParameter;
				oRenderParameter.poDocument = &oDocument;
				oRenderParameter.poRels = &oTempRelsWriter;
				oRenderParameter.poWriter = &oTempWriter;
				oRenderParameter.nType = RENDER_TO_OOX_PARAM_PLAIN;

				CString sResult = m_oField.m_oResult->RenderToOOX( oRenderParameter );
				if( _T("") != sResult )
					return false;
			}
			return true;
		}
private: void TryToPepairResult( RtfDocument& oDocument, RtfReader& oReader )
		 {
			OOXWriter oTempWriter( oDocument, _T("") );
			OOXRelsWriter oTempRelsWriter( _T(""), oDocument );

			RenderParameter oNewParametr;
			oNewParametr.poDocument = &oDocument;
			oNewParametr.poRels = &oTempRelsWriter;
			oNewParametr.poWriter = &oTempWriter;
			oNewParametr.nType = RENDER_TO_OOX_PARAM_PLAIN;

			CString sCharCode;
			CString sCharFont;

			CString sField = m_oField.m_oInsert->RenderToOOX(oNewParametr);
			int nStartTokenize = 0;
			CString sResTokenize = sField.Tokenize( _T(" "), nStartTokenize );
			if( _T("SYMBOL") != sResTokenize )
				return;
			sResTokenize = sField.Tokenize( _T(" \""), nStartTokenize );
			int nCommand = 0; //0 - none; 1 - \f; 3 - other
			bool bWaitRightBreak = false;
			while( sResTokenize != "" )
			{
				int nTokenLen = sResTokenize.GetLength();
				if( nTokenLen > 0 && sResTokenize[0] == '\"' && sResTokenize[nTokenLen - 1] != '\"' ) //текст в кавычках считается как один
				{
					//ищем следующую кавычку
					int nNextQuot = sField.Find( '\"', nStartTokenize );
					if( -1 != nNextQuot )
					{
						sResTokenize = sField.Mid( nStartTokenize - nTokenLen, nNextQuot - nStartTokenize + nTokenLen );
						nStartTokenize = nNextQuot + 1;
					}
				}
				if( nTokenLen > 0 && sResTokenize[0] == '\"' && sResTokenize[nTokenLen - 1] == '\"' )
					sResTokenize = sResTokenize.Mid( 1, nTokenLen - 2 );
				

				if( -1 != sResTokenize.Find( _T("\\f") ) )
					nCommand = 1;
				else if( -1 != sResTokenize.Find( _T("\\") ) )
					nCommand = 3;
				else if( 1 == nCommand )
				{
					sCharFont = sResTokenize;
					nCommand = 0;
				}
				else if( 0 == nCommand && _T("") == sCharCode )
				{
					sCharCode = sResTokenize;
				}
				else
					nCommand = 0;
				sResTokenize = sField.Tokenize( _T(" "), nStartTokenize );
			}
			if( _T("") == sCharCode || _T("") == sCharFont )
				return;
			
			int nCharCode = Strings::ToInteger( sCharCode );
			CStringA sCharA; sCharA.AppendChar( nCharCode );

			RtfFont oSymbolFont;
			if( true == oDocument.m_oFontTable.GetFont( sCharFont, oSymbolFont ) )
				oReader.m_oState->m_oCharProp.m_nFont = oSymbolFont.m_nID;
			else
			{
				oSymbolFont.m_sName = sCharFont;
				oSymbolFont.m_nID = oDocument.m_oFontTable.GetCount() + 1;
				oSymbolFont.m_nCodePage = CP_SYMBOL;
				oReader.m_oState->m_oCharProp.m_nFont = oSymbolFont.m_nID;
				oDocument.m_oFontTable.DirectAddItem( oSymbolFont );
			}

			int nSkipChar = 0;
			CString sResultSymbol = RtfAbstractReader::ExecuteTextInternal( oDocument, oReader, sCharA, false, 0, nSkipChar );
			m_oField.m_oResult = TextItemContainerPtr( new TextItemContainer() );
			RtfParagraphPtr oNewPar	= RtfParagraphPtr( new RtfParagraph() );
			RtfCharPtr oNewChar = RtfCharPtr( new RtfChar() );
			oNewChar->setText( sResultSymbol );
			oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;

			oNewPar->AddItem( oNewChar );
			m_oField.m_oResult->AddItem( oNewPar );
			m_oField.m_bTextOnly = true;
		 }
};
class BookmarkStartReader: public RtfAbstractReader
{
public: RtfBookmarkStart& m_oBookmarkStart;
		BookmarkStartReader( RtfBookmarkStart& oBookmark ):m_oBookmarkStart(oBookmark)
		{
		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("bkmkstart") == sCommand )
				;
			else if( _T("bkmkcolf") == sCommand )
			{
				if( true == hasParameter)
					m_oBookmarkStart.nFirstColumn = parameter;
			}
			else if( _T("bkmkcoll") == sCommand )
			{
				if( true == hasParameter)
					m_oBookmarkStart.nLastColumn = parameter;
			}
			else
				return false;
			return true;
		}
		void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
		{
			m_oBookmarkStart.m_sName.Append( sText );
		}

};
class BookmarkEndReader: public RtfAbstractReader
{
public: RtfBookmarkEnd& m_oBookmarkEnd;
		BookmarkEndReader( RtfBookmarkEnd& oBookmark ):m_oBookmarkEnd(oBookmark)
		{
		}
		void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
		{
			m_oBookmarkEnd.m_sName.Append( sText );
		}
};
class FootnoteReader;
//Destination имеет состояния
class ParagraphPropDestination // todo - последний параграф не обрабатывается
{
public:
	typedef enum{ is_normal,is_charBorder, is_borderTop, is_borderLeft, is_borderBottom, is_borderRight, is_borderBox, is_borderBar,
		is_borderCellLeft,is_borderCellTop,is_borderCellRight,is_borderCellBottom,is_borderCellLR,is_borderCellRL,
		is_borderRowLeft,is_borderRowTop,is_borderRowRight,is_borderRowBottom,is_borderRowVer,is_borderRowHor} InternalState;
//только для определения бордера
	
private: 
	InternalState m_eInternalState;
//		ItemContainer< RtfReaderParagraphPtr > m_aArray;
	RtfParagraphPtr m_oCurParagraph;

//реальные параграфы и таблицы
	std::vector< ITextItemPtr > aCellRenderables;
	std::vector< int > aItaps; //вложенность параграфов
	std::vector< RtfTableCellPtr > aCells;
	std::vector< int > aCellItaps; //вложенность cell
	std::vector< RtfTableRowPtr > aRows;
	std::vector< int > aRowItaps; //вложенность row
	int nCurItap;
	RtfRowProperty oCurRowProperty;

	RtfReader* m_oReader;
	bool m_bPar;// если последняя команда была par, то не надо добавлять параграф
public: 
	TextItemContainerPtr m_oTextItems;	//для разбивки на TextItem
	int nTargetItap;		//уровень который считается не таблицей ( для того чтобы читать параграфы в таблицах )
	RtfTab m_oCurTab;

	ParagraphPropDestination( )
	{
		nTargetItap = PROP_DEF;
		m_bPar = false;
		m_oTextItems = TextItemContainerPtr( new TextItemContainer() );
		nCurItap = 0;//main document
		m_eInternalState = is_normal;
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,RtfAbstractReader& oAbstrReader,CString sCommand, bool hasParameter, int parameter);
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
	{
		m_bPar = false;
		RtfCharPtr oNewChar( new RtfChar() );
		oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
		oNewChar->setText( sText );
		m_oCurParagraph->AddItem( oNewChar );
	}
	void AddItem( RtfParagraphPtr oItem, RtfReader& oReader, bool bEndCell, bool bEndRow );
	void Finalize( RtfReader& oReader ) // потому что это не reader и нужно как-то загонять последний параграф
	{
		if( false == m_bPar )
		{
			m_oCurParagraph->m_oProperty = oReader.m_oState->m_oParagraphProp;
			m_oCurParagraph->m_oOldList = RtfOldListPtr( new RtfOldList() );
			*m_oCurParagraph->m_oOldList = oReader.m_oState->m_oCurOldList;
			m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
			AddItem( m_oCurParagraph, oReader, false, false );
			m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());
		}
	}
	void ExecuteNumberChar( RtfDocument& oDocument, RtfReader& oReader, RtfAbstractReader& oAbstrReader, int nWinChar, int nMacChar )
	 {
		 RtfFont oFont;
		 if( true == oDocument.m_oFontTable.GetFont( oReader.m_oState->m_oCharProp.m_nFont, oFont ) )
		 {
			 bool bIsMac = false;
			 if( PROP_DEF != oFont.m_nCharset )
			 {
				 if( true == RtfUtility::IsMacCharset(oFont.m_nCharset) )
					 bIsMac = true;
			 }
			 else if( PROP_DEF != oFont.m_nCodePage )
			 {
				 if( true == RtfUtility::IsMacCodepage(oFont.m_nCodePage) )
					 bIsMac = true;
			 }
			 CStringA sBullet;
			 if( true == bIsMac )
				 sBullet.AppendChar( nMacChar );
			 else
				 sBullet.AppendChar( nWinChar );
			 int nSkip = 0;
			 CString sText = RtfAbstractReader::ExecuteTextInternal( oDocument, oReader, sBullet, false, 0, nSkip );
			 ExecuteText( oDocument, oReader, sText );
		 }
	 }
	//void ParseToRenderable( std::vector< boost::shared_ptr<ITextItem> >& aOutArray )
	//{
	//	std::vector< ITextItemPtr > aCellRenderables;
	//	std::vector< int > aItaps;
	//	std::vector< RtfTableCellPtr > aCells;
	//	std::vector< RtfTableRowPtr > aRows;

	//	int nCurItap = 0;//main document
	//	RtfRowProperty oCurRowProperty;
	//	//убрал -1 для последнего параграфа
	//	//todo подумать
	//	for( int i = 0; i < (int)m_aArray.size() - 1 ; i++ )
	//	{
	//		if( m_aArray[i]->m_oProperty.m_bInTable == false )
	//		{
	//			RtfParagraphPtr oNewParagraph( new RtfParagraph() );
	//			oNewParagraph->m_oProperty = m_aArray[i]->m_oProperty;
	//			for( int j = 0 ; j < (int)m_aArray[i]->GetCount(); j++ )
	//			{
	//				IDocumentElementPtr oCurElement;
	//				m_aArray[i]->GetItem( oCurElement, j ); 
	//				oNewParagraph->AddItem( oCurElement );
	//			}

	//			if( nCurItap > 0 ) //todo  ==1
	//			{
	//				RtfTablePtr oNewTable( new RtfTable() );
	//				oNewTable->m_oProperty = oCurRowProperty;
	//				for( int i = 0; i < (int)aRows.size(); i++ )
	//					oNewTable->AddItem( aRows[i] );
	//				oNewTable->CalculateGridProp();
	//				aRows.clear();
	//				aOutArray.push_back( oNewTable );
	//			}
	//			aOutArray.push_back(  oNewParagraph );
	//			nCurItap = m_aArray[i]->m_oProperty.m_nItap;
	//		}
	//		else
	//		{
	//			if( true == m_aArray[i]->m_bEndRow )
	//			{
	//				RtfTableRowPtr oNewTableRow( new RtfTableRow() );
	//				oNewTableRow->m_oProperty = m_aArray[i]->m_oProperty.m_oRowProperty;
	//				for( int i = 0; i < (int)aCells.size(); i++ )
	//					oNewTableRow->AddItem( aCells[i] );
	//				aCells.clear();
	//				aRows.push_back( oNewTableRow );
	//			}
	//			else
	//			{
	//				RtfParagraphPtr oTempPar( new RtfParagraph() );
	//				oTempPar->m_oProperty = m_aArray[i]->m_oProperty;
	//				for( int j = 0 ; j < (int)m_aArray[i]->GetCount(); j++ )
	//				{
	//					IDocumentElementPtr oCurElement;
	//					m_aArray[i]->GetItem( oCurElement, j ); 
	//					oTempPar->AddItem( oCurElement );
	//				}

	//				if( nCurItap > oTempPar->m_oProperty.m_nItap )
	//				{
	//					RtfTablePtr oNewTable( new RtfTable() );
	//					oNewTable->m_oProperty = oCurRowProperty;
	//					for( int k = 0; k < (int)aRows.size(); k++ )
	//						oNewTable->AddItem( aRows[k] );
	//					oNewTable->CalculateGridProp();
	//					aRows.clear();
	//					aCellRenderables.push_back( oNewTable );
	//					aItaps.push_back( oTempPar->m_oProperty.m_nItap );
	//				}
	//				nCurItap = oTempPar->m_oProperty.m_nItap;

	//				aCellRenderables.push_back( oTempPar );
	//				aItaps.push_back( nCurItap );


	//				if( true == m_aArray[i]->m_bEndCell )
	//				{
	//					RtfTableCellPtr oNewTableCell( new RtfTableCell() );
	//					for( int k = (int)aCellRenderables.size() - 1; k >= 0 ; k-- )
	//						if( aItaps[k] == nCurItap )
	//						{
	//							oNewTableCell->InsertItem(aCellRenderables[k], 0 );
	//							aCellRenderables.RemoveAt(k);
	//							aItaps.RemoveAt(k);
	//						}
	//						else
	//							break;
	//					aCells.push_back( oNewTableCell );
	//				}
	//			}
	//			oCurRowProperty = m_aArray[i]->m_oProperty.m_oRowProperty;
	//		}
	//			
	//	}
	//}

};
class FootnoteReader: public RtfAbstractReader
{
private: 
	ParagraphPropDestination m_oParPropDest;
public: 
	RtfFootnote& m_oRtfFootnote;
	FootnoteReader( RtfFootnote& oRtfFootnote ):m_oRtfFootnote(oRtfFootnote)
	{
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
	{
		if( _T("footnote") == sCommand )
		{
			oReader.m_nFootnote = 1;
		}
		else if( _T("ftnalt") == sCommand )
		{
			m_oRtfFootnote.m_bEndNote = true;
			oReader.m_nFootnote = 2;
		}
		else
			return m_oParPropDest.ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter );
		return true;
	}
	void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, CString sText )
	{
		m_oParPropDest.ExecuteText( oDocument, oReader, sText );
	}
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
	{
		m_oParPropDest.Finalize( oReader );
		m_oRtfFootnote.m_oContent = m_oParPropDest.m_oTextItems;
		oReader.m_nFootnote = PROP_DEF;
	}
};
//class RtfDefParPropReader: public RtfAbstractReader
//{
//private: ParagraphPropDestination m_oParPropDest;
//public: RtfDefParPropReader( )
//		{
//		}
//
//		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
//		{
//			if( _T("defpap") == sCommand )
//				return true;
//			else
//				return m_oParPropDest.ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter);
//		}
//		void ExitReader(RtfDocument& oDocument, RtfReader& oReader)
//		{
//			oDocument.m_oDefaultParagraphProp = oReader.m_oState->m_oParagraphProp;
//		}
//};

//class LockedExeceptReader: public RtfAbstractReader
//{
//private: RtfStyleException m_oCurException;
//public: bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
//		{
//			if( _T("lsdlockedexcept") == sCommand )
//				return true;
//			else if( _T("lsdpriority") == sCommand )
//			{
//				if( true == hasParameter )
//					m_oCurException.m_nPriority = parameter;
//			}
//			else if( _T("lsdunhideused") == sCommand )
//			{
//				if( true == hasParameter )
//					m_oCurException.m_nHiddenWhenUse = parameter;
//			}
//			else if( _T("lsdqformat") == sCommand )
//			{
//				if( true == hasParameter )
//					m_oCurException.m_nQFormat = parameter;
//			}
//			else if( _T("lsdlocked") == sCommand )
//			{
//				if( true == hasParameter )
//					m_oCurException.m_nLocked = parameter;
//			}
//			else if( _T("lsdsemihidden") == sCommand )
//			{
//				if( true == hasParameter )
//					m_oCurException.m_nSemiHidden = parameter;
//			}
//			else
//			{
//				return false;
//			}
//			return true;
//		}
//		void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, CString sText )
//		{
//			if( sText.Find(';') != -1 )
//			{
//				sText.Remove(';');
//				m_oCurException.m_sName += sText;
//				oDocument.m_oLatentStyleTable.AddItem( m_oCurException );
//				m_oCurException.SetDefaultRtf();
//			}
//			else
//			{
//				m_oCurException.m_sName += sText;
//			}
//		}
//};

//class LatentstyleTableReader: public RtfAbstractReader
//{
//public: bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
//		{
//			if( _T("latentstyles") == sCommand )
//				return true;
//			else if( _T("lsdstimax") == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nCount = parameter;
//			}
//			else if( _T("lsdlockeddef") == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nLocked = parameter;
//			}
//			else if( _T("lsdsemihiddendef") == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nSemiHidden = parameter;
//			}
//			else if( _T("lsdunhideuseddef") == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nUnHiddenWhenUse = parameter;
//			}
//			else if( _T("lsdqformatdef") == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nQFormat = parameter;
//			}
//			else if( _T("lsdprioritydef") == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nPriority = parameter;
//			}
//			else if( _T("lsdlockedexcept") == sCommand )
//			{
//				LockedExeceptReader oExceptionReader;
//				StartSubReader( oExceptionReader, oDocument, oReader );
//			}
//			else
//			{
//				return false;
//			}
//			return true;
//
//		}
//};
//

//class StyleTableReader: public RtfAbstractReader
//{
//class RtfStyleReader: public RtfAbstractReader
//{
////только для определения бордеров
//private: typedef enum { is_normal, is_tsbrdrt, is_tsbrdrb, is_tsbrdrl, is_tsbrdrr, is_tsbrdrh, is_tsbrdrv } InternalState;
//		InternalState m_eInternalState;
//		ParagraphPropDestination m_oParDest;
//		RtfStylePtr m_oCurStyle;
//
////		RtfTableStyleProperty m_oTableStyleProperty;
//
//public: RtfStyleReader()
//		{
//			m_oCurStyle = RtfParagraphStylePtr( new RtfParagraphStyle() );
//			m_eInternalState = is_normal;
//		}
//		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
//		{
//			if( _T("s") == sCommand )
//			{
//				if( true == hasParameter )
//				{
//					m_oCurStyle = RtfParagraphStylePtr( new RtfParagraphStyle() );
//					m_oCurStyle->m_nID = parameter;
//				}
//			}
//			else if( _T("cs") == sCommand )
//			{
//				if( true == hasParameter )
//				{
//					m_oCurStyle = RtfCharStylePtr( new RtfCharStyle() );
//					m_oCurStyle->m_nID = parameter;
//				}
//			}
//			else if( _T("ts") == sCommand )
//			{
//				if( true == hasParameter )
//				{
//					m_oCurStyle = RtfTableStylePtr( new RtfTableStyle() );
//					m_oCurStyle->m_nID = parameter;
//				}
//			}
//			COMMAND_RTF_INT( _T("sbasedon"), m_oCurStyle->m_nBasedOn, sCommand, hasParameter, parameter )
//			COMMAND_RTF_INT( _T("snext"), m_oCurStyle->m_nNext, sCommand, hasParameter, parameter )
//			COMMAND_RTF_INT( _T("slink"), m_oCurStyle->m_nLink, sCommand, hasParameter, parameter )
//			COMMAND_RTF_BOOL( _T("sqformat"), m_oCurStyle->m_bQFormat, sCommand, hasParameter, parameter )
//			COMMAND_RTF_INT( _T("spriority"), m_oCurStyle->m_nPriority, sCommand, hasParameter, parameter )
//			COMMAND_RTF_BOOL( _T("sunhideused"), m_oCurStyle->m_bUnhiddenWhenUse, sCommand, hasParameter, parameter )
//			COMMAND_RTF_BOOL( _T("slocked"), m_oCurStyle->m_bLocked, sCommand, hasParameter, parameter )
//			COMMAND_RTF_BOOL( _T("shidden"), m_oCurStyle->m_bHidden, sCommand, hasParameter, parameter )
//			else if( _T("ssemihidden") == sCommand )
//			{
//				if( true == hasParameter && 0 == parameter)
//					m_oCurStyle->m_nSemiHidden = 0;
//				else
//					m_oCurStyle->m_nSemiHidden = 1;
//			}
//			COMMAND_RTF_BOOL( _T("spersonal"), m_oCurStyle->m_bPersonal, sCommand, hasParameter, parameter )
//			COMMAND_RTF_BOOL( _T("scompose"), m_oCurStyle->m_bCompose, sCommand, hasParameter, parameter )
//			COMMAND_RTF_BOOL( _T("sreply"), m_oCurStyle->m_bReply, sCommand, hasParameter, parameter )
//
//			//tableStyleCommands
//			//else if( _T("tscellpaddt") == sCommand )
//			//{
//			//	if( true == hasParameter )
//			//		m_oTableStyleProperty.m_nPaddingTop = parameter;
//			//}
//			//else if( _T("tscellpaddl") == sCommand )
//			//{
//			//	if( true == hasParameter )
//			//		m_oTableStyleProperty.m_nPaddingLeft = parameter;
//			//}
//			//else if( _T("tscellpaddr") == sCommand )
//			//{
//			//	if( true == hasParameter )
//			//		m_oTableStyleProperty.m_nPaddingRight = parameter;
//			//}
//			//else if( _T("tscellpaddb") == sCommand )
//			//{
//			//	if( true == hasParameter )
//			//		m_oTableStyleProperty.m_nPaddingBottom = parameter;
//			//}
//			//else if( _T("tscellpaddft") == sCommand )
//			//{
//			//	if( true == hasParameter )
//			//		m_oTableStyleProperty.m_nIsPaddingTop = parameter;
//			//}
//			//else if( _T("tscellpaddfl") == sCommand )
//			//{
//			//	if( true == hasParameter )
//			//		m_oTableStyleProperty.m_nIsPaddingLeft = parameter;
//			//}
//			//else if( _T("tscellpaddfr") == sCommand )
//			//{
//			//	if( true == hasParameter )
//			//		m_oTableStyleProperty.m_nIsPaddingRight = parameter;
//			//}
//			//else if( _T("tscellpaddfb") == sCommand )
//			//{
//			//	if( true == hasParameter )
//			//		m_oTableStyleProperty.m_nIsPaddingBottom = parameter;
//			//}
//			//else if( _T("tsvertalt") == sCommand )
//			//	m_oTableStyleProperty.m_eAlign = RtfTableStyleProperty::av_tsvertalt;
//			//else if( _T("tsvertalc") == sCommand )
//			//	m_oTableStyleProperty.m_eAlign = RtfTableStyleProperty::av_tsvertalc;
//			//else if( _T("tsvertalb") == sCommand )
//			//	m_oTableStyleProperty.m_eAlign = RtfTableStyleProperty::av_tsvertalb;
//			//else if( _T("tsnowrap") == sCommand )
//			//	m_oTableStyleProperty.m_bNoCellWrap = 1;
//			//else if( _T("tscbandsh") == sCommand )
//			//{
//			//	if( true == hasParameter )
//			//		m_oTableStyleProperty.m_nRowBandNumber = parameter;
//			//}
//			//else if( _T("tscbandsv") == sCommand )
//			//{
//			//	if( true == hasParameter )
//			//		m_oTableStyleProperty.m_nCellBandNumber = parameter;
//			//}
//			//else if(_T("tsbghoriz") == sCommand )
//			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbghoriz;
//			//else if( _T("tsbgvert") == sCommand )
//			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgvert;
//			//else if( _T("tsbgfdiag") == sCommand )
//			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgfdiag;
//			//else if( _T("tsbgbdiag") == sCommand )
//			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgbdiag;
//			//else if( _T("tsbgcross") == sCommand )
//			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgcross;
//			//else if( _T("tsbgdcross") == sCommand )
//			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdcross;
//			//else if( _T("tsbgdkhor") == sCommand )
//			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkhoriz;
//			//else if( _T("tsbgdkvert") == sCommand )
//			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkvert;
//			//else if( _T("tsbgdkfdiag") == sCommand )
//			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkfdiag;
//			//else if( _T("tsbgdkbdiag") == sCommand )
//			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkbdiag;
//			//else if( _T("tsbgdkcross") == sCommand )
//			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkcross;
//			//else if( _T("tsbgdkdcross") == sCommand )
//			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkdcross;
//			//else if( _T("tscellcfpat") == sCommand )
//			//{
//			//	if( true == hasParameter )
//			//		m_oTableStyleProperty.m_oShading.m_nForeColor = parameter;
//			//}
//			//else if( _T("tscellcbpat") == sCommand )
//			//{
//			//	if( true == hasParameter )
//			//		m_oTableStyleProperty.m_oShading.m_nBackColor = parameter;
//			//}
//			//else if( _T("tscellpct") == sCommand )
//			//{
//			//	if( true == hasParameter )
//			//		m_oTableStyleProperty.m_oShading.m_nValue = parameter;
//			//}
//			//else if( _T("tsbrdrt") == sCommand )
//			//	m_eInternalState = is_tsbrdrt;
//			//else if( _T("tsbrdrb") == sCommand )
//			//	m_eInternalState = is_tsbrdrb;
//			//else if( _T("tsbrdrl") == sCommand )
//			//	m_eInternalState = is_tsbrdrl;
//			//else if( _T("tsbrdrr") == sCommand )
//			//	m_eInternalState = is_tsbrdrr;
//			//else if( _T("tsbrdrh") == sCommand )
//			//	m_eInternalState = is_tsbrdrh;
//			//else if( _T("tsbrdrv") == sCommand )
//			//	m_eInternalState = is_tsbrdrv;
//			//else if( _T("tscbandsh") == sCommand )
//			//{
//			//	if( true == hasParameter )
//			//		m_oTableStyleProperty.m_nRowBandNumber = parameter;
//			//}
//			//else if( _T("tscbandsv") == sCommand )
//			//{
//			//	if( true == hasParameter )
//			//		m_oTableStyleProperty.m_nCellBandNumber = parameter;
//			//}
//			else
//			{
//				bool bResult = false;
//				//if( is_tsbrdrt == m_eInternalState )
//				//	bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_oTableStyleProperty.m_oCellTopBorder );
//				//else if( is_tsbrdrb == m_eInternalState )
//				//	bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_oTableStyleProperty.m_oCellBottomBorder );
//				//else if( is_tsbrdrl == m_eInternalState )
//				//	bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_oTableStyleProperty.m_oCellLeftBorder );
//				//else if( is_tsbrdrr == m_eInternalState )
//				//	bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_oTableStyleProperty.m_oCellRightBorder );
//				//else if( is_tsbrdrh == m_eInternalState )
//				//	bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_oTableStyleProperty.m_oCellHorBorder );
//				//else if( is_tsbrdrv == m_eInternalState )
//				//	bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_oTableStyleProperty.m_oCellVerBorder );
//
//				//if( true == bResult )
//				//	return true;		
//				bResult = m_oParDest.ExecuteCommand( oDocument, oReader,(*this),sCommand, hasParameter, parameter );
//				if( true == bResult )
//					return true;
//				bResult = RtfCharPropCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCharProp );
//				if( true == bResult )
//					return true;
//
//				return false;
//			}
//			return true;
//
//		}
//
//		void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
//		{
//			if( sText.Find(';') != -1 )
//				sText.Remove(';');
//			m_oCurStyle->m_sName += sText;
//		}
//		void ExitReader(RtfDocument& oDocument, RtfReader& oReader)
//		{
//			switch( m_oCurStyle->m_eType )
//			{
//				case RtfStyle::stCharacter: 
//					{
//						RtfCharStylePtr m_oCurCharStyle = boost::shared_static_cast< RtfCharStyle, RtfStyle >( m_oCurStyle );
//						m_oCurCharStyle->m_oCharProp = oReader.m_oState->m_oCharProp;
//						oDocument.m_oStyleTable.AddItem( m_oCurStyle );
//						break;
//					}
//				case RtfStyle::stParagraph: 
//					{
//						RtfParagraphStylePtr m_oCurParStyle = boost::shared_static_cast< RtfParagraphStyle, RtfStyle >( m_oCurStyle );
//						m_oCurParStyle->m_oCharProp = oReader.m_oState->m_oCharProp;
//						m_oCurParStyle->m_oParProp = oReader.m_oState->m_oParagraphProp;
//						oDocument.m_oStyleTable.AddItem( m_oCurStyle );
//						break;
//					}
//				case RtfStyle::stTable: 
//					{
//						RtfTableStylePtr m_oCurTableStyle = boost::shared_static_cast< RtfTableStyle, RtfStyle >( m_oCurStyle );
//						m_oCurTableStyle->m_oCharProp = oReader.m_oState->m_oCharProp;
//						m_oCurTableStyle->m_oParProp = oReader.m_oState->m_oParagraphProp;
//						if( PROP_DEF == m_oCurTableStyle->m_oParProp.m_nSpaceBetween )
//							m_oCurTableStyle->m_oParProp.m_nSpaceBetween = 240;//интервал - единичный
//						m_oCurTableStyle->m_oTableProp = oReader.m_oState->m_oRowProperty;
//						m_oCurTableStyle->m_oRowProp = oReader.m_oState->m_oRowProperty;
//						//m_oCurTableStyle->m_oTableStyleProperty = m_oTableStyleProperty;
//						//надо определить базовый это стиль или например firstRow
//						RtfStylePtr oStyle;
//						if( true == oDocument.m_oStyleTable.GetStyle(m_oCurTableStyle->m_nID, oStyle) )
//						{
//							if( oStyle->m_eType == RtfStyle::stTable )
//							{//определяем какой это conditionalFormating
//								RtfTableStylePtr oStyleTable = boost::shared_static_cast<RtfTableStyle, RtfStyle>(oStyle);
//								if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleFirstRow )
//									oStyleTable->m_oFirstRow = m_oCurTableStyle;
//								else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleLastRow )
//									oStyleTable->m_oLastRow = m_oCurTableStyle;
//								else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleFirstCollumn )
//									oStyleTable->m_oFirstCol = m_oCurTableStyle;
//								else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleLastCollumn )
//									oStyleTable->m_oLastCol = m_oCurTableStyle;
//								else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleOddRowBand )
//									oStyleTable->m_oBandHorOdd = m_oCurTableStyle;
//								else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleEvenRowBand )
//									oStyleTable->m_oBandHorEven = m_oCurTableStyle;
//								else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleOddColBand )
//									oStyleTable->m_oBandVerOdd = m_oCurTableStyle;
//								else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleEvenColBand )
//									oStyleTable->m_oBandVerEven = m_oCurTableStyle;
//								else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleNWCell )
//									oStyleTable->m_oNWCell = m_oCurTableStyle;
//								else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleNECell )
//									oStyleTable->m_oNECell = m_oCurTableStyle;
//								else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleSWCell )
//									oStyleTable->m_oSWCell = m_oCurTableStyle;
//								else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleSECell )
//									oStyleTable->m_oSECell = m_oCurTableStyle;
//							}
//						}
//						else
//							oDocument.m_oStyleTable.AddItem( m_oCurStyle );
//						break;
//					}
//			}
//		}
//};
//		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
//		{
//			if( _T("stylesheet") == sCommand )
//				return true;
//			else
//			{
//				RtfStyleReader oStyleReader;
//				return StartSubReader( oStyleReader, oDocument, oReader );
//			}
//		}
//};

class ListTableReader: public RtfAbstractReader
{
public: 
	class ListReader: public RtfAbstractReader
	{
	public:
		class ListLevelReader: public RtfAbstractReader
		{
			private: RtfListLevelProperty & m_oListLevelProp;
			public: 
				ListLevelReader(RtfListLevelProperty & oListLevelProp):m_oListLevelProp(oListLevelProp)
				{
				}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("listlevel") == sCommand )
				;
			COMMAND_RTF_INT( _T("levelnfc"), m_oListLevelProp.m_nNumberType, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("levelnfcn"), m_oListLevelProp.m_nNumberType, sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( _T("lvltentative"), m_oListLevelProp.m_bTentative, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("leveljc"), m_oListLevelProp.m_nJustification, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("leveljcn"), m_oListLevelProp.m_nJustification, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("levelfollow"), m_oListLevelProp.m_nFollow, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("levelstartat"), m_oListLevelProp.m_nStart, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("levelnorestart"), m_oListLevelProp.m_nNoRestart, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("levellegal"), m_oListLevelProp.m_nLegal, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("levelpicture"), m_oListLevelProp.m_nPictureIndex, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("li"), m_oListLevelProp.m_nIndent, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("lin"), m_oListLevelProp.m_nIndentStart, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("fi"), m_oListLevelProp.m_nFirstIndent, sCommand, hasParameter, parameter )
			else if( _T("tx") == sCommand  )
			{
				if( true == hasParameter )
				{
					RtfTab oNewTab;
					oNewTab.m_nTab = parameter;
					m_oListLevelProp.m_oTabs.m_aTabs.push_back( oNewTab );
				}
			}
			else if( _T("leveltext") == sCommand  )
			{
				TextReader oLevelTextReader( m_oListLevelProp.m_sText );
				return StartSubReader( oLevelTextReader, oDocument, oReader );
			}
			else if( _T("levelnumbers") == sCommand  )
			{
				TextReader oLevelNumberReader( m_oListLevelProp.m_sNumber );
				return StartSubReader( oLevelNumberReader, oDocument, oReader );
			}
			else
			{
				return RtfCharPropCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter );
			}
			return true;
		}
		void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
		{
			m_oListLevelProp.m_oCharProp = oReader.m_oState->m_oCharProp;
			//убираем shading и border (word тоже так делает)
			m_oListLevelProp.m_oCharProp.m_poBorder.SetDefaultRtf();
			m_oListLevelProp.m_oCharProp.m_poShading.SetDefaultRtf();

		}
};
private: RtfListProperty& m_oListProp;
public: 
		ListReader(RtfListProperty& oListProp):m_oListProp(oListProp)
		{
		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("list") == sCommand )
				;
			COMMAND_RTF_INT( _T("listid"), m_oListProp.m_nID , sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("listtemplateid"), m_oListProp.m_nTemplateId , sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( _T("listsimple"), m_oListProp.m_nListSimple , sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( _T("listhybrid"), m_oListProp.m_bListHybrid , sCommand, hasParameter, parameter )
			else if( _T("listname") == sCommand )
			{
				TextReader oListNameReader( m_oListProp.m_sName );
				return StartSubReader( oListNameReader, oDocument, oReader );
			}
			else if( _T("listlevel") == sCommand  )
			{
				RtfListLevelProperty oListLevelProp;
				ListLevelReader oListLevelReader( oListLevelProp );
				bool bResult = StartSubReader( oListLevelReader, oDocument, oReader );
				if( true == bResult )
				{
					oListLevelProp.m_nLevel = m_oListProp.GetCount();
					m_oListProp.AddItem( oListLevelProp );
					return true;
				}
			}
			else
			{
				return false;
			}
			return true;
		}
};
class ListPictureReader: public RtfAbstractReader
{
public: bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("listpicture") == sCommand )
			{
			}
			else if( _T("shppict") == sCommand )
			{
				RtfShapePtr oNewPicture = RtfShapePtr( new RtfShape() );
				ShppictReader oShppictReader( *oNewPicture );
				StartSubReader( oShppictReader, oDocument, oReader );
				oDocument.m_oListTabel.m_aPictureList.AddItem( oNewPicture );
			}
			else
				return false;
			return true;
		}
};
public: ListTableReader()
		{

		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("listtable") == sCommand )
				return true;
			else if( _T("list") == sCommand )
			{
				RtfListProperty oListProp;
				ListReader oListReader(oListProp);
				bool bResult = StartSubReader( oListReader, oDocument, oReader );
				if( true == bResult )
				{
					oDocument.m_oListTabel.AddItem( oListProp );
					return true;
				}
				else
					return false;
			}
			else if( _T("listpicture") == sCommand )
			{
				ListPictureReader oListPictureReader;
				StartSubReader( oListPictureReader, oDocument, oReader );
			}
			else
				return false;
			return true;

		}
};
class ListOverrideTableReader: public RtfAbstractReader
{
private: 
	class ListOverrideReader : public RtfAbstractReader
	{
	private: 
		class lfolevelReader : public RtfAbstractReader
		{
		private: 
			RtfListOverrideProperty::ListOverrideLevels::ListOverrideLevel& m_oOverrideLevel;
		public: 
			lfolevelReader( RtfListOverrideProperty::ListOverrideLevels::ListOverrideLevel& oOverrideLevel ):m_oOverrideLevel(oOverrideLevel)
			{
			}
			bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
			{
			  if( _T("lfolevel") == sCommand)
				  return true;
			  COMMAND_RTF_INT( _T("listoverrideformat"), m_oOverrideLevel.m_nLevelIndex, sCommand, hasParameter, parameter )
			  COMMAND_RTF_INT( _T("listoverridestartat"), m_oOverrideLevel.m_nStart, sCommand, hasParameter, parameter )
			  else if( _T("listlevel") == sCommand )
			  {
				  m_oOverrideLevel.m_oLevel.m_nLevel = m_oOverrideLevel.m_nLevelIndex;
				  ListTableReader::ListReader::ListLevelReader oListLevelReader( m_oOverrideLevel.m_oLevel );
				  StartSubReader( oListLevelReader, oDocument, oReader );
			  }
			  else
				  return false;
			  return true;
			}
		};
		private: 
			RtfListOverrideProperty& m_oProperty;
		 public:
			ListOverrideReader(RtfListOverrideProperty& oProperty):m_oProperty(oProperty)
			{
			}
			bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
			{
				if( _T("listoverride") == sCommand)
					return true;
				else if( _T("listid") == sCommand )
				{
					if( true == hasParameter )
						m_oProperty.m_nListID = parameter;
				}
				else if( _T("ls") == sCommand )
				{
					if( true == hasParameter )
						m_oProperty.m_nIndex = parameter;
				}
				else if( _T("lfolevel") == sCommand )
				{
					RtfListOverrideProperty::ListOverrideLevels::ListOverrideLevel oOverrideLevel;
					lfolevelReader olfolevelReader( oOverrideLevel );
					StartSubReader( olfolevelReader, oDocument, oReader );
					m_oProperty.m_oOverrideLevels.m_aOverrideLevels.push_back( oOverrideLevel );
				}
				else
					return false;
				return true;
			}
		 };
public: 
	ListOverrideTableReader()
	{
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
	{
		if( _T("listoverridetable") == sCommand )
			return true;
		else if( _T("listoverride") == sCommand)
		{
			RtfListOverrideProperty oProperty;
			ListOverrideReader oListOverrideReader( oProperty );
			bool bResult = StartSubReader( oListOverrideReader, oDocument, oReader );
			if( true == bResult )
			{
				oDocument.m_oListOverrideTabel.AddItem(oProperty);
				return true;
			}
			else
				return false;
		}
		else
		{
			return false;
		}
		return true;

	}
};
class ParagraphReader : public RtfAbstractReader
{
//private: ItemContainer< RtfReaderParagraphPtr > m_aArray;
private: 
	CString m_sHeader;
public: 
	ParagraphPropDestination m_oParPropDest;

	ParagraphReader( CString sHeader, RtfReader& oReader ):m_sHeader(sHeader)
	{
		if( PROP_DEF != oReader.m_oState->m_oParagraphProp.m_nItap )
			m_oParPropDest.nTargetItap = oReader.m_oState->m_oParagraphProp.m_nItap;
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
	{
		if( m_sHeader == sCommand )
			return true;
		else
			return m_oParPropDest.ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter );
	}
	void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, CString sText )
	{
		m_oParPropDest.ExecuteText( oDocument, oReader, sText );
	}
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
	{
		m_oParPropDest.Finalize( oReader );
	}
};


class RtfDocumentCommand
{
public: 
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter)
	{

		if( _T("ansi") == sCommand )
			oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_ansi;
		else if( _T("mac") == sCommand )
			oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_mac;
		else if( _T("pc") == sCommand )
			oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_pc;
		else if( _T("pca") == sCommand )
			oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_pca;
		else if( _T("ansicpg") == sCommand )
		{
			oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_ansi;
			if( true == hasParameter )
				oDocument.m_oProperty.m_nAnsiCodePage = parameter;
		}
		else if( _T("deff") == sCommand )
		{
			if( true == hasParameter )
			{
				oDocument.m_oProperty.m_nDeffFont = parameter;
				oReader.m_nDefFont = parameter;
			}
		}

		COMMAND_RTF_BOOL( _T("hyphcaps"), oDocument.m_oProperty.m_bHypCaps, sCommand, hasParameter, parameter )
		COMMAND_RTF_BOOL( _T("hyphauto"), oDocument.m_oProperty.m_bAutoHyp, sCommand, hasParameter, parameter )
		COMMAND_RTF_INT( _T("hyphconsec"), oDocument.m_oProperty.m_nMaxHypen, sCommand, hasParameter, parameter )
		COMMAND_RTF_INT( _T("hyphhotz"), oDocument.m_oProperty.m_nHyphenationRight, sCommand, hasParameter, parameter )
		COMMAND_RTF_INT( _T("deftab"), oDocument.m_oProperty.m_nTabWidth, sCommand, hasParameter, parameter )

//Page Borders
		COMMAND_RTF_BOOL( _T("pgbrdrhead"), oDocument.m_oProperty.m_bDorderSurroundHeader, sCommand, hasParameter, parameter )
		COMMAND_RTF_BOOL( _T("pgbrdrfoot"), oDocument.m_oProperty.m_bDorderSurroundFotter, sCommand, hasParameter, parameter )
		COMMAND_RTF_BOOL( _T("pgbrdrsnap"), oDocument.m_oProperty.m_bAlignBordersAndEdges, sCommand, hasParameter, parameter )

//Page Information
		else if( _T("paperw") == sCommand )
		{
			if( true == hasParameter )
			{
				oDocument.m_oProperty.m_nPaperWidth = parameter;
				oReader.m_oCurSectionProp.m_nPageWidth = parameter;
			}
		}
		else if( _T("paperh") == sCommand )
		{
			if( true == hasParameter )
			{
				oDocument.m_oProperty.m_nPaperHeight = parameter;
				oReader.m_oCurSectionProp.m_nPageHeight = parameter;
			}
		}
		else if( _T("margl") == sCommand )
		{
			if( true == hasParameter )
			{
				oDocument.m_oProperty.m_nMarginLeft = parameter;
				oReader.m_oCurSectionProp.m_nMarginLeft = parameter;
			}
		}
		else if( _T("margr") == sCommand )
		{
			if( true == hasParameter )
			{
				oDocument.m_oProperty.m_nMarginRight = parameter;
				oReader.m_oCurSectionProp.m_nMarginRight = parameter;
			}
		}
		else if( _T("margt") == sCommand )
		{
			if( true == hasParameter )
			{
				oDocument.m_oProperty.m_nMarginTop = parameter;
				oReader.m_oCurSectionProp.m_nMarginTop = parameter;
			}
		}
		else if( _T("margb") == sCommand )
		{
			if( true == hasParameter )
			{
				oDocument.m_oProperty.m_nMarginBottom = parameter;
				oReader.m_oCurSectionProp.m_nMarginBottom = parameter;
			}
		}
		else if( _T("facingp") == sCommand )
		{
			if( true == hasParameter && 0 == parameter )
			{
				oDocument.m_oProperty.m_bFacingPage = 0;
				oReader.m_oCurSectionProp.m_bSwitchMargin = 0;
			}
			else
			{
				oDocument.m_oProperty.m_bFacingPage = 1;
				oReader.m_oCurSectionProp.m_bSwitchMargin = 1;
			}
		}
		else if( _T("gutter") == sCommand )
		{
			if( true == hasParameter )
			{
				oDocument.m_oProperty.m_nGutterWidth = parameter;
				oReader.m_oCurSectionProp.m_nGutterMarginWidth = parameter;
			}
		}
		else if( _T("ogutter") == sCommand )
		{
			if( true == hasParameter )
				oDocument.m_oProperty.m_nGutterWidthOutside = parameter;
		}
		else if( _T("margmirror") == sCommand )
		{
			if( true == hasParameter && 0 == parameter )
			{
				oDocument.m_oProperty.m_bSwitchMargins = 0;
				oReader.m_oCurSectionProp.m_bLandscapeFormat = 0;
			}
			else
			{
				oDocument.m_oProperty.m_bSwitchMargins = 1;
				oReader.m_oCurSectionProp.m_bLandscapeFormat = 1;
			}
		}
		else if( _T("landscape") == sCommand )
		{
			if( true == hasParameter && 0 == parameter )
				oDocument.m_oProperty.m_bLandScape = 0;
			else
				oDocument.m_oProperty.m_bLandScape = 1;
		}
		COMMAND_RTF_BOOL( _T("gutterprl"), oDocument.m_oProperty.m_bGutterAtTop, sCommand, hasParameter, parameter )
		COMMAND_RTF_BOOL( _T("viewbksp"), oDocument.m_oProperty.m_nDisplayBackground, sCommand, hasParameter, parameter )
		COMMAND_RTF_BOOL( _T("htmautsp"), oDocument.m_oProperty.m_bHtmlAutoSpace, sCommand, hasParameter, parameter )
		COMMAND_RTF_BOOL( _T("useltbaln"), oDocument.m_oProperty.m_bUseTabAlignment, sCommand, hasParameter, parameter )
		//Footnotes and Endnotes
		COMMAND_RTF_INT( _T("endnotes"), oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_EndSection )
		COMMAND_RTF_INT( _T("enddoc"), oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_EndDocument )
		COMMAND_RTF_INT( _T("ftntj"), oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_BeneathText )
		COMMAND_RTF_INT( _T("ftnbj"), oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_BottomPage )

		COMMAND_RTF_INT( _T("aendnotes"), oDocument.m_oProperty.m_eEndnotePlacement, sCommand, true, RtfDocumentProperty::ep_EndSection )
		COMMAND_RTF_INT( _T("aenddoc"), oDocument.m_oProperty.m_eEndnotePlacement, sCommand, true, RtfDocumentProperty::ep_EndDocument )
		COMMAND_RTF_INT( _T("aftnbj"), oDocument.m_oProperty.m_eEndnotePlacement, sCommand, true, RtfDocumentProperty::ep_BeneathText )
		COMMAND_RTF_INT( _T("aftntj"), oDocument.m_oProperty.m_eEndnotePlacement, sCommand, true, RtfDocumentProperty::ep_BottomPage )

		COMMAND_RTF_INT( _T("ftnstart"), oDocument.m_oProperty.m_nFootnoteStart, sCommand, hasParameter, parameter )
		COMMAND_RTF_INT( _T("aftnstart"), oDocument.m_oProperty.m_nEndnoteStart, sCommand, hasParameter, parameter )

		COMMAND_RTF_INT( _T("ftnrstpg"), oDocument.m_oProperty.m_eFootnoteRestart, sCommand, true, RtfDocumentProperty::fr_EachPage )
		COMMAND_RTF_INT( _T("ftnrestart"), oDocument.m_oProperty.m_eFootnoteRestart, sCommand, true, RtfDocumentProperty::fr_EachSection )
		COMMAND_RTF_INT( _T("ftnrstcont"), oDocument.m_oProperty.m_eFootnoteRestart, sCommand, true, RtfDocumentProperty::fr_Continuous )
		COMMAND_RTF_INT( _T("aftnrestart"), oDocument.m_oProperty.m_eEndnoteRestart, sCommand, true, RtfDocumentProperty::er_EachSection )
		COMMAND_RTF_INT( _T("aftnrstcont"), oDocument.m_oProperty.m_eEndnoteRestart, sCommand, true, RtfDocumentProperty::er_Continuous )

		COMMAND_RTF_INT( _T("ftnnar"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 0 )
		COMMAND_RTF_INT( _T("ftnnalc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 4 )
		COMMAND_RTF_INT( _T("ftnnauc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 3 )
		COMMAND_RTF_INT( _T("ftnnrlc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 2 )
		COMMAND_RTF_INT( _T("ftnnruc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 1 )
		COMMAND_RTF_INT( _T("ftnnchi"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 70 )
		COMMAND_RTF_INT( _T("ftnnchi"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 25 )
		COMMAND_RTF_INT( _T("ftnncnum"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 18 )
		COMMAND_RTF_INT( _T("ftnndbnum"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 10 )
		COMMAND_RTF_INT( _T("ftnndbnumd"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 11 )
		COMMAND_RTF_INT( _T("ftnndbnumt"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 16 )
		COMMAND_RTF_INT( _T("ftnndbnumk"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 17 )
		COMMAND_RTF_INT( _T("ftnndbar"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 20 )
		COMMAND_RTF_INT( _T("ftnnganada"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 24 )
		COMMAND_RTF_INT( _T("ftnngbnum"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 26 )
		COMMAND_RTF_INT( _T("ftnngbnumd"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 27 )
		COMMAND_RTF_INT( _T("ftnngbnuml"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 28 )
		COMMAND_RTF_INT( _T("ftnngbnumk"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 29 )
		COMMAND_RTF_INT( _T("ftnnzodiac"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 30 )
		COMMAND_RTF_INT( _T("ftnnzodiacd"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 31 )
		COMMAND_RTF_INT( _T("ftnnzodiacl"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 32 )

		COMMAND_RTF_INT( _T("aftnnar"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 0 )
		COMMAND_RTF_INT( _T("aftnnalc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 4 )
		COMMAND_RTF_INT( _T("aftnnauc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 3 )
		COMMAND_RTF_INT( _T("aftnnrlc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 2 )
		COMMAND_RTF_INT( _T("aftnnruc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 1 )
		COMMAND_RTF_INT( _T("aftnnchi"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 70 )
		COMMAND_RTF_INT( _T("aftnnchi"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 25 )
		COMMAND_RTF_INT( _T("aftnncnum"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 18 )
		COMMAND_RTF_INT( _T("aftnndbnum"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 10 )
		COMMAND_RTF_INT( _T("aftnndbnumd"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 11 )
		COMMAND_RTF_INT( _T("aftnndbnumt"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 16 )
		COMMAND_RTF_INT( _T("aftnndbnumk"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 17 )
		COMMAND_RTF_INT( _T("aftnndbar"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 20 )
		COMMAND_RTF_INT( _T("aftnnganada"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 24 )
		COMMAND_RTF_INT( _T("aftnngbnum"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 26 )
		COMMAND_RTF_INT( _T("aftnngbnumd"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 27 )
		COMMAND_RTF_INT( _T("aftnngbnuml"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 28 )
		COMMAND_RTF_INT( _T("aftnngbnumk"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 29 )
		COMMAND_RTF_INT( _T("aftnnzodiac"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 30 )
		COMMAND_RTF_INT( _T("aftnnzodiacd"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 31 )
		COMMAND_RTF_INT( _T("aftnnzodiacl"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 32 )
		else
			return false;
		return true;
	}

};
class RtfSectionCommand
{
private: 
	typedef enum{ is_none, is_border_left, is_border_top, is_border_right, is_border_bottom }InternalState;
		InternalState m_eInternalState;
		int nCurCollumnNumber;
public: RtfSectionCommand()
		{
			m_eInternalState = is_none;
			nCurCollumnNumber = PROP_DEF;
		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, RtfAbstractReader& oAbstrReader, CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("11111111") == sCommand )
				;
			COMMAND_RTF_BOOL( _T("rtlsect"), oReader.m_oCurSectionProp.m_bBidi, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("binfsxn"), oReader.m_oCurSectionProp.m_nPaperSourceFirst, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("binsxn"), oReader.m_oCurSectionProp.m_nPaperSourceOther, sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( _T("rtlgutter"), oReader.m_oCurSectionProp.m_bRtlGutter, sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( _T("endnhere"), oReader.m_oCurSectionProp.m_bEndnotes, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("ds"), oReader.m_oCurSectionProp.m_nStyle, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("sbknone"), oReader.m_oCurSectionProp.m_eSectionBreak, sCommand, true, RtfSectionProperty::sb_sbknone)
			COMMAND_RTF_INT( _T("sbkcol"), oReader.m_oCurSectionProp.m_eSectionBreak, sCommand, true, RtfSectionProperty::sb_sbkcol)
			COMMAND_RTF_INT( _T("sbkpage"), oReader.m_oCurSectionProp.m_eSectionBreak, sCommand, true, RtfSectionProperty::sb_sbkpage)
			COMMAND_RTF_INT( _T("sbkeven"), oReader.m_oCurSectionProp.m_eSectionBreak, sCommand, true, RtfSectionProperty::sb_sbkeven)
			COMMAND_RTF_INT( _T("sbkodd"), oReader.m_oCurSectionProp.m_eSectionBreak, sCommand, true, RtfSectionProperty::sb_sbkodd)
			else if( _T("cols") == sCommand )
			{
				if( true == hasParameter )
				{
					oReader.m_oCurSectionProp.m_nColumnNumber = parameter;
					oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty.resize( parameter );
				}
			}
			COMMAND_RTF_INT( _T("colsx"), oReader.m_oCurSectionProp.m_nColumnSpace, sCommand, hasParameter, parameter)
			else if( _T("colno") == sCommand )
			{
				if( true == hasParameter )
					nCurCollumnNumber = parameter - 1;
			}
			else if( _T("colsr") == sCommand )
			{
				if( true == hasParameter )
					if( nCurCollumnNumber >= 0 && nCurCollumnNumber < (int)oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty.size() )
						oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty[ nCurCollumnNumber ].m_nColumnSpaceToRightOfCol = parameter;
			}
			else if( _T("colw") == sCommand )
			{
				if( true == hasParameter )
					if( nCurCollumnNumber >= 0 && nCurCollumnNumber < (int)oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty.size() )
						oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty[ nCurCollumnNumber ].m_nColumnWidth = parameter;
			}
			COMMAND_RTF_BOOL( _T("linebetcol"), oReader.m_oCurSectionProp.m_bColumnLineBetween, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("sftntj"), oReader.m_oCurSectionProp.m_eFootnotesJust, sCommand, true, RtfSectionProperty::fj_sftntj)
			COMMAND_RTF_INT( _T("sftnbj"), oReader.m_oCurSectionProp.m_eFootnotesJust, sCommand, true, RtfSectionProperty::fj_sftnbj)
			//Footnotes and Endnotes
			COMMAND_RTF_INT( _T("sftnstart"), oReader.m_oCurSectionProp.m_nFootnotesStart, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("sftnrstpg"), oReader.m_oCurSectionProp.m_eFootnotesRestart, sCommand, true, RtfSectionProperty::fr_sftnrstpg)
			COMMAND_RTF_INT( _T("sftnrestart"), oReader.m_oCurSectionProp.m_eFootnotesRestart, sCommand, true, RtfSectionProperty::fr_sftnrestart)
			COMMAND_RTF_INT( _T("sftnrstcont"), oReader.m_oCurSectionProp.m_eFootnotesRestart, sCommand, true, RtfSectionProperty::fr_sftnrstcont)
			COMMAND_RTF_INT( _T("sftnnar"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnar)
			COMMAND_RTF_INT( _T("sftnnalc"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnalc)
			COMMAND_RTF_INT( _T("sftnnauc"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnauc)
			COMMAND_RTF_INT( _T("sftnnrlc"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnrlc)
			COMMAND_RTF_INT( _T("sftnnruc"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnruc)
			COMMAND_RTF_INT( _T("sftnnchi"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnchi)
			COMMAND_RTF_INT( _T("sftnnchosung"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnchosung)
			COMMAND_RTF_INT( _T("sftnncnum"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnncnum)
			COMMAND_RTF_INT( _T("sftnndbnum"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnndbnum)
			COMMAND_RTF_INT( _T("sftnndbnumd"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnndbnumd)
			COMMAND_RTF_INT( _T("sftnndbnumt"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnndbnumt)
			COMMAND_RTF_INT( _T("sftnndbnumk"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnndbnumk)
			COMMAND_RTF_INT( _T("sftnndbar"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnndbar)
			COMMAND_RTF_INT( _T("sftnnganada"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnganada)
			COMMAND_RTF_INT( _T("sftnngbnum"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnngbnum)
			COMMAND_RTF_INT( _T("sftnngbnumd"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnngbnumd)
			COMMAND_RTF_INT( _T("sftnngbnuml"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnngbnuml)
			COMMAND_RTF_INT( _T("sftnngbnumk"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnngbnumk)
			COMMAND_RTF_INT( _T("sftnnzodiac"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnzodiac)
			COMMAND_RTF_INT( _T("sftnnzodiacd"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnzodiacd)
			COMMAND_RTF_INT( _T("sftnnzodiacl"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnzodiacl)
								
			COMMAND_RTF_INT( _T("saftnstart"), oReader.m_oCurSectionProp.m_nEndnotesStart, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("saftnrestart"), oReader.m_oCurSectionProp.m_eEndnotesRestart, sCommand, true, RtfSectionProperty::er_saftnrestart)
			COMMAND_RTF_INT( _T("saftnrstcont"), oReader.m_oCurSectionProp.m_eEndnotesRestart, sCommand, true, RtfSectionProperty::er_saftnrstcont)
			COMMAND_RTF_INT( _T("saftnnar"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnar)
			COMMAND_RTF_INT( _T("saftnnalc"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnalc)
			COMMAND_RTF_INT( _T("saftnnauc"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnauc)
			COMMAND_RTF_INT( _T("saftnnrlc"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnrlc)
			COMMAND_RTF_INT( _T("saftnnruc"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnruc)
			COMMAND_RTF_INT( _T("saftnnchi"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnchi)
			COMMAND_RTF_INT( _T("saftnnchosung"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnchosung)
			COMMAND_RTF_INT( _T("saftnncnum"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnncnum)
			COMMAND_RTF_INT( _T("saftnndbnum"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnndbnum)
			COMMAND_RTF_INT( _T("saftnndbnumd"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnndbnumd)
			COMMAND_RTF_INT( _T("saftnndbnumt"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnndbnumt)
			COMMAND_RTF_INT( _T("saftnndbnumk"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnndbnumk)
			COMMAND_RTF_INT( _T("saftnndbar"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnndbar)
			COMMAND_RTF_INT( _T("saftnnganada"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnganada)
			COMMAND_RTF_INT( _T("saftnngbnum"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnngbnum)
			COMMAND_RTF_INT( _T("saftnngbnumd"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnngbnumd)
			COMMAND_RTF_INT( _T("saftnngbnuml"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnngbnuml)
			COMMAND_RTF_INT( _T("saftnngbnumk"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnngbnumk)
			COMMAND_RTF_INT( _T("saftnnzodiac"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnzodiac)
			COMMAND_RTF_INT( _T("saftnnzodiacd"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnzodiacd)
			COMMAND_RTF_INT( _T("saftnnzodiacl"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnzodiacl)
			//Line Numbering
			COMMAND_RTF_INT( _T("linemod"), oReader.m_oCurSectionProp.m_nLineModulus, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("linex"), oReader.m_oCurSectionProp.m_nLineX, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("linestarts"), oReader.m_oCurSectionProp.m_nLineStart, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("linerestart"), oReader.m_oCurSectionProp.m_eLineNumberRestart, sCommand, true, RtfSectionProperty::lnr_linerestart)
			COMMAND_RTF_INT( _T("lineppage"), oReader.m_oCurSectionProp.m_eLineNumberRestart, sCommand, true, RtfSectionProperty::lnr_lineppage)
			COMMAND_RTF_INT( _T("linecont"), oReader.m_oCurSectionProp.m_eLineNumberRestart, sCommand, true, RtfSectionProperty::lnr_linecont)
			//Page Information
			COMMAND_RTF_INT( _T("pgwsxn"), oReader.m_oCurSectionProp.m_nPageWidth, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("pghsxn"), oReader.m_oCurSectionProp.m_nPageHeight, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("marglsxn"), oReader.m_oCurSectionProp.m_nMarginLeft, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("margrsxn"), oReader.m_oCurSectionProp.m_nMarginRight, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("margtsxn"), oReader.m_oCurSectionProp.m_nMarginTop, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("margbsxn"), oReader.m_oCurSectionProp.m_nMarginBottom, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("guttersxn"), oReader.m_oCurSectionProp.m_nGutterMarginWidth, sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( _T("margmirsxn"), oReader.m_oCurSectionProp.m_bSwitchMargin, sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( _T("lndscpsxn"), oReader.m_oCurSectionProp.m_bLandscapeFormat, sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( _T("titlepg"), oReader.m_oCurSectionProp.m_bTitlePage, sCommand, hasParameter, parameter)
			//else if( _T("headery") == sCommand )
			//{
			//	static int nCount = 0;
			//	nCount++;
			//	ATLTRACE( "count %d\n", nCount );
			//}
			COMMAND_RTF_INT( _T("headery"), oReader.m_oCurSectionProp.m_nHeaderTop, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("footery"), oReader.m_oCurSectionProp.m_nFooterBottom, sCommand, hasParameter, parameter)
			//Page Numbers
			COMMAND_RTF_INT( _T("pgnstarts"), oReader.m_oCurSectionProp.m_nPageNumberStart, sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( _T("pgncont"), oReader.m_oCurSectionProp.m_bPageNumberContinuos, sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( _T("pgnrestart"), oReader.m_oCurSectionProp.m_bPageNumberRestart, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("pgnx"), oReader.m_oCurSectionProp.m_nPageNumberX, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("pgny"), oReader.m_oCurSectionProp.m_nPageNumberY, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("pgndec"), oReader.m_oCurSectionProp.m_ePageNumberFormat, sCommand, true, RtfSectionProperty::pnf_pgndec)
			COMMAND_RTF_INT( _T("pgnucrm"), oReader.m_oCurSectionProp.m_ePageNumberFormat, sCommand, true, RtfSectionProperty::pnf_pgnucrm)
			COMMAND_RTF_INT( _T("pgnlcrm"), oReader.m_oCurSectionProp.m_ePageNumberFormat, sCommand, true, RtfSectionProperty::pnf_pgnlcrm)
			COMMAND_RTF_INT( _T("vertalt"), oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalt)
			COMMAND_RTF_INT( _T("vertalb"), oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalb)
			COMMAND_RTF_INT( _T("vertalc"), oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalc)
			COMMAND_RTF_INT( _T("vertalj"), oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalj)
			//Text Flow
			COMMAND_RTF_INT( _T("stextflow"), oReader.m_oCurSectionProp.m_nTextFollow, sCommand, hasParameter, parameter)

			//Page Borders
			else if( _T("pgbrdrl") == sCommand )
				m_eInternalState = is_border_left;
			else if( _T("pgbrdrt") == sCommand )
				m_eInternalState = is_border_top;
			else if( _T("pgbrdrr") == sCommand )
				m_eInternalState = is_border_right;
			else if( _T("pgbrdrb") == sCommand )
				m_eInternalState = is_border_bottom;
			COMMAND_RTF_INT( _T("brdrart"), oReader.m_oCurSectionProp.m_nBorderArt, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT( _T("pgbrdropt"), oReader.m_oCurSectionProp.m_nBorderMeasure, sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( _T("pgbrdrsna"), oReader.m_oCurSectionProp.m_nBorderAlign, sCommand, hasParameter, parameter)
			else if( _T("header" == sCommand || "footer" == sCommand ||  "headerl") == sCommand || 
					 _T("headerr" == sCommand ||  "headerf" == sCommand ||  "footerl") == sCommand || 
					 _T("footerr" == sCommand ||  "footerf") == sCommand )
			{
				ParagraphReader oParagraphReader(sCommand, oReader);
				oAbstrReader.StartSubReader( oParagraphReader, oDocument, oReader );

				TextItemContainerPtr oNewFooterHeader = oParagraphReader.m_oParPropDest.m_oTextItems; 

				if( _T("header") == sCommand )
					oReader.m_oCurSectionProp.m_oHeaderRight = oNewFooterHeader;
				else if( _T("footer") == sCommand )
					oReader.m_oCurSectionProp.m_oFooterRight = oNewFooterHeader;
				else if( _T("headerl") == sCommand )
					oReader.m_oCurSectionProp.m_oHeaderLeft = oNewFooterHeader;
				else if( _T("headerr") == sCommand )
					oReader.m_oCurSectionProp.m_oHeaderRight = oNewFooterHeader;
				else if( _T("headerf") == sCommand )
					oReader.m_oCurSectionProp.m_oHeaderFirst = oNewFooterHeader;
				else if( _T("footerl") == sCommand )
					oReader.m_oCurSectionProp.m_oFooterLeft = oNewFooterHeader;
				else if( _T("footerr") == sCommand )
					oReader.m_oCurSectionProp.m_oFooterRight = oNewFooterHeader;
				else if( _T("footerr") == sCommand )
					oReader.m_oCurSectionProp.m_oFooterFirst = oNewFooterHeader;
			}
			else
			{
				bool bRes = false;
				if( is_border_left == m_eInternalState )
					bRes = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oCurSectionProp.m_oBorderLeft );
				else if( is_border_top == m_eInternalState )
					bRes = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oCurSectionProp.m_oBorderTop );
				else if( is_border_right == m_eInternalState )
					bRes = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oCurSectionProp.m_oBorderRight );
				else if( is_border_bottom == m_eInternalState )
					bRes = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oCurSectionProp.m_oBorderBottom );

				if( true == bRes )
					return true;
				return false;
			}

			return true;
		}

};
class RtfNormalReader : public RtfAbstractReader
{
public: 
		ParagraphPropDestination oParagraphReaderDestination;
		RtfSectionCommand oRtfSectionCommand;

		RtfNormalReader( RtfDocument& oDocument, RtfReader& oReader )
		{
			RtfSectionPtr oCurSection;
			SectDef( oDocument, oReader );
			if(true == oDocument.GetItem( oCurSection ) )
			{
				oCurSection->m_oProperty = oReader.m_oCurSectionProp;
				oParagraphReaderDestination.m_oTextItems = oCurSection;
			}
			m_nCurGroups = 0;
		}
		bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter )
		 {
			if( _T("colortbl") == sCommand )
			{
				RtfColorTableReader oColorTableReader;
				return StartSubReader( oColorTableReader, oDocument, oReader );
			}
			else if( _T("fonttbl") == sCommand )
			{
				RtfFontTableReader oFontTableReader;
				return StartSubReader( oFontTableReader, oDocument, oReader );
			}
			else if( _T("stylesheet") == sCommand )
			{
				Skip( oDocument, oReader );
				//StyleTableReader oStyleReader;
				//return StartSubReader( oStyleReader, oDocument, oReader );
			}
			//else if( _T("latentstyles") == sCommand )
			//{
			//	LatentstyleTableReader oLatentstyleReader;
			//	return StartSubReader( oLatentstyleReader, oDocument, oReader );
			//}
			else if( _T("listtable") == sCommand )
			{
				ListTableReader oListTableReader;
				return StartSubReader( oListTableReader, oDocument, oReader );

			}
			else if( _T("listoverridetable") == sCommand )
			{
				ListOverrideTableReader oListOverrideReader;
				return StartSubReader( oListOverrideReader, oDocument, oReader );
			}
			else if( _T("info") == sCommand )
			{
				RtfInfoReader oInfoReader;
				return StartSubReader( oInfoReader, oDocument, oReader );			
			}
			//else if( _T("colorschememapping") == sCommand )
			//{
			//	RtfColorSchemeReader oSchemeReader;
			//	return StartSubReader( oSchemeReader, oDocument, oReader );		
			//}
			//else if( _T("themedata") == sCommand )
			//{
			//	RtfThemeDataReader oThemeDataReader;
			//	return StartSubReader( oThemeDataReader, oDocument, oReader );	
			//}
			//else if( _T("defchp") == sCommand )
			//{
			//	RtfDefCharPropReader oDefCharPropReader( oDocument.m_oDefaultCharProp );
			//	return StartSubReader( oDefCharPropReader, oDocument, oReader );				}
			//else if( _T("defpap") == sCommand )
			//{
			//	RtfDefParPropReader oDefParPropReader;
			//	return StartSubReader( oDefParPropReader, oDocument, oReader );
			//}
			else if( _T("mmathPr") == sCommand )
			{
				RtfMathPropReader oMathPropReader;
				return StartSubReader( oMathPropReader, oDocument, oReader );
			}
			else if( _T("ftnsep" == sCommand || "ftnsepc") == sCommand || 
					_T("aftnsep" == sCommand || "aftnsepc") == sCommand )
			{
				ParagraphReader oParagraphReader(sCommand, oReader);
				StartSubReader( oParagraphReader, oDocument, oReader );
				if( _T("ftnsep") == sCommand )
				{
					oDocument.m_oFootnoteSep = oParagraphReader.m_oParPropDest.m_oTextItems;
					oDocument.m_oProperty.m_aSpecialFootnotes.push_back( 0 );
				}
				else if( _T("ftnsepc") == sCommand )
				{
					oDocument.m_oFootnoteCon = oParagraphReader.m_oParPropDest.m_oTextItems;
					oDocument.m_oProperty.m_aSpecialFootnotes.push_back( 1 );
				}
				else if( _T("aftnsep") == sCommand )
				{
					oDocument.m_oEndnoteSep = oParagraphReader.m_oParPropDest.m_oTextItems;
					oDocument.m_oProperty.m_aSpecialEndnotes.push_back( 0 );
				}
				else if( _T("aftnsepc") == sCommand )
				{
					oDocument.m_oEndnoteCon = oParagraphReader.m_oParPropDest.m_oTextItems;
					oDocument.m_oProperty.m_aSpecialEndnotes.push_back( 1 );
				}
			}
			else if( _T("sect") == sCommand )
			{
				RtfSectionPtr oCurSection;
				if(true == oDocument.GetItem( oCurSection ) )
					oCurSection->m_oProperty = oReader.m_oCurSectionProp;
				//вручную обнуляем footer, т.к. sectd может встретиться и после field
				oReader.m_oCurSectionProp.m_oHeaderLeft = TextItemContainerPtr();
				oReader.m_oCurSectionProp.m_oHeaderFirst = TextItemContainerPtr();
				oReader.m_oCurSectionProp.m_oHeaderRight = TextItemContainerPtr();
				oReader.m_oCurSectionProp.m_oFooterLeft = TextItemContainerPtr();
				oReader.m_oCurSectionProp.m_oFooterFirst = TextItemContainerPtr();
				oReader.m_oCurSectionProp.m_oFooterRight = TextItemContainerPtr();

				RtfSectionPtr oNewSection = RtfSectionPtr( new RtfSection() );
				oParagraphReaderDestination.Finalize( oReader );
				oParagraphReaderDestination.m_oTextItems = oNewSection;
				oDocument.AddItem( oNewSection );
			}
			else if(  _T("sectd") == sCommand  )
			{
				SectDef( oDocument, oReader );
			}
			else
			{
				bool bResult = false;
				bResult = oParagraphReaderDestination.ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter );
				if( true == bResult )
					return true;
				bResult = RtfDocumentCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter );
				if( true == bResult )
					return true;
				bResult = oRtfSectionCommand.ExecuteCommand( oDocument, oReader, (*this),sCommand, hasParameter, parameter );
				if( true == bResult )
					return true;
				bResult = RtfDocumentCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter );
				if( true == bResult )
					return true;
				return false;
			}
			return true;
		 }
		void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
		{
			oParagraphReaderDestination.ExecuteText( oDocument, oReader, sText );
		}
		void ExitReader(RtfDocument& oDocument, RtfReader& oReader)
		{
			oParagraphReaderDestination.Finalize(oReader);
			RtfSectionPtr oCurSection;
			if(true == oDocument.GetItem( oCurSection ) )
				oCurSection->m_oProperty = oReader.m_oCurSectionProp;

			if( NULL == oDocument.m_oFootnoteCon )
			{
				oDocument.m_oFootnoteCon = TextItemContainerPtr( new TextItemContainer() );
				RtfParagraphPtr oNewPar = RtfParagraphPtr( new RtfParagraph() );
				RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
				oNewChar->m_eType = RtfCharSpecial::rsc_chftnsepc;
				
				oNewPar->AddItem( oNewChar );
				oDocument.m_oFootnoteCon->AddItem( oNewPar );
				oDocument.m_oProperty.m_aSpecialFootnotes.push_back( 1 );
			}
			if( NULL == oDocument.m_oFootnoteSep )
			{
				oDocument.m_oFootnoteSep = TextItemContainerPtr( new TextItemContainer() );
				RtfParagraphPtr oNewPar = RtfParagraphPtr( new RtfParagraph() );
				RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
				oNewChar->m_eType = RtfCharSpecial::rsc_chftnsep;
				
				oNewPar->AddItem( oNewChar );
				oDocument.m_oFootnoteSep->AddItem( oNewPar );
				oDocument.m_oProperty.m_aSpecialFootnotes.push_back( 0 );
			}
			if( NULL == oDocument.m_oEndnoteCon )
			{
				oDocument.m_oEndnoteCon = TextItemContainerPtr( new TextItemContainer() );
				RtfParagraphPtr oNewPar = RtfParagraphPtr( new RtfParagraph() );
				RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
				oNewChar->m_eType = RtfCharSpecial::rsc_chftnsepc;
				
				oNewPar->AddItem( oNewChar );
				oDocument.m_oEndnoteCon->AddItem( oNewPar );
				oDocument.m_oProperty.m_aSpecialEndnotes.push_back( 1 );
			}
			if( NULL == oDocument.m_oEndnoteSep )
			{
				oDocument.m_oEndnoteSep = TextItemContainerPtr( new TextItemContainer() );
				RtfParagraphPtr oNewPar = RtfParagraphPtr( new RtfParagraph() );
				RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
				oNewChar->m_eType = RtfCharSpecial::rsc_chftnsep;
				
				oNewPar->AddItem( oNewChar );
				oDocument.m_oEndnoteSep->AddItem( oNewPar );
				oDocument.m_oProperty.m_aSpecialEndnotes.push_back( 0 );
			}
		}
private: 
	void SectDef( RtfDocument& oDocument, RtfReader& oReader )
	 {
		oReader.m_oCurSectionProp.SetDefaultRtf();
		//в соответствии с документацией ставим Page Information свойства как у документа
		oReader.m_oCurSectionProp.m_nPageWidth = oDocument.m_oProperty.m_nPaperWidth;
		oReader.m_oCurSectionProp.m_nPageHeight = oDocument.m_oProperty.m_nPaperHeight;
		oReader.m_oCurSectionProp.m_nMarginLeft = oDocument.m_oProperty.m_nMarginLeft;
		oReader.m_oCurSectionProp.m_nMarginRight = oDocument.m_oProperty.m_nMarginRight;
		oReader.m_oCurSectionProp.m_nMarginTop = oDocument.m_oProperty.m_nMarginTop;
		oReader.m_oCurSectionProp.m_nMarginBottom = oDocument.m_oProperty.m_nMarginBottom;
		oReader.m_oCurSectionProp.m_nGutterMarginWidth = oDocument.m_oProperty.m_nGutterWidth;
		oReader.m_oCurSectionProp.m_bSwitchMargin = oDocument.m_oProperty.m_bFacingPage;
		oReader.m_oCurSectionProp.m_bLandscapeFormat = oDocument.m_oProperty.m_bLandScape;
	 }
};
