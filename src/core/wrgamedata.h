#pragma once

#include <QString>
#include <iostream>
#include <random>
#include <vector>

//! \class A single word converted from std::string through encoding.
struct WRGDictWord
{
    QString word;
    QString meaning;

    operator QString() const;

    WRGDictWord() = default;
    WRGDictWord(const QString &word, const QString &meaning);
};

//! \class Single game session data.
struct SessionData
{
    //! \brief Current round index.
    size_t roundIdx{0};

    /*! \brief If True, next round is just a repeat.
     *  \details Need to ask the player to enter the word correctly before proceeding further.
     */
    bool isNextRepeat{false};
    //! \brief Is all words from this dict finished?
    bool isFinished{false};

    //! \brief Indices of the words that were correctly done by the player.
    std::vector<size_t> correctlyDone;

    //! \brief Shuffled indices to show the dict.
    std::vector<size_t> showIndices;

    size_t n_correct() const;
    size_t n_incorrect() const;
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
    SessionData sessionData{};

    std::mt19937 random_generator{std::random_device{}()};

    WRGameData() = default;

    // only to avoid any pitfalls during the app development for now
    WRGameData(const WRGameData &) = delete;
    WRGameData &operator=(const WRGameData &) = delete;

    //! \brief Get the current word for the game session.
    const WRGDictWord &get_current_word() const;
};
