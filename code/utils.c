#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PRINT(x)\
printf("%s\n", x ? "true" : "false");


 /*
 in this file the are handful of useful functions 

 */

/*
	resiving a string pointer 
	and returnong a pointer to the next word in that string
*/	
char* next_word(char *s){

	if(s == NULL)
		return s;
	if (s[0] == '\n')
		return s;
	if (s[0] == '\0')
		return s;

	while (s[0] == ' '){  
		s = s + 1; 
	}
	if (s[0] == '\n')
		return s;
	if (s[0] == '\0')
		return s;
		
	while (s[0] != ' '){ 
		s = s + 1; 
		if (s[0] == '\n')
		return s;
		if (s[0] == '\0')
		return s;
	}

	
	return s;

}


/*
	as the name replys indicates, 
	in this function we checking if a word can be a label name

*/
int is_valid_label_name(char *name){

	if(name[strlen(name)-1] == ':'){  
	
		return 1; 
	}
	return 0;
}

/*
	yep, in this one we checking if a word is a register name
*/

int is_a_register(char *name){

	/* check if the length of the string is right*/
	if(strlen(name)>=8)
		return 0;
	
	if(!strcmp(name,"PSW"))
		return 1;

	if(name[0] == 'r'){
		int n = atoi(name+1);
		if ((n != 0) && (n < 8))
			return 1;
		if(name[1] == '0')
			return 1;
	}
	return 0;

}

/*
	you guessed it... 
*/

int is_a_comment(char *name){
	
	char first_word[80];
	sscanf(name, "%s", first_word );

	if(first_word[0] == ';')
		return 1;
	
	return 0;

}


int is_a_command(char *s){

	char op[40];
	sscanf(s, "%s", op);
	
	char *op_code[16]= {
	"mov", "cmp", "add", "sub", "not",
	"clr", "lea", "inc", "dec", "jmp",
	"bne", "get", "prn", "jsr", "rts", "hlt" 
	};

	int i=0;
	
	while (i<=15){
		
		if (strcmp(op, op_code[i])==0){
			
			return 	1;		
		}
		i++;
	}
	return 0;	

}


int is_a_directive(char *s){

	char name[40];
	sscanf(s, "%s", name);
	
	char *directives[5]= {
		".struct", ".string", ".data", ".entry", ".extern"};

	int i=0;
	
	while (i<=3){
		
		if (strcmp(name, directives[i])==0){
			
			return 	1;		
		}
		i++;
	}
	return 0;
	
}

int is_a_valid_macro_name(char *name){ 

	if(is_a_register(name) || is_a_command(name) || is_a_directive(name))
		return 0;
	else return 1;

}






 
/*

//test

int main(){

	char s[] = "LOOP:";
	char *d = ":asghrrsd";
	char *r1 = "r4";
	char *r2 = "r8";
	char *r3 = "r0";
	char *r4 = "PSW";
	char *r5 = "r7";

	s[1] = 'a';
	strcpy(s,d); 
	printf("%s",s);
}	
*/
