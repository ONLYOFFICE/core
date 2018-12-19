#include "FileDownloader_private.h"
#include "FileDownloader.h"

#if _IOS
    #import <Foundation/Foundation.h>
#else
    #include <Cocoa/Cocoa.h>
#endif

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
        if (m_sFilePath.empty())
        {
            m_sFilePath = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"DWD");
            if (NSFile::CFileBinary::Exists(m_sFilePath))
                NSFile::CFileBinary::Remove(m_sFilePath);
        }

        NSString* stringURL = StringWToNSString(m_sFileUrl);
        NSString *escapedURL = [stringURL stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLQueryAllowedCharacterSet]];
        NSURL  *url = [NSURL URLWithString:escapedURL];
        NSData *urlData = [NSData dataWithContentsOfURL:url];
        if ( urlData )
        {
            NSArray       *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
            NSString  *documentsDirectory = [paths objectAtIndex:0];
            
            NSString  *filePath = StringWToNSString ( m_sFilePath );
            [urlData writeToFile:filePath atomically:YES];
            
#if defined(_IOS)
            return 0;
#else
#ifndef _ASC_USE_ARC_
            if (!CFileDownloader::GetARCEnabled())
            {
                [stringURL release];
                [url release];
                [urlData release];
            }
#endif
#endif
            return 0;
        }
        
#if defined(_IOS)
        return 1;
#else
#ifndef _ASC_USE_ARC_
        if (!CFileDownloader::GetARCEnabled())
        {
            [stringURL release];
            [url release];
        }
#endif
#endif
        return 1;
    }
};

CFileDownloader_private::CFileDownloader_private(std::wstring sFileUrl, bool bDelete)
{
    m_pInternal = new CFileDownloaderBaseCocoa(sFileUrl, bDelete);
}
