#include "solver.h"
#include <chrono>
#include <deque>
#include <queue>

using namespace std::chrono;

void BFSSolver::solve(Board &b) {
  std::deque<Board> layouts;
  QSet<size_t> visited;
  layouts.push_back(b);
  int ops = 0;

  auto start = system_clock::now();
  while (!layouts.empty()) {
    auto end = system_clock::now();
    duration<double> diff = duration_cast<milliseconds>(end - start);
    Board cur = layouts.front();
    layouts.pop_front();
    if (cur.final()) {
      b = cur;
      emit updateLayout();
      emit updateTime(diff.count());
      emit updateOps(ops);
      break;
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
    if (ops++ % 1000 == 0) {
      b = cur;
      emit updateLayout();
      emit updateTime(diff.count());
      emit updateOps(ops);
    }
  }
}

void AStarSolver::solve(Board &b) {
  auto comp = [&](QPair<int, Board> &p1, QPair<int, Board> &p2) {
    return p1.first > p2.first;
  };
  std::priority_queue<QPair<int, Board>, std::vector<QPair<int, Board>>,
                      decltype(comp)>
      layouts(comp);
  QSet<size_t> visited;
  layouts.emplace(QPair<int, Board>(b.heuristic(), b));
  int ops = 0;

  auto start = system_clock::now();
  while (!layouts.empty()) {
    auto end = system_clock::now();
    duration<double> diff = duration_cast<milliseconds>(end - start);
    Board cur = layouts.top().second;
    layouts.pop();
    if (cur.final()) {
      b = cur;
      emit updateLayout();
      emit updateTime(diff.count());
      emit updateOps(ops);
      break;
    }
    if (visited.contains(cur.getHash()))
      continue;
    if (cur.canMoveDown()) {
      Board bb = cur.moveDown();
      layouts.emplace(QPair<int, Board>(bb.heuristic(), bb));
    }
    if (cur.canMoveLeft()) {
      Board bb = cur.moveLeft();
      layouts.emplace(QPair<int, Board>(bb.heuristic(), bb));
    }
    if (cur.canMoveRight()) {
      Board bb = cur.moveRight();
      layouts.emplace(QPair<int, Board>(bb.heuristic(), bb));
    }
    if (cur.canMoveUp()) {
      Board bb = cur.moveUp();
      layouts.emplace(QPair<int, Board>(bb.heuristic(), bb));
    }
    visited.insert(cur.getHash());
    if (ops++ % 1000 == 0) {
      b = cur;
      emit updateLayout();
      emit updateTime(diff.count());
      emit updateOps(ops);
    }
  }
}
