#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    mL_ = new QHBoxLayout;

    simpleLabel_ = new QLabel(tr("Speed Level 우유:"));
    QFont fnt;
    fnt.setPointSize(150);
    simpleLabel_->setFont(fnt);

    mL_->addWidget(simpleLabel_, 10);
    setLayout(mL_);
}

MainWindow::~MainWindow() = default;
