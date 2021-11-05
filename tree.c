#include"tree.h"
#include<string.h>
#include<stdlib.h>

/* *p_node not empty
    to judge the node with *p_item if should be the left branch of the node
return true if it should be */
static bool ToLeft(Node * p_node,Item *p_item);

/* p_node not empty
    to judge the node with *p_item if should be the right branch of the node
return true if it should be */
static bool ToRight(Node * p_node,Item *p_item);

/* item not in the tree
    to find a proper father_node for a node with *p_item
return &father_node to be revised or NULL if it existed */
static Node * SeekNodeL(Tree *p_tree,Item *p_item);

/* tree not empty
    to find a node with *p_item.value
return &node(branch) or NULL if not found */
static Tree SeekNodeS(Tree *p_tree,Item *p_item);

/* tree not empty
    to find a branch referring the node with *p_item.value
return &branch or NULL if not found */
static Tree * SeekBranch(Tree *p_tree,Item *p_item);

/* 
    CountItem() support function
 */
static void Count(Tree * tree);
/* 
    CountItem() support variable
 */
static unsigned count=0;

void InitializeTree(Tree *p_tree)
{
    *p_tree=NULL;
}

bool TreeIsFull(void)
{
    Tree temp;
    temp=(Tree)malloc(sizeof(Node));
    if(temp==NULL)
    {
        free(temp); 
        return true;
    }
    return false;
}

bool TreeIsEmpty(Tree * p_tree)
{
    if(*p_tree==NULL)
        return true;
    return false;
}

bool AddItem(Tree * tree,Item * item)
{
    //return false if item=NULL
    if(item==NULL)
        return false;
    //create a node
    Tree p_node=(Tree)malloc(sizeof(Node));
    if(p_node==NULL)
        return false;
    //initialize node
    p_node->item=*item;
    p_node->left=NULL;
    p_node->right=NULL;
    //find a proper location for node
    if(TreeIsEmpty(tree)==true)
    {
        *tree=p_node;
        return true;
    }
    else
    {
        Tree p_father_node;
        p_father_node=SeekNodeL(tree,&(p_node->item));
        if(p_father_node!=NULL)
        {
            if(ToLeft(p_father_node,&(p_node->item))==true)
                p_father_node->left=p_node;
            else
                p_father_node->right=p_node;
            return true;
        }
        else
            return false;
    }
}

void Count(Tree * p_tree)
{
    if(*p_tree!=NULL)
    {
        count++;
        Count(&((*p_tree)->left));
        Count(&((*p_tree)->right));
    }
}
unsigned TreeCount(Tree * p_tree)
{
    count=0;
    Count(p_tree);

    return count;
}

Item * SeekItem(Tree * p_tree,Item * p_item)
{
    return &((SeekNodeS(p_tree,p_item))->item);
}

void FreeTree(Tree *p_tree)
{
    if(*p_tree!=NULL)
    {
        if((*p_tree)->left!=NULL)
            FreeTree(&((*p_tree)->left));
        if((*p_tree)->right!=NULL)
            FreeTree(&((*p_tree)->right));

        free(*p_tree);
        *p_tree=NULL;
    }
}

void Traverse(Tree * p_tree,void(*func)(Item * item))
{
    if(*p_tree!=NULL)
    {
        func(&((*p_tree)->item));
        Traverse(&((*p_tree)->left),func);
        Traverse(&((*p_tree)->right),func);
    }
}

bool DeleteItem(Tree * p_tree,Item *p_item)
{
    Tree tree=*p_tree;
    if(strcmp(tree->item.value,(*p_item).value)!=0)//root isn't todo
    {
        Tree *p_todo=SeekBranch(p_tree,p_item);
        Tree todo;
        if(p_todo!=NULL)
            todo=*p_todo;
        else
        {
            printf("not found.\n");
            return false;
        }
        Tree left=todo->left;
        Tree right=todo->right;
        Tree temp=todo;
        *p_todo=NULL;//exclude itself in order not to disturb SeekNodeL and it'll complete delete mission1
        if(left!=NULL)
        {
            Tree father_branch=SeekNodeL(p_tree,&(left->item));
            if(father_branch==NULL)
            {
                *p_todo=temp;
                return false;
            }
            else
                if(ToLeft(father_branch,&(left->item))==true)
                    father_branch->left=left;
                else//it must be the right
                    father_branch->right=left;
        }
        if(right!=NULL)
        {
            Tree father_branch=SeekNodeL(p_tree,&(right->item));
            if(father_branch==NULL)
            {
                *p_todo=temp;
                return false;
            }
            else
                if(ToLeft(father_branch,&(right->item))==true)
                    father_branch->left=right;
                else//it must be the right
                    father_branch->right=right;
        }
        free(todo);
        return true;
        
    }
    else//root is todo
    {
        Tree *p_todo=p_tree;
        Tree todo=tree;
        Tree left=tree->left;
        Tree right=tree->right;
        if(left==NULL&&right==NULL)
        {
            free(todo);
            *p_todo=NULL;
            return true;
        }
        else if(left==NULL&&right!=NULL)
        {
            *p_todo=right;
            free(todo);
            return true;
        }
        else if(left!=NULL&&right==NULL)
        {
            *p_todo=left;
            free(todo);
            return true;
        }
        else if(left!=NULL&&right!=NULL)//right becomes root
        {
            Tree father_branch;
            *p_todo=right;//has excluded itself
            father_branch=SeekNodeL(p_tree,&(left->item));
            if(father_branch!=NULL)
            {
                if(ToLeft(tree,&(left->item))==true)
                    father_branch->left=left;
                else
                    father_branch->right=left;
                
                free(todo);
                return true;
            }
            else
            {
                *p_todo=todo;
                return false;
            }
        }
    }
}


static bool ToLeft(Node * p_node,Item *p_item)
{
    if(strcmp(p_item->value,p_node->item.value)<0)
        return true;
    return false;
}

static bool ToRight(Node * p_node,Item *p_item)
{
    if(strcmp(p_item->value,p_node->item.value)>0)
        return true;
    return false;
}

static Node * SeekNodeL(Tree *p_tree,Item *p_item)
{
    Tree tree=*p_tree;
    if(SeekItem(p_tree,p_item)==NULL)
    {
        if(tree->left==NULL&&tree->right!=NULL)
            if(ToLeft(tree,p_item)==true)
                return tree;
            else
                SeekNodeL(&(tree->right),p_item);
        else if(tree->left!=NULL&&tree->right==NULL)
            if(ToLeft(tree,p_item)==true)
                SeekNodeL(&(tree->left),p_item);
            else
                return tree;
        else if(tree->left!=NULL&&tree->right!=NULL)
            if(ToLeft(tree,p_item)==true)
                SeekNodeL(&(tree->left),p_item);
            else
                SeekNodeL(&(tree->right),p_item);
        else if(tree->left==NULL&&tree->right==NULL)
            return tree;
    }
    else
        return NULL;
}

static Tree SeekNodeS(Tree *p_tree,Item * p_item)
{
    if(TreeIsEmpty(p_tree)==true)
        return NULL;
    else
    {
        Tree tree=*p_tree;
        if(ToLeft(tree,p_item)==true)
            SeekNodeS(&(tree->left),p_item);
        else
            if(ToRight(tree,p_item)==true)
                SeekNodeS(&(tree->right),p_item);
            else
                return *p_tree;
    }
}

static Tree * SeekBranch(Tree *p_tree,Item *p_item)
{
    if(TreeIsEmpty(p_tree)==true)
        return NULL;
    else
    {
        Tree tree=*p_tree;
        if(ToLeft(tree,p_item)==true)
            SeekBranch(&(tree->left),p_item);
        else
            if(ToRight(tree,p_item)==true)
                SeekBranch(&(tree->right),p_item);
            else
                return p_tree;//p_branch which found
    }
}
