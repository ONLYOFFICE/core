#pragma once

#include "../BiffStructure.h"
#include "OfficeArtRecordHeader.h"

namespace XLS
{
	class CFRecord;
	
	class GlobalWorkbookInfo;
	typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;
}


namespace ODRAW
{

class OfficeArtRecord : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtRecord)
public:
	OfficeArtRecord(const unsigned char recVer, const unsigned short recType);

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);
	bool IsThisHeaderMine(const OfficeArtRecordHeader& rh);

	// For overriding:
	virtual const unsigned short GetInstanceToStore();
	virtual void loadFields(XLS::CFRecord& record) = 0;
	virtual void storeFields(XLS::CFRecord& record) = 0;

	static const XLS::ElementType	type = XLS::typeOfficeArtRecord;
	OfficeArtRecordHeader rh_own;

	enum OfficeArtRecordType
	{
		DggContainer	= 0xF000,
		BStoreContainer	= 0xF001, // Implemented
		DgContainer		= 0xF002, // Implemented
		SpgrContainer	= 0xF003, // Implemented
		SpContainer		= 0xF004, // Implemented
		SolverContainer	= 0xF005,
		FDGGBlock		= 0xF006, // Implemented
		FBSE			= 0xF007, // Implemented
		FDG				= 0xF008, // Implemented
		FSPGR			= 0xF009, // Implemented
		FSP				= 0xF00A, // Implemented
		FOPT			= 0xF00B, // Implemented

		ClientTextbox	= 0xF00D, // Implemented

		ChildAnchor		= 0xF00F,
		ClientAnchor	= 0xF010, // Implemented
		ClientData		= 0xF011, // Implemented
		FConnectorRule	= 0xF012,

		FArcRule		= 0xF014,

		FCalloutRule	= 0xF017,

		BlipEMF			= 0xF01A,
		BlipWMF			= 0xF01B,
		BlipPICT		= 0xF01C,
		BlipJPEG		= 0xF01D,
		BlipPNG			= 0xF01E,
		BlipDIB			= 0xF01F,
		BlipTIFF		= 0xF020,
		BlipJPEG_clone	= 0xF02A,

		FRITContainer			= 0xF118, // Implemented
		FDGSL					= 0xF119, // Omitted (This record SHOULD<4> be ignored)
		ColorMRUContainer		= 0xF11A, // Implemented
		FPSPL					= 0xF11D,
		SplitMenuColorContainer	= 0xF11E, // Implemented
		SecondaryFOPT			= 0xF121,
		TertiaryFOPT			= 0xF122, // Implemented	


	};
	
	enum OfficeArtClientAnchorType
	{
		CA_Sheet,
		CA_Chart,
		CA_HF
	};

	XLS::GlobalWorkbookInfoPtr	m_pGlobalWorkbookInfoPtr;
};

typedef boost::shared_ptr<OfficeArtRecord> OfficeArtRecordPtr;


} // namespace ODRAW
