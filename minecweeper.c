#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

#define CLOSED '='
#define MINE 'x'
#define MINE_FLAGGED 'b'

void levelSelect(int* sizex, int* sizey, int* mines);
void generateMines(int sizex, int sizey, int mines, char x[sizey][sizex]);
void askInp(int sizex, int sizey, char x[sizey][sizex]);
void printBoard(int sizex, int sizey, bool cek, char x[sizey][sizex]);
void printBoardBig(int sizex, int sizey, char x[sizey][sizex]);
char checkMines(int sizex, int sizey, int a, int b, char x[sizey][sizex]);
void checkAround(int sizex, int sizey, int a, int b, char x[sizey][sizex]);
void checkArounds(int sizex, int sizey, int a, int b, char x[sizey][sizex]);
bool isFinish(int sizex, int sizey, char x[sizey][sizex]);

int main(){
  //inisialize every array element
  int sizex, sizey, mines;
  levelSelect(&sizex, &sizey, &mines);
  char x[sizey][sizex];
  for(int i=0;i<sizey;i++){for(int j=0;j<sizex;j++){x[i][j] =CLOSED;}} //assign all element as '='
  generateMines(sizex, sizey, mines, x);//inisialize mines
  // printBoardBig(sizex, sizey, x);
  printBoard(sizex, sizey, 1, x);

  //looping should start here
  //change array element
  while(!isFinish(sizex, sizey, x)){askInp(sizex, sizey, x);}
}

void levelSelect(int* sizex, int* sizey, int* mines){
  int level=0;
  while(level < 1 || level > 4){
    printf("1. BEGINNER\n2. INTERMEDIATE\n3. EXPERT\nselect level: ");
    scanf("%d",&level);
  }
  switch (level) {
    case 1:
      *sizex = 10;
      *sizey = 10;
      *mines = 10;
      break;
    case 2:
      *sizex = 16;
      *sizey = 16;
      *mines = 40;
      break;
    case 3:
      *sizex = 30;
      *sizey = 16;
      *mines = 99;
      break;
  }
}

void generateMines(int sizex, int sizey, int mines, char x[sizey][sizex]){
  srand(time(0));
  for(int i=0;i<=mines;i++){
    int minesA = (rand()%sizey);
    int minesB = (rand()%sizex);
    x[minesA][minesB] = MINE;
    // }
  }
}

char checkMines(int sizex, int sizey, int a, int b, char x[sizey][sizex]){
  int count = 0;
  for(int i=-1;i<2;i++){
    for(int j=-1;j<2;j++){
      // printf("x(%d %d) = %c\n",i+a,j+b,x[a+i][b+j]); // for debugging
      if((a+i <= sizey-1 && a+i >= 0) && (b+j <= sizex-1 && b+j >= 0)){ //not allow scanning if a+i and j+b out of size
      if(x[a+i][b+j] == MINE || x[a+i][b+j] == MINE_FLAGGED){count++;} //scan element around
      }
    }
  }
  char countStr = count + '0';
  return countStr;
}

void checkAround(int sizex, int sizey, int a, int b, char x[sizey][sizex]){
  if(x[a][b]=='0'){
    for(int i=-1;i<2;i++){
      for(int j=-1;j<2;j++){
        if((a+i >= 0 && a+i <=sizey-1) && (b+j >= 0 && b+j <=sizex-1)){
          x[a+i][b+j] = checkMines(sizex, sizey, a+i, b+j, x);
        }
      }
    }
  }
}

void checkArounds(int sizex, int sizey, int a, int b, char x[sizey][sizex]){
  /* algorithm to check around
   * a, b | b+1 ... b+n | b-1 ... b-n
   * a+1, b | b+1 ... b+n | b-1 ... b-n
   * ...
   * a+n, b | b+1 ... b+n | b-1 ... b-n
   * a-1, b | b+1 ... b+n | b-1 ... b-n
   * ...
   * a-n, b | b+1 ... b+n | b-1 ... b-n
   */
  for(int k=0;k<=sizex;k++){ //honestly, I dont know how many loop it should be
    for(int i=a;i>=0;i--){
      for(int j=b;j>=0;j--){
        checkAround(sizex, sizey, i, j, x);
      }
    }
    for(int i=a;i>=0;i--){
      for(int j=b;j<sizex;j++){
        checkAround(sizex, sizey, i, j, x);
      }
    }
    for(int i=a;i<sizey;i++){
      for(int j=b;j>=0;j--){
        checkAround(sizex, sizey, i, j, x);
      }
    }
    for(int i=a;i<sizey;i++){
      for(int j=b;j<sizex;j++){
        checkAround(sizex, sizey, i, j, x);
      }
    }
  }
}

void askInp(int sizex, int sizey, char x[sizey][sizex]){
  int a,b;
  char c;
  scanf("%d %d %c", &b,&a,&c);
  switch (c) {
    case 'o':
      if(x[a][b] == MINE){
        printBoard(sizex, sizey, 0, x);
        printf("---");
        for(int i=0; i<(sizex+3)/2;i++) {printf("-");}
        printf("GAGAL BOS");
        for(int i=0; i<(sizex+3)/2;i++) {printf("-");}
        exit(EXIT_SUCCESS);
      }
      else {
        x[a][b] = checkMines(sizex, sizey, a, b, x);
        printf("membuka (%d, %d)\n",b,a);
        checkArounds(sizex, sizey, a, b, x);
      }
      break;
    case 'f':
      if(x[a][b]==CLOSED){x[a][b] = 'f';printf("menandai (%d, %d)\n",b,a);}
      else if (x[a][b]=='f') {x[a][b]=CLOSED;printf("menghapus tanda (%d, %d)\n",b,a);}
      else if (x[a][b]==MINE) {x[a][b]=MINE_FLAGGED;printf("menandai (%d, %d)\n",b,a);}
      else if (x[a][b]==MINE_FLAGGED) {x[a][b]=MINE;printf("menghapus tanda (%d, %d)\n",b,a);}
      break;
  }
  // printBoardBig(sizex, sizey, x);
  printBoard(sizex, sizey, 1, x);
}

void printBoard(int sizex, int sizey, bool cek, char x[sizey][sizex]){
  int a=0;
  int b=0;
  printf("  ");
  for(int i=0;i<sizex;i++){
    if(b == 10) b = 0;
    printf(" %d",b);
    b++;
  }  
  for(int i=0;i<sizey;i++){
    printf("\n%d |",a++);
    if(a == 10) a = 0;
    for(int j=0;j<sizex;j++){
      if (x[i][j]=='0') {printf(" |");}
      else if (x[i][j]== MINE && cek == 1) {printf("=|");} //hide mines 
      else if (x[i][j]==MINE_FLAGGED && cek == 1) {printf("f|");}
      else{printf("%c|",x[i][j]);}
    }
  }
  putchar('\n');
}

void printBoardBig(int sizex, int sizey, char x[sizey][sizex]){
  putchar('\n');
  //FIRST UPPER PART 
  //1st line
  for(int i=0;i<sizex;i++){printf(" _____");}putchar('\n');
  //2nd line
  putchar('|');
  for(int i=0;i<sizey;i++){printf("     |");}putchar('\n');
  
  for(int i=0;i<sizey;i++){
  //3rd line = print the arrray
  putchar('|');
    for(int j=0;j<sizex;j++){
      // printf("  %c  |",x[i][j]);
      if (x[i][j]=='0') {printf("     |");}
      else{printf("  %c  |",x[i][j]);}
    }
    printf("\n|");
    for(int k=0;k<sizex;k++){printf("_____|");};
    
    if(i<sizex-1){
      printf("\n|");
      for(int l=0;l<sizex;l++){printf("     |");}putchar('\n');
    }
  }
  putchar('\n');
}

bool isFinish(int sizex, int sizey, char x[sizey][sizex]){
  bool finish=0;
  int count=0;
  for(int i=0;i<sizey;i++){
    for(int j=0;j<sizex;j++){
      if(x[i][j]==CLOSED) count++;
    }
  }
  if(count == 0) finish=1;
  return finish;
}

