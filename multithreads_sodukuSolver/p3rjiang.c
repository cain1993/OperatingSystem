#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIM " \t\r\n\a"
#define READIN_BUF_SIZE 1024
int sudoku[9][9];
int matrix_valid=1;
typedef struct parameters{
          int row;
          int column;
}parameters;

// grid checker 
void *grid_check(void *para)
{
  struct parameters *index= (struct parameters*)para;
  int row = index->row;
  int col = index->column;
  int count[9]={0,0,0,0,0,0,0,0,0};
  int e_r = row+3;
  int e_c = col+3;
  int m_n[9];
  int i=0;
  int j=0;
  int valid;
            for(row;row<e_r;row++){
             
            	for(col=e_c-3;col<e_c;col++){
                       if (sudoku[row][col]==1)
                            count[0]++;
                       else if(sudoku[row][col]==2)
                            count[1]++;
                       else if(sudoku[row][col]==3)
                            count[2]++;
			else if(sudoku[row][col]==4)
                            count[3]++;
			else if(sudoku[row][col]==5)
                            count[4]++;
			else if(sudoku[row][col]==6)
                            count[5]++;
			else if(sudoku[row][col]==7)
                            count[6]++;
			else if(sudoku[row][col]==8)
                            count[7]++;
			else if(sudoku[row][col]==9)
                            count[8]++;
                         else;
                   
			}
		} 
		 
                 
   
        // count the miss number in the area  			 
   	for(i=0;i<9;i++){
      		 if (count[i]==0){
        		m_n[j]=i+1;
        		j++;
				}    
			}
             
                   valid= count[0]&count[1]&count[2]&count[3]&count[4]&count[5]&count[6]&count[7]&count[8];
             if (valid==0){
                 printf("Digit in subgrid row %d to %d and col %d to %d is missing.\nAnd the number is: ",e_r-2,e_r,e_c-2,e_c);
                 	for(i=0;i<j;i++)
                      	printf("%d ",m_n[i]);
                        printf("\n");
			matrix_valid=0;}
}

void *row_check(void *para)
{
  struct parameters *index= (struct parameters*)para;
  int row = index->row;
  int col = index->column;
  int count[9]={0,0,0,0,0,0,0,0,0};
  int m_n[9];
  int i=0;
  int j=0;
  int valid;
            	for(col;col<9;col++){
                       if (sudoku[row][col]==1)
                            count[0]++;
                       else if(sudoku[row][col]==2)
                            count[1]++;
                       else if(sudoku[row][col]==3)
                            count[2]++;
			else if(sudoku[row][col]==4)
                            count[3]++;
			else if(sudoku[row][col]==5)
                            count[4]++;
			else if(sudoku[row][col]==6)
                            count[5]++;
			else if(sudoku[row][col]==7)
                            count[6]++;
			else if(sudoku[row][col]==8)
                            count[7]++;
			else if(sudoku[row][col]==9)
                            count[8]++;
                        else;
                     }
	
// count the miss number in the area	 			 
   	for(i=0;i<9;i++){
      		 if (count[i]==0){
        		m_n[j]=i+1;
        		j++;
				}    
			}
             
                   valid= count[0]&count[1]&count[2]&count[3]&count[4]&count[5]&count[6]&count[7]&count[8];
                     if (valid==0){
                     printf("Digit in row %d is missing.And the number is:",row+1);
                 	for(i=0;i<j;i++)
                      	printf("%d ",m_n[i]);
                        printf("\n");
			matrix_valid=0;}
}

void *column_check(void *para)
{
  struct parameters *index= (struct parameters*)para;
  int row = index->row;
  int col = index->column;
  int count[9]={0,0,0,0,0,0,0,0,0};
  int m_n[9];
  int i=0;
  int j=0;
  int valid;
            	for(row;row<9;row++){
                       if (sudoku[row][col]==1)
                            count[0]++;
                       else if(sudoku[row][col]==2)
                            count[1]++;
                       else if(sudoku[row][col]==3)
                            count[2]++;
			else if(sudoku[row][col]==4)
                            count[3]++;
			else if(sudoku[row][col]==5)
                            count[4]++;
			else if(sudoku[row][col]==6)
                            count[5]++;
			else if(sudoku[row][col]==7)
                            count[6]++;
			else if(sudoku[row][col]==8)
                            count[7]++;
			else if(sudoku[row][col]==9)
                            count[8]++;
                        else;
                     }
 // count the miss number in the area 		 			 
   	for(i=0;i<9;i++){
      		 if (count[i]==0){
        		m_n[j]=i+1;
        		j++;
			}    
}
             
                   valid= count[0]&count[1]&count[2]&count[3]&count[4]&count[5]&count[6]&count[7]&count[8];
                     if (valid==0){
                     printf("Digit in column %d is missing.And the number is:",col+1);
                 	for(i=0;i<j;i++)
                      	printf("%d ",m_n[i]);
                        printf("\n");
                        matrix_valid=0;}
}


int main(int argc, char *argv[])
{
	//char *input=read_line(); 
	
	char *number;
	int temp;
	int row=0;
	int col=0;
        pthread_t tid[27];
        pthread_attr_t attr;
        
       //read in the txt file
        FILE *stream;
        char input[READIN_BUF_SIZE];
	if((stream=fopen("sudoku.txt","r"))==NULL) {  
        printf("cannot open file\n"); exit(1);  
    	}
	fgets(input,READIN_BUF_SIZE,stream);

        // store the string into an array
	for(row; row<9; row++){
		for(col; col<9; col++){
			number = strtok(input, DELIM);
        		while (number != NULL){
			     sudoku[row][col] = atoi(number);
			     col++;
			     number = strtok(NULL, DELIM);
			}
		}
	}
	
	int i, j;
	for(i=0; i<9; i++){
		for(j=0; j<9; j++){
			printf("%d ",sudoku[i][j]);
		}
	printf("\n");
        }

//      initialize the start index for 27 threads
        struct parameters index[27];
      
        for(row=0;row<9;row++){
        index[row].row=row;
        index[row].column=0;
	}

        for(col=0;col<9;col++){
        index[col+9].row=0;
        index[col+9].column=col;
	}
 
         int m=17;
        for(row=0;row<9;row=row+3){
              
            for(col=0;col<9;col=col+3){
                m++;
        	index[m].row=row;
        	index[m].column=col;
		}
	} 
//       initialize the 27 threads
       int thread;
       for (thread=0; thread<9;thread++) {
        pthread_attr_init(&attr);
        pthread_create(&tid[thread],&attr, row_check, (void*)&index[thread]);
    	}
      
        for (thread=9; thread<18;thread++) {
        pthread_attr_init(&attr);
        pthread_create(&tid[thread],&attr, column_check, (void*)&index[thread]);
    	}

        for (thread=18; thread<27;thread++) {
        pthread_attr_init(&attr);
        pthread_create(&tid[thread],&attr,grid_check,(void*)&index[thread]);
    	}
 //     wait for the thread to end          
        for(thread=0; thread<27;thread++){
             
		pthread_join(tid[thread],NULL);
        }
//      print out the result
        if (matrix_valid==1)
        printf("This matrix is valid\n");

}




