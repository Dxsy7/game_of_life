#include "stdio.h"
#include "time.h"
#include "windows.h"
#include "windowsx.h"
#include "iostream"
using namespace std;

#define WORLD_TIMER_ID (1)  //定时器ID
#define WORLD_TIMER_ELAPSE (1000)   //定时器超时时间
#define RANDOM_BTN_ID (1)
#define START_BTN_ID (2)
#define PAUSE_BTN_ID (3)
#define NEXT_BTN_ID (4)
#define KILL_ALL_BTN_ID (5)

#define WINDOW_WIDTH  820
#define WINDOW_HEIGHT 600
#define WORLD_WIDTH 40
#define WORLD_HEIGHT 20
#define CELL_SIZE 20

#define WIDTH 40
#define HEIGHT 20

class CWorld *g_world = NULL;   //全局世界指针

typedef struct _SCell              //细胞生存状态
{
    int Alive;
} 
SCell;

class CWorld
{
private:
    int width;
    int height;
    SCell* current_map;   //当前地图
    SCell* new_map;   //下一轮使用的地图
    void setCurCell(int x, int y, int Alive);
    void setNewCell(int x, int y, int Alive);
    int getAroundCellNum(int x, int y); //获得某个位置周围存活的细胞数量
    int locValid(int x, int y);   //判定输入位置是否有效, 1-有效 0-无效
    void swapMap(void) ;    //交换地图
    SCell* getCell(SCell* buf, int x, int y); //从地图buffer中获取某坐标的细胞指针
public:
    CWorld(int Width, int Height);
    ~CWorld();
    void InitMap(void);  //随机初始化地图
    void killAll(void);     //杀死所有细胞
    void nextStep(void);    //进入下一回合
    int getCellAlive(int x, int y); //获取细胞存活状态 , 返回值:1-存活, 0-死亡 -1-出错
    int setCellAlive(int x, int y, int Alive);    //设置细胞存活状态 , 返回值:0-成功 负值-失败
    int getWidth() { return width; }      //获得当前地图宽度
    int getHeight() { return height; }    //获得当前地图高度
};


SCell* current_map = (SCell *) new SCell[WIDTH * HEIGHT];
SCell* new_map = (SCell *) new SCell[WIDTH * HEIGHT];

void CWorld::killAll(void)                //清屏
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


void CWorld::InitMap()        //初始化细胞矩阵
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


void CWorld::setCurCell(int x, int y, int Alive)  //设置当前细胞矩阵细胞存活状态
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


void CWorld::setNewCell(int x, int y, int Alive)  //设置下一时刻细胞矩阵存活状态
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


int CWorld::getAroundCellNum(int x, int y)   //计算周围存活细胞数量
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


int CWorld::locValid(int x, int y) //边界判断
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

void CWorld::nextStep(void)     //根据规则计算下一时刻细胞矩阵
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


int CWorld::getCellAlive(int x, int y)     //获取细胞存活状态
{
    if (locValid(x, y) == 0)
    {
        return -1;
    }
    SCell* cell = getCell(current_map, x, y);
    return cell->Alive;
}


void CWorld::swapMap(void) //交换地图
{ 
	SCell* temp = current_map; 
	current_map = new_map; 
	new_map = temp; 
}    


SCell* CWorld::getCell(SCell* buf, int x, int y) //从地图中获取某坐标的细胞指针
{ 
	return buf + y * WIDTH + x; 
}


CWorld::CWorld(int Width, int Height)
{
    width = Width;
    height = Height;
    current_map =(SCell *) new SCell[width * height];
    new_map = (SCell *) new SCell[width * height];
    killAll();
}


CWorld::~CWorld()
{
    delete[] current_map;
    delete[] new_map;
}


/**********************************************************************/
/**********************************************************************/
/****************************界面**************************************/
/**********************************************************************/
//定时器超时回调函数
void CALLBACK WorldTimerCallBack(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
//描画函数
void CleanWorld(HDC hdc);
void DrawWorld(CWorld* world, int world_w, int world_h, HDC hdc);
void DrawCell(CWorld* world, HDC hdc);
void DrawGrid(HDC hdc, int w, int h);

void CreateButton(HWND hwnd, HINSTANCE hInstance);

/* 全局变量定义区 */
char* g_szApplicationName = "LifeGame";
char* g_szWindowClassName = "This Is My Window Class";

/* 消息回调函数 */
LRESULT CALLBACK WindowProc(HWND   hwnd,
    UINT   msg,
    WPARAM wParam,
    LPARAM lParam)
{
    //存储用户窗口的宽和高
    static int cxClient, cyClient;
    //界面字体宽高
    static int cxChar, cyChar;

    //用于创建后备缓冲
    static HDC      hdcBackBuffer;
    static HBITMAP  hBitmap;
    static HBITMAP  hOldBitmap;

    switch (msg)
    {
    case WM_CREATE:
        {
        RECT rect;

        GetClientRect(hwnd, &rect);

        cxClient = rect.right;
        cyClient = rect.bottom;

        //将窗口移动到屏幕中央
        int scrWidth, scrHeight;
        scrWidth = GetSystemMetrics(SM_CXSCREEN);
        scrHeight = GetSystemMetrics(SM_CYSCREEN);
        GetWindowRect(hwnd, &rect);
        MoveWindow(hwnd, (scrWidth - rect.right) / 2, (scrHeight - rect.bottom) / 2, rect.right - rect.left, rect.bottom - rect.top, FALSE);

        //创建世界和生物
        g_world = new CWorld(WORLD_WIDTH, WORLD_HEIGHT);
        DrawWorld(g_world, WORLD_WIDTH, WORLD_HEIGHT, GetDC(hwnd));
        CreateButton(hwnd, ((LPCREATESTRUCT)lParam)->hInstance);
        EnableWindow(GetDlgItem(hwnd, START_BTN_ID), TRUE); //启用开始按钮
        EnableWindow(GetDlgItem(hwnd, PAUSE_BTN_ID), FALSE);    //禁用暂停按钮
        EnableWindow(GetDlgItem(hwnd, NEXT_BTN_ID), TRUE);  //启用下一步按钮
        EnableWindow(GetDlgItem(hwnd, KILL_ALL_BTN_ID), TRUE);  //启用杀死所有按钮

        //后备缓冲区相关处理
        hdcBackBuffer = CreateCompatibleDC(NULL);
        HDC hdc = GetDC(hwnd);
        hBitmap = CreateCompatibleBitmap(hdc, cxClient, cyClient);
        hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);
        //销毁处理
        ReleaseDC(hwnd, hdc);

        }
        break;
    case WM_COMMAND:    //按钮被按下后的响应
    {
        int button_id = LOWORD(wParam);
        switch (button_id)
        {
        case RANDOM_BTN_ID: //随机初始化按钮按下
            g_world->InitMap();
            DrawWorld(g_world, WORLD_WIDTH, WORLD_HEIGHT, GetDC(hwnd));
            break;
        case START_BTN_ID:  //开始按钮按下
            SetTimer(hwnd, WORLD_TIMER_ID, WORLD_TIMER_ELAPSE, WorldTimerCallBack); //启动计时器
            EnableWindow(GetDlgItem(hwnd, RANDOM_BTN_ID), FALSE);   //禁用随机生成按钮
            EnableWindow(GetDlgItem(hwnd, START_BTN_ID), FALSE);    //禁用开始按钮
            EnableWindow(GetDlgItem(hwnd, PAUSE_BTN_ID), TRUE); //启用暂停按钮
            EnableWindow(GetDlgItem(hwnd, NEXT_BTN_ID), FALSE); //禁用下一步按钮
            EnableWindow(GetDlgItem(hwnd, KILL_ALL_BTN_ID), FALSE); //禁用杀死所有按钮
            break;
        case PAUSE_BTN_ID:  //暂停按钮按下
            KillTimer(hwnd, WORLD_TIMER_ID);    //销毁计时器
            EnableWindow(GetDlgItem(hwnd, RANDOM_BTN_ID), TRUE);    //启用随机生成按钮
            EnableWindow(GetDlgItem(hwnd, START_BTN_ID), TRUE); //启用开始按钮
            EnableWindow(GetDlgItem(hwnd, PAUSE_BTN_ID), FALSE);    //禁用暂停按钮
            EnableWindow(GetDlgItem(hwnd, NEXT_BTN_ID), TRUE);  //启用下一步按钮
            EnableWindow(GetDlgItem(hwnd, KILL_ALL_BTN_ID), TRUE);  //启用杀死所有按钮
            break;
        case NEXT_BTN_ID:   //下一步按钮按下
            g_world->nextStep();
            DrawWorld(g_world, WORLD_WIDTH, WORLD_HEIGHT, GetDC(hwnd));
            break;
        case KILL_ALL_BTN_ID:   //杀死所有细胞按钮按下
            g_world->killAll();
            DrawWorld(g_world, WORLD_WIDTH, WORLD_HEIGHT, GetDC(hwnd));
            break;
        default:
            break;
        }
    }
        break;
    case WM_KEYUP:
        //按下Esc退出
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
        break;
    case WM_PAINT:
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        //将后备缓冲区涂上白色背景
        BitBlt(hdcBackBuffer,
                0,
                0,
                cxClient,
                cyClient,
                NULL,
                NULL,
                NULL,
                BLACKNESS);

        //描画世界
        DrawGrid(hdcBackBuffer, WORLD_WIDTH, WORLD_HEIGHT);
        DrawCell(g_world, hdcBackBuffer);
        BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY);
        EndPaint(hwnd, &ps);
        break;
    case WM_SIZE:                                                  //（不要）
    {
        //变更窗口大小时的处理
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);

        SelectObject(hdcBackBuffer, hOldBitmap);
        DeleteObject(hBitmap);
        HDC hdc = GetDC(hwnd);
        hBitmap = CreateCompatibleBitmap(hdc,
            cxClient,
            cyClient);

        ReleaseDC(hwnd, hdc);
        SelectObject(hdcBackBuffer, hBitmap);
    }
    break;
    case WM_DESTROY:
        //销毁世界
        delete g_world;

        //清除并销毁后备缓冲区
        SelectObject(hdcBackBuffer, hOldBitmap);
        DeleteDC(hdcBackBuffer);
        DeleteObject(hBitmap);

        //终了程序,发送WM_QUIT消息  
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     szCmdLine,
    int       iCmdShow)
{
    HWND hWnd;  //窗口句柄
    WNDCLASSEX winclass;    //创建窗口类对象

                            //窗口类对象的初始化
  //WNDCLASSEX是window的数据类
    winclass.cbSize = sizeof(WNDCLASSEX);  //WNDCLASSEX的大小，可以用sizeof(WNDCLASSEX)来获得准确的值
    winclass.style = CS_HREDRAW | CS_VREDRAW; //从这个窗口类派生的窗口具有的风格。您可以用“or”操作符来把几个风格或到一起
    
	winclass.lpfnWndProc = WindowProc;//窗口处理函数的指针（windowproc是自己写的函数，当用户点击按钮时，win会调用该函数）
    
	winclass.cbClsExtra = 0;//指定紧跟在窗口类结构后的附加字节数
    winclass.cbWndExtra = 0;//指定紧跟在窗口实例的附加字节数。如果一个应用程序在资源中用CLASS伪指令注册一个对话框类时，则必须把这个成员设成DLGWINDOWEXTRA
    
	winclass.hInstance = hInstance;//本模块的实例句柄（winmain实参的第一个参数）
    
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//图标的句柄
    
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);//鼠标的句柄，这里是win提供的默认标准箭头
    
	winclass.hbrBackground = NULL;//背景画刷的句柄，即设置窗口的背景颜色
    
	winclass.lpszMenuName = NULL;//指向菜单的指针，用于加载和选用窗口
    
	winclass.lpszClassName = g_szWindowClassName;//为你的WNDCLASSEX类取一个别名
    winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);//和窗口类关联的小图标。如果该值为NULL。则把hIcon中的图标转换成大小合适的小图标。

    //注册窗口类，把新建的窗口类注册上去
    if (!RegisterClassEx(&winclass))//此处为出现ERROR的消息弹框
    {
        MessageBox(NULL, "Registration Failed!", "Error", 0);
        return 0;
    }

    //创建窗口  
    hWnd = CreateWindowEx(NULL,                 // extended style，窗口的扩展风格
        g_szWindowClassName,  // window class name，指向注册类名的指针
        g_szApplicationName,  // window caption，指向窗口名称的指针
        WS_OVERLAPPEDWINDOW,  // window style，窗口风格
        0,                    // initial x position，窗口的水平位置
        0,                    // initial y position，窗口的垂直位置
        WINDOW_WIDTH,         // initial x size
        WINDOW_HEIGHT,        // initial y size
        NULL,                 // parent window handle，父窗口的句柄
        NULL,                 // window menu handle，菜单的句柄或是子窗口的标识符
        hInstance,            // program instance handle，应用程序的实例句柄
        NULL);                // creation parameters，指向窗口的创建数据

                              //容错处理
    if (!hWnd)
    {
        MessageBox(NULL, "CreateWindowEx Failed!", "Error!", 0);
        return 0;
    }

    //显示窗口
    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    MSG msg;

	//不断从事件队列里取到消息(默认)
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);//将虚拟消息转化为字符消息
        DispatchMessage(&msg);//可调用回调函数，即windowproc()处理
    }

    return msg.wParam;
}

//超时后回调函数
void CALLBACK WorldTimerCallBack(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
    g_world->nextStep();
    DrawWorld(g_world, WORLD_WIDTH, WORLD_HEIGHT, GetDC(hwnd));
}

//描画整个世界
void DrawWorld(CWorld * world, int world_w, int world_h, HDC hdc)
{
    CleanWorld(hdc);
    DrawGrid(hdc, world_w, world_h);
    DrawCell(world, hdc);
}

//将世界涂成黑色(背景色)
void CleanWorld(HDC hdc)
{
    HPEN WhitePen = CreatePen(PS_SOLID, 1, RGB(0,0,0));//建立画笔
    HBRUSH WhiteBrush = CreateSolidBrush(RGB(0,0,0));//建立阴影画刷
    SelectObject(hdc, WhitePen);//选用GDI对象
    SelectObject(hdc, WhiteBrush);

	//画矩形
    Rectangle(hdc, 0, 0, WORLD_WIDTH * CELL_SIZE, WORLD_HEIGHT * CELL_SIZE);
    DeleteObject(WhitePen);//删除GDI对象
    DeleteObject(WhiteBrush);
}

//描画所有细胞
void DrawCell(CWorld* world, HDC hdc)
{
    HPEN BluePen = CreatePen(PS_SOLID, 1, RGB(255,250,240));
    HBRUSH BlueBrush = CreateSolidBrush(RGB(255,250,240));
    SelectObject(hdc, BluePen);
    SelectObject(hdc, BlueBrush);
    for (int i = 0; i < world->getWidth(); ++i)
    {
        for (int j = 0; j < world->getHeight(); ++j)
        {
            if (world->getCellAlive(i, j) == 1)
            {
                Rectangle(hdc, i * CELL_SIZE, j * CELL_SIZE, i * CELL_SIZE + CELL_SIZE, j * CELL_SIZE + CELL_SIZE);
            }
        }
    }

    DeleteObject(BluePen);
    DeleteObject(BlueBrush);
}

//描画网格
void DrawGrid(HDC hdc, int w, int h)
{
    HPEN GrayPen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
    SelectObject(hdc, GrayPen);
    for (int i = 0; i <= w; ++i)
    {
        MoveToEx(hdc, i * CELL_SIZE, 0, NULL);//移动目标
        LineTo(hdc, i * CELL_SIZE, h * CELL_SIZE);//画线
    }
    for (i = 0; i <= h; ++i)
    {
        MoveToEx(hdc, 0, i * CELL_SIZE, NULL);
        LineTo(hdc, w * CELL_SIZE, i * CELL_SIZE);
    }

    DeleteObject(GrayPen);
}

void CreateButton(HWND hwnd, HINSTANCE hInstance)
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    int cxClient = rect.right;//整个窗口最右边的x的值

    /* 建立按钮 */
    int cxChar = LOWORD(GetDialogBaseUnits());//返回值为水平对话框的基本单位
    int cyChar = HIWORD(GetDialogBaseUnits());
    //开始按钮
    int button_w = cxChar * 12;//按钮的长
    int button_h = cyChar * 2;//按钮的宽
    int button_y = 400+ cyChar * 3;//按钮位置的x值
    int random_btn_x = cxChar * 3;//按钮位置的y值
    int start_btn_x = random_btn_x + button_w + cxChar*5;
    int pause_btn_x = start_btn_x + button_w + cxChar*5;
    int next_btn_x = pause_btn_x + button_w+ cxChar*5;
    int kill_all_btn_x = next_btn_x + button_w+ cxChar*5;

    CreateWindow(TEXT("Button"), TEXT("随机初始化"),//按钮控件的类名
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,/*扁平样式*/
        random_btn_x,button_y , button_w, button_h,// /*X坐标*/, /*Y坐标*/, /*宽度*/, /*高度*/,
        hwnd, (HMENU)RANDOM_BTN_ID,//后者是控件唯一标识符
        hInstance, NULL);
    CreateWindow(TEXT("Button"), TEXT("开始"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        start_btn_x,button_y , button_w, button_h,
        hwnd, (HMENU)START_BTN_ID,
        hInstance, NULL);
    CreateWindow(TEXT("Button"), TEXT("暂停"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        pause_btn_x,button_y , button_w, button_h,
        hwnd, (HMENU)PAUSE_BTN_ID,
        hInstance, NULL);
    CreateWindow(TEXT("Button"), TEXT("下一步"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        next_btn_x, button_y, button_w, button_h,
        hwnd, (HMENU)NEXT_BTN_ID,
        hInstance, NULL);
    CreateWindow(TEXT("Button"), TEXT("清屏"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        kill_all_btn_x, button_y, button_w, button_h,
        hwnd, (HMENU)KILL_ALL_BTN_ID,
        hInstance, NULL);
}

