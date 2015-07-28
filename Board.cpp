// Board.cpp

#include "Board.h"
#include <QPainter> // 绘制棋盘需要
#include <QMouseEvent>
#include <QDebug>
#define GetRowCol(__row, __col, __id) \
    int __row = _s[__id]._row;\
    int __col = _s[__id]._col

Board::Board(QWidget *parent) : QFrame(parent)
{
    this->_r = 20;
    setMinimumSize(_r * 18 + 1, _r * 20 + 1);
    init(true);
}

Board::~Board()
{

}

void Board::init(bool bRedSide)
{
    for (int i = 0; i < 32; ++i) {
        _s[i].init(i);
    }
    if (bRedSide) {
        for (int i = 0; i < 32; ++i) {
            _s[i].rotate();
        }
    }

    _selectid = -1;
    _bRedTurn = true;
    _bSide = bRedSide;
    update();
}

// 绘制棋盘
void Board::paintEvent(QPaintEvent *)
{
    DrawBackground(); // 绘制背景颜色

    int r = height() / 20;
    _r = r;
    _off = QPoint(r + 1, r + 1);

    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    p.save();
    drawPlate(p);
    p.restore();

    p.save();
    drawPlace(p);
    p.restore();

    p.save();
    drawInitPosition(p);
    p.restore();

    p.save();
    drawStone(p);
    p.restore();
}

// 设置背景颜色
void Board::DrawBackground()
{
    QPalette p = this->palette();
    p.setColor(QPalette::Window, QColor(224, 255, 255));
    this->setPalette(p);
}

// 绘制棋盘
void Board::drawPlate(QPainter &p)
{
    // 绘制10条横线
    for (int i = 0; i < 10; ++i) {
        if (i == 0 || i == 9) { // 上下边框画笔设置的粗一些
            p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
        }
        else {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
        }
        p.drawLine(center(i, 0), center(i, 8));
    }

    // 绘制9条竖线
    for (int i = 0; i < 9; ++i) {
        if (i == 0 || i == 8) { // 中间有楚河汉界，不能画通
            p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
            p.drawLine(center(0, i), center(9, i));
        }
        else {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
            p.drawLine(center(0, i), center(4, i));
            p.drawLine(center(5, i), center(9, i));
        }
    }
}

// 绘制九宫格
void Board::drawPlace(QPainter &p)
{
    p.drawLine(center(0, 3), center(2, 5));
    p.drawLine(center(2, 3), center(0, 5));

    p.drawLine(center(9, 3), center(7, 5));
    p.drawLine(center(7, 3), center(9, 5));
}

// 绘制炮兵位置上的十字
void Board::drawInitPosition(QPainter &p)
{
    drawInitPosition(p, 3, 0);
    drawInitPosition(p, 3, 2);
    drawInitPosition(p, 3, 4);
    drawInitPosition(p, 3, 6);
    drawInitPosition(p, 3, 8);

    drawInitPosition(p, 6, 0);
    drawInitPosition(p, 6, 2);
    drawInitPosition(p, 6, 4);
    drawInitPosition(p, 6, 6);
    drawInitPosition(p, 6, 8);

    drawInitPosition(p, 2, 1);
    drawInitPosition(p, 2, 7);

    drawInitPosition(p, 7, 1);
    drawInitPosition(p, 7, 7);
}

void Board::drawInitPosition(QPainter& p, int row, int col)
{
    QPoint pt = center(row, col);
    QPoint off = QPoint(_r / 6, _r / 6);
    int len = _r / 3;

    QPoint ptStart;
    QPoint ptEnd;

    if (col != 0) {
        // 左上角
        ptStart = QPoint(pt.x() - off.x(), pt.y() - off.y());
        ptEnd = ptStart + QPoint(-len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, -len);
        p.drawLine(ptStart, ptEnd);

        // 左下角
        ptStart = QPoint(pt.x() - off.x(), pt.y() + off.y());
        ptEnd = ptStart + QPoint(-len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, +len);
        p.drawLine(ptStart, ptEnd);
    }

    if (col != 8) {
        // 右下角
        ptStart = QPoint(pt.x() + off.x(), pt.y() + off.y());
        ptEnd = ptStart + QPoint(+len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, +len);
        p.drawLine(ptStart, ptEnd);

        // 右上角
        ptStart = QPoint(pt.x() + off.x(), pt.y() - off.y());
        ptEnd = ptStart + QPoint(+len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, -len);
        p.drawLine(ptStart, ptEnd);
    }
}

// 绘制棋子
void Board::drawStone(QPainter &p)
{
    for (int i = 0; i < 32; ++i) {
        drawStone(p, i);
    }
}

void Board::drawStone(QPainter &p, int id)
{
    if (isDead(id)) {
        return;
    }

    QColor color;
    if (red(id)) {
        color = Qt::red;
    }
    else {
        color = Qt::black;
    }

    p.setPen(QPen(QBrush(color), 2));

    if (id == _selectid) {
        p.setBrush(Qt::gray);
    }
    else {
        p.setBrush(Qt::yellow);
    }

    p.drawEllipse(cell(id));

    p.setFont(QFont("system", _r * 1.2, 700)); // 设置字体大小和类型
    p.drawText(cell(id), name(id), QTextOption(Qt::AlignCenter));
}

// 返回棋盘行列对应的像素坐标
QPoint Board::center(int row, int col)
{
    QPoint pt(_r * col * 2, _r * row * 2);
    return pt + _off;
}

// 重载center函数，方便调用
QPoint Board::center(int id)
{
    return center(_s[id]._row, _s[id]._col);
}

QPoint Board::topLeft(int row, int col)
{
    return center(row, col) - QPoint(_r, _r);
}

QPoint Board::topLeft(int id)
{
    return center(id) - QPoint(_r, _r);
}

QRect Board::cell(int row, int col)
{
    return QRect(topLeft(row, col), QSize(_r * 2 - 1, _r * 2 - 1));
}

QRect Board::cell(int id)
{
    return QRect(topLeft(id), QSize(_r * 2 - 1, _r * 2 - 1));
}

// 判断点击位置属于哪个顶点
// 返回值为bool类型是为了处理点击在棋盘外的情况
bool Board::getClickRowCol(QPoint pt, int &row, int &col)
{
    row = pt.y() / (2 * _r) - 1;
    col = pt.x() / (2 * _r) - 1;

    QPoint c = center(row, col);
    int dx = c.x() - pt.x();
    int dy = c.y() - pt.y();
    int dist = dx * dx + dy * dy; // 和鼠标所处矩形左上顶点的距离
    if (dist < _r * _r) {
        return true;
    }

    row += 1;
    c = center(row, col);
    dx = c.x() - pt.x();
    dy = c.y() - pt.y();
    dist = dx * dx + dy * dy; // 和鼠标所处矩形左下顶点的距离
    if (dist < _r * _r) {
        return true;
    }

    row -= 1;
    col += 1;
    c = center(row, col);
    dx = c.x() - pt.x();
    dy = c.y() - pt.y();
    dist = dx * dx + dy * dy; // 和鼠标所处矩形右上顶点的距离
    if (dist < _r * _r) {
        return true;
    }

    row += 1;
    c = center(row, col);
    dx = c.x() - pt.x();
    dy = c.y() - pt.y();
    dist = dx * dx + dy * dy; // 和鼠标所处矩形右下顶点的距离
    if (dist < _r * _r) {
        return true;
    }

    return false;
}

QString Board::name(int id)
{
    return _s[id].name();
}

bool Board::red(int id)
{
    return _s[id]._red;
}

bool Board::sameColor(int id1, int id2)
{
    if (id1 == -1 || id2 == -1) {
        return false;
    }
    return red(id1) == red(id2);
}

int Board::getStoneId(int row, int col)
{
    for (int i = 0; i < 32; ++i) {
        if (_s[i]._row == row &&
            _s[i]._col == col && !isDead(i)) {
            return i;
        }
    }
    return -1; // 如果不是棋子返回-1
}

void Board::killStone(int id)
{
    if (id == -1) {
        return;
    }
    _s[id]._dead = true;
}

void Board::reliveStone(int id)
{
    if (id == -1) {
        return;
    }
    _s[id]._dead = false;
}

void Board::moveStone(int moveid, int row, int col)
{
    _s[moveid]._row = row;
    _s[moveid]._col = col;

    _bRedTurn = !_bRedTurn;
}

bool Board::isDead(int id)
{
    if (id == -1) {
        return true;
    }
    return _s[id]._dead;
}

void Board::saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps)
{
    GetRowCol(row1, col1, moveid);
    Step* step = new Step;
    step->_colFrom = col1;
    step->_colTo = col;
    step->_rowFrom = row1;
    step->_rowTo = row;
    step->_moveid = moveid;
    step->_killid = killid;

    steps.append(step);
}

void Board::backOne()
{
    if (this->_steps.size() == 0) {
        return;
    }

    Step* step = this->_steps.last();
    _steps.removeLast();
    back(step);

    update();
    delete step;
}

void Board::back(Step* step)
{
    reliveStone(step->_killid);
    moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
}

void Board::back()
{
    backOne();
}

void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() != Qt::LeftButton) { // 排除鼠标右键点击
        return;
    }

    click(ev->pos());
}

void Board::click(QPoint pt)
{
    // 看有没有点中象棋
    // 将pt转化成象棋的行列值
    // 判断这个行列值上面有没有棋子
    int row, col;
    bool bClicked = getClickRowCol(pt, row, col);
    if (!bClicked) {
        return;
    }

    int id = getStoneId(row, col);
    click(id, row, col);

}

void Board::click(int id, int row, int col)
{
    if (this->_selectid == -1) { // 如果点中的棋子之前未被选中
        trySelectStone(id);
    }
    else {
        tryMoveStone(id, row, col);
    }
}

void Board::trySelectStone(int id)
{
    if (id == -1) {
        return;
    }

    if (!canSelect(id)) {
        return;
    }

    _selectid = id;
    update();
}

void Board::tryMoveStone(int killid, int row, int col)
{
    if (killid != -1 && sameColor(killid, _selectid)) {
        trySelectStone(killid);
        return;
    }

    bool ret = canMove(_selectid, killid, row, col);
    if (ret) {
        moveStone(_selectid, killid, row, col);
        _selectid = -1;
        update();
    }
}

void Board::moveStone(int moveid, int killid, int row, int col)
{
    saveStep(moveid, killid, row, col, _steps);

    killStone(killid);
    moveStone(moveid, row, col);
}

bool Board::canMove(int moveid, int killid, int row, int col)
{
    // 如果moveid和killid颜色相同，则不能移动，还需要换选择
    if (_s[moveid]._red == _s[killid]._red) {
        _selectid = killid;
        update();
        return false;
    }


    switch (_s[moveid]._type) {
    case Stone::JIANG:
        return canMoveJiang(moveid, killid, row, col);

    case Stone::SHI:
        return canMoveShi(moveid, killid, row, col);

    case Stone::XIANG:
        return canMoveXiang(moveid, killid, row, col);

    case Stone::CHE:
        return canMoveChe(moveid, killid, row, col);

    case Stone::MA:
        return canMoveMa(moveid, killid, row, col);

    case Stone::PAO:
        return canMovePao(moveid, killid, row, col);

    case Stone::BING:
        return canMoveBing(moveid, killid, row, col);
    }

    return true;

}


bool Board::canMoveJiang(int moveid, int killid, int row, int col)
{
    // 可直接吃对方将
    if (killid != -1 && _s[killid]._type == Stone::JIANG)
    {
        return canMoveChe(moveid, killid, row, col);
    }

    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if (r != 1 || r != 10) {
        return false;
    }

    if (col < 3 || col > 5) {
        return false;
    }

    if (isBottomSide(moveid)) {
        if (row < 7) {
            return false;
        }
    }
    else {
        if (row > 2) {
            return false;
        }
    }

    return true;
}

bool Board::canMoveShi(int moveid, int, int row, int col)
{

    // 移动步长一个格子对角线
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if (r != 11) {
        return false;
    }

    if (col < 3 || col > 5) {
        return false;
    }

    if (isBottomSide(moveid)) {
        if (row < 7) {
            return false;
        }
    }
    else {
        if (row > 2) {
            return false;
        }
    }

    return true;
}

bool Board::canMoveXiang(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if (r != 22) { // 象走田，所以r应该等于22
        return false;
    }

    // 看象眼有没有棋子
    int rEye = (row + row1) / 2;
    int cEye = (col + col1) / 2;
    if (getStoneId(rEye, cEye) != -1) {
        return false;
    }

    // 判断是否在棋盘的下方
    if (isBottomSide(moveid)) {
        if (row < 4) {
            return false;
        }
    }
    else {
        if (row > 5) {
            return false;
        }
    }

    return true;
}

bool Board::canMoveChe(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int ret = getStoneCountAtLine(row1, col1, row, col);
    if (ret == 0) { // 在一行，并且中间没有棋子
        return true;
    }

    return false;
}

bool Board::canMoveMa(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    // 首先判断马要走马字
    if (r != 12 && r != 21) {
        return false;
    }

    // 判断有没有蹩马腿的情况
    if (r == 12) { // 列相差等于2
        if (getStoneId(row1, (col + col1) / 2) != -1) {
            return false;
        }
    }
    else { // 行相差等于2
        if (getStoneId((row + row1) / 2, col1) != -1) {
            return false;
        }
    }
    return true;
}

bool Board::canMovePao(int moveid, int killid, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int ret = getStoneCountAtLine(row, col, row1, col1);
    if (killid != -1) { // 如果炮要吃对方的棋子
        if (ret == 1) { // 中间有一个棋子，可以走
            return true;
        }
    }
    else { // 如果炮不吃棋子
        if (ret == 0) { // 中间没有棋子，可以走
            return true;
        }
    }

    return false;
}

bool Board::canMoveBing(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    // 首先判断兵只能走一步
    if (r != 1 && r != 10) {
        return false;
    }

    if (isBottomSide(moveid)) { // 下面一方的棋子
        if (row > row1) { // 如果目标行大于原始行，相当于并在后退
            return false;
        }
        if (row1 >= 5 && row == row1) { // 还没有过河就想横着走
            return false;
        }
    }
    else { // 上面一方的棋子
        if (row1 > row) { // 如果目标行小于原始行，相当于兵在后退
            return false;
        }
        if (row <= 4 && row == row1) { // 还没有过河就想横着走
            return false;
        }
    }
    return true;
}

bool Board::canSelect(int id)
{
    return _bRedTurn == _s[id]._red;
}

int Board::relation(int row1, int col1, int row, int col)
{
    return qAbs(row1 - row) * 10 + qAbs(col1 - col);
}

bool Board::isBottomSide(int id)
{
    return _bSide == _s[id]._red;
}

int Board::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;

    // 首先判断两个棋子是否在同一条直线上，如果不在同一条直线上，直接返回-1
    if (row1 != row2 && col1 != col2) {
        return -1;
    }
    if (row1 == row2 && col1 == col2) {
        return -1;
    }

    // 计算两个棋子之间的有多少个棋子
    if (row1 == row2) { // 在同一行
        int min = col1 < col2 ? col1 : col2;
        int max = col1 > col2 ? col1 : col2;
        for (int col = min + 1; col < max; ++col) {
            if (getStoneId(row1, col) != -1) {
                ++ret;
            }
        }
    }
    else { // 在同一列
        int min = row1 < row2 ? row1 : row2;
        int max = row1 > row2 ? row1 : row2;
        for (int row = min + 1; row < max; ++row) {
            if (getStoneId(row, col1) != -1) {
                ++ret;
            }
        }
    }

    return ret;
}

void Board::slotBack()
{
    back();
}
