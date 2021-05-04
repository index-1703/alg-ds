#include <stdio.h>
#include <stdlib.h>
#pragma warning (disable:4996)

#define TRUE 1

typedef enum {
  NONE = 0,
  LEFT = 1,
  RIGHT = 2
}way_t;

typedef struct tree_t {
  int num;
  struct tree_t* left;
  struct tree_t* parent;
  struct tree_t* right;
} tree_t;

void AddNode(tree_t** tree, int num) {
  tree_t* temp = (*tree), *newnode, *ptr = *tree;

  newnode = (tree_t*)malloc(sizeof(tree_t));
  if (!newnode)
    return;
  newnode->left = NULL;
  newnode->right = NULL;
  newnode->parent = NULL;
  newnode->num = num;

  if (!(*tree)) {
    (*tree) = newnode;
    return;
  }

  while (ptr) {
    if (num < ptr->num) {
      if (ptr->left)
        ptr = ptr->left;
      else
        break;
    }
    else if (num > ptr->num) {
      if (ptr->right)
        ptr = ptr->right;
      else
        break;
    }
    else {
      free(newnode);
      return;
    }
  }

  if (num < ptr->num)
    ptr->left = newnode;
  else
    ptr->right = newnode;

  newnode->parent = ptr;
}

tree_t* FindNode(tree_t* tree, int num) {
  tree_t* temp = tree;
  while (temp) {
    if (num < temp->num)
      temp = temp->left;
    else if (num > temp->num)
      temp = temp->right;
    else
      return temp;
  }
  return NULL;
}

void DeleteNode(tree_t** tree, tree_t* vertex) {
  if (!vertex) return;
  int temp;
  tree_t* max=vertex, *parent, *child;
  way_t parentway;

  if (vertex->left == NULL && vertex->right == NULL) {
    if ((vertex == (*tree))) {
      free(vertex);
      (*tree) = NULL;
      return;
    }

    parent = vertex->parent;
    parentway = parent->left == vertex ? LEFT : RIGHT;

    if (parentway == LEFT)
      parent->left = NULL;
    else
      parent->right = NULL;

    free(vertex);
    return;
  }

  if ((vertex->left != NULL && vertex->right == NULL) || (vertex->left == NULL && vertex->right != NULL)) {

    if (vertex == (*tree)) {
      if (vertex->left)
        (*tree) = vertex->left;
      else
        (*tree) = vertex->right;
      (*tree)->parent = NULL;
      free(vertex);
      return;
    }

    parent = vertex->parent;
    parentway = parent->left == vertex ? LEFT : RIGHT;

    if (vertex->left) //only left child
      child = vertex->left;
    else //only right child
      child = vertex->right;

    child->parent = vertex->parent;

    if (parentway == LEFT)
      parent->left = child;
    else
      parent->right = child;

    free(vertex);
    return;
  }

  max = max->right;

  while (max->left)
    max = max->left;

  temp = max->num;
  max->num = vertex->num;
  vertex->num = temp;

  if (max->right) {
    child= max->right;
    parent = max->parent;
    parentway = parent->left == max ? LEFT : RIGHT;

    if (parentway == LEFT)
      parent->left = child;
    else
      parent->right = child;
    
    child->parent = parent;
  }

  else {
    parent = max->parent;
    parentway = parent->left == max ? LEFT : RIGHT;

    if (parentway == LEFT)
      parent->left = NULL;
    else
      parent->right = NULL;
  }
  free(max);
  return;
}

void DestroyTree(tree_t* tree) {
  if (tree) {
    DestroyTree(tree->left);
    DestroyTree(tree->right);
    free(tree);
  }
}

int main(void) {
  char c; 
  int num=0; 
  tree_t* tree = NULL;

  while (1) {
    c = fgetc(stdin);

    if (c == EOF || c == '\n')
      break;
    
    scanf("%i", &num);

    //cs
    if (c == 'a') //append
      AddNode(&tree, num);

    else if (c == 'f') {
      if (FindNode(tree, num))
        printf("yes\n");
      else
        printf("no\n");
    }
    else if (c == 'r')
      DeleteNode(&tree, FindNode(tree, num));

    num = 0;
    c = fgetc(stdin);
  }
  
  DestroyTree(tree);

  return 0;
}