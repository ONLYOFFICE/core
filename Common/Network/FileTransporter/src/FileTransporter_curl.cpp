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

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "../../DesktopEditor/common/Directory.h"

namespace NSNetwork
{
	namespace NSFileTransport
	{

#ifndef USE_EXTERNAL_TRANSPORT

#include <fcntl.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h>

		namespace SecurityHelpers
		{
			// SSRF: CIDR range labels required for security audit trail
			static bool isSchemeAllowed(const std::string& url)
			{
				if (url.size() >= 8 && (url.substr(0, 7) == "http://" || url.substr(0, 7) == "HTTP://"))
					return true;
				if (url.size() >= 9 && (url.substr(0, 8) == "https://" || url.substr(0, 8) == "HTTPS://"))
					return true;
				return false;
			}

			static bool isPrivateIP(const struct sockaddr* addr)
			{
				if (addr->sa_family == AF_INET)
				{
					const struct sockaddr_in* sin = reinterpret_cast<const struct sockaddr_in*>(addr);
					uint32_t ip = ntohl(sin->sin_addr.s_addr);

					if ((ip & 0xFF000000) == 0x0A000000) return true;  // 10.0.0.0/8
					if ((ip & 0xFFF00000) == 0xAC100000) return true;  // 172.16.0.0/12
					if ((ip & 0xFFFF0000) == 0xC0A80000) return true;  // 192.168.0.0/16
					if ((ip & 0xFF000000) == 0x7F000000) return true;  // 127.0.0.0/8
					if ((ip & 0xFFFF0000) == 0xA9FE0000) return true;  // 169.254.0.0/16 link-local
					if ((ip & 0xFF000000) == 0x00000000) return true;  // 0.0.0.0/8
				}
				else if (addr->sa_family == AF_INET6)
				{
					const struct sockaddr_in6* sin6 = reinterpret_cast<const struct sockaddr_in6*>(addr);
					const uint8_t* b = sin6->sin6_addr.s6_addr;

					// ::1 loopback
					if (b[0]==0 && b[1]==0 && b[2]==0 && b[3]==0 &&
						b[4]==0 && b[5]==0 && b[6]==0 && b[7]==0 &&
						b[8]==0 && b[9]==0 && b[10]==0 && b[11]==0 &&
						b[12]==0 && b[13]==0 && b[14]==0 && b[15]==1)
						return true;

					if ((b[0] & 0xFF) == 0xFE && (b[1] & 0xC0) == 0x80) return true;  // fe80::/10 link-local
					if ((b[0] & 0xFE) == 0xFC) return true;                              // fc00::/7 ULA
				}
				return false;
			}

			static bool validateUrl(const std::string& url)
			{
				if (!isSchemeAllowed(url))
				{
					std::cerr << "[SECURITY] Blocked URL with disallowed scheme: " << url << std::endl;
					return false;
				}

				const char* block_env = std::getenv("BLOCK_PRIVATE_IPS");
				bool block_private = true;
				if (block_env && (std::strcmp(block_env, "false") == 0 || std::strcmp(block_env, "0") == 0))
					block_private = false;

				if (!block_private)
					return true;

				size_t host_start = url.find("://");
				if (host_start == std::string::npos)
					return false;
				host_start += 3;
				size_t host_end = url.find('/', host_start);
				std::string hostname = (host_end == std::string::npos)
					? url.substr(host_start)
					: url.substr(host_start, host_end - host_start);

				size_t port_pos = hostname.find(':');
				if (port_pos != std::string::npos)
					hostname = hostname.substr(0, port_pos);

				size_t cred_pos = hostname.find('@');
				if (cred_pos != std::string::npos)
					hostname = hostname.substr(cred_pos + 1);

				struct addrinfo hints = {};
				hints.ai_family = AF_UNSPEC;
				hints.ai_socktype = SOCK_STREAM;
				struct addrinfo* result = nullptr;
				int rc = getaddrinfo(hostname.c_str(), nullptr, &hints, &result);
				if (rc != 0 || !result)
					return true;

				bool is_private = false;
				for (struct addrinfo* rp = result; rp != nullptr; rp = rp->ai_next)
				{
					if (isPrivateIP(rp->ai_addr))
					{
						is_private = true;
						break;
					}
				}
				freeaddrinfo(result);

				if (is_private)
				{
					std::cerr << "[SECURITY] Blocked URL resolving to private/reserved IP: " << url << std::endl;
					return false;
				}

				return true;
			}

			// Distro labels required to identify which CA bundle paths apply per platform
			static const char* resolveCAPath()
			{
				const char* env_cert = std::getenv("SSL_CERT_FILE");
				if (env_cert && env_cert[0] != '\0')
					return env_cert;

				static const char* ca_paths[] = {
					"/etc/ssl/certs/ca-certificates.crt",                         // Debian/Ubuntu
					"/etc/pki/tls/cert.pem",                                      // RHEL/CentOS
					"/etc/ssl/cert.pem",                                           // Alpine/FreeBSD
					"/etc/pki/ca-trust/extracted/pem/tls-ca-bundle.pem",          // Fedora
				};
				for (const char* p : ca_paths)
				{
					if (access(p, R_OK) == 0)
						return p;
				}
				return nullptr;
			}

			static void configureSSL(CURL* curl)
			{
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
				const char* ca_path = resolveCAPath();
				if (ca_path)
					curl_easy_setopt(curl, CURLOPT_CAINFO, ca_path);
			}
		}

#else

#include "transport_external.h"

#endif
		class CFileTransporterBaseCURL : public CFileTransporterBase
		{
		public :
			CFileTransporterBaseCURL(const std::wstring &sDownloadFileUrl, bool bDelete = true)
				: CFileTransporterBase(sDownloadFileUrl, bDelete)
			{
			}
			CFileTransporterBaseCURL(const std::wstring &sUploadUrl, const unsigned char* cData, const int nSize)
				: CFileTransporterBase(sUploadUrl, cData, nSize)
			{
			}
			CFileTransporterBaseCURL(const std::wstring &sUploadUrl, const std::wstring &sUploadFilePath)
				: CFileTransporterBase(sUploadUrl, sUploadFilePath)
			{

			}
			virtual ~CFileTransporterBaseCURL()
			{
				if (m_bDelete && !m_sDownloadFilePath.empty())
				{
					std::string sFilePath = U_TO_UTF8(m_sDownloadFilePath);
					unlink(sFilePath.c_str());
				}
			}

#ifndef USE_EXTERNAL_TRANSPORT
			static size_t write_data(void *ptr, size_t size, size_t nmemb, int fd) {
				size_t written = write(fd, ptr, size * nmemb);
				return written;
			}

			/*int progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
			{
				// It's here you will write the code for the progress message or bar
				int percent = static_cast<int>((100.0 * NowDownloaded) / TotalToDownload);

				if(CFileTransporterBase::m_func_onProgress)
					CFileTransporterBase::m_func_onProgress(percent);
				return 0;
			}
			*/


			static size_t write_data_to_string(char *contents, size_t size, size_t nmemb, void *userp)
			{
				((std::string*)userp)->append((char*)contents, size * nmemb);
				return size * nmemb;
			}

			virtual int DownloadFile() override
			{
				CURL *curl;
				int fp;
				CURLcode res = CURLE_FAILED_INIT;
				std::string sUrl = U_TO_UTF8(m_sDownloadFileUrl);
				std::string sOut;
				const char *url = sUrl.c_str();

				if (!SecurityHelpers::validateUrl(sUrl))
					return 1;

				curl = curl_easy_init();
				if (curl)
				{
					fp = createUniqueTempFile(sOut);
					curl_easy_setopt(curl, CURLOPT_URL, url);
					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
					curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
					SecurityHelpers::configureSSL(curl);
					curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
					curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0");
					res = curl_easy_perform(curl);
					curl_easy_cleanup(curl);
					close(fp);
				}

				m_bComplete = (CURLE_OK == res);
				if (m_bComplete)
				{
					if (m_sDownloadFilePath.empty())
						m_sDownloadFilePath = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sOut.c_str(), sOut.length());
					else
						NSFile::CFileBinary::Move(UTF8_TO_U(sOut), m_sDownloadFilePath);
				}
				//int nRes = execl("/usr/bin/wget", stringWstingToUtf8String (m_sFileUrl).c_str(), "-P", stringWstingToUtf8String (m_sDownloadFilePath).c_str(), (char *)NULL);
				//m_bComplete = nRes >= 0;

				return m_bComplete ? 0 : 1;
			}

			virtual int UploadData() override
			{
				CURL *curl;
				CURLcode res = CURLE_FAILED_INIT;
				std::string sUrl = U_TO_UTF8(m_sUploadUrl);
				const char *url = sUrl.c_str();
				struct curl_slist *headerlist = NULL;
				std::string readBuffer;

				if (!SecurityHelpers::validateUrl(sUrl))
					return 1;

				curl = curl_easy_init();
				if(curl) {

					headerlist = curl_slist_append(headerlist, "Content-Type: application/octet-stream");

					curl_easy_setopt(curl, CURLOPT_POST, true);
					curl_easy_setopt(curl, CURLOPT_HEADER, true);
					curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
					curl_easy_setopt(curl, CURLOPT_URL, url);
					curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, m_nSize);
					curl_easy_setopt(curl, CURLOPT_POSTFIELDS, m_cData);

					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_string);
					curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

					SecurityHelpers::configureSSL(curl);

					res = curl_easy_perform(curl);

					long http_code = 0;
					curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

					if (res == CURLE_OK)
					{
						if (http_code == 200 || http_code == 1223)
						{
							size_t startLenghtPos = readBuffer.find("Content-Length:") + sizeof("Content-Length:");
							size_t endLenghtPos = readBuffer.substr(startLenghtPos, readBuffer.length()).find("\r");
							std::string dataSize = readBuffer.substr(startLenghtPos, endLenghtPos);
							readBuffer = readBuffer.substr(readBuffer.length() - std::stoi(dataSize));
							NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)readBuffer.c_str(), (LONG)readBuffer.length(), m_sResponse);
						}
						else
						{
							res = CURLE_HTTP_RETURNED_ERROR;
						}
					}

					/* always cleanup */
					curl_easy_cleanup(curl);
				}
				m_bComplete = (CURLE_OK == res);

				return m_bComplete ? 0 : 1;
			}

			virtual int UploadFile() override
			{
				//stub
				return -1;
			}

		protected:
			int createUniqueTempFile (std::string &filename)
			{
				std::string sTempPath = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(NSDirectory::GetTempPath());
				char szTempPath[4096];
				snprintf(szTempPath, sizeof(szTempPath), "%s/fileXXXXXX", sTempPath.c_str());
				int fd = mkstemp(szTempPath);
				if (-1 != fd)
					filename = szTempPath;
				return fd;
			}
#else
			virtual int DownloadFile() override
			{
				if (m_sDownloadFilePath.empty())
				{
					m_sDownloadFilePath = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSDirectory::GetTempPath(), L"DW");
					if (NSFile::CFileBinary::Exists(m_sDownloadFilePath))
						NSFile::CFileBinary::Remove(m_sDownloadFilePath);
				}
				return download_external(m_sDownloadFileUrl, m_sDownloadFilePath, m_func_onProgress, m_check_aborted);
			}
			virtual int UploadData() override
			{
				if (!m_sUploadUrl.empty() && m_cData != NULL && m_nSize != 0)
				{
					return uploaddata_external(m_sUploadUrl, m_cData, m_nSize);
				}
				return -1;
			}
			virtual int UploadFile() override
			{
				if (!m_sUploadUrl.empty() && !m_sUploadFilePath.empty())
				{
					return uploadfile_external(m_sUploadUrl, m_sUploadFilePath);
				}
				return -1;
			}

#endif
		};

		CFileTransporter_private::CFileTransporter_private(const std::wstring &sDownloadFileUrl, bool bDelete)
			: m_pInternal(new CFileTransporterBaseCURL(sDownloadFileUrl, bDelete))

		{
			m_pInternal->m_check_aborted = std::bind(&CBaseThread::isAborted, this);
		}

		CFileTransporter_private::CFileTransporter_private(const std::wstring &sUploadUrl, const unsigned char* cData, const int nSize)
			: m_pInternal(new CFileTransporterBaseCURL(sUploadUrl, cData, nSize))
		{
			m_pInternal->m_check_aborted = std::bind(&CBaseThread::isAborted, this);
		}

		CFileTransporter_private::CFileTransporter_private(const std::wstring &sUploadUrl, const std::wstring &sUploadFilePath)
			: m_pInternal(new CFileTransporterBaseCURL(sUploadUrl, sUploadFilePath))
		{
			m_pInternal->m_check_aborted = std::bind(&CBaseThread::isAborted, this);
		}
	}
}
