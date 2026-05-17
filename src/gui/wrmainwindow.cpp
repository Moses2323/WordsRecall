#include "wrmainwindow.h"

#include <QMenuBar>
#include <QSettings>
#include <iostream>

struct WRMainWindow::impl
{
    QMenu *fileMenu{nullptr};
    QSettings settings{"words_recall.ini", QSettings::IniFormat};
};

WRMainWindow::WRMainWindow()
    : pimpl_(new WRMainWindow::impl)
{
    pimpl_->fileMenu = menuBar()->addMenu(tr("&File"));
    qDebug() << pimpl_->settings.fileName();
}

WRMainWindow::~WRMainWindow() {}
