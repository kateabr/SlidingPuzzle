#ifndef SOLVER_H
#define SOLVER_H

#include "board.h"
#include <QObject>

class Solver : public QObject {
  Q_OBJECT
public:
  virtual void solve(Board &b) = 0;

signals:
  void updateLayout();
  void updateTime(int);
  void updateOps(int);
};

class BFSSolver : public Solver {
public:
  void solve(Board &b) override;
};

class AStarSolver : public Solver {
public:
  void solve(Board &b) override;
};

#endif // SOLVER_H
