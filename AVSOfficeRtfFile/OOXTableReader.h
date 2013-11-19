#pragma once

class OOXTableReader
{
public: bool Parse(  ReaderParameter oParam ,RtfTable& oOutputTable  );
public: void ApplyParagraphProperty( RtfTable& oOutputTable );
};