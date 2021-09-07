// J2kFile.h : Declaration of the CJ2kFile

#pragma once
#include "../BgraFrame.h"

namespace Jpeg2000
{
	class GRAPHICS_DECL CJ2kFile
	{
	public:
		bool Open(CBgraFrame* pFrame, const std::wstring& wsSrcPath, const std::wstring& wsXmlOptions, bool isBGRA);
		bool Open(CBgraFrame* pFrame, BYTE* pBuffer, int nSize,      const std::wstring& wsXmlOptions, bool isBGRA);
		bool Save(CBgraFrame* pFrame, const std::wstring& wsSrcPath, const std::wstring& wsXmlOptions);
		bool Open(BYTE** ppData, int& nComponentsCount, int& nWidth, int& nHeight, const std::wstring& wsSrcPath, const std::wstring& wsXmlOptions);

	private:
		void ApplyDecoderOptions(void *pParameters, const std::wstring& sXml);
		long ApplyEncoderOptions(void *pParameters, const std::wstring& sXml);
	};
}
