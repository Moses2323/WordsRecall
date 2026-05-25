#pragma once

#include <QWidget>
#include <memory>

class WRSettingsWidget : QWidget
{
    Q_OBJECT
public:
    explicit WRSettingsWidget(QWidget *parent = nullptr);
    virtual ~WRSettingsWidget();

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};
