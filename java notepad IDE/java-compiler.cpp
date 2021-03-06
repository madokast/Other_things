#include<stdlib.h>
#include<stdio.h>
#include<direct.h>
#include<string.h>
#include<Windows.h>

#define MAX_NAMELENTH 100

void clearBuffer(void);
void welcome(void);
void byebye(void);
void getCurrnetDir(char *);
void getNameOfJavaSourceFile(char *);
int getJavaSourceFileAbsoluteDir(char *, char *, char *, char *);
void createJavaSourceAndOpenIt(FILE*, char*, char*,char*);
void CompileJavaSourceFile(char*, char*, char*, char*, char*);
void RunJavaCompiledFile(char*, char*, char*);
char ISagain(void);

int main(void) 
{
	char CurrentWorkingDirectory[MAX_NAMELENTH];//current working directory
	char fileName[MAX_NAMELENTH];//the file name, named by user
	char postFix[] = ".java";//the java source file, post fix is ".java"
	char javaSourceFileAbsoluteDir[MAX_NAMELENTH + MAX_NAMELENTH];//the java file directory. CurrentWorkingDirectory + "\\" + fileName + postFix
	char cmdOPEN[3 * MAX_NAMELENTH];//the cmd for opening java source file
	char cmdCOM[3 * MAX_NAMELENTH];//the cmd for compiling java source file
	char cmdRUN[3 * MAX_NAMELENTH];//the cmd for run the java executable file
	FILE* javaFilePointer = NULL;//use it to create a java file
	int state = 1;//1-get the new name 0-open the last java source file
	char again = 'B';//Y-start nxct peogram N-exit R-reopen the sourve file for farther revision 

	welcome();
	getCurrnetDir(CurrentWorkingDirectory);

	while (again == 'B' || again == 'b' || again == 'R' || again == 'r') {

		if (again == 'R' || again == 'r')//revision modal
		{
			state = 0;//0-open the last java source file
		}

		while (state != 0)//get the name. 0-OK/1-notOK
		{
			//get the name and absolute direction to the new-named java source file
			getNameOfJavaSourceFile(fileName);
			state = getJavaSourceFileAbsoluteDir(javaSourceFileAbsoluteDir, CurrentWorkingDirectory, fileName, postFix);
		}

		state = 1;

		createJavaSourceAndOpenIt(javaFilePointer, javaSourceFileAbsoluteDir, cmdOPEN, fileName);
		CompileJavaSourceFile(javaSourceFileAbsoluteDir, cmdCOM, fileName, postFix, cmdOPEN);
		RunJavaCompiledFile(cmdRUN, CurrentWorkingDirectory, fileName);

		again = ISagain();
	}

	byebye();
	Sleep(1000);
	return 0;
}

void clearBuffer(void)
{
	char c;
	scanf_s("%c", &c, 1);

	while (c != '\n') {
		scanf_s("%c", &c, 1);
	}
}

void welcome(void)
{
	printf("*******************************************************\n");
	printf("+          The simplest JAVA Compiler  V0.9           +\n");
	printf("+            Enter the source file name               +\n");
	printf("+       it automatically opens with notepad++         +\n");
	printf("+                                                     +\n");
	printf("+               Program and Save it                   +\n");
	printf("+     Both Compiling and Re-compiling are provided    +\n");
	printf("*******************************************************\n");

	printf("\n\n");
}

void byebye(void)
{
	printf("\n\n");

	printf("oooOOOoooOOOoooOOOooo\n");
	printf("OO   ~Good Job~    OO\n");
	printf("oooOOOoooOOOoooOOOooo\n");
}

void getCurrnetDir(char * CurrentWorkingDirectory)
{
	//get the current working directory
	_getcwd(CurrentWorkingDirectory, MAX_NAMELENTH);
	printf("The current working directory is:[%s]\n", CurrentWorkingDirectory);
	printf("The java source file will be created here.\nHowever the java class file will br builded in .\\exeFile\n");
}

void getNameOfJavaSourceFile(char * fileName)
{
	char c;
	int state;
	state = 1;// 0-OK 1-notOK

	while (state == 1)
	{
		//get the name of the java source file
		printf("\n Please enter the name of java source file>>");
		scanf_s("%s", fileName, MAX_NAMELENTH);
		
		scanf_s("%c", &c, 1);//查看缓冲区是不是只剩下'\n'
		if (c != '\n')
		{
			clearBuffer();

		getNameOfJavaSourceFile01:printf("is your name [%s]? [Y/N]", fileName);
			scanf_s("%c", &c, 1);
			if (c == 'Y' || c == 'y') {
				clearBuffer();
				state = 0;//ok
			}
			else if (c == 'N' || c == 'n') {
				clearBuffer();
				state = 1;//not ok
			}
			else {
				clearBuffer();
				printf("!!invalid input\n");
				goto getNameOfJavaSourceFile01;
			}
		}
		else {
			state = 0;//OK
		}
		
	}
}

int getJavaSourceFileAbsoluteDir(char * javaSourceFileAbsoluteDir, char * CurrentWorkingDirectory, char * fileName, char * postFix)
{
	char c;
	int state;

	//get the absolute java file directory
	javaSourceFileAbsoluteDir[0] = '\0';
	strcat_s(javaSourceFileAbsoluteDir, MAX_NAMELENTH, CurrentWorkingDirectory);
	strcat_s(javaSourceFileAbsoluteDir, MAX_NAMELENTH, "\\");
	strcat_s(javaSourceFileAbsoluteDir, MAX_NAMELENTH, fileName);
	strcat_s(javaSourceFileAbsoluteDir, MAX_NAMELENTH, postFix);

	//user's making sure
	getAbsoluteJavaSourceFileDir01:printf("\nCreate [%s]\n[Y/N]>>", javaSourceFileAbsoluteDir);
	scanf_s("%c", &c, 1);
	if (c == 'Y' || c == 'y') {
		clearBuffer();
		state = 0;//ok
	}
	else if (c == 'N' || c == 'n') {
		clearBuffer();
		state = 1;//not ok
	}
	else {
		clearBuffer();
		printf("!!invalid input\n");
		goto getAbsoluteJavaSourceFileDir01;
	}

	return state;
}

void createJavaSourceAndOpenIt(FILE* javaFilePointer,char* javaSourceFileAbsoluteDir,char* cmdOPEN,char* fileName)
{
	int state;

	//create the java source file
	state = fopen_s(&javaFilePointer, javaSourceFileAbsoluteDir, "r");//try to open it
	if (state != 0) {//if state=0,open successfully. mean the file has existed
		//if state != 0 , fail to open it. mean the file doesn't exist
		//so create it
		state = fopen_s(&javaFilePointer, javaSourceFileAbsoluteDir, "w");
		if (state != 0) {//mean it failed to create the file. it's a fatal error 
			printf("\n!!!ERROR in create javaFile\nPress any key to terminate:>>");
			system("PAUSE");
			exit(1);
		}
		fprintf(javaFilePointer, "public class ");
		fprintf(javaFilePointer, "%s\n", fileName);
		fprintf(javaFilePointer, "{\n");
		fprintf(javaFilePointer, "\tpublic static void main(String[] args)\n");
		fprintf(javaFilePointer, "\t{\n");
		fprintf(javaFilePointer, "\t\tSystem.out.println(\"hello, world!\");\n");
		fprintf(javaFilePointer, "\t}\n");
		fprintf(javaFilePointer, "}\n");
	}
	fclose(javaFilePointer);//close it for notepad++ to open it whth cmd

	//cmd for open the file with notepad++
	cmdOPEN[0] = '\0';
	strcat_s(cmdOPEN, 3 * MAX_NAMELENTH, "\"D:\\download\\notepad\\notepad++.exe\" ");
	//strcat_s(cmdOPEN, 3 * MAX_NAMELENTH, "notepad ");
	strcat_s(cmdOPEN, 3 * MAX_NAMELENTH, javaSourceFileAbsoluteDir);
	printf("**open notepad++ for programming\nsave and close it for compiling\n");
	//printf("**open notepad for programming\nafter finishing, save and close it for compiling\n");
	system(cmdOPEN);
}

void CompileJavaSourceFile(char* javaSourceFileAbsoluteDir,char* cmdCOM,char* fileName,char* postFix,char* cmdOPEN)
{
	char c;

	//ask user to compiling it
	printf("\nStart compiling\n");

	//cmd for compiling it
	//like [javac -d ./exeFile HelloWorld.java]
	//javac -d ./exeFile HW.java
	cmdCOM[0] = '\0';
	strcat_s(cmdCOM, 3 * MAX_NAMELENTH, "javac -d ./exeFile ");
	strcat_s(cmdCOM, 3 * MAX_NAMELENTH, fileName);
	strcat_s(cmdCOM, 3 * MAX_NAMELENTH, postFix);
	system(cmdCOM);
	printf("**compilation finished\n");

	printf("\nneed to re-compile [%s]?\nY for re-compiling, others for runing![Y/N]>>", javaSourceFileAbsoluteDir);
	scanf_s("%c", &c, 1);
	clearBuffer();

	while (c == 'Y' || c == 'y') {
		system(cmdOPEN);
		system(cmdCOM);
		printf("**compilation finished\n");
		printf("\nneed to re-compile [%s]?\nY for re-compiling, others for runing![Y/N]>>", javaSourceFileAbsoluteDir);
		scanf_s("%c", &c, 1);
		clearBuffer();
	}
}

void RunJavaCompiledFile(char* cmdRUN, char* CurrentWorkingDirectory, char* fileName)
{
	//get cmd for run!
	//[java -classpath D:\Documents\JAVA_learning\exeFile HelloWorld]
	cmdRUN[0] = '\0';
	strcat_s(cmdRUN, 3 * MAX_NAMELENTH, "java -classpath ");
	strcat_s(cmdRUN, 3 * MAX_NAMELENTH, CurrentWorkingDirectory);
	strcat_s(cmdRUN, 3 * MAX_NAMELENTH, "\\exeFile ");
	strcat_s(cmdRUN, 3 * MAX_NAMELENTH, fileName);

	//run it
	//java -classpath D:\Documents\VS2017-learnCPP\java-automate\Project1\Project1\exeFile HW
	system(cmdRUN);
}

char ISagain(void)
{
	char c;

	printf("\n\n********************************\n");

	ISagain01:printf("Begin next program[B]\nRevise the last one[R]\nExit[E]\n>>");

	scanf_s("%c", &c, 1);
	clearBuffer();

	switch (c)
	{
	case 'B': case 'b'://Begin next program
	case 'R': case 'r'://Revise the last one
	case 'E': case 'e'://Exit
		return c;
		break;
	default:
		printf("!!invalid input\n");
		goto ISagain01;
		break;
	}
}
