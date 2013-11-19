#ifndef _CHTML_CSS_H
#define _CHTML_CSS_H

#include "..\..\HTMLReaderLib\HTMLTagWrapper.h"

namespace CSS
{

	static void RemoveWhiteSpaces(CString *psText)
	{
		psText->Remove( 0x01 ); psText->Remove( 0x10 );
		psText->Remove( 0x02 ); psText->Remove( 0x11 );
		psText->Remove( 0x03 ); psText->Remove( 0x12 );
		psText->Remove( 0x04 ); psText->Remove( 0x13 );
		psText->Remove( 0x05 ); psText->Remove( 0x14 );
		psText->Remove( 0x06 ); psText->Remove( 0x15 );
		psText->Remove( 0x07 ); psText->Remove( 0x16 );
		psText->Remove( 0x08 ); psText->Remove( 0x17 );
		psText->Remove( 0x09 ); psText->Remove( 0x18 );
		psText->Remove( 0x0a ); psText->Remove( 0x19 );
		psText->Remove( 0x0b ); psText->Remove( 0x1a );
		psText->Remove( 0x0c ); psText->Remove( 0x1b );
		psText->Remove( 0x0d ); psText->Remove( 0x1c );
		psText->Remove( 0x0e ); psText->Remove( 0x1d );
		psText->Remove( 0x0f ); psText->Remove( 0x1e );
		psText->Remove( 0x20 ); psText->Remove( 0x1f );
	}

	//------------------------------------------------------------------------------------------
	// CHtmlCSSSelector
	//------------------------------------------------------------------------------------------

	typedef enum
	{
		stUnknown = -1,
		stTag     =  0,
		stClass   =  1,
		stId      =  2,
		stAttr    =  3,
		stPseudo  =  4,
	} SelectorType;

	class CHtmlCSSSelector
	{
	public:

		CHtmlCSSSelector()
		{
			m_nWeight = 0;
			m_bValid  = false;
		}

		CHtmlCSSSelector(CString sBuffer)
		{
			m_nWeight = 0;
			m_bValid  = Parse(sBuffer);
		}
		~CHtmlCSSSelector()
		{
		}

		bool CheckValidation() const
		{
			return m_bValid;
		}


		void set_Operation(CString sOperation)
		{
			m_sOperation = sOperation;
		}

		const CString &get_Tag() const
		{
			return m_sTag;
		}

		const CString &get_Class() const
		{
			return m_sClass;
		}

		const CString &get_ID() const
		{
			return m_sID;
		}

		const CString &get_Operation() const
		{
			return m_sOperation;
		}

		const CSimpleArray<CString> &get_PseudoElements() const
		{
			return m_aPseudoElements;
		}

		const CSimpleArray<CString> &get_Attributes() const
		{
			return m_aAttributes;
		}

		const int &get_Weight() const
		{
			return m_nWeight;
		}

	private:

		bool Parse(CString &sBuffer)
		{
			int nPos = 0;
			CString sTemp;
			SelectorType eType = stTag;

			while( nPos < sBuffer.GetLength() )
			{
				int nChar = sBuffer.GetAt( nPos );

				if ( '.' == nChar )
				{
					AddElement( sTemp, eType );

					sTemp.Empty();
					eType = stClass;
				}
				else if ( '#' == nChar )
				{
					AddElement( sTemp, eType );

					sTemp.Empty();
					eType = stId;
				}
				else if ( '[' == nChar )
				{
					AddElement( sTemp, eType );

					sTemp.Empty();
					eType = stAttr;
				}
				else if ( ']' == nChar )
				{
					AddElement( sTemp, eType );

					sTemp.Empty();
					eType = stUnknown;
				}
				else if ( ':' == nChar )
				{
					AddElement( sTemp, eType );

					sTemp.Empty();
					eType = stPseudo;
				}
				else if ( ' ' == nChar && stAttr != eType )
				{
					AddElement( sTemp, eType );

					sTemp.Empty();
					eType = stUnknown;
				}
				else
				{
					sTemp.AppendChar( nChar );
				}

				nPos++;
			}

			AddElement( sTemp, eType );

			// Вычисляем вес данного селектора
			CalcWeight();

			return true;
		}

		void AddElement(CString &sElement, SelectorType eType)
		{
			if ( _T("") != sElement )
			{
				if ( stTag == eType )
				{
					m_sTag = sElement;
				}
				else if ( stClass == eType )
				{
					m_sClass = sElement;
				}
				else if ( stId == eType )
				{
					m_sID = sElement;
				}
				else if ( stAttr == eType )
				{
					m_aAttributes.Add( sElement );
				}
				else if ( stPseudo == eType )
				{
					m_aPseudoElements.Add( sElement );
				}
			}
		}

		void CalcWeight()
		{
			// Согласно спецификации CSS 3.0
			if ( _T("") != m_sTag && _T("*") != m_sTag )
			{
				m_nWeight += 1;
			}
			if ( _T("") != m_sClass )
			{
				m_nWeight += 10;
			}
			if ( _T("") != m_sID )
			{
				m_nWeight += 100;
			}

			m_nWeight += ( 10 * m_aAttributes.GetSize() );

			for ( int nIndex = 0; nIndex < m_aPseudoElements.GetSize(); nIndex++ )
			{
				// Псевдо-элементы имеют вес 1, а псевдо-классы - 10.
				// К псевдо-элементам относятся: first-letter, first-line, before, after. Все
				// остальное - псевдо-классы. Отдельно надо обрабатывать псевдо-класс :not().
				// Но это мы оставим на будущее.

				CString &sPseudo = m_aPseudoElements.GetData()[nIndex];
				if ( _T(":first-letter") == sPseudo || _T(":first-line") == sPseudo ||
					_T(":before")       == sPseudo || _T(":after")      == sPseudo )
				{
					m_nWeight += 1;
				}
				else
					m_nWeight += 10;

			}
		}

	private:

		bool    m_bValid;
		CString m_sTag;
		CString m_sClass;
		CString m_sID;

		CString m_sOperation;      // Отношение со следующим селектором. По умолчанию, пустая строка
		// означает простое наследование. ">" - child. "+" - first sibling.

		CSimpleArray<CString> m_aPseudoElements;
		CSimpleArray<CString> m_aAttributes;

		int     m_nWeight;

	};



	//------------------------------------------------------------------------------------------
	// CHtmlCSSProperty
	//------------------------------------------------------------------------------------------

	typedef enum
	{
		ptUnknown              = 0000,

		// Background Properties
		ptBackground           = 1000, // Sets all the background properties in one declaration
		ptBackgroundAttachment = 1001, // Sets whether a background image is fixed or scrolls with the rest of the page
		ptBackgroundColor      = 1002, // Sets the background color of an element
		ptBackgroundImage      = 1003, // Sets the background image for an element
		ptBackgroundPosition   = 1004, // Sets the starting position of a background image
		ptBackgroundRepeat     = 1005, // Sets how a background image will be repeated

		// Border and Outline Properties
		ptBorder               = 1100, // Sets all the border properties in one declaration
		ptBorderBottom         = 1101, // Sets all the bottom border properties in one declaration
		ptBorderBottomColor    = 1102, // Sets the color of the bottom border
		ptBorderBottomStyle    = 1103, // Sets the style of the bottom border
		ptBorderBottomWidth    = 1104, // Sets the width of the bottom border
		ptBorderColor          = 1105, // Sets the color of the four borders
		ptBorderLeft           = 1106, // Sets all the left border properties in one declaration
		ptBorderLeftColor      = 1107, // Sets the color of the left border
		ptBorderLeftStyle      = 1108, // Sets the style of the left border
		ptBorderLeftWidth      = 1109, // Sets the width of the left border
		ptBorderRight          = 1110, // Sets all the right border properties in one declaration
		ptBorderRightColor     = 1111, // Sets the color of the right border
		ptBorderRightStyle     = 1112, // Sets the style of the right border
		ptBorderRightWidth     = 1113, // Sets the width of the right border
		ptBorderStyle          = 1114, // Sets the style of the four borders
		ptBorderTop            = 1115, // Sets all the top border properties in one declaration
		ptBorderTopColor       = 1116, // Sets the color of the top border
		ptBorderTopStyle       = 1117, // Sets the style of the top border
		ptBorderTopWidth       = 1118, // Sets the width of the top border
		ptBorderWidth          = 1119, // Sets the width of the four borders
		ptOutline              = 1120, // Sets all the outline properties in one declaration
		ptOutlineColor         = 1121, // Sets the color of an outline
		ptOutlineStyle         = 1122, // Sets the style of an outline
		ptOutlineWidth         = 1123, // Sets the width of an outline

		// Dimension Properties
		ptHeight               = 1200, // Sets the height of an element
		ptMaxHeight            = 1201, // Sets the maximum height of an element
		ptMaxWidth             = 1202, // Sets the maximum width of an element
		ptMinHeight            = 1203, // Sets the minimum height of an element
		ptMinWidth             = 1204, // Sets the minimum width of an element
		ptWidth                = 1205, // Sets the width of an element

		// Font Properties
		ptFont                 = 1300, // Sets all the font properties in one declaration
		ptFontFamily           = 1301, // Specifies the font family for text
		ptFontSize             = 1302, // Specifies the font size of text
		ptFontStyle            = 1303, // Specifies the font style for text
		ptFontVariant          = 1304, // Specifies whether or not a text should be displayed in a small-caps font
		ptFontWeight           = 1305, // Specifies the weight of a font

		// Generated Content Properties
		ptContent              = 1400, // Used with the :before and :after pseudo-elements, to insert generated content
		ptCounterIncrement     = 1401, // Increments one or more counters
		ptCounterReset         = 1402, // Creates or resets one or more counters
		ptQuotes               = 1403, // Sets the type of quotation marks for embedded quotations

		// List Properties
		ptListStyle            = 1500, // Sets all the properties for a list in one declaration
		ptListStyleImage       = 1501, // Specifies an image as the list-item marker
		ptListStylePosition    = 1502, // Specifies if the list-item markers should appear inside or outside the content flow
		ptListStyleType        = 1503, // Specifies the type of list-item marker

		// Margin Properties
		ptMargin               = 1600, // Sets all the margin properties in one declaration
		ptMarginBottom         = 1601, // Sets the bottom margin of an element
		ptMarginLeft           = 1602, // Sets the left margin of an element
		ptMarginRight          = 1603, // Sets the right margin of an element
		ptMarginTop            = 1604, // Sets the top margin of an element

		// Padding Properties
		ptPadding              = 1700, // Sets all the padding properties in one declaration
		ptPaddingBottom        = 1701, // Sets the bottom padding of an element
		ptPaddingLeft          = 1702, // Sets the left padding of an element
		ptPaddingRight         = 1703, // Sets the right padding of an element
		ptPaddingTop           = 1704, // Sets the top padding of an element

		// Positioning Properties
		ptBottom               = 1800, // Sets the bottom margin edge for a positioned box
		ptClear                = 1801, // Specifies which sides of an element where other floating elements are not allowed
		ptClip                 = 1802, // Clips an absolutely positioned element
		ptCursor               = 1803, // Specifies the type of cursor to be displayed
		ptDisplay              = 1804, // Specifies the type of box an element should generate
		ptFloat                = 1805, // Specifies whether or not a box should float
		ptLeft                 = 1806, // Sets the left margin edge for a positioned box
		ptOverflow             = 1807, // Specifies what happens if content overflows an element's box
		ptPosition             = 1808, // Specifies the type of positioning for an element
		ptRight                = 1809, // Sets the right margin edge for a positioned box
		ptTop                  = 1810, // Sets the top margin edge for a positioned box
		ptVisibility           = 1811, // Specifies whether or not an element is visible
		ptZIndex               = 1812, // Sets the stack order of an element

		// Print Properties
		ptOrphans              = 1900, // Sets the minimum number of lines that must be left at the bottom of a page when a page break occurs inside an element
		ptPageBreakAfter       = 1901, // Sets the page-breaking behavior after an element
		ptPageBreakBefore      = 1902, // Sets the page-breaking behavior before an element
		ptPageBreakInside      = 1903, // Sets the page-breaking behavior inside an element
		ptWidows               = 1904, // Sets the minimum number of lines that must be left at the top of a page when a page break occurs inside an element

		// Table Properties
		ptBorderCollapse       = 2000, // Specifies whether or not table borders should be collapsed
		ptBorderSpacing        = 2001, // Specifies the distance between the borders of adjacent cells
		ptCaptionSide          = 2002, // Specifies the placement of a table caption
		ptEmptyCells           = 2003, // Specifies whether or not to display borders and background on empty cells in a table
		ptTableLayout          = 2004, // Sets the layout algorithm to be used for a table

		// Text Properties
		ptColor                = 2100, // Sets the color of text
		ptDirection            = 2101, // Specifies the text direction/writing direction
		ptLetterSpacing        = 2102, // Increases or decreases the space between characters in a text
		ptLineHeight           = 2103, // Sets the line height
		ptTextAlign            = 2104, // Specifies the horizontal alignment of text
		ptTextDecoration       = 2105, // Specifies the decoration added to text
		ptTextIndent           = 2106, // Specifies the indentation of the first line in a text-block
		ptTextShadow           = 2107, // Specifies the shadow effect added to text
		ptTextTransform        = 2108, // Controls the capitalization of text
		ptUnicodeBidi          = 2109, // 
		ptVerticalAlign        = 2110, // Sets the vertical alignment of an element
		ptWhiteSpace           = 2111, // Specifies how white-space inside an element is handled
		ptWordSpacing          = 2112, // Increases or decreases the space between words in a text

	} PropertyType;

	typedef enum 
	{
		svtUnknown   = 0000,

		// Measurement Values
		vtPercentage = 1000, // % percentage
		vtInch       = 1001, // in inch
		vtCm         = 1002, // cm centimeter
		vtMm         = 1003, // mm millimeter
		vtEm         = 1004, // em 1em is equal to the current font size. 2em means 2 times the size of the current font. E.g., if an element is displayed with a font of 12 pt, then '2em' is 24 pt. The 'em' is a very useful unit in CSS, since it can adapt automatically to the font that the reader uses
		vtEx         = 1005, // ex 1ex is the x-height of a font (x-height is usually about half the font-size)
		vtPt         = 1006, // pt point (1 pt is the same as 1/72 inch)
		vtPc         = 1007, // pc pica (1 pc is the same as 12 points)
		vtPx         = 1008, // px pixels (a dot on the computer screen)

		// Color Values
		vtColorName  = 1100, // A color name (e.g. red)
		vtColorRGB   = 1101, // rgb(x,x,x) An RGB value (e.g. rgb(255,0,0))
		vtColorRGB2  = 1102, // rgb(x%, x%, x%) An RGB percentage value (e.g. rgb(100%,0%,0%))
		vtColorHex   = 1103, // #rrggbb A HEX number (e.g. #ff0000)

		// Other Values
		vtString     = 1200, // String
		vtComplex    = 1201  // Тип составленный из нескольких простых

	} ValueType;

	class CHtmlCSSProperty
	{
	public:

		CHtmlCSSProperty()
		{
			m_eType = ptUnknown;
		}

		CHtmlCSSProperty(CString sBuffer)
		{
			Parse(sBuffer);

			TransformValue();
		}
		~CHtmlCSSProperty()
		{
		}

		void set_Value(CString sValue)
		{
			// TO DO: тут надо проверять передаваемое значение от типа m_eType
			m_sValue = sValue;
		}

		const CString &get_Property() const
		{
			return m_sProperty;
		}

		const CString &get_Value() const
		{
			return m_sValue;
		}

		const PropertyType &get_Type() const
		{
			return m_eType;
		}

		void ConvertValueToPx(double dEmValue = 16.0)
		{
			if ( ptFontSize == m_eType || ptMarginTop == m_eType || ptMarginBottom == m_eType || ptMarginLeft == m_eType || ptMarginRight == m_eType || ptTextIndent == m_eType )
			{
				// dEmValue - размер шрифта родительского тэга, или, что тоже самое, 1em
				int nPos = -1;
				if ( -1 != ( nPos = m_sValue.Find( _T("in") ) ) )
				{
					double dValue = _wtof( m_sValue.Mid( 0, nPos ) );
					dValue *= (2.54 / 0.0264);
					m_sValue.Empty();
					m_sValue.Format( _T("%.3fpx"), dValue );
				}
				else if ( -1 != ( nPos = m_sValue.Find( _T("cm") ) ) )
				{
					double dValue = _wtof( m_sValue.Mid( 0, nPos ) );
					dValue /= 0.0264;
					m_sValue.Empty();
					m_sValue.Format( _T("%.3fpx"), dValue );
				}
				else if ( -1 != ( nPos = m_sValue.Find( _T("mm") ) ) )
				{
					double dValue = _wtof( m_sValue.Mid( 0, nPos ) );
					dValue /= 0.00264;
					m_sValue.Empty();
					m_sValue.Format( _T("%.3fpx"), dValue );
				}
				else if ( -1 != ( nPos = m_sValue.Find( _T("em") ) ) )
				{
					double dValue = _wtof( m_sValue.Mid( 0, nPos ) );
					dValue *= dEmValue;
					m_sValue.Empty();
					m_sValue.Format( _T("%.3fpx"), dValue );
				}
				else if ( -1 != ( nPos = m_sValue.Find( _T("ex") ) ) )
				{
					// TO DO: надо протащить данный параметр по нормальному (аналогично 'em')
				}
				else if ( -1 != ( nPos = m_sValue.Find( _T("pt") ) ) )
				{
					double dValue = _wtof( m_sValue.Mid( 0, nPos ) );
					dValue *= (2.54 / 72 / 0.0264);
					m_sValue.Empty();
					m_sValue.Format( _T("%.3fpx"), dValue );
				}
				else if ( -1 != ( nPos = m_sValue.Find( _T("pc") ) ) )
				{
					double dValue = _wtof( m_sValue.Mid( 0, nPos ) );
					dValue *= (2.54 / 6 / 0.0264);
					m_sValue.Empty();
					m_sValue.Format( _T("%.3fpx"), dValue );
				}
				else if ( ptFontSize == m_eType && -1 != ( nPos = m_sValue.Find( _T("%") ) ) )
				{
					double dValue = _wtof( m_sValue.Mid( 0, nPos ) );
					dValue *= (dEmValue / 100);
					m_sValue.Empty();
					m_sValue.Format( _T("%.3fpx"), dValue );
				}
			}
		}

	private:

		void Parse(CString &sBuffer)
		{
			int nPos = sBuffer.Find( ':' );

			if ( -1 == nPos )
			{
				m_eType = ptUnknown;
			}
			else
			{
				m_sProperty = sBuffer.Mid( 0, nPos );
				m_sValue    = sBuffer.Mid( nPos + 1, sBuffer.GetLength() - (nPos + 1) );

				RemoveWhiteSpaces( &m_sProperty );

				if ( m_sProperty.GetLength() <= 2 )
				{
					m_eType = ptUnknown;
					return;
				}

				// Чтобы избежать большого количества сравнения строк проверим для начала по первым двум символам
				int nChar1 = m_sProperty.GetAt( 0 );
				int nChar2 = m_sProperty.GetAt( 1 );

				switch( nChar1 )
				{
				case 'b':
					{
						switch( nChar2 )
						{
						case 'a':
							{
								if      ( _T("background")            == m_sProperty ) m_eType = ptBackground;
								else if ( _T("background-attachment") == m_sProperty ) m_eType = ptBackgroundAttachment;
								else if ( _T("background-color")      == m_sProperty ) m_eType = ptBackgroundColor;
								else if ( _T("background-image")      == m_sProperty ) m_eType = ptBackgroundImage;
								else if ( _T("background-position")   == m_sProperty ) m_eType = ptBackgroundPosition;
								else if ( _T("background-repeat")     == m_sProperty ) m_eType = ptBackgroundRepeat;
								else                                                   m_eType = ptUnknown;

								break;
							}
						case 'o':
							{
								if      ( _T("border")              == m_sProperty ) m_eType = ptBorder;
								else if ( _T("border-bottom")       == m_sProperty ) m_eType = ptBorderBottom;
								else if ( _T("border-bottom-color") == m_sProperty ) m_eType = ptBorderBottomColor;
								else if ( _T("border-bottom-style") == m_sProperty ) m_eType = ptBorderBottomStyle;
								else if ( _T("border-bottom-width") == m_sProperty ) m_eType = ptBorderBottomWidth;
								else if ( _T("border-collapse")     == m_sProperty ) m_eType = ptBorderCollapse;
								else if ( _T("border-color")        == m_sProperty ) m_eType = ptBorderColor;
								else if ( _T("border-left")         == m_sProperty ) m_eType = ptBorderLeft;
								else if ( _T("border-left-color")   == m_sProperty ) m_eType = ptBorderLeftColor;
								else if ( _T("border-left-style")   == m_sProperty ) m_eType = ptBorderLeftStyle;
								else if ( _T("border-left-width")   == m_sProperty ) m_eType = ptBorderLeftWidth;
								else if ( _T("border-right")        == m_sProperty ) m_eType = ptBorderRight;
								else if ( _T("border-right-color")  == m_sProperty ) m_eType = ptBorderRightColor;
								else if ( _T("border-right-style")  == m_sProperty ) m_eType = ptBorderRightStyle;
								else if ( _T("border-right-width")  == m_sProperty ) m_eType = ptBorderRightWidth;
								else if ( _T("border-spacing")      == m_sProperty ) m_eType = ptBorderSpacing;
								else if ( _T("border-style")        == m_sProperty ) m_eType = ptBorderStyle;
								else if ( _T("border-top")          == m_sProperty ) m_eType = ptBorderTop;
								else if ( _T("border-top-color")    == m_sProperty ) m_eType = ptBorderTopColor;
								else if ( _T("border-top-style")    == m_sProperty ) m_eType = ptBorderTopStyle;
								else if ( _T("border-top-width")    == m_sProperty ) m_eType = ptBorderTopWidth;
								else if ( _T("border-width")        == m_sProperty ) m_eType = ptBorderWidth;
								else if ( _T("bottom")              == m_sProperty ) m_eType = ptBottom;
								else                                                 m_eType = ptUnknown;

								break;
							}
						default:
							{
								m_eType = ptUnknown; 
								break;
							}
						}
						break;
					}
				case 'c': 
					{
						switch( nChar2 )
						{
						case 'a':
							{
								if ( _T("caption-side") == m_sProperty ) m_eType = ptCaptionSide;
								else                                     m_eType = ptUnknown;

								break;
							}
						case 'l':
							{
								if      ( _T("clear") == m_sProperty ) m_eType = ptClear;
								else if ( _T("clip")  == m_sProperty ) m_eType = ptClip;
								else                                   m_eType = ptUnknown;

								break;
							}
						case 'o':
							{
								if      ( _T("color")             == m_sProperty ) m_eType = ptColor;
								else if ( _T("content")           == m_sProperty ) m_eType = ptContent;
								else if ( _T("counter-increment") == m_sProperty ) m_eType = ptCounterIncrement;
								else if ( _T("counter-reset")     == m_sProperty ) m_eType = ptCounterReset;
								else                                               m_eType = ptUnknown;

								break;
							}
						case 'u':
							{
								if ( _T("cursor") == m_sProperty ) m_eType = ptCursor;
								else                               m_eType = ptUnknown;

								break;
							}
						default:
							{
								m_eType = ptUnknown;
								break;
							}
						}
						break;
					}
				case 'd':
					{
						switch ( nChar2 )
						{
						case 'i':
							{
								if      ( _T("direction") == m_sProperty ) m_eType = ptDirection;
								else if ( _T("display")   == m_sProperty ) m_eType = ptDisplay;
								else                                       m_eType = ptUnknown;

								break;
							}
						default:
							{
								m_eType = ptUnknown;
								break;
							}
						}
						break;
					}
				case 'e':
					{
						if ( _T("empty-cells") == m_sProperty ) m_eType = ptEmptyCells;
						else                                    m_eType = ptUnknown;

						break;
					}
				case 'f':
					{
						switch( nChar2 )
						{
						case 'l':
							{
								if ( _T("float") == m_sProperty ) m_eType = ptFloat;
								else                              m_eType = ptUnknown;

								break;
							}
						case 'o':
							{
								if      ( _T("font")         == m_sProperty ) m_eType = ptFont;
								else if ( _T("font-family")  == m_sProperty ) m_eType = ptFontFamily;
								else if ( _T("font-size")    == m_sProperty ) m_eType = ptFontSize;
								else if ( _T("font-style")   == m_sProperty ) m_eType = ptFontStyle;
								else if ( _T("font-variant") == m_sProperty ) m_eType = ptFontVariant;
								else if ( _T("font-weight")  == m_sProperty ) m_eType = ptFontWeight;
								else                                          m_eType = ptUnknown;

								break;
							}
						default:
							{
								m_eType = ptUnknown;
								break;
							}
						}
						break;
					}
				case 'h':
					{
						if ( _T("height") == m_sProperty ) m_eType = ptHeight;
						else                               m_eType = ptUnknown;

						break;
					}
				case 'l':
					{
						switch( nChar2 )
						{
						case 'e':
							{
								if      ( _T("left")           == m_sProperty ) m_eType = ptLeft;
								else if ( _T("letter-spacing") == m_sProperty ) m_eType = ptLetterSpacing;
								else                                            m_eType = ptUnknown;

								break;
							}
						case 'i':
							{
								if      ( _T("line-height")         == m_sProperty ) m_eType = ptLineHeight;
								else if ( _T("list-style")          == m_sProperty ) m_eType = ptListStyle;
								else if ( _T("list-style-image")    == m_sProperty ) m_eType = ptListStyleImage;
								else if ( _T("list-style-position") == m_sProperty ) m_eType = ptListStylePosition;
								else if ( _T("list-style-type")     == m_sProperty ) m_eType = ptListStyleType;
								else                                                 m_eType = ptUnknown;

								break;
							}
						default:
							{
								m_eType = ptUnknown;
								break;
							}
						}
						break;
					}			
				case 'm':
					{
						switch( nChar2 )
						{
						case 'a':
							{
								if      ( _T("margin")        == m_sProperty ) m_eType = ptMargin;
								else if ( _T("margin-bottom") == m_sProperty ) m_eType = ptMarginBottom;
								else if ( _T("margin-left")   == m_sProperty ) m_eType = ptMarginLeft;
								else if ( _T("margin-right")  == m_sProperty ) m_eType = ptMarginRight;
								else if ( _T("margin-top")    == m_sProperty ) m_eType = ptMarginTop;
								else if ( _T("max-height")    == m_sProperty ) m_eType = ptMaxHeight;
								else if ( _T("max-width")     == m_sProperty ) m_eType = ptMaxWidth;
								else                                           m_eType = ptUnknown;

								break;
							}
						case 'i':
							{
								if      ( _T("min-height") == m_sProperty ) m_eType = ptMinHeight;
								else if ( _T("min-width")  == m_sProperty ) m_eType = ptMinWidth;
								else                                        m_eType = ptUnknown;

								break;
							}
						default:
							{
								m_eType = ptUnknown;
								break;
							}
						}
						break;
					}
				case 'o':
					{
						switch( nChar2 )
						{
						case 'r':
							{
								if      ( _T("orphans") == m_sProperty ) m_eType = ptOrphans;
								else                                     m_eType = ptUnknown;

								break;
							}
						case 'u':
							{
								if      ( _T("outline")       == m_sProperty ) m_eType = ptOutline;
								else if ( _T("outline-color") == m_sProperty ) m_eType = ptOutlineColor;
								else if ( _T("outline-style") == m_sProperty ) m_eType = ptOutlineStyle;
								else if ( _T("outline-width") == m_sProperty ) m_eType = ptOutlineWidth;
								else                                           m_eType = ptUnknown;

								break;
							}
						case 'v':
							{
								if      ( _T("overflow") == m_sProperty ) m_eType = ptOverflow;
								else                                      m_eType = ptUnknown;

								break;
							}
						default:
							{
								m_eType = ptUnknown;
								break;
							}
						}
						break;
					}
				case 'p':
					{
						switch( nChar2 )
						{
						case 'a':
							{
								if      ( _T("padding")           == m_sProperty ) m_eType = ptPadding;
								else if ( _T("padding-bottom")    == m_sProperty ) m_eType = ptPaddingBottom;
								else if ( _T("padding-left")      == m_sProperty ) m_eType = ptPaddingLeft;
								else if ( _T("padding-right")     == m_sProperty ) m_eType = ptPaddingRight;
								else if ( _T("padding-top")       == m_sProperty ) m_eType = ptPaddingTop;
								else if ( _T("page-break-after")  == m_sProperty ) m_eType = ptPageBreakAfter;
								else if ( _T("page-break-before") == m_sProperty ) m_eType = ptPageBreakBefore;
								else if ( _T("page-break-inside") == m_sProperty ) m_eType = ptPageBreakInside;
								else                                               m_eType = ptUnknown;

								break;
							}
						case 'o':
							{
								if      ( _T("position") == m_sProperty ) m_eType = ptPosition;
								else                                      m_eType = ptUnknown;

								break;
							}
						default:
							{
								m_eType = ptUnknown;
								break;
							}
						}
						break;
					}
				case 'q':
					{
						if ( _T("quotes") == m_sProperty ) m_eType = ptQuotes;
						else                               m_eType = ptUnknown;

						break;
					}
				case 'r':
					{
						if ( _T("right") == m_sProperty ) m_eType = ptRight;
						else                              m_eType = ptUnknown;

						break;
					}
				case 't':
					{
						switch( nChar2 )
						{
						case 'a':
							{
								if      ( _T("table-layout") == m_sProperty ) m_eType = ptTableLayout;
								else                                          m_eType = ptUnknown;

								break;
							}
						case 'e':
							{
								if      ( _T("text-align")      == m_sProperty ) m_eType = ptTextAlign;
								else if ( _T("text-decoration") == m_sProperty ) m_eType = ptTextDecoration;
								else if ( _T("text-indent")     == m_sProperty ) m_eType = ptTextIndent;
								else if ( _T("text-shadow")     == m_sProperty ) m_eType = ptTextShadow;
								else if ( _T("text-transform")  == m_sProperty ) m_eType = ptTextTransform;
								else                                             m_eType = ptUnknown;

								break;
							}
						case 'o':
							{
								if      ( _T("top") == m_sProperty ) m_eType = ptTop;
								else                                 m_eType = ptUnknown;

								break;
							}
						default:
							{
								m_eType = ptUnknown;
								break;
							}
						}
						break;
					}
				case 'u':
					{
						if   ( _T("unicode-bidi") == m_sProperty ) m_eType = ptUnicodeBidi;
						else                                       m_eType = ptUnknown;

						break;
					}
				case 'v':
					{
						switch( nChar2 )
						{
						case 'e':
							{
								if   ( _T("vertical-align") == m_sProperty ) m_eType = ptVerticalAlign;
								else                                         m_eType = ptUnknown;

								break;
							}
						case 'i':
							{
								if      ( _T("visibility") == m_sProperty ) m_eType = ptVisibility;
								else                                        m_eType = ptUnknown;

								break;
							}
						default:
							{
								m_eType = ptUnknown;
								break;
							}
						}
						break;
					}
				case 'w':
					{
						switch( nChar2 )
						{
						case 'h':
							{
								if   ( _T("white-space") == m_sProperty ) m_eType = ptWhiteSpace;
								else                                      m_eType = ptUnknown;

								break;
							}
						case 'i':
							{
								if      ( _T("widows") == m_sProperty ) m_eType = ptWidows;
								else if ( _T("width")  == m_sProperty ) m_eType = ptWidth;
								else                                    m_eType = ptUnknown;

								break;
							}
						case 'o':
							{
								if      ( _T("word-spacing") == m_sProperty ) m_eType = ptWordSpacing;
								else                                          m_eType = ptUnknown;

								break;
							}
						default:
							{
								m_eType = ptUnknown;
								break;
							}
						}
						break;
					}
				case 'z':
					{
						if   ( _T("z-index") == m_sProperty ) m_eType = ptZIndex;
						else                                  m_eType = ptUnknown;

						break;
					}
				default:
					{
						m_eType = ptUnknown; 
						break;
					}
				}
			}
		}

		void TransformValue()
		{
			// здесь мы парсим значения свойств CSS
			// Пока ничего не парсим, только преобразуем цвет

			if ( ptBackground     == m_eType || ptBackgroundColor  == m_eType || ptBorder            == m_eType ||
				ptBorder         == m_eType || ptBorderBottom     == m_eType || ptBorderBottomColor == m_eType ||
				ptBorderColor    == m_eType || ptBorderLeft       == m_eType || ptBorderLeftColor   == m_eType ||
				ptBorderRight    == m_eType || ptBorderRightColor == m_eType || ptBorderTop         == m_eType ||
				ptBorderTopColor == m_eType || ptOutline          == m_eType || ptOutlineColor      == m_eType ||
				ptColor          == m_eType )
			{
				m_sValue.Replace(_T("AliceBlue"),            _T("#F0F8FF"));
				m_sValue.Replace(_T("AntiqueWhite"),         _T("#FAEBD7"));
				m_sValue.Replace(_T("BlanchedAlmond"),       _T("#FFEBCD"));
				m_sValue.Replace(_T("BlueViolet"),           _T("#8A2BE2"));
				m_sValue.Replace(_T("BurlyWood"),            _T("#DEB887"));
				m_sValue.Replace(_T("CadetBlue"),            _T("#5F9EA0"));
				m_sValue.Replace(_T("Chartreuse"),           _T("#7FFF00"));
				m_sValue.Replace(_T("Chocolate"),            _T("#D2691E"));
				m_sValue.Replace(_T("CornFlowerBlue"),       _T("#6495ED"));
				m_sValue.Replace(_T("Cornsilk"),             _T("#FFF8DC"));
				m_sValue.Replace(_T("Crimson"),              _T("#DC143C"));
				m_sValue.Replace(_T("DarkBlue"),             _T("#00008B"));
				m_sValue.Replace(_T("DarkCyan"),             _T("#008B8B"));
				m_sValue.Replace(_T("DarkGoldenRod"),        _T("#B8860B"));
				m_sValue.Replace(_T("DarkGray"),             _T("#A9A9A9"));
				m_sValue.Replace(_T("DarkGreen"),            _T("#006400"));
				m_sValue.Replace(_T("DarkKhaki"),            _T("#BDB76B"));
				m_sValue.Replace(_T("DarkMagenta"),          _T("#8B008B"));

				m_sValue.Replace(_T("DarkOliveGreen"),       _T("#556B2F"));
				m_sValue.Replace(_T("DarkOrange"),           _T("#FF8C00"));
				m_sValue.Replace(_T("DarkOrchid"),           _T("#9932CC"));
				m_sValue.Replace(_T("DarkRed"),              _T("#8B0000"));
				m_sValue.Replace(_T("DarkSalmon"),           _T("#E9967A"));
				m_sValue.Replace(_T("DarkSeaGreen"),         _T("#8FBC8F"));
				m_sValue.Replace(_T("DarkSlateBlue"),        _T("#483D8B"));
				m_sValue.Replace(_T("DarkSlateGray"),        _T("#2F4F4F"));
				m_sValue.Replace(_T("DarkTurquoise"),        _T("#00CED1"));
				m_sValue.Replace(_T("DarkViolet"),           _T("#9400D3"));
				m_sValue.Replace(_T("DeepPink"),             _T("#FF1493"));
				m_sValue.Replace(_T("DeepSkyBlue"),          _T("#00BFFF"));
				m_sValue.Replace(_T("DimGray"),              _T("#696969"));
				m_sValue.Replace(_T("DodgerBlue"),           _T("#1E90FF"));
				m_sValue.Replace(_T("FireBrick"),            _T("#B22222"));
				m_sValue.Replace(_T("FloralWhite"),          _T("#FFFAF0"));
				m_sValue.Replace(_T("ForestGreen"),          _T("#228B22"));
				m_sValue.Replace(_T("Fuchsia"),              _T("#FF00FF"));
				m_sValue.Replace(_T("Gainsboro"),            _T("#DCDCDC"));
				m_sValue.Replace(_T("GhostWhite"),           _T("#F8F8FF"));
				m_sValue.Replace(_T("GreenYellow"),          _T("#ADFF2F"));
				m_sValue.Replace(_T("HoneyDew"),             _T("#F0FFF0"));
				m_sValue.Replace(_T("HotPink"),              _T("#FF69B4"));
				m_sValue.Replace(_T("IndianRed"),            _T("#CD5C5C"));

				m_sValue.Replace(_T("LavenderBlush"),        _T("#FFF0F5"));
				m_sValue.Replace(_T("LawnGreen"),            _T("#7CFC00"));
				m_sValue.Replace(_T("LemonChiffon"),         _T("#FFFACD"));
				m_sValue.Replace(_T("LightBlue"),            _T("#ADD8E6"));
				m_sValue.Replace(_T("LightCoral"),           _T("#F08080"));
				m_sValue.Replace(_T("LightCyan"),            _T("#E0FFFF"));
				m_sValue.Replace(_T("LightGoldenrodYellow"), _T("#FAFAD2"));
				m_sValue.Replace(_T("LightGray"),            _T("#D3D3D3"));
				m_sValue.Replace(_T("LightGreen"),           _T("#90EE90"));
				m_sValue.Replace(_T("LightPink"),            _T("#FFB6C1"));
				m_sValue.Replace(_T("LightSalmon"),          _T("#FFA07A"));
				m_sValue.Replace(_T("LightSeaGreen"),        _T("#20B2AA"));
				m_sValue.Replace(_T("LightSkyBlue"),         _T("#87CEFA"));
				m_sValue.Replace(_T("LightSlateGray"),       _T("#778899"));
				m_sValue.Replace(_T("LightSteelBlue"),       _T("#B0C4DE"));
				m_sValue.Replace(_T("LightYellow"),          _T("#FFFFE0"));
				m_sValue.Replace(_T("LimeGreen"),            _T("#32CD32"));
				m_sValue.Replace(_T("MediumAquaMarine"),     _T("#66CDAA"));
				m_sValue.Replace(_T("MediumBlue"),           _T("#0000CD"));
				m_sValue.Replace(_T("MediumOrchid"),         _T("#BA55D3"));

				m_sValue.Replace(_T("MediumPurple"),         _T("#9370D8"));
				m_sValue.Replace(_T("MediumSeaGreen"),       _T("#3CB371"));
				m_sValue.Replace(_T("MediumSlateBlue"),      _T("#7B68EE"));
				m_sValue.Replace(_T("MediumSpringGreen"),    _T("#00FA9A"));
				m_sValue.Replace(_T("MediumTurquoise"),      _T("#48D1CC"));
				m_sValue.Replace(_T("MediumVioletRed"),      _T("#C71585"));
				m_sValue.Replace(_T("MidnightBlue"),         _T("#191970"));
				m_sValue.Replace(_T("MintCream"),            _T("#F5FFFA"));
				m_sValue.Replace(_T("MistyRose"),            _T("#FFE4E1"));
				m_sValue.Replace(_T("Moccasin"),             _T("#BA55D3"));
				m_sValue.Replace(_T("NavajoWhite"),          _T("#FFDEAD"));
				m_sValue.Replace(_T("OldLace"),              _T("#FDF5E6"));
				m_sValue.Replace(_T("OliveDrab"),            _T("#688E23"));
				m_sValue.Replace(_T("OrangeRed"),            _T("#FF4500"));
				m_sValue.Replace(_T("PaleGoldenRod"),        _T("#EEE8AA"));
				m_sValue.Replace(_T("PaleGreen"),            _T("#98FB98"));
				m_sValue.Replace(_T("PaleTurquoise"),        _T("#AFEEEE"));
				m_sValue.Replace(_T("PaleVioletRed"),        _T("#D87093"));
				m_sValue.Replace(_T("PapayaWhip"),           _T("#FFEFD5"));
				m_sValue.Replace(_T("PeachPuff"),            _T("#FFDAB9"));
				m_sValue.Replace(_T("PowderBlue"),           _T("#B0E0E6"));

				m_sValue.Replace(_T("RosyBrown"),            _T("#BC8F8F"));
				m_sValue.Replace(_T("RoyalBlue"),            _T("#4169E1"));
				m_sValue.Replace(_T("SaddleBrown"),          _T("#8B4513"));
				m_sValue.Replace(_T("SandyBrown"),           _T("#F4A460"));
				m_sValue.Replace(_T("SeaShell"),             _T("#FFF5EE"));
				m_sValue.Replace(_T("SkyBlue"),              _T("#87CEEB"));
				m_sValue.Replace(_T("SlateGray"),            _T("#708090"));
				m_sValue.Replace(_T("SteelBlue"),            _T("#4682B4"));
				m_sValue.Replace(_T("WhiteSmoke"),           _T("#F5F5F5"));
				m_sValue.Replace(_T("YellowGreen"),          _T("#9ACD32"));

				m_sValue.Replace(_T("AquaMarine"),           _T("#7FFFD4"));
				m_sValue.Replace(_T("GoldenRod"),            _T("#DAA520"));
				m_sValue.Replace(_T("SeaGreen"),             _T("#2E8B57"));
				m_sValue.Replace(_T("SlateBlue"),            _T("#6A5ACD"));
				m_sValue.Replace(_T("SpringGreen"),          _T("#00FF7F"));

				m_sValue.Replace(_T("Aqua"),                 _T("#00FFFF"));
				m_sValue.Replace(_T("Azure"),                _T("#F0FFFF"));
				m_sValue.Replace(_T("Beige"),                _T("#F5F5DC"));
				m_sValue.Replace(_T("Bisque"),               _T("#FFE4C4"));
				m_sValue.Replace(_T("Black"),                _T("#000000"));
				m_sValue.Replace(_T("Blue"),                 _T("#0000FF"));
				m_sValue.Replace(_T("Brown"),                _T("#A52A2A"));
				m_sValue.Replace(_T("Coral"),                _T("#FF7F50"));
				m_sValue.Replace(_T("Cyan"),                 _T("#00FFFF"));
				m_sValue.Replace(_T("Gold"),                 _T("#FFD700"));
				m_sValue.Replace(_T("Gray"),                 _T("#808080"));
				m_sValue.Replace(_T("Green"),                _T("#008000"));
				m_sValue.Replace(_T("Indigo"),               _T("#4B0082"));
				m_sValue.Replace(_T("Ivory"),                _T("#FFFFF0"));
				m_sValue.Replace(_T("Khaki"),                _T("#F0E68C"));
				m_sValue.Replace(_T("Lavender"),             _T("#E6E6FA"));
				m_sValue.Replace(_T("Lime"),                 _T("#00FF00"));
				m_sValue.Replace(_T("Linen"),                _T("#FAF0E6"));
				m_sValue.Replace(_T("Magenta"),              _T("#FF00FF"));
				m_sValue.Replace(_T("Maroon"),               _T("#800000"));
				m_sValue.Replace(_T("Navy"),                 _T("#000080"));
				m_sValue.Replace(_T("Olive"),                _T("#808000"));
				m_sValue.Replace(_T("Orange"),               _T("#FFA500"));
				m_sValue.Replace(_T("Orchid"),               _T("#DA70D6"));
				m_sValue.Replace(_T("Peru"),                 _T("#CD853F"));
				m_sValue.Replace(_T("Pink"),                 _T("#FFC0CB"));
				m_sValue.Replace(_T("Plum"),                 _T("#DDA0DD"));
				m_sValue.Replace(_T("Purple"),               _T("#800080"));
				m_sValue.Replace(_T("Red"),                  _T("#FF0000"));
				m_sValue.Replace(_T("Salmon"),               _T("#FA8072"));
				m_sValue.Replace(_T("Sienna"),               _T("#A0522D"));
				m_sValue.Replace(_T("Silver"),               _T("#C0C0C0"));
				m_sValue.Replace(_T("Snow"),                 _T("#FFFAFA"));
				m_sValue.Replace(_T("Tan"),                  _T("#D2B48C"));
				m_sValue.Replace(_T("Teal"),                 _T("#008080"));
				m_sValue.Replace(_T("Thistle"),              _T("#D8BFD8"));
				m_sValue.Replace(_T("Tomato"),               _T("#FF6347"));
				m_sValue.Replace(_T("Turquoise"),            _T("#40E0D0"));
				m_sValue.Replace(_T("Violet"),               _T("#EE82EE"));
				m_sValue.Replace(_T("Wheat"),                _T("#F5DEB3"));
				m_sValue.Replace(_T("White"),                _T("#FFFFFF"));
				m_sValue.Replace(_T("Yellow"),               _T("#FFFF00"));
			}

		}
	private:


		PropertyType  m_eType;

		CString       m_sProperty;
		CString       m_sValue;

	};



	//------------------------------------------------------------------------------------------
	// CHtmlCSS
	//------------------------------------------------------------------------------------------

	class CHtmlCSS
	{
	public:

		CHtmlCSS()
		{
			m_bValid  = false;
			m_nWeight = 0;
		}


		CHtmlCSS(CString sCssSelector, CString sCssProps)
		{
			m_nWeight = 0;

			m_sCssSelector   = sCssSelector;
			m_sCssProperties = sCssProps;

			FormatSelectorsString (m_sCssSelector);

			/*
			// delete spaces, enters, newline chars from begin and end of selector
			for (int nChar = 0; nChar < m_sCssSelector.GetLength(); ++nChar)
			{
				if (!(m_sCssSelector[nChar] == ' ' || m_sCssSelector[nChar] == '\n' || m_sCssSelector[nChar] == '\r'))
				{
					if (0 != nChar)
						m_sCssSelector.Delete (0, nChar);
					break;
				}
			}
			for (int nChar = m_sCssSelector.GetLength()-1; nChar >= 0; --nChar)
			{
				if (!(m_sCssSelector[nChar] == ' ' || m_sCssSelector[nChar] == '\n' || m_sCssSelector[nChar] == '\r'))
				{
					if (nChar < (m_sCssSelector.GetLength() - 1))
						m_sCssSelector.Delete (nChar + 1, m_sCssSelector.GetLength() - nChar - 1);
					break;
				}
			}
			*/

			m_bValid = ParseSelector();
			m_bValid = ParseProperties();
		}

		const CString &get_Selector() const
		{
			return m_sCssSelector;
		}

		const CString &get_RawProperties() const
		{
			return m_sCssProperties;
		}
		CString get_Properties() const
		{
			CString sProperties;
			for ( int nIndex = 0; nIndex < m_aProperties.GetSize(); nIndex++ )
			{
				sProperties += m_aProperties.GetData()[nIndex].get_Property();
				sProperties += _T(":");
				sProperties += m_aProperties.GetData()[nIndex].get_Value();
				sProperties += _T(";");
			}
			return sProperties;
		}

		const int &get_Weight() const
		{
			return m_nWeight;
		}

		CHtmlCSSProperty *get_Property(PropertyType eType) const
		{
			for ( int nIndex = 0; nIndex < m_aProperties.GetSize(); nIndex++ )
			{
				if ( eType == m_aProperties[nIndex].get_Type() )
					return (m_aProperties.GetData() + nIndex);
			}

			return NULL;
		}
		int get_PropertiesCount() const
		{
			return m_aProperties.GetSize();
		}
		CHtmlCSSProperty *get_Property(int nIndex) const
		{
			if ( nIndex < 0 || nIndex >= m_aProperties.GetSize() )
				return NULL;

			return (m_aProperties.GetData() + nIndex);
		}


		void add_Property(CHtmlCSSProperty& oProperty)
		{
			m_aProperties.Add( oProperty );
		}
		~CHtmlCSS()
		{
		}

	private:

		static void DeleteSpacesAroundChar (TCHAR aChar, CString& aSelectors)
		{
			int nCharIndex = -1;
			nCharIndex = aSelectors.Find (aChar);
			while (-1 != nCharIndex)
			{
				// look at the right of character
				int nSpaceIndex = nCharIndex + 1;
				const int nCharCount = aSelectors.GetLength();
				while ((nSpaceIndex < nCharCount) && (aSelectors[nSpaceIndex] == ' '))
				{
					nSpaceIndex++;
				}
				if (nSpaceIndex > nCharIndex + 1)
					aSelectors.Delete (nCharIndex + 1, (nSpaceIndex - 1) - nCharIndex);

				// look at the left of character
				nSpaceIndex = nCharIndex - 1;

				while ((nSpaceIndex >= 0) && (aSelectors[nSpaceIndex] == ' '))
				{
					nSpaceIndex--;
				}
				if (nSpaceIndex + 1 < nCharIndex)
				{
					aSelectors.Delete (nSpaceIndex + 1, nCharIndex - (nSpaceIndex + 1));
					nCharIndex = nSpaceIndex + 1;
				}
				++nCharIndex;	// skip this char

				nCharIndex = aSelectors.Find (aChar, nCharIndex);
			}
		}
		static void FormatSelectorsString (CString& aSelectors)
		{
			aSelectors = aSelectors.Trim (_T(" \r\n"));

			// remove spaces around '>' and '+'
			DeleteSpacesAroundChar ('>', aSelectors);
			DeleteSpacesAroundChar ('+', aSelectors);
		}

		bool ParseSelector()
		{
			int nPos = 0;
			CString sTemp;

			while ( nPos < m_sCssSelector.GetLength() )
			{
				int nChar = m_sCssSelector.GetAt( nPos );
				if ( ' ' == nChar )
				{
					if ( _T("") != sTemp )
					{
						if ( (_T(">") == sTemp || _T("+") == sTemp) && m_aParentsSelectors.GetSize() > 0 )
						{
							(m_aParentsSelectors.GetData()[m_aParentsSelectors.GetSize() - 1]).set_Operation( sTemp );
						}
						else
						{
							CHtmlCSSSelector oSelector( sTemp );
							if ( oSelector.CheckValidation() )
								m_aParentsSelectors.Add( oSelector );
						}

						sTemp.Empty();
					}

					nPos++;
					continue;
				}
				else if ( '>' == nChar || '+' == nChar )
				{
					if ( _T("") != sTemp )
					{
						CHtmlCSSSelector oSelector( sTemp );
						if ( oSelector.CheckValidation() )
							m_aParentsSelectors.Add( oSelector );
					}

					if ( m_aParentsSelectors.GetSize() > 0 )
					{
						CString sOp; sOp.AppendChar( nChar );
						(m_aParentsSelectors.GetData()[m_aParentsSelectors.GetSize() - 1]).set_Operation( sOp );
					}
					sTemp.Empty();
					nPos++;
					continue;
				}
				else
				{
					sTemp.AppendChar( nChar );
					nPos++;
				}
			}

			if ( _T("") != sTemp )
			{
				if ( (_T(">") == sTemp || _T("+") == sTemp) && m_aParentsSelectors.GetSize() > 0 )
				{
					(m_aParentsSelectors.GetData()[m_aParentsSelectors.GetSize() - 1]).set_Operation( sTemp );
				}
				else
				{
					CHtmlCSSSelector oSelector( sTemp );
					if ( oSelector.CheckValidation() )
						m_aParentsSelectors.Add( oSelector );
				}

				sTemp.Empty();
			}

			// Последний элемент и есть идентификатор текущего стиля
			int nSize = m_aParentsSelectors.GetSize();

			for ( int nIndex = 0; nIndex < nSize; nIndex++ )
			{
				m_nWeight += m_aParentsSelectors.GetData()[nIndex].get_Weight();
			}

			if ( nSize > 0 )
			{
				m_oSelector = m_aParentsSelectors.GetData()[nSize - 1];
				m_aParentsSelectors.RemoveAt( nSize - 1 );
			}

			return true;
		}

		bool ParseProperties()
		{
			CString sTemp = m_sCssProperties;
			while ( sTemp.GetLength() > 0 )
			{
				int nPos = sTemp.Find( ';' );
				if ( -1 == nPos )
				{
					CHtmlCSSProperty oProperty(sTemp);
					if ( ptUnknown != oProperty.get_Type() )
						m_aProperties.Add( oProperty );

					sTemp.Empty();
					continue;
				}
				else
				{
					CHtmlCSSProperty oProperty( sTemp.Mid( 0, nPos ) );
					if ( ptUnknown != oProperty.get_Type() )
						m_aProperties.Add( oProperty );

					sTemp = sTemp.Mid( nPos + 1, sTemp.GetLength() - nPos - 1 );
				}
			}

			return true;
		}
	private:


		bool                            m_bValid;            // Корректный ли CSS?
		CString                         m_sCssSelector;      // Строка с идентификатором Css
		CString                         m_sCssProperties;    // Строка с элементами стиля

		CHtmlCSSSelector                m_oSelector;         // Селектор данного стиля
		CSimpleArray<CHtmlCSSSelector>  m_aParentsSelectors; // Селекторы для родительских тегов

		CSimpleArray<CHtmlCSSProperty>  m_aProperties;

		int                             m_nWeight;           // Вес данного селектора

	};


	//------------------------------------------------------------------------------------------
	// CHtmlCSSList
	//------------------------------------------------------------------------------------------

	class CHtmlCSSList
	{
	public:

		CHtmlCSSList()
		{
		}

		CHtmlCSSList(CString &sBuffer)
		{
			Parse( sBuffer );
		}

		void Parse(CString &sBuffer)
		{
			int nEndPos   = 0;
			int nStartPos = 0;
			while ( -1 != ( nEndPos = sBuffer.Find( _T("}"), nStartPos ) ) )
			{
				CString sCurCss = sBuffer.Mid( nStartPos, nEndPos - nStartPos + 1 );

				int nTempPos = 0;
				if ( -1 == ( nTempPos = sCurCss.Find( _T("{") ) ) )
				{
					nStartPos = nEndPos + 1;
					continue;
				}

				CString sCurCssId = sCurCss.Mid( 0, nTempPos );
				CString sCurCssPr = sCurCss.Mid( nTempPos + 1, sCurCss.GetLength() - nTempPos - 2 );

				if ( -1 == sCurCssId.Find( _T(",") ) )
				{
					CHtmlCSS oCss( sCurCssId, sCurCssPr );
					m_aCssList.Add( oCss );			
				}
				else
				{
					int nTempPos1 = 0;
					int nTempPos2 = 0;
					while ( -1 != ( nTempPos2 = sCurCssId.Find( _T(","), nTempPos1 ) ) )
					{
						CString sId = sCurCssId.Mid( nTempPos1, nTempPos2 - nTempPos1 );

						CHtmlCSS oCss( sId, sCurCssPr );
						m_aCssList.Add( oCss );

						nTempPos1 = nTempPos2 + 1;
					}

					CString sId = sCurCssId.Mid( nTempPos1, sCurCssId.GetLength() - nTempPos1 );
					CHtmlCSS oCss( sId, sCurCssPr );
					m_aCssList.Add( oCss );
				}


				nStartPos = nEndPos + 1;
			}
		}


		void Add(CHtmlCSS &oCSS)
		{
			m_aCssList.Add( oCSS );
		}
		//CString get_ElementCSS(htmlayout::dom::element &oHtmlNode)
		CString get_ElementCSS(CHTMLTagWrapper*   pHtmlNode)
		{
			CSimpleArray<CHtmlCSS> aCSSs;

			for ( int nIndex = 0; nIndex < m_aCssList.GetSize(); nIndex++ )
			{
				CHtmlCSS oCss = m_aCssList.GetData()[nIndex];

				USES_CONVERSION;
				if (pHtmlNode->Test (oCss.get_Selector()))
				{
					aCSSs.Add (oCss);
				}
			}

			int nSize = aCSSs.GetSize();
			if ( 0 == nSize )
				return _T("");
			else if ( 1 == nSize )
				return aCSSs.GetData()[0].get_RawProperties();
			else
			{
				// Сортируем по весу. Стили с меньшим весом должны стоять раньше.
				bool bFlag = true;

				while ( bFlag )
				{
					bFlag = false;
					for ( int nIndex = 0; nIndex < nSize - 1; nIndex++ )
					{
						if ( aCSSs.GetData()[nIndex].get_Weight() > aCSSs.GetData()[nIndex + 1].get_Weight() )
						{
							CHtmlCSS oCss(aCSSs.GetData()[nIndex]);
							aCSSs.SetAtIndex( nIndex, aCSSs.GetData()[nIndex + 1] );
							aCSSs.SetAtIndex( nIndex + 1, oCss );
						}
					}
				}

				CString sResult;
				for ( int nIndex = 0; nIndex < nSize; nIndex++ )
				{
					sResult += aCSSs.GetData()[nIndex].get_RawProperties();
				}

				return sResult;
			}

		}
		//CSimpleArray<CHtmlCSS*> get_CSS(htmlayout::dom::element &oHtmlNode)
		CSimpleArray<CHtmlCSS*> get_CSS(CHTMLTagWrapper* pHtmlNode)
		{
			CSimpleArray<CHtmlCSS*> aCSSs;

			for ( int nIndex = 0; nIndex < m_aCssList.GetSize(); nIndex++ )
			{
				CHtmlCSS *pCss = (m_aCssList.GetData() + nIndex);

				USES_CONVERSION;
				if (pHtmlNode->Test (((CString &)pCss->get_Selector()).GetBuffer()))
					aCSSs.Add( pCss );
			}

			int nSize = aCSSs.GetSize();
			if ( 0 == nSize || 1 == nSize )
				return aCSSs;
			else
			{
				// Сортируем по весу. Стили с меньшим весом должны стоять раньше.
				bool bFlag = true;

				while ( bFlag )
				{
					bFlag = false;
					for ( int nIndex = 0; nIndex < nSize - 1; nIndex++ )
					{
						if ( aCSSs.GetData()[nIndex]->get_Weight() > aCSSs.GetData()[nIndex + 1]->get_Weight() )
						{
							CHtmlCSS *pCss = aCSSs.GetData()[nIndex];
							aCSSs.SetAtIndex( nIndex, aCSSs.GetData()[nIndex + 1] );
							aCSSs.SetAtIndex( nIndex + 1, pCss );
						}
					}
				}

				return aCSSs;
			}

		}
		~CHtmlCSSList()
		{
		}

	private:

		CSimpleArray<CHtmlCSS> m_aCssList;

	};

};

#endif /* _CHTML_CSS_H */