#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  Widget w;
  w.show();
  w.setWindowTitle("Learning Project");

  return a.exec();
}
