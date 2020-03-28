#include "stdio.h"
#include "time.h"
#include "iostream"
using namespace std;

#define WIDTH 20
#define HEIGHT 20

typedef struct _SCell              //ϸ������״̬
{
    int Alive;
} 
SCell;

SCell* current_map = (SCell *) new SCell[WIDTH * HEIGHT];
SCell* new_map = (SCell *) new SCell[WIDTH * HEIGHT];

//����
void setCurCell(int x, int y, int Alive);//���õ�ǰ����x,y����ϸ�����״̬
void setNewCell(int x, int y, int Alive);//������һʱ�̣�x,y����ϸ�����״̬
int getAroundCellNum(int x, int y); //���㣨x,y����Χ����ϸ������
int locValid(int x, int y);   //�߽��ж�, 1-��Ч 0-��Ч
void swapMap(void);     //������ͼ
SCell* getCell(SCell* buf, int x, int y);  //�ӵ�ͼ�л�ȡĳ�����ϸ��ָ��
void InitMap();  //��ʼ����ͼ
void killAll(void);     //������ɱ������ϸ��
void nextStep(void);    //������һʱ��ϸ������
int getCellAlive(int x, int y); //��ȡϸ�����״̬ , ����ֵ:1-���, 0-���� -1-����



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

void setNewCell(int x, int y, int Alive)  //������һʱ��ϸ��������״̬
{
    if (locValid(x, y) == 0)
    {
        return;
    }
    else
    {
        SCell* cell = getCell(new_map, x, y);
        if (cell - new_map >= WIDTH * HEIGHT)
        {
            return;
        }
        cell->Alive = Alive;
    }
}

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

int locValid(int x, int y) //�߽��ж�
{
    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
    {
        return 0;
    }
    return 1;
}




/*
ÿ��ϸ����������ѭ�����ԭ��
1�� ���һ��ϸ����Χ��3��ϸ��Ϊ����һ��ϸ����Χ����8��ϸ���������ϸ��Ϊ��������ϸ����ԭ��Ϊ������תΪ������ԭ��Ϊ�����򱣳ֲ��䣩 ��
2�� ���һ��ϸ����Χ��2��ϸ��Ϊ�������ϸ��������״̬���ֲ��䣻
3�� ����������£���ϸ��Ϊ��
*/
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

int main()
{
	int a;
	InitMap();
	print();
		nextStep();
		printf("�Ƿ�鿴��һ���ݻ���1 or 0����");
		scanf("%d",&a);
		switch (a){
			case 1: print(); break;
			default:{return 0;break;}
		}

}