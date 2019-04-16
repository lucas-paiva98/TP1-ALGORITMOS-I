#include <stdio.h>
#include <stdlib.h>
#include "student.h"
#include "university.h"
// #include "utils.h"

int **allocateMatrix(int lines, int collumns) {
  int **matrix = (int **)malloc(lines * sizeof(int *));

  for (int i = 0; i < lines; i++) {
    matrix[i] = (int *)malloc(collumns * sizeof(int));
    for (int j = 0; j < collumns; j++) {
      matrix[i][j] = -1;
    }
  }
  return matrix;
}

int *allocateVector(int size) {
  int *vector = (int *)malloc(size * sizeof(int));
  return vector;
}

void updateStudentLowerGrade(UNIVERSITY *university, int currentUniversity) {
  int lines = university[currentUniversity].candidatesQuantityTotal;
  int newMinimumGrade = university[currentUniversity].studentsAproved[0][1];
  int newMinimumGradeStudent = university[currentUniversity].studentsAproved[0][0];

  // Achar a menor nota e seu respectivo aluno
  if (lines > 1) {
    for (int i = 1; i < lines; i++) {
      if (university[currentUniversity].studentsAproved[i][1] < newMinimumGrade) {
        newMinimumGrade = university[currentUniversity].studentsAproved[i][1];
        newMinimumGradeStudent = university[currentUniversity].studentsAproved[i][0];
        university[currentUniversity].gradeHasChanged = 1;
      }
    }
  } else {
    if (university[currentUniversity].studentsAproved[0][1] <= newMinimumGrade) {
        newMinimumGrade = university[currentUniversity].studentsAproved[0][1];
        newMinimumGradeStudent = university[currentUniversity].studentsAproved[0][0];
        university[currentUniversity].gradeHasChanged = 1;
      }
  }
    
  university[currentUniversity].minimumGrade = newMinimumGrade;
  university[currentUniversity].lowestGradeStudent = newMinimumGradeStudent;
  // printf("University[%d] = Student[%d]: %d \n", currentUniversity, newMinimumGradeStudent, newMinimumGrade);
}

void addStudentToUniversity(
 STUDENT *students,
 UNIVERSITY *universities,
 int currentStudent,
 int currentUniversity) {
  int freePosition = universities[currentUniversity].nextFreePosition;
  universities[currentUniversity].studentsAproved[freePosition][0] = currentStudent;
  universities[currentUniversity].studentsAproved[freePosition][1] = students[currentStudent].grade;
  students[currentStudent].isAproved = 1;
  students[currentStudent].universityAproved = currentUniversity;
  updateStudentLowerGrade(universities, currentUniversity);
 }

void removeStudentFromUniversity(UNIVERSITY *university, STUDENT *students, int currentUniversity) {
  int lowestGradeStudent = university[currentUniversity].lowestGradeStudent;
  for (int i = 0; i < university[currentUniversity].candidatesQuantityTotal; i++){
    int studentPosition = university[currentUniversity].studentsAproved[i][0];
     if (university[currentUniversity].studentsAproved[i][0] == lowestGradeStudent){
      university[currentUniversity].studentsAproved[i][0] = -1;
      university[currentUniversity].studentsAproved[i][1] = -1;
      university[currentUniversity].nextFreePosition = i;
      students[studentPosition].isAproved = 0;
      students[studentPosition].universityAproved = 0;
      break;
    }
  }
}

void changeLowestStudent(STUDENT *students, UNIVERSITY *universities, int currentStudent, int currentUniversity) {
  removeStudentFromUniversity(universities, students, currentUniversity);
  addStudentToUniversity(students, universities, currentStudent, currentUniversity);
}

int hasStablingMatchFinished(STUDENT *students, int studentQuantity) {
  int stableMatch = 0;
  int countUniversity = 0;
  for (int i = 1; i <= studentQuantity; i++) {
    if (students[i].isAproved) {
      stableMatch++;
    } else {
        for (int j = 0; j < students[i].preferenceSize; j++) {
          if (students[i].preferences[j] == -1) {
            countUniversity++;
          }
        }
        if (countUniversity == students[i].preferenceSize) {
          stableMatch++;
        }
        countUniversity = 0;
    }
  }
  printf("STABLE MATCH:%d\n",stableMatch);

  if (stableMatch == studentQuantity) {
    return 1;
  } else {
    return 0;
  }
  
}

int main() {
  //////////////////// UNIVERSITIES ///////////////////////////////
	FILE *fileUniversity;
  UNIVERSITY *universities;
  int universityQuantity = 0;
  int universityGradeAux = 0;
  int universityCandidatesAux = 0;
  int universityCount = 1;

  fileUniversity = fopen("universidades.txt", "r");

  if (fileUniversity == NULL) {
    printf("Error! Cant't open the file.\n");
    exit(1);
	}

  fscanf(fileUniversity, "%d", &universityQuantity);
  printf("universityQuantity:%d\n", universityQuantity);
  universities = (UNIVERSITY *)malloc((universityQuantity + 1) * sizeof(UNIVERSITY));

  while (!feof(fileUniversity)) {
    fscanf(fileUniversity, "%d", &universityCandidatesAux);
    fscanf(fileUniversity, "%d", &universityGradeAux);
    
    universities[universityCount].lowestGradeStudent = 0;
    universities[universityCount].candidatesQuantity = universityCandidatesAux;
    universities[universityCount].candidatesQuantityTotal = universityCandidatesAux;
    universities[universityCount].minimumGrade = universityGradeAux;
    universities[universityCount].studentsAproved = allocateMatrix(universityCandidatesAux, 2);
    universities[universityCount].nextFreePosition = 0;
    universities[universityCount].gradeHasChanged = 0;
    universityCount++;
  }

  for (int i = 1; i < universityCount; i++) {
    printf("%d ", universities[i].candidatesQuantityTotal);
    printf("%d ", universities[i].minimumGrade);
    printf("\n");
  }

  fclose(fileUniversity);
  
  //////////////// STUDENTS /////////////////////////
	FILE *fileStudent;
  STUDENT *students;
  int studentGrade = 0;
  int studentQuantity = 0;
  int studentAux = 0;
  int priorityListSize = 0;
  int studentPosition = 1;

  fileStudent = fopen("candidatos.txt", "r");

  if (fileStudent == NULL) {
    printf("Error! Cant't open the file.\n");
    exit(1);
  }

  fscanf(fileStudent, "%d", &studentQuantity);
  
  students = (STUDENT *)malloc((studentQuantity + 1) * sizeof(STUDENT));
  printf("studentQuantity:%d\n", studentQuantity);

  while(!feof(fileStudent)) {
    fscanf(fileStudent, "%d", &priorityListSize);
    fscanf(fileStudent, "%d", &studentGrade);
    
    students[studentPosition].preferences = allocateVector(priorityListSize);    
    students[studentPosition].preferenceSize = priorityListSize;
    students[studentPosition].grade = studentGrade;
    students[studentPosition].isAproved = 0;

    for (int i = 0; i < priorityListSize; i++) {
      fscanf(fileStudent, "%d", &studentAux);
      students[studentPosition].preferences[i] = studentAux;
    }
    studentPosition++;
  }

  fclose(fileStudent);

  /////////////////////////////////// MAIN ///////////////////////////////////
  while(!hasStablingMatchFinished(students, studentQuantity)){
    for (int currentStudent = 1; currentStudent <= studentQuantity; currentStudent++) {
      for (int j = 0; j < students[currentStudent].preferenceSize; j++) {
        int currentUniversity = students[currentStudent].preferences[j];
        if (currentUniversity != -1 && !students[currentStudent].isAproved) {
          students[currentStudent].preferences[j] = -1;
          if (students[currentStudent].grade >= universities[currentUniversity].minimumGrade) {
            if (universities[currentUniversity].candidatesQuantity > 0) {
              universities[currentUniversity].candidatesQuantity--;
              addStudentToUniversity(students, universities, currentStudent, currentUniversity);                 
            }
            else {            
              if (students[currentStudent].grade == universities[currentUniversity].minimumGrade) {
                if(universities[currentUniversity].gradeHasChanged
                  && currentStudent < universities[currentUniversity].lowestGradeStudent
                  ) {
                  changeLowestStudent(students, universities, currentStudent, currentUniversity);
                  break;
                }
              } else {
                  changeLowestStudent(students, universities, currentStudent, currentUniversity);
                  break;
              }
            }
            if (universities[currentUniversity].candidatesQuantity > 0) {
              universities[currentUniversity].nextFreePosition++;
            }
            break;
          } 
        }
      }
      // printf("currentStudent%d\n", currentStudent);
      // printf("Grupos com alocacao\n");
      // for (int i = 1; i <= studentQuantity; i++) {
      //   if(students[i].isAproved) {
      //     printf("%d %d\n", i, students[i].universityAproved);
      //   }
      // }

      // printf("currentStudent%d\n", currentStudent);
  // for (int i = 1; i <= studentQuantity; i++) {
  //   for (int j = 0; j < students[i].preferenceSize; j++) {
  //     printf("Student[%d] prefe: %d\n", i, students[i].preferences[j]);
  //   }
  //   printf("\n");
  // }
    }
  }


  printf("Grupos com alocacao\n");
  for (int i = 1; i <= studentQuantity; i++) {
    if(students[i].isAproved) {
      printf("%d %d\n", i, students[i].universityAproved);
    }
  }

  printf("Candidatos nao alocados\n");
  for (int i = 1; i <= studentQuantity; i++) {
    if(!students[i].isAproved) {
      printf("%d\n", i);
    }
  }
    free(students);
    free(universities);

    return 0;
}