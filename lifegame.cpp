void killAll(void)                //����
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
void InitMap()        //��ʼ��ϸ������
{
    killAll();

    srand((unsigned)time(NULL)); //��ʱ�����֣�ÿ�β����������һ��

    for (int i = 0; i < WIDTH; ++i)
    {
        for (int j = 0; j < HEIGHT; ++j)
        {
            int Alive = rand() % 2;  //����0��1�������
            setCurCell(i, j, Alive);
        }

    }
}
void setCurCell(int x, int y, int Alive)  //���õ�ǰϸ������ϸ�����״̬
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
int locValid(int x, int y)     //�߽��ж�
{
    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
    {
        return 0;
    }
    return 1;
}