#include<stdio.h>
#include<stdlib.h>
#include"A3_header_PES1201800269.h"

int main()
{

  int A;
  int B;
  int C;
  scanf("%d %d %d ",&A,&B,&C);
  int capacityA=A;
  int capacityB=B;
  int capacityC=C;

  int X;
  int Y;
  int Z;
  scanf("%d %d %d ",&X,&Y,&Z);

  int total = X+Y+Z;
  state_t start;
  state_t end;

  int P;
  int Q;
  int R;
  scanf("%d %d %d ",&P,&Q,&R);


  // array of pointers to functions
  void (*move[])( const state_t*,state_t*,int ,int ,int )={
    moveAB,
    moveAC,
    moveBA,
    moveBC,
    moveCA,
    moveCB,
  };
  // initialise and set the beginning state
  init_state(&start);
  init_state(&end);
  set_state(&start,X,Y,Z);
  set_state(&end,P,Q,R);
  list_t mylist;
  // to find the number of solutions
  int number_of_solutions=0;
  init_list(&mylist);
  int j=0;
  // to check validity of first and end state
  if (!valid(&start,capacityA,capacityB,capacityC,total) || !valid(&end,capacityA,capacityB,capacityC,total))
  {
    //invalid state
    j=1;
  }
  int i=0;
  state_t middle_state;

  add_at_end(&mylist,&start); // add the start state
  while(!is_empty(&mylist) && !j) // till the list is empty
  {

    i=mylist.tail->state.fn_index; // fetch the index of the function
    move[i](&mylist.tail->state,&middle_state,capacityA,capacityB,capacityC); // make the move

    if (!is_repeated(&mylist,&middle_state) &&  valid(&middle_state,capacityA,capacityB,capacityC,total)) // if next state is not repeated and is valid
    {
      add_at_end(&mylist,&middle_state); // add the state to the list
      if (is_same(&middle_state,&end)) // if the state matches with the end state
      {
        disp_list(&mylist); // display the list
        number_of_solutions++; // increase the number of solutions as a path is detected
      }
    }
    else
    {
      i=(mylist.tail->state.fn_index+1)%6; //increase the index
      while(!is_empty(&mylist) && (++mylist.tail->state.fn_index == 6))
      {
        remove_at_end(&mylist); // remove the state for backtracking
      }
    }


  }
  printf("%d\n",number_of_solutions);// print number of solutions











}
