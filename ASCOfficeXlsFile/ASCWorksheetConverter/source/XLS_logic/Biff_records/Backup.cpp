#include "stdafx.h"
#include "Backup.h"

namespace XLS
{;

Backup::Backup()
{
}


Backup::~Backup()
{
}


BaseObjectPtr Backup::clone()
{
	return BaseObjectPtr(new Backup(*this));
}


void Backup::writeFields(CFRecord& record)
{
	record << fBackup;
}


void Backup::readFields(CFRecord& record)
{
	record >> fBackup;
}

} // namespace XLS

