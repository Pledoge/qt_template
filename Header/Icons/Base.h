#ifndef BASE_H
#define BASE_H

#include <QMetaObject>
#include <QWidget>

class BaseIcon : public QWidget {
  Q_OBJECT

public:
  explicit BaseIcon(QWidget *parent = nullptr) : QWidget(parent) {}

  enum class IconColor {
    TEAM_COLOR = 0,
    YELLOW,
    GREEN,
    ORANGE,
    FUCHSIA,
    PINK,
    CYAN,
    BLACK,
    WHITE
  };

  virtual void setColor(const IconColor &color) = 0;
  virtual IconColor getColor() const = 0;
  virtual void setPosition(const QPoint &pos) = 0;
  virtual QPoint getPosition() const = 0;

protected:
  IconColor m_color = IconColor::TEAM_COLOR;
  QPoint m_position;
};
#endif // BASE_H