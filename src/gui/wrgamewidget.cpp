#include "wrgamewidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <src/core/wrgamelogic.h>

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

namespace {

QString ncorrect_nmax_to_str(const SessionData &session)
{
    QString ncorrect_str = QString::number(session.correctlyDone.size());
    QString nmaxcorrect_str = QString::number(session.roundIdx);
    return ncorrect_str + "/" + nmaxcorrect_str;
}

QString ncorrect_ntotal_to_str(const WRGameData &gdata)
{
    QString ncorrect_str = QString::number(gdata.sessionData.correctlyDone.size());
    QString ntotal_str = QString::number(gdata.mergedDicts.size());
    return ncorrect_str + "/" + ntotal_str;
}

} // namespace

// -----------------------------------------------------------------------------------------------

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
    pimpl_->correctLabel = new QLabel(tr("correct |"));
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
    pimpl_->meaningText->setReadOnly(true);
    pimpl_->wordLineEdit->setDisabled(true);
    pimpl_->mainLayout->addLayout(pimpl_->dictWordLayout);

    setLayout(pimpl_->mainLayout);

    // connects
    connect(pimpl_->startButton, &QPushButton::released, this, &WRGameWidget::start_);
    connect(pimpl_->resetButton, &QPushButton::released, this, &WRGameWidget::reset_);
    connect(pimpl_->wordLineEdit, &QLineEdit::returnPressed, this, &WRGameWidget::enterWord_);

    // initial enabling
}

WRGameWidget::~WRGameWidget() {}

void WRGameWidget::fill_from_settings(const WRSettings &settings)
{
    wr::fill_game_data_from_settings(settings, pimpl_->gameData);
    setBeforeGameState_();
}

void WRGameWidget::start_()
{
    pimpl_->startButton->setDisabled(true);
    pimpl_->meaningText->setText("");
    pimpl_->wordLineEdit->setText("");

    wr::start_game(pimpl_->gameData);
    updateFromGameSession_();

    pimpl_->wordLineEdit->setEnabled(true);
    pimpl_->resetButton->setEnabled(true);
}

void WRGameWidget::reset_()
{
    setBeforeGameState_();
}

void WRGameWidget::enterWord_()
{
    bool is_correct = wr::answer_action(pimpl_->wordLineEdit->text(), pimpl_->gameData);
    updateFromGameSession_();
    pimpl_->wordLineEdit->setText("");

    if (pimpl_->gameData.sessionData.isFinished) {
        finishGameSession_();
    }
}

void WRGameWidget::setBeforeGameState_()
{
    pimpl_->startButton->setDisabled(true);
    pimpl_->resetButton->setDisabled(true);
    pimpl_->wordLineEdit->setDisabled(true);

    pimpl_->wordLineEdit->setText("");
    pimpl_->meaningText->setText(pimpl_->gameData.initial_message);

    pimpl_->n_correctLabel->setText("");
    pimpl_->n_selectedLabel->setText(QString::number(pimpl_->gameData.mergedDicts.size()));

    pimpl_->startButton->setEnabled(true);
}

void WRGameWidget::updateFromGameSession_()
{
    pimpl_->meaningText->setText(pimpl_->gameData.get_current_word().meaning);
    pimpl_->n_correctLabel->setText(ncorrect_nmax_to_str(pimpl_->gameData.sessionData));

    if (pimpl_->gameData.sessionData.isNextRepeat)
        setMeaningTextWithAnswer_();
}

void WRGameWidget::setMeaningTextWithAnswer_()
{
    QString text = pimpl_->meaningText->toPlainText() + "\n\nWrong. Enter the correct answer:\n"
                   + pimpl_->gameData.get_current_word().word;
    pimpl_->meaningText->setText(text);
}

void WRGameWidget::finishGameSession_()
{
    QString finish_text = "CONGRADS! Your score " + ncorrect_ntotal_to_str(pimpl_->gameData);
    finish_text += "\n\n" + pimpl_->gameData.initial_message;
    pimpl_->gameData.initial_message = finish_text;
    setBeforeGameState_();
}
