/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "FilePass.h"
#include "../../Crypt/Decryptor.h"

namespace XLS
{


FilePass::FilePass()
{
}


FilePass::~FilePass()
{
}


BaseObjectPtr FilePass::clone()
{
	return BaseObjectPtr(new FilePass(*this));
}

void FilePass::readFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version == 0x0500) 
	{
        record >> key.key >> key.verificationBytes;

		record.getGlobalWorkbookInfo()->decryptor = 
							CRYPT::DecryptorPtr(new CRYPT::XORDecryptor(2, key.key, key.verificationBytes, record.getGlobalWorkbookInfo()->password));
	}
	else
	{
		record >> wEncryptionType;
		
		if(wEncryptionType == 0)
		{
			record >> key;
		
			record.getGlobalWorkbookInfo()->decryptor = 
							CRYPT::DecryptorPtr(new CRYPT::XORDecryptor(2, key.key, key.verificationBytes, record.getGlobalWorkbookInfo()->password));
		}
		else
		{
			majorVer = *record.getCurData<unsigned short>();

			cryptHeaderPtr = CRYPTO::RC4EncryptionHeaderPtr(new CRYPTO::RC4EncryptionHeader());

			cryptHeaderPtr->bStandard = 0x0001 == majorVer ? true : false; // _S2dvT1xU_R3bOPwre4_.xls

			cryptHeaderPtr->load (record);
			
			if (cryptHeaderPtr->bStandard)
			{
				record.getGlobalWorkbookInfo()->decryptor = 
							CRYPT::DecryptorPtr(new CRYPT::RC4Decryptor(cryptHeaderPtr->crypt_data_rc4, record.getGlobalWorkbookInfo()->password));
			}
			else
			{
				record.getGlobalWorkbookInfo()->decryptor = 
							CRYPT::DecryptorPtr(new CRYPT::ECMADecryptor());
				
				CRYPT::ECMADecryptor *crypter = dynamic_cast<CRYPT::ECMADecryptor *>(record.getGlobalWorkbookInfo()->decryptor.get());

				crypter->SetCryptData(cryptHeaderPtr->crypt_data_aes);
				crypter->SetPassword(record.getGlobalWorkbookInfo()->password);
			}
		}
	}
}

} // namespace XLS

