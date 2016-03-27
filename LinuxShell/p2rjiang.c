#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define JRY_TOK_BUFSIZE 100
#define DELIM " \t\r\n\a"
#define JRY_RL_BUFSIZE 1024
#include <pwd.h>

//  Function Declarations for builtin shell commands:
int jry_exit(char **args);

//  List of builtin commands, followed by their corresponding functions.
 
char *builtin_str[] = {
  "exit", 
};

int (*builtin_func[]) (char **) = {
  &jry_exit
};

int JRY_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/
/**
   @brief Builtin command: exit.
   @return Always returns 0, to terminate execution.
 */
int jry_exit(char **args)
{
  return 0;
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int JRY_launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("jry");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("jry");
  } else {
    // Parent process
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int JRY_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < JRY_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    } 
  }

  return JRY_launch(args);
}

/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *JRY_read_line(void)
{
  char *buffer;
  size_t bufsize = JRY_RL_BUFSIZE;
  size_t characters;
   
  buffer=(char*)malloc(bufsize*sizeof(char));
   

  if (!buffer) {
    fprintf(stderr, "jry: allocation error\n");
    exit(EXIT_FAILURE);
  }
    characters=getline(&buffer,&bufsize,stdin);
    return buffer;
}

/**
   @brief Split a line into tokens 
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **JRY_split_line(char *line)
{
  int bufsize = JRY_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;
   if (!tokens) {
    fprintf(stderr, "jry: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;


    token = strtok(NULL, DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

// analysing whether the input is legal or not
int analysing(char*line){
    int i=0;
    int p_count=0;
    int in_count=0;
    int out_count=0;
    int in_pos=0;// <
    int out_pos=0;//>
    int error_flag=0;
    int characters=0;
    int space_count=0;
       
	 if(strlen(line)>101){
    	 printf("System error:Cannot exceed 100 character\n");
    	 error_flag=1;
         return 0;
	} 
      
    
     characters=strlen(line)-1;
         // the beginner must be a word
                if(      (toascii(line[0])<65&&toascii(line[0])>47)||
                         toascii(line[0])>122||
                         toascii(line[0])==45 ||
                         toascii(line[0])==46||
                         toascii(line[0])==95    )
		{
                 printf("System Error: Operator cannot be at the first position \n");
                 error_flag=1;
            
                 return 0;
                 }
       // the last can't be an operator
                if (    (line[characters-1]=='<')||
                        (line[characters-1]=='>')|| 
                        (line[characters-1]=='|')||
                        (line[characters-1]=='.')||
			(line[characters-1]=='_')||
 			(line[characters-1]=='/')||
                         (line[characters-1]=='-')
		)
                  	{
                     printf("System Error: No Operator at the last\n");
                      error_flag=1;
                    
                      return 0; 
                 	}
                
	for(i;i<characters;i++)
	{         // judge the illgal operator
		if(     (toascii(line[i])>=65&&toascii(line[i])<=90) || 
			(toascii(line[i])>=45&&toascii(line[i])<=57) ||
		        (toascii(line[i])>=97&&toascii(line[i])<=122)||
			toascii(line[i])==32 ||toascii(line[i])==95|| 
			toascii(line[i])==60 ||toascii(line[i])==62||
			toascii(line[i])==124||line[i]== EOF){	
                //printf("test\n");
		}else{
		printf("System Error_illegal operator\n");
                error_flag=1;
		break;
                }
              
              // count the differen operator in the line  
              if(line[i]=='<')
                {
                 in_count++;
                 in_pos=i;
 		if(line[i+1]!=' '||line[i-1]!=' '){
                     printf("System Error_input missing space\n");
                     break;           
                  }
                 if(in_count>1){
                     printf("System Error double input\n");
                     error_flag=1;
		     break;
                        }
                 else;
                }
              else if(line[i]=='>')
                {
                 out_count++;
                 out_pos=i;
                if(line[i+1]!=' '||line[i-1]!=' '){
                     printf("System Error_out missing space\n");
                     break;           
                  }
                 if(out_count>1){
                     printf("System Error_out double output\n");
                     error_flag=1;
		     break;
                        }
                 else;
                }
               else if(line[i]=='|')
                {
                 p_count++;
                 if(line[i+1]!=' '||line[i-1]!=' '){
                     printf("System Error_pipe missing space\n");
                     break;           
                  }
                
                 if(((p_count&out_count)||(p_count&in_count)))
                       {
                        printf("System Error_pipe incompatible\n");
                        error_flag=1;
		        break;
                        }
                 else;
                }
             if(out_pos<in_pos)
            {     if(out_pos!=0){
                     printf("System Error wrong order input and output\n");
                     error_flag=1;
		     break;
			}
             } 
             else;
	// judge the EOF case
            if(line[i]==EOF)
           {error_flag=2;
            printf("EOF meeted");
            break;
            }
            else;         	
	} 
          // no order after input or output
         if(out_count^in_count==1){
              if (out_count ==1){
                for(space_count=(out_pos+2);space_count<characters;space_count++)
			{
		  if(line[space_count]==' '){
               		printf("System Error: No cmd after > and <\n");
               		error_flag=1;
               		break;}
			}
		}
	      else if(in_count==1){
		for(space_count=(in_pos+2);space_count<characters;space_count++)
			{
		  if(line[space_count]==' '){
               		printf("System Error: No cmd after > and < _1\n");
               		error_flag=1;
               		break;}
			}
		  }
              
            }  
          if(out_count&in_count==1)
		{
		  for(space_count=(in_pos+2);space_count<(out_pos-2);space_count++)
		   { 
		     if(line[space_count]==' ')
                       {
			printf("System Error: No cmd after < and >\n");
			error_flag=1;
			break;
			}
                   }
		  for(space_count=(out_pos+2);space_count<characters;space_count++)
		    { 
		     if(line[space_count]==' ')
                       {
			printf("System Error: No cmd after < and >\n");
			error_flag=1;
			break;
			}
                   }	
		}
     if(error_flag==1)
     return 0;  // syntax error
     else if(error_flag==2)
     return 1; // EOF exit
     else if(p_count>0)
     return 2;// pipe command
     else if (in_count>0||out_count>0)
     return 3;//redirection command
     else 
     return 4;//simple command
}
//redirection part
int redirection(char *line)
{
        	
	char **tokens = malloc(64 * sizeof(char*));
	char *token;
	char *partinput;
	char *fileout, *filein;
        int in, out;
        pid_t pid;
	int status;
        int position = 0;
	partinput = strtok(line, ">");
	fileout = strtok(NULL, DELIM);


	partinput = strtok(line, "<");
	filein = strtok(NULL, DELIM);
        	
        if((filein!=NULL)&&(access(filein,F_OK))==-1)
        {
           printf("No such file or directory\n");
           return 1;

	}

	
	token=strtok(partinput,DELIM);
 	while (token != NULL) {
    	tokens[position] = token;
    	position++;
	token = strtok(NULL, DELIM);
 	}
	tokens[position] = NULL;
		
	pid = fork();
	if (pid==0){
        in = open(filein, O_RDONLY);
 	out = open(fileout, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP |S_IWGRP| S_IWUSR);
	dup2(in, 0);
  	dup2(out, 1);
  	close(in);
  	close(out);
	 execvp(tokens[0], tokens);	
	} else if (pid<0){
	perror("Error forking.\n");
	} else{
	waitpid(0,&status,WUNTRACED);
	}
        return 1;
}
//pipe part
int pipe_command(char *line)
{
   	char *token;
        char *tokens[64][64];
        char *pipe_tokens[64];
	char *pipe_o;
        int position_p = 0;
	int p=0;
	pipe_o = strtok(line, "|");
        while (pipe_o!= NULL) {
    	pipe_tokens[position_p] = pipe_o;
    	position_p++;
	pipe_o = strtok(NULL, "|");
 	}
	pipe_tokens[position_p] = NULL;
        // divide the mulitiple command
        int pipefd[30][2];
	int pid;
 	int status;
       for(p;p<=(position_p-1);p++){
        // split the command in the pipe section
	int position1 = 0;
	token=strtok(pipe_tokens[p],DELIM);
 	while (token != NULL) {
    	tokens[p][position1] = token;
    	position1++;
	token= strtok(NULL, DELIM);
 	}
	tokens[p][position1] = NULL;
        }
        int num_pipes;
        num_pipes=position_p-1;
       // built the pipe 
       for(p=0;p<num_pipes;p++){
        
        pipe(pipefd[p]);
        }
      // built the process
       for(p=0;p<position_p;p++){
         int liter=0;
         pid =fork();
         if(pid==0){
           if(p==0){
             		close(1);
              		dup(pipefd[p][1]);
              		for (liter = 0; liter < num_pipes; liter++)
			{
				close(pipefd[liter][0]);
				close(pipefd[liter][1]);
			}
              execvp(tokens[p][0], tokens[p]);
              }
             else if((0<p)&&(p<num_pipes))
          		{
			close(0);
			close(1);
			dup(pipefd[p - 1][0]);
			dup(pipefd[p][1]);
			 for (liter = 0; liter < num_pipes; liter++)
			{
				close(pipefd[liter][0]);
				close(pipefd[liter][1]);
			}
			 execvp(tokens[p][0], tokens[p]);
		}
		else
		{
			close(0);
			dup(pipefd[p-1][0]);
			 for (liter = 0; liter < num_pipes; liter++)
			{
				close(pipefd[liter][0]);
				close(pipefd[liter][1]);
			}
			 execvp(tokens[p][0], tokens[p]);
			}
		}
	}
      for (p= 0; p< num_pipes; p++)
	{
	close(pipefd[p][0]);
	close(pipefd[p][1]);
	}    
      for(p= 0; p < position_p; p++){  
         wait(&status); 
    	}
       return 1;

}
// type the current directory
 void type_prompt(char *prompt)
{
    struct passwd *pwd;
    char hostname[256];
    char pathname[1024];
    int length;
    pwd = getpwuid(getuid());
    getcwd(pathname,1024);
    if(gethostname(hostname,256)==0)
        sprintf(prompt,"[Jry Shell]%s@%s:",pwd->pw_name,hostname);
    else
        sprintf(prompt,"[Jry Shell]%s@unknown:",pwd->pw_name);
    // add the path
    length = strlen(prompt);
    if(strlen(pathname) < strlen(pwd->pw_dir) || 
            strncmp(pathname,pwd->pw_dir,strlen(pwd->pw_dir))!=0)
        sprintf(prompt+length,"%s",pathname);
    else
        sprintf(prompt+length,"~%s",pathname+strlen(pwd->pw_dir));

   // judge the root
    length = strlen(prompt);
     if(geteuid()==0)
        sprintf(prompt+length,"# ");
    else
        sprintf(prompt+length,"$ ");
    return;
}

int main(int argc, char **argv)
{
  // Run command loop.
  char *line;
  char **args;
  int status;
  int command_prop;
  char prompt[1024];
  do {
    type_prompt(prompt);
    printf("%s",prompt);
    line = JRY_read_line();
   // judge the command
    command_prop=analysing(line);
    if (command_prop==0){
    //printf("0");
    continue;}
    else if(command_prop==1){
    //printf("1");
     status = 0;}//return code zero
    else if(command_prop==2){
    //printf("2");
    status=pipe_command(line);
     }
    else if(command_prop==3){
    //printf("3");
    status=redirection(line);
     }
    else if(command_prop==4){
    //printf("4");
    args = JRY_split_line(line);
    status = JRY_execute(args);// simple command; 
     }
    
  } while (status);

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}

