#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>


typedef struct node {
	struct node *next;
	struct node *prev;
	int frame;
	int page;
	int id;
	int page_faults;
	int use_bit;
} node;

typedef struct queue {
	node *front;
	node *rear;
	node *victim;
	int page_faults;
	int total_frames;
} queue;

void enqueue(queue*, int, int, int);

void enqueue(queue* q, int p_id1, int page, int frame) {
	node *new_node = (node *) malloc(sizeof(node));

	new_node->id = p_id1;
	new_node->page = page;
	new_node->frame = frame;
	new_node->use_bit = 1;

	if (q->front != NULL) {
		q->rear->next = new_node;
		new_node->prev = q->rear;
		q->rear = new_node;
		q->rear->next = q->front;
		q->front->prev = q->rear;
	} else {
		q->front = new_node;
		q->rear = new_node;
		q->victim = q->front;
		q->front = q->rear;
	}

}


node *dequeue1(queue *q) {
	node *ptr = NULL;
	if (q->front) {
		ptr = q->front;
		if (q->front == q->rear) {
			q->front = NULL;
			q->rear = NULL;
		} else {
			q->front = ptr->next;
			q->front->prev = q->rear;
			q->rear->next = q->front;
		}
	}
	
	return ptr;
}

int skip_entry(queue* page_table, int total_frames, int p_id, int page_num){

	node *temp = NULL;
	temp = page_table->front;
	int j = 0;
	int skip = 0;

	while(j < total_frames){
		if(temp->id == p_id && temp->page == page_num){
			skip = 1;
			temp->use_bit = 1;
			break;
		}
		temp = temp->next;
		j += 1;
	}

	return skip;
}

void update_processes(queue* processes, int* total_processes, int p_id){

		node *temp = NULL;
		int j = 0;
		int p_exists = 0;

		temp = processes->front;
		while(j < *total_processes){
			if(temp->id == p_id){
				p_exists = 1;
				break;
			}
			temp = temp->next;
			j += 1;
		}
		if(p_exists == 0){
			enqueue(processes, p_id, 0, 0);
			processes->rear->page_faults = 0;
			processes->rear->page_faults += 1;
			*total_processes += 1;
		}else{
			temp->page_faults += 1;
			p_exists = 0;
		}

}


// main function
int main() {

	char frames[10];
	int num_of_frames = 0;
	char replacement_type[7];
	int num_of_processes = 0;

	// get global or local
	fgets(replacement_type, sizeof(replacement_type), stdin);

	if (replacement_type[5] == 10){
		replacement_type[5] = '\0';
	}else{
		replacement_type[6] = '\0';
	}
	
	// printf("%d d", num_of_frames);

	int p_id;
	int page_num;
	int total_frames = 0;
	int total_processes = 0;

	int i = 0;
	int j = 0;
	int p_exists = 0;
	int k = 0;

	char *token;
	char input[10];

	node *temp = NULL;
	node *temp2 = NULL;

	// if global
	if(strcmp(replacement_type,"GLOBAL") == 0 || strcmp(replacement_type,"global") == 0){

		queue *page_table = (queue *) malloc(sizeof(queue));
		page_table->front = NULL;
		page_table->rear = NULL;

		queue *processes = (queue *) malloc(sizeof(queue));
		processes->front = NULL;
		processes->rear = NULL;

		// get # of frames
		scanf("%d *[^\n]", &num_of_frames);

		// loop to get input
		while (*(fgets(input, sizeof(input), stdin)) != '\n') {

			token = strtok(input, " ");

			// use this store the input, id and page
			while (token != NULL) {
				i += 1;
				if (i == 1) {
					p_id = atoi(token);
				} else if (i == 2) {
					page_num = atoi(token);
				}
				token = strtok(NULL, " ");
			}

			// printf("%d %d\n", p_id, page_num);


			j = 0;
			if(total_frames < num_of_frames){

				if(skip_entry(page_table,total_frames,p_id,page_num) == 1) {goto next_input;}

				enqueue(page_table, p_id, page_num, total_frames);

				update_processes(processes, &total_processes, p_id);

				total_frames += 1;
			} else{

				if(skip_entry(page_table,total_frames,p_id,page_num) == 1) {goto next_input;}

				temp = dequeue1(page_table);
				k = temp->frame;
				enqueue(page_table,p_id, page_num, k);

				update_processes(processes, &total_processes, p_id);

				k = 0;
			}
next_input: ;

			i = 0;
		}

		// output for global
		printf("PID Page Faults\n");

		j = 0;
		temp = processes->front;
		while(j < total_processes){
			printf("%d    %d\n", temp->id, temp->page_faults);
			temp = temp->next;
			j += 1;
		}

		j = 0;
		k = 0;
		temp = processes->front;
		while(j < total_processes){

			printf("Process %d page table\n", temp->id);
			printf("Page Frame\n");

			temp2 = page_table->front;
			k = 0;
			while(k < total_frames){

				if(temp->id == temp2->id){
					printf("%d    %d\n", temp2->page, temp2->frame);
				}
				k++;
				temp2 = temp2->next;
			}

			temp = temp->next;
			j++;
		}


	}
  	else if(strcmp(replacement_type,"LOCAL") == 0 || strcmp(replacement_type,"local") == 0){

  		queue *processes[50];

  		for(i = 0; i < 50; i++){
  			processes[i] = (queue *) malloc(sizeof(queue));;
  			processes[i]->front = NULL;
  			processes[i]->rear = NULL;
  			processes[i]->victim = NULL;
  		}

  		fgets(frames, sizeof(frames), stdin);
		num_of_frames = atoi(frames);

  		i = 0;

		while (*(fgets(input, sizeof(input), stdin)) != '\n') {

			token = strtok(input, " ");

			while (token != NULL) {
				i += 1;
				if (i == 1) {
					p_id = atoi(token);
				} else if (i == 2) {
					page_num = atoi(token);
				}
				token = strtok(NULL, " ");
			}

			// printf("%d %d\n", p_id, page_num);
			i = 0;
			while(i < total_processes){
				if (processes[i]->front->id == p_id){
					p_exists = 1;
					break;
				}
				i += 1;
			}

			if (p_exists == 0){
				enqueue(processes[total_processes],p_id,page_num,total_frames);
				processes[total_processes]->total_frames = 1;
				processes[total_processes]->page_faults = 1;
				total_processes += 1;
				total_frames = total_frames + num_of_frames;
			} else{

				if(processes[i]->total_frames < num_of_frames){
					if(skip_entry(processes[i],processes[i]->total_frames,p_id,page_num) == 1){
						goto next_input2;
					}else{
						enqueue(processes[i],p_id,page_num,(((i+1) * num_of_frames) - num_of_frames + processes[i]->total_frames));
						processes[i]->total_frames += 1;
						processes[i]->page_faults += 1;
					}
				} 
				else{
					if(skip_entry(processes[i],num_of_frames,p_id,page_num) == 1){
						goto next_input2;
					}else{
						
						temp = processes[i]->victim;
						while(temp->use_bit == 1){
							temp->use_bit = 0;
							temp = temp->next;
						}
						temp->page = page_num;
						temp->use_bit = 1;
						processes[i]->page_faults += 1;
						processes[i]->victim = temp->next;
					}
				}

			}
			
next_input2: ;
			p_exists = 0;
			i = 0;
		}

		// out put for local
		printf("PID Page Faults\n");

		j = 0;
		while(j < total_processes){
			printf("%d    %d\n", processes[j]->front->id, processes[j]->page_faults);
			j += 1;
		}

		j = 0;
		k = 0;
		
		while(j < total_processes){

			temp = processes[j]->front;
			printf("Process %d page table\n", temp->id);
			printf("Page Frame\n");

			k = 0;
			while(k < processes[j]->total_frames){
				printf("%d    %d\n", temp->page, temp->frame);
				temp = temp->next;
				k++;
			}

			j++;
		}


	} 
	else{
		// goto statement
	}



}

