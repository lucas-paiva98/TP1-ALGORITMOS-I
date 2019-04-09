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

// int findLowerGrade(int grades, int studentGrades) {

//   return;
// }

int main() {
  //////////////////// UNIVERSITIES ///////////////////////////////
	FILE *fileUniversity;
  UNIVERSITY *universities;
  int *universityCandidatesQuantity;
  int *universityMinimumGrade;
  int universityQuantity = 0;
  int universityGradeAux = 0;
  int universityCandidatesAux = 0;
  int i = 0, j = 0;

  fileUniversity = fopen("universidades.txt", "r");

  if (fileUniversity == NULL) {
    printf("Error! Cant't open the file.\n");
    exit(1);
	}

  fscanf(fileUniversity, "%d", &universityQuantity);
  printf("Quantity of universities=%d\n", universityQuantity);

  universities = (UNIVERSITY *)malloc(universityQuantity * sizeof(UNIVERSITY));

  universityCandidatesQuantity = allocateVector(universityQuantity);
  universityMinimumGrade = allocateVector(universityQuantity);

  while (!feof(fileUniversity)) {
    fscanf(fileUniversity, "%d", &universityCandidatesAux);
    fscanf(fileUniversity, "%d", &universityGradeAux);

    universities[i].candidatesQuantity = universityCandidatesAux; 
    universities[i].minimumGrade = universityGradeAux;

    universityCandidatesQuantity[i] = universityCandidatesAux;
    universityMinimumGrade[i] = universityGradeAux;
    i++;
  }

  for (int k = 0; k < i; k++){
    printf("%d ", universities[k].candidatesQuantity);
    printf("%d ", universities[k].minimumGrade);
    printf("\n");
  }

  fclose(fileUniversity);
  
  //////////////// STUDENTS /////////////////////////
	FILE *fileStudent;
  STUDENT *students;
  
  int **studentPreferences;
  int *studentGradesVector;
  int studentGrade = 0;
  int studentQuantity = 0;
  int studentAux = 0;
  int priorityListSize = 0;
  int studentPosition = 0;

  fileStudent = fopen("candidatos.txt", "r");

  if (fileStudent == NULL) {
    printf("Error! Cant't open the file.\n");
    exit(1);
  }

  fscanf(fileStudent, "%d", &studentQuantity);
  printf("Quantity of students=%d\n", studentQuantity);

  printf("Quantity of universities=%d\n", universityQuantity);
  
  students = (STUDENT *)malloc(studentQuantity * sizeof(STUDENT));
  
  studentPreferences = allocateMatrix(studentQuantity, universityQuantity);
  studentGradesVector = allocateVector(studentQuantity);

  while(!feof(fileStudent)) {
    fscanf(fileStudent, "%d", &priorityListSize);
    fscanf(fileStudent, "%d", &studentGrade);
    
    students[studentPosition].preferences = allocateVector(priorityListSize);    
    students[studentPosition].preferenceSize = priorityListSize;
    students[studentPosition].grade = studentGrade;

    studentGradesVector[studentPosition] = studentGrade;

    for (int i = 0; i < priorityListSize; i++) {
      fscanf(fileStudent, "%d", &studentAux);
      students[studentPosition].preferences[i] = studentAux;
      printf(" TESTE = %d\n", students[studentPosition].preferences[i]);
      studentPreferences[studentPosition][i] = studentAux;
    }
    studentPosition++;
  }

  for(int k=0; k < studentQuantity; k++) {
    for(int l = 0; l < universityQuantity; l++) {
      printf("%d ", studentPreferences[k][l]);
    }
    printf("\n");
  }

  for(int i = 0; i < 5; i++) {
    printf("%d ", studentGradesVector[i]);
  }
  
  printf("\n");

  for (int i = 0; i < studentQuantity; i++) {
    int aux = students[i].preferenceSize;
    for (int j = 0; j < aux; j++) {
      printf("%d ", students[i].preferences[aux]);
    }
    printf("\n");
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

  Estruturas necessárias para a representação dos matchs:
    - Um vetor para representar os alunos disponíveis para seleção
    - Um vetor dos alunos selecionados de cada universidade
    - Um vetor dos alunos que não entraram em nenhuma faculdade
  */

  int studentMatched[studentQuantity];

  for(int i = 0; i < studentQuantity; i++) {
    studentMatched[i] = 0;
  }
  
  for(int i = 0; i < studentQuantity; i++) {
    for(int j = 0; j < universityQuantity; j++) {
      int universitySelected = studentPreferences[i][j];
      if (
        studentGradesVector[i] >= universityMinimumGrade[universitySelected]
        && universityCandidatesQuantity[universitySelected] > 0
      ) {
        universityCandidatesQuantity[universitySelected]--;
        studentMatched[i] = 1;
        studentPreferences[i][j] = -1;
        break;
      }
      else if (studentGradesVector[i] >= universityMinimumGrade[universitySelected]) {

      }
    }
  }


  free(studentGradesVector);
  free(universityCandidatesQuantity);
  free(universityMinimumGrade);

  return 0;
}