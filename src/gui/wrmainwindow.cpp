#include "wrmainwindow.h"

#include <QMenuBar>
#include <QSettings>
#include <filesystem>
#include <iostream>
#include <src/core/wrpaths.h>
#include <src/gui/wrsettings.h>

namespace fs = std::filesystem;

struct WRMainWindow::impl
{
    fs::path dict_fld{};

    QMenu *fileMenu{nullptr};
    QSettings settings{"words_recall.ini", QSettings::IniFormat};
};

namespace {} // namespace

// -----------------------------------------------------------------------------------------------

WRMainWindow::WRMainWindow(QWidget *parent, const fs::path &dict_fld)
    : QMainWindow(parent)
    , pimpl_(new WRMainWindow::impl)
{
    pimpl_->dict_fld = (dict_fld.empty()) ? get_default_dictionaries_fld() : fs::absolute(dict_fld);

    // settings
    if (wr::need_recreate_settings(pimpl_->settings))
        wr::create_default_settings(pimpl_->settings);
    wr::update_settings_from_dir(pimpl_->settings, pimpl_->dict_fld);

    pimpl_->fileMenu = menuBar()->addMenu(tr("&File"));
}

WRMainWindow::~WRMainWindow() {}
