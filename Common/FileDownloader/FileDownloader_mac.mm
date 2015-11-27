#include "FileDownloader.h"
#include <Cocoa/Cocoa.h>

static NSString* StringWToNSString ( const std::wstring& Str )
{
    NSString* pString = [ [ NSString alloc ]
                         initWithBytes : (char*)Str.data()
                         length : Str.size() * sizeof(wchar_t)
                         encoding : CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingUTF32LE ) ];
    return pString;
}

class CFileDownloaderBaseCocoa : public CFileDownloaderBase
{
public :
    CFileDownloaderBaseCocoa(std::wstring sFileUrl, bool bDelete = true)
        : CFileDownloaderBase(sFileUrl, bDelete)
    {
    }
    virtual ~CFileDownloaderBaseCocoa()
    {
    }

    virtual int DownloadFile()
    {
        m_sFilePath = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"DWD");
        if (NSFile::CFileBinary::Exists(m_sFilePath))
            NSFile::CFileBinary::Remove(m_sFilePath);

        NSString* stringURL = StringWToNSString(m_sFileUrl);
        NSURL  *url = [NSURL URLWithString:stringURL];
        NSData *urlData = [NSData dataWithContentsOfURL:url];
        if ( urlData )
        {
            NSArray       *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
            NSString  *documentsDirectory = [paths objectAtIndex:0];

            NSString  *filePath = StringWToNSString ( m_sFilePath );
            [urlData writeToFile:filePath atomically:YES];

    #ifndef _ASC_USE_ARC_
            [stringURL release];
            [url release];
            [urlData release];
    #endif
            return 0;
        }
    #ifndef _ASC_USE_ARC_
        [stringURL release];
        [url release];
    #endif
        return 1;
    }
};

CFileDownloader::CFileDownloader(std::wstring sFileUrl, bool bDelete)
{
    m_pInternal = new CFileDownloaderBaseCocoa(sFileUrl, bDelete);
}
