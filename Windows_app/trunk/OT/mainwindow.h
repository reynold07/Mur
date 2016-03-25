#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QToolButton;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void setupUI();
    void setupActions();

    QToolButton* langue_CH;
    QToolButton* langue_EN;
};

#endif // MAINWINDOW_H
