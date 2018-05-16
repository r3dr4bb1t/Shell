#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

char * temp [100];
char * product[100]={'\0'};

void interactive();
void batchfile(FILE * fp);
int parse(char buf[255]);
void execute(char ** product);

int main(int argc, char * argv[])
{
	if (argc > 1)
	{

	 FILE *fp ;
	 fp = fopen(argv[1], "r");
		batchfile(fp);
		return 0;
	}
	interactive();
}

void batchfile(FILE * fp) 
{
	char buf[255];
	
	while(fgets(buf,sizeof(buf),fp)!=NULL)
	{	
		buf[strlen(buf) - 1] = '\0';
		if (strcmp(buf, "quit") == 0)
			exit(1);
		printf("%s\n",buf );
		parse(buf);
		printf("\n");
	}

}

void interactive()
{
	char cmd[255];
	while (1)
	{
		printf("prompt> ");
		if(fgets(cmd,255, stdin)=='\0')
			{	
				printf("\n");
				exit(1);
			}

		cmd[strlen(cmd) - 1] = '\0';

	 parse(cmd);
	}
}

int parse(char buf[255])
{
	int i = 1;
	int j = 1;

	int MultiCmdInOne = 0;

	if (strcmp(buf, "quit") == 0)
		exit(1);

	if (buf == '\0')
		exit(1);
	for (int k = 0; k < 255; k++)
	{
		if (buf[k] == ';')
			{MultiCmdInOne = 1;
				break;
			}
	}
		
	if (MultiCmdInOne == 1)
	{  
		temp[0] = strtok(buf, ";");
		while (1)
		{
			temp[i] = strtok(NULL, ";");
			if (temp[i] == NULL)
				break;
			i++;
		}
		for (int t = 0; t<i; ++t)
		{ 
			
			product[0] = strtok(temp[t], " ");

			while (1)
			{
				product[j] = strtok(NULL, " ");
				if (product[j] == NULL)
					break;
				j++;
			}
			execute(product);
			j =1;
			memset(product,'\0',100);
		}
		return 0;
	}
	

	product[0] = strtok(buf, " ");
	while (1)
	{
		product[j] = strtok(NULL, " ");
		if (product[j] == NULL)
			break;
		j++;
	}
	execute(product);
}

void execute(char **product)
{
	int status;
	int pid = fork();
	if (pid < 0)
		fprintf(stderr, "fork failed!");
	else if (pid == 0)
	{
		if (execvp(product[0], product) < 0)

		{
			printf("Invalid command\n");
			exit(1);
		}
	}
	else
		wait(&status);
}

	

	
