// main.cpp
// Chess游戏主程序
//
// Created by Lucifer Zhang on 2015-07-21.
// Copyright (c) 2015 Lucifer Zhang. All rights reserved.

#include <QApplication>
#include "Board.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Board board;
    board.show();
    return app.exec();
}
