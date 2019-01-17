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


#include "Hyperlink.h"

namespace ASCDocFileFormat
{
	Hyperlink::Hyperlink ()
	{

	}

	Hyperlink::Hyperlink (const vector<Run>& runs)
	{
		for (size_t i = 0; i < runs.size(); ++i)
		{
			textRuns.push_back(ParagraphItem(runs[i]));
		}
	}
	Hyperlink::~Hyperlink()
	{

	}
}

namespace ASCDocFileFormat
{
	void Hyperlink::BuildUrlRuns() const
	{
		std::wstring text;

		specialRuns.clear();

		text.push_back( TextMark::FieldBeginMark );

		ASCDocFileFormat::Run fieldBeginRun( ASCDocFileFormat::Text( text.c_str() ) );
		fieldBeginRun.AddProperty( (short)DocFileFormat::sprmCFSpec, (void*)&CFSpec );

		text		=	wstring( _T( " HYPERLINK" ) );

		if ( !url.empty() )
		{
			text	+=	wstring( _T( " \"" ) );
			text	+=	url;
			text.push_back( _T( '"' ) );
			text.push_back( _T( ' ' ) );
		}

		if ( !locationInTheFile.empty() )
		{
			text	+=	wstring( _T( " \\l \"" ) );
			text	+=	locationInTheFile;
			text.push_back( _T( '"' ) );
		}

		ASCDocFileFormat::Run specialRun (ASCDocFileFormat::Text(text.c_str()));

		text.clear();
		text.push_back( TextMark::FieldSeparator );

		ASCDocFileFormat::Run fieldSeperatorRun( ASCDocFileFormat::Text( text.c_str() ) );
		fieldSeperatorRun.AddProperty( (short)DocFileFormat::sprmCFSpec, (void*)&CFSpec );

		text.clear();
		text.push_back( TextMark::FieldEndMark );

		ASCDocFileFormat::Run fieldEndRun( ASCDocFileFormat::Text( text.c_str() ) );
		fieldEndRun.AddProperty( (short)DocFileFormat::sprmCFSpec, (void*)&CFSpec );

		specialRuns.push_back( ParagraphItem( fieldBeginRun ) );
		specialRuns.push_back( ParagraphItem( specialRun ) );
		specialRuns.push_back( ParagraphItem( fieldSeperatorRun ) );
		specialRuns.push_back( ParagraphItem( fieldEndRun ) );
	}

	/*========================================================================================================*/

	void Hyperlink::AddRun( const Run& _run )
	{
		textRuns.push_back( ParagraphItem( _run ) );
	}

	/*========================================================================================================*/

	void Hyperlink::SetURL( const wchar_t* _url )
	{
		if ( _url != NULL )
		{
			url = wstring( _url );
		}
	}

	wstring Hyperlink::GetURL() const
	{
		return url;
	}

	/*========================================================================================================*/

	void Hyperlink::SetLocationInTheFile( const wchar_t* _locationInTheFile )
	{
		if ( _locationInTheFile != NULL )
		{
			locationInTheFile = wstring( _locationInTheFile );
		}  
	}

	wstring Hyperlink::GetLocationInTheFile() const
	{
		return locationInTheFile;
	}

	/*========================================================================================================*/

	wstring Hyperlink::GetHyperlinkText() const
	{
		wstring hyperlinkText;

		for (list<ParagraphItem>::const_iterator iter = textRuns.begin(); iter != textRuns.end(); ++iter)
		{
			hyperlinkText += (*iter)->GetAllText();
		}

		return hyperlinkText;
	}


	/*========================================================================================================*/

	unsigned long Hyperlink::GetTextSize() const
	{
		return (unsigned long)GetAllText().size();
	}

	/*========================================================================================================*/

	wstring Hyperlink::GetAllText() const
	{
		BuildUrlRuns();

		wstring allText;

		for (unsigned int i = 0; i < specialRuns.size() - 1; ++i)
		{
			allText += specialRuns[i]->GetAllText();
		}

		allText += GetHyperlinkText();
		allText += specialRuns.back()->GetAllText();

		return allText;
	}

	/*========================================================================================================*/

	vector<Chpx> Hyperlink::GetRunProperties( vector<unsigned int>* runOffsets ) const
	{
		vector<Chpx> allRunsProperties;

		if ( runOffsets != NULL )
		{
			unsigned int allRunsOffset = 0;

			this->BuildUrlRuns();

			for ( unsigned int i = 0; i < ( this->specialRuns.size() - 1 ); i++ )
			{
				vector<unsigned int> runOffset;
				vector<Chpx> runProperties = this->specialRuns[i]->GetRunProperties( &runOffset );

				for ( unsigned int j = 0; j < runProperties.size(); j++ )
				{
					allRunsProperties.push_back( runProperties[j] );
					runOffsets->push_back( allRunsOffset + runOffset[j] );
				}

				allRunsOffset += ( sizeof(wchar_t) * this->specialRuns[i]->GetTextSize() );
			}

			for ( list<ParagraphItem>::const_iterator iter = this->textRuns.begin(); iter != this->textRuns.end(); iter++ )
			{
				vector<unsigned int> runOffset;
				vector<Chpx> runProperties = (*iter)->GetRunProperties( &runOffset );

				for ( unsigned int i = 0; i < runProperties.size(); i++ )
				{
					allRunsProperties.push_back( runProperties[i] );
					runOffsets->push_back( allRunsOffset + runOffset[i] );
				}

				allRunsOffset += ( sizeof(wchar_t) * (*iter)->GetTextSize() );
			}

			vector<unsigned int> runOffset;
			vector<Chpx> runProperties = this->specialRuns.back()->GetRunProperties( &runOffset );

			for ( unsigned int i = 0; i < runProperties.size(); i++ )
			{
				allRunsProperties.push_back( runProperties[i] );
				runOffsets->push_back( allRunsOffset + runOffset[i] );
			}

			allRunsOffset += ( sizeof(wchar_t) * this->specialRuns.back()->GetTextSize() );
		}

		return allRunsProperties;
	}

	/*========================================================================================================*/

	unsigned int Hyperlink::PrlSize () const
	{
		vector<unsigned int> runOffsets;

		return (unsigned int)GetRunProperties( &runOffsets ).size();
	}

	/*========================================================================================================*/

	vector<IParagraphItemPtr> Hyperlink::GetAllRunsCopy( vector<unsigned int>* runOffsets ) const
	{
		vector<IParagraphItemPtr> allRunsCopy;

		if ( runOffsets != NULL )
		{
			this->BuildUrlRuns();

			unsigned int runOffset = 0;

			for ( unsigned int i = 0; i < ( this->specialRuns.size() - 1 ); i++ )
			{
				allRunsCopy.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(this->specialRuns[i]->Clone()) ) );
				runOffsets->push_back( runOffset );
				runOffset += ( sizeof(wchar_t) * this->specialRuns[i]->GetTextSize() );
			}

			for ( list<ParagraphItem>::const_iterator iter = this->textRuns.begin(); iter != this->textRuns.end(); iter++ )
			{
				allRunsCopy.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>((*iter)->Clone()) ) );
				runOffsets->push_back( runOffset );
				runOffset += ( sizeof(wchar_t) * (*iter)->GetTextSize() );
			}

			allRunsCopy.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(this->specialRuns.back()->Clone()) ) );
			runOffsets->push_back( runOffset );
			runOffset += ( sizeof(wchar_t) * this->specialRuns.back()->GetTextSize() );
		}

		return allRunsCopy;
	}

	/*========================================================================================================*/

	IVirtualConstructor* Hyperlink::New() const
	{
		return new Hyperlink();
	}

	/*========================================================================================================*/

	IVirtualConstructor* Hyperlink::Clone() const
	{
		return new Hyperlink( *this );
	}

	/*========================================================================================================*/

	vector<CP> Hyperlink::GetFieldCharactersPositions() const
	{
		vector<CP> fieldCharactersPositions;

		BuildUrlRuns();

		fieldCharactersPositions.push_back ( CP( 0 ) );
		fieldCharactersPositions.push_back ( CP( specialRuns[0]->GetTextSize() + specialRuns[1]->GetTextSize() ) );
		fieldCharactersPositions.push_back ( CP( specialRuns[0]->GetTextSize() + specialRuns[1]->GetTextSize() + specialRuns[2]->GetTextSize() + GetHyperlinkText().size() ) );
		
		//int m0 = specialRuns[0]->GetTextSize();
		//int m1 = specialRuns[1]->GetTextSize();
		//int m2 = specialRuns[2]->GetTextSize();
		//int m3 = specialRuns[3]->GetTextSize();
		//int m4 = GetHyperlinkText().size();

		//fieldCharactersPositions.push_back ( CP( 0 ) );
		//fieldCharactersPositions.push_back ( CP( specialRuns[0]->GetTextSize() + specialRuns[1]->GetTextSize()) );
		//fieldCharactersPositions.push_back ( CP( 
		//	//specialRuns[0]->GetTextSize() 
		//	specialRuns[1]->GetTextSize() + 2
		//	//+ specialRuns[2]->GetTextSize()
		//	//+ specialRuns[3]->GetTextSize()  
		//	+ GetHyperlinkText().size()
		//	));

		return fieldCharactersPositions;
	}

	/*========================================================================================================*/

	vector<Fld> Hyperlink::GetFieldCharactersProperties() const
	{
		vector<Fld> fieldCharactersProperties;

		fieldCharactersProperties.push_back (Fld (FldChar::FldCharBegin, Constants::fltHYPERLINK ) );
		fieldCharactersProperties.push_back (Fld (FldChar::FldCharSeparate, 0 ) );
		fieldCharactersProperties.push_back (Fld (FldChar::FldCharEnd, (unsigned char)grffldEnd( false, false, false, false, false, false, false, true ) ) );

		return fieldCharactersProperties;
	}
}

namespace ASCDocFileFormat
{
	Hyperlink::const_iterator Hyperlink::begin() const
	{
		return textRuns.begin();
	}

	Hyperlink::const_iterator Hyperlink::end() const
	{
		return textRuns.end();
	}

	Hyperlink::iterator Hyperlink::begin()
	{
		return textRuns.begin();
	}

	Hyperlink::iterator Hyperlink::end()
	{
		return textRuns.end();
	}

	Hyperlink::const_reverse_iterator Hyperlink::rbegin() const
	{
		return textRuns.rbegin();
	}

	Hyperlink::const_reverse_iterator Hyperlink::rend() const
	{
		return textRuns.rend();
	}

	Hyperlink::reverse_iterator Hyperlink::rbegin()
	{
		return textRuns.rbegin();
	}

	Hyperlink::reverse_iterator Hyperlink::rend()
	{
		return textRuns.rend();
	}

	Hyperlink::const_reference Hyperlink::front() const
	{
		return *begin();
	}

	Hyperlink::const_reference Hyperlink::back() const
	{
		return *(--end());
	}

	Hyperlink::reference Hyperlink::front()
	{
		return *begin();
	}

	Hyperlink::reference Hyperlink::back()
	{
		return *(--end());
	}
}