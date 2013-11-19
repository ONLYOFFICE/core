#include "stdafx.h"
#include "Paragraph.h"

#include "..\Common\TextMark.h"

namespace AVSDocFileFormat
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

namespace AVSDocFileFormat
{
	void Paragraph::AddParagraphItem(const IParagraphItem& oItem)
	{
		paragraphItems.push_back(ParagraphItem(oItem, runsOffset));
		runsOffset += sizeof(WCHAR) * oItem.GetTextSize();
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

				paragraphItemOffset += (sizeof(WCHAR) * (*oIter)->GetTextSize());
				
				++nInd;
			}
		}

		return AllChpx;
	}

	void Paragraph::AddProperty (short sprm, void* operand)
	{
		Prl prl( sprm, reinterpret_cast<byte*>( operand ) );
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
		Prl prl( sprm, reinterpret_cast<byte*>( operand ) );
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
		byte PFInTable			=	1;
		int PItap				=	GetDepth();
		byte PFInnerTableCell	=	1;

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

				paragraphItemOffset += ( sizeof(WCHAR) * (*iter)->GetTextSize() );
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
				paragraphItemOffset += ( sizeof(WCHAR) * (*iter)->GetTextSize() );
			}
		}

		return allParagraphItems;
	}

}

namespace AVSDocFileFormat
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