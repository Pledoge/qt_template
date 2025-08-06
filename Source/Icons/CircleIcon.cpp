#include "Icons/CircleIcon.h"
#include <QApplication>
#include <QCursor>
#include <QPainter>
#include <QWheelEvent>
#include <qmath.h>

CircleIcon::CircleIcon(QWidget *parent)
    : BaseIcon(parent), m_radius(50), m_circleColor(QColor(100, 150, 255, 180)),
      m_borderColor(Qt::blue), m_borderWidth(2), m_dragMode(None),
      m_constraintRect(0, 0, 800, 600) {
  setBaseSize(m_radius * 2, m_radius * 2);
  setAttribute(Qt::WA_TranslucentBackground);
  setMouseTracking(true); // 启用鼠标追踪
}

void CircleIcon::setRadius(int radius) {
  radius = qBound(MIN_RADIUS, radius, MAX_RADIUS);
  if (m_radius != radius) {
    m_radius = radius;
    updateGeometry();
    emit sizeChanged(m_radius);
  }
}

void CircleIcon::setCircleColor(const QColor &color) {
  m_circleColor = color;
  update();
}

void CircleIcon::setBorderColor(const QColor &color) {
  m_borderColor = color;
  update();
}

void CircleIcon::setBorderWidth(int width) {
  if (width < 1)
    width = 1;
  if (m_borderWidth != width) {
    // 计算变粗后需要的额外空间
    int extra = width - m_borderWidth;
    if (extra > 0) {
      // 向外扩展widget的大小，使border向外变粗
      int grow = extra * 2;
      resize(width * 2 + m_radius * 2, width * 2 + m_radius * 2);
      move(pos() - QPoint(extra, extra)); // 保持圆心不变
    }
    m_borderWidth = width;
    update();
  }
}

void CircleIcon::setColor(const IconColor &color) {
  m_color = color;
  switch (color) {
  case IconColor::TEAM_COLOR:
    setBorderColor(QColorConstants::Red);
    break;
  case IconColor::YELLOW:
    setBorderColor(QColorConstants::Yellow);
    break;
  case IconColor::GREEN:
    setBorderColor(QColorConstants::Green);
    break;
  case IconColor::ORANGE:
    setBorderColor(QColorConstants::DarkYellow);
    break;
  case IconColor::FUCHSIA:
    setBorderColor(QColorConstants::Magenta);
    break;
  case IconColor::PINK:
    setBorderColor(QColorConstants::Svg::pink);
    break;
  case IconColor::CYAN:
    setBorderColor(QColorConstants::Cyan);
    break;
  case IconColor::BLACK:
    setBorderColor(QColorConstants::Black);
    break;
  case IconColor::WHITE:
    setBorderColor(QColorConstants::White);
    break;
  default:
    setBorderColor(QColorConstants::Black);
    break;
  }
}

BaseIcon::IconColor CircleIcon::getColor() const { return m_color; }

void CircleIcon::setPosition(const QPoint &pos) {
  if (pos != m_position) {
    m_position = pos;
    move(pos);
    emit positionChanged(pos);
  }
}

QPoint CircleIcon::getPosition() const { return m_position; }

void CircleIcon::setConstraintRect(const QRect &rect) {
  m_constraintRect = rect;
  constrainPosition();
}

void CircleIcon::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QRect circleRect = rect().adjusted(m_borderWidth, m_borderWidth,
                                     -m_borderWidth, -m_borderWidth);

  // 绘制圆形填充
  painter.setBrush(Qt::NoBrush);
  painter.setPen(QPen(m_borderColor, m_borderWidth));
  painter.drawEllipse(circleRect);

  // 如果正在调整大小，绘制调整大小的指示器
  if (m_dragMode == Resizing) {
    painter.setPen(QPen(Qt::red, 1, Qt::DashLine));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(circleRect);

    // 绘制调整大小的小方块
    int handleSize = 6;
    QPoint center = rect().center();
    QPoint handlePos(center.x() + m_radius, center.y());
    QRect handleRect(handlePos.x() - handleSize / 2,
                     handlePos.y() - handleSize / 2, handleSize, handleSize);
    painter.setBrush(Qt::red);
    painter.setPen(Qt::red);
    painter.drawRect(handleRect);
  }
}

void CircleIcon::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    m_dragStartPos = event->pos();
    m_widgetStartPos = pos();
    m_startRadius = m_radius;

    if (isOnBorder(event->pos())) {
      m_dragMode = Resizing;
      setCursor(Qt::SizeFDiagCursor);
    } else {
      m_dragMode = Moving;
      setCursor(Qt::ClosedHandCursor);
    }

    event->accept();
    setMouseTracking(false);
  }
}

void CircleIcon::mouseMoveEvent(QMouseEvent *event) {
  if (!(event->buttons() & Qt::LeftButton)) {
    // 非拖拽状态下的鼠标悬停处理
    if (m_dragMode == None) {
      if (isOnBorder(event->pos())) {
        setCursor(Qt::SizeFDiagCursor);
      } else {
        setCursor(Qt::OpenHandCursor);
      }
    }
    return;
  }

  // 添加移动阈值，避免微小移动导致的抖动
  QPoint delta = event->pos() - m_dragStartPos;
  if (delta.manhattanLength() < 3) {
    return; // 忽略小于3像素的移动
  }

  if (m_dragMode == Moving) {
    // 计算新位置
    QPoint newPos = m_widgetStartPos + delta;

    // 在移动前先检查约束
    int diameter = m_radius * 2;
    int minX = m_constraintRect.left();
    int maxX = m_constraintRect.right() - diameter;
    int minY = m_constraintRect.top();
    int maxY = m_constraintRect.bottom() - diameter;
    QPoint constrainedPos(qBound(minX, newPos.x(), maxX),
                          qBound(minY, newPos.y(), maxY));

    // 只有位置真正改变时才移动
    if (constrainedPos != pos()) {
      move(constrainedPos);
      emit positionChanged(pos());
    }

  } else if (m_dragMode == Resizing) {
    // 调整大小时也添加阈值
    QPoint center = rect().center();
    QPoint currentPos = event->pos();
    int newRadius = qSqrt(qPow(currentPos.x() - center.x(), 2) +
                          qPow(currentPos.y() - center.y(), 2));

    // 只有半径变化超过阈值时才更新
    if (qAbs(newRadius - m_radius) > 2) {
      setRadius(newRadius);
    }
  }

  event->accept();
}

void CircleIcon::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    m_dragMode = None;
    setCursor(Qt::ArrowCursor);
    setMouseTracking(true); // 恢复鼠标追踪
    constrainPosition();
    event->accept();
  }
}

void CircleIcon::wheelEvent(QWheelEvent *event) {
  // 使用鼠标滚轮调整大小
  int delta = event->angleDelta().y() > 0 ? 5 : -5;
  setRadius(m_radius + delta);
  event->accept();
}

void CircleIcon::updateGeometry() {
  int diameter = m_radius * 2;
  resize(diameter, diameter);
  constrainPosition();
  update();
}

bool CircleIcon::isOnBorder(const QPoint &pos) const {
  QPoint center = rect().center();
  int distance =
      qSqrt(qPow(pos.x() - center.x(), 2) + qPow(pos.y() - center.y(), 2));
  return qAbs(distance - m_radius) <= BORDER_TOLERANCE;
}

void CircleIcon::constrainPosition() {
  if (m_constraintRect.isNull())
    return;

  QPoint newPos = pos();
  int diameter = m_radius * 2;

  // 确保圆形完全在约束矩形内
  newPos.setX(qBound(m_constraintRect.left(), newPos.x(),
                     m_constraintRect.right() - diameter));
  newPos.setY(qBound(m_constraintRect.top(), newPos.y(),
                     m_constraintRect.bottom() - diameter));

  if (newPos != pos()) {
    move(newPos);
  }
}