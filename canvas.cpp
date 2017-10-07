#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent) {
  b = Board<int>::MakeBoard(gridSizeX, gridSizeY);
  solver = new SimpleSolver<int>;
}

Canvas::~Canvas() { delete solver; }

void Canvas::setGridSizeX(int sz) {
  gridSizeX = sz;
  b = Board<int>::MakeBoard(gridSizeX, gridSizeY);
  update();
}

void Canvas::setGridSizeY(int sz) {
  gridSizeY = sz;
  b = Board<int>::MakeBoard(gridSizeX, gridSizeY);
  update();
}

void Canvas::solvePuzzle() {
  solver->solve(b);
  update();
}

void Canvas::setSolvingMethod(int m) {
  delete solver;
  if (m == 0)
    solver = new SimpleSolver<int>();
}

void Canvas::paintEvent(QPaintEvent *e) {
  QPainter p(this);
  p.setBrush(Qt::black);
  p.setPen(QPen(Qt::black, 2));
  for (int i = 0; i <= gridSizeX; ++i) {
    int x = qMin(1 + i * (width() / gridSizeX), width() - 1);
    p.drawLine(x, 1, x, height() - 1);
  }
  for (int i = 0; i <= gridSizeY; ++i) {
    int y = qMin(1 + i * (height() / gridSizeY), height() - 1);
    p.drawLine(1, y, width() - 1, y);
  }

  QFont font = p.font();
  font.setPointSize(
      qMin(width() / (gridSizeX + 2), height() / (gridSizeY + 2)) * 0.8);
  p.setFont(font);

  for (int j = 0; j < gridSizeY; ++j)
    for (int i = 0; i < gridSizeX; ++i) {
      if (!b.getElem(i, j))
        continue;
      float posX = (width() * 1.0) / gridSizeX;
      float posY = (height() * 1.0) / gridSizeY;
      QRectF pos(i * posX, j * posY, width() / gridSizeX, height() / gridSizeY);
      p.drawText(pos, QString::number(b.getElem(i, j)),
                 QTextOption(Qt::AlignCenter));
    }
}
