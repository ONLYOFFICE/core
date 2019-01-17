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

#include "Paragraph.h"

#include "../Common/TextMark.h"

namespace ASCDocFileFormat
{
	Paragraph::Paragraph (short _style)
	{
		runsOffset	=	0;
		style		=	_style;
		depth		=	0;
	}

	Paragraph::~Paragraph()
	{
	}

	IVirtualConstructor* Paragraph::New() const
	{
		return new Paragraph();
	}

	IVirtualConstructor* Paragraph::Clone() const
	{
		return new Paragraph( *this );
	}
}

namespace ASCDocFileFormat
{
	void Paragraph::AddParagraphItem(const IParagraphItem& oItem)
	{
		paragraphItems.push_back(ParagraphItem(oItem, runsOffset));
		runsOffset += sizeof(wchar_t) * oItem.GetTextSize();
	}

	unsigned long Paragraph::GetTextSize() const
	{
		unsigned long paragraphTextSize = 0;

		for (list<ParagraphItem>::const_iterator iter = paragraphItems.begin(); iter != paragraphItems.end(); ++iter)
		{
			paragraphTextSize += (*iter)->GetTextSize();
		}

		return ( paragraphTextSize + 1 );
	}

	std::wstring Paragraph::GetAllText() const
	{
		std::wstring paragraphText;

		for (list<ParagraphItem>::const_iterator iter = paragraphItems.begin(); iter != paragraphItems.end(); ++iter)
			paragraphText += (*iter)->GetAllText();

		return paragraphText + TextMark::ParagraphEnd;
	}

	std::vector<Chpx> Paragraph::GetAllRunProperties(vector<unsigned int>* allRunsOffsets) const
	{
		std::vector<Chpx> AllChpx;

		if (NULL != allRunsOffsets)
		{
			unsigned int paragraphItemOffset = 0;
			
			long nInd = 0;
			for (std::list<ParagraphItem>::const_iterator oIter = paragraphItems.begin(); oIter != paragraphItems.end(); ++oIter)
			{				
				vector<unsigned int> runOffsets;
				vector<Chpx> paragraphItemChpxs = (*oIter)->GetRunProperties(&runOffsets);
				
				if (1 == paragraphItemChpxs.size() && paragraphItems.size() > 1)		// пустых данных не должно быть в конце списка (todo: потестить на документах)
				{
					if ((0 == paragraphItemChpxs[0].GetPrlSize()) && (nInd == paragraphItems.size() - 1))
						break;
				}

				for (size_t i = 0; i < paragraphItemChpxs.size(); ++i)
				{
					AllChpx.push_back(paragraphItemChpxs[i]);
					allRunsOffsets->push_back(paragraphItemOffset + runOffsets[i]);
				}

				paragraphItemOffset += (sizeof(wchar_t) * (*oIter)->GetTextSize());
				
				++nInd;
			}
		}

		return AllChpx;
	}

	void Paragraph::AddProperty (short sprm, void* operand)
	{
		Prl prl( sprm, reinterpret_cast<unsigned char*>( operand ) );
		properties.push_back( prl );
	}

	void Paragraph::AddProperty (const Prl& prl)
	{
		properties.push_back (prl);
	}

	void Paragraph::AddProperties(const vector<Prl>& prls)
	{
		for ( vector<Prl>::const_iterator iter = prls.begin(); iter != prls.end(); iter++ )
			properties.push_back( *iter ); 
	}

	void Paragraph::AddOrReplaceProperties (const vector<Prl>& arr)
	{
		for (vector<Prl>::const_iterator iter = arr.begin(); iter != arr.end(); ++iter)
		{
			RemovePropertyByCode (iter->GetSprmCode());
			AddProperty (*iter); 
		}    
	}

	void Paragraph::RemoveProperty(short sprm, void* operand)
	{
		Prl prl( sprm, reinterpret_cast<unsigned char*>( operand ) );
		properties.remove( prl );  
	}

	void Paragraph::RemovePropertyByCode(unsigned short sprm)
	{
		for (list<Prl>::iterator iter = properties.begin(); iter != properties.end(); ++iter)
		{
			if (iter->GetSprmCode() == sprm)
			{
				properties.erase(iter);
				RemovePropertyByCode(sprm);

				break;
			}
		}
	}

	void Paragraph::RemoveAllProperties()
	{
		properties.clear();
	}

	void Paragraph::SetStyle (short _style)
	{
		style = _style;
	}

	short Paragraph::GetStyle() const
	{
		return style;
	}

	PapxInFkp Paragraph::GetProperties() const
	{
		vector<Prl> prls(properties.begin(), properties.end());
		GrpPrlAndIstd gpai(style, prls);
		return PapxInFkp( gpai );
	}

	unsigned int Paragraph::GetParagraphPropertiesCount() const
	{
		return (unsigned int)properties.size();
	}

	Paragraph::operator std::wstring() const
	{
		std::wstring paragraphText;

		for (list<ParagraphItem>::const_iterator iter = paragraphItems.begin(); iter != paragraphItems.end(); ++iter)
		{
			paragraphText += (*iter)->GetAllText();
		}

		return paragraphText;
	}

	void Paragraph::SetDepth(unsigned int _depth)
	{
		unsigned char PFInTable			=	1;
		int PItap				=	GetDepth();
		unsigned char PFInnerTableCell	=	1;

		RemoveProperty ((short)0x2416, &PFInTable);
		RemoveProperty ((short)0x6649, &PItap);

		depth					=	_depth;

		PItap					=	GetDepth();

		AddProperty ((short)0x2416, &PFInTable);
		AddProperty ((short)0x6649, &PItap);
	}

	unsigned int Paragraph::GetDepth() const
	{
		return depth;
	}

	std::vector<PapxInFkp> Paragraph::GetAllParagraphsProperties(std::vector<unsigned int>* allParagraphsOffsets) const
	{
		vector<PapxInFkp> allParagraphsProperties;

		if ( allParagraphsOffsets != NULL )
		{
			allParagraphsProperties.push_back(GetProperties() );
			allParagraphsOffsets->push_back( 0 );
		}

		return allParagraphsProperties;
	}

	std::vector<TextItemPtr> Paragraph::GetAllParagraphsCopy() const
	{
		vector<TextItemPtr> thisParagraph;

		thisParagraph.push_back(TextItemPtr(static_cast<ITextItem*>(Clone())));

		return thisParagraph;
	}

	std::vector<ITextItem*> Paragraph::GetAllParagraphs()
	{
		vector<ITextItem*> thisParagraph;

		thisParagraph.push_back(this);

		return thisParagraph;
	}

	std::vector<IParagraphItemPtr> Paragraph::GetAllRunsCopy(std::vector<unsigned int>* allRunsOffsets) const
	{
		vector<IParagraphItemPtr> allRuns;

		if ( allRunsOffsets != NULL )
		{
			unsigned int paragraphItemOffset = 0;

			for (list<ParagraphItem>::const_iterator iter = this->paragraphItems.begin(); iter != this->paragraphItems.end(); ++iter)
			{
				vector<unsigned int> paragraphItemOffsets;
				vector<IParagraphItemPtr> paragraphItemRuns = (*iter)->GetAllRunsCopy( &paragraphItemOffsets );

				for (unsigned int i = 0; i < paragraphItemRuns.size(); ++i)
				{
					allRuns.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(paragraphItemRuns[i]->Clone()) ) );
					allRunsOffsets->push_back( paragraphItemOffset + paragraphItemOffsets[i] );
				}

				paragraphItemOffset += ( sizeof(wchar_t) * (*iter)->GetTextSize() );
			}
		}

		return allRuns;
	}

	std::vector<IParagraphItemPtr> Paragraph::GetAllParagraphItemsCopy(std::vector<unsigned int>* allParagraphItemsOffsets) const
	{
		vector<IParagraphItemPtr> allParagraphItems;

		if ( allParagraphItemsOffsets != NULL )
		{
			unsigned int paragraphItemOffset = 0;

			for ( list<ParagraphItem>::const_iterator iter = this->paragraphItems.begin(); iter != this->paragraphItems.end(); iter++ )
			{
				allParagraphItems.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>((*iter)->Clone()) ) );
				allParagraphItemsOffsets->push_back( paragraphItemOffset );
				paragraphItemOffset += ( sizeof(wchar_t) * (*iter)->GetTextSize() );
			}
		}

		return allParagraphItems;
	}

}

namespace ASCDocFileFormat
{
	Paragraph::const_iterator Paragraph::begin() const
	{
		return this->paragraphItems.begin();
	}

	Paragraph::const_iterator Paragraph::end() const
	{
		return this->paragraphItems.end();
	}

	Paragraph::iterator Paragraph::begin()
	{
		return this->paragraphItems.begin();
	}

	Paragraph::iterator Paragraph::end()
	{
		return this->paragraphItems.end();
	}

	Paragraph::const_reverse_iterator Paragraph::rbegin() const
	{
		return this->paragraphItems.rbegin();
	}

	Paragraph::const_reverse_iterator Paragraph::rend() const
	{
		return this->paragraphItems.rend();
	}

	Paragraph::reverse_iterator Paragraph::rbegin()
	{
		return this->paragraphItems.rbegin();
	}

	Paragraph::reverse_iterator Paragraph::rend()
	{
		return this->paragraphItems.rend();
	}

	Paragraph::const_reference Paragraph::front() const
	{
		return *this->begin();
	}

	Paragraph::const_reference Paragraph::back() const
	{
		return *(--this->end());
	}

	Paragraph::reference Paragraph::front()
	{
		return *this->begin();
	}

	Paragraph::reference Paragraph::back()
	{
		return *(--this->end());
	}
}
