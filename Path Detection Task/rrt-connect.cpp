#include<bits/stdc++.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
using namespace std;
using namespace cv;
Mat img;
int totstnodes=0;
int totennodes=0;
int reached =0 ;
int step_size = 10;
int iter = 0;
typedef struct
{
  int x;
  int y;
}coordi;//Defining the coordinate structure

struct nnode 
{
    vector<nnode *> children;
    nnode *parent;
    coordi position;
};

nnode* startnnodes[100000];
nnode* endnnodes[100000];
nnode st,en;
float nnode_dist(coordi p, coordi q)
{
  int m = p.x - q.x;
  int n = p.y - q.y;
  return sqrt(pow(m, 2) + pow(n, 2));
}

void init()
{
coordi start;coordi end;
for(int i=0;i<img.rows;i++)
{
for(int j=0;j<img.cols;j++)
{
if(img.at<Vec3b>(i,j)[1]>128)
{
start.x = i;
start.y = j;
}
if(img.at<Vec3b>(i,j)[2]>128)
{
end.x = i;
end.y = j;
}
}
}
st.position = start;
en.position = end;
st.parent = NULL;
en.parent = NULL;
startnnodes[totstnodes++] = &st;
endnnodes[totennodes++] = &en;
srand(time(NULL));
}

int near_nnodeen(nnode rnnode)
{
  float min_dist = 999.0, dist= nnode_dist(en.position, rnnode.position);
  int lnnode = 0, i = 0;
  for(i=0; i<totennodes; i++)
  {
    dist = nnode_dist(endnnodes[i]->position, rnnode.position);
    if(dist<min_dist)
    {
      min_dist = dist;
      lnnode = i;
    }
  }
  return lnnode;
}

int near_nnodest(nnode rnnode)
{
  float min_dist = 999.0, dist= nnode_dist(st.position, rnnode.position);
  int lnnode = 0, i = 0;
  for(i=0; i<totstnodes; i++)
  {
    dist = nnode_dist(startnnodes[i]->position, rnnode.position);
    if(dist<min_dist)
    {
      min_dist = dist;
      lnnode = i;
    }
  }
  return lnnode;
}

coordi steptake(coordi nnnode, coordi rnnode)
{
  coordi interm, step;
  float magn = 0.0, x = 0.0, y = 0.0;
  interm.x = rnnode.x - nnnode.x;
  interm.y = rnnode.y - nnnode.y;
  magn = sqrt((interm.x)*(interm.x) + (interm.y)*(interm.y));
  x = (float)(interm.x / magn);
  y = (float)(interm.y / magn);
  step.x = (int)(nnnode.x + step_size*x);
  step.y = (int)(nnnode.y + step_size*y);
  return step;
}

int check_validity_1(coordi p, coordi q)
{
  coordi large, small;
  int i = 0, j1 = 0, j2 = 0;
  double slope;
  if(q.x<p.x)
  {
    small = q;
    large = p;
  }
  else
  {
    small = p;
    large = q;
  }
  if(large.x == small.x)
    return 0;
  slope = ((double)large.y - small.y)/((double)large.x - small.x);
  for(i=small.x+1; i<large.x; i++)
  {
    j1 = (int)((i*slope) - (small.x)*(slope) + small.y);
    j2 = j1 + 1;
    if((i<0) || (i>400) || (j1<0) || (j1>400) || (j2<0) || (j2>400))
      continue;
    if(((int)img.at<Vec3b>(i, j1)[0] >200) && ((int)img.at<Vec3b>(i, j1)[1] >200) && ((int)img.at<Vec3b>(i, j1)[2] >200))
     return 0;
     if(((int)img.at<Vec3b>(i, j2)[0] >200) && ((int)img.at<Vec3b>(i, j2)[1]>200) && ((int)img.at<Vec3b>(i, j2)[2]>200))
      return 0;
  }
  return 1;
}

int check_validity_2(coordi p, coordi q)
{
  coordi large, small;
  int i = 0, j1 = 0, j2 = 0;
  double slope;
  if(q.y<p.y)
  {
    small = q;
    large = p;
  }
  else
  {
    small = p;
    large = q;
  }
  if(large.x == small.x)
    return 0;
  slope = ((double)large.y - small.y)/((double)large.x - small.x);
  for(i=small.y+1; i<large.y; i++)
  {
    j1 = (int)(((i - small.y)/slope) + small.x);
    j2 = j1 + 1;
    if((i<0) || (i>400) || (j1<0) || (j1>400) || (j2<0) || (j2>400))
      continue;
    if(((int)img.at<Vec3b>(j1, i)[0] >200) && ((int)img.at<Vec3b>(j1, i)[1] >200) && ((int)img.at<Vec3b>(j1, i)[2] >200))
     return 0;
     if(((int)img.at<Vec3b>(j2, i)[0] >200) && ((int)img.at<Vec3b>(j2, i)[1] >200) && ((int)img.at<Vec3b>(j2, i)[2] >200))
      return 0;
  }
  return 1;
}

void draw_path(nnode x)
{
  nnode up, down;
  int breaking = 0;
  down = x;
  up = *(x.parent);
  while(1)
  {
    line(img, Point(up.position.y, up.position.x), Point(down.position.y, down.position.x), Scalar(0, 255, 0), 2, 8);
    if(up.parent == NULL)
      break;
    up = *(up.parent);
    down = *(down.parent);
  }
}

void rrt()
{
if(iter%2==0)
{
  int flag1 = 0, index = 0, flag2 = 0;
    nnode* rnnode = new nnode;
    nnode* stepnnode = new nnode;
    (rnnode->position).x = rand() % 400 + 1;
    (rnnode->position).y = rand() % 400 + 1;
    index = near_nnodeen(*rnnode);
    if((nnode_dist(rnnode->position, endnnodes[index]->position)) < step_size)
      return;
    else
      stepnnode->position = steptake(endnnodes[index]->position, rnnode->position);
    flag1 = check_validity_1(endnnodes[index]->position, stepnnode->position);
    flag2 = check_validity_2(endnnodes[index]->position, stepnnode->position);
    if((flag1 == 1) && (flag2 == 1))
    {
      endnnodes[totennodes++] = stepnnode;
      stepnnode->parent = endnnodes[index];
      (endnnodes[index]->children).push_back(stepnnode);
      line(img, Point((stepnnode->position).y, (stepnnode->position).x), Point(endnnodes[index]->position.y, endnnodes[index]->position.x), Scalar(0, 255, 255), 2, 8);
      for(int i=stepnnode->position.x - 2; i < stepnnode->position.x + 2; i++)
      {
        for(int j=stepnnode->position.y - 2; j < stepnnode->position.y + 2; j++)
        {
          if((i<0) || (i>400) || (j<0) || (j>400))
            continue;

          img.at<Vec3b>(i, j)[0] = 255;
          img.at<Vec3b>(i, j)[1] = 0;
          img.at<Vec3b>(i, j)[2] = 0;
        }
      }
    }
}
else
{
nnode target = *endnnodes[totennodes];
int index = near_nnodest(target);
if((check_validity_1((startnnodes[index])->position,target.position))&&(check_validity_2((startnnodes[index])->position,target.position))&&(nnode_dist((startnnodes[index])->position,target.position)<step_size))
{
        reached = 1;
        startnnodes[totstnodes++] = &target;
        target.parent = startnnodes[index];
        (startnnodes[totstnodes-1]->children).push_back(&target);
        draw_path(st);
}
}
    iter++;
}

int main()
{
img = imread("/home/asus/KRSSG Work/task 3/subtask/rrt.png",1);
while(reached==0)
{
rrt();
}
namedWindow( "window", WINDOW_AUTOSIZE );
  imshow("window", img);
  imwrite("output.png", img);
  waitKey();
  return 0;
}
