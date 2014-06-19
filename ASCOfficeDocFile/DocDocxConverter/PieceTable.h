#pragma once

#include "PieceDescriptor.h"
#include "FileInformationBlock.h"

namespace DocFileFormat
{
	class PieceTable
	{
		friend class DocumentMapping;
		friend class MainDocumentMapping;
		friend class HeaderMapping;
		friend class FooterMapping;
		friend class FootnotesMapping;
		friend class EndnotesMapping;
		friend class CommentsMapping;
		friend class Table;
		friend class TextboxMapping;
		friend class NumberingMapping;

	public:
		~PieceTable();
		// Parses the pice table and creates a list of PieceDescriptors.
		PieceTable(FileInformationBlock* fib, IStream* tableStream);
		std::vector<WCHAR>* GetAllEncodingText(IStream* wordStream);
		std::vector<WCHAR>* GetEncodingChars(int fcStart, int fcEnd, IStream* wordStream);
		std::vector<WCHAR>* GetChars (int fcStart, int fcEnd, int cp, IStream* word);

	private:
		bool ReadSymbolsBuffer(int pos, int size, Encoding encoding, IStream* word, std::vector<WCHAR>* encodingChars);

	public:

		// A list of PieceDescriptor standing for each piece of text.
		std::list<PieceDescriptor> Pieces;
		// A dictionary with character positions as keys and the matching FCs as values
		std::map<int, int>* FileCharacterPositions;
		// A dictionary with file character positions as keys and the matching CPs as values
		std::map<int, int>* CharacterPositions;

		std::list<PieceDescriptor>::iterator	m_carriageIter;
	};
}
