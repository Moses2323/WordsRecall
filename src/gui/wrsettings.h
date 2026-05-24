#pragma once

#include <QSettings>
#include <filesystem>

namespace wr {

//! \brief Check if the settings file has to be recreated.
bool need_recreate_settings(const QSettings &settings);

//! \brief Makes the default settings file.
void create_default_settings(QSettings &settings);

//! \brief Updates the settings file based on files in the directory.
void update_settings_from_dir(QSettings &settings, const std::filesystem::path &dict_fld);

} // namespace wr