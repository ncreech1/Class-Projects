#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "sockettome.h"
#include "jrb.h"
#include "dllist.h"
#include "jval.h"

/* Nicholas Creech
 * 11/30/20
 * Lab A: JThreads
 */

JRB rooms;
pthread_mutex_t lock;

typedef struct room 
{
	Dllist clients;
	Dllist input;
	pthread_cond_t cond;
} Room;

typedef struct client
{
	char *name;
	FILE *fout;
} Client;

void * openRoom(void *v);
void * openChat(void *v);
void sendInput(Room *r, char *input);
void freeClient(int *fdp, char *username, char *roomName);

int main(int argc, char **argv)
{
	pthread_t *tidp;
	int x, fd, sock;
	int *fdp;
	char s[100];
	FILE *clientFile;
	Room *r;

	if(argc < 3)
	{
		fprintf(stderr, "usage: ./chat_server <port> <chat_room ... >\n");
		exit(1);
	}

	rooms = make_jrb();

	//Create server socket
	sock = serve_socket(atoi(argv[1]));
	pthread_mutex_init(&lock, NULL);

	//Open chat rooms from argv
	for(x = 2; x < argc; x++)
	{
		r = malloc(sizeof(Room));
		r->clients = new_dllist();
		r->input = new_dllist();
		pthread_cond_init(&r->cond, NULL);
		jrb_insert_str(rooms, argv[x], new_jval_v(r));
		tidp = malloc(sizeof(pthread_t));
		pthread_create(tidp, NULL, openRoom, r);
	}

	//Check for new client connections
	while(1)
	{
		fd = accept_connection(sock);
		fdp = malloc(sizeof(int));
		*fdp = fd;
		tidp = malloc(sizeof(pthread_t));
		pthread_create(tidp, NULL, openChat, fdp);	
	}

	return 0;
}

//Opens a new chat room thread
void * openRoom(void * v)
{
	Room* r;
	Dllist tmp;
	Client *c;
	char *output;

	r = (Room*)v;

	while(1)
	{
		//Check for new input from clients
		while(!dll_empty(r->input))
		{
			output = dll_first(r->input)->val.s;
	
			//Send input as output to all clients
			tmp = r->clients->flink;
			while(tmp != r->clients)
			{	
				c = (Client*)tmp->val.v;

				//Remove client if error occurs on output
				if(fputs(output, c->fout) == EOF || fflush(c->fout) == EOF)
				{
					tmp = tmp->flink;
					close(c->fout);
					free(c);
					dll_delete_node(tmp->blink);
				}

				else
					tmp = tmp->flink;
			}

			//Remove input from room list
			free(output);
			dll_delete_node(dll_first(r->input));
		}

		pthread_cond_wait(&r->cond, &lock);
	}
}

//Opens a new client chat thread
void * openChat(void * v)
{
	Client *user;
	FILE *fin, *fout;
	int *fdp;
	char *username, *roomName, *input;
	char s[1000];
	char s2[1000];
	JRB room;
	JRB tmpJRB;
	Dllist tmpDllist;

	fdp = (int *)v;

	//Open in and out stream from connection file descriptor
	fin = fdopen(*fdp, "r");
	if(fin == NULL) { perror("client fin"); pthread_exit(NULL); }
	fout = fdopen(*fdp, "w");
	if(fout == NULL) { perror("client fout"); pthread_exit(NULL); }

	//List chat rooms for client
	fputs("Chat Rooms:\n\n", fout);
	jrb_traverse(tmpJRB, rooms)
	{
		fputs(tmpJRB->key.s, fout);
		fputs(":", fout);

		//List current users (clients) in each room
		pthread_mutex_lock(&lock);
		dll_traverse(tmpDllist, ((Room*)tmpJRB->val.v)->clients)
		{
			fputs(" ", fout);
			fputs(((Client*)tmpDllist->val.v)->name, fout);
		}

		pthread_mutex_unlock(&lock);
		fputs("\n", fout);
	}

	//Get user name from client
	fputs("\nEnter your chat name (no spaces):\n", fout);
	if(fflush(fout) == EOF) { freeClient(fdp, NULL, NULL); pthread_exit(NULL); }
	if(fgets(s, 1000, fin) == NULL) { freeClient(fdp, NULL, NULL); pthread_exit(NULL); }
	username = malloc(sizeof(char) * strlen(s));
	memcpy(username, s, strlen(s));
	username[strlen(s) - 1] = '\0';

	//Get room name from client
	fputs("Enter chat room:\n", fout);
	if(fflush(fout) == EOF) { freeClient(fdp, username, NULL); pthread_exit(NULL); }
	if(fgets(s, 1000, fin) == NULL) { freeClient(fdp, username, NULL); pthread_exit(NULL); }
	roomName = malloc(sizeof(char) * strlen(s));
	memcpy(roomName, s, strlen(s));
	roomName[strlen(s) - 1] = '\0';
	
	//Find room
	room = jrb_find_str(rooms, roomName);
	if(room == NULL) { freeClient(fdp, username, roomName); pthread_exit(NULL); }

	//Add client to room and notify room of join
	user = malloc(sizeof(Client));
	user->name = username;
	user->fout = fout;
	pthread_mutex_lock(&lock);
	dll_append(((Room*)room->val.v)->clients, new_jval_v(user));
	pthread_mutex_unlock(&lock);
	sprintf(s, "%s has joined\n", username);
	sendInput((Room*)room->val.v, s);

	//Allow typing in chat room until EOF
	while(fgets(s, 1000, fin) != NULL)
	{
		sprintf(s2, "%s: %s\n", username, s);
		input = malloc(sizeof(char) * strlen(s2));
		memcpy(input, s2, strlen(s2));
		input[strlen(s2) - 1] = '\0';
		sendInput((Room*)room->val.v, input);
		free(input);
	}

	//Remove client from room
	pthread_mutex_lock(&lock);
	dll_traverse(tmpDllist, ((Room*)room->val.v)->clients)
	{
		if((Client*)tmpDllist->val.v == user)
		{
			free(user);
			dll_delete_node(tmpDllist);
			break;
		}
	}
	pthread_mutex_unlock(&lock);

	//Signal to room that client left
	sprintf(s, "%s has left\n", username);
	sendInput((Room*)room->val.v, s);
	close(fin);

	//Wait for room output to finish before closing fout
	while(1)
	{
		if(dll_empty(((Room*)room->val.v)->input))
		{
			close(fout);
			break;
		}
	}

	freeClient(fdp, username, roomName);
	return NULL;
}

//Sends the input string to a chat room (locks threads and signals condition)
void sendInput(Room *r, char *input)
{
	pthread_mutex_lock(&lock);
	dll_append(r->input, new_jval_s(strdup(input)));
	pthread_cond_signal(&r->cond);
	pthread_mutex_unlock(&lock);
}

//Frees memory for the client
void freeClient(int *fdp, char *username, char *roomName)
{
	close(*fdp);
	free(fdp);

	if(username != NULL)
		free(username);
	if(roomName != NULL)
		free(roomName);
}
