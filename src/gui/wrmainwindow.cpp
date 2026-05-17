#include "wrmainwindow.h"

#include <src/core/wrpaths.h>

#include <QMenuBar>
#include <QSettings>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

struct WRMainWindow::impl
{
    fs::path dict_fld{};

    QMenu *fileMenu{nullptr};
    QSettings settings{"words_recall.ini", QSettings::IniFormat};
};

namespace {

bool _need_recreate_settings(const QSettings &settings)
{
    auto keys = settings.allKeys();
    qDebug() << keys;
    if (keys.size() == 0)
        return true;

    if (!keys.contains("dict_format"))
        return true;

    return false;
}

void _create_default_settings(QSettings &settings)
{
    settings.setValue("dict_general/dict_format", QString("utf-8"));
    settings.sync();
}

void _update_settings_from_dict(QSettings &settings, const fs::path &dict_fld)
{
    throw std::runtime_error("Not implemented");
}

} // namespace

WRMainWindow::WRMainWindow(QWidget *parent, const fs::path &dict_fld)
    : QMainWindow(parent)
    , pimpl_(new WRMainWindow::impl)
{
    pimpl_->dict_fld = (dict_fld.empty()) ? get_default_dictionaries_fld() : dict_fld;

    // settings
    if (_need_recreate_settings(pimpl_->settings))
        _create_default_settings(pimpl_->settings);
    _update_settings_from_dict(pimpl_->settings, pimpl_->dict_fld);

    pimpl_->fileMenu = menuBar()->addMenu(tr("&File"));

    pimpl_->settings.setValue("dictionaries/name", QString("bra/bra/dict_1.txt"));
    pimpl_->settings.setValue("dictionaries/active", true);
    qDebug() << pimpl_->settings.allKeys();
    qDebug() << pimpl_->settings.value("dictionaries/name").toString();
}

WRMainWindow::~WRMainWindow() {}
