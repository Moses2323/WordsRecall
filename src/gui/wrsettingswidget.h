#pragma once

#include <QWidget>
#include <memory>
#include <src/core/wrsettings.h>

//! \class Settings widget. Opens as a separate window.
class WRSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    WRSettingsWidget(QWidget *parent, WRSettings &wrsettings);
    virtual ~WRSettingsWidget();

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private slots:
    //! Action for 'OK' button. Saves the chosen settings.
    void ok_();
    //! Action for 'Cancel' button. Cancels the chosen settings.
    void cancel_();
    //! Action for 'Refresh' button. Refresh dict files folder.
    void refresh_();

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;

    void toggles_to_checkboxes_();
    void checkboxes_states_to_toggles_();
    void clean_dict_checkboxes_();
};
