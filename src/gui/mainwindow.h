#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <string>

class MainWindow : public QWidget
{
    Q_OBJECT

    QLabel* simpleLabel_;
    QHBoxLayout* mL_;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void change_label(const std::string &s);
};
