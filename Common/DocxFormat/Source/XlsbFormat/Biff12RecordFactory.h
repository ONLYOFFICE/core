#ifndef CBIFF12RECORDFACTORY_H
#define CBIFF12RECORDFACTORY_H

#include <unordered_map>
#include <functional>
#include <memory>
#include "BaseRecord.h"

namespace XLSB {

    class CBiff12RecordFactory
    {
       std::unordered_map<int, std::function<BaseRecord*(void)>> factoryFunctionRegistry;

    public:
        static CBiff12RecordFactory* Instance();
        std::shared_ptr<BaseRecord> Create(int typeRecord);
        void RegisterFactoryFunction(int typeRecord, std::function<BaseRecord*(void)> classFactoryFunction);
    };

    template<class T> class Registrar
    {
    public:
        Registrar(int typeRecord)
        {
            // register the class factory function
            CBiff12RecordFactory::Instance()->RegisterFactoryFunction(typeRecord,
                    [](void) -> BaseRecord * { return new T();});
        }
    };

}
#endif // CBIFF12RECORDFACTORY_H
