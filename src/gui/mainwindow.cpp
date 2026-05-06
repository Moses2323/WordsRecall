#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    mL_ = new QHBoxLayout;

    simpleLabel_ = new QLabel(tr("Speed Level 우유:"));
    QFont fnt;
    fnt.setPointSize(15);
    simpleLabel_->setFont(fnt);

    mL_->addWidget(simpleLabel_, 10);
    setLayout(mL_);
}

MainWindow::~MainWindow() = default;

void MainWindow::change_label(const std::string &s)
{
    QString q_str = QString::fromUtf8(s.c_str(), s.size());
    simpleLabel_->setText(q_str);
}
