#ifndef CIRCLEICON_H
#define CIRCLEICON_H

#include "Base.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QWidget>

class CircleIcon : public BaseIcon {
  Q_OBJECT

public:
  explicit CircleIcon(QWidget *parent = nullptr);

  void setRadius(int radius);
  int getRadius() const { return m_radius; }

  void setCircleColor(const QColor &color);
  void setBorderColor(const QColor &color);
  void setColor(const IconColor &color) override;
  IconColor getColor() const override;
  void setPosition(const QPoint &pos) override;
  QPoint getPosition() const override;

  void setBorderWidth(int width);

  void setConstraintRect(const QRect &rect);

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override; // 鼠标滚轮调整大小

signals:
  void positionChanged(const QPoint &newPos);
  void sizeChanged(int newRadius);

private:
  enum DragMode { None, Moving, Resizing };

  void updateGeometry();
  bool isOnBorder(const QPoint &pos) const;
  void constrainPosition();

  int m_radius;
  QColor m_circleColor;
  QColor m_borderColor;
  int m_borderWidth;

  DragMode m_dragMode;
  QPoint m_dragStartPos;
  QPoint m_widgetStartPos;
  int m_startRadius;

  QRect m_constraintRect; // 约束范围

  static constexpr int MIN_RADIUS = 10;
  static constexpr int MAX_RADIUS = 200;
  static constexpr int BORDER_TOLERANCE = 5; // 边界检测容差
};

#endif // DRAGGABLECIRCLE_H