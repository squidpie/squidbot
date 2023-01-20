<!--
    README.md
    Copyright (C) 2023  Squidpie
-->
# squidbot
Stream Automation Framework

## Overview
Squidbot is a Stream Automation Framework designed to be a persistent hosted service.
The application is composed of three parts: The Core, Services, & Plugins.

#### Core
The core of Squidbot consists of an EventServer, ServiceManager, and PluginManager.
###### Event Server
This modules receives events from services and routes them to subscriber clients in plugins and other services.
###### Service/Plugin Manager
These modules are responsible for loading shared libraries from application context, and then providing an interface for loading/unloading/reloading individual services & plugins.

#### Services
Service modules are shared libraries loaded dynamically from a configured directory. <!-- insert link to sourcedoc for this variable when we create it -->
A service operates on an external interface, such as OBS or Twitch API calls, and generates event messages.
A service also provides a plugin interface, allowing for plugins to query or modify data that the service operates on.

#### Plugins
Plugin modules are user-compiled shared libraries loaded dynamically from a configured directory. <!-- insert link to sourcedoc for this variable when we create it -->
Plugins execute a set of actions <[trigger_action](https://docs.hdoc.io/squidpie/Squidbot/rA08D6B5985765040.html#55F1B715F183733C)> in response to an event trigger <[is_trigger](https://docs.hdoc.io/squidpie/Squidbot/rB5FB7492F026146A.html#FD94C56AC56CFE06)>.
Plugins can interface with Services to acquire and modify external data.

## Documentation
### Source
Source documentation is generated using hdoc: [Squidbot hdoc](https://docs.hdoc.io/squidpie/Squidbot/index.html)

## Setup
### Installation
-- WIP --
### Configuration
-- WIP --

## Development
### Plugins
-- WIP --
### Services
-- WIP --