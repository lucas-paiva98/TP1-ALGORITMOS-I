#include <stdio.h>
#include <stdlib.h>
#include "student.h"
#include "university.h"
#include "utils.h"

// Finds and updates the lowest grade and his respective student
void updateStudentLowerGrade(UNIVERSITY *university, int currentUniversity) {
  int lines = university[currentUniversity].candidatesQuantityTotal;
  int newMinimumGrade = university[currentUniversity].studentsApproved[0][1];
  int newMinimumGradeStudent = university[currentUniversity].studentsApproved[0][0];
  int newPositionLowest = 0;
  if (lines > 1) {
    for (int i = 1; i < lines; i++) {
      int currentGrade = university[currentUniversity].studentsApproved[i][1];
      int currentStudent = university[currentUniversity].studentsApproved[i][0];
        if (currentGrade < newMinimumGrade && currentGrade > 0) {
          newMinimumGrade = university[currentUniversity].studentsApproved[i][1];
          newMinimumGradeStudent = university[currentUniversity].studentsApproved[i][0];
          newPositionLowest = i;
        } else if (currentGrade == newMinimumGrade && currentGrade > 0) {
          if (currentStudent > newMinimumGradeStudent) {
            newMinimumGrade = university[currentUniversity].studentsApproved[i][1];
            newMinimumGradeStudent = university[currentUniversity].studentsApproved[i][0];
            newPositionLowest = i;
          }
        }
    }
  } 

  university[currentUniversity].positionLowest = newPositionLowest;
  university[currentUniversity].minimumGrade = newMinimumGrade;
  university[currentUniversity].lowestGradeStudent = newMinimumGradeStudent;
}

void addStudentToUniversity(
 STUDENT *students,
 UNIVERSITY *universities,
 int currentStudent,
 int currentUniversity) {

  int freePosition = universities[currentUniversity].nextFreePosition;
  universities[currentUniversity].studentsApproved[freePosition][0] = currentStudent;
  universities[currentUniversity].studentsApproved[freePosition][1] = students[currentStudent].grade;
  students[currentStudent].isApproved = 1;
  students[currentStudent].universityApproved = currentUniversity;
 }

// Remove the lowest grade student from an university
void removeStudentFromUniversity(UNIVERSITY *university, STUDENT *students, int currentUniversity) {
  int lowestGradeStudent = university[currentUniversity].lowestGradeStudent;
  int lowestPosition = university[currentUniversity].positionLowest;

  university[currentUniversity].studentsApproved[lowestPosition][0] = -1;
  university[currentUniversity].studentsApproved[lowestPosition][1] = -1;
  university[currentUniversity].nextFreePosition = lowestPosition;
  students[lowestGradeStudent].isApproved = 0;
  students[lowestGradeStudent].universityApproved = 0;
}

void changeLowestStudent(STUDENT *students, UNIVERSITY *universities, int currentStudent, int currentUniversity) {
  updateStudentLowerGrade(universities, currentUniversity);
  removeStudentFromUniversity(universities, students, currentUniversity);
  addStudentToUniversity(students, universities, currentStudent, currentUniversity);
}

// This function verifies if the stable match has finished. It verifies if the student is approved
// or if the student have applied to every university in his preference list.
int hasStablingMatchFinished(STUDENT *students, int studentQuantity) {
  int stableMatch = 0;
  int countUniversity = 0;
  for (int i = 1; i <= studentQuantity; i++) {
    if (students[i].isApproved) {
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

  if (stableMatch == studentQuantity) {
    return 1;
  } else {
    return 0;
  }
  
}

int main(int argc, char *argv[]) {

  for(int cont=0; cont < argc; cont++)
    printf("%d Parametro: %s\n", cont, argv[cont]);
  //////////////////// UNIVERSITIES ///////////////////////////////
	FILE *fileUniversity;
  UNIVERSITY *universities;
  int universityQuantity = 0;
  int universityGradeAux = 0;
  int universityCandidatesAux = 0;
  int universityCount = 1;

  fileUniversity = fopen(argv[1], "r");

  if (fileUniversity == NULL) {
    printf("Error! Cant't open the file.\n");
    exit(1);
	}

  fscanf(fileUniversity, "%d", &universityQuantity);

  universities = (UNIVERSITY *)malloc((universityQuantity + 1) * sizeof(UNIVERSITY));

  while (!feof(fileUniversity)) {
    fscanf(fileUniversity, "%d", &universityCandidatesAux);
    fscanf(fileUniversity, "%d", &universityGradeAux);
    
    universities[universityCount].lowestGradeStudent = 0;
    universities[universityCount].candidatesQuantity = universityCandidatesAux;
    universities[universityCount].candidatesQuantityTotal = universityCandidatesAux;
    universities[universityCount].minimumGrade = universityGradeAux;
    universities[universityCount].studentsApproved = allocateMatrix(universityCandidatesAux, 2);
    universities[universityCount].nextFreePosition = 0;
    universities[universityCount].gradeHasChanged = 0;
    universities[universityCount].positionLowest = -1;
    universityCount++;
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

  fileStudent = fopen(argv[2], "r");

  if (fileStudent == NULL) {
    printf("Error! Cant't open the file.\n");
    exit(1);
  }

  fscanf(fileStudent, "%d", &studentQuantity);
  
  students = (STUDENT *)malloc((studentQuantity + 1) * sizeof(STUDENT));

  while(!feof(fileStudent)) {
    fscanf(fileStudent, "%d", &priorityListSize);
    fscanf(fileStudent, "%d", &studentGrade);
    
    students[studentPosition].preferences = allocateVector(priorityListSize);    
    students[studentPosition].preferenceSize = priorityListSize;
    students[studentPosition].grade = studentGrade;
    students[studentPosition].isApproved = 0;

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
        // If the university that the student is applying isn't occupied and 
        // this student isn't approved then he can apply to a university  
        if (currentUniversity != -1 && !students[currentStudent].isApproved) {
          students[currentStudent].preferences[j] = -1;
          if (students[currentStudent].grade >= universities[currentUniversity].minimumGrade) {
            if (universities[currentUniversity].candidatesQuantity > 0) {
              universities[currentUniversity].candidatesQuantity--;
              addStudentToUniversity(students, universities, currentStudent, currentUniversity);
              if (universities[currentUniversity].candidatesQuantity == 0) {
                updateStudentLowerGrade(universities, currentUniversity);
                universities[currentUniversity].gradeHasChanged = 1;
              }
              universities[currentUniversity].nextFreePosition++;
            }
            // In this case, a student must be removed first to add a new student which has a greater
            // grade than the lowest in that university, but if they have the exactly the same grade
            // the student with the smaller id is approved
            else {           
              if (students[currentStudent].grade == universities[currentUniversity].minimumGrade) {
                if(universities[currentUniversity].gradeHasChanged
                  && currentStudent < universities[currentUniversity].lowestGradeStudent
                  ) {
                  changeLowestStudent(students, universities, currentStudent, currentUniversity);
                }
              } else {
                  changeLowestStudent(students, universities, currentStudent, currentUniversity);
              }
            }
            break;
          } 
        }
      }
    }
  }

  printf("Grupos com alocacao\n");
  for (int i = 1; i <= studentQuantity; i++) {
    if(students[i].isApproved) {
      printf("%d %d\n", i, students[i].universityApproved);
    }
  }

  printf("Candidatos nao alocados\n");
  for (int i = 1; i <= studentQuantity; i++) {
    if(!students[i].isApproved) {
      printf("%d\n", i);
    }
  }
    free(students);
    free(universities);

    return 0;
}