#include "CFileDocument.h"
#include "../../../DesktopEditor/common/File.h"


CFileDocument::CFileDocument(std::wstring sPathDir)
{
    NSFile::CFileBinary oFileBinary;

    oFileBinary.CreateFileW(sPathDir + L"/document.xml");
}

CFileDocument::~CFileDocument()
{
}

