#include "wrsettings.h"

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
