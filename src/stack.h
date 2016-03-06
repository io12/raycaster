#ifndef _STACK_
#define _STACK_

#define STACKMAX 440

struct stack {
	int stk[STACKMAX];
	int top;
}s;

void push(int item);

int pop();

#endif
