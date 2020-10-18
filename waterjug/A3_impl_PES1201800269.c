#include<stdio.h>
#include<stdlib.h>
#include"A3_header_PES1201800269.h"


// STATES
void init_state(state_t *ptr_state)
{
  ptr_state->jugA=0;// initialy zero
  ptr_state->jugB=0;
  ptr_state->jugC=0;
  ptr_state->fn_index=0;
}
void disp_state(const state_t *ptr_state)
{
  printf("%d %d %d ",ptr_state->jugA,ptr_state->jugB,ptr_state->jugC); // display the state
  printf("\n");
}
void set_state(state_t *ptr_state,int jugA,int jugB,int jugC) // set the state to the specific values
{
  ptr_state->jugA=jugA;
  ptr_state->jugB=jugB;
  ptr_state->jugC=jugC;
  ptr_state->fn_index=0;
}

void moveAB(const state_t *src,state_t*dst,int capacityA,int capacityB,int capacityC) // move water from A-> B
{
  int amount=capacityB - src->jugB; // free in B jug

  if (src->jugA - amount >=0)//  if A has more than the free space in B
  {
    dst->jugA = src->jugA - amount;
    dst->jugB = src->jugB + amount;
  }
  else // if A has less than the free space in B
  {
    dst->jugA = 0;
    dst->jugB = src->jugB + src->jugA;
  }
  dst->jugC=src->jugC;
  dst->fn_index=0;


}
void moveBA(const state_t *src,state_t*dst,int capacityA,int capacityB,int capacityC) // move water from B->A
{
  int amount=capacityA - src->jugA;

  if (src->jugB - amount >=0)
  {
    dst->jugB = src->jugB - amount;
    dst->jugA = src->jugA + amount;
  }
  else
  {
    dst->jugB = 0;
    dst->jugA = src->jugB + src->jugA;
  }
  dst->jugC=src->jugC;
  dst->fn_index=0;

}
void moveAC(const  state_t *src,state_t*dst,int capacityA,int capacityB,int capacityC) // move water from A->C
{
  int amount=capacityC - src->jugC;

  if (src->jugA - amount >=0)
  {
    dst->jugA = src->jugA - amount;
    dst->jugC = src->jugC + amount;
  }
  else
  {
    dst->jugA = 0;
    dst->jugC = src->jugA + src->jugC;
  }
  dst->jugB=src->jugB;
  dst->fn_index=0;

}
void moveCA(const state_t *src,state_t*dst,int capacityA,int capacityB,int capacityC) // move water from C->A
{
  int amount=capacityA - src->jugA;

  if (src->jugC - amount >=0)
  {
    dst->jugC = src->jugC - amount;
    dst->jugA = src->jugA + amount;
  }
  else
  {
    dst->jugC = 0;
    dst->jugA = src->jugA + src->jugC;
  }
  dst->jugB=src->jugB;
  dst->fn_index=0;

}
void moveBC(const state_t *src,state_t*dst,int capacityA,int capacityB,int capacityC) // move water from B-> C
{
  int amount=capacityC - src->jugC;

  if (src->jugB - amount >=0)
  {
    dst->jugB = src->jugB - amount;
    dst->jugC = src->jugC + amount;
  }
  else
  {
    dst->jugB = 0;
    dst->jugC = src->jugC + src->jugB;
  }
  dst->jugA=src->jugA;
  dst->fn_index=0;

}
void moveCB(const state_t *src,state_t*dst,int capacityA,int capacityB,int capacityC) // move water from C->B
{
  int amount=capacityB - src->jugB;

  if (src->jugC - amount >=0)
  {
    dst->jugC = src->jugC - amount;
    dst->jugB = src->jugB + amount;
  }
  else
  {
    dst->jugC = 0;
    dst->jugB = src->jugC + src->jugB;
  }
  dst->jugA=src->jugA;
  dst->fn_index=0;

}
// to check the validity of the state
int valid(const state_t*ptr_state,int capacityA,int capacityB,int capacityC,int total)
{
  return ((ptr_state->jugA>=0 && ptr_state->jugA<=capacityA) && (ptr_state->jugB>=0 && ptr_state->jugB<=capacityB)
   && (ptr_state->jugC>=0 && ptr_state->jugC<=capacityC) && (ptr_state->jugA+ptr_state->jugB+ptr_state->jugC==total));
}
// to check if two states are same or not
int is_same(const state_t*src1,const state_t*src2)
{
  return ((src1->jugA==src2->jugA) && (src1->jugB == src2->jugB) && (src1->jugC== src2->jugC));
}

// LISTS for doubly linked list

void init_list(list_t *ptr_list)
{
  ptr_list->head=NULL;
  ptr_list->tail=NULL;
}
// to display the list
void disp_list(list_t *ptr_list)
{
  node_t *temp=ptr_list->head;
  while(temp)
  {
    disp_state(&temp->state);
    temp=temp->next;
  }
  printf("\n");
}
// add the node at end
void add_at_end(list_t *ptr_list ,const state_t *ptr_state)
{
  node_t *temp=(node_t*)malloc(sizeof(node_t));

  temp->state=*ptr_state;
  if (ptr_list->head==NULL) // to check if list is empty
  {
    temp->next = NULL;
		temp->prev = ptr_list->tail;
		ptr_list->head = temp; // change head
		ptr_list->tail = temp; // change tail

  }
  else // if list is not empty
  {
    temp->next = NULL;
		temp->prev = ptr_list->tail;
		ptr_list->tail->next = temp;
		ptr_list->tail = temp;
  }

}
// delete from end
void remove_at_end(list_t *ptr_list)
{
  if(ptr_list->head == NULL) // if list is empty already
	{

	}
	else if(ptr_list->head == ptr_list->tail)// if one node is there
	{
		node_t* temp = ptr_list->tail;
		ptr_list->tail = temp->prev;
		ptr_list->head = NULL;
		free(temp);
	}
	else
	{
		node_t* temp = ptr_list->tail;
		ptr_list->tail = temp->prev;
		ptr_list->tail->next = NULL;
		free(temp);
	}
}
// to check if it is repeated
int is_repeated(const list_t *ptr_list,const state_t *ptr_state)
{
  node_t* temp = ptr_list->head;
	while(temp)
	{
		if (is_same(&temp->state, ptr_state))
    {
      return 1;
    }
    else
    {
      temp = temp->next;
    }
	}
	return 0;
}
// to check if list is empty or not
int is_empty(list_t *ptr_list)
{
  if (ptr_list->head!=NULL)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}
