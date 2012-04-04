#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <map>
#include <list>
#define TOLERANCE 25
#define TOLERANCE2 2

using namespace std;

class Edge;

class Point {
    private:
		double x;
		double y;
		long vel;
		int colour;
		Edge* curredge;
		int lastvertex;
            
    public:
	   Point(){;}
	   Point(double a, double b, long v = 0, int cr = 0)
	   {
			x = a;
			y = b;
			vel = v;
			colour = cr;
			curredge = NULL;
			lastvertex = 0;
	   }
	   double GetX(){return x;}
	   double GetY(){return y;}
	   void SetX(double a){x=a;}
	   void SetY(double b){y=b;}
	   int GetLastVertex(){return lastvertex;}
	   void SetLastVertex(int a){lastvertex = a;}
	   Edge* GetEdge(){return curredge;}
	   long GetV(){return vel;}
	   int GetColour(){return colour;} 
	   void SetColour(int cr){colour = cr;}
	   void SetV(long v){vel = v;}         
	   void SetEdge(Edge* e){curredge = e;}  
	   Point operator=(Point a)
	   {
			 x = a.GetX();
			 y = a.GetY();
			 return a;
	   }
	   double Distance(Point p)
	   {
		   return (sqrt((p.GetX()-x)*(p.GetX()-x)+(p.GetY()-y)*(p.GetY()-y)));
	   }
	   bool operator==(Point p){
		 return ((p.GetX()-x)*(p.GetX()-x)+(p.GetY()-y)*(p.GetY()-y) <=TOLERANCE);}
             
};

void printpoint(Point p)
{
    cout<<p.GetX()<<" "<<p.GetY()<<endl;
}

class Edge
{
   private:
       Point endpt1;
       Point endpt2;
       int colour;
       list<Edge> Neighbours;
   public:
       Edge(){;}
       Edge(Point a, Point b, int cr = 0){
           endpt1 = a;
           endpt2 = b;
           colour = cr;} 
       Point GetPt1(){
           return endpt1;}
       Point GetPt2(){
           return endpt2;}
       int GetColour(){return colour;}
       void SetColour(int cr){colour = cr;}       
       list<Edge> GetNeighbours(){
           return Neighbours;}
       void AddNeighbour(Edge& e){
            Neighbours.push_front(e);}

       bool isInside(Point p)
       {
            double a,b,c,d;
            if((p.GetX()<= endpt1.GetX() && p.GetX()>=endpt2.GetX()) || (p.GetX()>= endpt1.GetX() && p.GetX()<=endpt2.GetX())
            || (p.GetY()<= endpt1.GetY() && p.GetY()>=endpt2.GetY()) || (p.GetY()>= endpt1.GetY() && p.GetY()<=endpt2.GetY()))
            {
                a = (endpt1.GetY()-endpt2.GetY());
                b = (endpt2.GetX()-endpt1.GetX());
                c = (endpt1.GetX()-endpt2.GetX())*endpt1.GetX() + (endpt2.GetY()-endpt1.GetY())*endpt1.GetX();
                d = abs(a*p.GetX() + b*p.GetY() + c) / sqrt(a*a + b*b);
                if (d <= TOLERANCE2) return true;
            }
            return false;
		}                                                       
        bool operator==(Edge e)
        {
            return ((endpt1 == e.endpt1 && endpt2 == e.endpt2)||
                  (endpt1 == e.endpt2 && endpt2 == e.endpt1));
		}
       friend ostream& operator<<(ostream& os,Edge e);                    
};

ostream& operator<<(ostream& os,Edge e)
{
   os<<"("<<e.endpt1.GetX()<<","<<e.endpt1.GetY()<<")"<<"  "<<"("<<e.endpt2.GetX()<<","<<e.endpt2.GetY()<<")"<<endl;
   return os;
}
