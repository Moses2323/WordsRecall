/**
 * \file    wrgamedata.h
 * \brief   Game data structures. No GUI. No complicated logic.
 * 
 * Minimum of private fields. The overall idea:
 * - keep data separate from the logic.
 * - data should be as much as possible separated from GUI. GUI is just a representation of the data.
 */
#pragma once

#include <QString>
#include <iostream>
#include <random>
#include <vector>

//! \class A single word converted from std::string through encoding to QString.
struct WRGDictWord
{
    QString word;
    QString meaning;

    operator QString() const;

    WRGDictWord() = default;
    WRGDictWord(const QString &word, const QString &meaning);
};

//! \class Single game session data.
struct WRSessionData
{
    //! \brief Current round index. Changes only incrementally as +1.
    size_t roundIdx{0};

    /*! \brief If True, next round is just a repeat.
     *  \details Need to ask the player to enter the word correctly before proceeding further.
     *           It is done for better memorizing of the word.
     */
    bool isNextRepeat{false};
    //! \brief Is all words from this dict finished?
    bool isFinished{false};

    //! \brief Indices of the words that were correctly done by the player.
    std::vector<size_t> correctlyDone;

    //! \brief Shuffled indices for this game session.
    std::vector<size_t> showIndices;

    //! \brief Number of correctly entered words without any tips.
    size_t n_correct() const;
    /*! \brief Number of already entered incorrect words.
     *  \details The user saw these words and had an opportunity to enter them correctly.
     */
    size_t n_incorrect() const;

    void increase_round();

    //! \brief Full reset of the structure values. Like the game not yet even started.
    void reset();
};

//! \class All game related data.
struct WRGameData
{
    //! \brief All words from dicts will be merged here.
    std::vector<WRGDictWord> mergedDicts;

    //! \brief Message before the start of the game. Can contain a warning message.
    QString initial_message{""};

    //! \brief Current game session data.
    WRSessionData sessionData{};

    //! \brief Random generator engine. Used for shuffling the indices, for example.
    std::mt19937 random_generator{std::random_device{}()};

    WRGameData() = default;

    // only to avoid any pitfalls during the app logic
    WRGameData(const WRGameData &) = delete;
    WRGameData &operator=(const WRGameData &) = delete;

    //! \brief Get the current word for the game session. `mergedDicts[showIndices[roundIdx]]`.
    const WRGDictWord &get_current_word() const;

    //! \brief Check if the dict is empty (when it has 0 words).
    bool empty() const;
};
