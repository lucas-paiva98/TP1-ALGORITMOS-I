#include <stdio.h>
#include <stdlib.h>
#include "student.h"
#include "university.h"

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

void addStudentToUniversity(
 STUDENT *students,
 UNIVERSITY *universities,
 int currentStudent,
 int currentUniversity,
 int j) {
  int freePosition = universities[currentUniversity].nextFreePosition;
  universities[currentUniversity].studentsAproved[freePosition][0] = currentStudent;
  universities[currentUniversity].studentsAproved[freePosition][1] = students[currentStudent].grade;
  students[currentStudent].isAproved = 1;
  students[currentStudent].preferences[j] = -1;
  students[currentStudent].universityAproved = currentUniversity;
 }

void updateStudentLowerGrade(UNIVERSITY *university, int currentUniversity) {
  int lines = university[currentUniversity].candidatesQuantityTotal;
  int newMinimumGrade = university[currentUniversity].studentsAproved[0][1];
  int newMinimumGradeStudent = university[currentUniversity].lowestGradeStudent;

  // Achar a menor nota e seu respectivo aluno
  if (lines < 2) {
    if (university[currentUniversity].studentsAproved[0][1] < newMinimumGrade) {
      newMinimumGrade = university[currentUniversity].studentsAproved[0][1];
      newMinimumGradeStudent = university[currentUniversity].studentsAproved[0][0];
      printf("Student[%d]: %d \n", newMinimumGradeStudent, newMinimumGrade);
    }
  } else {
      for (int i = 2; i <= lines; i++) {
      // printf("ENTROU PORRA\n");
      // printf("%d < %d \n", university[currentUniversity].studentsAproved[i][1], newMinimumGrade);
      if (university[currentUniversity].studentsAproved[i][1] < newMinimumGrade) {
        newMinimumGrade = university[currentUniversity].studentsAproved[i][1];
        newMinimumGradeStudent = university[currentUniversity].studentsAproved[i][0];
        printf("Student[%d]: %d \n", newMinimumGradeStudent, newMinimumGrade);
      }
    }
  }
  printf("Antes uni[%d] %d < %d \n", currentUniversity, university[currentUniversity].studentsAproved[0][1], newMinimumGrade);
  university[currentUniversity].minimumGrade = newMinimumGrade;
  university[currentUniversity].lowestGradeStudent = newMinimumGradeStudent;
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
  printf("Quantity of universities=%d\n", universityQuantity);

  universities = (UNIVERSITY *)malloc((universityQuantity + 1) * sizeof(UNIVERSITY));

  while (!feof(fileUniversity)) {
    fscanf(fileUniversity, "%d", &universityCandidatesAux);
    fscanf(fileUniversity, "%d", &universityGradeAux);
    
    universities[universityCount].lowestGradeStudent = -1;
    universities[universityCount].candidatesQuantity = universityCandidatesAux;
    universities[universityCount].candidatesQuantityTotal = universityCandidatesAux;
    universities[universityCount].minimumGrade = universityGradeAux;
    universities[universityCount].studentsAproved = allocateMatrix(universityCandidatesAux, 2);
    universities[universityCount].nextFreePosition = 0;
    universityCount++;
  }

  for (int i = 1; i < universityCount; i++) {
    printf("%d ", universities[i].candidatesQuantity);
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
  /*
  Condições em que o loop deve continuar:
    - Algum aluno ainda não tentou a "inscrição" na faculdade
    
  Condições em que o loop deve parar:
    - Todos os alunos já tentaram a "inscrição" 
    - Não há instabilidade entre as escolhas.

  Condições para a universidade aceitar o aluno:
    - O aluno ter nota suficiente para entrar
    - A universidade ter vaga
 
  Condições para a universidade trocar o aluno:
    - Não ter vagas dispoíveis
    - O aluno que deseja entrar ter a nota maior do que algum dos alunos previamente selecionados
  
  Funções necessárias para a implementação do algoritmo
    - Achar a menor nota de um aluno classificado na universidade isGradeHigher

  Estruturas necessárias para a representação dos matchs:
    - Um vetor para representar os alunos disponíveis para seleção
    - Um vetor dos alunos selecionados de cada universidade
    - Um vetor dos alunos que não entraram em nenhuma faculdade
  */


  /* Enquanto o estudante não tiver aplicado em todas as suas
     faculdades na sua lista de preferência.
     Lembrar de verificar se o aluno está aprovado.
  */

  // Verificar se o aluno foi aprovado ou sua lista de preferências acabou
  // Modular essa parte !hasStablingMatchFinished(students, studentQuantity)
  while(!hasStablingMatchFinished(students, studentQuantity)){
    for (int currentStudent = 1; currentStudent <= studentQuantity; currentStudent++) {
      for (int j = 0; j < students[currentStudent].preferenceSize; j++) {
        int currentUniversity = students[currentStudent].preferences[j];
        if (currentUniversity != -1) {
          if (students[currentStudent].grade >= universities[currentUniversity].minimumGrade) {
            if (universities[currentUniversity].candidatesQuantity > 0) {
              universities[currentUniversity].candidatesQuantity--;
              addStudentToUniversity(students, universities, currentStudent, currentUniversity, j);
              updateStudentLowerGrade(universities, currentUniversity);
            }
            else {            
              // if(currentStudent < universities[currentUniversity].lowestGradeStudent) {
              removeStudentFromUniversity(universities, students, currentUniversity);
              addStudentToUniversity(students, universities, currentStudent, currentUniversity, j);
              updateStudentLowerGrade(universities, currentUniversity);
              // }
            }
            if (universities[currentUniversity].candidatesQuantity > 0) {
              universities[currentUniversity].nextFreePosition++;
            }
            break;
          } else {
            students[currentStudent].preferences[j] = -1;
          }
        }
      }
    }
  }


  // for (int i = 1; i <= studentQuantity; i++) {
  //   // for (int j = 0; j < students[i].preferenceSize; j++) {
  //   //   printf("Student[%d] prefe: %d\n", i, students[i].preferences[j]);
  //   // }
  //   printf("Student[%d] prefe: %d\n", i, students[i].isAproved);
  //   printf("\n");
  // }

  for(int i = 1; i <= universityQuantity; i++){
    printf("University[%d]: \n", i);
    for(int j = 0; j < universities[i].candidatesQuantityTotal; j++) {
      // printf("Student[%d] = %d ", j, universities[i].studentsAproved[j][0]);
      // printf("Grade = %d ", universities[i].studentsAproved[j][1]);
      // printf("Position = %d ", universities[i].nextFreePosition);
      printf("minimumGrade = %d ", universities[i].minimumGrade);
      printf("\n");
    }
    printf("\n");
  }

  for (int i = 1; i <= universityQuantity; i++)
  {
    printf("University[%d]: \n", i);
    for (int j = 0; j < universities[i].candidatesQuantityTotal; j++)
    {
      printf("Student[%d] ", universities[i].studentsAproved[j][0]);
      printf("Grade = %d ", universities[i].studentsAproved[j][1]);
      printf("\n");
    }
    printf("\n");
    }

    free(students);
    free(universities);

    return 0;
}