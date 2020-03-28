void killAll(void)                //清屏
{
    if (current_map != NULL && new_map != NULL)
    {
        for (int i = 0; i < WIDTH; ++i)
        {
            for (int j = 0; j < HEIGHT; ++j)
            {
                setCurCell(i, j, 0);
                setNewCell(i, j, 0);
            }
        }
    }

}
void InitMap()        //初始化细胞矩阵
{
    killAll();

    srand((unsigned)time(NULL)); //用时间做种，每次产生随机数不一样

    for (int i = 0; i < WIDTH; ++i)
    {
        for (int j = 0; j < HEIGHT; ++j)
        {
            int Alive = rand() % 2;  //产生0或1的随机数
            setCurCell(i, j, Alive);
        }

    }
}
void setCurCell(int x, int y, int Alive)  //设置当前细胞矩阵细胞存活状态
{
    if (locValid(x, y) == 0)
    {
        return;
    }
    else
    {
        SCell* cell = getCell(current_map, x, y);
        if (cell - current_map >= WIDTH * HEIGHT)
        {
            return;
        }
        cell->Alive = Alive;
    }
}
int locValid(int x, int y)     //边界判断
{
    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
    {
        return 0;
    }
    return 1;
}