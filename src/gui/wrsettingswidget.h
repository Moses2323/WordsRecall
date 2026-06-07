#pragma once

#include <QWidget>
#include <memory>
#include <src/core/wrsettings.h>
#include <src/gui/wrmainwindow.h>

//! \class Settings widget. Opens as a separate window.
class WRSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    /*! \brief Constructor.
     *  
     *  \param parent      Qt widget parent pointer.
     *  \param wrsettings  Ref to widget-independent settings.
     */
    WRSettingsWidget(WRMainWindow *parent, WRSettings &wrsettings);
    virtual ~WRSettingsWidget();

protected:
    //! \brief Action when the Settings window is closed (clicked X).
    virtual void closeEvent(QCloseEvent *event) override;

private slots:
    //! \brief Action for 'OK' button. Saves the chosen settings.
    void ok_();
    //! \brief Action for 'Cancel' button. Cancels the chosen settings.
    void cancel_();
    //! \brief Action for 'Refresh' button. Refresh dict files folder.
    void refresh_();

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;

    //! \brief Creates checkboxes widgets for each dict file based on std::vector.
    void toggles_to_checkboxes_();
    //! \brief Checkboxes widget states (chosen/not) transfer to std::vector with toggle states.
    void checkboxes_states_to_toggles_();
    //! \brief Destroys the dict files checkboxes widgets.
    void clean_dict_checkboxes_();
};
