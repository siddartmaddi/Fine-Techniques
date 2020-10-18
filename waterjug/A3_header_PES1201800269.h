#include<stdio.h>
#include<stdlib.h>
// structure for the state
typedef struct state
{
  int jugA;
  int jugB;
  int jugC;
  int fn_index;// tells the index to choose the function(ranges from 0-5 in this case)
}state_t;
// structure for the doubly linked list
typedef struct node
{
  state_t state;
  struct node *next;
  struct node *prev;
}node_t;

typedef struct list
{
  node_t *head;
  node_t *tail;

}list_t;

void init_state(state_t *ptr_state); // initialise the state
void disp_state(const state_t *ptr_state); // display the state
void set_state(state_t *ptr_state,int jugA,int jugB,int jugC); // set the state to specific value
// functions to move water from one jar to another
void moveAB(const state_t *src ,state_t*dst,int capacityA,int capacityB,int capacityC);
void moveBA(const state_t *src ,state_t*dst,int capacityA,int capacityB,int capacityC);
void moveAC(const state_t *src ,state_t*dst,int capacityA,int capacityB,int capacityC);
void moveCA(const state_t *src ,state_t*dst,int capacityA,int capacityB,int capacityC);
void moveBC(const state_t *src ,state_t*dst,int capacityA,int capacityB,int capacityC);
void moveCB(const state_t *src ,state_t*dst,int capacityA,int capacityB,int capacityC);

int valid(const state_t*ptr_state,int capacityA,int capacityB,int capacityC,int total);
int is_same(const state_t*src1,const state_t*src2);

// for the doubly linked list
void init_list(list_t *ptr_list); // initialise the list
void disp_list(list_t *ptr_list); // print the list
void add_at_end(list_t *ptr_list ,const state_t *ptr_state);// add node at the end
void remove_at_end(list_t *ptr_list);// delete node from end


int is_repeated(const list_t *ptr_list,const state_t *ptr_state); // to check if repeated
int is_empty(list_t *list); // to check if list is empty
