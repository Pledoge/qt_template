#ifndef CIRCLECONTAINER_H
#define CIRCLECONTAINER_H

#include "Icons/CircleIcon.h"
#include <QColorDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>



class UIContainer : public QWidget {
  Q_OBJECT

public:
  explicit UIContainer(QWidget *parent = nullptr);

  enum class IconType {
    LINE = 0,
    RECTANGLE,
    CIRCLE,
    ELLIPSE,
    ARC,
    FLOAT,
    INTEGER,
    TEXT
  };

protected:
  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
private slots:
  void addLine();
  void addRectangle();
  void addCircle();
  void addEllipse();
  void addArc();
  void addFloat();
  void addInteger();
  void addText();

  // void chooseCurrentIcon();

  void removeLastIcon();
  void changeIconColor();
  void onIconPositionChanged(const QPoint &pos);
  void onIconSizeChanged(int radius);

private:
  void setupUI();
  void updateConstraints();

  QList<CircleIcon *> m_circles;
  QRect m_drawArea;

  // UI控件
  QPushButton *m_addButton;
  QPushButton *m_removeButton;
  QPushButton *m_colorButton;
  QSpinBox *m_radiusSpinBox;
  QSpinBox *m_borderWidthSpinBox;
  QLabel *m_statusLabel;
};

#endif // CIRCLECONTAINER_H