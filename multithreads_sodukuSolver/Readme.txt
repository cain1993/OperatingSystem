Implementation and Design Description for Project 3
                  Ruiyuan Jiang
The goal of the program is to check whether the solution to the soduku puzzle is valid or not. And multi-thread programming skill is required in this project. Thus, I design my program in the following steps.
1.	In the main function, 27 child threads is created to check the row, column and sub-grid of the soduku matrix.
2.	There are three sub-functions called row_check, column_check and grid_check. They will be called in different thread that handle the check of row, column and grid separately. All of these functions start from the initial points pass to the child when they are created and check the all the digits in the corresponding area.
3.	The initial position passed into the thread is packaged as a structure which includes the index of row and column. The soduku matrix is set as a global variable for convenience so that we don’t bother passing the soduku matrix to the child thread.
4.	I set another global variable, matrix_valid, as a flag to show whether the matrix is valid or not. This flag will be set to zero whenever an error is discovered by the child thread.
5.	The parent thread (main function) will call pthread_join to wait for the termination of all child threads at the end of the program.

