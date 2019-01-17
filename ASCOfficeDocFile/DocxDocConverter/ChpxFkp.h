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

#include "Chpx.h"

namespace ASCDocFileFormat
{
	class ChpxFkp : public IStreamWriter
	{
	public:
		static const unsigned short CHPX_FKP_SIZE	=	512;
		static const unsigned char CHPX_FKP_MAX_CRUN			=	0x65;

		ChpxFkp(const vector<unsigned int>& inRgfc, const vector<Chpx>& arrChpxs) : crun(0)
		{
			crun = arrChpxs.size();
			rgfc = inRgfc;

			unsigned long chpxSizeInBytes = 0;

			chpxs.push_back( arrChpxs[0] );
			chpxSizeInBytes = chpxs[0].Size();
			rgb.push_back( (unsigned char)( ( CHPX_FKP_SIZE - 1 - chpxSizeInBytes ) / 2 ) );

			for (int i = 1; i < crun; ++i)
			{
				chpxs.push_back( arrChpxs[i] );
				chpxSizeInBytes = chpxs[i].Size();
				rgb.push_back( (unsigned char)( rgb[i-1] - 1 - ( chpxSizeInBytes / 2 ) ) );
			}
		}

		virtual ~ChpxFkp()
		{
		}

		//
		virtual unsigned char* GetBytes(unsigned long* size) const
		{
			unsigned char* bytes = NULL;

			if ( size != NULL )
			{
				*size = 512;
				bytes = new unsigned char[*size];

				if ( bytes != NULL )
				{
					memset( bytes, 0, *size );

					int i = 0;

					for (; i < (crun + 1); ++i)
					{
						FormatUtils::SetBytes((bytes + (i * sizeof(rgfc[i]))), (int)rgfc[i]);
					}

					i = (crun + 1) * sizeof(rgfc[i]);

					unsigned char *chpxsbytes = NULL;
					unsigned long chpxssize = 0;

					for (int j = 0; j < crun; ++j, ++i)
					{
						bytes[i]			=	rgb[j];
						unsigned char *chpxsbytes	=	chpxs[j].GetBytes(&chpxssize);

						if ( chpxsbytes != NULL )
						{
							memcpy((bytes + ( bytes[i] * 2 ) ), chpxsbytes, chpxssize);
							RELEASEARRAYOBJECTS (chpxsbytes);
						}
					}

					bytes[511] = crun;
				}
			}

			return bytes;
		}

		inline unsigned int GetEndOffset() const
		{
			return (*(rgfc.end() - 1));
		}

		//
		static vector<ChpxFkp> GetAllChpxFkps(const vector<unsigned int>& inRgfc, const vector<Chpx>& arrChpxs)
		{
			vector<ChpxFkp>			allChpxFkps;
			vector<unsigned int>	rgfc;
			vector<Chpx>			chpxs;

			rgfc.push_back( inRgfc[0] );
			unsigned int allChpxsSize = 0;
			unsigned int rgfcCount = 2;

			for (unsigned int i = 0; i < arrChpxs.size(); ++i)
			{
				unsigned int chpxSize = arrChpxs[i].Size();
				allChpxsSize += chpxSize;

				//Check if all rgfcs and rgbs + Chpxs less then 512 bytes
				if ( ( ( rgfcCount * sizeof(unsigned int) + ( rgfcCount - 1 ) * sizeof(unsigned int) ) >= ( CHPX_FKP_SIZE / 2 ) ) ||
					( allChpxsSize >= ( CHPX_FKP_SIZE / 2 ) ) )
				{
					ChpxFkp chpxFkp(rgfc, chpxs);
					allChpxFkps.push_back(chpxFkp);
					rgfc.clear();
					chpxs.clear();
					rgfc.push_back(inRgfc[i]);
					allChpxsSize	=	chpxSize;
					rgfcCount		=	2;
				}

				rgfc.push_back(inRgfc[i+1]);
				chpxs.push_back(arrChpxs[i]);
				++rgfcCount;
			}

			if (!rgfc.empty() && !chpxs.empty())
			{
				ChpxFkp chpxFkp(rgfc, chpxs);
				allChpxFkps.push_back(chpxFkp);

				rgfc.clear();
				chpxs.clear();
			}

			return allChpxFkps;
		}

	private:

		unsigned char					crun;

		vector<unsigned int>	rgfc;
		vector<unsigned char>			rgb;
		vector<Chpx>			chpxs;
	};
}