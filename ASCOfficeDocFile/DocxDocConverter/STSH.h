#pragma once

#include "LPStshi.h"
#include "LPStd.h"

namespace AVSDocFileFormat
{
	static const char StyleIDs[159][20] =
	{
		"Normal",
		"Heading1",
		"Heading2",
		"Heading3",
		"Heading4",
		"Heading5",
		"Heading6",
		"Heading7",
		"Heading8",
		"Heading9",
		"Index1",
		"Index2",
		"Index3",
		"Index4",
		"Index5",
		"Index6",
		"Index7",
		"Index8",
		"Index9",
		"TOC1",
		"TOC2",
		"TOC3",
		"TOC4",
		"TOC5",
		"TOC6",
		"TOC7",
		"TOC8",
		"TOC9",
		"NormalIndent", 
		"FootnoteText",
		"AnnotationText",
		"Header",
		"Footer",
		"IndexHeading",
		"Caption",
		"ToCaption",
		"EnvelopeAddress",
		"EnvelopeReturn",
		"FootnoteReference",
		"AnnotationReference",
		"LineNumber",
		"PageNumber",
		"EndnoteReference",
		"EndnoteText",
		"TableOfAuthoring",
		"Macro",
		"TOAHeading",
		"List",
		"ListBullet",
		"ListNumber",
		"List2",
		"List3",
		"List4",
		"List5",
		"ListBullet2",
		"ListBullet3",
		"ListBullet4",
		"ListBullet5",
		"ListNumber2",
		"ListNumber3",
		"ListNumber4",
		"ListNumber5",
		"Title",
		"Closing",
		"Signature",
		"NormalCharacter",
		"BodyText",
		"BodyTextIndent",
		"ListContinue",
		"ListContinue2",
		"ListContinue3",
		"ListContinue4",
		"ListContinue5",
		"MessageHeader",
		"Subtitle",
		"Salutation",
		"Date",
		"BodyText1I",
		"BodyText1I2",
		"NoteHeading",
		"BodyText2",
		"BodyText3",
		"BodyTextIndent2",
		"BodyTextIndent3",
		"BlockQuote",
		"Hyperlink",
		"FollowedHyperlink",
		"Strong",
		"Emphasis",
		"NavPane",
		"PlainText",
		"AutoSignature",
		"FormTop",
		"FormBottom",
		"HtmlNormal",
		"HtmlAcronym",
		"HtmlAddress",
		"HtmlCite",
		"HtmlCode",
		"HtmlDfn",
		"HtmlKbd",
		"HtmlPre",
		"htmlSamp",
		"HtmlTt",
		"HtmlVar",
		"TableNormal",
		"AnnotationSubject",
		"NormalList",
		"OutlineList1",
		"OutlineList2",
		"OutlineList3",
		"TableSimple",
		"TableSimple2",
		"TableSimple3",
		"TableClassic1",
		"TableClassic2",
		"TableClassic3",
		"TableClassic4",
		"TableColorful1",
		"TableColorful2",
		"TableColorful3",
		"TableColumns1",
		"TableColumns2",
		"TableColumns3",
		"TableColumns4",
		"TableColumns5",
		"TableGrid1",
		"TableGrid2",
		"TableGrid3",
		"TableGrid4",
		"TableGrid5",
		"TableGrid6",
		"TableGrid7",
		"TableGrid8",
		"TableList1",
		"TableList2",
		"TableList3",
		"TableList4",
		"TableList5",
		"TableList6",
		"TableList7",
		"TableList8",
		"Table3DFx1",
		"Table3DFx2",
		"Table3DFx3",
		"TableContemporary",
		"TableElegant",
		"TableProfessional",
		"TableSubtle1",
		"tableSubtle2",
		"TableWeb1",
		"TableWeb2",
		"TableWeb3",
		"Acetate",
		"TableGrid",
		"TableTheme",
		"Max",
		"User",
		"Null"
	};

	class StyleID
	{
	public:
		StyleID() : styleID(0x0FFE)
		{

		}

		StyleID (unsigned short _styleID) : styleID(_styleID)
		{

		}

		operator unsigned short() const
		{
			return styleID;
		}

		operator short() const
		{
			return (short)styleID;
		}

	private:

		unsigned short styleID;
	};

	class STSH : public IOperand
	{
	public:
		STSH () : bytes(NULL), sizeInBytes(0)
		{
		}

		STSH (const LPStshi& lpstshi, const vector<LPStd>& rglpstd) : rglpstd(rglpstd), bytes(NULL), sizeInBytes(0)
		{
			sizeInBytes = lpstshi.Size();

			for (vector<LPStd>::const_iterator iter = rglpstd.begin(); iter != rglpstd.end(); ++iter)
				sizeInBytes += iter->Size();

			bytes = new byte[sizeInBytes];

			if ( NULL != bytes )
			{
				memset(bytes, 0, sizeInBytes);
				memcpy(bytes, (byte*)lpstshi, lpstshi.Size());

				unsigned int lPStdOffset = 0;

				for (vector<LPStd>::const_iterator iter = rglpstd.begin(); iter != rglpstd.end(); ++iter)
				{
					memcpy( (bytes + lpstshi.Size() + lPStdOffset ), (byte*)(*iter), iter->Size() );
					lPStdOffset += iter->Size();
				}
			}
		}

		STSH (const STSH& oSTSH) : rglpstd(oSTSH.rglpstd), bytes(NULL), sizeInBytes(oSTSH.sizeInBytes)
		{
			bytes = new byte[sizeInBytes];

			if ( NULL != bytes )
			{
				memset ( bytes, 0, sizeInBytes );
				memcpy ( bytes, oSTSH.bytes, sizeInBytes );
			}
		}

		bool operator == (const STSH& oSTSH)
		{
			return ( ( sizeInBytes == oSTSH.sizeInBytes ) && ( memcmp( bytes, oSTSH.bytes, sizeInBytes ) == 0 ) );
		}

		bool operator != (const STSH& oSTSH)
		{
			return !( this->operator == ( oSTSH ) );
		}

		STSH& operator = (const STSH& oSTSH)
		{
			if (*this != oSTSH)
			{
				RELEASEARRAYOBJECTS(bytes);

				sizeInBytes	=	oSTSH.sizeInBytes;
				rglpstd		=	oSTSH.rglpstd;

				bytes		=	new byte[sizeInBytes];
				if (bytes)
					memcpy (bytes, oSTSH.bytes, sizeInBytes );
			}

			return *this;
		}

		virtual ~STSH()
		{
			RELEASEARRAYOBJECTS(bytes);
		}

		inline const LPStd& operator [] (short istd) const
		{
			return rglpstd[istd];
		}

		unsigned int Count() const
		{
			return rglpstd.size();
		}

		virtual operator byte*() const
		{
			return bytes;
		}

		virtual operator const byte*() const
		{
			return (const byte*)bytes;
		}

		virtual unsigned int Size() const
		{
			return sizeInBytes;
		}

	private:

		vector<LPStd>	rglpstd;
		byte*			bytes;
		unsigned int	sizeInBytes;
	};
}