#include "wrsettingswidget.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include <src/core/wrsettings.h>
#include <sstream>
#include <vector>

struct WRSettingsWidget::impl
{
    // Settings guts.
    WRSettings &wrsettings;
    WRMainWindow *parentMainWindow{nullptr};

    QScrollArea *dictSelectionWidgetScrollArea{nullptr};
    QVBoxLayout *mainLayout{nullptr};
    // Widget for selecting dict files.
    QGroupBox *dictSelectionWidget{nullptr};
    // Dict files selection checkboxes.
    std::vector<QCheckBox *> toggleBoxes_;

    // Settings widget buttons.
    QHBoxLayout *buttonsLayout{nullptr};
    QPushButton *okButton{nullptr};
    QPushButton *cancelButton{nullptr};
    QPushButton *refreshButton{nullptr};

    impl(WRSettings &wrsettings, WRMainWindow *parentMainWindow)
        : wrsettings(wrsettings)
        , parentMainWindow(parentMainWindow)
    {}
};

namespace {

constexpr int _MIN_SIZE_X = 50;
constexpr int _MIN_SIZE_Y = 50;

} // namespace

// -----------------------------------------------------------------------------------------------

WRSettingsWidget::WRSettingsWidget(QWidget *parent,
                                   WRSettings &wrsettings,
                                   WRMainWindow *main_window)
    : QWidget(parent)
    , pimpl_(new WRSettingsWidget::impl(wrsettings, main_window))
{
    setMinimumSize(_MIN_SIZE_X, _MIN_SIZE_Y);

    // layouts    
    pimpl_->mainLayout = new QVBoxLayout;
    pimpl_->dictSelectionWidgetScrollArea = new QScrollArea;
    pimpl_->dictSelectionWidgetScrollArea->setMinimumSize(_MIN_SIZE_X, _MIN_SIZE_Y);
    pimpl_->dictSelectionWidgetScrollArea->setSizeAdjustPolicy(
        QScrollArea::SizeAdjustPolicy::AdjustToContents);
    pimpl_->dictSelectionWidgetScrollArea->setContentsMargins(0, 0, 0, 0);
    pimpl_->mainLayout->addWidget(pimpl_->dictSelectionWidgetScrollArea);

    pimpl_->buttonsLayout = new QHBoxLayout;

    pimpl_->okButton = new QPushButton(tr("&OK"));
    pimpl_->cancelButton = new QPushButton(tr("&Cancel"));
    pimpl_->refreshButton = new QPushButton(tr("&Refresh"));

    pimpl_->buttonsLayout->addWidget(pimpl_->okButton);
    pimpl_->buttonsLayout->addWidget(pimpl_->cancelButton);
    pimpl_->buttonsLayout->addWidget(pimpl_->refreshButton);

    pimpl_->mainLayout->addLayout(pimpl_->buttonsLayout);

    setLayout(pimpl_->mainLayout);

    // action connects
    connect(pimpl_->okButton, &QPushButton::released, this, &WRSettingsWidget::ok_);
    connect(pimpl_->cancelButton, &QPushButton::released, this, &WRSettingsWidget::cancel_);
    connect(pimpl_->refreshButton, &QPushButton::released, this, &WRSettingsWidget::refresh_);

    // init actions
    toggles_to_checkboxes_();
}

WRSettingsWidget::~WRSettingsWidget() {}

void WRSettingsWidget::ok_()
{
    checkboxes_states_to_toggles_();
    wr::dump_toggles_to_settings_file(pimpl_->wrsettings.settings, pimpl_->wrsettings.toggles);
    pimpl_->parentMainWindow->new_settings_saved();
    close();
}

void WRSettingsWidget::cancel_()
{
    toggles_to_checkboxes_();
    close();
}

void WRSettingsWidget::refresh_()
{
    wr::update_settings_from_dir(pimpl_->wrsettings);
    toggles_to_checkboxes_();
}

void WRSettingsWidget::closeEvent(QCloseEvent *event)
{
    toggles_to_checkboxes_();
    return QWidget::closeEvent(event);
}

void WRSettingsWidget::clean_dict_checkboxes_()
{
    if (pimpl_->dictSelectionWidget != nullptr) {
        (void) pimpl_->dictSelectionWidgetScrollArea->takeWidget();
        delete pimpl_->dictSelectionWidget;
        pimpl_->dictSelectionWidget = nullptr;
        pimpl_->toggleBoxes_.clear();
    }
}

void WRSettingsWidget::toggles_to_checkboxes_()
{
    clean_dict_checkboxes_();

    pimpl_->dictSelectionWidget = new QGroupBox(tr("Dictionaries:"));
    QVBoxLayout *layout = new QVBoxLayout;

    for (const WRDictToggleSetting &tg : pimpl_->wrsettings.toggles) {
        QCheckBox *checkbox = new QCheckBox(tg.shortFilename());
        checkbox->setChecked(tg.isActive);
        layout->addWidget(checkbox);
        pimpl_->toggleBoxes_.push_back(checkbox);
    }
    pimpl_->dictSelectionWidget->setLayout(layout);

    pimpl_->dictSelectionWidgetScrollArea->setWidget(pimpl_->dictSelectionWidget);
}

void WRSettingsWidget::checkboxes_states_to_toggles_()
{
    if (pimpl_->dictSelectionWidget == nullptr)
        return;

    for (size_t i = 0; i < pimpl_->toggleBoxes_.size(); ++i) {
        WRDictToggleSetting &toggle = pimpl_->wrsettings.toggles[i];
        const QCheckBox *toggleBox = pimpl_->toggleBoxes_[i];
        if (toggle.shortFilename() != toggleBox->text()) {
            std::stringstream ess;
            ess << "stored toggle settings short filename '"
                << toggle.shortFilename().toStdString() << "' is not equal to toggle box text '"
                << toggleBox->text().toStdString() << "'";
            throw std::runtime_error(ess.str());
        }

        toggle.isActive = toggleBox->isChecked();
    }
}
