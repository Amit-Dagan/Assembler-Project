#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "label.c"
#include "binary.c"

/*
	in this file we are translating a line of assembli code
	the translation is in bits inside a intiger
	the translated line and its address will be saved in la linked list of lines
	
*/


node* labels = NULL;

line* lines = NULL;

int ic = 100;


void translate_first_line();
void register_direct_translate();
void label_access_translate();
void direct_translate();
void immediate_translate();
int is_two_registers(char *s);

void translate_data();
void translate_string();
void translate_struct();
void translate_for_zero_ops();
void translate_for_one_ops();
void translate_for_two_ops();
void two_register_direct_translate();

int get_op_code(char *s){
	
	char op[4];
	sscanf(s, "%s", op);
	
	char *op_code[16]= {
	"mov", "cmp", "add", "sub", "not",
	"clr", "lea", "inc", "dec", "jmp",
	"bne", "get", "prn", "jsr", "rts", "hlt" 
};

	int i=0;
	
	while (i<=15){
		
		if (strcmp(op, op_code[i])==0){
			
			return 	i;		
		}
		i++;
	}
	return -1;	

}

void get_op_code_in_binary(int *bin, char *op){

	if (strcmp(op, "mov")==0){
		*bin = (*bin | 0<<6);
	}

	else if (strcmp(op, "cmp")==0){
		*bin = (*bin | 1<<6);
	}

	else if (strcmp(op, "add")==0){
		*bin = (*bin | 2<<6);
	}

	else if (strcmp(op, "sub")==0){
		*bin = (*bin | 3<<6);
	}

	else if (strcmp(op, "not")==0){
		*bin = (*bin | 4<<6);
	}

	else if (strcmp(op, "clr")==0){
		*bin = (*bin | 5<<6);
	}

	else if (strcmp(op, "lea")==0){
		*bin = (*bin | 6<<6);
	}

	else if (strcmp(op, "inc")==0){
		*bin = (*bin | 7<<6);
	}

	else if (strcmp(op, "dec")==0){
		*bin = (*bin | 8<<6);
	}

	else if (strcmp(op, "jmp")==0){
		*bin = (*bin | 9<<6);
	}

	else if (strcmp(op, "bne")==0){
		*bin = (*bin | 10<<6);
	}

	else if (strcmp(op, "get")==0){
		*bin = (*bin | 11<<6);
	}

	else if (strcmp(op, "prn")==0){
		*bin = (*bin | 12<<6);
	}
	
	else if (strcmp(op, "jsr")==0){
		*bin = (*bin | 13<<6);
	}

	else if (strcmp(op, "rts")==0){
		*bin = (*bin | 14<<6);
	}

	else if (strcmp(op, "hlt")==0){
		*bin = (*bin | 15<<6);
	}
	
	return;
}



int num_of_op(char *s){

	int n = get_op_code(s);
	
	if (n == 0 || n == 1 || n == 2 || n == 3 || n == 6 )
		return 2;
		
	if (n == 14 || n == 15 )
		return 0;
	
	if (n == -1)
		return -1;
	
	return 1;	
}

int is_immediate(char *s){

	char temp[80];
	char *pointer;
	int num;
	
	sscanf(s ,"%s", temp);
	if (temp[0]== '#'){
		pointer = temp + 1;
		num = atoi(pointer);
		if (num != 0)
			return 1;
		//TODO check if number itself is 0
		else printf("ERROR in is_immediate\n");
	}
	return 0;
} 


int is_direct(char *s){

	char temp[80];
	sscanf(s ,"%s", temp);

	if (is_a_register(temp))
		return 0;
	
	if (strstr(s, ".") == NULL)
		return 1;

	return 0;
}

int is_label_access(char *s){

	if (strstr(s, ".") != NULL)
		return 1;

	return 0;
}


int is_register_direct(char *s){

	char temp[80];
	sscanf(s ,"%s", temp);

	if (is_a_register(temp))
		return 1;
	return 0;
}


int l_value_op(char *s){

	if (is_immediate(s))
		return 1;
	
	if (is_register_direct(s))
		return 1;
		
	if (is_label_access(s))
		return 2;
		
	if (is_direct(s))
		return 1;
	return -1;
}


int l_valeu(char *s){

	char *temp = strdup(s);
	char op[80];
	int l = 0;
	int n;
	int num_of_ops;		
	
	sscanf(s ,"%s", op);
	n = get_op_code(op);
	
	if (n != -1){
		l++;
	}
	else 
		printf("ERROR in l_valeu\n");
		
	num_of_ops = num_of_op(op);	
	temp = next_word(temp); 
	
	if (num_of_ops == 1){	
		l += l_value_op(temp);	
	}
	
	if (num_of_ops == 2){
	
		if (is_two_registers(temp))
			return (l+1);
	
		char *token;
		char* rest = temp;

		while ((token = strtok_r(rest, ",", &rest))){		 
			 l += l_value_op(token);
		}			
	}	
	if (num_of_ops == 0){	
		return l;	
	}	
	return l;
	
}

int is_two_registers(char *s){

	char *temp;
	char *token, *rest; 
	temp = strdup((s));
	rest = temp;

	token = strtok_r(rest, ",", &rest);
	
	if (is_register_direct(token)){
	 	token = strtok_r(rest, ",", &rest);
		if (is_register_direct(token))
	 		return 1;
	 	}
	return 0;

}

void address_type_first(int *bin, char *s){

		
	if (is_label_access(s)){
		*bin = (*bin | 2<<4);
		return;
	}
	else if (is_immediate(s)){
		*bin = (*bin | 0<<4);
		return;
		}
	else if (is_register_direct(s)){
		*bin = (*bin | 3<<4);		
		return; 
		}
	else if (is_direct(s)){
		*bin = (*bin | 1<<4);
		return;
	}
}


void address_type_second(int *bin, char *s){
	
	if (is_label_access(s)){
		*bin = (*bin | 2<<2);
		return;
	}
	else if (is_immediate(s)){
		*bin = (*bin | 0<<2);
		return;
		}
	else if (is_register_direct(s)){
		*bin = (*bin | 3<<2);		
		return;
		}
	else if (is_direct(s)){
		*bin = (*bin | 1<<2);
		return;
	}
}



void translate(char *s){ /* s = " add #123, r23" / "add label.1, label123.23"  */

	char first_word[10];
	sscanf(s, "%s", first_word);
	if (strcmp(first_word, ".data") == 0){
		translate_data(s);
		return;
		}

	if (strcmp(first_word, ".string") == 0){
		translate_string(s); 
		return;
		}

	if (strcmp(first_word, ".struct") == 0){
		translate_struct(s);  
		return;
		}
 

	int num = num_of_op(s);
	
	if (num == 0)
		translate_for_zero_ops(s);

	else if (num == 1)
		translate_for_one_ops(s);
		
	else if (num == 2)
		translate_for_two_ops(s);


}

void translate_data(char *s){/* ".data 12,123, 32 ,1" */

	int bin = 0;
	char num_str[4];
	char *data = next_word(s);
	int num;
	char *token;
	char *temp;
	temp = strdup(data);
	char* rest = temp;

	while ((token = strtok_r(rest, ",", &rest))){
		bin = 0;
		sscanf(token, "%s", num_str);
		num = atoi(num_str);
		if ((num > 0) || (num_str[0] == '0')){
			bin = num;	
		}
		else if (num<0){
			bin = (num*-1);
			bin = (bin | 1<<9);
			bin = (bin ^ 1023);	
			bin += 1;	
		}
		lines = insertLineToEnd(lines, ic, bin); 
		ic++;
	} 
}


void translate_string(char *s){ /*.string  "addseafesf" */

	int bin = 0;
	char *str = next_word(s);
	
	while (str[0] == ' ')
		str += 1;

	str += 1;	
	
	while (isalpha(str[0])){
		bin = str[0];
		lines = insertLineToEnd(lines, ic, bin); 
		ic++;
		str += 1;	
	}
	lines = insertLineToEnd(lines, ic, 0); 
	ic++;
}

void translate_struct(char *s){/* ".struct  1, "asefasf"" */

	int bin = 0;
	char *str = strstr(s, ".struct");
	str = next_word(str);
	int num;
	sscanf(str, "%d", &num);

	if (num<0){
		bin = (num*-1);
		bin = (bin | 1<<9);
		bin = (bin ^ 1023);	
		bin += 1;	
	}
	
	else{
	bin = num;
	}

	lines = insertLineToEnd(lines, ic, bin); 
	ic++;
	char *chars = strstr(str, "\"");
	chars += 1;
	while (isalpha(chars[0]) != 0){
		bin = chars[0];
		lines = insertLineToEnd(lines, ic, bin); 
		ic++;
		chars += 1;	
	}
	lines = insertLineToEnd(lines, ic, 0); 
	ic++;
}

void translate_for_zero_ops(char *s){

	translate_first_line(s);
 
} 
     

void translate_for_one_ops(char *s){

	char op[4];
	char address[80];
	sscanf(s ,"%s %s", op, address); 
	translate_first_line(s);
	
	if (is_immediate(address)) {
	
		immediate_translate(address);
		return;
	}	
	else if (is_direct(address)){
		
		direct_translate(address);
		return;
	}
	else if (is_label_access(address)){ /* DATA.2*/
		
		label_access_translate(address);
		return;
	}	
 	else if (is_register_direct(address)){
 	
 		register_direct_translate(address,2);
 		return;
 	}

	printf("ERROR in translate_for_one_ops");
	return;

}



void translate_for_two_ops(char *s){ /* "add r2 , r2" / "mov #23, X"*/

	char op[4];
	sscanf(s ,"%s", op); 
	translate_first_line(s);
	
	char *temp;
	char *token, *rest; 
	temp = strdup(next_word(s));
	rest = temp;
	
	if (is_two_registers(temp)){
		
		two_register_direct_translate(temp);
		return;
	}
	
	int pos = 1;
	while ((token = strtok_r(rest, ",", &rest)) != NULL){

		if (is_immediate(token)) {
		
			immediate_translate(token);
		}	
		else if (is_direct(token)){
			
			direct_translate(token);
		}
		else if (is_label_access(token)){ 
			
			label_access_translate(token);
		}	
	 	else if (is_register_direct(token)){
	 	
	 		register_direct_translate(token, pos);
	 	}
	 pos++;	
	 
	}
	return;
}  

void translate_first_line(char *s){

	char op[4];
	int bin = 0;
	char address[80];
	sscanf(s ,"%s", op);
		
	get_op_code_in_binary(&bin, op);
	
	if(num_of_op(op) == 0){
		
		lines = insertLineToEnd(lines, ic, bin); 
		ic +=1;
		return;
	}
	if(num_of_op(op) == 1){
			
		sscanf(s ,"%s %s", op, address); 	
		address_type_second(&bin, address);
		lines = insertLineToEnd(lines, ic, bin); 
	
		ic += 1;
		return;
	}


	if(num_of_op(op) == 2){
		
		char *temp;
		char clean[80];
		char *token, *rest; 
		temp = strdup(next_word(s));
		rest = temp;
		token = strtok_r(rest, ",", &rest);	
		sscanf(token , "%s", clean);
		address_type_first(&bin, clean);
		token = strtok_r(rest, ",", &rest);
		sscanf(token , "%s", clean);	
		address_type_second(&bin, clean);		
		
		lines = insertLineToEnd(lines, ic, bin); 
		ic += 1;
		return;
	}
}


void two_register_direct_translate(char *address){ /* address = "r2 , r1 " / "r0, r5" */

	char first_register[2];
	char second_register[2];
	int bin = 0;
	char *temp;
	char *token, *rest; 
	temp = strdup(address);
	rest = temp;
	
	token = strtok_r(rest, ",", &rest);	
	sscanf(token, "%s", first_register);
	bin = (bin | (atoi(first_register+1)<<6));
	
	token = strtok_r(rest, ",", &rest);
	sscanf(token, "%s", second_register);
	bin = (bin | (atoi(second_register+1)<<2));	
	
	lines = insertLineToEnd(lines, ic, bin);  
	ic += 1;
	return;

}

void register_direct_translate(char *address, int pos){ /* address = "r6 " / "r2"*/

	char *temp = strdup(address);
	char clean[3];
	sscanf(temp, "%s", clean);
 	char *register_num = clean + 1;
 	int address_int = atoi (register_num);
 	int bin = 0;
 	if (pos==2) 
 		bin = address_int<<2;
 	else if (pos==1) 
 		bin = address_int<<6;
 	else printf("register position wrong");
	lines = insertLineToEnd(lines, ic, bin); 
	ic += 1;
 	return;

}

void label_access_translate(char *address){ /* address = " MAIN.12" / "DATA.32"*/

	char *temp;
	char *token, *rest; 
	temp = strdup(address);
	rest = temp;
	
	token = strtok_r(rest, ".", &rest);
	
	int bin = 0;
	char label[80];
	sscanf(token, "%s", label);

	
	if(is_extern(labels, label)){ // if extern
	
		bin = (bin | 1);
		lines = insertLineToEnd(lines, ic, bin); 
		char address_str[5];
		sprintf(address_str, "%d", ic);
		labels = insertToEnd(labels, label, address_str);
		ic += 1;
	}
	else{								// if not extern
	
		strcat(label, ":");
		char *temp_1 = strdup(searchText(labels, label));
	
		bin = (atoi(temp_1))<<2;
		bin = (bin | 2);

		lines = insertLineToEnd(lines, ic, bin);
		ic += 1; 
	}
	
	token = strtok_r(rest, ".", &rest);
	
	
	int address_int;	
	if ((sscanf(token, "%d", &address_int)) == -1){
		printf("ERROR in label_access_translate");
	}
	bin = 0;
	bin = address_int<<2; 

	lines = insertLineToEnd(lines, ic, bin); 
	ic += 1;
	return;	
}

void direct_translate(char *address){ /* address = " MAIN" / "END" */

	int bin = 0;
	char label[80];
	sscanf(address, "%s", label);
	
	
	if(is_extern(labels, label)){ /* if extern */
	
		bin = (bin | 1);
		lines = insertLineToEnd(lines, ic, bin); 
		char address_str[5];
		sprintf(address_str, "%d", ic);
		labels = insertToEnd(labels, label, address_str);
		ic += 1;

	}
	
	else{								/* if not extern */
		strcat(label, ":");
		char *temp_1 = strdup(searchText(labels, label));
	
		bin = (atoi(temp_1))<<2;
		bin = (bin | 2);

		lines = insertLineToEnd(lines, ic, bin);
		ic += 1; 
	}
	
}

void immediate_translate(char *address){            /* "#12, "-32" */

	char clean[5];
	sscanf(address, "%s", clean);
 	int bin = 0;
	int address_int;	
	char *temp = clean + 1 ;
	sscanf(temp, "%d", &address_int);
	if (address_int<0){
		
			bin = (bin | (address_int*-1)<<2);
			bin = (bin | 1<<9);
			bin = (bin ^ 508);	
			bin += 4;
			lines = insertLineToEnd(lines, ic, bin); 
			ic += 1;
			return;
		
		}
	else {
		bin = address_int<<2; 

		lines = insertLineToEnd(lines, ic, bin); 
		ic += 1;
		return;
	}
}



/*
int main(){

	     
	labels = readLabels("macro.am");
	
	
	while (labels->name != NULL){
	
		printf("label name = %s, adress = %s \n",labels->name,labels->text);
		labels = labels->next; 
	
	}
	
	

	translate(" .data  6,-9,15");
	//translate(" add r2,STR");
	//translate(" jmp END");
	//translate(" prn #-5");
	//translate(" sub r1, r4");
	//translate(" inc K");
  
 
	 	
	while (lines != NULL){
	
		printf("\naddress = %d \t line = %d", lines->address	,  decimal_to_binary(lines->bin));
		lines = lines->next;
	
	}
  



	return 1;



}
 


*/
















  
