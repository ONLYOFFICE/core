#include "oMathPara.h"
namespace OOX
{	
	namespace Logic
	{
		CString      COMathPara::toXML() const
		{
			CString sResult = _T("<m:oMathPara>");

			for ( unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
			{
				if ( m_arrItems[nIndex])
				{
					sResult += m_arrItems[nIndex]->toXML();
				}
			}
			
			sResult += _T("</m:oMathPara>");

			return sResult;
		}
	}//namespace Logic
}//namespace OOX