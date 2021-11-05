#ifndef TREE_H_
#define TREE_H_

#include<stdbool.h>

#define WLEN 40

struct word{
    char value[WLEN];
    unsigned rating;
};
typedef struct word Item;

struct node{
    Item item;
    struct node *left;
    struct node *right;
};

typedef struct node Node;
typedef Node * Tree;

void InitializeTree(Tree *p_tree);
bool TreeIsEmpty(Tree * tree);
bool TreeIsFull(void);
unsigned TreeCount(Tree * tree);
/* initialized tree,p_item 
    to add a node with *p_item to tree.
return false if p_item=NULL or failed to create a node or item exist */
bool AddItem(Tree * tree,Item * p_item);//to be continued
void FreeTree(Tree * p_tree);//to be continued
/* initialized tree
    to find a item by toright()and toleft().
return p_item,NULL if not found */
Item * SeekItem(Tree * p_tree,Item * p_item);
/* initialized tree

 */
void Traverse(Tree * p_tree,void(*func)(Item * item));//done
/* initialized tree, p_item!=NULL

 */
bool DeleteItem(Tree * p_tree,Item *item);

#endif 