
#include "stdio.h"
#include "time.h"
#include "iostream"
using namespace std;

#define WIDTH 20
#define HEIGHT 20

typedef struct _SCell              //ϸ�����״̬
{
    int Alive;
} 
SCell;

SCell* current_map = (SCell *) new SCell[WIDTH * HEIGHT];
SCell* new_map = (SCell *) new SCell[WIDTH * HEIGHT];

//����
void setCurCell(int x, int y, int Alive);//���õ�ǰ����x,y����ϸ����״̬
void setNewCell(int x, int y, int Alive);//������һʱ�̣�x,y����ϸ����״̬
int getAroundCellNum(int x, int y); //���㣨x,y����Χ����ϸ�����
int locValid(int x, int y);   //�߽��ж�, 1-��Ч 0-��Ч
void swapMap(void);     //������ͼ
SCell* getCell(SCell* buf, int x, int y);  //�ӵ�ͼ�л�ȡĳ����ϸ��ָ��
void InitMap();  //��ʼ����ͼ
void killAll(void);     //�����ɱ������ϸ��
void nextStep(void);    //������һʱ��ϸ�����
int getCellAlive(int x, int y); //��ȡϸ����״̬ , ����ֵ:1-���, 0-���� -1-���




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

void setNewCell(int x, int y, int Alive)  //������һʱ��ϸ�������״̬
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

int getAroundCellNum(int x, int y)   //������Χ���ϸ�����
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



int locValid(int x, int y)     //边界判断

{
    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
    {
        return 0;
    }
    return 1;
}





/*
ÿ��ϸ���������ѭ�����ԭ��
1�� ���һ��ϸ����Χ��3��ϸ��Ϊ��һ��ϸ����Χ����8��ϸ�����ϸ��Ϊ����ϸ����ԭ��Ϊ�����תΪ����ԭ��Ϊ���򱣳ֲ��䣩 ��
2�� ���һ��ϸ����Χ��2��ϸ��Ϊ�����ϸ�������״̬���ֲ��䣻
3�� ����������£���ϸ��Ϊ��
*/

int getAroundCellNum(int x, int y)   //计算周围存活细胞数量
{
    int count = 0;
    if (locValid(x, y) == 0)  //边界判断
    {   
        return -1;
    }
    //测试目标位置周围的八个相邻位置
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


void nextStep(void)     //根据规则计算下一时刻细胞矩阵

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
	swapMap();
}




int getCellAlive(int x, int y)     //获取细胞存活状态

{
    if (locValid(x, y) == 0)
    {
        return -1;
    }
    SCell* cell = getCell(current_map, x, y);
    return cell->Alive;
}

void swapMap(void)//交换地图
{
        SCell* temp=current_map;
	current_map=new_map;
	new_map=temp;
	
}	



SCell* getCell(SCell* buf, int x, int y) //从地图中获取某坐标的细胞指针


{ 
	return buf + y * WIDTH + x; 
}




void print()  //输出细胞矩阵


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
	ramdomInit();
	print();
	while(1)
	{
		nextTurn();
		printf("�Ƿ�鿴��һ���ݻ���1 or 0����");
		scanf("%d",&a);
		switch (a){
			case 1: print(); break;
			default:{return 0;break;}
		}
	}

}

  
