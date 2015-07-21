// Board.cpp

#include "Board.h"
#include <QPainter> // 绘制棋盘需要

Board::Board(QWidget *parent) : QWidget(parent)
{
    for (int i = 0; i < 32; ++i) {
        _s[i].init(i);
    }
}

// 绘制棋盘
void Board::paintEvent(QPaintEvent *)
{
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
    QPoint c = center(id);
    QRect rect = QRect(c.x() - _r, c.y() - _r, _r * 2, _r * 2);
    painter.drawEllipse(center(id), _r, _r);
    painter.drawText(rect, _s[id].getText(), QTextOption(Qt::AlignCenter));
}
