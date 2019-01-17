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
#pragma once

#include "Common.h"

#include "OfficeArtRecordHeader.h"
#include "IOfficeArtRecord.h"

namespace OfficeArt
{
	struct OfficeArtIDCL	//	Record specifies a file identifier cluster, which is used to group shape identifiers within a drawing
	{
		OfficeArtIDCL () : dgid (0), cspidCur(0)
		{

		}

		OfficeArtIDCL (unsigned int idgid, unsigned int icspidCur) : dgid (idgid), cspidCur(icspidCur)
		{

		}

		unsigned int dgid;
		unsigned int cspidCur;
	};

	struct OfficeArtFDGG 
	{
		OfficeArtFDGG () : spidMax (0), cidcl(0), cspSaved (0), cdgSaved(0)
		{

		}

		OfficeArtFDGG (MSOSPID id, unsigned int cid, unsigned int csp, unsigned int cdg) : spidMax (id), cidcl(cid), cspSaved (csp), cdgSaved(cdg) 
		{

		}

		MSOSPID			spidMax;			// Maximum shape ID
		unsigned int	cidcl;				// Number of FileIdClusters
		unsigned int	cspSaved;			// Total number of shapes saved
		unsigned int	cdgSaved;			// Total number of drawings saved
	};

	class OfficeArtFDGGBlock : public IOfficeArtRecord
	{
	public:
		OfficeArtFDGGBlock (): rh(0x0, 0x000, 0xF006, 0x00000010), bytes (NULL), size(0)
		{

		}

		OfficeArtFDGGBlock (const OfficeArtFDGGBlock& office): rh (office.rh), head (office.head), bytes (NULL), size(0)
		{
			for (size_t i = 0; i < office.Rgidcl.size(); ++i)
				Rgidcl.push_back(office.Rgidcl[i]);

			Initialize();
		}

		virtual operator const unsigned char* () const
		{
			return (const unsigned char*)(bytes);
		}

		virtual operator unsigned char* () const
		{
			return (unsigned char*)(bytes);
		}

		virtual unsigned int Size() const
		{
			return size;
		}

		virtual IOfficeArtRecord* New() const
		{
			return new OfficeArtFDGGBlock;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtFDGGBlock(*this);
		}

		inline void SetHead (const OfficeArtFDGG& oHead)
		{
			head	=	oHead;
		}

		virtual void PushBack (const OfficeArtIDCL& record)
		{
			Rgidcl.push_back(record);
			Initialize();
		}


		virtual ~OfficeArtFDGGBlock()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

	private:
		void Initialize()
		{
			rh		=	OfficeArtRecordHeader (0x0, 0x000, 0xF006, 0x00000010 + (int)Rgidcl.size() * 0x00000008);
		
			size	=	sizeof(rh);
			size	+=	sizeof(OfficeArtFDGG);
			size	+=	sizeof(OfficeArtIDCL) * (int)Rgidcl.size();

			RELEASEARRAYOBJECTS (bytes);

			if (size)
			{
				bytes = new unsigned char[size];

				if (bytes)
				{
					memset(bytes, 0, size);

					unsigned int offset = 0;

					memcpy ( (bytes + offset), (unsigned char*)(rh), sizeof(rh));
					offset += sizeof(rh);

					memcpy ( (bytes + offset), &head, sizeof(OfficeArtFDGG));
					offset += sizeof(OfficeArtFDGG);

					for (size_t i = 0; i < Rgidcl.size(); ++i)
					{
						memcpy ( (bytes + offset), &Rgidcl[i], sizeof(OfficeArtIDCL));
						offset +=  sizeof(OfficeArtIDCL);
					}
				}
			}
		}


	protected:

		OfficeArtRecordHeader	rh;
		OfficeArtFDGG			head;
		vector <OfficeArtIDCL>	Rgidcl;	// An array of OfficeArtIDCL elements

		unsigned char*					bytes;
		unsigned int			size;
	};
}