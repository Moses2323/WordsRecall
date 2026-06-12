#include "wrgamedata.h"
#include <src/core/wrdefined.h>
#include <sstream>

WRGDictWord::WRGDictWord(const QString &word, const QString &meaning)
    : word(word)
    , meaning(meaning)
{}

WRGDictWord::operator QString() const
{
    return "WRGDictWord(word='" + word + "', meaning='" + meaning + "')";
}

size_t SessionData::n_correct() const
{
    return correctlyDone.size();
}

size_t SessionData::n_incorrect() const
{
    if (roundIdx < n_correct()) {
        std::stringstream ess;
        ess << "SessionData has current index = " << roundIdx
            << ", but number of correct words = " << n_correct();
        throw wr::game_logic_error(ess.str());
    }

    // if the next round is repeat, then `roundIdx` is not yet increased, but the wrong word
    // was already entered by the player => number of incorrect words += 1
    size_t repeat_round_shift = isNextRepeat ? 1 : 0;
    return roundIdx + repeat_round_shift - n_correct();
}

void SessionData::reset()
{
    roundIdx = 0;
    isFinished = false;
    isNextRepeat = false;
    correctlyDone.clear();
    showIndices.clear();
    //! \todo questionable
    showIndices.shrink_to_fit();
}

const WRGDictWord &WRGameData::get_current_word() const
{
    size_t current_idx = sessionData.showIndices[sessionData.roundIdx];
    return mergedDicts[current_idx];
}
