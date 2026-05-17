#pragma once

#include <QMainWindow>
#include <filesystem>
#include <memory>

class WRMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    WRMainWindow(QWidget *parent, const std::filesystem::path &dict_fld);
    virtual ~WRMainWindow();

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};
