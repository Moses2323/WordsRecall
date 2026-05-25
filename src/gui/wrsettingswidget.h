#pragma once

#include <QWidget>
#include <memory>
#include <src/gui/wrsettings.h>

class WRSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    WRSettingsWidget(QWidget *parent, WRSettings &wrsettings);
    virtual ~WRSettingsWidget();

public slots:
    void refresh();

private slots:
    void ok_();
    void cancel_();

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;

    void toggles_to_checkboxes_();
    void checkboxes_states_to_toggles_();
    void clean_dict_checkboxes_();
};
