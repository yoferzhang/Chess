// Board.cpp

#include "Board.h"
#include <QPainter> // 绘制棋盘需要
#include <QMouseEvent>

Board::Board(QWidget *parent) : QWidget(parent)
{
    for (int i = 0; i < 32; ++i) {
        _s[i].init(i);
    }
    _selectid = -1;
}

// 绘制棋盘
void Board::paintEvent(QPaintEvent *)
{
    DrawBackground();

    QPainter painter(this);

    int d = 40; // d表示棋盘格子的直径，也就是棋子的直径
    _r = d / 2;

    // 绘制10条横线
    for (int i = 1; i <= 10; ++i) {
        painter.drawLine(QPoint(d, i * d), QPoint(9 * d, i * d));
    }

    // 绘制9条竖线
    for (int i = 1; i <= 9; ++i) {
        if (1 == i || 9 == i) { // 中间有楚河汉界，不能全部都画通
            painter.drawLine(QPoint(i * d, d), QPoint(i * d, 10 * d));
        }
        else {
            painter.drawLine(QPoint(i * d, d), QPoint(i * d, 5 * d));
            painter.drawLine(QPoint(i * d, 6 * d), QPoint(i * d, 10 * d));
        }
    }

    // 绘制九宫格
    painter.drawLine(QPoint(4 * d, d), QPoint(6 * d, 3 * d));
    painter.drawLine(QPoint(4 * d, 3 * d), QPoint(6 * d, d));
    painter.drawLine(QPoint(4 * d, 8 * d), QPoint(6 * d, 10 * d));
    painter.drawLine(QPoint(4 * d, 10 * d), QPoint(6 * d, 8 * d));

    // 绘制32个棋子
    for (int i = 0; i < 32; ++i) {
        drawStone(painter, i);
    }
}

// 设置背景颜色
void Board::DrawBackground()
{
    QPalette p = this->palette();
    p.setColor(QPalette::Window, QColor(224, 255, 255));
    this->setPalette(p);
}

QPoint Board::center(int row, int col)
{
    QPoint ret;
    ret.rx() = (col + 1) * _r * 2;
    ret.ry() = (row + 1) * _r * 2;
    return ret;
}

// 重载center函数，方便调用
QPoint Board::center(int id)
{
    return center(_s[id]._row, _s[id]._col);
}

void Board::drawStone(QPainter &painter, int id)
{
    if (_s[id]._dead) {
        return;
    }

    QPoint c = center(id);
    QRect rect = QRect(c.x() - _r, c.y() - _r, _r * 2, _r * 2);

    if (id == _selectid) {
        painter.setBrush(QBrush(Qt::gray));
    }
    else {
        painter.setBrush(QBrush(QColor(255, 228, 181))); // 将棋子底色设置为鹿皮色
    }

    painter.setPen(Qt::black); // 先将画笔设置成黑色绘制圆形

    painter.drawEllipse(center(id), _r, _r); // 绘制圆形

    if (_s[id]._red) { // 将上方的棋子字体颜色设置为红色
        painter.setPen(Qt::red);
    }

    painter.setFont(QFont("system", _r, 700)); // 设置字体大小和类型

    painter.drawText(rect, _s[id].getText(), QTextOption(Qt::AlignCenter));
}

// 判断点击位置是否在某个顶点的圆内
// 返回值为bool类型是为了处理点击在棋盘外的情况
bool Board::getRowCol(QPoint pt, int &row, int &col)
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

void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    // 首先要获取鼠标点击的位置
    QPoint pt = ev->pos();

    // 看有没有点中象棋
    // 将pt转化成象棋的行列值
    // 判断这个行列值上面有没有棋子
    int row, col;
    bool bRet = getRowCol(pt, row, col);
    if (bRet == false) { // 点击位置无效
        return;
    }

    int i;
    int clickid = -1;
    for (i = 0; i < 32; ++i) {
        if (_s[i]._row == row &&
            _s[i]._col == col &&
            _s[i]._dead == false) {
            break;
        }
    }
    if (i < 32) {
        clickid = i;
    }

    if (_selectid == -1) { // 如果点中的棋子之前未被选中
        if (clickid != -1) {
            _selectid = clickid;
            update();
        }
    }
    else {
        _s[_selectid]._row = row;
        _s[_selectid]._col = col;
        if (clickid != -1) {
            _s[clickid]._dead = true;
        }
        _selectid = -1;
        update();
    }
}
