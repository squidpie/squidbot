#pragma once

#include <typeindex>

#include "event_server.h"
#include "logging.h"
#include "lib/core_context.h"

class ServiceBase;
class ServiceInterfaceBase;

template <class S> class Service;

class ServiceManagerBase;
class ServiceLoader;
class ServiceManager;

typedef void (*create_service_t)(std::string, std::shared_ptr<Context>);

typedef std::unordered_map<std::type_index,
                           std::shared_ptr<ServiceBase>>
    ServiceMap_t;