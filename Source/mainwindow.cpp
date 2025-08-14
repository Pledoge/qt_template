#include "mainwindow.h"
#include "SettingsDialog.h"
#include "ui_mainwindow.h"

void openSettingsDialog() {
  SettingsDialog settingsDialog;
  settingsDialog.exec();
}

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  connect(ui->test_button, &QPushButton::clicked, this, &openSettingsDialog);
  connect(ui->textEdit, &QTextEdit::textChanged, ui->test_button,
          &QPushButton::click);
  QPixmap logo(":/Resources/images/head.jpg");
  setWindowIcon(QIcon(logo));
}

Widget::~Widget() { delete ui; }
