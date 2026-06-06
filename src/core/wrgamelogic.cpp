#include "wrgamelogic.h"

#include <map>
#include <set>
#include <src/core/dict_parser.h>

namespace {

using MultiMeaningWords = std::map<std::string, std::set<std::string>>;

struct MergedResultRaw
{
    std::vector<DictWord> merged;
    MultiMeaningWords unexpected_multimeaning;
};

QString _unencrypt(wr::DictFileEncoding encoding, const std::string &s)
{
    switch (encoding) {
    case wr::DictFileEncoding::utf8:
        return QString::fromUtf8(s.c_str(), s.size());
    case wr::DictFileEncoding::utf16:
        return QString::fromUtf16(reinterpret_cast<const char16_t *>(s.c_str()),
                                  s.size() / sizeof(char16_t));
    case wr::DictFileEncoding::latin1:
        return QString::fromLatin1(s.c_str(), s.size());
    case wr::DictFileEncoding::not_set:
    default:
        throw wr::unknown_encoding(std::string("Unknown encoding provided to ") + __FUNCTION__);
    }
}

MergedResultRaw _merge_dicts_raw(std::vector<std::vector<DictWord>> &&dicts)
{
    std::vector<DictWord> merged(std::move(dicts[0]));
    dicts[0].resize(0); // just in case
    size_t max_possible_n = merged.size();
    for (const std::vector<DictWord> &one_dict : dicts)
        max_possible_n += one_dict.size();
    merged.reserve(max_possible_n);

    // merge + check meanings repetition
    MultiMeaningWords multi_mean_words;
    for (std::vector<DictWord> &one_dict : dicts) {
        for (DictWord &dword : one_dict) {
            bool already_has = false;
            for (const DictWord &merged_dword : merged) {
                if ((dword.word == merged_dword.word) && (dword.meaning != merged_dword.meaning)) {
                    // should not happen often, so don't want to optimize more
                    already_has = true;
                    multi_mean_words[dword.word].insert(dword.meaning);
                    multi_mean_words[dword.word].insert(merged_dword.meaning);
                }
            }

            if (!already_has)
                merged.push_back(std::move(dword));
        }
    }

    return MergedResultRaw(merged, multi_mean_words);
}

std::vector<WRMultiMeaning> _multimeaning_to_qstring(const MultiMeaningWords &mm_std,
                                                     wr::DictFileEncoding encoding)
{
    std::vector<WRMultiMeaning> mm_qstr;
    mm_qstr.reserve(mm_std.size());

    for (const auto &mm_elem : mm_std) {
        QString word = _unencrypt(encoding, mm_elem.first);
        std::vector<QString> meanings;
        meanings.reserve(mm_elem.second.size());

        for (const std::string &meaning_std : mm_elem.second) {
            meanings.push_back(_unencrypt(encoding, meaning_std));
        }

        mm_qstr.emplace_back(word, meanings);
    }

    return mm_qstr;
}

} // namespace

// -----------------------------------------------------------------------------------------------

namespace wr {

WRMergedResult merge_dicts(const std::vector<WRDictToggleSetting> &toggles,
                           wr::DictFileEncoding encoding)
{
    // parse files separately
    std::vector<std::vector<DictWord>> dicts;
    size_t n_chosen_dicts = 0;
    for (const WRDictToggleSetting &tg : toggles)
        if (tg.is_active)
            ++n_chosen_dicts;
    dicts.reserve(n_chosen_dicts);

    for (const WRDictToggleSetting &tg : toggles)
        if (tg.is_active)
            dicts.push_back(parse_dict_file(tg.dict_file));

    // merge, but only std::string
    MergedResultRaw merged_raw = _merge_dicts_raw(std::move(dicts));
    dicts.resize(0); // just in case

    // to QString
    std::vector<WRMultiMeaning> multimeaning_qstring
        = _multimeaning_to_qstring(merged_raw.unexpected_multimeaning, encoding);
    std::vector<WRGDictWord> merged;
    merged.reserve(merged_raw.merged.size());
    for (const DictWord &dw_std : merged_raw.merged) {
        merged.emplace_back(_unencrypt(encoding, dw_std.word), _unencrypt(encoding, dw_std.meaning));
    }

    return WRMergedResult(std::move(merged), std::move(multimeaning_qstring));
}

} // namespace wr
