#pragma once

#include <QSettings>

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
