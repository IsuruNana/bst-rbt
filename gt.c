#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bst.h"
#include "gt.h"

typedef struct GTNODE GTNODE;

struct GTNODE
{
    int frequency;
    void *value;
    void (*display)(FILE *, void *);
    int (*comparator)(void *, void *); //Knows how to compare the value it stores
};

struct GT
{
    BST *gtBST;
    int words;
    void (*display)(FILE *, void *);
	  int (*comparator)(void *, void *);
};

//-------------------------Private Prototypes---------------------//

static GTNODE *findGTNODE(GT *tree, void *value);
static void displayGTNODE(FILE *fp, void *value); //Given to BST
static int compareGTNODE(void *node1, void *node2); //Given to BST
static void swapGTNODE(BSTNODE *node1, BSTNODE *node2);

GTNODE *findGTNODE(GT *tree, void *value)
{
  GTNODE *gtNode = malloc(sizeof(GTNODE));
  GTNODE *returnNode = NULL;

  gtNode->value = value;   //Need to wrap searching value in GTNODE
  gtNode->display = tree->display;
  gtNode->comparator = tree->comparator;

  BSTNODE *bstNODE = findBST(tree->gtBST, gtNode);

  if(bstNODE != NULL)
  {
    returnNode = getBSTNODE(bstNODE);
    free(gtNode);
    return returnNode;
  }

  return NULL;
}

void displayGTNODE(FILE *fp, void *value) //Given to BST
{
    GTNODE *gtNode = value;
    gtNode->display(fp, gtNode->value);
    if(gtNode->frequency > 1) fprintf(fp, "-%d", gtNode->frequency);
}

int compareGTNODE(void *node1, void *node2) //Given to BST
{
    GTNODE *nodeA = node1;
    GTNODE *nodeB = node2;
    int compare = nodeA->comparator(nodeA->value, nodeB->value);
    return compare;
}

void swapGTNODE(BSTNODE *node1, BSTNODE *node2)
{
  GTNODE *gt1 = getBSTNODE(node1);
  GTNODE *gt2 = getBSTNODE(node2);

  /* swap the keys stored in the GT value objects */
  GTNODE *vtemp = gt1;
  gt1 = gt2;
  gt2 = vtemp;

  setBSTNODE(node1, gt1);
  setBSTNODE(node2, gt2);
}

//------------------------End private functions--------------------//

//--------------------Beginning of public functions--------------//

GT *newGT(void (*d)(FILE *,void *), int (*c)(void *,void *))
{
    GT *gt = malloc(sizeof(GT));
    assert(gt != NULL);

    gt->display = d;         // Doesnt really use them. Stores them to pass
    gt->comparator = c;     // on to nodes in the future.

    gt->gtBST = newBST(displayGTNODE, compareGTNODE, swapGTNODE);
    gt->words =0;

    return gt;
}
void insertGT(GT *gt ,void *value)
{
  GTNODE *gtWrapperNode = findGTNODE(gt, value);

  if(gtWrapperNode != NULL) gtWrapperNode->frequency++;

  else
  {
    //printf("Not found need to insert\n");
    GTNODE *dataNode = malloc(sizeof(GTNODE));

    dataNode->value = value;
    dataNode->frequency = 1;
    dataNode->comparator = gt->comparator;
    dataNode->display = gt->display;

    insertBST(gt->gtBST, dataNode);
  }

  gt->words++;
}
int findGT(GT *gt ,void *value)
{
  GTNODE *gtNode = findGTNODE(gt, value);
  int freq = 0;

  if(gtNode != NULL) freq = gtNode->frequency;

  return freq;
}
void deleteGT(GT *gt ,void *value)
{
  GTNODE *gtWrapperNode = findGTNODE(gt, value);

  if(gtWrapperNode != NULL)
  {
    if(gtWrapperNode->frequency == 1) deleteBST(gt->gtBST, gtWrapperNode);
    else gtWrapperNode->frequency--;
    gt->words--;
  }

  else  //Not found
  {
    printf("Value ");
    gt->display(stdout, value);
    printf(" not found.\n");
  }
}
int sizeGT(GT *gt)
{
  int size = sizeBST(gt->gtBST);
  return size;
}
int wordsGT(GT *gt)
{
  int words = gt->words;
  return words;
}
void statisticsGT(FILE *fp, GT *gt)
{
  fprintf(fp, "Words/Phrases: %d\n", wordsGT(gt));
  statisticsBST(fp, gt->gtBST);
}
void displayGT(FILE *fp, GT *gt)
{
  displayBST(fp, gt->gtBST);
}

//--------------------End of public functions--------------//
