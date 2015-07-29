#pragma once

#include "OfficeArtContainer.h"

namespace ODRAW
{;

class OfficeArtDggContainer : public OfficeArtContainer
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtDggContainer)
public:
	OfficeArtDggContainer(const OfficeArtClientAnchorType anchor_type) : OfficeArtContainer(0x0F, DggContainer, anchor_type) {}
	XLS::BiffStructurePtr clone() { return XLS::BiffStructurePtr(new OfficeArtDggContainer(*this)); }

};

class OfficeArtSpgrContainer : public OfficeArtContainer
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtSpgrContainer)
public:
	OfficeArtSpgrContainer(const OfficeArtClientAnchorType anchor_type) : OfficeArtContainer(0x0F, SpgrContainer, anchor_type) {}
	XLS::BiffStructurePtr clone() { return XLS::BiffStructurePtr(new OfficeArtSpgrContainer(*this)); }

};

class OfficeArtSpContainer : public OfficeArtContainer
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtSpContainer)
public:
	OfficeArtSpContainer(const OfficeArtClientAnchorType anchor_type) : OfficeArtContainer(0x0F, SpContainer, anchor_type) {}
	XLS::BiffStructurePtr clone() { return XLS::BiffStructurePtr(new OfficeArtSpContainer(*this)); }

};

class OfficeArtClientData : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtClientData)
public:
	OfficeArtClientData() : OfficeArtRecord(0, ClientData) {}
	XLS::BiffStructurePtr clone() { return XLS::BiffStructurePtr(new OfficeArtClientData(*this)); }
	virtual void loadFields(XLS::CFRecord& record) {}
	virtual void storeFields(XLS::CFRecord& record) {}

};

class OfficeArtClientTextbox : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtClientTextbox)
public:
	OfficeArtClientTextbox() : OfficeArtRecord(0, ClientTextbox) {}
	XLS::BiffStructurePtr clone() { return XLS::BiffStructurePtr(new OfficeArtClientTextbox(*this)); }
	virtual void loadFields(XLS::CFRecord& record) {}
	virtual void storeFields(XLS::CFRecord& record) {}

};



typedef boost::shared_ptr<OfficeArtSpgrContainer> OfficeArtSpgrContainerPtr;
typedef boost::shared_ptr<OfficeArtSpContainer> OfficeArtSpContainerPtr;
typedef boost::shared_ptr<OfficeArtClientData> OfficeArtClientDataPtr;
typedef boost::shared_ptr<OfficeArtClientTextbox> OfficeArtClientTextboxPtr;


} // namespace XLS
