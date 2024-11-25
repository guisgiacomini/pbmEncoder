#include <stdio.h>
#include <stdbool.h>

int isEqual(int row, int col, int matriz[row][col]);
void processaMatriz(int row, int col, int matriz[row][col]);

int main(){

  int row, col;
  scanf("%d", &row);
  scanf("%d", &col);

  int matriz[row][col];

  for(int i = 0; i < row; i++){
    for(int j = 0; j < col; j++){
      scanf("%d", &matriz[i][j]);
    }
  }

  processaMatriz(row, col, matriz);

  return 0;
}

int isEqual(int row, int col, int matriz[row][col]){
  int anchor = matriz[0][0];

  for(int i = 0; i < row; i++){
    for(int j = 0; j < col; j++){
      if(anchor != matriz[i][j]){
        return -1;
      }
    }
  }
  return anchor;
}

void processaMatriz(int row, int col, int matriz[row][col]){

  if (row == 1 && col == 1) {
    int thePhilosopher = isEqual(row, col, matriz);
    if (thePhilosopher == 1) {
      printf("P");
    } else if (thePhilosopher == 0) {
      printf("B");
    } 
    return;
  }

  int thePhilosopher = isEqual(row, col, matriz);
  if(thePhilosopher == 1){
    printf("P");
  }
  else if(thePhilosopher == 0){
    printf("B");
  }

  else{

    printf("X");

    int meioRow = row / 2;
    int meioCol = col / 2;

    int m1[meioRow + (row % 2)][meioCol + (col % 2)]; // Quadrante superior esquerdo

    int m2[meioRow + (row % 2)][meioCol];             // Quadrante superior direito

    int m3[meioRow][meioCol + (col % 2)];             // Quadrante inferior esquerdo

    int m4[meioRow][meioCol];                         // Quadrante inferior direito

    // Define os quadrantes

    //m1
    for (int i = 0; i < meioRow + (row % 2); i++) {
      for (int j = 0; j < meioCol + (col % 2); j++) {
        m1[i][j] = matriz[i][j];
      }
    }
    processaMatriz(meioRow + (row % 2), meioCol + (col % 2), m1);

    //m2
    if(col > 1){
      for (int i = 0; i < meioRow + (row % 2); i++) {
        for (int j = meioCol + (col % 2); j < col; j++) {
          m2[i][j - meioCol + (col % 2)] = matriz[i][j];
        }
      }
      processaMatriz(meioRow + (row % 2), meioCol, m2);
    }


    //m3
    if(row > 1){
      for (int i = meioRow + (row % 2); i < row; i++) {
        for (int j = 0; j < meioCol + (col % 2); j++) {
          m3[i - meioRow + (row % 2)][j] = matriz[i][j];
        }
      }
      processaMatriz(meioRow, meioCol + (col % 2), m3);
    }

    //m4
    if(row > 1 && col > 1){
      for (int i = meioRow + (row % 2); i < row; i++) {
        for (int j = meioCol + (col % 2); j < col; j++) {
          m4[i - meioRow + (row % 2)][j - meioCol + (col % 2)] = matriz[i][j];
        }
      } 
      processaMatriz(meioRow, meioCol, m4);
    }
  }
}