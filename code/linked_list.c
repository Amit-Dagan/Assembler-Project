#include <stdlib.h>
#include <stdio.h>
#include <string.h>

  
  
/*
	in this file we are implementing linked list
	it will be used in the macro and label handeling
	as well as for the transelation of the file in the end

*/
  
  
  
  
  
     
#define BLOCK_ALLOCATION(pointer, type, size)\
pointer = (type*) malloc(sizeof(type) * (size))

#define BLOCK_RELLOCATION(pointer, type, size, b)\
type *SAVED_NAME_POINTER = (type*) realloc(pointer ,sizeof(type) * (size));\
if(SAVED_NAME_POINTER != NULL){\
	pointer = SAVED_NAME_POINTER;\
	b = true;}\
else {printf("not enough memorry");\
	b = false;}


/*
	node struct used for the macro and labels
*/
struct node
{
        char name[81];
		char text[81];
		struct node *next;
};
typedef struct node node;

/*
	line struct is used for transleting
*/
struct line
{
        int address;
		int bin; 
		struct line *next;
};
typedef struct line line;



line* newLine();
line* insertLineToEnd();

node* newNode();
node* insertToStart();
node* insertToEnd();
int length();
void destroy();

/*
	making a new node
*/
node* newNode(char *insertName ,char *insertText) {

	node *temp = NULL;
	BLOCK_ALLOCATION(temp, node, 1);
	if(temp != NULL){
		strcpy(temp->name, insertName);
		strcpy(temp->text, insertText);	
		temp->next = NULL;
	}
	else
		 printf("ERROR - not enogh memory");
	return temp;
}

/*
	inserting a new node to the start of a list
	returning the new head
*/
node* insertToStart(node* head, char *insertName ,char *insertText){

				
	node *temp = newNode(insertName, insertText);       

	if(head == NULL){
		head = temp;
		return head;
	}

    if(temp != NULL){

		temp->next = head;
		head = temp;
	}
	else printf("ERROR - not enogh memory, returning original head");
	
	return head;
	    
}

/*
	inserting a new node to the end of a list
	returning the head
*/

node* insertToEnd(node* head, char *insertName ,char *insertText){


	node *temp = newNode(insertName, insertText);  
	temp->next = NULL;
	if(head == NULL){
		head = temp;
		return head;
	}
	  
	node *temp_head = head;   

    while(temp_head->next != NULL){

		temp_head = temp_head->next;
	}
	
	temp->next = NULL;
	temp_head->next = temp;
	
	return head;
	    
}



/* returning the length of the list */
int length(node* head){

	int num = 0;

	while(head != NULL){

		head = head->next;
		num++;
		
	}	
	return num;

}

/*
	getting a macro name end returning the start of the sub list of the macro
	returning null if cant find the macro
*/
node* searchMacro(node* head, char* nodeName){

	while(head != NULL){

		if (strcmp(head->name,nodeName) == 0){
			return head;		
		} 		
		head = head->next;
	}	
	return NULL;
}


/*
	getging a name of a node and returnong the text
	 returning null if cant find the node

*/

char* searchText(node* head, char* nodeName){

	while(head != NULL){

		if (strcmp(head->name,nodeName) == 0){
			return head->text;		
		} 		
		head = head->next;
	}	
	return NULL;
} 

/*
	destroy all nodes in a list
*/
void destroy(node* head){		

	if(head == NULL) 
		return;

	while(head != NULL){

		node *temp = head;
		head = head->next;
		free(temp);
		
	}
	return;

}

/*
	destroy all nodes in a line list
*/
void destroy_lines(line* head){		

	if(head == NULL) 
		return;

	line *temp = NULL;
	while(head != NULL){
		temp = head;
		head = head->next;
		free(temp);
	}
	return;

}

/*
	check if a string is a name of a node inside a list
	usful for the macro handaling
	there for the name
*/
int is_a_macro(node *head, char* str){

	if(head == NULL) 
		return 0;
		
	node *temp = head;

	while(temp != NULL){

		if(strcmp(temp->name, str) == 0 )
			return 1;

		temp = temp->next;	
	}
	return 0;

}
/*
	check if a node inside a list of label is an extern
*/

int is_extern(node *head, char* str){

	if(head == NULL) 
		return 0;
		
	node *temp = head;

	while(temp != NULL){
		if(strcmp(temp->name, str) == 0 ){
			if(strcmp(temp->text, ".extern") == 0 )
				return 1;
		}
		temp = temp->next;	
	}
	return 0;

}

/*
	check if a node inside a list of label is an entry
*/

int is_entry(node *head, char* str){

	if(head == NULL) 
		return 0;
		
	node *temp = head;

	while(temp != NULL){
		if(strcmp(temp->name, str) == 0 ){
			if(strcmp(temp->text, ".entry") == 0 )
				return 1;
		}
		temp = temp->next;	
	}
	return 0;

}








/*

this is a tester fot the linked list

int main(){
	
	line* lines = NULL;
	
	if (lines == NULL)
		printf("\nkusemek\n");
	
	lines = insertLineToEnd(lines, 1, 12);
	
	if (lines == NULL)
		printf("\nimima shel hakusemek\n");
	
	lines = insertLineToEnd(lines, 2, 13);
	lines = insertLineToEnd(lines, 3, 32);	

	while (lines != NULL){
	
		printf("address = %d \nline = %d\n", lines->address	,  lines->bin);
		lines = lines->next;
	
	}


	
	return 0;
}
*/




/*
	making a new line
*/

line* newLine(int insert_address ,int insert_bin) {

	line *temp = NULL;
	BLOCK_ALLOCATION(temp, line, 1);
	if(temp != NULL){
		temp->address = insert_address;
		temp->bin = insert_bin;	
		temp->next = NULL;
	}
	else
		 printf("ERROR - not enogh memory in line list");
	return temp;
}



/*
	inserting a new line to the end of a list
	returnong the head
*/

line* insertLineToEnd(line* head, int insert_address ,int insert_bin){


	line *temp = newLine(insert_address, insert_bin);  
	temp->next = NULL;	

	if(head == NULL){
		//printf("head Null chek is good");
		head = temp;
		return head;
	}
	  
	line *temp_head = head;   

    while(temp_head->next != NULL){

		temp_head = temp_head->next;
	}
	

	temp_head->next = temp;
	
	return head;
	    
}

















