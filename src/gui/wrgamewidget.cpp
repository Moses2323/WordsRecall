#include "wrgamewidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

struct WRGameWidget::impl
{
    WRGameData &gameData;

    QVBoxLayout *mainLayout{nullptr};

    QHBoxLayout *buttonsLayout{nullptr};
    QPushButton *startButton{nullptr};
    QPushButton *resetButton{nullptr};

    // e.g. "3/5 correct 20 words selected"
    QHBoxLayout *gameStatsLayout{nullptr};
    QLabel *n_correctLabel{nullptr};
    QLabel *correctLabel{nullptr};
    QLabel *n_selectedLabel{nullptr};
    QLabel *selectedLabel{nullptr};

    QVBoxLayout *dictWordLayout{nullptr};
    QLabel *meaningLabel{nullptr};
    QTextEdit *meaningText{nullptr};
    QLabel *wordLabel{nullptr};
    QLineEdit *wordLineEdit{nullptr};

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
{
    pimpl_->mainLayout = new QVBoxLayout(this);

    // buttons
    pimpl_->buttonsLayout = new QHBoxLayout;
    pimpl_->startButton = new QPushButton(tr("Start"));
    pimpl_->resetButton = new QPushButton(tr("Reset"));
    pimpl_->buttonsLayout->addWidget(pimpl_->startButton);
    pimpl_->buttonsLayout->addWidget(pimpl_->resetButton);
    pimpl_->buttonsLayout->addStretch();
    pimpl_->startButton->setDisabled(true);
    pimpl_->resetButton->setDisabled(true);
    pimpl_->mainLayout->addLayout(pimpl_->buttonsLayout);

    // correctness info
    pimpl_->gameStatsLayout = new QHBoxLayout;
    pimpl_->n_correctLabel = new QLabel("");
    pimpl_->correctLabel = new QLabel(tr("correct"));
    pimpl_->n_selectedLabel = new QLabel("");
    pimpl_->selectedLabel = new QLabel(tr("words selected"));
    pimpl_->gameStatsLayout->addWidget(pimpl_->n_correctLabel);
    pimpl_->gameStatsLayout->addWidget(pimpl_->correctLabel);
    pimpl_->gameStatsLayout->addWidget(pimpl_->n_selectedLabel);
    pimpl_->gameStatsLayout->addWidget(pimpl_->selectedLabel);
    pimpl_->gameStatsLayout->addStretch();
    pimpl_->mainLayout->addLayout(pimpl_->gameStatsLayout);

    // enter fields
    pimpl_->dictWordLayout = new QVBoxLayout;
    pimpl_->meaningLabel = new QLabel(tr("Meaning:"));
    pimpl_->meaningText = new QTextEdit("");
    pimpl_->wordLabel = new QLabel(tr("Word (enter your answer here):"));
    pimpl_->wordLineEdit = new QLineEdit;
    pimpl_->dictWordLayout->addWidget(pimpl_->meaningLabel);
    pimpl_->dictWordLayout->addWidget(pimpl_->meaningText);
    pimpl_->dictWordLayout->addWidget(pimpl_->wordLabel);
    pimpl_->dictWordLayout->addWidget(pimpl_->wordLineEdit);
    pimpl_->meaningText->setDisabled(true);
    pimpl_->wordLineEdit->setDisabled(true);
    pimpl_->mainLayout->addLayout(pimpl_->dictWordLayout);

    setLayout(pimpl_->mainLayout);

    // connects
    connect(pimpl_->startButton, &QPushButton::released, this, &WRGameWidget::start_);
    connect(pimpl_->resetButton, &QPushButton::released, this, &WRGameWidget::reset_);

    // initial enabling
}

WRGameWidget::~WRGameWidget() {}

void WRGameWidget::start_()
{
    qDebug() << "start button action";
}

void WRGameWidget::reset_()
{
    qDebug() << "reset button action";
}
