#ifndef BIFF12RECORDFACTORY_H
#define BIFF12RECORDFACTORY_H

#include <unordered_map>
#include <functional>
#include <memory>
#include "Biff12RecordBase.h"

namespace XLSB
{
    class CBiff12RecordFactory
    {
       std::unordered_map<int, std::function<CBiff12RecordBase*(void)>> factoryFunctionRegistry;

    public:
        static CBiff12RecordFactory* Instance();
        std::shared_ptr<CBiff12RecordBase> Create(int typeRecord);
        void RegisterFactoryFunction(int typeRecord, std::function<CBiff12RecordBase*(void)> classFactoryFunction);
    };

    template<class T> class Registrar
    {
    public:
        Registrar(int typeRecord)
        {
            // register the class factory function
            CBiff12RecordFactory::Instance()->RegisterFactoryFunction(typeRecord,
                    [](void) -> CBiff12RecordBase * { return new T();});
        }
    };
}
#endif // BIFF12RECORDFACTORY_H
