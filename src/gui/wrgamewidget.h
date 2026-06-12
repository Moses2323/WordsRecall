/**
 * \file    wrgamewidget.h
 * \brief   Game related widget (in the center of app).
 */
#pragma once

#include <QWidget>
#include <memory>
#include <src/core/wrgamedata.h>
#include <src/core/wrsettings.h>

//! \class Main widget for the game.
class WRGameWidget : public QWidget
{
    Q_OBJECT
public:
    WRGameWidget(QWidget *parent, WRGameData &gameData);
    virtual ~WRGameWidget();

    WRGameWidget(const WRGameWidget &) = delete;
    WRGameWidget &operator=(const WRGameWidget &) = delete;

    //! \brief Fill the widget from settings. Sets the "before game start" state.
    void fill_from_settings(const WRSettings &settings);

private slots:
    //! \brief Action after pressing the "Start" button.
    void start_();
    //! \brief Action after pressing the "Reset" button.
    void reset_();
    //! \brief Action after the user entered the word in the LineEditor (and pressed Enter).
    void enterWord_();

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;

    void setBeforeGameState_();

    //! \brief Update the GameWidget sub-widgets based on the current game session struct state.
    void updateFromGameSession_();

    /*! \brief Update the text window for word's meaning to provide the answer.
     *  
     *  If the user entered the word incorrectly, we want to give him the answer and
     *  ask him to enter the word correctly.
     */
    void setMeaningTextWithAnswer_();

    //! \brief Put the state after the game session is over (all words have been seen).
    void finishGameSession_();
};
