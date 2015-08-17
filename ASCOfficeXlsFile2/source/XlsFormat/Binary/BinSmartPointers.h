#pragma once

#include <list>
#include <boost/shared_ptr.hpp>

namespace XLS
{

class CFStream;
typedef boost::shared_ptr<CFStream> CFStreamPtr;

class CFRecord;
typedef boost::shared_ptr<CFRecord> CFRecordPtr;

typedef std::list<CFRecordPtr> CFRecordPtrList;


} // namespace XLS

