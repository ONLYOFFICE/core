#pragma once
#ifndef OOX_SETTINGS_FILE_INCLUDE_H_
#define OOX_SETTINGS_FILE_INCLUDE_H_

#include "./../File.h"
#include "Zoom.h"
#include "ProofState.h"
#include "MathPr.h"
#include "ClrSchemeMapping.h"
#include "ShapeDefaults.h"
#include "property.h"
#include "FootnotePr.h"
#include "EndnotePr.h"
#include "HdrShapeDefaults.h"


namespace OOX
{
	namespace Settings
	{
		class File : public OOX::File
		{
		public:
			File();
			File(const boost::filesystem::wpath& filename);
			virtual ~File();

		public:
			virtual void read(const boost::filesystem::wpath& filename);
			virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

		public:
			virtual const FileType type() const;
			virtual const boost::filesystem::wpath DefaultDirectory() const;
			virtual const boost::filesystem::wpath DefaultFileName() const;

		public:			
			nullable<bool>						EvenAndOddHeaders;

		private:
			nullable<Zoom>						m_zoom;
			nullable<ProofState>				m_proofState;
			nullable<int>						m_defaultTabStop;
			nullable<std::string>				m_characterSpacingControl; 
			nullable<std::string>				m_compat;
			nullable<MathPr>					m_mathPr;
			nullable<FootnotePr>				m_footnotePr;
			nullable<EndnotePr>					m_endnotePr;
			nullable<std::string>				m_themeFontLang;
			nullable<ClrSchemeMapping>			m_clrSchemeMapping;
			nullable<ShapeDefaults>				m_shapeDefaults;
			nullable<std::string>				m_decimalSymbol;
			nullable<std::string>				m_listSeparator;	
			nullable<HdrShapeDefaults>			m_hdrShapeDefaults;
			nullable<int>						m_drawingGridHorizontalSpacing;
			nullable<int>						m_displayHorizontalDrawingGridEvery;
	};
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTING_FILE_INCLUDE_H_