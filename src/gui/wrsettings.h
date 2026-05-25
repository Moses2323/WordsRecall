#pragma once

#include <QSettings>
#include <filesystem>
#include <vector>

struct WRDictToggleSetting
{
    std::filesystem::path dict_file{};
    bool is_active{false};

    bool operator<(const WRDictToggleSetting &oth) const;
};

//! \class
class WRSettings
{
public:
    WRSettings() = default;
    WRSettings(const WRSettings &) = delete;
    WRSettings &operator=(const WRSettings &) = delete;

    QSettings settings{"words_recall.ini", QSettings::IniFormat};
    std::vector<WRDictToggleSetting> toggles;
};

namespace wr {

//! \brief Check if the settings file has to be recreated.
bool need_recreate_settings(const QSettings &settings);

//! \brief Makes the default settings file.
void create_default_settings(QSettings &settings);

//! \brief Updates the settings file based on files in the directory.
void update_settings_from_dir(QSettings &settings, const std::filesystem::path &dict_fld);

} // namespace wr
