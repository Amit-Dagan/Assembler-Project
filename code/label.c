# include <stdio.h>
# include <string.h>
# include <ctype.h> 
# include <stdlib.h>
# include "linked_list.c"
# include "utils.c"
//# include "decoder.c"

/*
	in this fie we a handling all the labels 
	we are putting the in a list 
*/

int l_valeu(char *s);

char* next_digit(char *s);
int num_of_ints(char *s); 
int num_of_chars(char *str);
int num_of_struct(char *str);


/*
	this function collecting all the labels int the text
	making a temprary "no label" file
	and returning a list of all the labels and thers address
*/

node* readLabels(char* name){

	node *labelList = NULL;			/* list of all the labels*/
	node *data_labelList = NULL;	/* list of all the data labels*/
	node *entry_list = NULL; 		/* list of all the entry labels*/
	node *extern_list = NULL;		/* list of all the extern labels*/

	FILE *input, *temp_file;
	
	char temp_file_name[] = "no_labels"; 
	/* a temorary file, int this file there are no labels, 
	this file will be translated later and the deleted */
	
	
	input = fopen(name, "r");
	temp_file = fopen(temp_file_name, "w+");	
	if ( input == NULL )
    {
        printf( "file failed to open." ) ;
		printf( "please put the file in the same directory as this program." ) ;
		
    }
	
	char line[81], corrent_label_name[81], *s2, label_type[81];
	char temp_str[81];
	char type_data[] =   ".data";
	char type_struct[] = ".struct";
	char type_string[] = ".string";
	char type_entry[] =  ".entry";
	char type_extern[] = ".extern";
	int ic = 100, dc = 0;
	
 
	while(fgets(line, 81, input) != NULL){
	
		if (strcmp(line,"\n")==0) 	/*if its an empy line ignore it*/
			continue;
		if (is_a_comment(line))		/*if its a comment line ignore it*/
			continue;
			
		sscanf(line, "%s %s", corrent_label_name, label_type);
		
		if (is_valid_label_name(corrent_label_name)){
			
			/*checking if the label is a data type*/
			
			if(strcmp(label_type,type_struct) == 0){
			
				sprintf(temp_str, "%d",dc);
				data_labelList = insertToEnd(data_labelList, corrent_label_name, temp_str);
				dc += num_of_struct(line);

				s2 = next_word(line);
				fputs(s2, temp_file);
				continue;
			}
			if(strcmp(label_type,type_data) == 0){
			
				sprintf(temp_str, "%d",dc);
				data_labelList = insertToEnd(data_labelList, corrent_label_name, temp_str);
				dc += num_of_ints(line);

				s2 = next_word(line);
				fputs(s2, temp_file);
				continue;
			}
			if(strcmp(label_type,type_string) == 0){

				sprintf(temp_str, "%d",dc);
				data_labelList = insertToEnd(data_labelList, corrent_label_name, temp_str);
				dc += num_of_chars(line);

				s2 = next_word(line);
				fputs(s2, temp_file);
				continue;
			}

			sprintf(temp_str, "%d",ic);
			labelList = insertToEnd(labelList, corrent_label_name, temp_str);
			s2 = next_word(line);
			ic += l_valeu(s2); 
			s2 = next_word(line);
			fputs(s2, temp_file);
			//printf("%d\n", ic);
			
		}
		else if(strcmp(corrent_label_name,type_entry) == 0){
			entry_list = insertToEnd(entry_list, label_type, corrent_label_name);
			continue;
		}
		else if(strcmp(corrent_label_name,type_extern) == 0){
			extern_list = insertToEnd(extern_list, label_type, corrent_label_name);
			continue;
		}	
		else {
			/* if its not a label but is a data type*/
			fputs(line, temp_file);
			if (strcmp(corrent_label_name, type_string) == 0)
				dc += num_of_chars(line);
			
			else if (strcmp(corrent_label_name, type_data)  ==0)
				dc += num_of_ints(line);
				
			else if (strcmp(corrent_label_name, type_struct)==0)
				dc += num_of_struct(line);
			
			else
				ic += l_valeu(line);
			//printf("%d\n", ic);
		}

	}
	
	node* temp_head = data_labelList;
	
	int dc_to_ic;
	while (temp_head != NULL){
		dc_to_ic = atoi(temp_head->text);
		dc_to_ic += ic;
		sprintf(temp_str, "%d",dc_to_ic);
		labelList = insertToEnd(labelList, temp_head->name , temp_str);
		temp_head = temp_head->next;
	}
	temp_head = extern_list;
	while (temp_head != NULL){
	
		labelList = insertToEnd(labelList, temp_head->name , temp_head->text);
		temp_head = temp_head->next;
	}
	temp_head = entry_list;
	while (temp_head != NULL){
	
		labelList = insertToEnd(labelList, temp_head->name , temp_head->text);
		temp_head = temp_head->next;
	}

	destroy(data_labelList);
	fclose(input);
	fclose(temp_file);
	
	return labelList;
	
}

/* calculating the dc for struct */
int num_of_struct(char *str){

	char *token;
	char *temp;
	temp = strdup(str);
	char* rest = temp;
	int num;
	int sum = 0;
	char check[4];
	
	
	token = strtok_r(rest, ",", &rest);

	token = next_word(token);
	while(token[0]==' ')
		token += 1;
	num = atoi(token);
	if (sprintf(check, "%d", num)==-1){
		printf("unable to read int in struct");
		return -1;
		}
	if (strcmp(check, token)!= 0)
		return -1;
	sum++;
	
	printf("%d\n", num);
	token = strtok_r(rest, ",", &rest);
	printf("%s\n", token);
	
	while (token[0] == ' ')
		token += 1;
	
	if (token[0] == '"')
		token += 1;
	else printf("error");
	
	while (isalpha(token[0])){
		token += 1;
		sum++;
	}	
	return ++sum;
}


/* calculating the dc for ints */
int num_of_ints(char *str){

	char num_str[4];
	str = strstr(str, ".data");
	str = next_word(str);
	int num;
	char *token;
	char *temp;
	temp = strdup(str);
	char* rest = temp;
	int sum = 0; 

	while ((token = strtok_r(rest, ",", &rest))){
		sscanf(token, "%s", num_str);
		num = atoi(num_str);
		if ((num != 0) || (num_str[0] == '0'))
			sum++;
		else printf("ERROR in label / num_of_ints \n"); //TODO 0
	}
	return sum;
}


/* calculating the dc for string */
int num_of_chars(char *str){

	str = strstr(str, ".string");
	str = next_word(str);

	int sum = 0; 

	while (str[0] == ' ')
		str += 1;
	
	if (str[0] == '"')
		str += 1;
	else printf("error");
	
	while (isalpha(str[0])){
		str += 1;
		sum++;
	}
	return ++sum;

}

/*
int main(){

	printf ("%d",num_of_srtuct_2(".struct 113, \"daddda\""));

	
	node* labels = NULL;
	labels = readLabels("macro.am");
	while (labels->name != NULL){
	
		printf("label name = %s, adress = %s \n",labels->name,labels->text);
		labels = labels->next;
	
	}
	
}

*/

























