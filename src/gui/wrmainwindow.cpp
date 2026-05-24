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

namespace {

struct _DictToggleSetting
{
    fs::path dict_file{};
    bool is_active{false};

    bool operator<(const _DictToggleSetting &oth) const
    {
        return dict_file.filename() < oth.dict_file.filename();
    }
};

bool _need_recreate_settings(const QSettings &settings)
{
    auto keys = settings.allKeys();

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

std::vector<_DictToggleSetting> _read_dir_as_toggles(const fs::path &dict_fld)
{
    size_t n_files = 0;
    for (auto const &_file : fs::directory_iterator(dict_fld))
        n_files++;

    std::vector<_DictToggleSetting> toggles;
    toggles.reserve(n_files);
    for (const fs::path &file : fs::directory_iterator(dict_fld)) {
        toggles.push_back(_DictToggleSetting(fs::absolute(file), false));
    }

    std::sort(toggles.begin(), toggles.end());
    return toggles;
}

void _fill_settings_toggles_with_existing_vals(QSettings &settings,
                                               std::vector<_DictToggleSetting> &toggles)
{
    ReadArraySettingsGuard ras_guard(settings, "dictionaries");
    size_t n_settings = ras_guard.size();
    if (n_settings % 2 != 0) {
        std::stringstream ess;
        ess << "Number of settings for dictionaries cannot be odd, but " << n_settings
            << " was found";
        throw std::runtime_error(ess.str());
    }
    size_t n_files = n_settings / 2;

    for (size_t i_fl = 0; i_fl < n_files; i_fl++) {
        std::string i_fl_str = std::to_string(i_fl);
        std::string filename = settings.value("name_" + i_fl_str).toString().toStdString();

        for (_DictToggleSetting &tg : toggles)
            if (filename == tg.dict_file.filename())
                tg.is_active = settings.value("is_active_" + i_fl_str).toBool();
    }
}

void _update_settings_from_dir(QSettings &settings, const fs::path &dict_fld)
{
    std::vector<_DictToggleSetting> toggles = _read_dir_as_toggles(dict_fld);
    _fill_settings_toggles_with_existing_vals(settings, toggles);

    {
        BeginGroupSettingsGuard guard(settings, "dictionaries");

        settings.remove("");
        for (size_t i = 0; i < toggles.size(); ++i) {
            settings.setValue("name_" + std::to_string(i),
                              QString(toggles[i].dict_file.filename().string().c_str()));
            settings.setValue("is_active_" + std::to_string(i), toggles[i].is_active);
        }
    }

    settings.sync();
}

} // namespace

// -----------------------------------------------------------------------------------------------

WRMainWindow::WRMainWindow(QWidget *parent, const fs::path &dict_fld)
    : QMainWindow(parent)
    , pimpl_(new WRMainWindow::impl)
{
    pimpl_->dict_fld = (dict_fld.empty()) ? get_default_dictionaries_fld() : fs::absolute(dict_fld);

    // settings
    if (_need_recreate_settings(pimpl_->settings))
        _create_default_settings(pimpl_->settings);
    _update_settings_from_dir(pimpl_->settings, pimpl_->dict_fld);

    pimpl_->fileMenu = menuBar()->addMenu(tr("&File"));
}

WRMainWindow::~WRMainWindow() {}
