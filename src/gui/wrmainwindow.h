#pragma once

#include <QMainWindow>
#include <memory>

class WRMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    WRMainWindow();
    virtual ~WRMainWindow();

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};
