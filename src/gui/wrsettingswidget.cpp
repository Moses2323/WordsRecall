#include "wrsettingswidget.h"

struct WRSettingsWidget::impl
{};

WRSettingsWidget::WRSettingsWidget(QWidget *parent)
    : QWidget(parent)
    , pimpl_(new WRSettingsWidget::impl)
{}

WRSettingsWidget::~WRSettingsWidget() {}
