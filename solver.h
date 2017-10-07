#ifndef SOLVER_H
#define SOLVER_H

#include "board.h"
#include <QQueue>

template <typename T> class Solver {
public:
  virtual void solve(Board<T> &b) = 0;
};

template <typename T> class SimpleSolver : public Solver<T> {
public:
  void solve(Board<T> &b) override {
    QQueue<Board<T>> layouts;
    QSet<size_t> visited;
    layouts.push_back(b);
    while (!layouts.empty()) {
      Board<T> cur = layouts.takeFirst();
      if (cur.final()) {
        b = cur;
        return;
      }
      if (visited.contains(cur.getHash()))
        continue;
      if (cur.canMoveDown())
        layouts.push_back(cur.moveDown());
      if (cur.canMoveLeft())
        layouts.push_back(cur.moveLeft());
      if (cur.canMoveRight())
        layouts.push_back(cur.moveRight());
      if (cur.canMoveUp())
        layouts.push_back(cur.moveUp());
      visited.insert(cur.getHash());
    }
  }
};

#endif // SOLVER_H
