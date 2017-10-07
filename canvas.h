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

protected:
  void paintEvent(QPaintEvent *e);

private:
  int gridSizeX = 3;
  int gridSizeY = 3;
  Board<int> b;
  Solver<int> *solver;
};

#endif // CANVAS_H
