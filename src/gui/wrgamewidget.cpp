#include "wrgamewidget.h"

struct WRGameWidget::impl
{};

namespace {} // namespace

WRGameWidget::WRGameWidget(QWidget *parent)
    : QWidget(parent)
    , pimpl_(new WRGameWidget::impl)
{}

WRGameWidget::~WRGameWidget() {}
