#include "oMathPara.h"
namespace OOX
{	
	namespace Logic
	{
		CString      COMathPara::toXML() const
		{
			CString sResult = _T("<m:oMathPara>");

			if ( m_oOMath.IsInit() )
				sResult += m_oOMath->toXML();

			if ( m_oOMathParaPr.IsInit() )
				sResult += m_oOMathParaPr->toXML();

			sResult += _T("</m:oMathPara>");

			return sResult;
		}
	}//namespace Logic
}//namespace OOX