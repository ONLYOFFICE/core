/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "FileTransporter_private.h"
#include "../include/FileTransporter.h"
#include "Session.h"

#ifdef USE_EXTERNAL_TRANSPORT
#include "transport_external.h"
#endif

#if _IOS
#import <Foundation/Foundation.h>
#else
#include <Cocoa/Cocoa.h>
#endif

namespace NSNetwork
{
	namespace NSFileTransport
	{
		class CSessionMAC : public CSession_private
		{
		public:
			NSURLSession* m_session;

		public:
			CSessionMAC()
			{
				m_session = nil;
			}
			~CSessionMAC()
			{
				m_session = nil;
			}

			void Create()
			{
				NSURLSessionConfiguration *configuration = [NSURLSessionConfiguration defaultSessionConfiguration];

				std::map<std::string, std::string>::iterator iter;

				iter = m_props.find("timeoutIntervalForResource");
				if (iter != m_props.end())
					configuration.timeoutIntervalForResource = std::stoi(iter->second);

				iter = m_props.find("HTTPMaximumConnectionsPerHost");
				if (iter != m_props.end())
					configuration.HTTPMaximumConnectionsPerHost = std::stoi(iter->second);

				iter = m_props.find("allowsCellularAccess");
				if (iter != m_props.end())
					configuration.allowsCellularAccess = (std::stoi(iter->second) == 1) ? YES : NO;

				iter = m_props.find("requestCachePolicy");
				if (iter != m_props.end())
				{
					if ("NSURLRequestUseProtocolCachePolicy" == iter->second)
						configuration.requestCachePolicy = NSURLRequestUseProtocolCachePolicy;
				}

				m_session = [NSURLSession sessionWithConfiguration:configuration];
			}
		};

		CSession_private* CreateSession()
		{
			return new CSessionMAC();
		}
	}
}

namespace NSNetwork
{
	namespace NSFileTransport
	{
		static NSString* StringWToNSString ( const std::wstring& Str )
		{
			// Returns +1 retained NSString (alloc/init). Caller must release in non-ARC.
			NSString* pString = [ [ NSString alloc ]
				initWithBytes : (char*)Str.data() length : Str.size() * sizeof(wchar_t)
					 encoding : CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingUTF32LE ) ];
			return pString;
		}

		// NSURLComponents correctly handles mixed input (already-encoded + raw characters)
		// without double-encoding, but normalises unreserved percent-sequences (%5F → _).
		// If preserving the exact encoding is required, use [NSURL URLWithString:] instead.
		static NSURL* SafeURLFromString(NSString* urlString)
		{
			if (!urlString || urlString.length == 0)
				return nil;

			NSURL* url = [NSURL URLWithString:urlString];
			if (url)
				return url;

			// Fallback: the string contains unencoded characters (spaces, cyrillic,
			// brackets, etc.). URLFragmentAllowedCharacterSet includes '%', so
			// existing percent-sequences won't be double-encoded.
			NSString* encoded =
				[urlString stringByAddingPercentEncodingWithAllowedCharacters:
							   [NSCharacterSet URLFragmentAllowedCharacterSet]];
			if (!encoded)
				return nil;

			return [NSURL URLWithString:encoded];
		}

		class CFileTransporterBaseCocoa : public CFileTransporterBase
		{
		public :
			CFileTransporterBaseCocoa(const std::wstring &sDownloadFileUrl, bool bDelete = true)
				: CFileTransporterBase(sDownloadFileUrl, bDelete)
			{
			}
			CFileTransporterBaseCocoa(const std::wstring &sUploadUrl, const unsigned char* cData, const int nSize)
				: CFileTransporterBase(sUploadUrl, cData, nSize)
			{
			}
			CFileTransporterBaseCocoa(const std::wstring &sUploadUrl, const std::wstring &sUploadFilePath)
				: CFileTransporterBase(sUploadUrl, sUploadFilePath)
			{

			}
			virtual ~CFileTransporterBaseCocoa()
			{
			}

			virtual int DownloadFile() override
			{
				if (m_sDownloadFilePath.empty())
				{
					m_sDownloadFilePath = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"DWD");
					if (NSFile::CFileBinary::Exists(m_sDownloadFilePath))
						NSFile::CFileBinary::Remove(m_sDownloadFilePath);
				}

#ifdef USE_EXTERNAL_TRANSPORT
				int nExternalTransport = download_external(m_sDownloadFileUrl, m_sDownloadFilePath);
				if (0 == nExternalTransport)
					return 0;
#endif

				// stringURL: +1 retained (alloc/init). Must be released on all exit paths in non-ARC.
				NSString* stringURL = StringWToNSString(m_sDownloadFileUrl);
				NSURL* url = SafeURLFromString(stringURL);

				if (!url)
				{
					NSLog(@"[DownloadFile] Invalid URL: %@", stringURL);
#if !defined(_IOS)
#ifndef _ASC_USE_ARC_
					if (!GetARCEnabled())
						[stringURL release];
#endif
#endif
					return 1;
				}

				// Select session. If the caller provided one, prefer it; fall back to the shared
				// singleton. [NSURLSession sharedSession] is a singleton -- never retain/release it.
				NSURLSession* session = nil;
				if (m_pSession && m_pSession->m_pInternal)
				{
					CSessionMAC* sessionInternal = (CSessionMAC*)m_pSession->m_pInternal;
					session = sessionInternal->m_session;
				}
				if (!session)
					session = [NSURLSession sharedSession];

				// NSData initWithContentsOfURL / dataWithContentsOfURL must NOT be used for HTTP:
				// - Fails silently on CDN responses with Transfer-Encoding: chunked
				//   (returns NSCocoaErrorDomain Code=256 on some iOS versions)
				// - Does not follow HTTP redirects reliably
				// - Ignores Content-Encoding (gzip, br)
				// - Has no timeout, can block indefinitely
				// NSURLSession handles all of the above correctly on all supported iOS/macOS versions.
				NSURLRequest* urlRequest = [NSURLRequest requestWithURL:url];

				dispatch_semaphore_t sem = dispatch_semaphore_create(0);

				// In non-ARC, __block variables are NOT automatically retained by the block.
				// |data| passed to the completion handler is autoreleased; without an explicit
				// retain it may be deallocated before semaphore_wait returns on the calling thread.
				// We therefore retain inside the block and release after use below.
				__block NSData* resultData = nil;

				NSURLSessionDataTask* task =
					[session dataTaskWithRequest:urlRequest
						completionHandler:^(NSData* data, NSURLResponse* response, NSError* error)
						{
							if (error)
							{
								NSLog(@"[DownloadFile] Network error: %@, URL: %@", error, url);
							}
							else if ([response isKindOfClass:[NSHTTPURLResponse class]])
							{
								NSInteger status = [(NSHTTPURLResponse*)response statusCode];
								if (status >= 200 && status < 300)
								{
#if !defined(_IOS)
#ifndef _ASC_USE_ARC_
									if (!GetARCEnabled())
										resultData = [data retain];
									else
#endif
#endif
										resultData = data;
								}
								else
								{
									NSLog(@"[DownloadFile] HTTP %ld, URL: %@", (long)status, url);
								}
							}
							else
							{
								// Non-HTTP scheme (e.g. file://) -- accept unconditionally.
#if !defined(_IOS)
#ifndef _ASC_USE_ARC_
								if (!GetARCEnabled())
									resultData = [data retain];
								else
#endif
#endif
									resultData = data;
							}

							dispatch_semaphore_signal(sem);
						}];

				[task resume];

				// 60-second hard timeout. DISPATCH_TIME_FOREVER must not be used: if the network
				// stack silently drops the connection the completion handler may never fire,
				// leaving this thread suspended indefinitely (deadlock).
				const int64_t kTimeoutSeconds = 60;
				dispatch_time_t timeout = dispatch_time(DISPATCH_TIME_NOW, kTimeoutSeconds * NSEC_PER_SEC);
				long waitResult = dispatch_semaphore_wait(sem, timeout);

				// dispatch_semaphore_create returns a +1 object. Release it in non-ARC.
				// In ARC (including all iOS targets) GCD objects are managed automatically.
#if !defined(_IOS)
#ifndef _ASC_USE_ARC_
				if (!GetARCEnabled())
					dispatch_release(sem);
#endif
#endif

				if (waitResult != 0)
				{
					// Timeout expired. Cancel the in-flight task so that the completion handler
					// is called with NSURLErrorCancelled (data == nil), preventing it from writing
					// to resultData after we return and corrupting memory in non-ARC.
					[task cancel];
					NSLog(@"[DownloadFile] Timeout after %llds, URL: %@", (long long)kTimeoutSeconds, url);
#if !defined(_IOS)
#ifndef _ASC_USE_ARC_
					if (!GetARCEnabled())
					{
						// resultData could theoretically be non-nil if the completion handler
						// raced with the timeout check on a multicore device before cancel ran.
						[resultData release];
						[stringURL release];
					}
#endif
#endif
					return 1;
				}

				if (!resultData)
				{
#if !defined(_IOS)
#ifndef _ASC_USE_ARC_
					if (!GetARCEnabled())
						[stringURL release];
#endif
#endif
					return 1;
				}

				// filePath: +1 retained (alloc/init). Must be released in non-ARC.
				NSString* filePath = StringWToNSString(m_sDownloadFilePath);
				BOOL written = [resultData writeToFile:filePath atomically:YES];

				if (!written)
					NSLog(@"[DownloadFile] Failed to write file: %@", filePath);

#if !defined(_IOS)
#ifndef _ASC_USE_ARC_
				if (!GetARCEnabled())
				{
					[resultData release];
					[filePath release];
					[stringURL release];
				}
#endif
#endif

				return written ? 0 : 1;
			}

			virtual int UploadData() override
			{
#ifdef USE_EXTERNAL_TRANSPORT
				int nExternalTransport = uploaddata_external(m_sUploadUrl, m_cData, m_nSize);
				if (0 == nExternalTransport)
					return 0;
#endif
				//stub
				return -1;
			}

			virtual int UploadFile() override
			{
#ifdef USE_EXTERNAL_TRANSPORT
				int nExternalTransport = uploadfile_external(m_sUploadUrl, m_sUploadFilePath);
				if (0 == nExternalTransport)
					return 0;
#endif
				//stub
				return -1;
			}
		};

		CFileTransporter_private::CFileTransporter_private(const std::wstring &sDownloadFileUrl, bool bDelete)
		{
			m_pInternal = new CFileTransporterBaseCocoa(sDownloadFileUrl, bDelete);
		}

		CFileTransporter_private::CFileTransporter_private(const std::wstring &sUploadUrl, const unsigned char* cData, const int nSize)
		{
			m_pInternal = new CFileTransporterBaseCocoa(sUploadUrl, cData, nSize);
		}

		CFileTransporter_private::CFileTransporter_private(const std::wstring &sUploadUrl, const std::wstring &sUploadFilePath)
		{
			m_pInternal = new CFileTransporterBaseCocoa(sUploadUrl, sUploadFilePath);
		}
	}
}
