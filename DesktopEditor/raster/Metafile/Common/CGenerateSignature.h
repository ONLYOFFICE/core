#ifndef CGENERATESIGNATURE_H
#define CGENERATESIGNATURE_H

#include <string>
#include <vector>

#include "../../../common/File.h"

#include "../../../graphics/config.h"

namespace MetaFile
{
	class GRAPHICS_DECL CGenerateWmfSignature
	{
	public:
		CGenerateWmfSignature();
		CGenerateWmfSignature(unsigned short ushWidth, unsigned short ushHeight);

		void SetSize(unsigned short ushWidth, unsigned short ushHeight);
		void SetFont(short shFontHeight, const std::wstring& wsFontName);
		void SetBKColor(char chR, char chG, char chB);
		void SetBKColor(int nColor);

		bool GenerateSignature(const std::wstring& wsFilePath, const std::vector<std::wstring>& arStrings, bool bIsUnsigned = false);
	private:

		void UpdateSizes(unsigned int& unMaxSize, unsigned int& unSize, const std::vector<std::wstring>& arStrings, bool bIsUnsigned);

		void GenerateHeader(const std::vector<std::wstring>& arStrings, bool bIsUnsigned);
		void GenerateBKMode();
		void GeneratePen(unsigned int unColor, unsigned int unWidth);
		void GenerateMiddleLine();
		void GenerateLine(short shX1, short shY1, short shX2, short shY2);
		void GeneratePolyLine(const std::vector<std::pair<short, short>>& arPoints);
		void GenerateRegion(short shX1, short shY1, short shX2, short shY2);
		void GenerateBrush(int nColor);
		void GeneratePaintRegion();
		void GenerateBackground();
		void GenerateCheckmark();
		void GenerateString(const std::wstring& wsString, unsigned int unLength = 0);
		void GenerateFont(short shFontHeight, const std::wstring& wsFontName);
		void GenerateSelectObjects();
		void GenerateSelectObject(unsigned short ushIndex);
		void GenerateDeleteObject(unsigned short ushIndex);
		void GenerateStrings(const std::vector<std::wstring>& arStrings);
		void GenerateString(const std::wstring& wsString, unsigned short ushX, unsigned short ushY);
		void GenerateCrossHair();
		void GenerateEof();

		NSFile::CFileBinary m_oFile;

		unsigned short m_ushWidth;
		unsigned short m_ushHeight;

		short          m_shFontHeight;
		std::wstring   m_wsFontName;

		int            m_nBKColor;

		short          m_shObjectIndex;
	};
}
#endif // CGENERATESIGNATURE_H
