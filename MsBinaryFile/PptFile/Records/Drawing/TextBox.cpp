#include "TextBox.h"

using namespace PPT;

CRecordTextBox::CRecordTextBox()
{

}

CRecordTextBox::~CRecordTextBox()
{

}

void CRecordTextBox::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
