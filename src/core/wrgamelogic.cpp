#include "wrgamelogic.h"

#include <src/core/dict_parser.h>

namespace {} // namespace

namespace wr {

std::vector<WRGDictWord> merge_dicts(const std::vector<WRDictToggleSetting> &toggles)
{
    std::vector<std::vector<DictWord>> dicts;
    size_t n_chosen_dicts = 0;
    for (const WRDictToggleSetting &tg : toggles)
        if (tg.is_active)
            ++n_chosen_dicts;
    dicts.reserve(n_chosen_dicts);

    for (const WRDictToggleSetting &tg : toggles)
        if (tg.is_active)
            dicts.push_back(parse_dict_file(tg.dict_file));

    return std::vector<WRGDictWord>{};
}

} // namespace wr