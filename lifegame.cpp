#include "stdio.h"
#include "time.h"
#include "iostream"
using namespace std;

#define WIDTH 20
#define HEIGHT 20

typedef struct _SCell              //细胞生存状态
{
    int Alive;
} 
SCell;

SCell* current_map = (SCell *) new SCell[WIDTH * HEIGHT];
SCell* new_map = (SCell *) new SCell[WIDTH * HEIGHT];

//声明
void setCurCell(int x, int y, int Alive);//设置当前矩阵（x,y）的细胞存活状态
void setNewCell(int x, int y, int Alive);//设置下一时刻（x,y）的细胞存活状态
int getAroundCellNum(int x, int y); //计算（x,y）周围存活的细胞数量
int locValid(int x, int y);   //边界判定, 1-有效 0-无效
void swapMap(void);     //交换地图
SCell* getCell(SCell* buf, int x, int y);  //从地图中获取某坐标的细胞指针
void InitMap();  //初始化地图
void killAll(void);     //清屏，杀死所有细胞
void nextStep(void);    //计算下一时刻细胞矩阵
int getCellAlive(int x, int y); //获取细胞存活状态 , 返回值:1-存活, 0-死亡 -1-出错



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

void setNewCell(int x, int y, int Alive)  //设置下一时刻细胞矩阵存活状态
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

int locValid(int x, int y) //边界判断
{
    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
    {
        return 0;
    }
    return 1;
}




/*
每个细胞的生死遵循下面的原则：
1． 如果一个细胞周围有3个细胞为生（一个细胞周围共有8个细胞），则该细胞为生（即该细胞若原先为死，则转为生，若原先为生，则保持不变） 。
2． 如果一个细胞周围有2个细胞为生，则该细胞的生死状态保持不变；
3． 在其它情况下，该细胞为死
*/
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
	InitMap();
	print();
		nextStep();
		printf("是否查看下一步演化（1 or 0）：");
		scanf("%d",&a);
		switch (a){
			case 1: print(); break;
			default:{return 0;break;}
		}

}