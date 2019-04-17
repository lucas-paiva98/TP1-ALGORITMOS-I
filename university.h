#ifndef _UNIVERSITY
#define _UNIVERSITY

typedef struct {
	int minimumGrade;
	int candidatesQuantity;
	int candidatesQuantityTotal;
	int **studentsApproved;
	int nextFreePosition;
	int lowestGradeStudent;
	int gradeHasChanged;
}UNIVERSITY;

#endif
