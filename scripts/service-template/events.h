/*
events.h
Copyright (C) 2023  Squidpie
 */

#pragma once

#include <string>

#include "utils/uuid.h"

// NOLINTNEXTLINE
const std::string TEMPLATENAME_NAMESPACE = "";

struct TemplateNameEvents {
  uuid_t TEMPLATENAME_EVENT_TYPE =
      uuid(TEMPLATENAME_NAMESPACE + "TEMPLATENAME_EVENT_TYPE");
};

static inline TemplateNameEvents TEMPLATENAME_EVENTS;
