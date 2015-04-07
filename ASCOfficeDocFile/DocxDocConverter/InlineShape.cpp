

#include "InlineShape.h"

namespace ASCDocFileFormat
{
	InlineShape::InlineShape ()
	{

	}

	InlineShape::InlineShape (const vector<Run>& oRuns)
	{
		for (size_t i = 0; i < oRuns.size(); ++i)
		{
			m_oTextRuns.push_back(ParagraphItem(oRuns[i]));
		}
	}

	InlineShape::~InlineShape()
	{

	}

	IVirtualConstructor* InlineShape::New() const
	{
		return new InlineShape();
	}

	IVirtualConstructor* InlineShape::Clone() const
	{
		return new InlineShape( *this );
	}
}

namespace ASCDocFileFormat
{
	void InlineShape::BuildRuns()  const
	{
		m_oSpecialRuns.clear();

		//static const unsigned char CFLoc		=	0;
		static const unsigned char CFSpec	=	1;

		ASCDocFileFormat::Run oBegin(ASCDocFileFormat::Text((wchar_t)TextMark::FieldBeginMark));

		//oBegin.AddProperty( (short)DocFileFormat::sprmCPicLocation, (void*)&CFLoc );
		oBegin.AddProperty( (short)DocFileFormat::sprmCFSpec, (void*)&CFSpec );

		ASCDocFileFormat::Run oSpecial (ASCDocFileFormat::Text(L" SHAPE  \\* MERGEFORMAT "));

		ASCDocFileFormat::Run oSeperator(ASCDocFileFormat::Text((wchar_t)TextMark::FieldSeparator));
		//oSeperator.AddProperty( (short)DocFileFormat::sprmCPicLocation, (void*)&CFLoc );
		oSeperator.AddProperty( (short)DocFileFormat::sprmCFSpec, (void*)&CFSpec );

		ASCDocFileFormat::Run oEnd(ASCDocFileFormat::Text((wchar_t)TextMark::FieldEndMark));
		//oEnd.AddProperty( (short)DocFileFormat::sprmCPicLocation, (void*)&CFLoc );
		oEnd.AddProperty( (short)DocFileFormat::sprmCFSpec, (void*)&CFSpec );

		m_oSpecialRuns.push_back(ParagraphItem(oBegin));
		m_oSpecialRuns.push_back(ParagraphItem(oSpecial));
		m_oSpecialRuns.push_back(ParagraphItem(oSeperator));
		m_oSpecialRuns.push_back(ParagraphItem(oEnd));

		/*

		ASCDocFileFormat::Run oBegin(ASCDocFileFormat::Text((wchar_t)TextMark::FieldBeginMark));

		oBegin.AddProperty( (short)DocFileFormat::sprmCPicLocation, (void*)&CFLoc );
		oBegin.AddProperty( (short)DocFileFormat::sprmCFSpec, (void*)&CFSpec );

		ASCDocFileFormat::Run oSpecial (ASCDocFileFormat::Text(L" SHAPE  \\* MERGEFORMAT "));

		ASCDocFileFormat::Run oSeperator( ASCDocFileFormat::Text((wchar_t)TextMark::FieldSeparator));
		oSeperator.AddProperty( (short)DocFileFormat::sprmCPicLocation, (void*)&CFLoc );
		oSeperator.AddProperty( (short)DocFileFormat::sprmCFSpec, (void*)&CFSpec );

		ASCDocFileFormat::Run oEnd;
		
		//oEnd.AddRunItem (m_oShape);
		oEnd.AddRunItem (m_oPicture);
		oEnd.AddRunItem (ASCDocFileFormat::Text(TextMark::FieldEndMark));
		
		m_oSpecialRuns.push_back(ParagraphItem(oBegin));
		m_oSpecialRuns.push_back(ParagraphItem(oSpecial));
		m_oSpecialRuns.push_back(ParagraphItem(oSeperator));
		m_oSpecialRuns.push_back(ParagraphItem(oEnd));
		*/
	}

	void InlineShape::AddRun(const Run& oRun)
	{
		m_oTextRuns.push_back(ParagraphItem(oRun));
	}

	void InlineShape::SetShape(const ASCDocFileFormat::CShapeRun& oShape)
	{
		m_oShape	=	oShape;
	}

	void InlineShape::SetPicture(const ASCDocFileFormat::Picture& oPicture)
	{
		m_oPicture	=	oPicture;
	}

	void InlineShape::RemoveAllRuns ()
	{
		m_oTextRuns.clear ();
	}

	unsigned long InlineShape::GetTextSize() const
	{
		return (unsigned long)GetAllText().size();
	}

	std::wstring InlineShape::GetAllText() const
	{
		BuildRuns();

		std::wstring allText;

		for (unsigned int i = 0; i < m_oSpecialRuns.size() - 1; ++i)
		{
			allText += m_oSpecialRuns[i]->GetAllText();
		}

		allText += GetInnerText();
		allText += m_oSpecialRuns.back()->GetAllText();

		return allText;
	}

	std::vector<Chpx> InlineShape::GetRunProperties(vector<unsigned int>* runOffsets) const
	{
		vector<Chpx> allRunsProperties;

		if ( runOffsets != NULL )
		{
			unsigned int allRunsOffset = 0;

			BuildRuns();

			for (unsigned int i = 0; i < ( m_oSpecialRuns.size() - 1 ); ++i)
			{
				vector<unsigned int> runOffset;
				vector<Chpx> runProperties = m_oSpecialRuns[i]->GetRunProperties( &runOffset );

				for ( unsigned int j = 0; j < runProperties.size(); j++ )
				{
					allRunsProperties.push_back( runProperties[j] );
					runOffsets->push_back( allRunsOffset + runOffset[j] );
				}

				allRunsOffset += ( sizeof(wchar_t) * m_oSpecialRuns[i]->GetTextSize() );
			}

			for ( list<ParagraphItem>::const_iterator iter = m_oTextRuns.begin(); iter != m_oTextRuns.end(); iter++ )
			{
				vector<unsigned int> runOffset;
				vector<Chpx> runProperties = ((IParagraphItem*)iter.operator->())->GetRunProperties( &runOffset );

				for ( unsigned int i = 0; i < runProperties.size(); i++ )
				{
					allRunsProperties.push_back( runProperties[i] );
					runOffsets->push_back( allRunsOffset + runOffset[i] );
				}

				allRunsOffset += ( sizeof(wchar_t) * ((IParagraphItem*)iter.operator->())->GetTextSize() );
			}

			vector<unsigned int> runOffset;
			vector<Chpx> runProperties = m_oSpecialRuns.back()->GetRunProperties( &runOffset );

			for ( unsigned int i = 0; i < runProperties.size(); i++ )
			{
				allRunsProperties.push_back( runProperties[i] );
				runOffsets->push_back( allRunsOffset + runOffset[i] );
			}

			allRunsOffset += ( sizeof(wchar_t) * m_oSpecialRuns.back()->GetTextSize() );
		}

		return allRunsProperties;
	}


	unsigned int InlineShape::PrlSize () const
	{
		vector<unsigned int> runOffsets;

		return (unsigned int)GetRunProperties( &runOffsets ).size();
	}

	vector<IParagraphItemPtr> InlineShape::GetAllRunsCopy(vector<unsigned int>* runOffsets) const
	{
		vector<IParagraphItemPtr> allRunsCopy;

		if ( runOffsets )
		{
			BuildRuns();

			unsigned int runOffset = 0;

			for (unsigned int i = 0; i < ( m_oSpecialRuns.size() - 1 ); ++i)
			{
				allRunsCopy.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(m_oSpecialRuns[i]->Clone()) ) );
				runOffsets->push_back( runOffset );
				runOffset += ( sizeof(wchar_t) * m_oSpecialRuns[i]->GetTextSize() );
			}

			for (list<ParagraphItem>::const_iterator iter = m_oTextRuns.begin(); iter != m_oTextRuns.end(); ++iter)
			{
				allRunsCopy.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(((IParagraphItem*)iter.operator->())->Clone()) ) );
				runOffsets->push_back( runOffset );
				runOffset += ( sizeof(wchar_t) * ((IParagraphItem*)iter.operator->())->GetTextSize() );
			}

			allRunsCopy.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(m_oSpecialRuns.back()->Clone()) ) );
			runOffsets->push_back( runOffset );
			runOffset += ( sizeof(wchar_t) * m_oSpecialRuns.back()->GetTextSize() );
		}

		return allRunsCopy;
	}

	vector<CP> InlineShape::GetFieldCharactersPositions() const
	{
		vector<CP> fieldCharactersPositions;

		BuildRuns();

		fieldCharactersPositions.push_back ( CP( 0 ) );
		fieldCharactersPositions.push_back ( CP( m_oSpecialRuns[0]->GetTextSize() + m_oSpecialRuns[1]->GetTextSize() ) );
		fieldCharactersPositions.push_back ( CP( m_oSpecialRuns[0]->GetTextSize() + 
			m_oSpecialRuns[1]->GetTextSize() + 
			m_oSpecialRuns[2]->GetTextSize() + 
			GetInnerText().size()  /*- 2*/ ) );

		return fieldCharactersPositions;
	}

	vector<Fld> InlineShape::GetFieldCharactersProperties() const
	{
		vector<Fld> fieldCharactersProperties;

		fieldCharactersProperties.push_back (Fld (FldChar::FldCharBegin,	Constants::fltSHAPE ) );
		fieldCharactersProperties.push_back (Fld (FldChar::FldCharSeparate, (unsigned char)grffldEnd( true,   true,  true,  true,  true,  true,  true, true ) ) );
		fieldCharactersProperties.push_back (Fld (FldChar::FldCharEnd,		(unsigned char)grffldEnd( false, false, false, false, false, false, false, true ) ) );

		return fieldCharactersProperties;
	}

	std::wstring InlineShape::GetInnerText() const
	{
		wstring strText;

		for (list<ParagraphItem>::const_iterator iter = m_oTextRuns.begin(); iter != m_oTextRuns.end(); ++iter)
		{
			strText += ((IParagraphItem*)iter.operator->())->GetAllText();
		}

		return strText;
	}
}

namespace ASCDocFileFormat
{
	InlineShape::const_iterator InlineShape::begin() const
	{
		return m_oTextRuns.begin();
	}

	InlineShape::const_iterator InlineShape::end() const
	{
		return m_oTextRuns.end();
	}

	InlineShape::iterator InlineShape::begin()
	{
		return m_oTextRuns.begin();
	}

	InlineShape::iterator InlineShape::end()
	{
		return m_oTextRuns.end();
	}

	InlineShape::const_reverse_iterator InlineShape::rbegin() const
	{
		return m_oTextRuns.rbegin();
	}

	InlineShape::const_reverse_iterator InlineShape::rend() const
	{
		return m_oTextRuns.rend();
	}

	InlineShape::reverse_iterator InlineShape::rbegin()
	{
		return m_oTextRuns.rbegin();
	}

	InlineShape::reverse_iterator InlineShape::rend()
	{
		return m_oTextRuns.rend();
	}

	InlineShape::const_reference InlineShape::front() const
	{
		return *begin();
	}

	InlineShape::const_reference InlineShape::back() const
	{
		return *(--end());
	}

	InlineShape::reference InlineShape::front()
	{
		return *begin();
	}

	InlineShape::reference InlineShape::back()
	{
		return *(--end());
	}
}