#include<stdio.h>
#include "kdtree.h"
using namespace std;

bool fncomp (Point lhs, Point rhs) 
{if(lhs.GetX()<rhs.GetX())return true;
  else if(lhs.GetX()==rhs.GetX())
  {
       if(lhs.GetY()<rhs.GetY())return true;
       else return false;
  }
  else return false;
}


vector<Rectangle> FindIClusters(Divider Reg, double width, double height)
{
	  vector<Rectangle>ans;
	  Point LL1=Reg.GetLL();
	  Point UR1=Reg.GetUR();
	  double xi=LL1.GetX();
	  double yi=LL1.GetY();               
	  
	  double xll=xi;
	  double yll=yi;
	  double xur=xll+width;
	  double yur=yll+height;
	 
	  double xf=UR1.GetX();
	  double yf=UR1.GetY();
	  while(yur<=yf)
	  {         while(xur<=xf)
				{
						   ans.push_back(Rectangle(Point(xll,yll),Point(xur,yur)));
						   xll+=width;
						   xur+=width;
				}   
				if(xll<xf){ans.push_back(Rectangle(Point(xll,yll),Point(xf,yur)));}
				xll=xi;
				yll+=height;
				xur=xll+width;
				yur=yur+height;
	  }
	  if(yll<yf)
	  {         while(xur<=xf)
				{
						   ans.push_back(Rectangle(Point(xll,yll),Point(xur,yf)));
						   xll+=width;
						   xur+=width;
				}   
				if(xll<xf){ans.push_back(Rectangle(Point(xll,yll),Point(xf,yf)));}
	  }
	  return ans;
}
                            




void FindMean_h(vector<Point> list,vector<Point*> &ans)
{
     int size=list.size();
     double xt=0,yt=0;
     for(int i=0;i<size;i++)
     {
             xt+=list[i].GetX();
             yt+=list[i].GetY();
     }
     if(size!=0)
     {Point** p = new Point*;
     *p=new Point(xt/size,yt/size);
     ans.push_back(*p);
     }
}     

 void FindMean(vector<vector<Point> > ClusteredPoints,vector<Point*> &ans)
{
              
               int msize= ClusteredPoints.size();               
               for(int i=0;i<msize;i++)
               {FindMean_h(ClusteredPoints[i],ans);}
           
}
           

void FindClusters(KDTNode* Root, Divider Reg,vector<Point*>&ans)
{
	  int noofruns=5;
	  int noofcells=5;
	  int finaldensity=1; 
	  Point LL = Reg.GetLL();
	  Point UR = Reg.GetUR();
	  double width = (UR.GetX()-LL.GetX())/noofcells;//to have approx 9 clusters
	  double height = (UR.GetY()-LL.GetY())/noofcells;
	  vector<Rectangle> InitialCluster = FindIClusters(Reg,width,height);
	  int NoClusters=InitialCluster.size();
	  vector<vector<Point> > ClusteredPoints;
	  ClusteredPoints.resize(NoClusters);
	  
	  for(int i=0;i<NoClusters;i++)
	  {
			  Region* R=&InitialCluster[i];
			  SearchKDT(Root, Reg, 0, R, ClusteredPoints[i]);
	   }
	  InitialCluster.clear();
		 
	 for(int i=0;i<noofruns;i++)
	 {
	  vector<Point*>Mean;
	  FindMean(ClusteredPoints,Mean);
	  int Meansize= Mean.size();
		/*for(int i=0;i<Meansize;i++)//to check modification of mean
		{
		cout<<Mean[i]->GetX()<<" "<<Mean[i]->GetY()<<"\t";
		}*/	
	  
	  
					
	   bool(*fn_pt)(Point,Point) = fncomp;
	   map <Point,int,bool(*)(Point,Point)> Meanmap (fn_pt);
	  for(int i=0;i<Meansize;i++)
	  {
		  Meanmap[*Mean[i]]=i;
	  }
	  KDTNode* MeanKDT = BuildKDT(Mean);
	  //PrintKDT(MeanKDT," ");
	  
   
	  vector<Point> TotalList;
	  for(int i=0;i<NoClusters;i++)
	  {
			  int tsize=ClusteredPoints[i].size();
			  for(int j=0;j<tsize;j++)
			  {
					  TotalList.push_back(ClusteredPoints[i][j]);
			  }
	  }
	  ClusteredPoints.clear();//cout<<"test"<<ClusteredPoints.size()<<endl;
	  ClusteredPoints.resize(Meansize);
	 
	  int TotalListsize=TotalList.size();//cout<<"total size"<<TotalListsize<<endl;
	  for(int i=0;i<TotalListsize;i++)
	  {
			  Point* np = NearestNew(TotalList[i], MeanKDT);
			 (ClusteredPoints[Meanmap[*np]]).push_back(TotalList[i]);
	  }
	  TotalList.clear();
	  TotalListsize=0;
  }
  vector<Point*>tans;
  FindMean(ClusteredPoints,tans);
  int tsize=tans.size();
  for(int i=0;i<tsize;i++)
  {
		//Region* R=&(Circle(5.0,*tans[i]));
		Region* R = new Circle(5.0,*tans[i]);
		vector<Point>temp;
		SearchKDT(Root,Reg,0,R,temp);
		if( (int)temp.size() < finaldensity){;}
		else{ans.push_back(tans[i]);}
	}		  
             
}
void printvector(vector<Point> v)
{
	int size=v.size();
	for(int i=0;i<size;i++)
	printf("(%3.1f,%3.1f)  ",v[i].GetX(),v[i].GetY());
}
 
int main()
{
	srand(time(NULL));
	long n;
	Point* p;
	cout << "Enter number of points\n";
	cin >> n;
	cout << "Creating random points\n";
	vector<Point*> PList(0);
	for (long i = 0; i < n; i++)
	{
   	//p = new Point((((double)(rand() % 2000))/10 - 100) ,(((double)(rand() % 2000))/10 - 100));
   	p=new Point((i*5)%97,(i*97)%87);
   	PList.push_back(p);
	}
	KDTNode* root = BuildKDT(PList,0);
	PrintKDT(root);
	cout<<"\nTree is Constructed\n";

	cout << "Enter point to find nearest neighbour" << endl;
	double x,y;
	cout <<"X: ";
	cin >> x;
	cout << "Y: ";
	cin >> y;
	Point* NN = NearestNew(Point(x,y),root);
	printf("Nearest Neighbours Found is: (%3.1f,%3.1f)\n ",NN->GetX(),NN->GetY());
	
	Divider Rroot(Point(-101,-101),Point(101,101));
    Circle CSearch(40.0,Point(40,40));
    Region* Reg= &CSearch;
    vector<Point>thisans;
    SearchKDT(root,Rroot,0,Reg,thisans);
    cout<<"Sample search yields:\n";printvector(thisans);
    
    vector<Point> ans;
    Region* Re;
    cout<<"\n\nEnter Region to be searched\n0:Rectangle \n1:Circle\n";
    int flag;cin>>flag;
    if(flag==0)
    {cout<<"\nEnter Coordiates of LowerLeft And UpperRight points of Rectangle\n";
		double x1,y1,x2,y2;
		cout<<"LowerLeft corner\n";
		cout <<"X: ";
		cin>>x1;
		cout<<"Y: ";
		cin>>y1;
		cout<<"UpperRight corner\n";
		cout <<"X: ";
		cin>>x2;
		cout<<"Y: ";
		cin>>y2;		
		Re = new Rectangle(Point(x1,y1),Point(x2,y2));
	}
	else
	{
		cout<<"\nEnter Cordinates of Center and Radius of Circle\n";
		double x1,y1,r;
		cout<<"X: ";
		cin>>x1;
		cout<<"Y: ";
		cin>>y1;
		cout<<"Radius: ";
		cin>>r;
		Re = new Circle(r,Point(x1,y1));
	}	
    SearchKDT(root,Rroot,0,Re,ans);
    cout<<"\nSearch done and Points found are: \n";
    int size=ans.size();
    cout<<size<<endl;
    for(int i=0;i<size;i++)
    {
            printf("(%3.1f,%3.1f)  ",ans[i].GetX(),ans[i].GetY());
    }
    cout<<"\n";
    
    vector<Point*>MeanMod;
    FindClusters(root,Rroot,MeanMod);
    int Meansize=MeanMod.size();
    printf("\nNo of Clusters Found are: %d\n",Meansize);
    for(int i=0;i<Meansize;i++)
       {
            printf("(%3.1f,%3.1f)  ",MeanMod[i]->GetX(),MeanMod[i]->GetY());
       }
   printf("\n");
    return 0;
}

