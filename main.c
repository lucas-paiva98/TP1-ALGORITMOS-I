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

void updateStudentLowerGrade(UNIVERSITY *university, int currentUniversity) {
  int collumn = university[currentUniversity].candidatesQuantity;
  int minimumGrade = university[currentUniversity].studentsAproved[0][1];

  // Achar a menor nota e seu respectivo aluno
  for (int i = 1; i < collumn; i++) {
    if (university[currentUniversity].studentsAproved[i][1] < minimumGrade) {
      minimumGrade = university[currentUniversity].studentsAproved[i][1];
    }
  }
}

// Verificar o index do estudante para mapear a posição no vetor
void addStudentToUniversity(
  STUDENT *students,
  UNIVERSITY *universities,
  int currentStudent,
  int currentUniversity
  ) {
  int freePosition = universities[currentUniversity-1].nextFreePosition;
  students[currentStudent]->isAproved = 1;
  students[currentStudent]->preferences[j] = -1;
  students[currentStudent]->universityAproved = currentUniversity;
  // Adicionar o aluno aprovado no vetor da universidade
  universities[currentUniversity-1]->studentsAproved[freePosition][0] = currentStudent;
  universities[currentUniversity-1]->studentsAproved[freePosition][1] = students[currentStudent].grade;
  updateStudentLowerGrade(&universities, currentUniversity);
}

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
    universities[universityCount].nextFreePosition = 0;
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
  // Modular essa parte
  while(!hasStablingMatchFinished()){
    for (int currentStudent = 0; currentStudent < studentQuantity; currentStudent++) {
      for (int j = 0; j < students[currentStudent].preferenceSize; j++) {
        int currentUniversity = students[currentStudent].preferences[j];
        if (students[currentStudent].grade >= universities[currentUniversity].minimumGrade) {
        // Pensar em uma forma melhor de montar o array/matriz de alunos aprovados
          if (universities[currentUniversity].candidatesQuantity > 0) {
            addStudentToUniversity();
            universities[currentUniversity].candidatesQuantity--;
          }
          else {            
            if(currentStudent < universities[currentUniversity].lowestGradeStudent) {
              addStudentToUniversity(&students, &universities, currentStudent, currentUniversity);
              // Remover o aluno de menor nota
              // Atualizar a menor nota dos aprovados e o aluno de menor nota
              removeStudentFromUniversity();
            }
          }
          break;
        }
      }
    }
  }

  free(students);
  free(universities);

  return 0;
}