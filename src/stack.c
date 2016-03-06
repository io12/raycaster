#include "stack.h"

void push(int item) {
	if (s.top == (STACKMAX - 1)) {
		return;
	}
	else {
		++s.top;
		s.stk[s.top] = item;
	}
	return;
}

int pop() {
	int item;
	if (s.top == -1) {
		return s.top;
	}
	else {
		item = s.stk[s.top];
		s.top--;
	}
	return item;
}
