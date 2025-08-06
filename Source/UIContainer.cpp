#include "UIContainer.h"
#include <QColorDialog>
#include <QPainter>
#include <QRandomGenerator>
#include <QResizeEvent>

UIContainer::UIContainer(QWidget *parent)
    : QWidget(parent), m_drawArea(50, 100, 700, 500) {
  setupUI();
  setMinimumSize(800, 650);
}

void UIContainer::setupUI() {
  setWindowTitle("可拖拽圆形演示");

  // 创建控制面板
  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  QHBoxLayout *buttonLayout = new QHBoxLayout();

  m_addButton = new QPushButton("添加圆形");
  m_removeButton = new QPushButton("删除圆形");
  m_colorButton = new QPushButton("改变颜色");

  m_radiusSpinBox = new QSpinBox();
  m_radiusSpinBox->setRange(10, 200);
  m_radiusSpinBox->setValue(50);
  m_radiusSpinBox->setSuffix(" px");

  m_borderWidthSpinBox = new QSpinBox();
  m_borderWidthSpinBox->setRange(1, 20);
  m_borderWidthSpinBox->setValue(2);
  m_borderWidthSpinBox->setSuffix(" px");

  m_statusLabel = new QLabel("状态: 准备就绪");

  buttonLayout->addWidget(new QLabel("半径:"));
  buttonLayout->addWidget(m_radiusSpinBox);
  buttonLayout->addWidget(new QLabel("边框宽度:"));
  buttonLayout->addWidget(m_borderWidthSpinBox);
  buttonLayout->addWidget(m_addButton);
  buttonLayout->addWidget(m_removeButton);
  buttonLayout->addWidget(m_colorButton);
  buttonLayout->addStretch();

  mainLayout->addLayout(buttonLayout);
  mainLayout->addWidget(m_statusLabel);
  mainLayout->addStretch();

  // 连接信号槽
  connect(m_addButton, &QPushButton::clicked, this, &UIContainer::addCircle);
  connect(m_removeButton, &QPushButton::clicked, this,
          &UIContainer::removeLastIcon);
  connect(m_colorButton, &QPushButton::clicked, this,
          &UIContainer::changeIconColor);

  // 添加初始圆形
  addCircle();
}

void UIContainer::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);

  QPainter painter(this);

  // 绘制约束区域边界
  painter.setPen(QPen(Qt::black, 2, Qt::DashLine));
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(m_drawArea);

  // 绘制说明文字
  painter.setPen(Qt::black);
  painter.drawText(m_drawArea.topLeft() + QPoint(10, -10), "拖拽区域");
  painter.drawText(10, height() - 60, "操作说明:");
  painter.drawText(10, height() - 40, "• 左键拖拽移动圆形");
  painter.drawText(10, height() - 20, "• 拖拽边缘调整大小");
  painter.drawText(200, height() - 20, "• 鼠标滚轮调整大小");
}

void UIContainer::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);

  // 更新绘制区域
  m_drawArea = QRect(50, 100, width() - 100, height() - 150);
  updateConstraints();
}

void UIContainer::addCircle() {
  CircleIcon *circle = new CircleIcon(this);
  circle->setRadius(m_radiusSpinBox->value());
  circle->setConstraintRect(m_drawArea);
  // 设置随机颜色
  QColor randomColor =
      QColor::fromHsv(QRandomGenerator::global()->bounded(360), 180, 220, 180);
  circle->setCircleColor(randomColor);

  // 设置随机位置
  int x = m_drawArea.left() + QRandomGenerator::global()->bounded(
                                  m_drawArea.width() - circle->width());
  int y = m_drawArea.top() + QRandomGenerator::global()->bounded(
                                 m_drawArea.height() - circle->height());
  circle->move(x, y);
  circle->move(x, y);

  // 连接信号
  connect(circle, &CircleIcon::positionChanged, this,
          &UIContainer::onIconPositionChanged);
  connect(circle, &CircleIcon::sizeChanged, this,
          &UIContainer::onIconSizeChanged);
  connect(m_radiusSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), circle,
          [this, circle]() { circle->setRadius(m_radiusSpinBox->value()); });
  connect(circle, &CircleIcon::sizeChanged, m_radiusSpinBox,
          &QSpinBox::setValue);
  connect(m_borderWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
          circle, [this, circle]() {
            circle->setBorderWidth(m_borderWidthSpinBox->value());
          });
  m_circles.append(circle);
  circle->show();

  m_statusLabel->setText(
      QString("状态: 已添加圆形，当前共 %1 个").arg(m_circles.size()));
}

void UIContainer::addLine() {}
void UIContainer::addRectangle() {}
void UIContainer::addEllipse() {}
void UIContainer::addArc() {}

void UIContainer::addFloat() {}
void UIContainer::addInteger() {}

void UIContainer::addText() {}

void UIContainer::removeLastIcon() {
  if (!m_circles.isEmpty()) {
    CircleIcon *circle = m_circles.takeLast();
    circle->deleteLater();
    m_statusLabel->setText(
        QString("状态: 已删除圆形，当前共 %1 个").arg(m_circles.size()));
  }
}

void UIContainer::changeIconColor() {
  if (m_circles.isEmpty())
    return;

  QColor color = QColorDialog::getColor(Qt::blue, this, "选择圆形颜色");
  if (color.isValid()) {
    color.setAlpha(180); // 设置透明度
    for (CircleIcon *circle : m_circles) {
      circle->setCircleColor(color);
    }
  }
}

void UIContainer::onIconPositionChanged(const QPoint &pos) {
  CircleIcon *circle = qobject_cast<CircleIcon *>(sender());
  if (circle) {
    m_statusLabel->setText(
        QString("状态: 圆形移动到 (%1, %2)").arg(pos.x()).arg(pos.y()));
  }
}

void UIContainer::onIconSizeChanged(int radius) {
  CircleIcon *circle = qobject_cast<CircleIcon *>(sender());
  if (circle) {
    circle->setRadius(radius);
    m_statusLabel->setText(QString("状态: 圆形大小调整为 %1 px").arg(radius));
  }
}

void UIContainer::updateConstraints() {
  for (CircleIcon *circle : m_circles) {
    circle->setConstraintRect(m_drawArea);
  }
}