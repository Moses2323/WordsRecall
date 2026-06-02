#include "wrgamewidget.h"

struct WRGameWidget::impl
{
    WRGameData &gameData;

    explicit impl(WRGameData &gameData)
        : gameData(gameData)
    {}

    impl(const impl &) = delete;
    impl &operator=(const impl &) = delete;
};

namespace {} // namespace

WRGameWidget::WRGameWidget(QWidget *parent, WRGameData &gameData)
    : QWidget(parent)
    , pimpl_(new WRGameWidget::impl(gameData))
{}

WRGameWidget::~WRGameWidget() {}
