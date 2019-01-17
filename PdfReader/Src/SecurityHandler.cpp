/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#include "StringExt.h"
#include "PDFDoc.h"
#include "Decrypt.h"
#include "GlobalParams.h"
#include "SecurityHandler.h"

namespace PdfReader
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// SecurityHandler
	//-------------------------------------------------------------------------------------------------------------------------------

	SecurityHandler *SecurityHandler::Make(PDFDoc *pDocument, Object *pEncryptDict)
	{
		SecurityHandler *pSecurityHandler = NULL;
		Object oFilter;
		pEncryptDict->DictLookup("Filter", &oFilter);
		if (oFilter.IsName("Standard"))
		{
			pSecurityHandler = new StandardSecurityHandler(pDocument, pEncryptDict);
		}
		else if (oFilter.IsName())
		{
			// TO DO: Error "Couldn't find the security handler"
			pSecurityHandler = NULL;
		}
		else
		{
			// TO DO: Error "Missing or invalid 'Filter' entry in encryption dictionary"
			pSecurityHandler = NULL;
		}
		oFilter.Free();
		return pSecurityHandler;
	}

	SecurityHandler::SecurityHandler(PDFDoc *pDocument)
	{
		m_pDocument = pDocument;
	}

	SecurityHandler::~SecurityHandler()
	{
	}

	bool SecurityHandler::CheckEncryption(const std::wstring &ownerPassword, const std::wstring &userPassword)
	{
		return Authorize(ownerPassword, userPassword);
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// StandardSecurityHandler
	//-------------------------------------------------------------------------------------------------------------------------------

	StandardSecurityHandler::StandardSecurityHandler(PDFDoc *pDocument, Object *pEncryptDict) :
		SecurityHandler(pDocument), m_seOwnerEncryptionKey(NULL), m_seUserEncryptionKey(NULL), m_sePermsValue(NULL),
			m_seFileID(NULL), m_seOwnerKey(NULL), m_seUserKey(NULL)
	{
		m_bValid     = false;

		Object oVersion, oRevision, oLength, oOwnerKey, oUserKey, oPermission, oPermsValue, oOwnerEncryptionKey, oUserEncryptionKey;
		
		pEncryptDict->DictLookup("V",		&oVersion);
		pEncryptDict->DictLookup("R",		&oRevision);
		pEncryptDict->DictLookup("Length",	&oLength);
		pEncryptDict->DictLookup("O",		&oOwnerKey);
		pEncryptDict->DictLookup("U",		&oUserKey);
		pEncryptDict->DictLookup("P",		&oPermission);

		Object oFileID;
		m_pDocument->GetXRef()->GetTrailerDict()->DictLookup("ID", &oFileID);

		if (oVersion.IsInt() && oRevision.IsInt() && oOwnerKey.IsString() && 
			oUserKey.IsString() && oPermission.IsInt())
		{
			m_nEncryptVersion  = oVersion.GetInt();
			m_nEncryptRevision = oRevision.GetInt();
			m_eCryptType       = cryptRC4;

			// В случае Revision = 2, ключ должен быть 40-бит - некоторые PDF-генераторы неправильно пишут значение Length
			if (m_nEncryptRevision == 2 || !oLength.IsInt())
			{
				m_nFileKeyLength = 5; // 5 байт = 40-бит
			}
			else
			{
				m_nFileKeyLength = oLength.GetInt() / 8;
			}
			m_bEncryptMetadata = true;

			Object oCryptFilters, oStreamFilter, oStringFilter;
			pEncryptDict->DictLookup("CF", &oCryptFilters);
			pEncryptDict->DictLookup("StmF", &oStreamFilter);
			pEncryptDict->DictLookup("StrF", &oStringFilter);

			if (oCryptFilters.IsDict() && oStreamFilter.IsName() && oStringFilter.IsName() && !strcmp(oStreamFilter.GetName(), oStringFilter.GetName()))
			{
				Object oCryptCurFilter;
				if (oCryptFilters.DictLookup(oStreamFilter.GetName(), &oCryptCurFilter)->IsDict())
				{
					Object oCFM;
					oCryptCurFilter.DictLookup("CFM", &oCFM);
					if (oCFM.IsName("V2"))
					{
						m_eCryptType = cryptRC4;						
					}
					else if (oCFM.IsName("AESV2"))
					{
						m_eCryptType = cryptAES128;
					}
					else if (oCFM.IsName("AESV3"))
					{
						m_eCryptType = cryptAES256;
					}

					oCFM.Free();
				}
				oCryptCurFilter.Free();

				Object oCFLength;
				if (oCryptCurFilter.DictLookup("Length", &oCFLength)->IsInt())
				{
					// Согласно спецификации, должно быть oCFLength / 8
					m_nFileKeyLength = oCFLength.GetInt();
				}
				oCFLength.Free();	

			}
			oStringFilter.Free();
			oStreamFilter.Free();
			oCryptFilters.Free();
			Object oEncryptMetadata;
			if (pEncryptDict->DictLookup("EncryptMetadata", &oEncryptMetadata)->IsBool())
			{
				m_bEncryptMetadata = oEncryptMetadata.GetBool();
			}
			oEncryptMetadata.Free();

			m_nPermissionFlags = oPermission.GetInt();
			m_seOwnerKey = oOwnerKey.GetString()->Copy();
			m_seUserKey = oUserKey.GetString()->Copy();

			if (pEncryptDict->DictLookup("OE", &oOwnerEncryptionKey))
			{
				if (oOwnerEncryptionKey.IsString())
				{
					m_seOwnerEncryptionKey = oOwnerEncryptionKey.GetString()->Copy();
				}
			}
			if (pEncryptDict->DictLookup("UE", &oUserEncryptionKey))
			{
				if (oUserEncryptionKey.IsString())
				{
					m_seUserEncryptionKey = oUserEncryptionKey.GetString()->Copy();
				}
			}	
			if (pEncryptDict->DictLookup("Perms", &oPermsValue))
			{
				if (oPermsValue.IsString())
				{
					m_sePermsValue = oPermsValue.GetString()->Copy();
				}
			}

			if (oFileID.IsArray())
			{
				Object oFileIDString;
				if (oFileID.ArrayGet(0, &oFileIDString)->IsString())
				{
					m_seFileID = oFileIDString.GetString()->Copy();
				}
				else
				{
					m_seFileID = new StringExt();
				}
				oFileIDString.Free();
			}
			else
			{
				m_seFileID = new StringExt();
			}
			m_bValid = true;

		}
		else
		{
		}

		oFileID.Free();
		oPermission.Free();
		oUserKey.Free();
		oOwnerKey.Free();
		oLength.Free();
		oRevision.Free();
		oVersion.Free();
	}

	StandardSecurityHandler::~StandardSecurityHandler()
	{
		if (m_seFileID)
		{
			delete m_seFileID;
		}
		if (m_seOwnerKey)
		{
			delete m_seOwnerKey;
		}
		if (m_seUserKey)
		{
			delete m_seUserKey;
		}
		if (m_seOwnerEncryptionKey)
		{
			delete m_seOwnerEncryptionKey;
		}
		if (m_seUserEncryptionKey)
		{
			delete m_seUserEncryptionKey;
		}
		if (m_sePermsValue)
		{
			delete m_sePermsValue;
		}
	}

	bool StandardSecurityHandler::Authorize(const std::wstring &ownerPassword, const std::wstring &userPassword)
	{
		if (!m_bValid)
		{
			return false;
		}
		if (!Decrypt::MakeFileKey(this, ownerPassword, userPassword))
		{
			return false;
		}
		return true;
	}
}