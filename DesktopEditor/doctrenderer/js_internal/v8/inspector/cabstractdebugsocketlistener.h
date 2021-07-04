#ifndef CABSTRACTDEBUGSOCKETLISTENER_H
#define CABSTRACTDEBUGSOCKETLISTENER_H

#include "v8-inspector.h"

class CAbstractDebugSocketListener {
public:
    virtual bool hasMessage() const = 0;
    virtual v8_inspector::StringView getMessage() = 0;
    virtual ~CAbstractDebugSocketListener() = default;
};

#endif // CABSTRACTDEBUGSOCKETLISTENER_H
