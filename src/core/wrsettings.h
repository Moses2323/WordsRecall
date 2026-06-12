/**
 * \file    wrsettings.h
 * \brief   Application settings as a struct + functions for it. No GUI.
 */
#pragma once

#include <QSettings>
#include <filesystem>
#include <src/core/wrdefined.h>
#include <vector>

//! \class Toggle for a single dict file.
struct WRDictToggleSetting
{
    std::filesystem::path dictFile{};
    bool isActive{false};

    QString shortFilename() const;

    bool operator<(const WRDictToggleSetting &oth) const;
};

//! \class Application settings data class. No GUI.
class WRSettings
{
public:
    //! \brief Qt settings for INI file.
    QSettings settings{"words_recall.ini", QSettings::IniFormat};
    //! \brief Toggles for dict files, chosen by user.
    std::vector<WRDictToggleSetting> toggles;
    //! \brief Encoding for dict files.
    wr::DictFileEncoding encoding{wr::DictFileEncoding::not_set};

    const std::filesystem::path &getDictFld() const;
    void setDictFld(const std::filesystem::path &dict_fld);

    WRSettings() = default;
    WRSettings(const WRSettings &) = delete;
    WRSettings &operator=(const WRSettings &) = delete;

private:
    //! \brief Main dir for dictionary files.
    std::filesystem::path dictFld_{};
};

namespace wr {

//! \brief Check if the settings file has to be recreated.
bool need_recreate_settings(const QSettings &settings);

//! \brief Makes the default settings file. Does not fill dictionary files info.
void create_default_settings(QSettings &settings);

//! \brief Updates the settings file based on files in the directory.
void update_settings_from_dir(WRSettings &wrsettings);

//! \brief Fills the settings struct from INI file.
void read_settings_from_ini_file(WRSettings &wrsettings);

//! \brief Dump toggles array state to the INI file.
void dump_toggles_to_settings_file(QSettings &settings,
                                   const std::vector<WRDictToggleSetting> &toggles);

} // namespace wr
