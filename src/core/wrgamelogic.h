/**
 * \file    wrgamelogic.h
 * \brief   Game logic related functions.
 */
#pragma once

#include <core/wrdefined.h>
#include <core/wrgamedata.h>
#include <core/wrsettings.h>
#include <vector>

//! \struct Word with multiple meaning. Not supported by the application => as a warning.
struct WRMultiMeaning
{
    QString word;
    std::vector<QString> meanings;
};

//! \struct Dict merged result.
struct WRMergedResult
{
    //! \brief Merged dicts.
    std::vector<WRGDictWord> merged;
    /*! \brief If multiple meaning for the same word is found across dicts - it goes here.
     *  \details A word with multiple meanings is not supported.
     */
    std::vector<WRMultiMeaning> unexpectedMultimeaning;
};

namespace wr {

/*! Merge all chosen dicts.
 *  
 *  \param toggles     Toggles for chosen dict files. Defines which dict files to merge.
 *  \param encoding    Encoding used for dict files (utf-8 is a common one).
 *  \return Merge result as a single dict with QStrings. 
 *          Also contains the check for words with the same meaning.
 */
WRMergedResult merge_dicts(const std::vector<WRDictToggleSetting> &toggles,
                           wr::DictFileEncoding encoding);

//! \brief Multi-meaning words as a warning message.
QString unexpected_multimeaning_as_warning_message(const std::vector<WRMultiMeaning> &dict_mm);

//! \brief Populate game data from settings.
void fill_game_data_from_settings(const WRSettings &settings, WRGameData &gamedata);

//! \brief Prepare data for the start of the game (after 'Start' button is pressed).
void start_game(WRGameData &gamedata);

/*! Answer action.
 *  
 *  \param answer    User answer.
 *  \param gamedata  Game data.
 *  \return Was the answer correct?
 */
bool answer_action(const QString &answer, WRGameData &gamedata);

} // namespace wr
