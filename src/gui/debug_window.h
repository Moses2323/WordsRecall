#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <string>

class WRDebugWindow : public QWidget
{
    Q_OBJECT

    QLabel* simpleLabel_;
    QHBoxLayout* mL_;
public:
    explicit WRDebugWindow(QWidget *parent = nullptr);
    ~WRDebugWindow() override;

    void change_label(const std::string &s);
};
