#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <algorithm>
#include <limits.h>
#include <unistd.h>
#include <bits/stdc++.h> 
#include <iostream> 
#include <utility> 

using namespace std;

#define HEIGHT 800
#define BUTTON_HEIGHT 100
#define POINTSIZE 20.0f
#define WIDTH 1200
#define SPACING 7
#define X_CENTER WIDTH/2
#define Y_CENTER HEIGHT/2 + BUTTON_HEIGHT + 50
#define MAX_V 15

#define MAX(x,y) ((x>y)?x:y)
#define MIN(x,y) ((x<y)?x:y)


struct node
{
  int x;
  int y;
  int nodeNum;
  int color;
};

struct colours
{
  float *rArray;
  float *gArray;
  float *bArray;
};

struct eqnParameters /* (xCoeff * x + yCoeff * y + C = 0) */
{
  int node1;
  int node2;
  int xCoeff;
  int yCoeff;
  int C;
};

struct edge
{
  int node1;
  int node2;
  int weight;
};

struct allflags
{
    int mouse;  
    int del;
    int djk;
    int edgeIns;
    int edgeDel;
    int directed;
    int weighted;
}flag;

struct strDjk
{
  int dist;
  int visited;
  int prev;
};

//global variables
int djkNode;
int oldIndex;
int djkStart, djkEnd;
int node1, node2;
int* finalPath;
int* fullPath;
int fullPathIndex;
int *r;
int finalPathLength = 0;
char colorName[][40] = {"Yellow", "Purple", "Cyan", "Bight Blue", "Orange", "Light Brown", "Copper", "Brown", "Navy Blue", "Maroon","Lime Green", "Olive Green", "Gray", "Voilet", "Turquoise"};

//function definitions
void flagsInit(int d,int w);
int minDist(strDjk* djk, int noOfVertices);
void reset();
void mouse(int button, int state, int mousex, int mousey);
void chromaticNumber();
void keyboard(unsigned char c, int x, int y);
void dijkstra(int start, int end);
void printLegend();

class Graph
{
  private:
    int noOfVertices;
    int noOfEdges;
    int **AdjacencyMatrix;
    node *vertexArray;
    eqnParameters *allEquations;
    struct colours obj;
    struct edge* edgeWeights;


  public:
    void initGraph(int Num)
    {
      noOfVertices = Num;
      AdjacencyMatrix = (int**)calloc(Num, sizeof(int*));
      for(int i=0; i<Num; i++)
      {
        AdjacencyMatrix[i]=(int*)calloc(Num, sizeof(int));
      }
      vertexArray=(node*)calloc(Num, sizeof(node));
      for(int i=0; i<noOfVertices; i++)
      {
        vertexArray[i].nodeNum = i;
        vertexArray[i].color = -1;
      }

    }

    void setMatrix()
    {
      noOfEdges=0;
      for(int i=0; i<noOfVertices; i++)
      {
        for(int j=0; j<noOfVertices; j++)
        {
          scanf("%d", &AdjacencyMatrix[i][j]);
          //printf("%d\n",AdjacencyMatrix[i][j]);
          if(AdjacencyMatrix[i][j]>=1)
          {
            noOfEdges++;
          }
        }
        //printf("\n");
      }
      if(flag.directed ==0)
      {
        noOfEdges/=2;
      }

      allEquations = (eqnParameters*)calloc(300, sizeof(eqnParameters));

    }

    void setArray()
    {
      int xCentre = X_CENTER;
      int yCentre = Y_CENTER;
      if(vertexArray[0].x == 0 && vertexArray[0].y == 0)
      {
        vertexArray[0].x = X_CENTER;
        vertexArray[0].y = Y_CENTER;
      }
      int j=10;
      for(int i=1; i<noOfVertices; i++)
      {
        if(vertexArray[i].x == 0 && vertexArray[i].y == 0)
        {
          if((i)%4==1)
          {
            vertexArray[i].x = xCentre+j*10;
            vertexArray[i].y = yCentre+j*10;
          }
          if((i)%4==2)
          {
            vertexArray[i].x = xCentre-j*10;
            vertexArray[i].y = yCentre+j*10;
          }
          if((i)%4==3)
          {
            vertexArray[i].x = xCentre+j*10;
            vertexArray[i].y = yCentre-j*10;
          }
          if((i)%4==0)
          {
            vertexArray[i].x = xCentre-j*10;
            vertexArray[i].y = yCentre-j*10;
          }
        }

        if(i%4==0)
        {
          j+=SPACING;
        }
      }
      makeAllEquations();
      
    } 

    static bool compareWeight(edge e1, edge e2) 
    { 
      return (e1.weight < e2.weight); 
    } 

    edge* lineThickness()
    {
       int index=0;
       edgeWeights = (edge*)malloc(sizeof(edge)*noOfEdges);
       if(flag.directed==1)
       {
          for(int i=0;i<noOfVertices;i++)
          {
              for(int j=0;j<noOfVertices;j++)
              {
                  if(AdjacencyMatrix[i][j]>0)
                  {
                      edgeWeights[index].node1 = i;
                      edgeWeights[index].node2 = j;
                      edgeWeights[index].weight = AdjacencyMatrix[i][j];
                      index++;
                  }
              }
          }
       }
       else
       {
          for(int i=0;i<noOfVertices;i++)
          {
              for(int j=0;j<i;j++)
              {
                  if(AdjacencyMatrix[i][j]>0)
                  {
                      printf("%d",index);
                      edgeWeights[index].node1 = i;
                      edgeWeights[index].node2 = j;
                      edgeWeights[index].weight = AdjacencyMatrix[i][j];
                      index++;
                  }
              }
          }
       }
       std::sort(edgeWeights, edgeWeights+noOfEdges, compareWeight);
       return(edgeWeights);
    
    }

    void insertNode()
    {
          reset();
                  
          int newNode=noOfVertices;
          int edgeNode=0;
          int k=0;      
          int i,j;

          noOfVertices++;
          r = (int*)realloc(r, sizeof(int)*(noOfVertices));
          vertexArray = (node*)realloc(vertexArray, sizeof(node)*(noOfVertices));
          AdjacencyMatrix=(int**)realloc(AdjacencyMatrix, sizeof(int*)*(noOfVertices));
          
          
          for(i=0; i<noOfVertices-1; i++)
          {
            AdjacencyMatrix[i]=(int*)realloc(AdjacencyMatrix[i], sizeof(int)*(noOfVertices));
          }

          AdjacencyMatrix[noOfVertices-1]=(int*)calloc(noOfVertices, sizeof(int));

          for(i=0; i<noOfVertices-1; i++)
          {
            AdjacencyMatrix[i][noOfVertices-1] = 0;
          }

          vertexArray[noOfVertices-1].x=0;
          vertexArray[noOfVertices-1].y=0;
          vertexArray[noOfVertices-1].nodeNum = noOfVertices-1;
          
          
          //IF INPUT IS NOT DIRECTED
          if(flag.directed==0)
          {
            //IF INPUT IS WEIGHTED
            if(flag.weighted == 1)
            {
              for(int i=0;i<noOfVertices-1;i++)
              {
                scanf("%d",&AdjacencyMatrix[noOfVertices-1][i]);
                //printf("Hello%d\n", AdjacencyMatrix[noOfVertices-1][i]);
                AdjacencyMatrix[noOfVertices-1][i] = AdjacencyMatrix[i][noOfVertices-1];
              }
            }

            //IF INPUT IS NOT WEIGHTED
            else
            {
              scanf("%d", &edgeNode);
              while((k<noOfVertices)&&(edgeNode!=0))
              {
                  scanf("%d",&edgeNode);
                  if(edgeNode!=0)
                  {
                      AdjacencyMatrix[noOfVertices-1][edgeNode-1]=1;
                      AdjacencyMatrix[edgeNode-1][noOfVertices-1]=1;
                  }
                  k++;
              }
            }
          }


          //IF INPUT IS DIRECTED
          else
          {
              //IF INPUT IS NOT WEIGHTED
              if(flag.weighted == 0)
              {
                k=0;
                //printf("directed\n");
                scanf("%d", &edgeNode);
                while((k<noOfVertices)&&(edgeNode!=0))
                {
                  scanf("%d",&edgeNode);
                  //printf("%d\t", edgeNode);
                  if(edgeNode!=0)
                  {
                    AdjacencyMatrix[noOfVertices-1][edgeNode-1]=1;
                  }
                  k++;
                }

                //printf("\n");
                k=0;
                scanf("%d", &edgeNode);
                while((k<noOfVertices)&&(edgeNode!=0))
                {
                  scanf("%d",&edgeNode);
                  //printf("%d\t", edgeNode);
                  if(edgeNode!=0)
                  {
                    AdjacencyMatrix[edgeNode-1][noOfVertices-1]=1;
                  }
                  k++; 
                }
              }

              //IF INPUT IS WEIGHTED
              else
              {
                  for(int i=0;i<noOfVertices;i++)
                  {
                    scanf("%d",&AdjacencyMatrix[noOfVertices-1][i]);
                  }

                  for(int i=0;i<noOfVertices;i++)
                  {
                    scanf("%d",&AdjacencyMatrix[i][noOfVertices-1]);
                  }
              }


          }
          //printf("\n");
          vertexArray[noOfVertices-1].color = -1;        

        
        setArray();
        printLegend();

    }
    
    void deleteNode(int delnode)
    {
      reset();

      int i,j; 
      for(i=delnode;i<noOfVertices-1;i++)
      {
        for(j=0;j<noOfVertices;j++)
         {
            AdjacencyMatrix[j][i]=AdjacencyMatrix[j][i+1];  
            AdjacencyMatrix[i][j]=AdjacencyMatrix[i+1][j];  
         }
      }
      
      for(i=0;i<noOfVertices;i++)
      {
        AdjacencyMatrix[i][i]=0;
      }
      
      AdjacencyMatrix=(int**)realloc(AdjacencyMatrix, sizeof(int*)*(noOfVertices-1));
      for(i=0; i<noOfVertices-1; i++)
      {
           AdjacencyMatrix[i]=(int*)realloc(AdjacencyMatrix[i], sizeof(int)*(noOfVertices-1));
      }

      for(i=delnode; i<noOfVertices-1; i++)
      {
        vertexArray[i].x = vertexArray[i+1].x;
        vertexArray[i].y = vertexArray[i+1].y;
        vertexArray[i].nodeNum = vertexArray[i+1].nodeNum;
        r[i] = r[i+1];
      }
      vertexArray = (node*)realloc(vertexArray, sizeof(node)*(noOfVertices-1));
      r = (int*)realloc(r, sizeof(int)*(noOfVertices-1));
      noOfVertices--;
    }

    void deleteEdge(int i)
    {
        reset();

        int node1, node2;
        int X = allEquations[i].node1; 
        int Y = allEquations[i].node2;

        for(int j = 0; j<noOfVertices; j++)
        {
          if(X == vertexArray[j].nodeNum)
          {
            node1 = j;
          }

          if(Y == vertexArray[j].nodeNum)
          {
            node2 = j;
          }
        }
        //printf("inside delete %d %d %d\n",i, X, Y);
        AdjacencyMatrix[node1][node2] = 0;
        AdjacencyMatrix[node2][node1] = 0;
        for(int j=i; j<noOfEdges -1; j++)
        {
          allEquations[i] = allEquations[i+1]; 
        }
        noOfEdges--;
        //setArray();
    }


    void insertEdge(int node1, int node2)
    {
        reset();

        AdjacencyMatrix[node1][node2] = 1;
        if(flag.directed == 0)
        {
          AdjacencyMatrix[node2][node1] = 1;
        }

        allEquations = (eqnParameters*)realloc(allEquations, ((noOfEdges+1)*sizeof(eqnParameters)));
    }

    void makeAllEquations()
    {
      int k = 0, gcd = 0;
      if(flag.directed == 0)
      {
        for(int i=0; i < noOfVertices; i++)
        {
          for(int j=0; j< i; j++)
          {
            if(AdjacencyMatrix[i][j]>=1)
            {
              allEquations[k].node1 = vertexArray[i].nodeNum;
              allEquations[k].node2 = vertexArray[j].nodeNum;

              gcd = std::__gcd((vertexArray[j].y - vertexArray[i].y), (vertexArray[j].x - vertexArray[i].x));

              if(gcd==0) gcd=1;

              allEquations[k].xCoeff = (vertexArray[j].y - vertexArray[i].y)/gcd;
              allEquations[k].yCoeff = (vertexArray[i].x - vertexArray[j].x)/gcd;
              allEquations[k].C = (-1)*((allEquations[k].xCoeff)*(vertexArray[i].x) + (allEquations[k].yCoeff)*(vertexArray[i].y));
              
              //printf("%dx + %dy + %d\t(%d, %d)\n",allEquations[k].xCoeff, allEquations[k].yCoeff, allEquations[k].C, i, j);
              k++;
            }
          }
        }
      }
      else
      {
        for(int i=0; i < noOfVertices; i++)
        {
          for(int j=0; j< noOfVertices; j++)
          {
            if(AdjacencyMatrix[i][j]>=1)
            {
              allEquations[k].node1 = vertexArray[i].nodeNum;
              allEquations[k].node2 = vertexArray[j].nodeNum;

              gcd = std::__gcd((vertexArray[j].y - vertexArray[i].y), (vertexArray[j].x - vertexArray[i].x));

              if(gcd==0) gcd=1;

              allEquations[k].xCoeff = (vertexArray[j].y - vertexArray[i].y)/gcd;
              allEquations[k].yCoeff = (vertexArray[i].x - vertexArray[j].x)/gcd;
              allEquations[k].C = (-1)*((allEquations[k].xCoeff)*(vertexArray[i].x) + (allEquations[k].yCoeff)*(vertexArray[i].y));
              
              //printf("%dx + %dy + %d\t(%d, %d)\n",allEquations[k].xCoeff, allEquations[k].yCoeff, allEquations[k].C, i, j);
              k++;
            }
          }
        }
      }
    }

    

    void vertexColor()
    {
      node *point = getArray();
      float redIndex[] = {1, 1, 0, 0, 1, 0.737255, 0.72, 0.647059, 0.137255, 0.556863, 0.196078, 0.309804, 0.752941, 0.309804, 0.678431};
      float greenIndex[] = {1, 0, 1, 0.19, 0.5, 0.560784, 0.45, 0.164706, 0.137255, 0.137255, 0.8, 0.309804, 0.752941, 0.184314, 0.917647};
      float blueIndex[] = {0, 1, 1, 1, 0.623529, 0.560784, 0.20, 0.164706, 0.556863, 0.419608, 0.196078, 0.184314, 0.752941, 0.309804, 0.917647};
      
      obj.rArray = redIndex;
      obj.gArray = greenIndex;
      obj.bArray = blueIndex;

      int i;
      for(i=0; i<noOfVertices; i++)
      {
        if(vertexArray[i].nodeNum>=0)
        {
          glColor3f(redIndex[(vertexArray[i].nodeNum)%15], greenIndex[(vertexArray[i].nodeNum)%15], blueIndex[(vertexArray[i].nodeNum)%15]);
        }

        if(vertexArray[i].color!=-1)
        {
          glColor3f(redIndex[vertexArray[i].color], greenIndex[vertexArray[i].color], blueIndex[vertexArray[i].color]); 
        }

        if(r[i]==1)
        {
          glColor3f(0, 0, 0);
        }

        glVertex2i(point[i].x, point[i].y);
      }    
    }

    int **getMatrix()
    {
      return AdjacencyMatrix;
    }

    void printVertexArray()
    {
        for(int i=0; i<noOfVertices; i++)
        {
            printf("%d, (%d, %d)\n", i, vertexArray[i].x, vertexArray[i].y);
        }
    }

    void printAdjMat()
    {
        for(int i=0;i<noOfVertices;i++)
        {
          for(int j=0;j<noOfVertices;j++)
          {
            printf("%d ",AdjacencyMatrix[i][j]);
          }
          printf("\n");
        }
    }

    void setArrayByIndex(int index, int x, int y)
    {
        vertexArray[index].x = x;
        vertexArray[index].y = y;
        makeAllEquations();
        return;
    }

    node *getArray()
    {
      return vertexArray;
    }

    int getNoOfVertices()
    {
      return noOfVertices;
    }

    int getNoOfEdges()
    {
      return noOfEdges;
    }

    void printVertexCo()
    {
      for(int i = 0; i<noOfVertices; i++)
      {
        printf("(%d, %d)\n", vertexArray[i].x, vertexArray[i].y);
      }
    }

    eqnParameters* getAllEquations()
    {
        return allEquations;
    }

    void setNoOfVertices(int x)
    {
      noOfVertices = x;
    }
    void freeGraph()
    {
      for(int i=0; i<noOfVertices; i++)
      {
        free(AdjacencyMatrix[i]);
      }
      free(AdjacencyMatrix);
      free(vertexArray);
    }
};

Graph X;

bool isMiddle(int n1, int n2, int x, int y)
{
  int noOfVertices = X.getNoOfVertices();
  node *vertexArray = X.getArray();
  int x1, y1, x2, y2;
  for(int i=0; i<noOfVertices; i++)
  {
    if(vertexArray[i].nodeNum == n1)
    {
      x1 = vertexArray[i].x;
      y1 = vertexArray[i].y;
    }
    if(vertexArray[i].nodeNum == n2)
    {
      x2 = vertexArray[i].x;
      y2 = vertexArray[i].y;
    }

  }

  if((x >= MIN(x1, x2)) && (x <= MAX(x1, x2)) && (y >= MIN(y1, y2)) && (y <= (MAX(y1, y2))))
  {
    return true;
  }
  return false;
}


void flagsInit(int d,int w)
{
    flag.mouse = 0; 
    flag.del = 0;
    flag.djk=0;
    flag.edgeDel = 0;
    flag.edgeIns = 0;
    flag.directed = d;
    flag.weighted = w;
}

void reset()
{

  //Resetting DJK
  for(int i=0;i<fullPathIndex;i++)
  {
     r[finalPath[i]] = 0;
  }

  //Resetting Chromatic No
  node *vertexArray = X.getArray();
  int noOfVertices = X.getNoOfVertices();
  for(int i=0; i<noOfVertices; i++)
  {
    vertexArray[i].color = -1;
  }


}

int minDist(strDjk* djk, int noOfVertices)
{
  int i;
  int min_node;
  int min = INT_MAX;
  for(i=0;i<noOfVertices;i++)
  {
    if( djk[i].dist < min && djk[i].visited==0 )
    {
      min = djk[i].dist;
      min_node=i;
    }
  }
  return(min_node);
}

void Display();

void dijkstra(int start, int end)
{
  

  int i,j; 
  int nextNode;
  int noOfVertices;
  int** graph;
  int AdjacencyMatrix;
  strDjk* djk;

  
  fullPathIndex = 0;
  noOfVertices = X.getNoOfVertices();
  graph = X.getMatrix();
  finalPath = (int*)malloc(sizeof(int)*noOfVertices);
  fullPath = (int*)malloc(sizeof(int)*noOfVertices*noOfVertices);
  djk = (strDjk*)malloc(sizeof(strDjk)*noOfVertices);

  for(int i=0; i<noOfVertices; i++)
  {
    r[i] = 0;
  }
  
  for(i=0;i<noOfVertices;i++)
  {
    djk[i].dist = INT_MAX;
    djk[i].visited = 0;
  }
  //printf("outside for\n");

  djk[start].dist = 0;
  
  for(i=0;i<noOfVertices;i++)
  { 
    nextNode = minDist(djk, noOfVertices);
    djkNode = nextNode;
    djk[nextNode].visited = 1;
    //sleep(5);
    //glutDisplayFunc(Display);
    //glutPostRedisplay();
    fullPath[fullPathIndex] = nextNode;
    fullPathIndex++;
    for(j=0;j<noOfVertices;j++)
    {

      if( j!=start && graph[nextNode][j]!=0 && djk[nextNode].dist+graph[nextNode][j]<djk[j].dist)
      {
        djkNode = j;
        djk[j].dist = djk[nextNode].dist+graph[nextNode][j];
        fullPath[fullPathIndex] = j;
        fullPathIndex++;
        djk[j].prev=nextNode;
        //Display();

        //sleep(5);
        //glutDisplayFunc(Display);
        //glutPostRedisplay();
        if(j==end)
        {
          break;
        }
      }
    }
    if(j==end)
    break;
  } 
  
  //free(djk);
  //free(stack);
  //if you want proper path
  i=end;
  j=0; 
  while(i!=start)
  {
    finalPath[j] = i;
    j++;
    i=djk[i].prev;
  }
  finalPath[j] = i;
  
  printf("FULL PATH IS - \n");
  for(i=0;i<fullPathIndex;i++)
  {
    //printf("%d\n",fullPath[i]);
  }
  
  printf("THE SHORTEST PATH IS - \n");
  for(i=j;i>=0;i--)
  {
    printf("%d\n",finalPath[i]);
  }
  finalPathLength=j+1;
  for(int i=0; i<finalPathLength; i++)
  {
      r[finalPath[i]] = 1;
  }
    
  Display();
  flag.djk = 0;
  free(djk);
}  



void makeRect(int x1, int x2, int y1, int y2, float r, float g, float b)
{
    glColor3f(r,g,b);
    //glLineWidth(30);
    glBegin(GL_POLYGON);
    glVertex2i(x1,y1);
    glVertex2i(x2,y1);
    glVertex2i(x2,y2);
    glVertex2i(x1,y2);
    //glLineWidth(1);
    glEnd();
    glFlush();   
}



void Display()
{
    int x = 0, y = 0;
    float factor=0.5;
    int node1x,node1y,node2x,node2y;
    int count = X.getNoOfVertices();
    int noOfEdges = X.getNoOfEdges();
    //edge* edgeWeights = X.lineThickness();
    node *point = X.getArray();
    int **AdjacencyMatrix = X.getMatrix();
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT); // clear display window
    
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const double w = glutGet(GLUT_WINDOW_WIDTH);
    const double h = glutGet(GLUT_WINDOW_HEIGHT);
    gluOrtho2D(0.0, w, 0.0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    

    for (int i=0; i<count; i++)
    {
      for(int j=0; j<count; j++)
      {
        if(flag.directed==1)
        {
          if((AdjacencyMatrix[i][j]>=1) && (AdjacencyMatrix[j][i]>=1)) 
          {
            glColor3f(1, 0, 0);
            glVertex2i(point[i].x, point[i].y);
            glVertex2i(point[j].x, point[j].y);
          }
          else if (AdjacencyMatrix[i][j]>=1)
          { 
            if(point[i].x > point[j].x)
            {  
              glColor3f(0, 0, 1);
            }
            else if (point[i].x < point[j].x)
            {
              glColor3f(0, 1, 0);
            }
            else
            {
              if(point[i].y > point[j].y)
              {
                glColor3f(0, 0, 1);
              }
              else
              {
                glColor3f(0, 1, 0);
              }
            }

            if((r[i]==1) && (r[j]==1))
            {
              glColor3f(0, 0, 0);
            }
            
            glVertex2i(point[i].x, point[i].y);
            glVertex2i(point[j].x, point[j].y);               
          }
        }
        else if(AdjacencyMatrix[i][j]>=1) 
        {
          if((r[i]==1) && (r[j]==1))
          {
             glColor3f(0, 0, 0);
          }
          else
          {
            glColor3f(1, 0, 0);
          }
          glVertex2i(point[i].x, point[i].y);
          glVertex2i(point[j].x, point[j].y);
        }
        
      }
    }
    
 
    glLineWidth(8);
    glBegin(GL_LINES);
    int f,t;

      for(int j=0;j<count;j++)
      {
        if(finalPath!=NULL && point[j].nodeNum == finalPath[0])
        {
          r[finalPath[0]]=1;
          f=j;
        }
      }
      for(int i=1; i<finalPathLength; i++)
      {
        r[finalPath[i]] = 1;
        for(int j=0;j<count;j++)
        {
            if(point[j].nodeNum == finalPath[i])
            {
                t=j;
                //glLineWidth(8);
                glColor3f(0,0,0);
                glVertex2i(point[f].x, point[f].y);
                glVertex2i(point[t].x, point[t].y);
                f=t;
                break;

            }
        }
      }
    glEnd();

    glPointSize(POINTSIZE);
    glBegin(GL_POINTS);
    
    X.vertexColor();

    /* For djk Path */
    /*if(flag.djk == 3)
    {
      for(int i=0;i<fullPathIndex;i++)
      {
        for(int j=0;j<count;j++)
        {
          if(fullPath[i] == point[j].nodeNum)
          {
            printf("in if\n");
            glColor3f(1, 0, 0);
            glVertex2i(point[j].x, point[j].y);
            //sleep(1);
          }
        }
      }
      flag.djk = 0;
    }*/
    
    glEnd();
    makeRect(0,WIDTH,0,200,0.8,0.8,0.8);
    makeRect(120,270,20,80,0,0,0);
    makeRect(120,270,120,180,0.35,0.35,0.35);
    makeRect(390,540,20,80,0,0,0);
    makeRect(390,540,120,180,0.35,0.35,0.35);
    makeRect(660,810,20,80,0.5,0.5,0.5);
    makeRect(930,1080,20,80,0,0,0);
    makeRect(930,1080,120,180,0.35,0.35,0.35);
    glFlush();
}

int dfs_recurse(int i,int count, int *visit, int **AdjacencyMatrix,int noOfVertices)
{
  int w;
    visit[i]=count;
    for(w=0;w<noOfVertices;w++)
    {
      if(visit[w]==0&&AdjacencyMatrix[i][w]>=1)
        dfs_recurse(w,count,visit,AdjacencyMatrix,noOfVertices);
    }
}
int count_components_dfs(int **AdjacencyMatrix, int noOfVertices)
{
  int i,count=0;
  int* visit=(int*)malloc(noOfVertices*sizeof(int));
  for(i=0;i<noOfVertices;i++)
  {
    visit[i]=0;
  }
  for(i=0;i<noOfVertices;i++)
  {
    if(visit[i]==0)
    {
      count++;
      dfs_recurse(i,count,visit,AdjacencyMatrix,noOfVertices);
    }
  }
  return count;
}

void profile()
{
      int i,j,k=0;
      int countEdge=0;
      int countSelf=0;
      int countComplete=0;
      int noOfVertices = X.getNoOfVertices();
      int** AdjacencyMatrix = X.getMatrix();
      int *selfVertices=(int*)malloc(sizeof(int)*noOfVertices);
      //int i,j;
      for(i=0;i<noOfVertices;i++)
      {
        selfVertices[i]=0;
      }
      
      for(i=0;i<noOfVertices;i++)
      {
        for(j=0;j<noOfVertices;j++)
        {
          if(AdjacencyMatrix[i][j]>=1)
          {  
            countEdge++;
          }
        }
      }

      for(i=0;i<noOfVertices;i++)
      {
        if((AdjacencyMatrix[i][i]==0))
        { 
          countComplete++;
        }
      }

      for(i=0;i<noOfVertices;i++)
      {
        if(AdjacencyMatrix[i][i]>=1)
        {
            countSelf++;
            if(k<noOfVertices)
            {

              selfVertices[k]=i+1;
              k++;
            }
        }
      }
      printf("--------------------------------------------------------------------------------------------------------\n");
      printf("\t\t\t\t\tPROFILE OF THE GRAPH\n");
      printf("--------------------------------------------------------------------------------------------------------\n");
      
      //direction
      printf("Direction\t\t\t: ");
      if(flag.directed==0)
      {      
          printf("Undirected\n");
      }
      else
      {  
        printf("Directed\n");
      }

      //completeness
      printf("Completeness\t\t\t: ");
      if(flag.directed==0)
      {
            if(countComplete==noOfVertices && (countEdge==(noOfVertices*(noOfVertices-1))/2)) 
            {
              printf("Complete\n");
            }
            else
            {
              printf("Incomplete\n");
            }
      }
      else
      {
          if(countComplete==noOfVertices && (countEdge==(noOfVertices*(noOfVertices-1)))) 
          {
            printf("Complete\n");
          }
          else
          {
            printf("Incomplete\n");
          }
      }
      
      //vertices
      printf("No of Vertices\t\t\t: %d\n",noOfVertices);

      //edgess
      printf("No of Edges\t\t\t: %d\n",countEdge);
         
      // connected components
      if(flag.directed==0)
      {
          int result=count_components_dfs(AdjacencyMatrix,noOfVertices);
          printf("No of Connected Components \t: %d\n",result);
      }

      //self loop
      int j1 =0;  
      printf("No of Self-Loops\t\t: %d",countSelf);
      if(countSelf!=0)
      {
          printf("\t(Vertices with Self-Loop : ");
          for(i=0;i<k+1;i++)
          {
              if((selfVertices[i]!=0) && (j1<countSelf-1))
              {  
                  printf("%d, ",selfVertices[i]);
                  j1++;
              }
              else if(selfVertices[i]!=0)
              {
                 printf("%d)",selfVertices[i]);
              }

          }
      }
      printf("\n");  
      printf("--------------------------------------------------------------------------------------------------------\n");     
}  
  

void mouse(int button, int state, int mousex, int mousey)
{
    mousey = HEIGHT + BUTTON_HEIGHT - mousey; 
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {   

        int x,y,i;
        int noVertices, noEdges;
        
        node* vertexArray;
        
        x = mousex;
        y = mousey;
        vertexArray = X.getArray();
        noVertices = X.getNoOfVertices();
        noEdges = X.getNoOfEdges();

        i=0;

        printf("MOUSE DETECTED (%d, %d)", mousex, mousey);
        while(i<noVertices)
        {
              if((vertexArray[i].x+10)>=x && (vertexArray[i].x-10)<=x && (vertexArray[i].y-10)<=y && (vertexArray[i].y+10)>=y)
              {
                printf("-NODE %d ",vertexArray[i].nodeNum);
                break;
              }
              i++;
        }
        printf("\n");

        // when you want to insert a node
        if(x > 120 && x < 270 && y > 20 && y < 80)
        {
          if(noVertices>16)
          {
            printf("CAN NOT INSERT MORE NODES\n");
          }
          else
          {
            printf("NODE INSERTED SUCCESSFULLY\n");
            X.insertNode();
          }
        }

        //when you want to delete a node
        else if(x > 390 && x < 540 && y > 20 && y < 80 && flag.del == 0)
        {
          printf("CLICK ON THE NODE THAT YOU WISH TO DELETE\n");
          flag.del = 1;
        }

        //implementing djk
        else if(x > 930 && x < 1080 && y > 20 && y < 80 && flag.del == 0)
        {
            reset();
            flag.djk = 1;
            printf("CLICK ON START NODE\n");
        } 

        //when you want to delete a edge
        else if(x > 390 && x < 540 && y > 120 && y < 180 && flag.del == 0)
        {
            flag.edgeDel = 1;
            printf("CLICK ON THE EDGE THAT YOU WANT TO DELETE\n");
        }

        //to insert edge
        else if(x > 120 && x < 270 && y > 120 && y < 180 && flag.del == 0)
        {
            flag.edgeIns = 1;
            printf("CLICK ON FIRST NODE\n");
        }

        //profiling
        else if(x > 660 && x < 810 && y > 20 && y < 80)
        {
            profile();
        }

        //
        else if(x > 930 && x < 1080 && y > 120 && y < 180)
        {
            chromaticNumber();
        }

        //deleting the edge 
        else if(flag.edgeDel == 1)
        {
            int flagX = 0;
            eqnParameters* aE = X.getAllEquations();
            i = 0;
            while(i < noEdges)
            {
              for(int j=x-5; j<=x+5; j++)
              {
                for(int k=y-5; k<=y+5; k++)
                {
                  //printf("Result: %d, %d\n", aE[i].xCoeff*x + aE[i].yCoeff*y + aE[i].C, i);
                  if(((aE[i].xCoeff*j + aE[i].yCoeff*k + aE[i].C) == 0) && (isMiddle(aE[i].node1, aE[i].node2, j, k)))
                  {
                      flagX=1;
                      break;
                  }
                }
                if(flagX==1)
                {
                  break;
                }
              }
              if(flagX==1)
              {
                break;
              }
              i++;
            }

            if(i==noEdges)
            {
              printf("INVALID EDGE\n");
              return;
            }
            else if(flagX==1)
            {
                flag.edgeDel = 0;
                printf("%d\n", i);
                X.deleteEdge(i);
                printf("EDGE WILL BE DELETED\n");
            }


        }

        //inserting edge
        else if(flag.edgeIns == 1)
        {
            while(i<noVertices)
            {
                if((vertexArray[i].x+10)>=x && (vertexArray[i].x-10)<=x && (vertexArray[i].y-10)<=y && (vertexArray[i].y+10)>=y)
                {break;}
                i++;
            }
            if(i==noVertices)
            {
                printf("NOT A NODE\n");
                return;
            }

            else
            {
                node1 = i;
                printf("CLICK ON SECOND NODE\n");
                flag.edgeIns = 2;
            }
        }

        else if(flag.edgeIns == 2)
        {
            while(i<noVertices)
            {
                if((vertexArray[i].x+10)>=x && (vertexArray[i].x-10)<=x && (vertexArray[i].y-10)<=y && (vertexArray[i].y+10)>=y)
                {break;}
                i++;
            }
            if(i==noVertices)
            {
                printf("NOT A NODE\n");
                return;
            }

            else
            {
                node2 = i;
                printf("EDGE HAS BEEN ADDED\n");
                flag.edgeIns = 0;
                X.insertEdge(node1, node2);
            }
        }
        
        //selecting the node that you wish to delete
        else if(flag.del == 1)
        {
          
          while(i<noVertices)
          {
              if((vertexArray[i].x+10)>=x && (vertexArray[i].x-10)<=x && (vertexArray[i].y-10)<=y && (vertexArray[i].y+10)>=y)
              {break;}
              i++;
          }
          if(i==noVertices)
          {
              printf("NOT A NODE\n");
              return;
          }

          else
          {
              printf("NODE %d WILL BE DELETED\n",vertexArray[i].nodeNum);
              flag.del = 0;
              X.deleteNode(i);
                
          }
        }

        //selecting start point for djk
        else if(flag.djk == 1)
        {
          while(i<noVertices)
            {
                if((vertexArray[i].x+10)>=x && (vertexArray[i].x-10)<=x && (vertexArray[i].y-10)<=y && (vertexArray[i].y+10)>=y)
                {break;}
                i++;
            }
        
            // if it is not then dont do anything.
            if(i==noVertices)
            {
                printf("NO NODE FOUND. INVALID POINT.CLICK ON ANOTHER POINT.\n");
                return;
            }

            else
            {
                djkStart = i;
                printf("--------------------------------------------------------------------------------------------------------\n");
                printf("\t\t\t\tDIJKSTRAS ALGORITHM\n");
                printf("--------------------------------------------------------------------------------------------------------\n");
                printf("START NODE - %d\nCHOOSE END NODE\n",vertexArray[i].nodeNum);
                flag.djk=2;
            }

        }

        

        //selecting end point
        else if(flag.djk == 2)
        {
            while(i<noVertices)
            {
                if((vertexArray[i].x+10)>=x && (vertexArray[i].x-10)<=x && (vertexArray[i].y-10)<=y && (vertexArray[i].y+10)>=y)
                {break;}
                i++;
            }
        
            // if it is not then dont do anything.
            if(i==noVertices)
            {
                printf("NO NODE FOUND. INVALID POINT.CLICK ON ANOTHER POINT.\n");
                return;
            }

            else
            {
                printf("END NODE - %d\nDIJSTRAS HAS BEEN IMPLEMENTED\n",vertexArray[i].nodeNum);
                djkEnd = i;
                flag.djk=3;
                dijkstra(djkStart, djkEnd);
            }
        }


        

        // selecting a node if you want to move a node
        else if(flag.mouse==0)
        {
            //X.printVertexArray();
            while(i<noVertices)
            {
                if((vertexArray[i].x+10)>=x && (vertexArray[i].x-10)<=x && (vertexArray[i].y-10)<=y && (vertexArray[i].y+10)>=y)
                {break;}
                i++;
            }
        
            // if it is not then dont do anything.
            if(i==noVertices)
            {
                printf("NO NODE FOUND. INVALID POINT\n");
                return;
            }

            else
            {
                printf("NODE %d FOUND.CLICK ON THE POINT WHERE YOU WANT TO MOVE THE NODE TO. \n", vertexArray[i].nodeNum);
                flag.mouse=1;
                //X.printVertexArray();
                //old_index = (i>1)?(i-2):i;
                //glutMouseFunc(mouse);
                oldIndex = i;
                
            }
        }

        //selecting the place where you want to move
        else if(flag.mouse == 1)
        {
            printf("NODE MOVED SUCCESSFULLY\n");
            flag.mouse=0;
            X.setArrayByIndex(oldIndex, x, y);
            //X.printVertexArray();
        }

        else
        {
            printf("INVALID POINT\n");
        }
    }

    //glutPostRedisplay();
    Display();
}

void keyboard(unsigned char c, int x, int y)
{
  if((int)c==27) //Press Escape Key to Exit
  {
    X.freeGraph();
    exit(1);
  }
}

void chromaticNumber()
{
      int cr;
      int noOfVertices = X.getNoOfVertices();
      node* vertexArray = X.getArray();
      int** AdjacencyMatrix = X.getMatrix();
      bool *X = (bool*)calloc(noOfVertices, sizeof(bool));
      
      for(int i=0; i<noOfVertices; i++)
      {
        X[i] = false;
      }

      vertexArray[0].color = 0;

      for(int i=1; i<noOfVertices; i++)
      {
        for(int j=0; j<i; j++)
        {
          if((AdjacencyMatrix[i][j]>=1) && (vertexArray[j].color!=-1))
          {
            X[vertexArray[j].color] = true;
          }
        }

        
        for(cr=0; cr<noOfVertices; cr++)
        {
          if(X[cr]==false)
          {
            break;
          }
        }
        vertexArray[i].color = cr;

        for(int j=0; j<i; j++)
        {
          if((AdjacencyMatrix[i][j]>=1) && (vertexArray[j].color!=-1))
          {
            X[vertexArray[j].color] = false;
          }
        }

      }
      printf("--------------------------------------------------------------------------------------------------------\n");
      printf("\t\t\t\tCHROMATIC NUMBER\n");
      printf("--------------------------------------------------------------------------------------------------------\n");
      for(int i=0; i<noOfVertices; i++)
      {
        cout<<vertexArray[i].color<<", "<<vertexArray[i].nodeNum<<endl; 
      }

      cout<<"Chromatic Number: "<<cr+1<<endl;
}

void printLegend()
{
    printf("-------------------------------------------------LEGEND-------------------------------------------------\n");
    printf("NODE COLOURS\n");
    for(int i=0; i<X.getNoOfVertices(); i++)
    {
      printf("Node %d: %s\n", i+1, colorName[i]);
    }
    printf("\nEDGE COLOURS\n");
    printf("Green : Unidirectional\n\tUpwards\n\tLeft to Right\n");
    printf("Blue  : Unidirectional\n\tDownwards\n\tRight to Left\n");
    printf("Red   : Bidirectional\n");
    printf("\nBUTTONS\n");
    printf("BLACK:\n1 - Insert Node\n2 - Delete Node\n3 - Djk\n");
    printf("DARK GREY:\n1 - Insert Edge\n2 - Delete Edge\n3 - Chromatic number\n");
    printf("LIGHT GREY:\nProfiling\n");
    printf("--------------------------------------------------------------------------------------------------------\n");
    
}

int main(int argc, char *argv[])
{
    int n;
    char d,w;
    int dd,ww;
    
    //printf("ENTER NO OF NODES");
    scanf("%d\n", &n);
    
    //printf("IS IT DIRECTED");
    scanf("%c\n", &d);
    if(d=='Y')
    {
       dd=1;
    }
    else
    {
       dd=0;
    }
    
    //printf("IS IT WEIGHTED");
    scanf("%c\n", &w);
    if(w=='Y')
    {
       ww=1;
    }
    else
    {
       ww=0;
    }

    flagsInit(dd,ww);
    //printf("\n%d %d\n",flag.directed,flag.weighted);
    X.initGraph(n);
    printLegend();
    X.setMatrix();
    r=(int*)calloc(X.getNoOfVertices(), sizeof(int));
    X.setArray();
    //X.chromaticNumber();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(WIDTH, HEIGHT+BUTTON_HEIGHT);
    glutCreateWindow("Basics");
    glutDisplayFunc(Display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
