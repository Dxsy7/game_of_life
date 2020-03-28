int getAroundCellNum(int x, int y)   //������Χ���ϸ������
{
    int count = 0;
    if (locValid(x, y) == 0)  //�߽��ж�
    {   
        return -1;
    }
    //����Ŀ��λ����Χ�İ˸�����λ��
    for (int i = x - 1; i <= x + 1; ++i)
    {
        for (int j = y - 1; j <= y + 1; ++j)
        {
            if (i == x && j == y)
            {
                continue;
            }
            if (locValid(i, j) == 1)
            {
                if (getCellAlive(i, j) == 1)
                {
                    count++;
                }
            }
        }
    }
    return count;
}
void nextStep(void)     //���ݹ��������һʱ��ϸ������
{
    int aroundNum = 0;
    for (int i = 0; i < WIDTH; ++i)
    {
        for (int j = 0; j < HEIGHT; ++j)
        {
            aroundNum = getAroundCellNum(i, j);
            if (aroundNum == 2)
            {
                setNewCell(i, j, getCellAlive(i, j));
            }
            else if (aroundNum == 3)
            {
                setNewCell(i, j, 1);
            }
            else
            {
                setNewCell(i, j, 0);
            }
        }
    }
}
int getCellAlive(int x, int y)     //��ȡϸ�����״̬
{
    if (locValid(x, y) == 0)
    {
        return -1;
    }
    SCell* cell = getCell(current_map, x, y);
    return cell->Alive;
}
SCell* getCell(SCell* buf, int x, int y) //�ӵ�ͼ�л�ȡĳ�����ϸ��ָ��
{ 
	return buf + y * WIDTH + x; 
}
void print()  //���ϸ������
{
	int k;
	for(int i=0;i<WIDTH;i++)
	{
		for(int j=0;j<HEIGHT;j++)
		{
		  k=getCellAlive(i,j);
		  if(k==0)  printf("0 ");
		  else if(k==1) printf("1 ");
		}
		printf("\n");
	}
	
}


