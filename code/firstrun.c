# include <stdio.h> 
# include <string.h>
# include <ctype.h> 
#define LINE_LENGTH 81


/* 
this function get a file name and making a new .am file
returning 1 if it stumble upon error
returning 0 if it was able to make am file

the function using linked list to save all the macros names and text

*/

int makeAmFile(char* name){

	char outputName[40];	
	char temp_file_name[] = "temp_file";
	char am[] = ".am";
	strcpy(outputName, name);
	strcat(outputName, am); /* outputName is "name".am */

	/* Declare the input file and output file pointers */
    FILE *input, *output, *temp_file;

	input = fopen(name, "r");	
	if ( input == NULL )
    {
        printf( "\nfile failed to open." ) ;
		printf( "\nplease put the file in the same directory of this program.\n" ) ;
		return 1;
    }

	output = fopen(outputName, "w+");
	temp_file = fopen(temp_file_name, "w+");
	
	char line[LINE_LENGTH], corrent_macro_name[LINE_LENGTH];
	char temp_str[LINE_LENGTH];
	int inMacro = 0;

	char macro[] = "macro";
	char endmacro[] = "endmacro";
	
	node *macroList = NULL; /* the head of the macro list , the nodes declared in linked_list.c*/



	while(fgets(line, LINE_LENGTH, input) != NULL){
		
		/*
			first run, now we are searching the macro declaration 
			saving the in a list and deleting them from the file
	
		*/
	
		
		if(!inMacro){
			
			sscanf(line, "%s %s", temp_str, corrent_macro_name);
			if(strcmp(temp_str,macro) == 0 ){
				if(!is_a_macro(macroList, corrent_macro_name)){
					if(is_a_valid_macro_name(corrent_macro_name)){ /* somethig wrong here*/
						inMacro = 1;
						continue;
					}
					else {
						printf("invalide macro name\n");
						return 1;}
				}
			}
		}
		
		if(inMacro){
		
			/* if we are reading a line thats inside a macro declaration */
			sscanf(line, "%s", temp_str);
			if(strcmp(temp_str,endmacro) == 0){
				inMacro = 0;	
				continue;
			}
			
			else{
				macroList = insertToEnd(macroList, corrent_macro_name, line); 
				/* save the line in a node inside the list*/
			}
			
		
		}
		else 
			fputs(line, temp_file);

	}
	
	/*
		second run
		now we are looking for macros calls
		and replacing them with the macro text
	
	*/
	
	rewind(temp_file);
	node* temp_node;
	while(fgets(line, LINE_LENGTH, temp_file) != NULL){
	
		sscanf(line, "%s", temp_str);
		
		if (is_a_macro(macroList, temp_str)){
		/* if whe word is a macro name*/
		
			temp_node = searchMacro(macroList, temp_str);
			while(strcmp(temp_node->name,temp_str) == 0){
			
				fputs(temp_node->text, output);
				if(temp_node->next == NULL)
					break;
				temp_node = temp_node->next;
				
			
			}
		}
		else
			fputs(line, output);
	}
	
	
	destroy(macroList);
	fclose(input);
	fclose(output);
	fclose(temp_file);
	remove(temp_file_name);
	return 0;
}
/*
texter for the macro function
int main(){


	
	makeAmFile("macro");
	
	
	
	return 0;
	

}
*/





















































