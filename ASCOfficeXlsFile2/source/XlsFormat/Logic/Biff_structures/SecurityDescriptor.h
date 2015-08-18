#pragma once

#include "BiffStructure.h"


#if !defined(_WIN32) && !defined(_WIN64)

    typedef struct _ACL {
        unsigned char  AclRevision;
        unsigned char  Sbz1;
        _UINT16   AclSize;
        _UINT16   AceCount;
        _UINT16   Sbz2;
    } ACL;
    typedef ACL *PACL;

    typedef void* PSID;
    typedef _UINT16   SECURITY_DESCRIPTOR_CONTROL, *PSECURITY_DESCRIPTOR_CONTROL;

    typedef struct _SECURITY_DESCRIPTOR
    {
       unsigned char  Revision;
       unsigned char  Sbz1;
       SECURITY_DESCRIPTOR_CONTROL Control;
       PSID Owner;
       PSID Group;
       PACL Sacl;
       PACL Dacl;

    } SECURITY_DESCRIPTOR, *PISECURITY_DESCRIPTOR;
#else
	#include <windows.h>
#endif

namespace XLS
{

class CFRecord;

class SecurityDescriptor : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SecurityDescriptor)
public:
	SecurityDescriptor();
	SecurityDescriptor(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeSecurityDescriptor;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	SECURITY_DESCRIPTOR sd;
};

} // namespace XLS

