#ifndef BIFF12RECORDCONTINUEDBASE_H
#define BIFF12RECORDCONTINUEDBASE_H

#include "Biff12RecordBase.h"
#include <memory>

namespace XLSB
{
    class CBiff12RecordContinuedBase: public CBiff12RecordBase
    {
        public:
            std::list<std::shared_ptr<CBiff12RecordBase>> listRecords;
            CBiff12RecordContinuedBase();
    };
}
#endif // BIFF12RECORDCONTINUEDBASE_H
