
#include <stdio.h>
#include <stdlib.h>
#pragma pack(2)
//#include "assignment_1.h"
#define BLOCK_SIZE sizeof(block_) // size of the bookkeeping  structure

char *p = NULL;
static int SIZE = 0;

typedef union block_ // union used for aligning the bookkeeping structure to specific bytes
{
	struct
	{
		char status_; // '0' for free and '1' for allocated
		int size_; // size allocated
		int next_; // index of next structure
	}data_;
	char ALIGN[10]; // to align the size to 10 bytes
}block_;

void allocate(int n)
{
	block_ *temp;
	p = (char*)malloc(sizeof(char)*n);  // allocating memory to p array
	if(p!=NULL) // initialization
	{
		temp = (block_*)p;

		temp->data_.status_ = '0'; //'0' is free
		temp->data_.size_ = n - BLOCK_SIZE; // size left
		temp->data_.next_ = -1; // last node

		SIZE = n; // size of the whole array
	}
	else
	{
		SIZE = 0;
	}
}

void *mymalloc(int size){


	// WORST FIT ALGORITHM
	void *return_address = NULL; // contains the ADDRESS of the free node
	if(SIZE == 0){
		return return_address;
	}
	int i=0;
	block_ *temp1;
	int max=-1; // max contains the max free space for the worst case
	int j=0;


	// FINDING THE LARGEST FREE BLOCK AND POINTING TO IT
	while(i<SIZE){ // run through the loop to find the maximum free space
		temp1=(block_*)(&p[i]);
		if(max < temp1->data_.size_ && temp1->data_.status_ == '0'){ // to check if size of curr block is not allocated and its size is greater than max size
			max = temp1->data_.size_;
			j=i;
		}
		i+=(temp1->data_.size_+BLOCK_SIZE); // increment to the next block and check the condition again

	}
	block_* temp=(block_*)(&p[j]);
	// allocating memory to the block with maximum free space available (checking if size available can accomodate struct and req size )
	if(temp->data_.size_ > size+BLOCK_SIZE && temp->data_.status_ == '0'){
		return_address=(void*)(&p[j+BLOCK_SIZE]);
		block_ *next = (block_*)(&p[j+size+BLOCK_SIZE]);
		next->data_.status_ = '0'; // as it is free
		next->data_.size_ = temp->data_.size_ - BLOCK_SIZE - size;   // as this is the size left after allocating
		next->data_.next_ = temp->data_.next_;
		temp->data_.status_='1'; // it is the allocated memory
		temp->data_.next_ = j+size+BLOCK_SIZE;
		temp->data_.size_ = size;
		return return_address;
	}
	else if(temp->data_.size_ <=size+BLOCK_SIZE && temp->data_.status_ == '0' && temp->data_.size_ >=size){ // to check if some small free space is available
		return_address=(void*)(&p[j+BLOCK_SIZE]);
		temp->data_.status_='1'; // making it allocated
		return return_address;
	}
	else{
		return return_address;
	}
}


void myfree(void *b)
{
	if(  b == NULL || SIZE == 0  )
	{
		return ;
	}
	int n;
	int i=0;
	// initialization of the two pointers to loop through
	block_ *curr;
	block_ *prev;

	curr = (block_*)(&p[0]);
	n = SIZE;
	while(i<n)
	{
		prev = curr;
		curr = (block_*)(&p[i]);
		i = i + BLOCK_SIZE;
		if(curr->data_.status_ == '0')
		{
			i = i + curr->data_.size_;
		}
		else if(b == (void*)(&p[i]))  // finding the given pointer while iterating through the loop
		{
			block_ *next;
			i = i + curr->data_.size_;
			next = (block_*)(&p[i]);
			i = i + BLOCK_SIZE;
			if(next->data_.status_ == '0') // checking if the next block is also free then we can merge it into one free block
			{
				curr->data_.size_ += BLOCK_SIZE + next->data_.size_;
				curr->data_.next_ = next->data_.next_;
			}
			if(prev != curr) //	prev block is not first, and is free
			{
				if(prev->data_.status_ == '0')
				{
					prev->data_.size_ += BLOCK_SIZE + curr->data_.size_;
					prev->data_.next_ = curr->data_.next_;
				}
			}
			curr->data_.status_ = '0'; // free marked
			return ;
		}
	}
}

void print_book()
{
	printf("%lu\n%lu\n\n", sizeof(block_), sizeof(block_));
}
// DISPLAYING THE MAP FOR MEMORY MANAGEMENT
void display_mem_map()
{

	block_ *curr;
	// to check if nothing is curr
	if(SIZE == 0)
	{
		return ;
	}
	int start,size,n,status,i=0;
	n = SIZE;
	curr = (block_*)(&p[0]); // pointing to the starting block
	printf("start\tsize\tstatus\n" );
	while(i<n && i != -1)
	{
		curr = (block_*)(&p[i]);

		if(curr->data_.size_ != 0)
			{
				size = BLOCK_SIZE;
			start = i;
			printf("%d\t%d\tBook\n", start, size);
			i = i + size;

			start = i;
			size = curr->data_.size_;
			printf("%d\t%d\t", start, size);
			if(curr->data_.status_ == '0') // for checking free space
			{
				printf("Free\n");
			}
			else // printing the allocated memory
			{
				printf("Allocated\n");
			}
		}
		i = curr->data_.next_; // to iterate through the loop
	}
	printf("\n");
}

int main(){
	printf("vuh");
	allocate(1000);
	display_mem_map()
	int *a=mymalloc(500);
	display_mem_map()
	int *b=mymalloc(200);
	display_mem_map()
	int *c=mymalloc(300);
	display_mem_map()
	//myfree(b);
	display_mem_map()
	//int *d=mymalloc(400);
	//display_mem_map()
	//allocate(100);
	//display_mem_map()
	return 0;
}
