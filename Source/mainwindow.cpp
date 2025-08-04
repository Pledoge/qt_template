#include "mainwindow.h"
#include "SettingsDialog.h"
#include "ui_mainwindow.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  connect(ui->test_button, &QPushButton::clicked, this, []() {
    SettingsDialog settingsDialog;
    settingsDialog.show();
    settingsDialog.exec();
  });
  QPixmap logo(":/Resources/images/head.jpg");
  setWindowIcon(QIcon(logo));
}

Widget::~Widget() { delete ui; }
