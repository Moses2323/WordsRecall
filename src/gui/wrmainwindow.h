#pragma once

#include <QMainWindow>
#include <filesystem>
#include <memory>

//! Main window for the application.
class WRMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    WRMainWindow(QWidget *parent, const std::filesystem::path &dict_fld);
    virtual ~WRMainWindow();

private slots:
    void openSettings_();

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;

    void createMenuActions_();
};
