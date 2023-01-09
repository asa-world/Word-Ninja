/************************************************************************
程序名：单词忍者
版本号：2.0
开发者：Asa
程序功能：1）拥有用户登录系统，可注册、登录账号
		  2）词典功能，可查询单词汉意及例句
		  3）背单词功能，三大模式任你选
		  4）模式一：单词自动闪现，加强单词记忆
		  5）模式二：随机出现汉意，默写单词
		  6）模式三：随机出现单词，默写单词汉意
		  7）安全性高，登录后关闭程序需要重新登录
		  8)精美界面，界面友好，交互性强
		  9）颜表情让你的英语学习之旅不再乏味
		  10)宅男福利，美女真人发声，soul不再孤单
************************************************************************/


/*对库函数的定义*/
#include<stdio.h>
#include<string.h>
#include <graphics.h>	//引用图形库
#include<stdlib.h>	//定义杂项函数及内存分配函数
#include<math.h>
#include<tchar.h>	
#include<conio.h>	//控制输出
#include<io.h>
#include<time.h>
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")// 引用 Windows Multimedia API,用于播放音乐


/*宏定义*/
#define USERNUM 100		//定义可承载用户数
#define NAMELENTH 50	//定义用户名称长度
#define PASSWORDLENTH 10		//定义用户密码长度
#define WORDNUM 200		//定义词库数量
#define WORDLEN 20		//定义用户输入默写单词长度
#define CHINESELEN 50		//定义用户输入汉意长度


int i;		//提前定义循环变量，随用随取，减少代码行数
int peoplenum = 0;	//注册编号，同时也代表注册人数
int ifregister = 0;	//判断用户是否登录
int ifplay = 0;		//判断音乐是否播放
int num;	//用户编号


/*定义界面图片*/
IMAGE welcome;	//欢迎界面
IMAGE welcome_click;	//欢迎界面点击效果
IMAGE firstinterface;	//一级菜单
IMAGE click_register;	//点击登录按钮效果
IMAGE click_inquire;	//点击查单词按钮效果
IMAGE click_recite;	//点击背单词按钮效果
IMAGE click_exit;	//点击退出按钮效果
IMAGE click_play;	//点击播放按钮效果
IMAGE recitemenu;	//背单词界面
IMAGE recite_first;	//选择单词闪现模式
IMAGE recite_second;	//选择记忆单词模式
IMAGE recite_third;	//选择联想汉意模式
IMAGE recite_exit;	//选择返回
IMAGE flashwordmenu;	//单词闪现界面
IMAGE flashwordmenu_click_next;		//单词闪现界面点击下一个效果
IMAGE flashwordmenu_click_detail;	//单词闪现界面点击解析效果
IMAGE flashwordmenu_click_exit;		//单词闪现界面点击返回效果
IMAGE recitemenuhaha;		//单词默写界面
IMAGE recitemenuhaha_exit;	//单词默写界面点击返回效果
IMAGE recitemenuhaha_detail;	//单词默写界面点击解析效果
IMAGE recitemenuhaha_next;		//单词默写界面点击下一个效果
IMAGE thinkchinesemenu;		//汉意联想界面
IMAGE thinkchinesemenu_next;	//汉意联想界面点击下一个效果
IMAGE thinkchinesemenu_exit;	//汉意联想界面点击返回效果
IMAGE thinkchinesemenu_detail;	//汉意联想界面点击解析效果
IMAGE secondmenu;	//二级菜单
IMAGE exitmenu;	//结束界面


/*定义用户信息的结构体*/
struct user
{
	wchar_t name[NAMELENTH];	//用户名称
	wchar_t password[PASSWORDLENTH];		//用户输入的密码
	wchar_t word[WORDLEN];	//用户输入的默写单词
	wchar_t Chinese[CHINESELEN];	//用户输入的汉意
	int serialnum;	//定义用户编号
}user[USERNUM];


/*定义单词信息的结构体*/
struct word
{
	wchar_t spell[20];		//单词拼写
	wchar_t translation[3][100];	//单词汉意
	wchar_t sentence[8][100];		//例句
}word[WORDNUM];


/*函数声明*/
void Welcome(void);		//欢迎界面函数

void SaveFile(void);	//文件保存函数

void Firstmenu(void);	//一级菜单函数

void namedouble(void);		//判断用户名是否被占用函数

void Stu_Logon(void);		//用户登录系统

void research(void);			//查单词函数

void recite(void);		//背单词函数

void reciteword(void);		//默写单词函数

void flashword(void);	//单词闪现函数

void thinkchinese(void);	//汉意联想函数

void clickexit(void);		//退出函数


/*欢迎界面函数*/
void Welcome(void)
{
	MCIERROR mciError; // 存储播放音乐过程中的异常情况代码
	loadimage(&welcome, _T("res\\welcome.png"));	//导入欢迎界面
	putimage(0, 0, &welcome);	//显示欢迎界面	
	/*下面进行鼠标交互*/
	MOUSEMSG msg;	//定义变量，储存鼠标信息
	FlushMouseMsgBuffer();	// 清空鼠标消息缓冲区，避免无效鼠标信息带入到正式判断中
	while (true)	// 主循环，循环监听鼠标信息
	{
		while (MouseHit())	//监听鼠标信息;当有鼠标消息的时候执行,可检测连续的鼠标信息
		{
			msg = GetMouseMsg();	//获取鼠标消息
			if (WM_LBUTTONDOWN == msg.uMsg)		//判断鼠标信息;鼠标左键按下
			{
				if (msg.x > 954 && msg.x < 1125 && msg.y > 566 && msg.y < 611)	//鼠标点击特定区域，即ENTER按钮所在区域
				{
					loadimage(&welcome_click, _T("res\\welcome_click.png"));	//导入点击效果图片
					putimage(0, 0, &welcome_click);		//显示点击效果图片
					Sleep(100);		//延时，降低CPU占用率，并且做到点击效果
					mciError = mciSendString(_T("open \"res\\hello.mp3\" alias hello"), NULL, 0, NULL);		//打开音乐
					mciError = mciSendString(_T("play hello"), NULL, 0, NULL);		//播放音乐
					Firstmenu();		//调用一级菜单函数
					mciError = mciSendString(_T("stop hello"), NULL, 0, NULL);	//暂停音乐
					mciError = mciSendString(_T("close hello"), NULL, 0, NULL);	//关闭音乐
				}
			}
		}
	}
}


/*数据保存函数*/
void save(void)
{
	wchar_t temp1[50];	//储存peoplenum数字
	wchar_t temp2[50];	//储存ifregister数字
	wchar_t temp3[50];	//储存num数字
	wchar_t temp[50];	//储存user.serialnum数字
	int i;
	wchar_t s[50];
	_stprintf_s(temp1,_T("%d"),peoplenum);		//转换peoplenum数字类型
	_stprintf_s(temp2, _T("%d"),ifregister);	//转换ifregister数字类型
	_stprintf_s(temp3, _T("%d"), num);			//转换num数字类型
	WritePrivateProfileString(_T("data"),_T("peoplenum"),temp1, _T(".\\savedata.ini"));
	WritePrivateProfileString(_T("data"), _T("ifregister"), temp2, _T(".\\savedata.ini"));
	WritePrivateProfileString(_T("data"), _T("num"), temp3, _T(".\\savedata.ini"));
	for (i = 0; i < peoplenum; i++)		//循环写入用户数据
	{
		_stprintf_s(temp, _T("%d"), user[i].serialnum);		//转换user.serialnum数字类型
		_stprintf_s(s, _T("user %d"), i);	
		WritePrivateProfileString(s, _T("name"),user[i].name, _T(".\\savedata.ini"));
		WritePrivateProfileString(s, _T("password"),user[i].password, _T(".\\savedata.ini"));
		WritePrivateProfileString(s, _T("serialnum"),temp, _T(".\\savedata.ini"));
	}

}


/*一级菜单函数*/
void Firstmenu(void)
{
	wchar_t s[20];
	loadimage(&firstinterface, _T("res\\firstinterface.png"));	//导入一级菜单界面
	putimage(0, 0, &firstinterface);		//显示一级菜单界面
	/*下面进行鼠标交互*/
	MOUSEMSG msg;		//定义变量，储存鼠标信息
	FlushMouseMsgBuffer();	//清空鼠标缓存，避免无效的鼠标信息带入函数中
	while (true)	//主循环，循环监听鼠标信息
	{
		while (MouseHit())
		{
			msg = GetMouseMsg();	//获取鼠标信息
			if (WM_LBUTTONDOWN == msg.uMsg)		//截取鼠标左键按下信息
			{
				if (msg.x > 1156 && msg.x < 1215 && msg.y>644 && msg.y < 698)	//点击播放音乐按钮
				{
					
					loadimage(&click_play,_T("res\\click_play.png"));
					putimage(0,0,&click_play);
					Sleep(100);
					loadimage(&firstinterface, _T("res\\firstinterface.png"));	//导入一级菜单界面
					putimage(0, 0, &firstinterface);		//显示一级菜单界面
					if (ifplay == 0)
					{
						mciSendString(_T("open \"res\\Valentin-A Little Story.mp3\" alias bgm"), NULL, 0, NULL);		//打开音乐
						mciSendString(_T("play bgm repeat"), NULL, 0, NULL);		//播放音乐
						ifplay++;
					}
					else
					{
						mciSendString(_T("stop bgm"), NULL, 0, NULL);	//暂停音乐
						mciSendString(_T("close bgm"), NULL, 0, NULL);	//关闭音乐
						ifplay--;
					}
					
				}
				if (msg.x > 46 && msg.x < 248 && msg.y>122 && msg.y < 181)		//点击登录按钮
					if (ifregister == 1)	//验证重复登录
					{
						HWND hwnd;		//定义消息框
						hwnd = GetHWnd();		//获得窗口句柄，在Windows下，句柄是一个窗口的标识，得到句柄后，可以实现对窗口的控制。
						MessageBox(hwnd, _T("您已经登录了哦\n请不要重复登录"), _T("(＞﹏＜)"), MB_OK | MB_ICONSTOP);
						Firstmenu();
					}
					else		
						Stu_Logon();	//调用登录函数
				if (msg.x > 38 && msg.x < 233 && msg.y>267 && msg.y < 327)	//点击查单词按钮
				{
					loadimage(&click_inquire,_T("res\\click_inquire.png"));	//导入点击查单词效果图片
					putimage(0,0,&click_inquire);	//显示点击查单词效果图片
					Sleep(100);		////延时，降低CPU占用率，并且做到点击效果
					research();//调用单词查询函数
				}
				if (msg.x > 46 && msg.x < 248 && msg.y>396 && msg.y < 455)	//点击单词斩杀按钮
				{
					if (ifregister == 0)	//验证是否登录
					{
						HWND hwnd;		//定义消息框
						hwnd = GetHWnd();		//获得窗口句柄，在Windows下，句柄是一个窗口的标识，得到句柄后，可以实现对窗口的控制。
						MessageBox(hwnd, _T("您还未登录哦"), _T("(＞﹏＜)"), MB_OK | MB_ICONSTOP);
						void Firstmenu();
					}
					else
					{
						loadimage(&click_recite, _T("res\\click_recite.png"));	//导入背单词点击效果图片
						putimage(0, 0, &click_recite);	//显示背单词点击效果图片
						Sleep(100);		////延时，降低CPU占用率，并且做到点击效果
						recite();	//调用背单词函数
					}
				}
				if (msg.x > 46 && msg.x < 248 && msg.y>545 && msg.y < 605)	//点击退出按钮
				{
					mciSendString(_T("open \"res\\see you.mp3\" alias a"), NULL, 0, NULL);		//打开音乐
					mciSendString(_T("play a"), NULL, 0, NULL);		//播放音乐
					loadimage(&click_exit, _T("res\\click_exit.png"));	//导入点击退出效果图片
					putimage(0, 0, &click_exit);		//显示点击退出效果图片
					Sleep(100);		//延时，降低CPU占用率，同时做到点击效果
					mciSendString(_T("stop bgm"), NULL, 0, NULL);	//暂停音乐
					mciSendString(_T("close bgm"), NULL, 0, NULL);	//关闭音乐
					clickexit();	//调用退出函数
				}
			}
		}
	}

}


/*登录函数*/
void Stu_Logon(void)
{
	HWND hwnd;		//定义消息框
	loadimage(&click_register, _T("res\\click_register.png"));	//导入登录点击效果图片
	putimage(0, 0, &click_register);		//显示登录点击效果图片
	hwnd = GetHWnd();		//获得窗口句柄，在Windows下，句柄是一个窗口的标识，得到句柄后，可以实现对窗口的控制。
	if (MessageBox(hwnd, _T("亲，您有账号吗？"), _T(" ◔ ‸◔？"), MB_YESNO) == IDNO)		//如果没有注册
	{
		if (MessageBox(hwnd, _T("那您想和我一起踏上苦涩的英语学习之旅吗？"), _T("｡:.ﾟヽ(｡◕‿◕｡)ﾉﾟ.:｡+ﾟ"), MB_YESNO) == IDYES)	//询问用户注册意愿，如果愿意注册
		{
			InputBox(user[peoplenum].name, 50, _T("不愧是你\n给自己起一个名字吧"), _T("╰(●’◡’●)╮"), _T("张树茂NB"),0, 0, true);		//起名
			namedouble();	//判断用户名是否被占用
			InputBox(user[peoplenum].password, 50, _T("请设置密码"), _T("<(￣︶￣)>"),_T("张树茂NB"),0, 0, true);
			MessageBox(hwnd, _T("恭喜\n注册成功啦"), _T("（＾∀＾●）"), MB_OK);
			peoplenum++;
			user[peoplenum - 1].serialnum = peoplenum;
			save();
			Firstmenu();
		}
		else Firstmenu();		//如果不愿注册，返回一级菜单
	}
	else
	{
		wchar_t username0[NAMELENTH];	//储存一个临时用户名
		wchar_t userpassword0[PASSWORDLENTH];	//储存一个临时密码
		/*下面填入用户名*/
		do {
			if (InputBox(username0, sizeof(username0), _T("请输入您的用户名"), _T("(●’◡’●)ﾉ"), _T("张树茂NB"), 0, 0, false) == true)
			{
				for (i = 0; i < peoplenum; i++)
				if (wcscmp(username0, user[i].name) == 0)	//找到该用户
				{
					num = i;
					break;		//跳出for循环
				}
				if(i>=peoplenum)
					MessageBox(hwnd, _T("出错啦\n未找到您的信息"), _T("(＞﹏＜)"), MB_OK);
				else break;
			}
			else Firstmenu();
		
		} while (1);
		/*下面填入密码*/
		do {
			if (InputBox(userpassword0, sizeof(userpassword0), _T("请输入您的密码"), _T("<(￣︶￣)>"), _T("张树茂NB"), 0, 0, false) == true)
			{
				if (wcscmp(userpassword0, user[num].password) == 0)//密码符合 
				{
				MessageBox(hwnd, _T("等您很久了，欢迎回来"), _T("｡:.ﾟヽ(｡◕‿◕｡)ﾉﾟ.:｡+ﾟ"), MB_OK);
				ifregister = 1;//标志着已经成功登录 
				break;
				}//结束密码输入模块 
			else
				MessageBox(hwnd, _T("密码错误"), _T("(＞﹏＜)"), MB_ICONSTOP | MB_SETFOREGROUND);
			}
			else Firstmenu();

		} while (1);
	}
	Firstmenu();
}


/*判断用户名是否被占用函数*/
void namedouble(void)
{
	int i;
	for (i = 0; i < peoplenum; i++)
	{
		if (wcscmp(user[i].name, user[peoplenum].name) == 0)
		{
			if(InputBox(user[peoplenum].name, sizeof(user[peoplenum].name), _T("名字被占用了，再想一个吧"), _T("哎呀呀"), NULL, 0, 0, false)==true)
			namedouble();	//递归
			else Firstmenu();
		}
	}
}


/*查单词函数*/
void research(void)
{
	int i,j;	//定义循环变量
	wchar_t look[50];		//存储查询单词
	loadimage(&secondmenu,_T("res\\secondmenu.png"));	//导入二级菜单图片
	putimage(0,0,&secondmenu);		//显示二级菜单图片
	if(InputBox(look,sizeof(look),_T("请输入您要查的单词"),_T("亲"), NULL, 0, 0, false)==false)
		Firstmenu();	//停止查时返回一级菜单
	else
	{
		for (i = 0; i <WORDNUM; i++)
		{
			if (wcscmp(word[i].spell, look) == 0)	//逐个比较词库中的词与输入的单词
			{
				/*输出拼写*/
				settextstyle(100, 0, _T("微软雅黑"));	//设置字体高，宽，字体
				setbkmode(TRANSPARENT);		//无背景模式
				settextcolor(RGB(0, 0, 0));		//设置颜色
				RECT A1 = { 100,90,590,236 };	//设定显示区域
				drawtext(word[i].spell, &A1, DT_CENTER);
				/*输出汉意*/
				for (j = 0; j < 3; j++)
				{
				settextstyle(30, 0, _T("微软雅黑"));
				RECT A1 = { 114,270+40*j,561,360+40*j};
				drawtext(word[i].translation[j], &A1, DT_CENTER);
				}
				/*逐个输出例句*/
				for (j = 0; j < 8; j++)		
				{
				settextstyle(25, 0, _T("微软雅黑"));	//设置字体高，宽，字体
				setbkmode(TRANSPARENT);		//无背景模式
				settextcolor(RGB(0, 0, 0));		//设置颜色
				RECT A1 = { 623,108 + 45 * j,1120,200 + 45 * j };	//设定显示区域
				drawtext(word[i].sentence[j],&A1, DT_WORDBREAK);
				}
				/*下面进行鼠标交互*/
				MOUSEMSG msg;	//定义变量，储存鼠标信息
				FlushMouseMsgBuffer();	// 清空鼠标消息缓冲区，避免无效鼠标信息带入到正式判断中
				while (true)	// 主循环，循环监听鼠标信息
				{
					while (MouseHit())	//监听鼠标信息;当有鼠标消息的时候执行,可检测连续的鼠标信息
					{
						msg = GetMouseMsg();	//获取鼠标消息
						if (WM_LBUTTONDOWN == msg.uMsg)		//判断鼠标信息;鼠标左键按下
						{
							if (msg.x > 101 && msg.x < 183 && msg.y > 615 && msg.y < 678)	//点击返回按钮
								Firstmenu();		//调用一级菜单
						}
					}
				}
			}
		}
		if (i >= WORDNUM)		//没找到该单词时
		{
			HWND hwnd;		//定义消息框
			hwnd = GetHWnd();		//获得窗口句柄，在Windows下，句柄是一个窗口的标识，得到句柄后，可以实现对窗口的控制。
			if (MessageBox(hwnd, _T("抱歉未找到该单词\n您想要重新输入吗"), _T("(＞﹏＜)"), MB_YESNO) == IDYES)
				research();
			else Firstmenu();
		}
	}
}


/*背单词界面函数*/
void recite(void)
{
	loadimage(&recitemenu,_T("res\\recitemenu.png"));	//导入背单词界面
	putimage(0,0,&recitemenu);	//显示背单词界面
	/*下面进行鼠标交互*/
	MOUSEMSG msg;		//定义变量，储存鼠标信息
	FlushMouseMsgBuffer();	//清空鼠标缓存，避免无效的鼠标信息带入函数中
	while (true)	//主循环，循环监听鼠标信息
	{
		while (MouseHit())
		{
			msg = GetMouseMsg();	//获取鼠标信息
			if (WM_LBUTTONDOWN == msg.uMsg)		//截取鼠标左键按下信息
			{
				if (msg.x > 389 && msg.x < 492 && msg.y>174 && msg.y < 480)		//点击单词闪现按钮
				{
					loadimage(&recite_first, _T("res\\recite_first.png"));
					putimage(0, 0, &recite_first);
					Sleep(100);
					flashword();
				}
				if (msg.x > 631 && msg.x < 741 && msg.y>174 && msg.y < 480)		//点击单词默写按钮
				{
					loadimage(&recite_second, _T("res\\recite_second.png"));
					putimage(0, 0, &recite_second);
					Sleep(100);
					reciteword();
				}
				if (msg.x > 891 && msg.x < 1001 && msg.y>174 && msg.y < 480)	//点击汉意联想按钮
				{
					loadimage(&recite_third, _T("res\\recite_third.png"));
					putimage(0, 0, &recite_third);
					Sleep(100);
					thinkchinese();
				}
				if (msg.x > 44 && msg.x < 239 && msg.y>549 && msg.y < 606)		//点击返回按钮
				{
					loadimage(&recite_exit, _T("res\\recite_exit.png"));
					putimage(0, 0, &recite_exit);
					Sleep(100);
					Firstmenu();
				}
			}
		}
	}

}


/*单词闪现函数*/
void flashword()
{
	int j;
	int nums;	//储存随机数
	srand((unsigned)time(NULL));
	nums = rand()%(WORDNUM+1);		//随机数赋值
	loadimage(&flashwordmenu,_T("res\\flashwordmenu.png"));
	putimage(0,0,&flashwordmenu);
	settextstyle(90, 0, _T("微软雅黑"));	//设置字体高，宽，字体
	setbkmode(TRANSPARENT);		//无背景模式
	settextcolor(RGB(0, 0, 0));		//设置颜色
	RECT A1 = { 100,90,590,236 };	//设定显示区域
	drawtext(word[nums].spell, &A1, DT_CENTER);
	/*下面进行鼠标交互*/
	MOUSEMSG msg;		//定义变量，储存鼠标信息
	FlushMouseMsgBuffer();	//清空鼠标缓存，避免无效的鼠标信息带入函数中
	while (true)	//主循环，循环监听鼠标信息
	{
		while (MouseHit())
		{
			msg = GetMouseMsg();	//获取鼠标信息
			if (WM_LBUTTONDOWN == msg.uMsg)		//截取鼠标左键按下信息
			{
				if (msg.x > 1020 && msg.x < 1172 && msg.y>613 && msg.y < 660)	//点击下一个按钮
				{
					loadimage(&flashwordmenu_click_next,_T("res\\flashwordmenu_click_next.png"));
					putimage(0,0,&flashwordmenu_click_next);
					Sleep(100);
					flashword();
				}
				if (msg.x > 1020 && msg.x < 1170 && msg.y>539 && msg.y < 586)		//点击解析按钮
				{
					loadimage(&flashwordmenu_click_detail,_T("res\\flashwordmenu_click_detail.png"));
					putimage(0,0,&flashwordmenu_click_detail);
					Sleep(100);
					loadimage(&flashwordmenu, _T("res\\flashwordmenu.png"));
					putimage(0, 0, &flashwordmenu);
					/*输出拼写*/
					settextstyle(90, 0, _T("微软雅黑"));	//设置字体高，宽，字体
					setbkmode(TRANSPARENT);		//无背景模式
					settextcolor(RGB(0, 0, 0));		//设置颜色
					RECT A1 = { 100,90,590,236 };	//设定显示区域
					drawtext(word[nums].spell, &A1, DT_CENTER);
					/*输出汉意*/
					for (j = 0; j < 3; j++)
					{
						settextstyle(30, 0, _T("微软雅黑"));
						RECT A1 = { 114,270 + 40 * j,561,360 + 40 * j };
						drawtext(word[nums].translation[j], &A1, DT_CENTER);
					}
					/*逐个输出例句*/
					for (j = 0; j < 8; j++)
					{
						settextstyle(25, 0, _T("微软雅黑"));	//设置字体高，宽，字体
						setbkmode(TRANSPARENT);		//无背景模式
						settextcolor(RGB(0, 0, 0));		//设置颜色
						RECT A1 = { 623,108 + 45 * j,1120,200 + 45 * j };	//设定显示区域
						drawtext(word[nums].sentence[j], &A1, DT_WORDBREAK);
					}
				}	
				if (msg.x > 33 && msg.x < 113 && msg.y>618 && msg.y < 674)	//点击返回按钮
						recite();
			}
		}
	}

}


/*单词默写函数*/
void reciteword()
{
	wchar_t inputword[50];
	int nums;
	srand((unsigned)time(NULL));
	nums = rand() % (WORDNUM + 1);
	loadimage(&recitemenuhaha,_T("res\\recitemenuhaha.png"));
	putimage(0,0,&recitemenuhaha);
	for (i = 0; i < 3; i++)
	{
		settextstyle(30, 0, _T("微软雅黑"));
		setbkmode(TRANSPARENT);		//无背景模式
		settextcolor(RGB(0, 0, 0));
		RECT A1 = { 400-15*i,300 + 40 * i,880-15*i,400 + 40 * i };
		drawtext(word[nums].translation[i], &A1, DT_CENTER);
	}
	while (1)
	{
		Sleep(1000);
		InputBox(inputword, sizeof(inputword), _T("请输入与汉意相对应的单词"), _T("亲"), NULL, 0, 0, true);
		if (wcscmp(word[nums].spell, inputword) == 0)
		{
			HWND hwnd;		//定义消息框
			hwnd = GetHWnd();		//获得窗口句柄，在Windows下，句柄是一个窗口的标识，得到句柄后，可以实现对窗口的控制。
			MessageBox(hwnd, _T("您太厉害了\n默写对啦"), _T("o(*≧▽≦)ツ"), MB_OK);
			break;
		}
		else
		{
			HWND hwnd;		//定义消息框
			hwnd = GetHWnd();		//获得窗口句柄，在Windows下，句柄是一个窗口的标识，得到句柄后，可以实现对窗口的控制。
			MessageBox(hwnd, _T("oops\n默写的不对哦"), _T("(＞﹏＜)"), MB_OK);
			if (MessageBox(hwnd, _T("您想要再试一次吗"), _T("(๑•̀ㅂ•́)و✧"), MB_YESNO) == IDNO)
			break;
		}
	}
	/*下面进行鼠标交互*/
	MOUSEMSG msg;	//定义变量，储存鼠标信息
	FlushMouseMsgBuffer();	// 清空鼠标消息缓冲区，避免无效鼠标信息带入到正式判断中
	while (true)	// 主循环，循环监听鼠标信息
	{
		while (MouseHit())	//监听鼠标信息;当有鼠标消息的时候执行,可检测连续的鼠标信息
		{
			msg = GetMouseMsg();	//获取鼠标消息
			if (WM_LBUTTONDOWN == msg.uMsg)		//判断鼠标信息;鼠标左键按下
			{
				if (msg.x > 149 && msg.x < 286 && msg.y > 468 && msg.y < 539)	//点击返回按钮
				{	
					loadimage(&recitemenuhaha_exit, _T("res\\recitemenuhaha_exit.png"));
					putimage(0, 0, &recitemenuhaha_exit);
					Sleep(100);
					recite();	
				}
				if (msg.x > 399 && msg.x < 551 && msg.y > 524 && msg.y < 621)	//点击解析按钮
				{
					loadimage(&recitemenuhaha_detail, _T("res\\recitemenuhaha_detail.png"));
					putimage(0, 0, &recitemenuhaha_detail);
					Sleep(100);
					loadimage(&secondmenu, _T("res\\secondmenu.png"));	//导入二级菜单图片
					putimage(0, 0, &secondmenu);		//显示二级菜单图片
					/*输出拼写*/
					settextstyle(100, 0, _T("微软雅黑"));	//设置字体高，宽，字体
					setbkmode(TRANSPARENT);		//无背景模式
					settextcolor(RGB(0, 0, 0));		//设置颜色
					RECT A1 = { 100,90,590,236 };	//设定显示区域
					drawtext(word[nums].spell, &A1, DT_CENTER);
					/*输出汉意*/
					for (i = 0; i < 3; i++)
					{
						settextstyle(30, 0, _T("微软雅黑"));
						RECT A1 = { 114,270 + 40 * i,561,360 + 40 * i };
						drawtext(word[nums].translation[i], &A1, DT_CENTER);
					}
					/*逐个输出例句*/
					for (i = 0; i < 8; i++)
					{
						settextstyle(25, 0, _T("微软雅黑"));	//设置字体高，宽，字体
						setbkmode(TRANSPARENT);		//无背景模式
						settextcolor(RGB(0, 0, 0));		//设置颜色
						RECT A1 = { 623,108 + 45 * i,1120,200 + 45 * i };	//设定显示区域
						drawtext(word[nums].sentence[i], &A1, DT_WORDBREAK);
					}
					MOUSEMSG msg1;	//定义变量，储存鼠标信息
					FlushMouseMsgBuffer();	// 清空鼠标消息缓冲区，避免无效鼠标信息带入到正式判断中
					while (true)	// 主循环，循环监听鼠标信息
					{
						while (MouseHit())	//监听鼠标信息;当有鼠标消息的时候执行,可检测连续的鼠标信息
						{
							msg1 = GetMouseMsg();	//获取鼠标消息
							if (WM_LBUTTONDOWN == msg1.uMsg)
							if (msg1.x > 101 && msg1.x < 183 && msg1.y > 615 && msg1.y < 678)	//点击返回按钮
								reciteword();
						}
					}
				}
				if (msg.x > 702 && msg.x < 863 && msg.y > 563 && msg.y < 636)	//点击下一个按钮
				{
					loadimage(&recitemenuhaha_next, _T("res\\recitemenuhaha_next.png"));
					putimage(0, 0, &recitemenuhaha_next);
					Sleep(100);
					reciteword();
				}
			}
		}
	}

}


/*联想汉意函数*/
void thinkchinese()
{
	wchar_t inputchinese[50];
	int nums;
	srand((unsigned)time(NULL));
	nums = rand() % (WORDNUM + 1);
	loadimage(&thinkchinesemenu,_T("res\\thinkchinesemenu.png"));
	putimage(0,0,&thinkchinesemenu);
	/*输出拼写*/
	settextstyle(100, 0, _T("微软雅黑"));	//设置字体高，宽，字体
	setbkmode(TRANSPARENT);		//无背景模式
	settextcolor(RGB(0, 0, 0));		//设置颜色
	RECT A1 = { 363,245,870,400 };	//设定显示区域
	drawtext(word[nums].spell, &A1, DT_CENTER);
	while (1)
	{
		Sleep(1000);
		InputBox(inputchinese, sizeof(inputchinese), _T("请输入单词汉意"), _T("亲"), NULL, 0, 0, true);
		if (wcspbrk(word[nums].translation[0], inputchinese)|| wcspbrk(word[nums].translation[1], inputchinese)||wcspbrk(word[nums].translation[2], inputchinese))
		{
			HWND hwnd;		//定义消息框
			hwnd = GetHWnd();		//获得窗口句柄，在Windows下，句柄是一个窗口的标识，得到句柄后，可以实现对窗口的控制。
			MessageBox(hwnd, _T("您太厉害了\n默写对啦"), _T("o(*≧▽≦)ツ"), MB_OK);
			break;
		}
		else
		{
			HWND hwnd;		//定义消息框
			hwnd = GetHWnd();		//获得窗口句柄，在Windows下，句柄是一个窗口的标识，得到句柄后，可以实现对窗口的控制。
			MessageBox(hwnd, _T("oops\n默写的不对哦"), _T("(＞﹏＜)"), MB_OK);
			if (MessageBox(hwnd, _T("您想要再试一次吗"), _T("(๑•̀ㅂ•́)و✧"), MB_YESNO) == IDNO)
				break;
		}
	}
	/*下面进行鼠标交互*/
	MOUSEMSG msg;	//定义变量，储存鼠标信息
	FlushMouseMsgBuffer();	// 清空鼠标消息缓冲区，避免无效鼠标信息带入到正式判断中
	while (true)	// 主循环，循环监听鼠标信息
	{
		while (MouseHit())	//监听鼠标信息;当有鼠标消息的时候执行,可检测连续的鼠标信息
		{
			msg = GetMouseMsg();	//获取鼠标消息
			if (WM_LBUTTONDOWN == msg.uMsg)		//判断鼠标信息;鼠标左键按下
			{
				if (msg.x > 234 && msg.x < 377 && msg.y >584 && msg.y < 638)	//鼠标点击特定区域
				{
					loadimage(&thinkchinesemenu_exit, _T("res\\thinkchinesemenu_exit.png"));
					putimage(0, 0, &thinkchinesemenu_exit);
					Sleep(100);
					recite();
				}
				if (msg.x > 831 && msg.x < 970 && msg.y > 486 && msg.y <567)	//鼠标点击特定区域
				{
					loadimage(&thinkchinesemenu_detail, _T("res\\thinkchinesemenu_detail.png"));
					putimage(0, 0, &thinkchinesemenu_detail);
					Sleep(100);
					loadimage(&secondmenu, _T("res\\secondmenu.png"));	//导入二级菜单图片
					putimage(0, 0, &secondmenu);		//显示二级菜单图片
					/*输出拼写*/
					settextstyle(100, 0, _T("微软雅黑"));	//设置字体高，宽，字体
					setbkmode(TRANSPARENT);		//无背景模式
					settextcolor(RGB(0, 0, 0));		//设置颜色
					RECT A1 = { 100,90,590,236 };	//设定显示区域
					drawtext(word[nums].spell, &A1, DT_CENTER);
					/*输出汉意*/
					for (i = 0; i < 3; i++)
					{
						settextstyle(30, 0, _T("微软雅黑"));
						RECT A1 = { 114,270 + 40 * i,561,360 + 40 * i };
						drawtext(word[nums].translation[i], &A1, DT_CENTER);
					}
					/*逐个输出例句*/
					for (i = 0; i < 8; i++)
					{
						settextstyle(25, 0, _T("微软雅黑"));	//设置字体高，宽，字体
						setbkmode(TRANSPARENT);		//无背景模式
						settextcolor(RGB(0, 0, 0));		//设置颜色
						RECT A1 = { 623,108 + 45 * i,1120,200 + 45 * i };	//设定显示区域
						drawtext(word[nums].sentence[i], &A1, DT_WORDBREAK);
					}
					MOUSEMSG msg1;	//定义变量，储存鼠标信息
					FlushMouseMsgBuffer();	// 清空鼠标消息缓冲区，避免无效鼠标信息带入到正式判断中
					while (true)	// 主循环，循环监听鼠标信息
					{
						while (MouseHit())	//监听鼠标信息;当有鼠标消息的时候执行,可检测连续的鼠标信息
						{
							msg1 = GetMouseMsg();	//获取鼠标消息
							if (WM_LBUTTONDOWN == msg1.uMsg)
								if (msg1.x > 101 && msg1.x < 183 && msg1.y > 615 && msg1.y < 678)
									thinkchinese();
						}
					}
				}
				if (msg.x > 831 && msg.x < 1011 && msg.y > 574 && msg.y < 625)	//鼠标点击特定区域
				{
					loadimage(&thinkchinesemenu_next, _T("res\\thinkchinesemenu_next.png"));
					putimage(0, 0, &thinkchinesemenu_next);
					Sleep(100);
					thinkchinese();
				}
			}
		}
	}

}


/*退出函数*/
void clickexit(void)
{
	loadimage(&exitmenu, _T("res\\exitmenu.png"));	//导入退出界面
	putimage(0, 0, &exitmenu);	//显示退出界面
	ifregister = 0;
	save();
	settextstyle(40, 0, _T("微软雅黑"));	//设置字体高，宽，字体
	setbkmode(TRANSPARENT);		//无背景模式
	settextcolor(RGB(255, 0, 0));		//设置颜色
	RECT A1 = { 632,327,822,423 };	//设定显示区域
	/*生成动画*/
	Sleep(100);
	drawtext(_T("("), &A1, DT_LEFT);
	Sleep(100);
	drawtext(_T("(o"), &A1, DT_LEFT);
	Sleep(100);
	drawtext(_T("(o´"), &A1, DT_LEFT);
	Sleep(100);
	drawtext(_T("(o´ω"), &A1, DT_LEFT);
	Sleep(100);
	drawtext(_T("(o´ω`"), &A1, DT_LEFT);
	Sleep(100);
	drawtext(_T("(o´ω`o)"), &A1, DT_LEFT);
	Sleep(200);
	drawtext(_T("(o´ω`o)ﾉ"), &A1, DT_LEFT);

}


int main()
{
	wchar_t transform[50];		//定义宽字符型，用于转换
	wchar_t key[20];
	int j;
	/*数据更新*/
	peoplenum = GetPrivateProfileInt(_T("data"),_T("peoplenum"),-1,_T(".\\savedata.ini"));	//更新当前人数
	ifregister = GetPrivateProfileInt(_T("data"), _T("ifregister"), -1, _T(".\\savedata.ini"));	//重新登陆设置
	num = GetPrivateProfileInt(_T("data"), _T("num"), -1, _T(".\\savedata.ini"));
	for (i = 0; i < peoplenum; i++)		//导入用户数据
	{
		_stprintf_s(transform, _T("user %d"), i);		//将int型转换为宽字符型
		 GetPrivateProfileString(transform,_T("name"),_T("error"),user[i].name,sizeof(user[i].name),_T(".\\savedata.ini"));
		 GetPrivateProfileString(transform,_T("password"),_T("error"),user[i].password,sizeof(user[i].password), _T(".\\savedata.ini"));
		user[i].serialnum= GetPrivateProfileInt(transform, _T("serialnum"), -1, _T(".\\savedata.ini"));
	}
	for (i = 0; i <WORDNUM; i++)		//导入词库
	{
		_stprintf_s(transform, _T("%d"), i);		//将int型转换为宽字符型
		GetPrivateProfileString(transform,_T("spell"),_T("error"),word[i].spell,sizeof(word[i].spell),_T(".\\wordworld.ini"));
		GetPrivateProfileString(transform,_T("translation 0"),_T(""),word[i].translation[0],sizeof(word[i].translation[0]),_T(".\\wordworld.ini"));
		GetPrivateProfileString(transform, _T("translation 1"), _T("(๑•̀ㅂ•́)و✧"), word[i].translation[1], sizeof(word[i].translation[1]), _T(".\\wordworld.ini"));
		GetPrivateProfileString(transform, _T("translation 2"), _T("o(^▽^)o"), word[i].translation[2], sizeof(word[i].translation[2]), _T(".\\wordworld.ini"));
		for (j = 0; j < 8; j++)		//逐个输出汉意
		{
			_stprintf_s(transform, _T("%d"), i);		//将int型转换为宽字符型
			_stprintf_s(key, _T("sentence %d"), j);		//将键赋值
			GetPrivateProfileString(transform, key, _T("error"),word[i].sentence[j], sizeof(word[i].sentence[j]), _T(".\\wordworld.ini"));
		}
	}
	initgraph(1217, 700);	//定义窗口大小
	Welcome();		//调用欢迎界面函数
	return 0;
}