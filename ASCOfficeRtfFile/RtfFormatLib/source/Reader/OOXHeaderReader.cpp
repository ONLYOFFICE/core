#include "OOXHeaderReader.h"
#include "OOXTextItemReader.h"
#include "OOXRelsReader.h"

bool OOXHeaderReader::Parse( ReaderParameter oParam, CString sRID, TextItemContainerPtr& oOutput, bool bHeader )
{
	if (m_ref == NULL) return false;
	if (m_ref->m_oId.IsInit() == false ) return false;

	OOX::CHdrFtr    *pHdrFtr = oParam.oDocx->GetHeaderOrFooter(m_ref->m_oId->GetValue());
	if (pHdrFtr == NULL) return false;	

	OOXTextItemReader oOOXTextItemReader;
	oOutput = TextItemContainerPtr( new TextItemContainer() );
	oOOXTextItemReader.m_oTextItems = oOutput;

	for (long i=0; i < pHdrFtr->m_arrItems.size(); i++)
	{
		oOOXTextItemReader.Parse(pHdrFtr->m_arrItems[i], oParam );
	}
	
	return true;
}
