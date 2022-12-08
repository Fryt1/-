#include<iostream>
#include<graphics.h>
#include<vector>
#include <conio.h>
using namespace std;

constexpr auto swidth = 960;
constexpr auto sheight = 700;

bool PointInRect(int x, int y, RECT& r)
{
	return(r.left <= x && r.right >= x&& y >= r.top&& y <= r.bottom);
}

bool RectDuangRect(RECT& r1, RECT& r2)
{
	RECT R;
	R.left = r1.left - (r2.right - r2.left);
	R.top = r1.top - (r2.bottom - r2.top);
	R.right = r1.right;
	R.bottom = r1.bottom;
	return(R.left < r2.left&& r2.left <= R.right  &&r2.top>=R.top&& r2.top <= R.bottom);
}
void welcom()
{
	LPCTSTR title = _T("飞机大战");
	LPCTSTR tplay = _T("开始游戏");
	LPCTSTR texit = _T("退出游戏");
	RECT tplayr, texitr;
	BeginBatchDraw();
	setbkcolor(WHITE);
	cleardevice();
	settextstyle(60, 0, _T("黑体"));
	settextcolor(BLACK);
	outtextxy(swidth / 2 - textwidth(title) / 2, sheight / 10, title);
	settextstyle(40, 0, _T("黑体"));

	tplayr.left = swidth / 2 - textwidth(tplay) / 2;
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = sheight / 5*1.5;
	tplayr.bottom = tplayr.top + textheight(tplay);

	texitr.left = swidth / 2 - textwidth(texit) / 2;
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = sheight / 5*2;
	texitr.bottom = texitr.top + textheight(texit);
	
	
	outtextxy(tplayr.left, tplayr.top, tplay);
	outtextxy(texitr.left, texitr.top, texit);
	


	EndBatchDraw();
	
	bool is_live = 1;
	while (is_live)
	{
		
		ExMessage mouse;
		  getmessage(&mouse,EX_MOUSE);
		if (mouse.lbutton)
		{
			if (PointInRect(mouse.x, mouse.y, tplayr))
			{
				return;
			}
			else if (PointInRect(mouse.x, mouse.y, texitr))
			{
				exit(0);
			}

		}



	}

	

	

}
void Over(unsigned long long &kill)
{
	ExMessage enter;
	TCHAR *str = new TCHAR[128];
	_stprintf_s(str, 128,_T("击杀数：%d"),kill);
	settextcolor(RED);
	settextstyle(20,0, _T("黑体"));
	outtextxy(swidth / 2 - textwidth(str) / 2, sheight / 5, str);
	while (1)
	{
		enter = getmessage(EX_KEY);
		if (enter.vkcode == 0x0D) 
		{
			return;

		}

	}


}
class BK
{
public:
	BK(IMAGE &img):img(img),y(-sheight)
	{

	}
	void Show()
	{
		if (y == 0)
		{
			y = -sheight;
		}
		y += 4;
		putimage(0, y, &img);

	}

	

private:
	IMAGE& img;
	int x, y;

};

class Hero
{
public:
	Hero(IMAGE& img) :img(img)
	{
		rect.left = swidth / 2 - img.getwidth() / 2;
		rect.top = sheight - img.getheight();
		rect.right = rect.left + img.getwidth();
		rect.bottom = sheight;

	}
	void Show()
	{
		putimage(rect.left, rect.top, & img);
	}
	void Control()
	{
		ExMessage mess;
		if (peekmessage(&mess, EX_MOUSE))
		{
			rect.left = mess.x - img.getwidth() / 2;
			rect.top = mess.y - img.getheight() / 2;
			rect.right = rect.left + img.getwidth();
			rect.bottom = rect.top + img.getheight();
		}
		
	}
	void slep()
	{
		if (_kbhit())
		{
			char v = _getch();
			if (v == 0x20)
			{
				Sleep(600);
				while ((true))
				{
					if (_kbhit())
					{
						v = _getch();
						if (v == 0x20)
						{
							break;
						}
					}
					Sleep(16);

				}
				
				
			}

		}
	}


	IMAGE& img;
	RECT rect;
	RECT& GetRect(){ return rect; }
};
class Enemy
{
public:
	Enemy(IMAGE& img, int x):img(img)
	{
		rect.left = x;
		rect.top = -img.getheight();
		rect.right = rect.left + img.getwidth();
		rect.bottom = 0;

	}
	bool Show()
	{
		if (rect.top >= sheight)
		{
			return 0;
		}
		rect.top += 4;
		rect.bottom += 4;
		putimage(rect.left, rect.top, &img);
		return 1;
	}
	

		IMAGE& img;
		RECT rect;
		RECT& GetRect() { return rect; }
};
class Bullet{
public:
	Bullet(IMAGE& img,RECT pr) :img(img)
	{
		rect.left = pr.left + (pr.right - pr.left)/2-(img.getwidth()/2);
		rect.right = rect.left + img.getwidth();
		rect.top = pr.top - img.getheight();
		rect.bottom = rect.top + img.getheight();


	}
	bool Show()
	{
		if (rect.bottom <= 0) {
			return false;
		}
		rect.bottom -= 6;
		rect.top -= 6;
		putimage(rect.left, rect.top, &img);
		return true;
		

		
	}
	RECT& GetRect() { return rect; }
	IMAGE& img;
	RECT rect;
	

};
bool AddEnemy(vector<Enemy*> &es,IMAGE &enemyimg)
{
	Enemy* e = new Enemy(enemyimg, abs(rand()) % (swidth - enemyimg.getwidth()));
	
	for (auto& i : es)
	{
		if (RectDuangRect(i->GetRect(), e->GetRect()))
		{
			delete e;
			return false;
		}

	}
	es.push_back(e);
		return true;

}

bool Play()
{
	setbkcolor(WHITE);
	cleardevice();
	bool is_play = 1;
	IMAGE myimg, enemyimg, bkimg, bulletimg;
	loadimage(&myimg, _T("images\\me1.png"));
	loadimage(&enemyimg, _T("images\\enemy1.png"));
	loadimage(&bkimg, _T("images\\bk.png"),960,1400);
	loadimage(&bulletimg, _T("images\\bullet1.png"));
	BK bk = BK(bkimg);
	Hero hp = Hero(myimg);
	vector<Enemy*> es;
	vector<Bullet*>bs;

	int bsing = 0;
	unsigned long long kill = 0;
	for (int i = 0; i < 3; i++)
	{
		AddEnemy(es, enemyimg);
	}
	
	while ((is_play))
	{
		bsing++;
		if (bsing == 5)
		{
			bsing = 0;
			bs.push_back(new Bullet(bulletimg, hp.GetRect()));
		}
		BeginBatchDraw();
		bk.Show();
		Sleep(6);
		flushmessage();
		Sleep(2);
		hp.slep();
		
		hp.Control();
		auto bsit = bs.begin();
		
		
			
			cout << bs.size() << endl;
			 
		
		
		hp.Show();
		for (auto& i : bs)
		{
			i->Show();
		}
		
		auto it = es.begin();
		while (it != es.end())
		{
			if (RectDuangRect((*it)->GetRect(),hp.GetRect())){ is_play = 0; }
			auto bit = bs.begin();
			while (bit != bs.end())
			{
				if (RectDuangRect((*bit)->GetRect(), (*it)->GetRect()))
				{
					delete(*it);
					 es.erase(it);
					 it = es.begin();
					delete(*bit);
					bs.erase(bit);
					kill++;
					break;

				}
				bit++;
				
			}

			if (!(*it)->Show())
			{
				delete(*it);
				es.erase(it);
				it = es.begin();
				
			}
			else
			{
				it++;

			}
			
			
			
			

		}
		
		for (int i = 0; es.size() < 5; i++)
		{
			AddEnemy(es, enemyimg);
		}
		EndBatchDraw();

		

		
		
		
		


		
	}
	Over(kill);
	
	
	return 0;
}


int main()
{
	initgraph(swidth, sheight, EX_SHOWCONSOLE);
	BOOL is_live = 1;
	while (1)
	{
		welcom();


		is_live = Play();

	}
	

	
	

	
	

	return 0;

}