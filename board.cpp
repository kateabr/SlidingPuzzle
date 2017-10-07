#include "board.h"

Board Board::MakeBoard(int x, int y) {
  Board res;
  res.layout = new byte[x * y];
  res.gridSizeX = x;
  res.gridSizeY = y;

  std::iota(res.layout, res.layout + x * y, 0);

  do {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(res.layout, res.layout + x * y,
                 std::default_random_engine(seed));
  } while (!Board::validLayout(res));

  res.generateHash();

  return res;
}

bool Board::validLayout(const Board &b) {
  /*
   * source: http://www.cs.mcgill.ca/~newborn/nxp_puzzleOct9.htm
   * An NxP-puzzle with an odd number of columns can be
   * solved only if NI is an even number.
   *
   * An NxP-puzzle with an even number of columns
   * and an even number of rows can be solved only
   * if (NI + Row of the blank) is an even number.
   *
   * An NxP-puzzle with an even number of columns
   * and an odd number of rows can be solved only
   * if (NI + Row of the blank) is an odd number.
   */

  int inv = 0; // inversions

  int k = b.zeroPos() / b.gridSizeY + 1; // row of zero tile

  for (int i = 1; i < b.gridSizeX * b.gridSizeY; ++i)
    if (b.layout[i])
      for (int j = 0; j < i; ++j)
        if (b.layout[j]) {
          if (b.layout[j] > b.layout[i])
            ++inv;
        }

  if (b.gridSizeY % 2)
    return !(inv % 2);
  else if (!(b.gridSizeX % 2))
    return !((k + inv) % 2);
  else
    return (k + inv) % 2;
}

const byte *Board::getLayout() const { return layout; }

const byte &Board::getElem(int i, int j) const {
  return layout[j * gridSizeX + i];
}

Board Board::moveUp() const {
  Board res(*this);
  std::swap(res.layout[zeroPos()], res.layout[zeroPos() + gridSizeX]);
  res.generateHash();
  return res;
}

Board Board::moveDown() const {
  Board res(*this);
  std::swap(res.layout[zeroPos()], res.layout[zeroPos() - gridSizeX]);
  res.generateHash();
  return res;
}

bool Board::canMoveDown() { return zeroPos() >= gridSizeX; }

bool Board::canMoveUp() { return zeroPos() <= gridSizeX * (gridSizeY - 1) - 1; }

bool Board::canMoveLeft() { return (zeroPos() + 1) % gridSizeX; }

bool Board::canMoveRight() { return zeroPos() % gridSizeX; }

size_t Board::getHash() const { return boardHash; }

void Board::print() const {
  QDebug deb = qDebug();
  for (int i = 0; i < gridSizeX * gridSizeY; ++i)
    deb << layout[i] << " ";
  deb << endl;
}

Board Board::moveLeft() const {
  Board res(*this);
  std::swap(res.layout[zeroPos()], res.layout[zeroPos() + 1]);
  res.generateHash();
  return res;
}

Board Board::moveRight() const {
  Board res(*this);
  std::swap(res.layout[zeroPos()], res.layout[zeroPos() - 1]);
  res.generateHash();
  return res;
}

int Board::zeroPos() const {
  return std::distance(layout,
                       std::find(layout, layout + gridSizeX * gridSizeY, 0));
}

bool Board::final() const {
  for (int i = 0; i < gridSizeX * gridSizeY - 1; ++i)
    if (layout[i] != (i + 1))
      return false;
  return true;
}

int Board::heuristic() const {
  int res = 0;
  int correctI = 0;
  int correctJ = 0;
  int curI = 0;
  int curJ = 0;
  for (int i = 0; i < gridSizeX * gridSizeY; ++i)
    if (layout[i]) {
      correctI = (layout[i] - 1) / gridSizeY;
      correctJ = (layout[i] - 1) % gridSizeY;
      curI = i / gridSizeY;
      curJ = i % gridSizeY;
      res += qAbs(correctI - curI) + qAbs(correctJ - curJ);
    }
  return res;
}

Board::~Board() { delete[] layout; }

Board::Board(const Board &other)
    : gridSizeX(other.gridSizeX), gridSizeY(other.gridSizeY) {
  layout = new byte[gridSizeX * gridSizeY];
  boardHash = other.boardHash;
  std::copy(other.layout, other.layout + gridSizeX * gridSizeY, layout);
}

Board::Board(Board &&other)
    : gridSizeX(other.gridSizeX), gridSizeY(other.gridSizeY),
      layout(other.layout), boardHash(other.boardHash) {
  other.layout = nullptr;
}

Board &Board::operator=(const Board &other) {
  gridSizeX = other.gridSizeX;
  gridSizeY = other.gridSizeY;
  boardHash = other.boardHash;
  std::copy(other.layout, other.layout + gridSizeX * gridSizeY, layout);
  return *this;
}

Board &Board::operator=(Board &&other) {
  gridSizeX = other.gridSizeX;
  gridSizeY = other.gridSizeY;
  delete[] layout;
  layout = other.layout;
  other.layout = nullptr;
  boardHash = other.boardHash;
  return *this;
}

void Board::generateHash() {
  boardHash = 0;
  std::hash<std::string> strHash;
  std::string tempHash = "";
  for (int i = 0; i < gridSizeX * gridSizeY; ++i) {
    tempHash += std::to_string(layout[i]);
  }
  boardHash = strHash(tempHash);
}

Board::Board() : layout(nullptr), gridSizeX(0), gridSizeY(0) {}
