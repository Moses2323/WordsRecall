#include "debug_window.h"

WRDebugWindow::WRDebugWindow(QWidget *parent)
    : QWidget(parent)
{
    mL_ = new QHBoxLayout;

    simpleLabel_ = new QLabel("Speed Level 우유:");
    QFont fnt;
    fnt.setPointSize(15);
    simpleLabel_->setFont(fnt);

    mL_->addWidget(simpleLabel_, 10);
    setLayout(mL_);
}

WRDebugWindow::~WRDebugWindow() = default;

void WRDebugWindow::change_label(const std::string &s)
{
    QString q_str = QString::fromUtf8(s.c_str(), s.size());
    simpleLabel_->setText(q_str);
}
