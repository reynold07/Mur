#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupActions();
    setupUI();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupUI()
{
    //line1

    //line 2 tools short cut
    QLabel* langueSetLabel = new QLabel("Langue");
    langueSetLabel->setFixedWidth(180);
    langueSetLabel->setWordWrap(true);
    QHBoxLayout* buttonsLangueLayout = new QHBoxLayout;
    buttonsLangueLayout->addWidget(langue_CH);
    buttonsLangueLayout->addWidget(langue_EN);
    QHBoxLayout* toolsLayout = new QHBoxLayout;
    toolsLayout->addWidget(langueSetLabel);
    toolsLayout->addLayout(buttonsLangueLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(toolsLayout);

    QWidget* widget = new QWidget;
    widget->setLayout(mainLayout);
    setCentralWidget(widget);


    setWindowTitle(tr("Saturn"));
    setWindowIcon(QIcon(":/pic/main.ico"));
}

void MainWindow::setupActions()
{
    langue_CH = new QToolButton;
    //langue_CH->setText(tr("CH"));
    langue_CH->setIcon(QIcon(":/pic/CH.ico"));
    langue_CH->setIconSize(QSize(24,24));
    langue_CH->setAutoRaise(true);

    langue_EN = new QToolButton;
    //langue_EN->setText(tr("EN"));
    langue_EN->setIcon(QIcon(":/pic/EN.ico"));
    langue_EN->setIconSize(QSize(24,24));
    langue_EN->setAutoRaise(true);
}

