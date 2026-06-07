#include "wrmainwindow.h"

#include <QAction>
#include <QMenuBar>
#include <QSettings>
#include <filesystem>
#include <iostream>
#include <memory>
#include <src/core/wrgamedata.h>
#include <src/core/wrgamelogic.h>
#include <src/core/wrpaths.h>
#include <src/core/wrsettings.h>
#include <src/gui/wrgamewidget.h>
#include <src/gui/wrsettingswidget.h>

namespace fs = std::filesystem;

struct WRMainWindow::impl
{
    //! \brief All application settings guts.
    WRSettings wrsettings;
    WRGameData gameData;

    // ---- Qt GUI related

    //! \brief Menu for 'File' tab.
    QMenu *fileMenu{nullptr};
    QAction *aOpenSettings_{nullptr};

    std::unique_ptr<WRSettingsWidget> settingsWidget{nullptr};

    WRGameWidget *gameWidget_{nullptr};
};

namespace {} // namespace

// -----------------------------------------------------------------------------------------------

WRMainWindow::WRMainWindow(QWidget *parent, const fs::path &dict_fld)
    : QMainWindow(parent)
    , pimpl_(new WRMainWindow::impl)
{
    fs::path dict_fld_param = (dict_fld.empty()) ? wr::get_default_dictionaries_fld()
                                                 : fs::absolute(dict_fld);
    pimpl_->wrsettings.set_dict_fld(dict_fld_param);

    // fill settings from INI file
    if (wr::need_recreate_settings(pimpl_->wrsettings.settings))
        wr::create_default_settings(pimpl_->wrsettings.settings);
    wr::read_settings_from_ini_file(pimpl_->wrsettings);

    create_menu_actions_();

    pimpl_->gameWidget_ = new WRGameWidget(this, pimpl_->gameData, pimpl_->wrsettings);
    setCentralWidget(pimpl_->gameWidget_);
}

WRMainWindow::~WRMainWindow() {}

void WRMainWindow::open_settings_()
{
    if (!pimpl_->settingsWidget) {
        pimpl_->settingsWidget.reset(new WRSettingsWidget(nullptr, pimpl_->wrsettings));
    }
    pimpl_->settingsWidget->show();
}

void WRMainWindow::create_menu_actions_()
{
    // 'File' menu
    pimpl_->fileMenu = menuBar()->addMenu(tr("&File"));
    pimpl_->aOpenSettings_ = new QAction(tr("&Settings..."), this);
    pimpl_->aOpenSettings_->setStatusTip(tr("Open settings"));
    connect(pimpl_->aOpenSettings_, &QAction::triggered, this, &WRMainWindow::open_settings_);
    pimpl_->fileMenu->addAction(pimpl_->aOpenSettings_);
}
