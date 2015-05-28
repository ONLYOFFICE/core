#pragma once
#ifndef PPTX_LIMIT_TEXTSHAPETYPE_INCLUDE_H_
#define PPTX_LIMIT_TEXTSHAPETYPE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TextShapeType : public BaseLimit
		{
		public:
			TextShapeType()
			{
				m_strValue = _T("textNoShape");
	 		}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("textArchDown") == strValue) ||
					(_T("textArchDownPour") == strValue) ||
					(_T("textArchUp") == strValue) ||
					(_T("textArchUpPour") == strValue) ||
					(_T("textButton") == strValue) ||
					(_T("textButtonPour") == strValue) ||
					(_T("textCanDown") == strValue) ||
					(_T("textCanUp") == strValue) ||
					(_T("textCascadeDown") == strValue) ||
					(_T("textCascadeUp") == strValue) ||
					(_T("textChevron") == strValue) ||
					(_T("textChevronInverted") == strValue) ||
					(_T("textCircle") == strValue) ||
					(_T("textCirclePour") == strValue) ||
					(_T("textCurveDown") == strValue) ||
					(_T("textCurveUp") == strValue) ||
					(_T("textDeflate") == strValue) ||
					(_T("textDeflateBottom") == strValue) ||
					(_T("textDeflateInflate") == strValue) ||
					(_T("textDeflateInflateDeflate") == strValue) ||
					(_T("textDeflateTop") == strValue) ||
					(_T("textDoubleWave1") == strValue) ||
					(_T("textFadeDown") == strValue) ||
					(_T("textFadeLeft") == strValue) ||
					(_T("textFadeRight") == strValue) ||
					(_T("textFadeUp") == strValue) ||
					(_T("textInflate") == strValue) ||
					(_T("textInflateBottom") == strValue) ||
					(_T("textInflateTop") == strValue) ||
					(_T("textNoShape") == strValue) ||
					(_T("textPlain") == strValue) ||
					(_T("textRingInside") == strValue) ||
					(_T("textRingOutside") == strValue) ||
					(_T("textSlantDown") == strValue) ||
					(_T("textSlantUp") == strValue) ||
					(_T("textStop") == strValue) ||
					(_T("textTriangle") == strValue) ||
					(_T("textTriangleInverted") == strValue) ||
					(_T("textWave1") == strValue) ||
					(_T("textWave2") == strValue) ||
					(_T("textWave4") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("textArchDown") == m_strValue) return 0;
				if (_T("textArchDownPour") == m_strValue) return 1;
				if (_T("textArchUp") == m_strValue) return 2;
				if (_T("textArchUpPour") == m_strValue) return 3;
				if (_T("textButton") == m_strValue) return 4;
				if (_T("textButtonPour") == m_strValue) return 5;
				if (_T("textCanDown") == m_strValue) return 6;
				if (_T("textCanUp") == m_strValue) return 7;
				if (_T("textCascadeDown") == m_strValue) return 8;
				if (_T("textCascadeUp") == m_strValue) return 9;
				if (_T("textChevron") == m_strValue) return 10;
				if (_T("textChevronInverted") == m_strValue) return 11;
				if (_T("textCircle") == m_strValue) return 12;
				if (_T("textCirclePour") == m_strValue) return 13;
				if (_T("textCurveDown") == m_strValue) return 14;
				if (_T("textCurveUp") == m_strValue) return 15;
				if (_T("textDeflate") == m_strValue) return 16;
				if (_T("textDeflateBottom") == m_strValue) return 17;
				if (_T("textDeflateInflate") == m_strValue) return 18;
				if (_T("textDeflateInflateDeflate") == m_strValue) return 19;
				if (_T("textDeflateTop") == m_strValue) return 20;
				if (_T("textDoubleWave1") == m_strValue) return 21;
				if (_T("textFadeDown") == m_strValue) return 22;
				if (_T("textFadeLeft") == m_strValue) return 23;
				if (_T("textFadeRight") == m_strValue) return 24;
				if (_T("textFadeUp") == m_strValue) return 25;
				if (_T("textInflate") == m_strValue) return 26;
				if (_T("textInflateBottom") == m_strValue) return 27;
				if (_T("textInflateTop") == m_strValue) return 28;
				if (_T("textNoShape") == m_strValue) return 29;
				if (_T("textPlain") == m_strValue) return 30;
				if (_T("textRingInside") == m_strValue) return 31;
				if (_T("textRingOutside") == m_strValue) return 32;
				if (_T("textSlantDown") == m_strValue) return 33;
				if (_T("textSlantUp") == m_strValue) return 34;
				if (_T("textStop") == m_strValue) return 35;
				if (_T("textTriangle") == m_strValue) return 36;
				if (_T("textTriangleInverted") == m_strValue) return 37;
				if (_T("textWave1") == m_strValue) return 38;
				if (_T("textWave2") == m_strValue) return 39;
				if (_T("textWave4") == m_strValue) return 40;
				return 29;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0: m_strValue = _T("textArchDown"); break;
				case 1: m_strValue = _T("textArchDownPour"); break;
				case 2: m_strValue = _T("textArchUp"); break;
				case 3: m_strValue = _T("textArchUpPour"); break;
				case 4: m_strValue = _T("textButton"); break;
				case 5: m_strValue = _T("textButtonPour"); break;
				case 6: m_strValue = _T("textCanDown"); break;
				case 7: m_strValue = _T("textCanUp"); break;
				case 8: m_strValue = _T("textCascadeDown"); break;
				case 9: m_strValue = _T("textCascadeUp"); break;
				case 10: m_strValue = _T("textChevron"); break;
				case 11: m_strValue = _T("textChevronInverted"); break;
				case 12: m_strValue = _T("textCircle"); break;
				case 13: m_strValue = _T("textCirclePour"); break;
				case 14: m_strValue = _T("textCurveDown"); break;
				case 15: m_strValue = _T("textCurveUp"); break;
				case 16: m_strValue = _T("textDeflate"); break;
				case 17: m_strValue = _T("textDeflateBottom"); break;
				case 18: m_strValue = _T("textDeflateInflate"); break;
				case 19: m_strValue = _T("textDeflateInflateDeflate"); break;
				case 20: m_strValue = _T("textDeflateTop"); break;
				case 21: m_strValue = _T("textDoubleWave1"); break;
				case 22: m_strValue = _T("textFadeDown"); break;
				case 23: m_strValue = _T("textFadeLeft"); break;
				case 24: m_strValue = _T("textFadeRight"); break;
				case 25: m_strValue = _T("textFadeUp"); break;
				case 26: m_strValue = _T("textInflate"); break;
				case 27: m_strValue = _T("textInflateBottom"); break;
				case 28: m_strValue = _T("textInflateTop"); break;
				case 29: m_strValue = _T("textNoShape"); break;
				case 30: m_strValue = _T("textPlain"); break;
				case 31: m_strValue = _T("textRingInside"); break;
				case 32: m_strValue = _T("textRingOutside"); break;
				case 33: m_strValue = _T("textSlantDown"); break;
				case 34: m_strValue = _T("textSlantUp"); break;
				case 35: m_strValue = _T("textStop"); break;
				case 36: m_strValue = _T("textTriangle"); break;
				case 37: m_strValue = _T("textTriangleInverted"); break;
				case 38: m_strValue = _T("textWave1"); break;
				case 39: m_strValue = _T("textWave2"); break;
				case 40: m_strValue = _T("textWave4"); break;
				default:m_strValue = _T("textNoShape"); break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TEXTSHAPETYPE_INCLUDE_H_