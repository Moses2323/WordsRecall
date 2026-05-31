#pragma once

#include <QWidget>
#include <memory>

//! \class Main widget for the game.
class WRGameWidget : public QWidget
{
    Q_OBJECT
public:
    WRGameWidget(QWidget *parent = nullptr);
    virtual ~WRGameWidget();

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};
