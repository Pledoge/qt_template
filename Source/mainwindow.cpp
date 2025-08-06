#include "mainwindow.h"
#include "Icons/CircleIcon.h"
#include "SettingsDialog.h"
#include "UIContainer.h"
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

  UIContainer *circleContainer = new UIContainer(this);
  circleContainer->show();
}

Widget::~Widget() { delete ui; }
