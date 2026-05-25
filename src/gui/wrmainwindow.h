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
    /*! \brief Opens a special Settings widget.
     *  
     *  You can close and re-open the settings window, but it won't be
     *  recreated. Qt just hides it when you close.
     */
    void open_settings_();

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;

    void create_menu_actions_();
};
