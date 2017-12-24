#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "perProcessLinkedList.h"

typedef struct per_process_entry {
	int fcb_index;//fcb pointer
	int file_poisiton;
 }per_process_entry;


struct process_node {
	per_process_entry data;
      	int key;
	struct process_node *next;
};


process_node *p_head = NULL;

process_node** getPHead() {
	 process_node** result = &p_head;
	return result;
}
//display the list
void p_printList() {
   process_node *ptr = p_head;
   printf("\n[ ");
	
   //start from the beginning
   while(ptr != NULL) {
      printf("(%d,%s) ",ptr->key,ptr->data.firstname);
      ptr = ptr->next;
   }
	
   printf(" ]\n");
}

void p_printListToFile( char * fileName) {
	//printf(" %s\n", fileName);
	FILE *fp = fopen(fileName, "w+");

	process_node *ptr = p_head;

	while( ptr != NULL) {
		fprintf(fp, "%d %d\n", ptr->data.fcb_index, ptr->data.file_position);
		ptr = ptr->next;
	}

	fclose(fp);
}


//insert link at the first location
void p_insertFirst(int key,  student data) {
   //create a link
    process_node *link = ( process_node*) malloc(sizeof( process_node));
	
   link->key = key;
   link->data = data;
	
   //point it to old first node
   link->next = p_head;
	
   //point first to new first node
   p_head = link;
}

process_node* p_deleteFirst() {

   //save reference to first link
    process_node *tempLink = p_head;
	
   //mark next to first link as first 
   p_head = p_head->next;
	
   //return the deleted link
   return tempLink;
}

//is list empty
bool p_isEmpty() {
   return p_head == NULL;
}

 process_node** p_find( int key) {
	//start from the first link
	 process_node* current = p_head;

  	 //if list is empty
   	if(p_head == NULL) {
      		return NULL;
   	}

   	//navigate through list
   	while(current->key != key) {
	
      		//if it is last node
      		if(current->next == NULL) {
        		 return NULL;
      		} else {
         		//go to next link
         		current = current->next;
      		}
  	}      
	
   	//if data found, return the current Link
	 student_node** result = &current;
   	return result;

}

//delete a link with given key
 process_node* p_delete(int key) {

   //start from the first link
    process_node* current = p_head;
    process_node* previous = NULL;
	
   //if list is empty
   if(p_head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->key != key) {

      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }

   //found a match, update the link
   if(current == p_head) {
      //change first to point to next link
      p_head = p_head->next;
   } else {
      //bypass the current link
      previous->next = current->next;
   }    
	
   return current;
}

int p_length() {
   int length = 0;
    student_node *current;
	
   for(current = p_head; current != NULL; current = current->next) {
      length++;
   }
	
   return length;
}

//Used only by runqueue to extract min_vruntime
void p_sort() {

   int i, j, k, tempKey;
    per_process_entry tempData;
    process_node *current;
    process_node *next;
	
   int size = s_length();
   k = size ;
	
   for ( i = 0 ; i < size - 1 ; i++, k-- ) {
      current = p_head;
      next = p_head->next;
		
      for ( j = 1 ; j < k ; j++ ) {   
		
         if ( current->data.sid > next->data.sid ) {
            tempData = current->data;
            current->data = next->data;
            next->data = tempData;

            tempKey = current->key;
            current->key = next->key;
            next->key = tempKey;
         }
			
         current = current->next;
         next = next->next;
      }
   }   
}

void p_destroyList() {
	while( p_head != NULL) {
		process_node * temp = p_deleteFirst();
		free(temp);
	}
}
