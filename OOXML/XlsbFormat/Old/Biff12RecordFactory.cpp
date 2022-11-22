#include "Biff12RecordFactory.h"
namespace XLSB {

    CBiff12RecordFactory* CBiff12RecordFactory::Instance()
    {
        static CBiff12RecordFactory factory;
        return &factory;
    }

    std::shared_ptr<CBiff12RecordBase> CBiff12RecordFactory::Create(int typeRecord) {
        CBiff12RecordBase* instance = nullptr;

        // find name in the registry and call factory method.
        auto it = factoryFunctionRegistry.find(typeRecord);
        if (it != factoryFunctionRegistry.end())
        {
            instance = it->second();
        }

        // wrap instance in a shared ptr and return
        if (instance != nullptr)
        {
            return std::shared_ptr<CBiff12RecordBase>(instance);
        }
        else
        {
            return nullptr;
        }
    }

    void CBiff12RecordFactory::RegisterFactoryFunction(int typeRecord, std::function<CBiff12RecordBase*(void)> classFactoryFunction)
    {
        // register the class factory function
        factoryFunctionRegistry.insert(std::make_pair(typeRecord, classFactoryFunction));
    }
}
