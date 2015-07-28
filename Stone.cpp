// Stone.cpp

#include "Stone.h"
#include <QDebug>

Stone::Stone()
{

}

Stone::~Stone()
{

}

void Stone::init(int id)
{
    struct {
        int row, col;
        Stone::TYPE type;
    } pos[16] = {
    {0, 0, Stone::CHE},
    {0, 1, Stone::MA},
    {0, 2, Stone::XIANG},
    {0, 3, Stone::SHI},
    {0, 4, Stone::JIANG},
    {0, 5, Stone::SHI},
    {0, 6, Stone::XIANG},
    {0, 7, Stone::MA},
    {0, 8, Stone::CHE},

    {2, 1, Stone::PAO},
    {2, 7, Stone::PAO},
    {3, 0, Stone::BING},
    {3, 2, Stone::BING},
    {3, 4, Stone::BING},
    {3, 6, Stone::BING},
    {3, 8, Stone::BING},
    };

    //_id = id;
    _dead = false;
    _red = id < 16;

    if (id < 16) { // 上方的棋子
        this->_row = pos[id].row;
        this->_col = pos[id].col;
        this->_type = pos[id].type;
    }
    else { // 下方的棋子
        this->_row = 9 - pos[id - 16].row;
        this->_col = 8 - pos[id - 16].col;
        this->_type = pos[id - 16].type;
    }
}

QString Stone::name()
{
    switch (this->_type)
    {
    case CHE:
        return "车";
    case MA:
        return "马";
    case PAO:
        return "炮";
    case BING:
        return "兵";
    case JIANG:
        return "将";
    case SHI:
        return "士";
    case XIANG:
        return "相";
    }
    return "错误";
}

void Stone::rotate()
{
    this->_col = 8 - this->_col;
    this->_row = 9 - this->_row;
}
