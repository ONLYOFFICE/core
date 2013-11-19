#pragma once

namespace DocFileFormat
{
	class PieceDescriptor
	{
		friend class PieceTable;
	public:
		/// Parses the bytes to retrieve a PieceDescriptor
		PieceDescriptor(unsigned char *bytes, unsigned int size) : fc(0), encoding(ENCODING_INVALID_VALUE), cpStart(0), cpEnd(0)
		{
			if (8 == size)
			{
				//get the fc value
				unsigned int fcValue = FormatUtils::BytesToUInt32(bytes, 2, size);

				//get the flag
				bool flag = FormatUtils::BitmaskToBool((int)fcValue, 0x40000000);

				//delete the flag
				fcValue = fcValue & 0xBFFFFFFF;

				//find encoding and offset
				if (flag)
				{
					this->encoding = ENCODING_WINDOWS_1251;
					this->fc = (unsigned int)( fcValue / 2 );
				}
				else
				{
					this->encoding = ENCODING_UNICODE;
					this->fc = fcValue;
				}
			}
			else
			{
				//!!!TODO!!!
				//throw new ByteParseException("Cannot parse the struct PCD, the length of the struct doesn't match");
			}
		}

	public:
		/// File offset of beginning of piece.
		/// This is relative to the beginning of the WordDocument stream.
		unsigned int fc;
		/// The encoding of the piece
		Encoding encoding;
		int cpStart;
		int cpEnd;
	};
}
