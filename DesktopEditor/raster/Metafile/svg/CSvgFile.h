#ifndef CSVGFILE_H
#define CSVGFILE_H

#include "../../../graphics/IRenderer.h"
#include "../../../graphics/config.h"
#include "../../../graphics/pro/Fonts.h"

#include "CSvgParser.h"
#include "CSvgStorage.h"

class GRAPHICS_DECL CSvgFile
{
	public:
		CSvgFile();
		~CSvgFile();

		bool ReadFromBuffer(BYTE* pBuffer, unsigned int unSize);
		bool OpenFromFile(const std::wstring& wsFile);
		bool Load(const std::wstring& wsContent);

		void Close();

		void GetBounds(double& dX, double& dY, double& dWidth, double& dHeight) const;

		void SetFontManager(NSFonts::IFontManager* pFontManager);

		bool Draw(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight) const;
	private:
		void Init();

		SVG::CSvgParser  *m_pParser;
		SVG::CSvgStorage *m_pStorage;
};

#endif // CSVGFILE_H
