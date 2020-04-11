/*
 * Copyright 2020 Xinyue Lu
 *
 * DualSynth bridge - plugin.
 *
 */

#pragma once

#include "version.hpp"
#include "tmedian.hpp"

namespace Plugin {
  const char* Identifier = "in.7086.neo_tmedian";
  const char* Namespace = "neo_tmedian";
  const char* Description = "Neo Temporal Median Filter " PLUGIN_VERSION;
}

std::vector<register_vsfilter_proc> RegisterVSFilters()
{
  return std::vector<register_vsfilter_proc> { VSInterface::RegisterFilter<TMedian> };
}

std::vector<register_avsfilter_proc> RegisterAVSFilters()
{
  return std::vector<register_avsfilter_proc> { AVSInterface::RegisterFilter<TMedian> };
}
