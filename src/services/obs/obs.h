#ifndef OBS_H
#define OBS_H

#include "service.h"

class OBS: virtual public ServiceBase {
public:
    ~OBS() {}
    void load();
    void unload();
    bool run();
};
#endif