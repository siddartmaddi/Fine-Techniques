#include<stdio.h>
#include<stdlib.h>
#include"assignment_4.h"

FILE* init_tree(const char* filename) // initializing the tree structure onto the file
{
  FILE *fp;
  tree_t tree;
  fp=fopen(filename,"r+");
  if (fp==NULL) // if file not found
  {
    fp=fopen(filename,"w");
    tree.free_head=-1; // init it is -1
    tree.root=-1;
    fwrite(&tree,sizeof(tree_t),1,fp); // writing the structure onto the file
    fclose(fp);
    fopen(filename,"r+");

  }
  // if present then just return file pointer
  fread(&tree,sizeof(tree_t),1,fp);
  return fp;
}

void close_tree(FILE *fp)
{
  fclose(fp); // for closing the file
}

void inorder(node_t *node,int value,FILE *fp) // recursive function for inorder
{
  node_t node1;
  node_t node2;
  int first;
  int second;
  if (node->left_offset !=-1 || node->right_offset!=-1) // when one or both children present
  {
    first=node->left_offset;
    if (first!=-1)
    {
      fseek(fp,first,SEEK_SET);
      fread(&node1,sizeof(node_t),1,fp);
      inorder(&node1,first,fp);
    }
    printf("%d ",node->key);
    second=node->right_offset;
    if (second!=-1)
    {
      fseek(fp,second,SEEK_SET);
      fread(&node2,sizeof(node_t),1,fp);
      inorder(&node2,second,fp);
    }
  }
  else // when no child then print parent
  {
    printf("%d ",node->key);
    return;
  }

}
void display_inorder(FILE *fp)
{
  // inorder : print left childen then parent then right child
  tree_t tree;
  fseek(fp,0,SEEK_SET);
  fread(&tree,sizeof(tree_t),1,fp);
  node_t pres_node;
  int pres=tree.root;
  if (pres!=-1)
  {
    fseek(fp,pres,SEEK_SET);
    fread(&pres_node,sizeof(node_t),1,fp);
    inorder(&pres_node,pres,fp); // calling the recursive function
  }
  printf("\n");


}

void preorder(node_t *node,int value,FILE *fp) // recursive function for preorder
{
  node_t node1;
  node_t node2;
  int first;
  int second;
  if (node->left_offset !=-1 || node->right_offset!=-1) // when one or both children present
  {
    first=node->left_offset;
    printf("%d ",node->key);
    if (first!=-1)
    {
      fseek(fp,first,SEEK_SET);
      fread(&node1,sizeof(node_t),1,fp);
      preorder(&node1,first,fp);
    }
    second=node->right_offset;
    if(second!=-1)
    {
      fseek(fp,second,SEEK_SET);
      fread(&node2,sizeof(node_t),1,fp);
      preorder(&node2,second,fp);
    }

  }
  else // when no children
  {
    printf("%d ",node->key);
    return;

  }
}

void display_preorder(FILE *fp)
{
  // preorder : print parent then left child then right child
  tree_t tree;
  fseek(fp,0,SEEK_SET);
  fread(&tree,sizeof(tree_t),1,fp);
  node_t pres_node;
  int pres=tree.root;
  if (pres!=-1)
  {
    fseek(fp,pres,SEEK_SET);
    fread(&pres_node,sizeof(node_t),1,fp);
    preorder(&pres_node,pres,fp); // calling the recursive function for preorder
  }
  printf("\n");


}

// insert the key into the tree in the file
// if the key already exists just return
void insert_key(int key, FILE *fp)
{
  tree_t ptr_tree;
  fseek(fp,0,SEEK_SET);
  fread(&ptr_tree,sizeof(tree_t),1,fp);// reading the tree

  int temp;
  node_t temp_node;
  node_t curr_node;
  // to check if any free spaces in between
  if (ptr_tree.free_head==-1) // no free spaces in between so insert in the end
  {
    fseek(fp,0,SEEK_END);
    temp=ftell(fp);
    temp_node.key=key;
  }
  else // free spaces present in middle
  {
    temp=ptr_tree.free_head;
    fseek(fp,temp,SEEK_SET);
    fread(&temp_node,sizeof(node_t),1,fp);
    ptr_tree.free_head=temp_node.left_offset;
    temp_node.key=key;
  }
  temp_node.left_offset=-1;
  temp_node.right_offset=-1;
  int val=ptr_tree.root;
  if (val==-1) // no nodes present
  {
    ptr_tree.root=temp;
    fseek(fp,0,SEEK_SET);
    fwrite(&ptr_tree,sizeof(tree_t),1,fp);
    fseek(fp,temp,SEEK_SET);
    fwrite(&temp_node,sizeof(node_t),1,fp);
  }
  else //to find the position of the node
  {
    int prev=-1;
    node_t pres_node,prev_node;
    int pres=ptr_tree.root;

    fseek(fp,pres,SEEK_SET);
    fread(&pres_node,sizeof(node_t),1,fp);
    while(pres!=-1)
    {
      if (pres_node.key==key)
      {
        return ;
      }
      else
      {
        while(pres_node.key > key && pres!=-1) // if child is less than parent add it to the left
        {
          prev=pres;
          prev_node=pres_node;
          pres=pres_node.left_offset;
          if (pres!=-1)
          {
            fseek(fp,pres,SEEK_SET);
            fread(&pres_node,sizeof(node_t),1,fp);
          }
        }
        while(pres_node.key < key && pres!=-1) // if child is greater than parent add it to right
        {
          prev=pres;
          prev_node=pres_node;
          pres=pres_node.right_offset;
          if (pres!=-1)
          {
            fseek(fp,pres,SEEK_SET);
            fread(&pres_node,sizeof(node_t),1,fp);
          }
         }
      }
    }
  if (prev!=-1)
  {
    if (key > prev_node.key)
    {
      prev_node.right_offset=temp;
      fseek(fp,prev,SEEK_SET);
      fwrite(&prev_node,sizeof(node_t),1,fp);
    }
    else
    {
      prev_node.left_offset=temp;
      fseek(fp,prev,SEEK_SET);
      fwrite(&prev_node,sizeof(node_t),1,fp);
    }

  }
  else // if adding to the root
  {
    ptr_tree.root=temp;
  }
  fseek(fp,0,SEEK_SET);
  fwrite(&ptr_tree,sizeof(tree_t),1,fp); // writing back the tree
  fseek(fp,temp,SEEK_SET);
  fwrite(&temp_node,sizeof(node_t),1,fp);
  }
}

// delete the key from the tree in the file
// the key may or may not exist
void delete_key(int key, FILE *fp)
{
  tree_t tree;
  fseek(fp,0,SEEK_SET);
  fread(&tree,sizeof(tree_t),1,fp);
  if (tree.root==-1) // no node in the tree
  {
    return;
  }
  node_t pres_node;
  node_t prev_node;
  int pres;
  int prev=-1;
  pres=tree.root;
  fseek(fp,pres,SEEK_SET);
  fread(&pres_node,sizeof(node_t),1,fp);
  while(key!=pres_node.key && pres!=-1) // to find where the node with key is present
  {
    if (key > pres_node.key) // in BST child node with key greater than parent is placed at right of parent
    {
      prev=pres;
      prev_node=pres_node;
      pres=pres_node.right_offset;
      if (pres!=-1)
      {
        fseek(fp,pres,SEEK_SET);
        fread(&pres_node,sizeof(node_t),1,fp);
      }
    }
    else // if child node is smaller than parent
    {
      prev=pres;
      prev_node=pres_node;
      pres=pres_node.left_offset;
      if (pres!=-1)
      {
        fseek(fp,pres,SEEK_SET);
        fread(&pres_node,sizeof(node_t),1,fp);
      }
    }
  }
  if (key==pres_node.key) // if key is found
  {
    if (pres_node.left_offset==-1 && pres_node.right_offset==-1) // case1: no child nodes present
    {
      if (prev==-1) // root element
      {
        tree.free_head=tree.root;
        tree.root=-1;
      }
      else
      {
        if (key < prev_node.key) // if req node is left child
        {
          prev_node.left_offset=-1;
        }
        else if (key > prev_node.key) // if required node is right child
        {
          prev_node.right_offset=-1;
        }
        pres_node.left_offset=tree.free_head; // add to the free_head
        tree.free_head=pres;
        fseek(fp,prev,SEEK_SET);
        fwrite(&prev_node,sizeof(node_t),1,fp); // update node
        fseek(fp,pres,SEEK_SET);
        fwrite(&pres_node,sizeof(node_t),1,fp);

      }
      fseek(fp,0,SEEK_SET);
      fwrite(&tree,sizeof(tree_t),1,fp); //update the tree

    }
    else if (pres_node.left_offset==-1) // case2 : right child node present
    {
      if (prev==-1)
      {
        tree.free_head=tree.root;
        tree.root=pres_node.right_offset;
      }
      else
      {
        if (key < prev_node.key) // left child
        {
          prev_node.left_offset=pres_node.right_offset;
        }
        else if (key > prev_node.key) // right child
        {
          prev_node.right_offset=pres_node.right_offset;
        }
        pres_node.left_offset=tree.free_head;
        tree.free_head=pres;
        fseek(fp,prev,SEEK_SET);
        fwrite(&prev_node,sizeof(node_t),1,fp);
        fseek(fp,pres,SEEK_SET);
        fwrite(&pres_node,sizeof(node_t),1,fp);
      }
      fseek(fp,0,SEEK_SET);
      fwrite(&tree,sizeof(tree_t),1,fp); // update

    }
    else if (pres_node.right_offset==-1)// csae3: left child node present
    {
      if (prev==-1)
      {
        tree.free_head=tree.root;
        tree.root=pres_node.left_offset;
      }
      else
      {
        if (key < prev_node.key) //left child
        {
          prev_node.left_offset=pres_node.left_offset;
        }
        else if (key > prev_node.key) // right child
        {
          prev_node.right_offset=pres_node.left_offset;
        }
        pres_node.left_offset=tree.free_head;
        tree.free_head=pres;
        fseek(fp,prev,SEEK_SET);
        fwrite(&prev_node,sizeof(node_t),1,fp);
        fseek(fp,pres,SEEK_SET);
        fwrite(&pres_node,sizeof(node_t),1,fp);
      }
      fseek(fp,0,SEEK_SET);
      fwrite(&tree,sizeof(tree_t),1,fp); //update tree
    }
    else // both children present;
    {
      node_t p_node;
      node_t temp_node;
      int temp;
      int p=-1;
      temp=pres_node.right_offset; // move right once
      fseek(fp,temp,SEEK_SET);
      fread(&temp_node,sizeof(node_t),1,fp);
      // finding inorder successor
      while(temp_node.left_offset!=-1) // go till the left most child which is the inorder successor
      {
        p=temp;
        p_node=temp_node;
        temp=temp_node.left_offset;
        fseek(fp,temp,SEEK_SET);
        fread(&temp_node,sizeof(node_t),1,fp);

      }
      pres_node.key=temp_node.key; // updating the value from inorder successor then deleting inorder successor
      if (p==-1)
      {
        pres_node.right_offset=temp_node.right_offset;
      }
      else
      {
        p_node.left_offset=temp_node.right_offset;
      }
      temp_node.left_offset=tree.free_head;
      tree.free_head=temp;
      if (p!=-1)
      {
        fseek(fp,p,SEEK_SET);
        fwrite(&p_node,sizeof(node_t),1,fp);
      }
      fseek(fp,temp,SEEK_SET); // updating
      fwrite(&temp_node,sizeof(node_t),1,fp);
      fseek(fp,0,SEEK_SET);
      fwrite(&tree,sizeof(tree_t),1,fp);
      fseek(fp,pres,SEEK_SET);
      fwrite(&pres_node,sizeof(node_t),1,fp);

    }

  }
  else
  {
    // when no node with key found
  }
}
