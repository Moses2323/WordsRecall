#pragma once

#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>

class MainWindow : public QWidget
{
    Q_OBJECT

    QLabel* simpleLabel_;
    QHBoxLayout* mL_;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
};
