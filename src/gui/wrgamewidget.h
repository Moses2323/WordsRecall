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
    WRGameWidget(QWidget *parent, WRGameData &gameData, WRSettings &wrsettings);
    virtual ~WRGameWidget();

    WRGameWidget(const WRGameWidget &) = delete;
    WRGameWidget &operator=(const WRGameWidget &) = delete;

private slots:
    void start_();
    void reset_();
    void enterWord_();

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};
