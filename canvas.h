#ifndef CANVAS_H
#define CANVAS_H

#include "board.h"
#include "solver.h"
#include <QtWidgets>

class Canvas : public QWidget {
  Q_OBJECT
public:
  explicit Canvas(QWidget *parent = nullptr);
  ~Canvas();

public slots:
  void setGridSizeX(int sz);
  void setGridSizeY(int sz);
  void solvePuzzle();
  void setSolvingMethod(int m);

signals:
  void updateTime(int);
  void updateOps(int);
  void showControls(bool);

protected:
  void paintEvent(QPaintEvent *);

private:
  void connectSolverWithUI();

  QVector<QColor> colors;

  int gridSizeX = 3;
  int gridSizeY = 3;
  Board b;
  Solver *solver;

  QFuture<void> solveFuture;
  QFutureWatcher<void> solveFutureWatcher;
};

#endif // CANVAS_H
