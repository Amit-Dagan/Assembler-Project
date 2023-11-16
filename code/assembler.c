# include "decoder.c"
# include "firstrun.c"
    
   /*
		in this file we are managing the all translate prosses 
   */
   
/*
	as the name suggest, this function get a file name and mackong all the necessary files 
*/
    
void translate_file(char *file_name){   
 
	ic = 100;   
	lines = NULL;    
	int is_there_extern = 0;
	int is_there_entry = 0;
	
	int error = makeAmFile(file_name); /* the make am file return 1 in case of an error*/ 
	
	if (error){ 
		return;
	}
	
	char name_am[40];
	strcpy(name_am, file_name);
	strcat(name_am,  ".am");
	
	char name_ext[40];
	strcpy(name_ext, file_name);
	strcat(name_ext,  ".ext"); 
	
	char name_ent[40];
	strcpy(name_ent, file_name);
	strcat(name_ent,  ".ent");  
	
	
	labels = readLabels(name_am);
	
	
	FILE *input, *translates_file, *ext, *ent;
	
	char translates_file_name[40];
	strcpy(translates_file_name, file_name);
	strcat(translates_file_name,  "_translate"); 
	
	input = fopen("no_labels", "r");

	if ( input == NULL ) 
    {
        printf( "file failed to open." ) ;
		printf( "please put the file in the same directory as this program." ) ;
		return;
    }   
    
    translates_file = fopen(translates_file_name, "w+");	

    char text_line[80]; 	
    
    while(fgets(text_line, 80, input) != NULL){
    
    	if (strcmp(text_line,"\n")!=0) 
    		translate(text_line);
    }
      
   
    line *temp_head = lines; 

    
    while (temp_head != NULL){
    
			char t_t_line[5];
			int first_half_add = ((temp_head->address & 992)>>5);
			int second_half_add = (temp_head->address & 31);
			int first_half_line = ((temp_head->bin & 992)>>5);
			int second_half_line = (temp_head->bin & 31);
			  
			t_t_line[0] = get_thirty_two_charecter(first_half_add);
			t_t_line[1] = get_thirty_two_charecter(second_half_add);
			t_t_line[2] = get_thirty_two_charecter(first_half_line);
			t_t_line[3] = get_thirty_two_charecter(second_half_line);
			t_t_line[4] = '\0';
			
			fputs(t_t_line, translates_file);
			fputs("\n", translates_file);
			//fprintf(translates_file ,"%d\n",decimal_to_binary(temp_head->bin) );	
			temp_head = temp_head->next;
		}
    destroy_lines(lines);
    
    node *temp_label_head = labels; 
    while (temp_label_head != NULL){ 
		if (strcmp(temp_label_head->text, ".extern") == 0){
		    ext = fopen(name_ext, "w+");
		    is_there_extern = 1; 	
		    break;
		    }
		    temp_label_head = temp_label_head->next;

	}
	
	node *temp2_label_head = labels;
	if (is_there_extern){
	
		temp_label_head = labels;
		temp2_label_head = labels;
		
	    while (temp_label_head != NULL){ 
	    	if (strcmp(temp_label_head->text, ".extern") == 0){

	    		temp2_label_head = labels;
	    		while (temp2_label_head != NULL){ 
	    		
	    			if (strcmp(temp2_label_head->name, temp_label_head->name ) == 0){
	    				if(strcmp(temp2_label_head->text, ".extern") != 0){
	    					int UwU = atoi(temp2_label_head->text);
	    					int UwU_first = ((UwU & 992)>>5);
	    					int UwU_second = (UwU & 31);
	    					char address_of_extern[3];
	    					address_of_extern[0] = get_thirty_two_charecter(UwU_first);
	    					address_of_extern[1] = get_thirty_two_charecter(UwU_second);
	    					address_of_extern[2] = '\0';
	    					fputs( temp_label_head->name ,ext);
	    					fputs( "\t" ,ext);
	    					fputs( address_of_extern ,ext);
	    					fputs( "\n" ,ext);
	    					
	    					}			
	    			}
	    			
	    			temp2_label_head = temp2_label_head->next;
	    		}
	    	}
	    	temp_label_head = temp_label_head->next;
	    
	    }  
	
	}
	temp_label_head = labels;
	
    while (temp_label_head != NULL){ 
		if (strcmp(temp_label_head->text, ".entry") == 0){
		    ent = fopen(name_ent, "w+");
		    is_there_entry = 1;	
		    break;
		    }
		    temp_label_head = temp_label_head->next;

	}
	
	if (is_there_entry){
	
		temp_label_head = labels;
		temp2_label_head = labels;
		
	    while (temp_label_head != NULL){ 
	    	if (strcmp(temp_label_head->text, ".entry") == 0){

	    		temp2_label_head = labels;
	    		while (temp2_label_head != NULL){ 
	    		
	    		char label_name_bla[80];	
	    		sscanf(temp_label_head->name, "%s", label_name_bla);
	    		strcat(label_name_bla, ":");
	    		
	    			if (strcmp(label_name_bla, temp2_label_head->name ) == 0){
	    				if(strcmp(temp2_label_head->text, ".entry") != 0){
	    					int UwU = atoi(temp2_label_head->text);
	    					int UwU_first = ((UwU & 992)>>5);
	    					int UwU_second = (UwU & 31);
	    					char address_of_entry[3];
	    					address_of_entry[0] = get_thirty_two_charecter(UwU_first);
	    					address_of_entry[1] = get_thirty_two_charecter(UwU_second);
	    					address_of_entry[2] = '\0';
	    					fputs( temp_label_head->name ,ent);
	    					fputs( "\t" ,ent);
	    					fputs( address_of_entry ,ent);
	    					fputs( "\n" ,ent);
	    					
	    					}			
	    			}
	    			
	    			temp2_label_head = temp2_label_head->next;
	    		}
	    	}
	    	temp_label_head = temp_label_head->next;
	    
	    }
	 
	}


	  
	destroy(labels);
    remove("no_labels");
    fclose(input);
    fclose(translates_file);
	return;   
}





int main(int argc,char* argv[]){

    int counter;
	if(argc==1)
        printf("\nNo Extra Command Line Argument Passed Other Than Program Name");
    if(argc>=2)
    {
        for(counter=1; counter<argc; counter++)
            translate_file(argv[counter]);
    }
    

	return 1;

}
	
