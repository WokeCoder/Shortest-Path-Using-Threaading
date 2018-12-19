/*
 ============================================================================
 Name        : CSCI3431-Assignment2-A00310101.c
 Author      : Tye Borden
 Version     :
 Copyright   : Your copyright notice
 Description : This is a simple program that uses  the  Floyd-Warshall All
 pairs shortest path algorithm via single thread and multi thread options.
 This programs runs by first reading in where the user wants to run the
 program single or multi threaded, then reads in the number of nodes and
 edges. Then reads in the nodes then, the wait of the line between them.
 Eg 1 2 1 ==> node 1 node 2 weight 1. Then program returns the time taken
 to process  the nodes lines and there weight to find the shortest path from
 to another.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#define  INF 999999 //This is the acting infinite value to be used

int **g; // This is the graph matrix
int **d; // This is the distance matrix
int node,edges;

sem_t dbLock; // This is the lock for semaphore

/*This is struct that is used to hold the values*/

struct arg_s{
	int n;
	int i;
	int k;
};
/*This struc is used to hold the values passed in via multi-threading*/
struct args_t
{
	int k;
	int i;
};
/*
 * This function simply prints whatever 2D matrix is passed to it.
 * @param input is a given 2D matrix
 * */
 void printM(int ** input){
	 for(int r = 0;r < node;r++){
	 		for(int c = 0;c < node;c++){
	 			if(input[r][c] == INF ){
	 				printf("INF ");
	 			}else{
	 			printf("%3d ",input[r][c]);
	 			}
	 		}
	 		printf("\n");
	 	}
 }
 /*
  * This function takes values read in for node's weight and stores them in
  * the graph matrix.
  * @prama x is a 2D martix, input is and array of args_s struct
  * */
 void initG(int ** x ,struct arg_s *input)
 {
 	for (int t = 0; t < edges; t++)
 	{
 		/*
 		 * Takes the values of k puts the value in its respctive place
 		 * in the matrix.
 		 * */
 		x[(input[t].n - 1)][(input[t].i - 1)] = input[t].k;
 		x[(input[t].i - 1)][(input[t].n - 1)] = input[t].k;
 	}
 }
 /*
  * This function sets all nodes that reference them selves weight to zero,
  * and all others to INF
  * @param input is a 2D matrix
  * */
 void initM(int **input){
	 for(int r = 0;r < node;r++){
	 		for(int c = 0;c < node;c++){
	 			if(r == c){
	 				input[r][c] = 0; //Setting the same node to 0
	 			}else{
	 				input[r][c] = INF;//Setting the top half graph values t INF
	 				input[c][r] = INF;//Setting the bottom half of the graph to INF
	 			}
	 		}
	 	}
 }
/*
 * This function runs the process as a single thread to using the FW shortest
 * path.
 * */
void spC(){
	for(int k = 0; k < node;k++){
	for(int i =0; i < node;i++){
				for(int j = 0;j<node;j++){
					if(d[i][k] + d[k][j] < d[i][j] ){
						d[i][j] = d[i][k] + d[k][j];
					}
				}
		}
	}
}
/*
 * This is a thread function used by the mulithread function to perform
 * the inner most loop of the FW shortest path. Using semaphores to restrict
 * access to while path values are being updated.
 * @param args in this will struc args_t */
void *worker(void *args){
	sleep(1);
	struct args_t *wTemp =  args;
	int i = wTemp->i;//Converting the struct values to local variables
	int k = wTemp->k;
	for(int j = 0;j < node;j++){

				if(d[i][k] + d[k][j] < d[i][j]){
				sem_wait(&dbLock);//Locking the critical section
				d[i][j] = d[i][k] + d[k][j];//Updating values
				sem_post(&dbLock);//unlocking critical section
			}
	}
	pthread_exit(0);
}
/*
 * This function performs a multi threaded approach to FW shortest path using
 * multiple threads one is created for each node to find its shortest path
 * to each other node if one exists. If none the INF is left its place.
 * */
void mthC(){
	pthread_t *thread;
	pthread_attr_t attr;
	thread = malloc(node * sizeof(pthread_t)); //Setting the thread amount
	pthread_attr_init(&attr);
	struct args_t temp[node];
	for(int k = 0; k < node;k++){
		for(int i =0; i < node;i++){
			temp[i].k = k;
			temp[i].i = i;
			pthread_create(&thread[i],&attr,worker,(void *) &temp[i]);
			}
		for(int i = 0;i <node ;i++){
			pthread_join(thread[i],NULL);
		}
		}
	free(thread);
}
/*
 * This program will use Floyd-Warshall All-Pairs-Shortest-Path algorithm
 * either using single or multi threaded function. Then in with produce matrix
 * of shortest weight paths from the n nodes to each other.
 * */
int main(void) {
	/*Asking the user would the like to uses single or multi threaded option*/
	printf("Would you like single = 0  or multi = 1 threaded: \n");
	int opt;
	/*Reading the response*/
	scanf("%d",&opt);

	clock_t start,end;
	start = clock(); //Clock starts timing the process
	sem_init(&dbLock,0,1);//Initializing the lock for the semaphore
	printf("Enter values node and edges:\n");
	scanf("%d %d",&node,&edges);
	printf("Enter nodes to connect and line weight eg: 1 2 1:\n");

	struct arg_s arr[edges];//Array of input values
	for(int i = 0; i < edges;i++){
		int valid = 1;
		while(valid  == 1){//Checking to make inputs are valid
			scanf("%d %d %d", &arr[i].n,&arr[i].i,&arr[i].k);
			if((arr[i].n < 1 || arr[i].n > node)
					||( arr[i].i < 1 || arr[i].i > node)
					|| (arr[i].k < 0 || arr[i].k > INF)){
				printf("Error invalid values entered!\n");
			}else{
				valid = 0;
			}
		}

	}
	g = malloc(node * sizeof(int *)); //Creating the size of the matrix
	d = malloc(node * sizeof(int *)); //Creating the size of the distance m
	for(int i = 0;i < node;i++){
		g[i]=malloc(node * sizeof(int));//Setting length of rows Graph matrix
		d[i]=malloc(node * sizeof(int));//Setting length of rows distance m
	}
	initM(g);
	initM(d);
	initG(g,arr);
	initG(d,arr);


	if(opt == 0) spC();//Using the user input to perform single threaded
	else mthC();//Using the user input to perform multi-threaded


	printM(d);// Printing the final matrix
	free(g);
	free(d);
	sem_destroy(&dbLock);
	end = clock();//Ending the clock
	//Printing the time taken to complete.
	double timet = (double) (end-start)/CLOCKS_PER_SEC;
	if(opt == 0) {
		printf("\nTotal  for single thread time %f seconds\n", timet);
	}else{
		printf("\nTotal  for multi-thread time %f seconds\n", timet);
	}
	return 0;
}
