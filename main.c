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
  int universityQuantity = 0;
  int universityGradeAux = 0;
  int universityCandidatesAux = 0;
  int universityCount = 0;

  fileUniversity = fopen("universidades.txt", "r");

  if (fileUniversity == NULL) {
    printf("Error! Cant't open the file.\n");
    exit(1);
	}

  fscanf(fileUniversity, "%d", &universityQuantity);
  printf("Quantity of universities=%d\n", universityQuantity);

  universities = (UNIVERSITY *)malloc(universityQuantity * sizeof(UNIVERSITY));

  while (!feof(fileUniversity)) {
    fscanf(fileUniversity, "%d", &universityCandidatesAux);
    fscanf(fileUniversity, "%d", &universityGradeAux);

    universities[universityCount].candidatesQuantity = universityCandidatesAux; 
    universities[universityCount].minimumGrade = universityGradeAux;
    universities[universityCount].studentsAproved = allocateMatrix(universityCandidatesAux, 2);
    universityCount++;
  }

  for (int i = 0; i < universityCount; i++) {
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
  int studentPosition = 0;

  fileStudent = fopen("candidatos.txt", "r");

  if (fileStudent == NULL) {
    printf("Error! Cant't open the file.\n");
    exit(1);
  }

  fscanf(fileStudent, "%d", &studentQuantity);
  
  students = (STUDENT *)malloc(studentQuantity * sizeof(STUDENT));

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
  while(!hasStablingMatchFinished()){
    for (int i = 0; i < studentQuantity; i++) {
      for (int j = 0; j < students[i].preferenceSize; j++) {
        int currentUniversity = students[i].preferences[j];
        if (students[i].grade >= universities[currentUniversity].minimumGrade) {
        // Pensar em uma forma melhor de montar o array/matriz de alunos aprovados
        // Modular essa parte
        students[i].isAproved = 1;
        students[i].preferences[j] = -1;
        students[i].universityAproved = currentUniversity;
        // Adicionar o aluno aprovado no vetor da universidade
        universities[currentUniversity-1].studentsAproved[][];
          if (universities[currentUniversity].candidatesQuantity > 0){
            universities[currentUniversity].candidatesQuantity--;
          }
          else {
            // Remover o estudante com menor nota
            // Atualizar a minimumGrade
          }
        }
      }
    }
    }
  }

  free(students);
  free(universities);

  return 0;
}