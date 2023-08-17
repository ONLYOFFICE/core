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
			NSString* pString = [ [ NSString alloc ]
					initWithBytes : (char*)Str.data() length : Str.size() * sizeof(wchar_t)
			  encoding : CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingUTF32LE ) ];
			return pString;
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

				NSString* stringURL = StringWToNSString(m_sDownloadFileUrl);
				NSString* escapedURL = [stringURL stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLQueryAllowedCharacterSet]];

				int nResult = 1;

				if (m_pSession)
				{
					NSURLRequest* urlRequest = [NSURLRequest requestWithURL:[NSURL URLWithString:escapedURL]];

					__block NSData* result = nil;
					dispatch_semaphore_t sem = dispatch_semaphore_create(0);

					NSURLSession* _session = ((CSessionMAC*)m_pSession->m_pInternal)->m_session;
					if (nil == _session)
						_session = [NSURLSession sharedSession];

					[[_session dataTaskWithRequest:urlRequest
											completionHandler:^(NSData *data, NSURLResponse* response, NSError *error) {
						if (error == nil)
							result = data;

						dispatch_semaphore_signal(sem);
					}] resume];

					dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);

					if (result)
					{
						NSString* filePath = StringWToNSString(m_sDownloadFilePath);
						[result writeToFile:filePath atomically:YES];

						nResult = 0;
					}
					else
					{
						nResult = 1;
					}

					return nResult;
				}
				else
				{
					NSURL* url = [NSURL URLWithString:escapedURL];
					NSData* urlData = [NSData dataWithContentsOfURL:url];
					if ( urlData )
					{
						NSString* filePath = StringWToNSString(m_sDownloadFilePath);
						[urlData writeToFile:filePath atomically:YES];

	#if defined(_IOS)
						return 0;
	#else
	#ifndef _ASC_USE_ARC_
						if (!GetARCEnabled())
						{
							[stringURL release];
							//[url release];
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
					if (!GetARCEnabled())
					{
						[stringURL release];
						//[url release];
					}
	#endif
	#endif
					return 1;
				}
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
