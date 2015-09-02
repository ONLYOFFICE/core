#include "./filedownloader.h"
#include <Cocoa/Cocoa.h>

static NSString* StringWToNSString ( const std::wstring& Str )
{
    NSString* pString = [ [ NSString alloc ]
                         initWithBytes : (char*)Str.data()
                         length : Str.size() * sizeof(wchar_t)
                         encoding : CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingUTF32LE ) ];
    return pString;
}

int CFileDownloader::DownloadFileAll(std::wstring sFileURL)
{
    NSString* stringURL = StringWToNSString(sFileURL);
    NSURL  *url = [NSURL URLWithString:stringURL];
    NSData *urlData = [NSData dataWithContentsOfURL:url];
    if ( urlData )
    {
        NSArray       *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString  *documentsDirectory = [paths objectAtIndex:0];

        NSString  *filePath = StringWToNSString ( m_sFilePath );
        [urlData writeToFile:filePath atomically:YES];
        return 1;
    }
    return 0;
}
