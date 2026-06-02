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

size_t SessionData::n_incorrect() const
{
    if (currentIdx < n_correct) {
        std::stringstream ess;
        ess << "SessionData has current index = " << currentIdx
            << ", but number of correct words = " << n_correct;
        throw wr::game_logic_error(ess.str());
    }
    return currentIdx - n_correct;
}

void SessionData::reset()
{
    currentIdx = 0;
    n_correct = 0;
    showIndices.clear();
    //! \todo questionable
    showIndices.shrink_to_fit();
}
