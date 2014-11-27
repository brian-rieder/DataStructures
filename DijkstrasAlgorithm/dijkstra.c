#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FATAL(msg) {							\
    fprintf(stderr, "FATAL %s:%d %s\n", __FILE__, (int) __LINE__, msg); \
    exit(1);								\
  }

/********************************************************************
 * ECE368 Project 3
 *
 * Name: Brian Rieder
 * PUID: brieder
 *
 * Project Description:
 * Implement Dijksta's shortest path algorith for weighted, undirected
 * graphs. Two input files will be given. The first will represent a
 * map whose vertices are points on a plane and are connected by edges
 * whose weights are Euclidean distances. The second will contain a 
 * series of queries in the form of source-destination vertex pairs.
 *******************************************************************/

//----------------------------------------------Structure Definitions

typedef struct edgelist_t {
  int connectedto;
  struct edgelist_t * next;
} EdgeList;

typedef struct node_t {
  int value;
  unsigned long weight;
  int xcoord;
  int ycoord;
  EdgeList * edges;
  struct node_t * next;
} Node;

//-------------------------------------------------------Program Body

int readNumberFromFile(FILE * fp)
{
  int value = 0;
  //Put the file pointer in the right place
  char ch = fgetc(fp);
  while(ch == ' ' || ch == '\n')
    ch = fgetc(fp);
  //Go through the file and find the number
  while(ch != '\n' && ch != ' ') {
    value *= 10;
    value += ch - '0';
    ch = fgetc(fp);
  }
  return value;
}

Node * nodeConstructor(int nodeValue, int nodeXcoord, int nodeYcoord)
{
  Node * retnode = malloc(sizeof(Node));
  retnode->value = nodeValue;
  retnode->weight = 4294967295;
  retnode->xcoord = nodeXcoord;
  retnode->ycoord = nodeYcoord;
  retnode->edges = NULL;
  retnode->next = NULL;
  return retnode;
}

void edgeListInserter(Node * vertex, int edgeconnect)
{
  if(vertex->edges == NULL) {
    vertex->edges = malloc(sizeof(EdgeList));
    vertex->edges->connectedto = edgeconnect;
    vertex->edges->next = NULL;
    return;
  }
  EdgeList * tracker = vertex->edges;
  while(tracker->next != NULL)
    tracker = tracker->next;
  tracker->next = malloc(sizeof(EdgeList));
  tracker->next->connectedto = edgeconnect;
  tracker->next->next = NULL;
}

void insertConnection(Node * vertex, int firstnode, int secondnode)
{
  int flag1 = 1;
  int flag2 = 1;
  Node * itrNode = vertex;
  while(itrNode != NULL && (flag1 == 1 || flag2 == 1)) {
    if(itrNode->value == firstnode) {
      edgeListInserter(itrNode, secondnode);
      flag1 = 0;
    }
    if(itrNode->value == secondnode) {
      edgeListInserter(itrNode, firstnode);
      flag2 = 0;
    }
    itrNode = itrNode->next;
  }
}

int * readQueries(FILE * query, int numQueries)
{
  int i;
  int * retArr = malloc(sizeof(int) * 2 * numQueries);
  for(i = 0; i < (2 * numQueries); ++i) {
    retArr[i] = readNumberFromFile(query);
  }
  return retArr;
}

Node * findVertexByValue(int searchValue, Node * origin)
{
  while(origin != NULL) {
    if(origin->value == searchValue)
      return origin;
    origin = origin->next;
  }
  FATAL("Attempted to search for non-existent vertex!");
  return NULL;
}

int squareroot(int num)
{
  int val;
  if(num == 0)
    return 0;
  if(num == 1)
    return 1;
  int i;
  for(i = 1; i < num/2; ++i) {
    if(i * i > num)
      break;
    val = i;
  }
  return val;
}

int square(int num)
{
  return(num * num);
}

int distanceFormula(Node * node1, Node * node2)
{
  //d = sqrt((x2 - x1)^2 + (y2 - y1)^2)
  return(squareroot(square(node1->xcoord - node2->xcoord) + 
		    square(node1->ycoord - node2->ycoord)));
}

int main(int argc, char * * argv)
{
  //Verify that the correct number of arguments is given
  if(argc != 3)
    FATAL("Invalid number of command line arguments!");
  //Open the map file for reading
  FILE * map = fopen(argv[1], "r");
  //Read the number of vertices in the file
  int numVertices = readNumberFromFile(map);
  //Read the number of edges in the file
  int numEdges = readNumberFromFile(map);
  //Read vertice data and assign it to nodal linked list "vertex"
  int nodenum = readNumberFromFile(map);
  int nodexcoord = readNumberFromFile(map);
  int nodeycoord = readNumberFromFile(map);
  Node * vertex = nodeConstructor(nodenum, nodexcoord, nodeycoord);
  int i;
  Node * tmpitr = vertex;
  for(i = 1; i < numVertices; ++i) {
    nodenum = readNumberFromFile(map);
    nodexcoord = readNumberFromFile(map);
    nodeycoord = readNumberFromFile(map);
    tmpitr->next = nodeConstructor(nodenum, nodexcoord, nodeycoord);
    tmpitr = tmpitr->next;
  }
  //Read edge data from the file and assign it to respective lists
  for(i = 0; i < numEdges; ++i) {
    int firstnode = readNumberFromFile(map);
    int secondnode = readNumberFromFile(map);
    insertConnection(vertex, firstnode, secondnode);
  }
  fclose(map);
  //NOTE: At this point, the graph is complete, so begin algorithm
  //Open the query file for reading
  FILE * query = fopen(argv[2], "r");
  //Read the number of queries
  int numQueries = readNumberFromFile(query);
  //Read query data and store it for reference
  int * queryArr = readQueries(query, numQueries);
  //Store origin node in list
  Node * origin = vertex;
  //There may need to be a loop here
  //Find the starting point of the query
  i=0;
  int startVal = queryArr[i];
  int endVal = queryArr[i+1];
  Node * start = findVertexByValue(startVal, origin);


  return EXIT_SUCCESS;
}
