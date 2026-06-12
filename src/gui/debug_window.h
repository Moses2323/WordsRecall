/**
 * \file    debug_window.h
 * \brief   Simple window used only for debugging. Not used in the main application.
 */
#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <string>

//! Simple Widget for dictionary debugging with a single label.
class WRDebugWindow : public QWidget
{
    Q_OBJECT

    QLabel* simpleLabel_;
    //! \brief Main layout for this widget.
    QHBoxLayout* mL_;
public:
    explicit WRDebugWindow(QWidget *parent = nullptr);
    ~WRDebugWindow() override;

    //! \brief Put the label (assumed UTF-8 encoding).
    void change_label(const std::string &s);
};
