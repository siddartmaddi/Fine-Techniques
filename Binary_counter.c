#include <stdlib.h>
#include <stdio.h>
#include<math.h>
#include "assignment_2.h"
#define  print(int) printf("%d ", int)

typedef struct node
{ // a structure for each node
  int data;
  struct node *next;
}node;

typedef struct tab
{ // structure to store head pointer
  struct node *head;
}table;

static unsigned int Log2n(unsigned int n){ // function to compute log to the base 2
  if(n<=1)
    return 0;
  else if(n%2==0) // for even numbers
    return 1 + Log2n(n/2);
  else // for odd case
    return 1 + Log2n((n+1)/2);
}

static node* insert(int value){ // function to insert a node in the list
    if(value == -1)
        return NULL;
    node* n = (node*)malloc(sizeof(node));
    n->data = value;
    n->next = NULL;
    return n;
}

static node* merge(node* n1,node* n2){ // function to merge two lists into one by checking the head of the two lists
    node *p ;
    node* f;
    p = n2;
    while(p){
        print(p->data);
        p = p->next;
    }
    printf("x ");
    p=n1;
    while(p){
        print(p->data);
        p = p->next;

    }

    printf("= ");
    if(n1->data > n2-> data){ // to check if first list's head is greater than second list's head
        f = insert(n1->data);
        f->next= insert(n2->data); // insert the second lists head after the first list's head
        node* t = n1->next;
        print(n1->data);
        print(n2->data);
        p = f->next;
        while(t){
            p -> next = insert(t->data);
            print(t->data);
            t=t->next;
            p=p->next;
        }
    }
    else{ // to check if second lists head is greater than first list's head
        f = insert(n2->data);
        f->next= insert(n1->data); // insert the first lists head after second lists head
        node* t = n2->next;
        print(n2->data);
        print(n1->data);
        p = f->next;
        while(t){
            p->next = insert(t->data);
            print(t->data); // printing the value
            t=t->next;
            p=p->next;
        }
    }

    printf("\n");
    return f;
 }

void find_second_greatest(int *a,int length){
    a[length] = -1;
    int n=ceil(Log2n(length))+1; // to find the array size

    table* t=(table*)malloc(sizeof(table)*n);
    for (int i=0;i<n;i++){
        t[i].head=NULL;
    }
    int j = 0;
    for(int i = 0; i<length; i+=2){
        int min = -1;
        int max = -1;
        if(a[i] > a[i+1]){ // when two elements are chosen checks which is min and which is max
            min = a[i+1];
            max = a[i];
        }
        else{
            min = a[i];
            max = a[i+1];
        }
        if (min!=-1) // if min is -1 dont print
        {
          print(a[i]);
          printf("x ");
          print(a[i+1]);
          printf("= ");
          print(max);
          print(min); //don't print if min is -1
          printf("\n");
        }

        t[0].head = insert(max);
        t[0].head->next = insert(min);
        j = 1;
        while(t[j].head){
            t[j].head = merge(t[j-1].head, t[j].head); //merging the lists
            t[j-1].head = NULL;
            j+=1;
        }
        t[j].head = t[j-1].head;
        t[j-1].head = NULL;
    }

    int k=0;
    node *temp_list=NULL;
    while(k<n)
    {
      if (t[k].head!=NULL)
      {
        temp_list=t[k].head;
        break;
      }
      k++;
    }
    // to merge the lists after the numbers from the array are finished
    int index=k+1;
    while(index<n)
    {
      if (t[index].head!=NULL)
      {
        if (t[index].head ->data > temp_list->data)
        {
          t[index].head=merge(t[index].head,temp_list);
          temp_list=t[index].head;
        }
        else
        {
          temp_list=merge(temp_list,t[index].head);

        }
      }
      index++;
    }
    // finding the second  max in the last list
    temp_list=temp_list->next;
    int max=temp_list->data;
    while(temp_list!=NULL)
    {
      if (temp_list->data > max)
      {
        max=temp_list->data;
      }
      temp_list=temp_list->next;
    }
    printf("\n%d",max);
    free(temp_list);
}
