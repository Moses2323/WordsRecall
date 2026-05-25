#include "wrmainwindow.h"

#include <QAction>
#include <QMenuBar>
#include <QSettings>
#include <filesystem>
#include <iostream>
#include <src/core/wrpaths.h>
#include <src/gui/wrsettings.h>

namespace fs = std::filesystem;

struct WRMainWindow::impl
{
    //! \brief Main dir for dictionary files.
    fs::path dict_fld{};
    //! \brief All application settings guts.
    WRSettings wrsettings;

    // ---- Qt GUI related

    //! \brief Menu for 'File' tab.
    QMenu *fileMenu{nullptr};
    QAction *aOpenSettings_{nullptr};
};

namespace {} // namespace

// -----------------------------------------------------------------------------------------------

WRMainWindow::WRMainWindow(QWidget *parent, const fs::path &dict_fld)
    : QMainWindow(parent)
    , pimpl_(new WRMainWindow::impl)
{
    pimpl_->dict_fld = (dict_fld.empty()) ? get_default_dictionaries_fld() : fs::absolute(dict_fld);

    // settings
    if (wr::need_recreate_settings(pimpl_->wrsettings.settings))
        wr::create_default_settings(pimpl_->wrsettings.settings);
    wr::update_settings_from_dir(pimpl_->wrsettings.settings, pimpl_->dict_fld);

    createMenuActions_();
}

WRMainWindow::~WRMainWindow() {}

void WRMainWindow::openSettings_()
{
    //! \todo add open settings action
    qDebug() << "openSettings_() action was called";
}

void WRMainWindow::createMenuActions_()
{
    // 'File' menu
    pimpl_->fileMenu = menuBar()->addMenu(tr("&File"));
    pimpl_->aOpenSettings_ = new QAction(tr("&Settings..."), this);
    pimpl_->aOpenSettings_->setStatusTip(tr("Open settings"));
    connect(pimpl_->aOpenSettings_, &QAction::triggered, this, &WRMainWindow::openSettings_);
    pimpl_->fileMenu->addAction(pimpl_->aOpenSettings_);
}
