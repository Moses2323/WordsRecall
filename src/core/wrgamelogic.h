#pragma once

#include <src/core/wrgamedata.h>
#include <src/core/wrsettings.h>
#include <vector>

namespace wr {

std::vector<WRGDictWord> merge_dicts(const std::vector<WRDictToggleSetting> &toggles);

} // namespace wr
