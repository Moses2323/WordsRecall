#include "wrsettingswidget.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <src/core/wrsettings.h>
#include <sstream>
#include <vector>

struct WRSettingsWidget::impl
{
    // Settings guts.
    WRSettings &wrsettings;

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

    explicit impl(WRSettings &wrsettings)
        : wrsettings(wrsettings)
    {}
};

WRSettingsWidget::WRSettingsWidget(QWidget *parent, WRSettings &wrsettings)
    : QWidget(parent)
    , pimpl_(new WRSettingsWidget::impl(wrsettings))
{
    setMinimumSize(300, 100);

    // layouts
    pimpl_->mainLayout = new QVBoxLayout;
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
        pimpl_->mainLayout->removeWidget(pimpl_->dictSelectionWidget);
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
        QCheckBox *checkbox = new QCheckBox(tg.short_filename());
        checkbox->setChecked(tg.is_active);
        layout->addWidget(checkbox);
        pimpl_->toggleBoxes_.push_back(checkbox);
    }
    pimpl_->dictSelectionWidget->setLayout(layout);

    pimpl_->mainLayout->insertWidget(0, pimpl_->dictSelectionWidget);
}

void WRSettingsWidget::checkboxes_states_to_toggles_()
{
    if (pimpl_->dictSelectionWidget == nullptr)
        return;

    for (size_t i = 0; i < pimpl_->toggleBoxes_.size(); ++i) {
        WRDictToggleSetting &toggle = pimpl_->wrsettings.toggles[i];
        const QCheckBox *toggleBox = pimpl_->toggleBoxes_[i];
        if (toggle.short_filename() != toggleBox->text()) {
            std::stringstream ess;
            ess << "stored toggle settings short filename '"
                << toggle.short_filename().toStdString() << "' is not equal to toggle box text '"
                << toggleBox->text().toStdString() << "'";
            throw std::runtime_error(ess.str());
        }

        toggle.is_active = toggleBox->isChecked();
    }
}
