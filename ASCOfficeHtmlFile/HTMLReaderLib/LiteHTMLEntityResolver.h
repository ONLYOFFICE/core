#pragma once
#include "../ASCOfficeHtmlFile/src/stdafx.h"
#include "LiteHTMLCommon.h"


#pragma warning(push, 4)


class CLiteHTMLEntityResolver
{
private:
	class CCharEntityRefs : public CAtlMap<CString, TCHAR>
	{
	public:
		CCharEntityRefs() : CAtlMap<CString, TCHAR>(55 /*iso8859-1*/ + 38/*math*/ + 52/*greek*/ + 50/*other*/)
		{
			/** C0 Controls and Basic Latin */
			(*this)[_T("quot")]		= _T('\x22');
			(*this)[_T("amp")]		= _T('\x26');
			(*this)[_T("apos")]		= _T('\x27');
			(*this)[_T("lt")]		= _T('\x3C');
			(*this)[_T("gt")]		= _T('\x3E');
			/** ISO 8859-1 (Latin-1) characters */
			(*this)[_T("nbsp")]		= _T('\xA0');
			(*this)[_T("iexcl")]	= _T('\xA1');
			(*this)[_T("cent")]		= _T('\xA2');
			(*this)[_T("pound")]	= _T('\xA3');
			(*this)[_T("current")]	= _T('\xA4');
			(*this)[_T("yen")]		= _T('\xA5');
			(*this)[_T("brvbar")]	= _T('\xA6');
			(*this)[_T("sect")]		= _T('\xA7');
			(*this)[_T("uml")]		= _T('\xA8');
			(*this)[_T("copy")]		= _T('\xA9');
			(*this)[_T("ordf")]		= _T('\xAA');
			(*this)[_T("laquo")]	= _T('\xAB');
			(*this)[_T("not")]		= _T('\xAC');
			(*this)[_T("shy")]		= _T('\xAD');
			(*this)[_T("reg")]		= _T('\xAE');
			(*this)[_T("macr")]		= _T('\xAF');
			(*this)[_T("deg")]		= _T('\xB0');
			(*this)[_T("plusmn")]	= _T('\xB1');
			(*this)[_T("sup2")]		= _T('\xB2');
			(*this)[_T("sup3")]		= _T('\xB3');
			(*this)[_T("acute")]	= _T('\xB4');
			(*this)[_T("micro")]	= _T('\xB5');
			(*this)[_T("para")]		= _T('\xB6');
			(*this)[_T("middot")]	= _T('\xB7');
			(*this)[_T("cedil")]	= _T('\xB8');
			(*this)[_T("sup1")]		= _T('\xB9');
			(*this)[_T("ordm")]		= _T('\xBA');
			(*this)[_T("raquo")]	= _T('\xBB');
			(*this)[_T("frac14")]	= _T('\xBC');
			(*this)[_T("frac12")]	= _T('\xBD');
			(*this)[_T("frac34")]	= _T('\xBE');
			(*this)[_T("iquest")]	= _T('\xBF');
			(*this)[_T("Agrave")]	= _T('\xC0');
			(*this)[_T("Aacute")]	= _T('\xC1');
			(*this)[_T("Acirc")]	= _T('\xC2');
			(*this)[_T("Atilde")]	= _T('\xC3');
			(*this)[_T("Auml")]		= _T('\xC4');
			(*this)[_T("Aring")]	= _T('\xC5');
			(*this)[_T("AElig")]	= _T('\xC6');
			(*this)[_T("Ccedil")]	= _T('\xC7');
			(*this)[_T("Egrave")]	= _T('\xC8');
			(*this)[_T("Eacute")]	= _T('\xC9');
			(*this)[_T("Ecirc")]	= _T('\xCA');
			(*this)[_T("Euml")]		= _T('\xCB');
			(*this)[_T("Igrave")]	= _T('\xCC');
			(*this)[_T("Iacute")]	= _T('\xCD');
			(*this)[_T("Icirc")]	= _T('\xCE');
			(*this)[_T("Iuml")]		= _T('\xCF');
			(*this)[_T("ETH")]		= _T('\xD0');
			(*this)[_T("Ntilde")]	= _T('\xD1');
			(*this)[_T("Ograve")]	= _T('\xD2');
			(*this)[_T("Oacute")]	= _T('\xD3');
			(*this)[_T("Ocirc")]	= _T('\xD4');
			(*this)[_T("Otilde")]	= _T('\xD5');
			(*this)[_T("Ouml")]		= _T('\xD6');
			(*this)[_T("times")]	= _T('\xD7');
			(*this)[_T("Oslash")]	= _T('\xD8');
			(*this)[_T("Ugrave")]	= _T('\xD9');
			(*this)[_T("Uacute")]	= _T('\xDA');
			(*this)[_T("Ucirc")]	= _T('\xDB');
			(*this)[_T("Uuml")]		= _T('\xDC');
			(*this)[_T("Yacute")]	= _T('\xDD');
			(*this)[_T("THORN")]	= _T('\xDE');
			(*this)[_T("szlig")]	= _T('\xDF');
			(*this)[_T("agrave")]	= _T('\xE0');
			(*this)[_T("aacute")]	= _T('\xE1');
			(*this)[_T("acirc")]	= _T('\xE2');
			(*this)[_T("atilde")]	= _T('\xE3');
			(*this)[_T("auml")]		= _T('\xE4');
			(*this)[_T("aring")]	= _T('\xE5');
			(*this)[_T("aelig")]	= _T('\xE6');
			(*this)[_T("ccedil")]	= _T('\xE7');
			(*this)[_T("egrave")]	= _T('\xE8');
			(*this)[_T("eacute")]	= _T('\xE9');
			(*this)[_T("ecirc")]	= _T('\xEA');
			(*this)[_T("euml")]		= _T('\xEB');
			(*this)[_T("igrave")]	= _T('\xEC');
			(*this)[_T("iacute")]	= _T('\xED');
			(*this)[_T("icirc")]	= _T('\xEE');
			(*this)[_T("iuml")]		= _T('\xEF');
			(*this)[_T("eth")]		= _T('\xF0');
			(*this)[_T("ntilde")]	= _T('\xF1');
			(*this)[_T("ograve")]	= _T('\xF2');
			(*this)[_T("oacute")]	= _T('\xF3');
			(*this)[_T("ocirc")]	= _T('\xF4');
			(*this)[_T("otilde")]	= _T('\xF5');
			(*this)[_T("ouml")]		= _T('\xF6');
			(*this)[_T("divide")]	= _T('\xF7');
			(*this)[_T("oslash")]	= _T('\xF8');
			(*this)[_T("ugrave")]	= _T('\xF9');
			(*this)[_T("uacute")]	= _T('\xFA');
			(*this)[_T("ucirc")]	= _T('\xFB');
			(*this)[_T("uuml")]		= _T('\xFC');
			(*this)[_T("yacute")]	= _T('\xFD');
			(*this)[_T("thorn")]	= _T('\xFE');
			(*this)[_T("yuml")]		= _T('\xFF');

			/** Math Symbols */
			(*this)[_T("forall")]	= _T('\x2200');		// for all
			(*this)[_T("part")]		= _T('\x2202');		// part
			(*this)[_T("exist")]	= _T('\x2203');		// exist
			(*this)[_T("empty")]	= _T('\x2205');		// empty
			(*this)[_T("nabla")]	= _T('\x2207');		// nabla
			(*this)[_T("isin")]		= _T('\x2208');		// isis
			(*this)[_T("notin")]	= _T('\x2209');		// notin
			(*this)[_T("ni")]		= _T('\x220B');		// ni
			(*this)[_T("prod")]		= _T('\x220F');		// prod
			(*this)[_T("sum")]		= _T('\x2211');		// sum
			(*this)[_T("minus")]	= _T('\x2212');		// minus
			(*this)[_T("lowast")]	= _T('\x2217');		// lowast
			(*this)[_T("radic")]	= _T('\x221A');		// square root
			(*this)[_T("prop")]		= _T('\x221D');		// proportional to
			(*this)[_T("ifin")]		= _T('\x221E');		// infinity
			(*this)[_T("ang")]		= _T('\x2220');		// angle
			(*this)[_T("and")]		= _T('\x2227');		// and
			(*this)[_T("or")]		= _T('\x2228');		// or
			(*this)[_T("cap")]		= _T('\x2229');		// cap
			(*this)[_T("cup")]		= _T('\x222A');		// cup
			(*this)[_T("int")]		= _T('\x222B');		// integral
			(*this)[_T("there4")]	= _T('\x2234');		// therefore
			(*this)[_T("sim")]		= _T('\x223C');		// similar to
			(*this)[_T("cong")]		= _T('\x2245');		// congruent to
			(*this)[_T("asymp")]	= _T('\x2248');		// almost equal
			(*this)[_T("ne")]		= _T('\x2260');		// not equal
			(*this)[_T("equiv")]	= _T('\x2261');		// equivalent
			(*this)[_T("le")]		= _T('\x2264');		// less or equal
			(*this)[_T("ge")]		= _T('\x2265');		// greater or equal
			(*this)[_T("sub")]		= _T('\x2282');		// subset of
			(*this)[_T("sup")]		= _T('\x2283');		// superset of
			(*this)[_T("nsub")]		= _T('\x2284');		// not subset of
			(*this)[_T("sube")]		= _T('\x2286');		// subset or equal
			(*this)[_T("supe")]		= _T('\x2287');		// superset or equal
			(*this)[_T("oplus")]	= _T('\x2295');		// circled plus
			(*this)[_T("otimes")]	= _T('\x2297');		// circled times
			(*this)[_T("perp")]		= _T('\x22A5');		// perpendicular
			(*this)[_T("sdot")]		= _T('\x22C5');		// dot operator

			/** Greek Letters */
			(*this)[_T("Alpha")]		= _T('\x0391');		// Alpha
			(*this)[_T("Beta")]			= _T('\x0392');		// Beta
			(*this)[_T("Gamma")]		= _T('\x0393');		// Gamma
			(*this)[_T("Delta")]		= _T('\x0394');		// Delta
			(*this)[_T("Epsilon")]		= _T('\x0395');		// Epsilon
			(*this)[_T("Zeta")]			= _T('\x0396');		// Zeta
			(*this)[_T("Eta")]			= _T('\x0397');		// Eta
			(*this)[_T("Theta")]		= _T('\x0398');		// Theta
			(*this)[_T("Iota")]			= _T('\x0399');		// Iota
			(*this)[_T("Kappa")]		= _T('\x039A');		// Kappa
			(*this)[_T("Lambda")]		= _T('\x039B');		// Lambda
			(*this)[_T("Mu")]			= _T('\x039C');		// Mu
			(*this)[_T("Nu")]			= _T('\x039D');		// Nu
			(*this)[_T("Xi")]			= _T('\x039E');		// Xi
			(*this)[_T("Omicron")]		= _T('\x039F');		// Omicron
			(*this)[_T("Pi")]			= _T('\x03A0');		// Pi
			(*this)[_T("Rho")]			= _T('\x03A1');		// Rho
			//(*this)[_T("sigmaf")]			= _T('\x03A2');		// Sigmaf - undefined by HTML
			(*this)[_T("Sigma")]		= _T('\x03A3');		// Sigma
			(*this)[_T("Tau")]			= _T('\x03A4');		// Tau
			(*this)[_T("Upsilon")]		= _T('\x03A5');		// Upsilon
			(*this)[_T("Phi")]			= _T('\x03A6');		// Phi
			(*this)[_T("Chi")]			= _T('\x03A7');		// Chi
			(*this)[_T("Psi")]			= _T('\x03A8');		// Psi
			(*this)[_T("Omega")]		= _T('\x03A9');		// Omega

			(*this)[_T("alpha")]		= _T('\x03B1');		// alpha
			(*this)[_T("beta")]			= _T('\x03B2');		// beta
			(*this)[_T("gamma")]		= _T('\x03B3');		// gamma
			(*this)[_T("delta")]		= _T('\x03B4');		// delta
			(*this)[_T("epsilon")]		= _T('\x03B5');		// epsilon
			(*this)[_T("zeta")]			= _T('\x03B6');		// zeta
			(*this)[_T("eta")]			= _T('\x03B7');		// eta
			(*this)[_T("theta")]		= _T('\x03B8');		// theta
			(*this)[_T("iota")]			= _T('\x03B9');		// iota
			(*this)[_T("kappa")]		= _T('\x03BA');		// kappa
			(*this)[_T("lambda")]		= _T('\x03BB');		// lambda
			(*this)[_T("mu")]			= _T('\x03BC');		// mu
			(*this)[_T("nu")]			= _T('\x03BD');		// nu
			(*this)[_T("xi")]			= _T('\x03BE');		// xi
			(*this)[_T("omicron")]		= _T('\x03BF');		// omicron
			(*this)[_T("pi")]			= _T('\x03C0');		// pi
			(*this)[_T("rho")]			= _T('\x03C1');		// rho
			(*this)[_T("sigmaf")]		= _T('\x03C2');		// sigmaf
			(*this)[_T("sigma")]		= _T('\x03C3');		// sigma
			(*this)[_T("tau")]			= _T('\x03C4');		// tau
			(*this)[_T("upsilon")]		= _T('\x03C5');		// upsilon
			(*this)[_T("phi")]			= _T('\x03C6');		// phi
			(*this)[_T("chi")]			= _T('\x03C7');		// chi
			(*this)[_T("psi")]			= _T('\x03C8');		// psi
			(*this)[_T("omega")]		= _T('\x03C9');		// omega

			(*this)[_T("thetasym")]		= _T('\x03D1');		// theta symbol
			(*this)[_T("upsih")]		= _T('\x03D2');		// upsilon symbol
			(*this)[_T("piv")]			= _T('\x03D6');		// pi symbol

			/** Other Entities */
			(*this)[_T("OElig")]		= _T('\x0152');		// capital ligature OE
			(*this)[_T("oelig")]		= _T('\x0153');		// small ligature OE
			(*this)[_T("Scaron")]		= _T('\x0160');		// capital S with caron
			(*this)[_T("scaron")]		= _T('\x0161');		// small S with caron
			(*this)[_T("Yuml")]			= _T('\x0178');		// capital Y with diaeres
			(*this)[_T("fnof")]			= _T('\x0192');		// f with hook
			(*this)[_T("circ")]			= _T('\x02C6');		// modifier letter circumflex accent
			(*this)[_T("tilde")]		= _T('\x02DC');		// small tilde
			(*this)[_T("ensp")]			= _T('\x2002');		// en space
			(*this)[_T("emsp")]			= _T('\x2003');		// em space
			(*this)[_T("thinsp")]		= _T('\x2009');		// thin space
			(*this)[_T("zwnj")]			= _T('\x200C');		// zero width non-joiner
			(*this)[_T("zwj")]			= _T('\x200D');		// zero width joiner
			(*this)[_T("lrm")]			= _T('\x200E');		// left-to-right mark
			(*this)[_T("rlm")]			= _T('\x200F');		// right-to-left mark
			(*this)[_T("ndash")]		= _T('\x2013');		// en dash
			(*this)[_T("mdash")]		= _T('\x2014');		// em dash
			(*this)[_T("lsquo")]		= _T('\x2018');		// left single quotation mark
			(*this)[_T("rsquo")]		= _T('\x2019');		// right single quotation mark
			(*this)[_T("sbquo")]		= _T('\x201A');		// single low-9 quotation mark
			(*this)[_T("ldquo")]		= _T('\x201C');		// left double quotation mark
			(*this)[_T("rdquo")]		= _T('\x201D');		// right double quotation mark
			(*this)[_T("bdquo")]		= _T('\x201E');		// double low-9 quotation mark
			(*this)[_T("dagger")]		= _T('\x2020');		// dagger
			(*this)[_T("Dagger")]		= _T('\x2021');		// double dagger
			(*this)[_T("bull")]			= _T('\x2022');		// bullet
			(*this)[_T("hellip")]		= _T('\x2026');		// horizontal ellipsis
			(*this)[_T("permil")]		= _T('\x2030');		// per mille
			(*this)[_T("prime")]		= _T('\x2032');		// minutes
			(*this)[_T("Prime")]		= _T('\x2033');		// seconds
			(*this)[_T("lsaquo")]		= _T('\x2039');		// single left angle quotation
			(*this)[_T("rsaquo")]		= _T('\x203A');		// single right angle quotation
			(*this)[_T("oline")]		= _T('\x203E');		// overline
			(*this)[_T("euro")]			= _T('\x20AC');		// euro
			(*this)[_T("trade")]		= _T('\x2122');		// trademark
			(*this)[_T("larr")]			= _T('\x2190');		// left arrow
			(*this)[_T("uarr")]			= _T('\x2191');		// up arrow
			(*this)[_T("rarr")]			= _T('\x2192');		// right arrow
			(*this)[_T("darr")]			= _T('\x2193');		// down arrow
			(*this)[_T("harr")]			= _T('\x2194');		// left right arrow
			(*this)[_T("crarr")]		= _T('\x21B5');		// carriage return arrow
			(*this)[_T("lceil")]		= _T('\x2308');		// left ceiling
			(*this)[_T("rceil")]		= _T('\x2309');		// right ceiling
			(*this)[_T("lfloor")]		= _T('\x230A');		// left floor
			(*this)[_T("rfloor")]		= _T('\x230B');		// right floor
			(*this)[_T("loz")]			= _T('\x25CA');		// lozenge
			(*this)[_T("spades")]		= _T('\x2660');		// spade
			(*this)[_T("clubs")]		= _T('\x2663');		// club
			(*this)[_T("hearts")]		= _T('\x2665');		// heart
			(*this)[_T("diams")]		= _T('\x2666');		// diamond




		}
	};

// Constructors
public:
	CLiteHTMLEntityResolver()	{ }

// Operations
public:
	/**
	 * Resolves a character or numeric entity reference.
	 *
	 * @param rStr - entity to resolve
	 * @param ch - variable that will receive the result
	 *
	 * @return number of TCHARs used to resolve entity reference
	 */
	static UINT resolveEntity(LPCTSTR lpszEntity, TCHAR &chSubst)
	{
		ATLASSERT(m_CharEntityRefs.GetCount());
		ATLASSERT(AtlIsValidString(lpszEntity));

		LPCTSTR	lpszBegin = lpszEntity, 
				lpszEnd = ::_tcschr(lpszEntity, _T(';'));
		TCHAR	chTemp = 0;
		
		// entity references always end with a semi-colon ';'
		if (lpszEnd == NULL)
			return (0);

		// skip leading white-space characters
		while (::_istspace(*lpszBegin))
			lpszBegin = ::_tcsinc(lpszBegin);
		
		// remaining string (including semi-colon) 
		// must be at least 4 characters in length
		if (lpszEnd - lpszBegin < 3)
			return (0U);

		// entity references always begin with an ampersand '&' symbol
		if (*lpszBegin != _T('&'))
			return (0U);
		lpszBegin = ::_tcsinc(lpszBegin);

		// numeric (decimal or hexadecimal) entity reference?
		if (*lpszBegin == _T('#'))
		{
			lpszBegin = ::_tcsinc(lpszBegin);
			chTemp = *lpszBegin;
			int	radix = (::_istdigit(chTemp) ? 10 : 
						(chTemp == _T('x') || 
							chTemp == _T('X') ? 16 : 0));
			if (radix)
			{
				if (radix == 16)
					lpszBegin = ::_tcsinc(lpszBegin);
				
				unsigned long	ulNum = ::_tcstoul(lpszBegin, NULL, radix);
				chSubst = (TCHAR)ulNum;
				lpszEnd = ::_tcsinc(lpszEnd);
				return (UINT)(lpszEnd - lpszEntity);
			}
		}

		// character entity reference?
		else
		{
			CString	strKey(lpszBegin, int (lpszEnd - lpszBegin));

			/*

			// because some character entity references are 
			// case-sensitive, we must fix them manually
			if (!strKey.CompareNoCase(_T("eth")) || 
				!strKey.CompareNoCase(_T("thorn")))
			{
				if (::_istupper(strKey[0]))
					strKey.MakeUpper();
				else
					strKey.MakeLower();
			}
			else if (!strKey.CompareNoCase(_T("Oslash")))
			{
				strKey.MakeLower();
				strKey.SetAt(0, _T('O'));
			}
			else if (!strKey.CompareNoCase(_T("AElig")))
			{
				strKey.MakeLower();
				strKey.SetAt(0, _T('A'));
				strKey.SetAt(1, _T('E'));
			}
			else
			{
				
				//CString	strT = strKey.Mid(1);
				//strKey.MakeLower();
				
				//if (strT.CompareNoCase(_T("grave")) == 0 || 
				//	strT.CompareNoCase(_T("acute")) == 0 || 
				//	strT.CompareNoCase(_T("circ"))  == 0 || 
				//	strT.CompareNoCase(_T("uml"))   == 0 || 
				//	strT.CompareNoCase(_T("tilde")) == 0 || 
				//	strT.CompareNoCase(_T("cedil")) == 0 || 
				//	strT.CompareNoCase(_T("ring"))  == 0)
				//{
				//	strKey.SetAt(0, strT[0]);
				//}	
			}
			*/
			
			// is this a known entity reference?
			if (m_CharEntityRefs.Lookup(strKey, chTemp))
			{
				chSubst = chTemp;
				lpszEnd = ::_tcsinc(lpszEnd);
				return (UINT) (lpszEnd - lpszEntity);
			}
		}

		return (0U);
	}

// Data Members
private:
	static CCharEntityRefs	m_CharEntityRefs;
};
#pragma warning(pop)
