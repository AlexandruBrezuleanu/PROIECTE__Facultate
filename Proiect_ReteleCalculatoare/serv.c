#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <sqlite3.h>
#include <graphics.h>

#undef printf
#define MAXINFO 10000
#define MAX_CLIENTS 100
#define MSGLG 1000
#define NAMELG 32

char vremea[MAXINFO];
char carb[MAXINFO];
char sport[MAXINFO];
static _Atomic unsigned int cli_count = 0;
static int uid = 1;
int gd = DETECT,gm;

//Structura pentru client
typedef struct{
	struct sockaddr_in address;
	int sockfd;
	int uid;
	char name[32];
	int info;//0-nu e abonat la stiri ; 1-altfel
	int nrstr;//numarul strazii
	int speed;//viteza
	int borna;//unde pe strada se afla
	int xpoz,ypoz;//pozitia in mapa
} client_t;

client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int min(int x,int y)
{
	 if(x>y) return y;
	 return x;
}
//Adaug clientul in coada
void queue_add(client_t *cl){
   pthread_mutex_lock(&clients_mutex);
	for(int i=0; i < MAX_CLIENTS; ++i){
		if(!clients[i]){
			clients[i] = cl;
			break;
		}
	}
   pthread_mutex_unlock(&clients_mutex);
}

//Scot clientul din coada
void queue_remove(int uid){
	pthread_mutex_lock(&clients_mutex);
	for(int i=0; i < MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid == uid){
				clients[i] = NULL;
				break;
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}
void verticaltext(char *s,int x,int y)
{
   char nou[20];
   int lg=strlen(s);
   for(int i=0;i<lg;i++)
   {
    sprintf(nou,"%c",s[i]);
    if(nou[0]>='A'&&nou[0]<='Z') outtextxy(x-3,y,nou);
    else outtextxy(x-2,y,nou);
    y+=10;
   }
}
int callbackdraw(void *NotUsed, int argc, char **argv, char **azColName) {
    int x1=atoi(argv[0]),y1=atoi(argv[1]),x2=atoi(argv[2]),y2=atoi(argv[3]);
    line(x1,y1,x2,y2);
    return 0;
}
//desenez mapa
void drawmap()
{
	 cleardevice();
	 sqlite3 *db;
   char *err_msg = 0;
   int rc = sqlite3_open("bazedate.db", &db);
	 char *sql = "SELECT x1,y1,x2,y2 FROM Map";      
   rc = sqlite3_exec(db, sql, callbackdraw, 0, &err_msg);
   sqlite3_close(db);
   //scriu numele strazilor
     verticaltext("Str Morilor",30,160);
     verticaltext("Str Unirii",185,165);
     verticaltext("Str Olaru",495,255);
     verticaltext("Str",340,84);verticaltext("Obor",338,146);
     verticaltext("Str",608,162);verticaltext("Titan",608,232);
     outtextxy(272,25,"Str.");outtextxy(343,25,"Carpati");
     outtextxy(118,105,"Str.");outtextxy(187,105,"Silvestru");
     outtextxy(430,185,"Str.");outtextxy(495,185,"Victoriei");
     outtextxy(260,265,"Str. Libertatii");
     outtextxy(280,390,"Str. Constitutiei");
     setcolor(BROWN);
     bar(150,430,490,470);
     outtextxy(220,450,"MONITORIZAREA TRAFICULUI");
     setcolor(WHITE);
   //
   char idul[10];
   pthread_mutex_lock(&clients_mutex);
   for(int i=0; i < MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->nrstr!=0){
				memset(idul,0,sizeof(idul));
				sprintf(idul,"%d",clients[i]->uid);
				setcolor(BROWN);
        circle(clients[i]->xpoz,clients[i]->ypoz,10);
        floodfill(clients[i]->xpoz,clients[i]->ypoz,BROWN);
        setcolor(WHITE);
        outtextxy(clients[i]->xpoz-5,clients[i]->ypoz-5,idul);
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);

}
int callbackinit(void *NotUsed, int argc, char **argv, char **azColName) {
    int id=atoi(argv[0]),str=atoi(argv[1]),maxspeed=atoi(argv[2]),lg=atoi(argv[3]);
    int x1=atoi(argv[4]),y1=atoi(argv[5]),x2=atoi(argv[6]),y2=atoi(argv[7]),drawx,drawy;
    int newspeed=maxspeed-20+rand()%31;
    int newlg=rand()%lg;
    int catre;
    char mesaj[200];
  if(x1==x2)
  {
  	drawx=x1;
  	if(y1>y2)
  	drawy=y1-newlg/20;
    else drawy=y1+newlg/20; 
  }
  else if(y1==y2)
  {
  	drawy=y1;
  	if(x1>x2)
  		drawx=x1-newlg/20;
  	else drawx=x1+newlg/20;
  }
  pthread_mutex_lock(&clients_mutex);
  for(int i=0; i < MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid == id){
				clients[i]->nrstr=str;
				clients[i]->speed=newspeed;
				clients[i]->borna=newlg;
				clients[i]->xpoz=drawx;clients[i]->ypoz=drawy;
				catre=clients[i]->sockfd;
				break;
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
	drawmap();
  if(newspeed<=maxspeed)
  sprintf(mesaj,"Va aflati pe %s. Viteza maxima admisa pe aceasta strada este %d km\\h.\nCirculati cu o viteza regulamentara: %d km\\h.",argv[8],maxspeed,newspeed);
  else
  sprintf(mesaj,"Va aflati pe %s. Viteza maxima admisa pe aceasta strada este %d km\\h.\nATENTIE! Circulati cu o viteza neregulamentara: %d km\\h.",argv[8],maxspeed,newspeed);	
  if(write(catre, mesaj, strlen(mesaj)+1) < 0){
		perror("ERROR: write to descriptor failed");
	}
  return 0;
}

//setez pozitia clientului cand se conecteaza
void client_set_poz(int id){
   int str=rand()%56+1;//o strada random
   char strada[10],idul[10];
   sprintf(strada,"%d",str);
   sprintf(idul,"%d",id);
   sqlite3 *db;
   char *err_msg = 0;
   int rc = sqlite3_open("bazedate.db", &db);
	 char sql[300] = "SELECT ";  
	 strcat(sql,idul);strcat(sql,",");strcat(sql,strada);strcat(sql,",VitezaMax,Lungime,x1,y1,x2,y2,Name FROM Map WHERE Nr=");strcat(sql,strada);    
   rc = sqlite3_exec(db, sql, callbackinit, 0, &err_msg);
   sqlite3_close(db);
}
int callbackvreme(void *NotUsed, int argc, char **argv, char **azColName) {
    
    for (int i = 0; i < argc; i++) {
        strcat(vremea,azColName[i]);
        strcat(vremea,":");
        strcat(vremea,argv[i]);
        strcat(vremea," | ");
    }
    strcat(vremea,"\n"); 
    return 0;
}
int callbackcarb(void *NotUsed, int argc, char **argv, char **azColName) {
    
    for (int i = 0; i < argc; i++) {
        strcat(carb,azColName[i]);
        strcat(carb,":");
        strcat(carb,argv[i]);
        strcat(carb," | ");
    }
    strcat(carb,"\n"); 
    return 0;
}
int callbacksport(void *NotUsed, int argc, char **argv, char **azColName) {
    
    strcat(sport,argv[0]);
    strcat(sport,":  ");
    strcat(sport,argv[1]);
    strcat(sport,"\n"); 
    strcat(sport,"\n"); 
    return 0;
}

//Trimit mesaj catre toti clientii in afara de expediator
void send_message(char *s, int uid){
	pthread_mutex_lock(&clients_mutex);
	for(int i=0; i<MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid != uid){
				if(write(clients[i]->sockfd, s, strlen(s)+1) < 0){
					perror("ERROR: write to descriptor failed");
					break;
				}
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

int callbackaccident(void *NotUsed, int argc, char **argv, char **azColName) {
   int uid=atoi(argv[0]);
   char mesaj[500]="";
   if(strcmp(argv[1],"accident")==0||strcmp(argv[1],"blocaj")==0)
   sprintf(mesaj,"Atentie! Clientul %s[%d] a raportat un %s pe %s. Circulati cu viteza redusa.",argv[2],uid,argv[1],argv[3]);
   else if(strcmp(argv[1],"radar")==0||strcmp(argv[1],"filtru")==0)
   	sprintf(mesaj,"Atentie! Clientul %s[%d] a raportat un %s al politiei rutiere pe %s.",argv[2],uid,argv[1],argv[3]);
   pthread_mutex_lock(&clients_mutex);
   for(int i=0; i<MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid != uid){
				if(write(clients[i]->sockfd, mesaj, strlen(mesaj)+1) < 0){
					perror("ERROR: write to descriptor failed");
					break;
				}
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
   return 0;
}

//trimit informatii despre accidente,blocaje,radar,filtre ale politiei
void info_send_to_all(char s[40], int uid,char cliname[40],int strada){
	  
  sqlite3 *db;
  char *err_msg = 0;
  char idul[10],nrstrada[10];
  sprintf(idul,"%d",uid);
  sprintf(nrstrada,"%d",strada);
  int rc = sqlite3_open("bazedate.db", &db);
  char sql[300] = "SELECT ";
  strcat(sql,idul);strcat(sql,",'");strcat(sql,s);strcat(sql,"','");strcat(sql,cliname);strcat(sql,"',Name FROM Map WHERE Nr=");strcat(sql,nrstrada);   
  rc = sqlite3_exec(db, sql, callbackaccident, 0, &err_msg);
  sqlite3_close(db);
}

//Primeste informatii despre vreme, sport, combustibil
void info_get(char *s, int fdsock){
  sqlite3 *db;
  char *err_msg = 0;
  int rc = sqlite3_open("bazedate.db", &db);

	if(strstr(s,"vreme")!=NULL)
	{
    memset(vremea,0,sizeof(vremea));
    char *sql = "SELECT * FROM Vreme";      
    rc = sqlite3_exec(db, sql, callbackvreme, 0, &err_msg);
    sqlite3_close(db);
    vremea[strlen(vremea)-1]=0;
    if(write(fdsock, vremea, strlen(vremea)+1 )<0)
					perror("ERROR: write to descriptor failed");
	}
	else if(strstr(s,"carburant")!=NULL)
	{
    memset(carb,0,sizeof(carb));
    char *sql = "SELECT * FROM Benzinarii";      
    rc = sqlite3_exec(db, sql, callbackcarb, 0, &err_msg);
    sqlite3_close(db);
    carb[strlen(carb)-1]=0;
    if(write(fdsock, carb, strlen(carb)+1 )<0)
					perror("ERROR: write to descriptor failed");
	}
	else if(strstr(s,"sport")!=NULL)
	{
    memset(sport,0,sizeof(sport));
    char *sql = "SELECT * FROM Sport";      
    rc = sqlite3_exec(db, sql, callbacksport, 0, &err_msg);
    sqlite3_close(db);
    sport[strlen(sport)-1]=0;
    if(write(fdsock, sport, strlen(sport)+1 )<0)
					perror("ERROR: write to descriptor failed");
	}
}
int callbackupdate2(void *NotUsed, int argc, char **argv, char **azColName) {
	 int id=atoi(argv[0]),str=atoi(argv[1]),newborna=atoi(argv[2]);
    int x1=atoi(argv[3]),y1=atoi(argv[4]),x2=atoi(argv[5]),y2=atoi(argv[6]),drawx,drawy;
    int maxspeed=atoi(argv[7]);
    int newspeed=maxspeed-20+rand()%31;
    int catre;
    char mesaj[200];
  if(x1==x2)
  {
  	drawx=x1;
  	if(y1>y2)
  		drawy=y1-newborna/20;
  	else drawy=y1+newborna/20;
  }
  else if(y1==y2)
  {
  	drawy=y1;
  	if(x1>x2)
  		drawx=x1-newborna/20;
  	else drawx=x1+newborna/20;
  }
  pthread_mutex_lock(&clients_mutex);
  for(int i=0; i < MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid == id){
				clients[i]->nrstr=str;
				clients[i]->speed=newspeed;
				clients[i]->borna=newborna;
				clients[i]->xpoz=drawx;clients[i]->ypoz=drawy;
				catre=clients[i]->sockfd;
				break;
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
  drawmap();
  if(newspeed<=maxspeed)
    sprintf(mesaj,"Va aflati pe %s. Viteza maxima admisa pe aceasta strada este %d km\\h.\nCirculati cu o viteza regulamentara: %d km\\h.",argv[8],maxspeed,newspeed);
  else
    sprintf(mesaj,"Va aflati pe %s. Viteza maxima admisa pe aceasta strada este %d km\\h.\nATENTIE! Circulati cu o viteza neregulamentara: %d km\\h.",argv[8],maxspeed,newspeed);	
  if(write(catre, mesaj, strlen(mesaj)+1) < 0){
		perror("ERROR: write to descriptor failed");
	    }
  return 0;
}
int callbackupdate1(void *NotUsed, int argc, char **argv, char **azColName) {
	int uid1=atoi(argv[0]),nrstr1=atoi(argv[1]),borna1=atoi(argv[2]),crtspeed=atoi(argv[3]);
	int x1=atoi(argv[4]),y1=atoi(argv[5]),x2=atoi(argv[6]),y2=atoi(argv[7]),lgmax=atoi(argv[8]),maxspeed1=atoi(argv[9]);
	int newborna,newspeed,drawx,drawy,catre;
	char mesaj[200];
	newborna=borna1+((crtspeed*1000)/60);
	if(newborna<=lgmax)//ramane pe aceiasi strada
	{
			if(x1==x2)
	  {
	  	drawx=x1;
	  	if(y1>y2)
  		drawy=y1-newborna/20;
  	  else drawy=y1+newborna/20;
	  }
	  else if(y1==y2)
	  {
	  	drawy=y1;
	  	if(x1>x2)
  		drawx=x1-newborna/20;
  	  else drawx=x1+newborna/20;
	  }
	  newspeed=maxspeed1-20+rand()%31;
	  pthread_mutex_lock(&clients_mutex);
	  for(int i=0; i < MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid == uid1){
				clients[i]->speed=newspeed;
				clients[i]->borna=newborna;
				clients[i]->xpoz=drawx;clients[i]->ypoz=drawy;
				catre=clients[i]->sockfd;
				break;
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
	drawmap();
	if(newspeed<=maxspeed1)
      sprintf(mesaj,"Va aflati pe %s. Viteza maxima admisa pe aceasta strada este %d km\\h.\nCirculati cu o viteza regulamentara: %d km\\h.",argv[10],maxspeed1,newspeed);
    else
      sprintf(mesaj,"Va aflati pe %s. Viteza maxima admisa pe aceasta strada este %d km\\h.\nATENTIE! Circulati cu o viteza neregulamentara: %d km\\h.",argv[10],maxspeed1,newspeed);	
    if(write(catre, mesaj, strlen(mesaj)+1) < 0){
		perror("ERROR: write to descriptor failed");
	}
}
  else//am schimbat strada
  {
      newborna-=lgmax;
      sqlite3 *db;
	  char *err_msg = 0;
	  char newbornastr[20];
	  sprintf(newbornastr,"%d",newborna);
	  int rc = sqlite3_open("bazedate.db", &db);
	  char sql[300];
	  sprintf(sql,"SELECT %s,Nr,%s,x1,y1,x2,y2,VitezaMax,Name FROM Map WHERE Start=%s and Finish!=%s ORDER BY RANDOM() LIMIT 1",argv[0],newbornastr,argv[12],argv[11]);
	  rc = sqlite3_exec(db, sql, callbackupdate2, 0, &err_msg);
	  sqlite3_close(db);
  }
	return 0;
}

//Clientul trimite update la server cu noua sa viteza si pozitie
void updated(int id,int nrstr,int bornaa,int vit){
  sqlite3 *db;
  char *err_msg = 0;
  char idul[10],nrstrada[10],borna[10],sped[10];
  sprintf(idul,"%d",id);
  sprintf(nrstrada,"%d",nrstr);
  sprintf(borna,"%d",bornaa);
  sprintf(sped,"%d",vit);
  int rc = sqlite3_open("bazedate.db", &db);
  char sql[300];
  sprintf(sql,"SELECT %s,%s,%s,%s,x1,y1,x2,y2,Lungime,VitezaMax,Name,Start,Finish FROM Map WHERE Nr=%s",idul,nrstrada,borna,sped,nrstrada);
  rc = sqlite3_exec(db, sql, callbackupdate1, 0, &err_msg);
  sqlite3_close(db);
}


//Tratez fiecare client
void *handle_client(void *arg){
	char text[MSGLG];
	char name[NAMELG];
	int leave_flag = 0;

	cli_count++;
	client_t *cli = (client_t *)arg;

	// Name
	if(recv(cli->sockfd, name, NAMELG, 0) <= 0 || strlen(name) <  2 || strlen(name) >= 32){
		printf("Numele nu a fost introdus sau a fost introdus gresit.\n");
		fflush(stdout);
		leave_flag = 1;
	} else{
		strcpy(cli->name, name);
		strcat(text,cli->name);
        strcat(text," s-a logat cu succes.");
        printf("%s\n", text);
		fflush(stdout);

        //ii trimit mesaj cu id-ul sau la inceput
		char initial[100];
		sprintf(initial,"Id-ul asignat de server la conectarea in aplicatie este %d.",cli->uid);
		if(write(cli->sockfd, initial, strlen(initial)+1)< 0)
			perror("ERROR: write to descriptor failed");

		client_set_poz(cli->uid);
		send_message(text, cli->uid);
	}

	bzero(text, MSGLG);

	while(1){
		if (leave_flag) {
			break;
		}
        //primesc mesaj de la client si in functie de comanda o tratez
		int receive = recv(cli->sockfd, text, MSGLG, 0);
		if (receive > 0)
		{
			if(strlen(text) > 0)
			{
				if(strcmp(text,"info_set")==0)
				{
					cli->info=1;//e abonat
                    if(write(cli->sockfd, "Acum poti primi informatii despre vreme, evenimente sportive, preturi de combustibili",86)< 0)
					    perror("ERROR: write to descriptor failed");
				    printf("[%s(%d)]: %s\n", cli->name,cli->uid,text);
				    fflush(stdout);
				}
				else
				if(strcmp(text,"info_get_vreme")==0||strcmp(text,"info_get_sport")==0||strcmp(text,"info_get_carburant")==0)
				{
					   if(cli->info==1)
                         info_get(text,cli->sockfd);
                       else
             	       if(write(cli->sockfd, "NU esti abonat in a primi informatii despre vreme, evenimente sportive, preturi de combustibili",96)< 0)
					      perror("ERROR: write to descriptor failed");
                    printf("[%s(%d)]: %s\n", cli->name,cli->uid,text);
				    fflush(stdout);
				}
				else if(strcmp(text,"blocaj")==0||strcmp(text,"accident")==0||strcmp(text,"radar")==0||strcmp(text,"filtru")==0)
				{
				      info_send_to_all(text, cli->uid,cli->name,cli->nrstr);
				      printf("[%s(%d)]: %s\n", cli->name,cli->uid,text);
				      fflush(stdout);
			    }
			  else if(strcmp(text,"update")==0)
			  {
                 updated(cli->uid,cli->nrstr,cli->borna,cli->speed);
				 printf("[%s(%d)]: %s\n", cli->name,cli->uid,text);
				 fflush(stdout);
			  }
			  else if(strcmp(text,"id_get")==0)
			  {
			     char newmsg[100];
			     sprintf(newmsg,"Id-ul asignat de server clientului %s este %d.",cli->name,cli->uid);
			     if(write(cli->sockfd, newmsg, strlen(newmsg)+1)< 0)
					      perror("ERROR: write to descriptor failed");
                 printf("[%s(%d)]: %s\n", cli->name,cli->uid,text);
				 fflush(stdout);
			  }
			  else if(strcmp(text, "exit") == 0)
			  {
			  	strcpy(text,cli->name);
		    	strcat(text," s-a delogat cu succes.");
		    	printf("%s\n", text);
				fflush(stdout);
				send_message(text, cli->uid);
				leave_flag = 1;
			  }
			}
		} 
		else 
			if (receive == 0 )//daca clientul a dat CTRL+C
			{
			strcpy(text,cli->name);
    	    strcat(text," s-a delogat cu succes.");
    	    printf("%s\n", text);
			fflush(stdout);
			send_message(text, cli->uid);
			leave_flag = 1;
		  } 
		  else //eroare la recv	  
		  {
			printf("ERROR: -1\n");
			leave_flag = 1;
		  }

		bzero(text, MSGLG);
	}

  //Elimin clientul din coada si detasez thread-ul
  close(cli->sockfd);
  queue_remove(cli->uid);
  free(cli);
  cli_count--;
  pthread_detach(pthread_self());

  return NULL;
}

int main(int argc, char **argv){
	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char *ip = "127.0.0.1";
	int port = atoi(argv[1]);
	int option = 1;
	int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    pthread_t tid;

  /* Setari socket */
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  //stabilirea familiei de socket-uri
  serv_addr.sin_family = AF_INET;
  //stabilirea adresei
  serv_addr.sin_addr.s_addr = inet_addr(ip);
  //utilizam un port utilizator
  serv_addr.sin_port = htons(port);

  /* Ignor SIGPIPE */
	signal(SIGPIPE, SIG_IGN);
  //utilizarea optiunii SO_REUSEADDR ce permite repornirea imediata a serverului dupa inchidere si SO_REUSEPORT 
	if(setsockopt(listenfd, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option)) < 0){
		perror("ERROR: setsockopt failed");
    return EXIT_FAILURE;
	}

	/* Bind */
  if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("ERROR: Socket binding failed");
    return EXIT_FAILURE;
  }

  /* Listen */
  if (listen(listenfd, 10) < 0) {
    perror("ERROR: Socket listening failed");
    return EXIT_FAILURE;
	}
	initgraph(&gd,&gm,NULL);//deschid consola grafica
    drawmap();
    srand(time(NULL));//initializez random seed-ul
    sleep(1);
	printf("=== BINE ATI VENIT! ===\n");
  
	while(1){
		socklen_t clilen = sizeof(cli_addr);
		connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);

		//Verific daca a fost atins numarul maxim de clienti
		if((cli_count + 1) == MAX_CLIENTS){
			printf("Numarul maxim de clienti a fost atins. Respins: ");
			printf(":%d\n", cli_addr.sin_port);
			close(connfd);
			continue;
		}

		/* Completez campurile structurii client*/
		client_t *cli = (client_t *)malloc(sizeof(client_t));
		cli->address = cli_addr;
		cli->sockfd = connfd;
		cli->uid = uid++;
        cli->info=0;//initial nu este conectat pentru a primi informatii
        cli->nrstr=0;cli->speed=0;cli->borna=0;cli->xpoz=0;cli->ypoz=0;
		//Adaug clientul in coada si creez thread-ul
		queue_add(cli);
		pthread_create(&tid, NULL, &handle_client, (void*)cli);

		sleep(1);
	}

	return EXIT_SUCCESS;
}
