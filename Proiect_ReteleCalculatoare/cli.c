#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#define MSGLG 1000
#define NAMELG 32

// Global variables
volatile sig_atomic_t flag = 0;
int sockfd = 0;
char name[NAMELG];

void catch_ctrl_c_and_exit(int sig) {
    flag = 1;
}

/* trimit comenzi la sever*/
void send_msg_handler() {
  char text[MSGLG] = {};
	char comanda[MSGLG + 32] = {};

  while(1) {
  	//primesc comanda de la tastatura
  	fgets(text, MSGLG, stdin);
    text[strlen(text)-1]=0;
    if (strcmp(text, "exit") == 0) {
			break;
    } else {
    	//verific ca textul sa respecte formatul comenzii
    	if(strcmp(text,"accident")==0||strcmp(text,"blocaj")==0||strcmp(text,"radar")==0||strcmp(text,"filtru")==0||strcmp(text,"info_set")==0||strcmp(text,"info_get_sport")==0||strcmp(text,"info_get_vreme")==0||strcmp(text,"info_get_carburant")==0||strcmp(text,"id_get")==0)
    	{
    	strcat(comanda,text);
      send(sockfd, comanda, strlen(comanda)+1, 0);
      }
      else
      	printf("Comanda introdusa este gresita\n");
    }

		bzero(text, MSGLG);
    bzero(comanda, MSGLG + 32);
  }
  catch_ctrl_c_and_exit(2);
}

/*primesc raspuns de la server*/
void recv_msg_handler() {
  char text[MSGLG] = {};
  while (1) {
		int receive = recv(sockfd, text, MSGLG, 0);
    if (receive > 0) {
      printf("[server]: %s\n\n", text);
      fflush(stdout);
    } else if (receive <= 0) {//0 sau eroarae
			break;
    } 
		memset(text, 0, sizeof(text));
  }
}


int main(int argc, char **argv){
	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char *ip = "127.0.0.1";
	int port = atoi(argv[1]);

	signal(SIGINT, catch_ctrl_c_and_exit);

	printf("Va rugam, introduceti-va numele: ");
	fflush(stdout);

  fgets(name, NAMELG, stdin);
  name[strlen(name)-1]=0;

	if (strlen(name) > 32 || strlen(name) < 2){
		printf("Numele trebuie sa aiba intre 2 si 32 de caractere");
		return EXIT_FAILURE;
	}

	struct sockaddr_in server_addr;

	/* Setari socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//stabilirea familiei de socket-uri
  server_addr.sin_family = AF_INET;
  //stabilirea adresei
  server_addr.sin_addr.s_addr = inet_addr(ip);
  //utilizam un port utilizator
  server_addr.sin_port = htons(port);


  // Connectarea la Server
  int err = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (err == -1) {
		printf("ERROR: connect\n");
		return EXIT_FAILURE;
	}

	// Trimit Numele Clientului la server
	send(sockfd, name, NAMELG, 0);

	printf("=== BINE ATI VENIT! ===\n");
	printf("COMENZI POSIBILE:\n");
	printf("=======================================\n");
	printf("* id_get * Pentru a afla id-ul asignat de server la conectarea in aplicatie.\n");
  printf("* accident * Pentru a raporta un accident intalnit in trafic.\n");
  printf("* blocaj * Pentru a raporta un blocaj/obstacol intalnit in trafic.\n");
  printf("* radar * Pentru a raporta o masina de politie dotata cu aparat radar.\n");
  printf("* filtru * Pentru a raporta un filtru al politiei rutiere.\n");
  printf("* info_set * Pentru a va abona in a primi informatii despre vreme, combustibili, diverse evenimente sportive. \n");
  printf("* info_get_vreme * Pentru a primi informatii despre vreme.\n");
  printf("* info_get_sport * Pentru a primi informatii despre evenimente sportive.\n");
  printf("* info_get_carburant * Pentru a primi informatii despre preturile carburantilor.\n");
  printf("=======================================\n");
  fflush(stdout);

	pthread_t send_msg_thread;
	//cream thread-ul care se va ocupa cu trimiterea comenzilor la server
  if(pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0){
		printf("ERROR: pthread\n");
    return EXIT_FAILURE;
	}

	pthread_t recv_msg_thread;
	//cream thread-ul care se va ocupa cu receptarea raspunsurilor primite de la server
  if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0){
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}

  //in thread-ul principal trimit la un interval de 60 de secunde update cu privire la viteza
	char comanda[MSGLG + 32] = {};
	while (1){
		if(flag){
			printf("\nV-ati deconectat cu succes.\n");
			fflush(stdout);
			break;
    }
  	sleep(60);
    strcat(comanda,"update");
    send(sockfd, comanda, strlen(comanda)+1, 0);
		bzero(comanda, MSGLG + 32);
	}

	close(sockfd);

	return EXIT_SUCCESS;
}
