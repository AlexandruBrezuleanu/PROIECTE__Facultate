typedef struct expr {
  int intvalue; int intvec[100];
  int boolvalue; int boolvec[100];
    char strvalue[100]; char strvec[100][100];
    float floatvalue; float floatvec[100];
    int vecdim;
  int type;//1-int,2-string,3-float,4-bool
  int isconst;
  int unde;//0-global 1-main
  char idul[100];
} expr;
typedef struct denumire {
     char denum[100];
     int tip;
     int unde;
} denumire;
typedef struct functii {
  char tipul[100][100];
  char numepar[100][100];
  char numefct[100];
  char tipreturn[100];
  int nrpar;
  int unde;//0-global 1-main
} functii;

typedef struct arbore{
    char tip[100];
    int nod_stang,nod_drept,val;
} arbore;
