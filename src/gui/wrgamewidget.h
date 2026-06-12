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

    void fill_from_settings(const WRSettings &settings);

private slots:
    void start_();
    void reset_();
    void enterWord_();

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;

    void setBeforeGameState_();
    void updateFromGameSession_();
    void setMeaningTextWithAnswer_();
    void finishGameSession_();
};
