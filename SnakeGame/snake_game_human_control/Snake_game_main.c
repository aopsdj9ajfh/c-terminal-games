#include "Snake_game.h"
#include <stdlib.h>
#include <time.h>

int main() {
    //设置随机数种子，保证每次运行游戏，食物加载位置都不一样
    srand((unsigned int)time(NULL));
    //启动游戏
    GameRun();

    return 0;
}