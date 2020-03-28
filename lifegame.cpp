#include "stdio.h"
#include "time.h"
#include "iostream"
using namespace std;

#define m_width 20
#define m_height 20

typedef struct _SCell
{
    int isAlive;
} 
SCell;

SCell* m_cur_map = (SCell *) new SCell[m_width * m_height];
SCell* m_new_map = (SCell *) new SCell[m_width * m_height];

//声明
void setCurCell(int x, int y, int isAlive);
void setNewCell(int x, int y, int isAlive);
int getAroundCellNum(int x, int y); //获得某个位置周围存活的细胞数量
int isPosValid(int x, int y);   //判定输入位置是否有效, 1-有效 0-无效
void swapMap(void);     //交换地图
SCell* getCell(SCell* buf, int x, int y);  //从地图buffer中获取某坐标的细胞指针
void ramdomInit();  //随机初始化地图
void killAll(void);     //杀死所有细胞
void nextTurn(void);    //进入下一回合
int getCellAlive(int x, int y); //获取细胞存活状态 , 返回值:1-存活, 0-死亡 -1-出错
int setCellAlive(int x, int y, int isAlive);    //设置细胞存活状态 , 返回值:0-成功 负值-失败


void killAll(void)
{
    if (m_cur_map != NULL && m_new_map != NULL)
    {
        for (int i = 0; i < m_width; ++i)
        {
            for (int j = 0; j < m_height; ++j)
            {
                setCurCell(i, j, 0);
                setNewCell(i, j, 0);
            }
        }
    }

}

void ramdomInit()
{
    killAll();

    srand((unsigned)time(NULL)); //用时间做种，每次产生随机数不一样

    for (int i = 0; i < m_width; ++i)
    {
        for (int j = 0; j < m_height; ++j)
        {
            int isAlive = rand() % 2;  //产生0或1的随机数
            setCurCell(i, j, isAlive);
        }

    }
}

void setCurCell(int x, int y, int isAlive)
{
    if (isPosValid(x, y) == 0)
    {
        return;
    }
    else
    {
        SCell* cell = getCell(m_cur_map, x, y);
        if (cell - m_cur_map >= m_width * m_height)
        {
            return;
        }
        cell->isAlive = isAlive;
    }
}

void setNewCell(int x, int y, int isAlive)
{
    if (isPosValid(x, y) == 0)
    {
        return;
    }
    else
    {
        SCell* cell = getCell(m_new_map, x, y);
        if (cell - m_new_map >= m_width * m_height)
        {
            return;
        }
        cell->isAlive = isAlive;
    }
}

int getAroundCellNum(int x, int y)
{
    int count = 0;

    if (isPosValid(x, y) == 0)
    {   //输入不合法
        return -1;
    }
    //尝试目标位置周围的八个相邻位置
    for (int i = x - 1; i <= x + 1; ++i)
    {
        for (int j = y - 1; j <= y + 1; ++j)
        {
            if (i == x && j == y)
            {
                continue;
            }
            if (isPosValid(i, j) == 1)
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

int isPosValid(int x, int y)
{
    if (x >= m_width || x < 0 || y >= m_height || y < 0)
    {
        return 0;
    }
    return 1;
}




/*
每个细胞的生死遵循下面的原则：
1． 如果一个细胞周围有3个细胞为生（一个细胞周围共有8个细胞），则该细胞为生（即该细胞若原先为死，则转为生，若原先为生，则保持不变） 。
2． 如果一个细胞周围有2个细胞为生，则该细胞的生死状态保持不变；
3． 在其它情况下，该细胞为死（即该细胞若原先为生，则转为死，若原先为死，则保持不变）
*/
void nextTurn(void)
{
    int aroundNum = 0;
    for (int i = 0; i < m_width; ++i)
    {
        for (int j = 0; j < m_height; ++j)
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

int getCellAlive(int x, int y)
{
    if (isPosValid(x, y) == 0)
    {
        return -1;
    }
    SCell* cell = getCell(m_cur_map, x, y);
    return cell->isAlive;
}

int setCellAlive(int x, int y, int isAlive)
{
    if (isPosValid(x, y) == 0)
    {
        return -1;
    }
    if (isAlive != 0 && isAlive != 1)
    {
        return -2;
    }
    SCell* cell = getCell(m_cur_map, x, y);
    cell->isAlive = isAlive;
    return 0;
}

void swapMap(void) //交换地图
{ 
	SCell* temp = m_cur_map; 
	m_cur_map = m_new_map; 
	m_new_map = temp; 
}    

SCell* getCell(SCell* buf, int x, int y) //从地图buffer中获取某坐标的细胞指针
{ 
	return buf + y * m_width + x; 
}

void print()
{
	int k;
	for(int i=0;i<m_width;i++)
	{
		for(int j=0;j<m_height;j++)
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
		printf("是否查看下一步演化（1 or 0）：");
		scanf("%d",&a);
		switch (a){
			case 1: print(); break;
			default:{return 0;break;}
		}
	}

}
