#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <graphics.h>
#include <errno.h>
int num=0;
#define LEFT  0x004B
#define RIGHT 0x004D
#define UP    0x0048
#define DOWN  0x0050
#define ESC   0x001B
struct Node
{
	int x;
	int y;
	int num;	         //蛇的节数
	struct Node *pre;
	struct Node *next;
};

struct igrass//定义毒草
{
	int x;
	int y;
	int flag2;
};
struct Food
{
	int x;
	int y;
	int flag;		//flag为1即已经有食物，为0则表示没有食物
};

struct   Mine     //定义地雷
{
	int x;
	int y;
	int flag3;
};

struct Node *head,*rear,*p,*pt,*px;
struct Food food;
struct igrass grass[6];
struct Mine mine[6];
int direction,temp,ken,tim=0;
char per;
void writDoc()
	{
	char *str = "ikeikei";
	//int i=2;
    FILE *fp;
	fp=fopen("D:/TCS/load.txt","w");          //打开名为load的文件，操作方式为写入，将返回值赋给err
    if(fp==NULL){
		printf("Open error");
		fclose(fp);
		return;
	}
	fwrite(str,sizeof(char) * 8,1,fp);
    fwrite(&direction, sizeof(char), 1, fp);
	fwrite(&food, sizeof(food), 1, fp);
	fwrite(&grass, sizeof(struct igrass)*5, 1, fp);
	fwrite(&mine, sizeof(struct Mine)*5, 1, fp);
	while (pt != NULL) {
	fwrite(pt, sizeof(struct Node), 1, fp);
	pt = pt->next;
	}

    fclose(fp);

	}

void readDoc()
{FILE *fp;
   fp=fopen("D:/TCS/load.txt","r");
  head = (struct Node*)malloc(sizeof(struct Node));
  fread(&direction, sizeof(struct Node), 1, fp);
  fread(&food, sizeof(food), 1, fp);
	fread(&grass, sizeof(struct igrass)*5, 1, fp);
	fread(&mine, sizeof(struct Mine)*5, 1, fp);
  //head->pre = NULL;
  //head->next = NULL;
  //	p = head;
   fread(&head, sizeof(struct Node), 1, fp);
//	head->pre = NULL;
//	head->next = NULL;
//	p = head;
	while (!feof(fp))     //判断文件指针是否移到了结尾
	{
		px = (struct Node*)malloc(sizeof(struct Node));
		fread(pt, sizeof(struct Node), 1, fp);
		px->pre = p;
		p->next = pt;
		px->next = NULL;
		p = px;
	}
	p = head;
//	while (pt->next->next != NULL)       //由于函数原因，最后
//		pt = pt->next;                     //链表会多出一截，所
//	pt->next = NULL;                              //以需要去掉尾部多余
//	free(p);
//	p = pt;                                    //的部分
}

void DrawFence()    //绘制游戏场景
{
	setcolor(LIGHTBLUE);
	setfont(35, 0, _T("微软雅黑"));
    outtextxy(300, 250, _T("请选择难度:(A,B,C)"));
	outtextxy(300, 300, _T("A. 简单"));
	outtextxy(300, 330, _T("B. 中等"));
	outtextxy(300, 360, _T("C. 难"));
	per=getch();
	switch(per){
            case 'A': temp=80,ken=2;break;
            case 'B': temp=70,ken=4;break;
			case 'C': temp=50,ken=6;break;}
	cleardevice(); 
	int i;
	rectangle(600,43,780,560);
	setcolor(RED);
	settextstyle(24, 0, _T("宋体"));
	outtextxy(640,65,"贪吃蛇");
	setcolor(LIGHTBLUE);
	settextstyle(16, 0, _T("宋体"));
	outtextxy(620,95,"绿色代表食物");
	outtextxy(620,110,"蓝色代表毒草");
	outtextxy(620,125,"黄色代表地雷");
    outtextxy(620,170,"小蛇撞到自己或");
    outtextxy(620,185,"者围墙则游戏结束");
    outtextxy(620,200,"按U键实现存档功能");
    outtextxy(620,250,"吃到毒草长度少一段");
    outtextxy(620,265,"吃到地雷长度为一半");
	outtextxy(620,280,"难度越大地雷毒草越多");
	setfillcolor(LIGHTBLUE);//画围墙
	for (i=50; i<=591; i+=10)
	{		
		bar(i,40,i+7,47);	//上边
		bar(i, 551, i+7,558);	//下边
	}
	for (i=40; i<=550; i+=10)
	{
		bar(50, i, 57, i+7);		//左边
		bar(591, i, 598, i+
			7);	//右边
	}
}

void GamePlay()
{int i;
	int gameover=0,score=0;
	int direction=RIGHT,c=RIGHT;

	/*初始化蛇*/
	head=(struct Node *)malloc(sizeof(struct Node));
    head->x=100;
    head->y=100;
	head->num=1;
    head->pre=NULL;
	/**/
	p=(struct Node *)malloc(sizeof(struct Node));
	head->next=p;
	p->x=90;
    p->y=100;
	p->num=2;
    p->pre=head;
	p->next=NULL;
    rear=p;

	food.flag=0;
	for(i=0;i<5;i++)
	grass[i].flag2=0;
	for(i=0;i<3;i++)
	mine[i].flag3=0;

	srand((unsigned)time(NULL));	//生成随机数
	
    
   

	while(1)
	{   //setbkcolor(LIGHTBLUE);
        setcolor(GREEN);
		rectangle(food.x, food.y, food.x+9, food.y+9);
		for(i=0;i<ken;i++)
		{	if(grass[i].flag2==0)
		{
		    grass[i].x =10*( rand()%40 + 6);
            grass[i].y =10*( rand()%35 + 6);
			setcolor(BLUE);
			rectangle(grass[i].x, grass[i].y, grass[i].x+9, grass[i].y+9);
			grass[i].flag2=1;
		}}
		for(i=0;i<ken;i++)
	   if(grass[i].x==head->x && grass[i].y==head->y)
	   {
		   setcolor(0);
		   rectangle(grass[i].x, grass[i].y, grass[i].x+9, grass[i].y+9);
		   num--;
		   pt=head;
		   while(pt->next!=NULL)
		   {
			   pt=pt->next;
		   }
		   p=pt->pre;
		   p->next=NULL;
		   pt->pre=NULL;
		   grass[i].flag2=0;
		   score-=10;
		   rear=p;
		   setcolor(0);
		   rectangle(pt->x, pt->y, pt->x+9, pt->y+9);
		   setfillcolor(BLACK);
		   fillcircle(pt->x+5,pt->y+5,6);
       }
	   for(i=0;i<ken;i++)
	   {
	    if(mine[i].flag3==0)
		{
		    mine[i].x =10*( rand()%40 + 6);
            mine[i].y =10*( rand()%35 + 6);
			setcolor(YELLOW);
			rectangle(mine[i].x, mine[i].y, mine[i].x+9, mine[i].y+9);
			mine[i].flag3=1;
		}
	   if(mine[i].x==head->x && mine[i].y==head->y)
	   {
		   setcolor(0);
		   rectangle(mine[i].x, mine[i].y, mine[i].x+9, mine[i].y+9);
		   pt=head;
		   int n;
		   n=num;
		  while(n>=num/2)
		   {
			   pt=pt->next;
			   n--;
		   }
		   p=pt->pre;
		   p->next=NULL;
		   pt->pre=NULL;
		   mine[i].flag3=0;
		   score-=10;
		   rear=p;
		   num=num/2;
		   while(pt!=NULL)
		   {setcolor(0);
		   rectangle(pt->x, pt->y, pt->x+9, pt->y+9);
		   setfillcolor(BLACK);
		   fillcircle(pt->x+5,pt->y+5,6);

			pt=pt->next;}
       }}

	      
	      
		if(food.flag==0)	//地图上没有食物时
		{
			food.x =10*( rand()%40 + 6 );
			food.y =10*( rand()%35 + 6 );

			/*打印食物*/
			
			setcolor(GREEN);
			rectangle(food.x, food.y, food.x+9, food.y+9);
			food.flag=1;
		}
		if(food.x==head->x && food.y==head->y)	//吃到食物后
		{ 
			num++;
			/*把画面上的食物清除*/
			setcolor(0);
			rectangle(food.x, food.y, food.x+9, food.y+9);
			/*开辟新节点*/
			p=(struct Node *)malloc(sizeof(struct Node));
            pt=head;
            while(pt->next!=NULL)
			{
                pt=pt->next;
			}
            p->pre=pt;
            pt->next=p;
            p->next=NULL;
            rear=p;
			food.flag=0;	//此时地图上无食物
			score += 10;
		}
		/*控制方向*/
		if( kbhit() )
		{
			c=getch();
			if(direction!=RIGHT && c==LEFT)
                direction=c;
            else if(direction!=LEFT && c==RIGHT)
                direction=c;
            else if(direction!=UP && c==DOWN)
                direction=c; 
            else if(direction!=DOWN && c==UP)
                direction=c;
			else if(c =='o'|| c =='O')
			{
				outtextxy(620,290,"保存成功！");
				writDoc();
			}
			else if(c==ESC)
				break;
			else if(c=='u'||c=='U')
			{
				readDoc();
			}


		}
		pt=rear;
		/*去除蛇的最后一节*/
		setcolor(0);
		rectangle(pt->x,pt->y,pt->x+9,pt->y+9);
		setfillcolor(BLACK);
		fillcircle(pt->x+5,pt->y+5,6);

        
		while(pt!=head )
        {
            pt->x=pt->pre->x;
            pt->y=pt->pre->y;
            pt=pt->pre;
        }
		tim++;
		if(tim%11==0)
		{
			for(i=0;i<ken;i++)
			{setcolor(BLACK);
			rectangle(grass[i].x, grass[i].y, grass[i].x+9, grass[i].y+9);}
		}
		if(tim%13==0)
		{
			for(i=0;i<ken;i++)
			{setcolor(BLUE);
			rectangle(grass[i].x, grass[i].y, grass[i].x+9, grass[i].y+9);}
		}
		/*实现蛇的移动*/
		if(direction==RIGHT)
        {
            head->x+=10;
            if(head->x>580)
			{gameover=1;
			break;}
        }
        else if(direction==LEFT)
        {
            head->x-=10;
            if(head->x<60)
			{gameover=1;
			break;}
        }
        else if(direction==UP)
        {
            head->y-=10;
            if(head->y<50)
              {gameover=1;
			break;}
        }
        else if(direction==DOWN)
        {
            head->y+=10;
            if(head->y>540)
            	{gameover=1;
			break;}
        }
		/*蛇撞到自己死亡*/
		pt=head->next;
        while(pt!=NULL)
        {
            if(head->x==pt->x && head->y==pt->y)
            {
                gameover=1;
                break;
            }
            pt=pt->next ;
        }
        if(gameover==1)
            break;
		/*画蛇*/
		pt=head;
		while(pt!=NULL)
		{
			if(pt==head)	//画蛇头
			{
               // setcolor(WHITE);
	            //circle(pt->x+5,pt->y+5,5);
				setfillcolor(YELLOW);  
				fillcircle(pt->x+5,pt->y+5,6); 

			}
			else			//画蛇身
			{
				setcolor(RED);
				rectangle(pt->x,pt->y,pt->x+9,pt->y+9);
			}
			pt=pt->next;
		}
		_sleep(temp);             //蛇的节点越多，蛇跑的越快。
	}
	if(gameover==1)
    {

		setcolor(RED);
		setfont(70, 0, _T("微软雅黑"));
		setbkcolor(BLACK);
		outtextxy(150, 230, _T("YOU LOSE!"));
	printf("The score : %d\n",10*num);}

	getch();
}


int main()
{    
	initgraph(800,600);

	FlushBatchDraw();//初始化图形
	DrawFence();
	GamePlay();
//	getch();

	return 0;
}

 
