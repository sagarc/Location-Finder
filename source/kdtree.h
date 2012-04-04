#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <map>
#include <list>
#include "region.h"
using namespace std;

//class for KDtree construction
class KDTNode 
{
     private:
		 Point* point;            
		 int depth;
     public:
		 KDTNode(){;}
		 KDTNode(Point& p, int d)
		 {
			point = &p;
			depth = d;
			Left = NULL;
			Right = NULL;
		 }
		 KDTNode* Left;
		 KDTNode* Right;
		 Point* GetPoint(){return point;}
		 int GetDepth(){return depth;}
		 bool IsLeaf(){return (Left == NULL) && (Right == NULL);}
};

KDTNode* BuildKDTHelper(vector<Point*> PListX, vector<Point*> PListY, int depth)
{
     KDTNode* knode;
     if (PListX.size() == 0) return NULL;
     if (PListX.size() == 1)
     {
		knode = new KDTNode(*(PListX[0]),depth);
		knode->Left = NULL;
		knode->Right = NULL;
		return knode;
     }
     vector<Point*> NX1, NX2, NY1, NY2;
     Point* split;
     if (depth % 2 == 0)
     {
	   if (PListX.size() == 2)
	   {
		  NX1.push_back(PListX[0]);
		  NY1.push_back(PListX[0]);
		  NX2.push_back(PListX[1]);
		  NY2.push_back(PListX[1]);
		  split = PListX[0];
	   }
	   else
	   {
		   split = PListX[(PListX.size()-1)/2];
		   for (vector<Point*>::iterator it = PListX.begin(); it <= PListX.begin()+(PListX.size()-1)/2;it++)
		   {
			   NX1.push_back(*it);
		   }
		   for (vector<Point*>::iterator it = PListX.begin()+(PListX.size()-1)/2 +1; it < PListX.end();it++)
		   {
			   NX2.push_back(*it);
		   }
		   PListX.clear();
		   PListX.resize(0);

		   for (vector<Point*>::iterator it = PListY.begin(); it < PListY.end();it++)
		   {
			   if ((*it)->GetX() <= split->GetX())
				  NY1.push_back(*it);
			   else
				   NY2.push_back(*it);
		   } 
		   PListY.clear();
		   PListY.resize(0);

	   }
	   knode = new KDTNode(*split,depth);
     }
     else
     {
	   if (PListY.size() == 2)
	   {
		  NX1.push_back(PListY[0]);
		  NY1.push_back(PListY[0]);
		  NX2.push_back(PListY[1]);
		  NY2.push_back(PListY[1]);
		  split = PListY[0];
	   }
	   else
	   {
		   split = PListY[(PListY.size()-1)/2];
		   for (vector<Point*>::iterator it = PListY.begin(); it <= PListY.begin()+(PListY.size()-1)/2;it++)
		   {
			   NY1.push_back(*it);
		   }
		   for (vector<Point*>::iterator it = PListY.begin()+(PListY.size()-1)/2 +1; it < PListY.end();it++)
		   {
			   NY2.push_back(*it);
		   }
		   PListY.clear();
		   PListY.resize(0);

		   for (vector<Point*>::iterator it = PListX.begin(); it < PListX.end();it++)
		   {
			   if ((*it)->GetY() <= split->GetY())
				  NX1.push_back(*it);
			   else
				   NX2.push_back(*it);
		   }
		   PListX.clear();
		   PListX.resize(0);

	   }   
	   knode = new KDTNode(*split,depth);
     }
     knode->Left = BuildKDTHelper(NX1,NY1,depth+1);
     knode->Right = BuildKDTHelper(NX2,NY2,depth+1);
     return knode;
}

bool CompareX(Point* a, Point* b) {return (a->GetX() < b->GetX());}
bool CompareY(Point* a, Point* b) {return (a->GetY() < b->GetY());}

//Given vector of points builds KDTree out of it
KDTNode* BuildKDT(vector<Point*> PList, int depth = 0)
{
	 vector<Point*> PX, PY;
	 for (vector<Point*>::iterator it = PList.begin(); it < PList.end();it++)
	 {
		 PX.push_back(*it);
		 PY.push_back(*it);
	 }
	 sort(PX.begin(),PX.end(),CompareX);
	 sort(PY.begin(),PY.end(),CompareY);
	 return BuildKDTHelper(PX,PY,depth);
}


// Given point p and KDTree finds nearest point 
Point* NearestNew(Point p, KDTNode* n)
{
   	Point* best;
   	if(n->IsLeaf())
   	{      
		 if(*n->GetPoint() == p)
		 {
			 Point** ptemp = new Point*;
			 *ptemp = new Point(INFINITY,INFINITY);     
			 return *ptemp;
		 }
		 else return (n->GetPoint());
	 }
   	else if(n->GetDepth() % 2 == 0)
   	{
		if(p.GetX()<= n->GetPoint()->GetX())
		{
			best = NearestNew(p,n->Left);
			if((p.Distance(*best)) <= abs(p.GetX()- n->GetPoint()->GetX()) || n->Right==NULL) return best;
			else 
			{
				Point* temp = NearestNew(p, n->Right);
				if(p.Distance(*best) <= p.Distance(*temp)) return best;
				else return temp;
			}
		}
		else
		{
			if(n->Right == NULL) return NearestNew(p,n->Left);
			best = NearestNew(p,n->Right);
			if((p.Distance(*best)) <= abs(p.GetX()- n->GetPoint()->GetX()) || n->Left == NULL) return best;
			else
			{
				Point* temp = NearestNew(p, n->Left);
				if(p.Distance(*best) <= p.Distance(*temp)) return best;
				else return temp;
			}
		}
	} 	
   	else{
		if(p.GetY()<= n->GetPoint()->GetY())
		{
			best = NearestNew(p,n->Left);
			if((p.Distance(*best)) <= abs(p.GetY()- n->GetPoint()->GetY()) || n->Right == NULL) return best;
			else
			{
				Point* temp = NearestNew(p, n->Right);
				if(p.Distance(*best) <= p.Distance(*temp)) return best;
				else return temp;}
			}
		else
		{
			if(n->Right == NULL) return NearestNew(p,n->Left);
			best = NearestNew(p,n->Right);
			if((p.Distance(*best)) <= abs(p.GetY()- n->GetPoint()->GetY()) || n->Left == NULL) return best;
			else
			{
				Point* temp = NearestNew(p, n->Left);
				if(p.Distance(*best) <= p.Distance(*temp)) return best;
				else return temp;
			}
		}
	}
}

Point* Find(Point* p,KDTNode* n)
{
   if(n->IsLeaf())
   {      
		if(*n->GetPoint() == *p)
		{
			return n->GetPoint();
		}
		else return NULL;
	}
   	else if(n->GetDepth() % 2 == 0)
   	{
		if(p->GetX()<= n->GetPoint()->GetX())
		{
			if(n->Left==NULL) return NULL;
			else return Find(p,n->Left);
		}
		else 
		{
			if(n->Right == NULL) return NULL;
			else return Find(p,n->Right);
		}
	}
    else
	{
        	if(p->GetY()<= n->GetPoint()->GetY()){
            if(n->Left==NULL) return NULL;
            else return Find(p,n->Left);}
        	else 
        	{
             	if(n->Right == NULL) return NULL;
             	else return Find(p,n->Right);
			}
	}
}


Divider FindRegion(KDTNode* n, Divider nR,int depth, bool isLeft)
{
      Point LL=nR.GetLL();
      Point UR=nR.GetUR();
      if(depth%2==0)//vertical line
      {
		double x = n->GetPoint()->GetX();
		if(isLeft)//left region
		{                                                         
			  Point modUR(x,UR.GetY());
			  Divider lR(LL,modUR);
			  return lR;
		}
		else //rigth region
		{
				  Point modLL(x,LL.GetY());
				  Divider rR(modLL,UR);
				  return rR;
		}
      }
      else //horizontal line
      {
			double y=n->GetPoint()->GetY(); ///horizontal line
			if(isLeft)
			{                                                         
					  Point modUR(UR.GetX(),y);
					  Divider lR(LL,modUR);
					  return lR;
			}
			else
			{
					  Point modLL(LL.GetX(),y);
					  Divider rR(modLL,UR);
					  return rR;
			}
      }
}
void VisitKDT(KDTNode* n,vector<Point>& ans)
{
	 if(n->IsLeaf())ans.push_back(*(n->GetPoint()));
	 else
	 {
		 VisitKDT(n->Left,ans);
		 VisitKDT(n->Right,ans);
	 }
}
                      
//Given KDtree and region , outputs all points belonging in this region      
void SearchKDT(KDTNode* n, Divider nR, long depth, Region* R, vector<Point>& ans)
{			  
	 if(n->IsLeaf())
	 {					
		Point* temp=n->GetPoint();
		if(R->Intersects(*temp))
		{
			ans.push_back(*temp);
			return;
		}
		else return;
	 }
	 vector<Point> tvector;
	 Divider *lR=new Divider(FindRegion(n,nR,depth,1));
	 //*lR = FindRegion(n,nR,depth,1);//isLeft-to sy whether left or right child
	 
	 int test1=R->IsContained(*lR); 
	 if(test1==2)
	 {                    
		delete lR;
		lR=NULL;
		tvector.clear();
		VisitKDT(n->Left,tvector);
		int tsize=tvector.size();
		for(int i=0;i<tsize;i++)
		{
			  ans.push_back(tvector[i]);
		} 
		tvector.clear();                                  
	 }
	 else if(test1==1)
	 {
		SearchKDT(n->Left,*lR,depth+1,R,ans);
		delete lR;
		lR=NULL;                                        	 
	 }
	 Divider *rR =new Divider (FindRegion(n,nR,depth,0));//isRight-to sy whether left or right child
	 int test2=R->IsContained(*rR);
	 if(test2==2)
	{              
		delete rR;
		rR=NULL; 	 
		tvector.clear();
		VisitKDT(n->Right,tvector);
		int tsize=tvector.size();
		for(int i=0;i<tsize;i++)
		{
			  ans.push_back(tvector[i]);
		}  
		tvector.clear();                                 
	}
	 else if(test2==1)
	 {
		SearchKDT(n->Right,*rR,depth+1,R,ans);
		delete rR;
		rR=NULL;
	 }
}

//pretty printing of Kdtree
void PrintKDT(KDTNode* root, string space = "")
{
 	if (root == NULL) return;
 	if (root->IsLeaf())
 	{
    	cout << space << root->GetPoint()->GetX() << " " << root->GetPoint()->GetY() << endl;
    	return;
 	}
 	else if (root->GetDepth() % 2 == 0)
 	{
      	cout << space << "x=" << root->GetPoint()->GetX() << endl;
      	if (root->Left != NULL) PrintKDT(root->Left,space+"	");
      	if (root->Right != NULL) PrintKDT(root->Right,space+"	");
 	}
 	else
 	{
      	cout << space << "y=" << root->GetPoint()->GetY() << endl;
      	if (root->Left != NULL) PrintKDT(root->Left,space+"	");
      	if (root->Right != NULL) PrintKDT(root->Right,space+"	");
 	}
}
