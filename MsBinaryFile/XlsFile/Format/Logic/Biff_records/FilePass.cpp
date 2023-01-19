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

