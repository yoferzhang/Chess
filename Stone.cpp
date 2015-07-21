#include "Stone.h"

Stone::Stone()
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
    };
    _id = id;
    _dead = false;
    _red = id < 16;
}

QString Stone::getText()
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
