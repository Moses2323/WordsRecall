#pragma once

#include <QString>
#include <iostream>
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
    size_t currentIdx{0};
    //! \brief Number of correct words.
    size_t n_correct{0};
    //! \brief Shuffled indices to show the dict.
    std::vector<size_t> showIndices;

    size_t n_incorrect() const;
    void reset();
};

//! \class All game related data.
struct WRGameData
{
    std::vector<WRGDictWord> mergedDicts;
    SessionData sessionData{};
};
