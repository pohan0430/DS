#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0

typedef int bool;

typedef struct {
	int ipos;
	int jpos;
	int dir;
}Position;

typedef struct {
	Position *pos;
	int size;
	int top;
	int bottom;
}Stack;

Stack create (int);
bool isFull (Stack);
bool isEmpty (Stack);
bool push (Stack*, Position);
Position pop (Stack*);
Position top (Stack);
Position bottom (Stack);

bool walk (int **, Stack *, int, int); 

int Direction, LastDir;
bool NEW = true; // if first time to walk on

int main () {
	int row, column;
	scanf("%d %d", &row, &column);
       	int **maze = malloc ((row+2) * sizeof(int *));
	*maze = malloc ((column+2) * sizeof(int));
	*(maze+row+1) = malloc ((column+2) * sizeof(int));
	for (int i = 0; i < column+2; i++) {
		maze[0][i] = maze[row+1][i] = 1;
	}
	for (int i = 1; i < row+1; i++) {
		*(maze+i) = malloc ((column+2) * sizeof(int));
		maze[i][0] = maze[i][column+1] = 1;
		for (int j = 1; j < column+1; j++) {
			scanf("%d", &maze[i][j]);
		}
	}

	Stack s1 = create(row*column);
	Position start = {1, 1, 0};
	push(&s1, start);
	while(walk(maze, &s1, row, column));
	
	if (Direction > 3 && s1.pos[s1.top].ipos == 1 && s1.pos[s1.top].jpos == 1) {
		printf("no answer\n");
	}
	else {
		while(s1.bottom <= s1.top) {
			printf("%d %d\n", bottom(s1).ipos - 1, bottom(s1).jpos - 1);
			s1.bottom += 1;
		}
	}
	
	return 0;
}

bool walk (int **maze, Stack *s, int row, int column) {
	int imove[8] = {0, 1, 1, 1, 0, -1, -1, -1};
       	int jmove[8] = {1, 1, 0, -1, -1, -1, 0, 1};
	int ipos = s->pos[s->top].ipos, jpos = s->pos[s->top].jpos;

	if(NEW){
		LastDir = Direction; // the direction of last time
		Direction = 0;
	}
	else {
		Direction += 1; // go new direction
	}

	for (;Direction < 8; Direction++) {
		if (Direction > 3 && ipos == 1 && jpos == 1) {
			return false; // no way to go out the maze
		}
		if (Direction == (LastDir+4)%8) {
			continue; // don't go back
		}
		else if (!maze[ipos+imove[Direction]][jpos+jmove[Direction]]) {
			Position next = {ipos+imove[Direction], jpos+jmove[Direction], Direction};
			push(s, next);
			NEW = true; // find new way
			if (ipos+imove[Direction] == row && jpos+jmove[Direction] == column) {
				return false; // go out the maze
			}
			return true;
		}
	}
	Position last = pop(s);
	Direction = last.dir;
	LastDir = top(*s).dir;
	NEW = false;
	return true;
}

Stack create (int n) {
	Stack s1;
	s1.size = n;
	s1.top = -1;
	s1.bottom = 0;
	s1.pos = (Position *) malloc( s1.size * sizeof(Position));
	return s1;
}

bool isFull (Stack s1) {
	return s1.top == s1.size-1;
}

bool isEmpty (Stack s1) {
	return s1.top == -1;
}

bool push (Stack *s1, Position item) {
	if (isFull(*s1)) {
		return false;
	}
	else {
		s1->top++;
		s1->pos[s1->top].ipos = item.ipos;
		s1->pos[s1->top].jpos = item.jpos;
		s1->pos[s1->top].dir = item.dir;
		return true;
	}
}

Position pop (Stack *s1) {
	if (isEmpty(*s1)) {
		Position p1 = {0, 0, 0};
		return p1;
	}
	else {
		return s1->pos[s1->top--];
	}
}

Position top (Stack s1) {
	return s1.pos[s1.top];
}
	
Position bottom (Stack s1) {
	return s1.pos[s1.bottom];
}


