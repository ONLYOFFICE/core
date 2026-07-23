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
#pragma once

#include "IVisitable.h"
#include "PictureDescriptor.h"

namespace DocFileFormat
{
	class WordDocument;
	class OleObject: public IVisitable
	{
		friend class OleObjectMapping;  
  
	public:
		enum LinkUpdateOption
		{
			NoLink = 0,
			Always = 1,
			OnCall = 3
		};	
		int					nWordVersion;				
		
		std::wstring		ObjectId;
		std::wstring		ClassId;

		std::wstring		UserType;		// Display name of the linked object or embedded object.
		std::wstring		ClipboardFormat;
		std::wstring		Link;
		std::wstring		Program;
		LinkUpdateOption	updateMode = NoLink;
		std::wstring		UpdateMode;

		bool				isLinked = false;
		bool				isEquation = false;
		bool				isEmbedded = false;
		bool				isPackage = false;
		
		std::pair<boost::shared_array<char>, size_t> embeddedData;
		
		PictureDescriptor	pictureDesciptor;

		OleObject( const CharacterPropertyExceptions* chpx, WordDocument* document);
		virtual ~OleObject() {}

	private:
		POLE::Storage	*oleStorage = NULL;
	  
		bool processLinkInfoStream		( const std::wstring& linkStream );      
		void processEquationNativeStream( const std::wstring& eqStream );
		void processPICStream			( const std::wstring& picStream );
		void processMETAStream			( const std::wstring& metaStream );
		bool processCompObjStream		( const std::wstring& compStream );
		bool processOleStream			( const std::wstring& oleStreamName );
		bool processPackageStream		(const std::wstring& oleStreamName);

		void processOleStream			( VirtualStreamReader& reader );
		void processLinkInfoStream		( VirtualStreamReader& reader );
		void processCompObjStream		( VirtualStreamReader& reader );

		std::wstring getOleEntryName	( const CharacterPropertyExceptions* chpx );
};
}
