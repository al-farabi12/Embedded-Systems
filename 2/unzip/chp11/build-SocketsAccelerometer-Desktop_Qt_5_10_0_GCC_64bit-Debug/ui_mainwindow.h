/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDial>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *mainPanel;
    QSlider *rollSlider;
    QSlider *pitchSlider;
    QDial *pitchDial;
    QDial *rollDial;
    QDoubleSpinBox *pitchSpinBox;
    QDoubleSpinBox *rollSpinBox;
    QLabel *topPitchLabel;
    QLabel *topRollLabel;
    QLabel *botPitchLabel;
    QLabel *sideRollLabel;
    QPushButton *exitButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 300);
        mainPanel = new QWidget(MainWindow);
        mainPanel->setObjectName(QStringLiteral("mainPanel"));
        rollSlider = new QSlider(mainPanel);
        rollSlider->setObjectName(QStringLiteral("rollSlider"));
        rollSlider->setGeometry(QRect(10, 200, 351, 20));
        rollSlider->setMinimum(-90);
        rollSlider->setMaximum(90);
        rollSlider->setValue(0);
        rollSlider->setSliderPosition(0);
        rollSlider->setOrientation(Qt::Horizontal);
        rollSlider->setTickPosition(QSlider::TicksBothSides);
        pitchSlider = new QSlider(mainPanel);
        pitchSlider->setObjectName(QStringLiteral("pitchSlider"));
        pitchSlider->setGeometry(QRect(370, 10, 19, 201));
        pitchSlider->setMinimum(-90);
        pitchSlider->setMaximum(90);
        pitchSlider->setValue(0);
        pitchSlider->setSliderPosition(0);
        pitchSlider->setOrientation(Qt::Vertical);
        pitchSlider->setTickPosition(QSlider::TicksBothSides);
        pitchDial = new QDial(mainPanel);
        pitchDial->setObjectName(QStringLiteral("pitchDial"));
        pitchDial->setGeometry(QRect(20, 30, 141, 191));
        pitchDial->setMinimum(-90);
        pitchDial->setMaximum(90);
        pitchDial->setValue(0);
        rollDial = new QDial(mainPanel);
        rollDial->setObjectName(QStringLiteral("rollDial"));
        rollDial->setGeometry(QRect(200, 30, 141, 191));
        rollDial->setMinimum(-90);
        rollDial->setMaximum(90);
        rollDial->setValue(0);
        rollDial->setSliderPosition(0);
        pitchSpinBox = new QDoubleSpinBox(mainPanel);
        pitchSpinBox->setObjectName(QStringLiteral("pitchSpinBox"));
        pitchSpinBox->setGeometry(QRect(30, 10, 121, 22));
        pitchSpinBox->setDecimals(4);
        pitchSpinBox->setMinimum(-90);
        pitchSpinBox->setMaximum(90);
        rollSpinBox = new QDoubleSpinBox(mainPanel);
        rollSpinBox->setObjectName(QStringLiteral("rollSpinBox"));
        rollSpinBox->setGeometry(QRect(210, 10, 121, 22));
        rollSpinBox->setReadOnly(true);
        rollSpinBox->setDecimals(4);
        rollSpinBox->setMinimum(-90);
        rollSpinBox->setMaximum(90);
        topPitchLabel = new QLabel(mainPanel);
        topPitchLabel->setObjectName(QStringLiteral("topPitchLabel"));
        topPitchLabel->setGeometry(QRect(70, 40, 46, 13));
        topRollLabel = new QLabel(mainPanel);
        topRollLabel->setObjectName(QStringLiteral("topRollLabel"));
        topRollLabel->setGeometry(QRect(260, 40, 46, 13));
        botPitchLabel = new QLabel(mainPanel);
        botPitchLabel->setObjectName(QStringLiteral("botPitchLabel"));
        botPitchLabel->setGeometry(QRect(340, 10, 46, 13));
        sideRollLabel = new QLabel(mainPanel);
        sideRollLabel->setObjectName(QStringLiteral("sideRollLabel"));
        sideRollLabel->setGeometry(QRect(10, 220, 41, 20));
        exitButton = new QPushButton(mainPanel);
        exitButton->setObjectName(QStringLiteral("exitButton"));
        exitButton->setGeometry(QRect(300, 220, 75, 23));
        MainWindow->setCentralWidget(mainPanel);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "EBB Accelerometer Server Application", nullptr));
        topPitchLabel->setText(QApplication::translate("MainWindow", "Pitch", nullptr));
        topRollLabel->setText(QApplication::translate("MainWindow", "Roll", nullptr));
        botPitchLabel->setText(QApplication::translate("MainWindow", "Pitch", nullptr));
        sideRollLabel->setText(QApplication::translate("MainWindow", "Roll", nullptr));
        exitButton->setText(QApplication::translate("MainWindow", "Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
