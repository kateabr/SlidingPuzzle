#include "canvas.h"
#include <QtConcurrent/QtConcurrent>

Canvas::Canvas(QWidget *parent) : QWidget(parent) {
  b = Board::MakeBoard(gridSizeX, gridSizeY);
  solver = new BFSSolver;

  connectSolverWithUI();
  connect(&solveFutureWatcher, &QFutureWatcher<void>::finished,
          [&] { emit showControls(true); });
}

Canvas::~Canvas() { delete solver; }

void Canvas::setGridSizeX(int sz) {
  gridSizeX = sz;
  b = Board::MakeBoard(gridSizeX, gridSizeY);
  update();
}

void Canvas::setGridSizeY(int sz) {
  gridSizeY = sz;
  b = Board::MakeBoard(gridSizeX, gridSizeY);
  update();
}

void Canvas::solvePuzzle() {
  solveFuture = QtConcurrent::run([&]() {
    emit showControls(false);
    solver->solve(b);
  });
  solveFutureWatcher.setFuture(solveFuture);
}

void Canvas::setSolvingMethod(int m) {
  delete solver; // auto disconnect
  if (m == 0)
    solver = new BFSSolver();
  else if (m == 1)
    solver = new AStarSolver();

  connectSolverWithUI();
}

void Canvas::connectSolverWithUI() {
  connect(solver, &Solver::updateLayout, [&]() { update(); });
  connect(solver, &Solver::updateOps, [&](int ops) { emit updateOps(ops); });
  connect(solver, &Solver::updateTime,
          [&](int time) { emit updateTime(time); });
}

void Canvas::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.setBrush(Qt::black);
  p.setPen(QPen(Qt::black, 2));
  for (int i = 0; i <= gridSizeX - 1; ++i) {
    p.drawLine(1 + i * (width() / gridSizeX), 1, 1 + i * (width() / gridSizeX),
               height() - 1);
  }
  p.drawLine(width() - 1, 1, width() - 1, height() - 1);

  for (int i = 0; i <= gridSizeY - 1; ++i) {
    p.drawLine(1, 1 + i * (height() / gridSizeY), width() - 1,
               1 + i * (height() / gridSizeY));
  }
  p.drawLine(1, height() - 1, width() - 1, height() - 1);

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
