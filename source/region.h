#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <map>
#include <list>
#include "prim.h"
using namespace std;

class Divider
{
    private:
		Point LL;
		Point UR;
    public:
		Divider(){;}
		Divider(Point a, Point b)
		{
			LL = a;
			UR = b;
		}
		Divider(const Divider& d)
		{
			LL=d.LL;
			UR=d.UR;
		}
		Point GetLL(){return LL;}
		Point GetUR(){return UR;}  
		bool Intersects(Point p);     
};

bool Divider::Intersects(Point p)
{
    return ((LL.GetX() <= p.GetX()) &&
            (UR.GetX() >= p.GetX()) &&
             (LL.GetY() <= p.GetY()) &&
              (UR.GetY() >= p.GetY()));
}

class Region
{
	public:	
	virtual bool Intersects(Point x) =0;
	virtual int IsContained(Divider r) =0;
};

class Rectangle:public Region
{
    private:
		Point LL;
		Point UR;
    public:
		Rectangle(){;}
		Rectangle(Point a, Point b)
		{
			LL = a;
			UR = b;
		}
		Point GetLL(){return LL;}
		Point GetUR(){return UR;}
		bool Intersects(Point p);
		int IsContained(Divider r);
	   
};
bool Rectangle::Intersects(Point p)
{
    return ((LL.GetX() <= p.GetX()) &&
            (UR.GetX() >= p.GetX()) &&
             (LL.GetY() <= p.GetY()) &&
              (UR.GetY() >= p.GetY()));
}
int Rectangle::IsContained(Divider r)
{
    Point LL=r.GetLL();
    Point UR=r.GetUR();
    Point LR(UR.GetX(),LL.GetY());
    Point UL(LL.GetX(),UR.GetY());
	bool l1 =Intersects(LL);
	bool l2 =Intersects(LR);
	bool l3 =Intersects(UL);
	bool l4 =Intersects(UR);
       
    if(l1&&l2&&l3&&l4)return 2;
    Point ll=this->GetLL();
    Point ur=this->GetUR();
    Point lr(ur.GetX(),ll.GetY());
    Point ul(ll.GetX(),ur.GetY());
    if(l1||l2||l3||l4)return 1;
    if(r.Intersects(ll) || r.Intersects(lr) || r.Intersects(ur) || r.Intersects(ul))return 1;
    
    else return 0;
}


class Circle :public Region
{
    private:
		double radius;
		Point center;
			
    public:
	   Circle(){;}
	   Circle(double x,Point y){radius=x;center=y;}
	   double GetR(){return radius;}
	   Point GetCenter(){return center;}
	   bool Intersects(Point p);
	   int IsContained(Divider r);                          
};

bool Circle:: Intersects(Point p)
{
	double sqdistance = (p.GetX() - center.GetX()) * (p.GetX()-center.GetX()) + (p.GetY()-center.GetY()) * (p.GetY()-center.GetY());
	if(sqdistance<=radius*radius){return true;}
	else return false;
}

int Circle::IsContained(Divider r)
{
	Point LL=r.GetLL();
    Point UR=r.GetUR();
    Point LR(UR.GetX(),LL.GetY());
    Point UL(LL.GetX(),UR.GetY());
	bool l1 =Intersects(LL);
	bool l2 =Intersects(LR);
	bool l3 =Intersects(UL);
	bool l4 =Intersects(UR);
	if(l1&&l2&&l3&&l4)return 2;
	bool l5= ( (radius - abs(r.GetLL().GetX()  - this->GetCenter().GetX() ))>0.0) ;
	bool l6=( abs(LL.GetY() - center.GetY()) < radius)?true:false;
	bool l7=( abs(UR.GetX() - center.GetX()) < radius)?true:false;
	bool l8=( abs(UR.GetY() - center.GetY()) < radius)?true:false;

	if(r.Intersects(center)||l5||l6||l7||l8)return 1;

	else return 0;
}
