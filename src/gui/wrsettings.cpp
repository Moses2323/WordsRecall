#include "wrsettings.h"

namespace fs = std::filesystem;

namespace {

//! \class RAII-like for QSettings .beginReadArray().
class ReadArraySettingsGuard
{
    QSettings &settings_;
    bool readOk_{false};
    int size_{-1};

public:
    //! Ouput size from .beginReadArray() method.
    int size() const { return size_; }

    ReadArraySettingsGuard(QSettings &settings, const QAnyStringView &prefix);
    ~ReadArraySettingsGuard();
};

//! \class RAII-like for QSettings .beginGroup().
class BeginGroupSettingsGuard
{
    QSettings &settings_;
    bool beginGroupOk_{false};

public:
    BeginGroupSettingsGuard(QSettings &settings, const QAnyStringView &prefix);
    ~BeginGroupSettingsGuard();
};

ReadArraySettingsGuard::ReadArraySettingsGuard(QSettings &settings, const QAnyStringView &prefix)
    : settings_(settings)
{
    size_ = settings_.beginReadArray(prefix);
    readOk_ = true; // only if no exception occurs
}

ReadArraySettingsGuard::~ReadArraySettingsGuard()
{
    if (readOk_)
        settings_.endArray();
}

BeginGroupSettingsGuard::BeginGroupSettingsGuard(QSettings &settings, const QAnyStringView &prefix)
    : settings_(settings)
{
    settings_.beginGroup(prefix);
    beginGroupOk_ = true;
}

BeginGroupSettingsGuard::~BeginGroupSettingsGuard()
{
    if (beginGroupOk_)
        settings_.endGroup();
}

std::vector<WRDictToggleSetting> _read_dir_as_toggles(const fs::path &dict_fld)
{
    size_t n_files = 0;
    for (auto const &_file : fs::directory_iterator(dict_fld))
        n_files++;

    std::vector<WRDictToggleSetting> toggles;
    toggles.reserve(n_files);
    for (const fs::path &file : fs::directory_iterator(dict_fld)) {
        toggles.push_back(WRDictToggleSetting(fs::absolute(file), false));
    }

    std::sort(toggles.begin(), toggles.end());
    return toggles;
}

void _fill_settings_toggles_with_existing_vals(QSettings &settings,
                                               std::vector<WRDictToggleSetting> &toggles)
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

        for (WRDictToggleSetting &tg : toggles)
            if (filename == tg.dict_file.filename())
                tg.is_active = settings.value("is_active_" + i_fl_str).toBool();
    }
}

} // namespace

// -----------------------------------------------------------------------------------------------

bool WRDictToggleSetting::operator<(const WRDictToggleSetting &oth) const
{
    return dict_file.filename() < oth.dict_file.filename();
}

namespace wr {

bool need_recreate_settings(const QSettings &settings)
{
    auto keys = settings.allKeys();

    if (keys.size() == 0)
        return true;

    if (!keys.contains("dict_format"))
        return true;

    return false;
}

void create_default_settings(QSettings &settings)
{
    settings.setValue("dict_general/dict_format", QString("utf-8"));
    settings.sync();
}

void update_settings_from_dir(QSettings &settings, const fs::path &dict_fld)
{
    std::vector<WRDictToggleSetting> toggles = _read_dir_as_toggles(dict_fld);
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

} // namespace wr
