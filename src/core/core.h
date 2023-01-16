#pragma once

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

class RunActionBase;
class RunActionContextBase;
class RunnerBase;
template <class R> class Runner;

class Event;
class EventServerBase;
class EventClientBase;

class Context;
class CoreContext;

class PluginBase;
class PluginManagerBase;

class ServiceBase;
class ServiceInterfaceBase;
class ServiceDataBase;
template <class S> class Service;
class ServiceLoader;
class ServiceManagerBase;

typedef std::unordered_map<std::type_index,
                           std::pair<std::string, std::shared_ptr<ServiceBase>>>
    ServiceMap_t;
typedef void (*create_service_t)(std::string, std::shared_ptr<Context>);
