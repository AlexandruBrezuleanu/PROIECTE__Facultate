#include <iostream>
#include <fstream>
#include <graphics.h>
#include<stdlib.h>
#include <cmath>
#include <stdio.h>

#include <vector>
#include <queue>
#include <stack>
#define PI 3.1415926535897932384
#define MAX 1200
using namespace std;
ofstream fout("txt.out");
int catte;
int dl[]= {-1,0,1,0};
int dc[]= {0,1,0,-1};
struct elem
{
    int l,c;
} prec[MAX][MAX];///elementul precedent
int a[MAX][MAX];///matricea
int cc[MAX][MAX];///numarul de colturi
bool lee(int i,int j,int i1,int j1);
queue<pair<int,int>>q;
void bordare();
int n=680,m=880;
int unu,doi;
int nodstart=-1,nodend=-1,x1,y1,x2,y2;///nodurile de la care pleca o legatura
int nodlegat;///vad daca nodlegat=2 sterg linia
struct liniile/// retinerea legaturiilor
{
    int st,dr,ndst,nddr;///nodurile de start/final cu numarul nodurilor de legatura
    int nrpuncte;
    vector<elem>pozz;///pozitiile prin care trece linia
};
vector<liniile>linn;
vector<int>restante;

struct
{
    int vst, vdr, nvst, nvdr;
} pere[101];


void nu_obstacol(double xst,double yst,double xdr,double ydr);
void obstacol(double xst,double yst,double xdr,double ydr);
void fara_obstacol(double xst,double yst,double xdr,double ydr);
double distt(double xa,double ya,double xb,double yb);
void desenlinie(double xa,double ya,double xb,double yb);
void golire();
int selectarelegaturi(int i,double x, double y);
void golirematrice();
bool verificareLegatura(double xst,double yst,double xdr,double ydr);
bool verificareLegaturaMutare(int crt,double xst,double yst);
void bordarelinieleg(int x,int y);
void sterglinie();
void refacerelegaturi2();
bool refacerelegaturi();
void stergerelegaturi();
bool verificapunct(int x,int y,int x1,int y1);
bool nu_are_linii(int Obiect);


int but; ///aici calculez care buton a fost apasat
int contor = 0; ///numarul pieselor ce au fost adaugate
int selectat = 0; ///daca o piesa a fost selectata sau nu
char sir[30];///numele butoanelor
int Obiect_selectat=-1; ///retin obiectul care a fost selectat
bool ook;/// variabila care verifica daca o piesa e creata sau nu
int nr_limba; ///1 - romana, 2 - engleza, 3 - germana, 4 - franceza
int nr_culoare; ///apesi culoarea, se schimba culoarea scrisului, pieselor, liniile (15 - alb, 9 - blue, 10 - green, 11 - cyan, 14 - yellow, 5 - magenta)
bool ecranSchimbat = false; ///cand schimb ecranul pentru instructiuni
char fisier[101]; ///fisierul de salvare
char auxFisier[101]; ///un char auxiliar pentru a putea reprezenta pe ecran informatia
char folder[180]; ///pentru a pune fisiere in folder
int numarFisiere = 0; ///pentru optiunea de a deschide fisiere
int retinvaloarea;

struct PiesaNoua
{
    double x_st, x_dr, y_st, y_dr, x, y,tip,dimm,ang;
    int valoare;
    struct
    {
        int nr_noduri;
        struct
        {
            double x_nod, y_nod;
            bool okk=0;
        } nod[4];
    };
} v[101];

void buton(); ///unde apesi butoanele
void creazaPiesa(int but,double x,double y,double dim);///creeaza o piesa daca e posibil
void DateNoi(double x, double y, double val_x, double val_y,int carePiesa);

bool verificareMargine(double x, double y, double val_x, double val_y);
bool verificareMargineMutare(int Obiect,double xst,double yst,double xdr,double ydr);
int selectare(double x, double y); ///verifica daca am selectat o piesa sau nu si intoarce numarul ei
void selectare_Piesa(int Obiect);
void deselectare_Piesa(int Obiect);
void redesenarePiese(); ///cand termini de citit manualul de instructiuni, sa te intorci la spatiul de lucru
void MarimeFizica();

void instructiune(int nr_culoare, int nr_limba);

void Piese(int aux); ///ca sa apara piesele.
void visualPiesa(double x, double y, int but); ///sa vezi cum arata piesa
void Schimb_Piesa(int i,int j);///schimb piesa i cu piesa j
void stergere_Piesa(int Obiect);///sterg piesa
void stergere_PiesaTotal(int Obiect);/// sterg piesa inclusiv cu legaturi


void cautPiesa_si_Desen(int crt);///cand vreau sa mut o piesa o desenez la dimensiunea si unghiul sau
void salvareDate(); ///salvarea datelor in fisiere
void StergereEcran(); ///stergi spatiul de lucru
void intrebareUtilizator(); ///inainte ca utilizatorul sa faca o noua actiune cu fisierele, intrebam daca datele actuale vrea sa le salveze

double val=0,dim=0.7;///val-unghiul la care se roteste iar dim-dimensiunea figurii
double speed=10;///viteza de deplasare
double cosinus(double val)
{
    return cos(val*PI/180.0);
}
double sinus(double val)
{
    return sin(val*PI/180.0);
}
double newx(double xn,double yn,double xv,double yv)///coordonata x dupa rotire
{
    double xrez=xn,yrez=yn;
    xrez-=xv;
    yrez-=yv;
    xrez=xrez*cosinus(val)-sinus(val)*yrez;
    xrez+=xv;
    return xrez;
}
double newy(double xn,double yn,double xv,double yv)///coordonata y dupa rotire
{
    double xrez=xn,yrez=yn;
    xrez-=xv;
    yrez-=yv;
    yrez=sinus(val)*xrez+cosinus(val)*yrez;
    yrez+=yv;
    return yrez;
}

///piesele desenate
void generator(double x,double y,double c);
void intrerupator(double x,double y,double c);
void tranzistor(double x,double y,double c);
void rezistor(double x,double y,double c);
void bec(double x,double y,double c);
void baterie(double x,double y,double c);
void voltmetru(double x,double y,double c);
void ampermetru(double x,double y,double c);
void difuzor(double x,double y,double c);
///grafica aplicatiei
void limba();
void culoare();
void redesenarePiese();
void redesenarePiese2();
int main()
{
    initwindow(1015, 800);
    bordare();
    nr_limba = 1;
    fisier[0]='1';
    nr_culoare = 15;
    culoare();
    buton();
    getch();
    closegraph();
    return 0;
}
void instructiune(int nr_culoare, int nr_limba) ///aici scrii instructiunile
{
    setcolor(nr_culoare);
    outtextxy(40, 475, "Credits:");
    outtextxy(20, 500, "Brezuleanu Alexandru");
    outtextxy(20, 525, "Mihai");
    outtextxy(20, 560, "Opariuc Rares Ioan");
    setfillstyle(SOLID_FILL, BLACK);
    bar(50, 185, 160, 215);
    bar(50, 360, 160, 390);
    bar(175, 0, 1015, 800);
    if (nr_limba == 1) ///romana
    {

        outtextxy(60, 195, "Limba");
        readimagefile("romana.jpg", 30, 220, 125, 280);
        outtextxy(50, 360, "Culoare");

        outtextxy(200, 30, "Electron - Vizualizator de scheme electronice");
        outtextxy(200, 80, "Aici este manualul de instructiuni a aplicatiei.");
        outtextxy(200, 110, "Pentru a pune piesele, apasati butonul cu numele piesei si apasati pe spatiul de lucru pentru a crea piesa.");
        outtextxy(200, 140, "In coltul din dreapta sus este piesa selectata curenta.");
        outtextxy(200, 170, "In partea din dreapta a aplicatiei, sunt butoanele care modifica piesele sau aplicatia.");
        outtextxy(200, 200, "Butonul Marimea fizica - Selectati o piesa si apasati in spatiul aferent.");
        outtextxy(200, 220, "Tastati valoarea si apasati pe butonul de enter pentru a se retine valoarea.");
        outtextxy(200, 250, "Butonul Stergere - Selectati o piesa si apasati pe buton pentru ca piesa sa fie stearsa.");
        outtextxy(200, 280, "Butonul Marire/Rotire - Selectati o piesa si apasati sageti/plus/minus pentru a roti/mari/micsora piesa selectata.");
        outtextxy(200, 310, "Butonul Salvare/Salvare ca/ Deschide - Daca vreti sa salvati proiectul sau sa deschideti un alt proiect,");
        outtextxy(200, 330, "apasati pe butoanele mentionate.");
        outtextxy(200, 360, "Limba - Daca vreti sa schimbati limba, apasati pe sageti pentru a schimba in romana, engleza, germana sau franceza.");
        outtextxy(200, 390, "Culori - Daca vreti sa schimbati culoarea aplicatiei, apasati pe unul dintre cele 6 patratele colorate.");
        outtextxy(200, 420, "Butonul Sterge ecran - Daca vreti sa stergeti tot spatiul de lucru, apasati pe butonul mentionat.");
        outtextxy(200, 450, "Butonul Inchide - Daca ati terminat de lucru, inchideti aplicatia.");
    }
    else if (nr_limba == 2) ///engleza
    {
        outtextxy(60, 195, "Language");
        readimagefile("engleza.jpg", 30, 220, 125, 280);
        outtextxy(50, 360, "Colour");

        outtextxy(200, 30, "Electron - Electronic schema viewer");
        outtextxy(200, 80, "Here is the instruction manual of the application.");
        outtextxy(200, 110, "To put the parts, press the button with the part name and press on the workspace to create the part.");
        outtextxy(200, 140, "In the upper right corner is the currently selected track.");
        outtextxy(200, 170, "On the right side of the application are the buttons that modify the parts or the application.");
        outtextxy(200, 200, "Physical Size Button - Select a track and click in the space provided,");
        outtextxy(200, 220, "Type the value and press the enter button to hold the value.");
        outtextxy(200, 250, "Delete button - Select a track and press the button to delete the track.");
        outtextxy(200, 280, "Zoom / Rotate button - Select a track and press the / plus / minus arrows to rotate / zoom in / out the selected track.");
        outtextxy(200, 310, "Save/Save As/Open button - If you want to save the project or open another project,");
        outtextxy(200, 330, "press the mentioned buttons.");
        outtextxy(200, 360, "Language - If you want to change the language, click on the arrows to change to Romanian, English, German or French.");
        outtextxy(200, 390, "Colors - If you want to change the color of the application, click on one of the 6 colored squares.");
        outtextxy(200, 420, "Wipe screen button - If you want to wipe the entire workspace, click on the button mentioned.");
        outtextxy(200, 450, "Quit Button - If you have finished working, close the application.");
    }
    else if (nr_limba == 3) ///germana
    {
        outtextxy(60, 195, "Sprache");
        readimagefile("germana.jpg", 30, 220, 125, 280);
        outtextxy(50, 360, "Farbe");

        outtextxy(200, 30, "Electron - Elektronischer Schema-Viewer");
        outtextxy(200, 80, "Hier ist die Bedienungsanleitung der Anwendung.");
        outtextxy(200, 110, "Um die Teile zu platzieren, drücken Sie die Taste mit dem Teilenamen und drücken Sie auf den Arbeitsbereich, ");
        outtextxy(200, 130, "um das Teil zu erstellen.");
        outtextxy(200, 160, "In der oberen rechten Ecke befindet sich die aktuell ausgewählte Spur.");
        outtextxy(200, 190, "Auf der rechten Seite der Anwendung befinden sich die Schaltflächen, mit denen die Teile der Anwendung geändert werden.");
        outtextxy(200, 220, "Schaltfläche Physische Größe - Wählen Sie eine Spur aus und klicken Sie in das dafür vorgesehene Feld,");
        outtextxy(200, 240, "Geben Sie den Wert ein und drücken Sie die Eingabetaste, um den Wert zu halten.");
        outtextxy(200, 270, "Schaltfläche Abwischen - Wählen Sie einen Titel aus und drücken Sie die Taste, um den Titel zu löschen.");
        outtextxy(200, 300, "Schaltfläche Vergrößern/Drehen - Wählen Sie eine Spur aus und drücken Sie die Pfeiltasten / Plus / Minus,");
        outtextxy(200, 320, "um die ausgewählte Spur zu drehen / zu vergrößern / zu verkleinern.");
        outtextxy(200, 350, "Schaltfläche Speichern/Speichern als/Öffnen - Wenn Sie das Projekt speichern oder ein anderes Projekt öffnen möchten,");
        outtextxy(200, 370, "Drücken Sie die genannten Tasten.");
        outtextxy(200, 390, "Sprache - Wenn Sie die Sprache ändern möchten, klicken Sie auf die Pfeile, um zu");
        outtextxy(200, 420, "Rumänisch, Englisch, Deutsch oder Französisch zu wechseln.");
        outtextxy(200, 450, "Farben - Wenn Sie die Farbe der Anwendung ändern möchten, klicken Sie auf eines der 6 farbigen Quadrate.");
        outtextxy(200, 480, "Bildschirmschaltfläche löschen - Wenn Sie den gesamten Arbeitsbereich löschen möchten,");
        outtextxy(200, 500, "klicken Sie auf die angegebene Schaltfläche.");
        outtextxy(200, 530, "Verlassen - Wenn Sie mit der Arbeit fertig sind, schließen Sie die Anwendung.");
    }
    else if (nr_limba == 4) ///franceza
    {
        outtextxy(60, 195, "Langue");
        readimagefile("franceza.jpg", 30, 220, 125, 280);
        outtextxy(50, 360, "Couleur");
        outtextxy(200, 30, "Electron - Visualiseur de schéma électronique");
        outtextxy(200, 80, "Voici le manuel d'instructions de l'application.");
        outtextxy(200, 110, "Pour mettre les pièces, appuyez sur le bouton avec le nom de la pièce et appuyez sur l'espace de travail pour créer la pièce.");
        outtextxy(200, 140, "Dans le coin supérieur droit se trouve la piste actuellement sélectionnée.");
        outtextxy(200, 170, "Sur le côté droit de l'application se trouvent les boutons qui modifient les parties de l'application.");
        outtextxy(200, 200, "Bouton Grandeur physique - Sélectionnez une piste et cliquez dans l'espace prévu,");
        outtextxy(200, 220, "Tapez la valeur et appuyez sur le bouton Entrée pour maintenir la valeur.");
        outtextxy(200, 250, "Bouton Effacer - Sélectionnez une piste et appuyez sur le bouton pour supprimer la piste.");
        outtextxy(200, 280, "Bouton Agrandir/Tourner - Sélectionnez une piste et appuyez sur les flèches / plus / moins pour faire");
        outtextxy(200, 280+20, "pivoter / agrandir / réduire la piste sélectionnée.");
        outtextxy(200, 330, "Bouton Enregistrer/Enregistrer sous/Ouvert - Si vous souhaitez enregistrer le projet ou ouvrir un autre projet,");
        outtextxy(200, 350, "appuyez sur les boutons mentionnés.");
        outtextxy(200, 380, "Langue - Si vous souhaitez changer de langue, cliquez sur les flèches pour passer");
        outtextxy(200, 400, "en roumain, anglais, allemand ou français");
        outtextxy(200, 430, "Couleur - Si vous souhaitez changer la couleur de l'application, cliquez sur l'un des 6 carrés de couleur.");
        outtextxy(200, 460, "Bouton Essuyer l'écran - Si vous souhaitez effacer tout l'espace de travail, cliquez sur le bouton mentionné.");
        outtextxy(200, 490, "Bouton Quitter - Si vous avez fini de travailler, fermez l'application.");
    }
}
void limba()
{
    ///sterg continul patratelor pentru a rescrie cuvintele
    int i;
    setfillstyle(SOLID_FILL, BLACK);
    for (i = 0; i <= 800; i = i + 100)
        bar(i + 1, 1, i + 100 -1, 100 - 1);
    setfillstyle(SOLID_FILL, BLACK);
    for (i = 100; i <= 800; i = i + 100)
        bar(901, i+1, 915 + i-1, 900 + i + 100-1);
    if (nr_limba == 1) ///romana
    {
        ///ca sa rescrii cuvintele, acopar imaginea

        for (i = 0; i <= 8; i++)
        {
            switch (i)
            {
            case 0:
                strcpy(sir, "Generator");
                break;
            case 1:
                strcpy(sir, "Intrerupator");
                break;
            case 2:
                strcpy(sir, "Tranzistor");
                break;
            case 3:
                strcpy(sir, "Rezistor");
                break;
            case 4:
                strcpy(sir, "Bec");
                break;
            case 5:
                strcpy(sir, "Baterie");
                break;
            case 6:
                strcpy(sir, "Voltmetru");
                break;
            case 7:
                strcpy(sir, "Ampermetru");
                break;
            case 8:
                strcpy(sir, "Difuzor");
                break;
            }
            outtextxy(10 + i * 100, 40, sir);
        }
        for (i = 1; i <= 8; i++)
        {
            switch (i)
            {
            case 1:
                outtextxy(935, i * 100 + 75, "Stergere");
                break;
            case 2:
                readimagefile("left_arrow.jpg", 930, 230, 950, 245);
                readimagefile("right_arrow.jpg", 970, 230, 990, 245);
                outtextxy(940, i * 100 + 10, "Rotire");
                break;
            case 3:
                outtextxy(940, 260, "Marire");
                readimagefile("plus.jpg", 960, 275, 990, 295);
                readimagefile("minus.jpg", 930, 275, 950, 295);
                break;
            case 4:
                outtextxy(920, (i-1) * 100 + 25, "Instructiuni");
                outtextxy(920, (i-1) * 100 + 50, "de utilizare");
                break;
            case 5:
                outtextxy(930, (i-1) * 100 + 10, "Fisier nou");
                outtextxy(930, (i-1) * 100 + 40, "Salvare");
                outtextxy(930, (i-1) * 100 + 70, "Deschide");
                break;
            case 6:
                outtextxy(940, (i-1) * 100 + 10, "Limba");
                readimagefile("romana.jpg", 910, 530, 1005, 570);
                readimagefile("left_arrow.jpg", 910, 575, 930, 590);
                readimagefile("right_arrow.jpg", 990, 575, 1010, 590);
                break;
            case 7:
                outtextxy(935, (i-1) * 100 + 15, "Culoare");
                break;
            case 8:
                outtextxy(935, (i-1) * 100 + 65, "Inchide");
                break;
            }
        }
        //(15 - alb, 9 - blue, 10 - green, 11 - cyan, 14 - yellow, 5 - magenta)
        outtextxy(910, 120, "Marime");
        outtextxy(910, 140, "fizica");
        outtextxy(910, 718, "Stergere ecran");
    }
    else if (nr_limba == 2) ///engleza
    {
        for (i = 0; i <= 8; i++)
        {
            switch (i)
            {
            case 0:
                strcpy(sir, "Generator");
                break;
            case 1:
                strcpy(sir, "Switch");
                break;
            case 2:
                strcpy(sir, "Transistor");
                break;
            case 3:
                strcpy(sir, "Resistor");
                break;
            case 4:
                strcpy(sir, "Light bulb");
                break;
            case 5:
                strcpy(sir, "Batery");
                break;
            case 6:
                strcpy(sir, "Voltmeter");
                break;
            case 7:
                strcpy(sir, "Ammeter");
                break;
            case 8:
                strcpy(sir, "Loudspeaker");
                break;
            }
            outtextxy(10 + i * 100, 40, sir);
        }
        for (i = 1; i <= 8; i++)
        {
            switch (i)
            {
            case 1:
                outtextxy(935, i * 100 + 75, "Delete");
                break;
            case 2:
                readimagefile("left_arrow.jpg", 930, 230, 950, 245);
                readimagefile("right_arrow.jpg", 970, 230, 990, 245);
                outtextxy(940, i * 100 + 10, "Rotate");
                break;
            case 3:
                outtextxy(940, 260, "Zoom");
                readimagefile("plus.jpg", 960, 275, 990, 295);
                readimagefile("minus.jpg", 930, 275, 950, 295);
                break;
            case 4:
                outtextxy(920, (i-1) * 100 + 25, "Instructions");
                outtextxy(920, (i-1) * 100 + 50, "for use");
                break;
            case 5:
                outtextxy(930, (i-1) * 100 + 10, "New file");
                outtextxy(930, (i-1) * 100 + 40, "Save");
                outtextxy(930, (i-1) * 100 + 70, "Open");
                break;
            case 6:
                outtextxy(930, (i-1) * 100 + 10, "Language");
                readimagefile("engleza.jpg", 910, 530, 1005, 570);
                readimagefile("left_arrow.jpg", 910, 575, 930, 590);
                readimagefile("right_arrow.jpg", 990, 575, 1010, 590);
                break;
            case 7:
                outtextxy(935, (i-1) * 100 + 15, "Colour");
                break;
            case 8:
                outtextxy(945, (i-1) * 100 + 65, "Quit");
                break;
            }
        }
        outtextxy(910, 120, "Physical");
        outtextxy(910, 140, "size");
        outtextxy(915, 718, "Wipe screen");
    }
    else if (nr_limba == 3) ///germana
    {
        for (i = 0; i <= 8; i++)
        {
            switch (i)
            {
            case 0:
                strcpy(sir, "Generator");
                break;
            case 1:
                strcpy(sir, "Schalter");
                break;
            case 2:
                strcpy(sir, "Transistoren");
                break;
            case 3:
                strcpy(sir, "Widerstehen");
                break;
            case 4:
                strcpy(sir, "Glühbirne");
                break;
            case 5:
                strcpy(sir, "Batterie");
                break;
            case 6:
                strcpy(sir, "Voltmeter");
                break;
            case 7:
                strcpy(sir, "Amperemeter");
                break;
            case 8:
                strcpy(sir, "Lautsprecher");
                break;
            }
            outtextxy(10 + i * 100, 40, sir);
        }
        for (i = 1; i <= 8; i++)
        {
            switch (i)
            {
            case 1:
                outtextxy(925, i * 100 + 75, "Abwischen");
                break;
            case 2:
                readimagefile("left_arrow.jpg", 930, 230, 950, 245);
                readimagefile("right_arrow.jpg", 970, 230, 990, 245);
                outtextxy(935, i * 100 + 10, "Drehen");
                break;
            case 3:
                outtextxy(925, 260, "Vergrößern");
                readimagefile("plus.jpg", 960, 275, 990, 295);
                readimagefile("minus.jpg", 930, 275, 950, 295);
                break;
            case 4:
                outtextxy(920, (i-1) * 100 + 25, "Gebrauchsan");
                outtextxy(920, (i-1) * 100 + 50, "weisung");
                break;
            case 5:
                outtextxy(915, (i-1) * 100 + 10, "Neue Datei");
                outtextxy(915, (i-1) * 100 + 40, "Speichern");
                outtextxy(915, (i-1) * 100 + 73, "Öffnen");
                break;
            case 6:
                outtextxy(930, (i-1) * 100 + 10, "Sprache");
                readimagefile("germana.jpg", 910, 530, 1005, 570);
                readimagefile("left_arrow.jpg", 910, 575, 930, 590);
                readimagefile("right_arrow.jpg", 990, 575, 1010, 590);
                break;
            case 7:
                outtextxy(935, (i-1) * 100 + 15, "Farbe");
                break;
            case 8:
                outtextxy(925, (i-1) * 100 + 65, "Verlassen");
                break;
            }
        }
        outtextxy(905, 120, "Physische");
        outtextxy(910, 140, "Größe");
        outtextxy(920, 710, "Bildschirm");
        outtextxy(920, 730, "abwischen");
    }
    else if (nr_limba == 4) ///franceza
    {
        for (i = 0; i <= 8; i++)
        {
            switch (i)
            {
            case 0:
                strcpy(sir, "Générateur");
                break;
            case 1:
                strcpy(sir, "Intrerupateur");
                break;
            case 2:
                strcpy(sir, "Transistors");
                break;
            case 3:
                strcpy(sir, "Résister");
                break;
            case 4:
                strcpy(sir, "Ampoule");
                break;
            case 5:
                strcpy(sir, "Batterie");
                break;
            case 6:
                strcpy(sir, "Voltmètre");
                break;
            case 7:
                strcpy(sir, "Ampèremètre");
                break;
            case 8:
                strcpy(sir, "Haut-parleur");
                break;
            }
            outtextxy(10 + i * 100, 40, sir);
        }
        for (i = 1; i <= 8; i++)
        {
            switch (i)
            {
            case 1:
                outtextxy(935, i * 100 + 75, "Effacer");
                break;
            case 2:
                readimagefile("left_arrow.jpg", 930, 230, 950, 245);
                readimagefile("right_arrow.jpg", 970, 230, 990, 245);
                outtextxy(935, i * 100 + 10, "Tourner");
                break;
            case 3:
                outtextxy(935, 260, "Agrandir");
                readimagefile("plus.jpg", 960, 275, 990, 295);
                readimagefile("minus.jpg", 930, 275, 950, 295);
                break;
            case 4:
                outtextxy(920, (i-1) * 100 + 25, "Mode");
                outtextxy(920, (i-1) * 100 + 50, "d'emploi");
                break;
            case 5:
                outtextxy(905, (i-1) * 100 + 10, "Nouveau fichier");
                outtextxy(905, (i-1) * 100 + 40, "Enregistrer");
                outtextxy(905, (i-1) * 100 + 70, "Ouvert");
                break;
            case 6:
                outtextxy(930, (i-1) * 100 + 10, "Langue");
                readimagefile("franceza.jpg", 910, 530, 1005, 570);
                readimagefile("left_arrow.jpg", 910, 575, 930, 590);
                readimagefile("right_arrow.jpg", 990, 575, 1010, 590);
                break;
            case 7:
                outtextxy(935, (i-1) * 100 + 15, "Couleur");
                break;
            case 8:
                outtextxy(935, (i-1) * 100 + 65, "Quitter");
                break;
            }
        }
        outtextxy(910, 120, "Grandeur");
        outtextxy(910, 140, "physique");
        outtextxy(910, 720, "Essuyer l'écran");
    }
    for (i = 0; i < 900; i = i + 100)
        rectangle(i, 0, i + 100, 100);
    for (i = 100; i <= 800; i = i + 100)
        rectangle(900, i, 915 + i, 900 + i + 100);
    line(900, 433, 1015, 433);
    line(900, 463, 1015, 463);
    line(900, 253, 1015, 253);
    line(900, 170, 1015, 170);
    line(900, 750, 1015, 750);
    setfillstyle(SOLID_FILL, WHITE);
    bar(910, 635, 935, 660);
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(945, 635, 970, 660);
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(980, 635, 1005, 660);
    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(910, 670, 935, 695);
    setfillstyle(SOLID_FILL, YELLOW);
    bar(945, 670, 970, 695);
    setfillstyle(SOLID_FILL, MAGENTA);
    bar(980, 670, 1005, 695);
    rectangle(975, 118, 1013, 135);
    outtextxy(980, 145, "Set");
    rectangle(977, 145, 1013, 160);
}
void culoare()
{
    int i;
    ///culorile
    setfillstyle(SOLID_FILL, WHITE);
    bar(910, 635, 935, 660);
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(945, 635, 970, 660);
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(980, 635, 1005, 660);
    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(910, 670, 935, 695);
    setfillstyle(SOLID_FILL, YELLOW);
    bar(945, 670, 970, 695);
    setfillstyle(SOLID_FILL, MAGENTA);
    bar(980, 670, 1005, 695);
    setcolor(nr_culoare);
    for (i = 0; i < 900; i = i + 100)
        rectangle(i, 0, i + 100, 100);
    for (i = 100; i <= 800; i = i + 100)
        rectangle(900, i, 915 + i, 900 + i + 100);
    line(900, 433, 1015, 433);
    line(900, 463, 1015, 463);
    line(900, 253, 1015, 253);
    line(900, 170, 1015, 170);
    line(900, 750, 1015, 750);
    rectangle(900, 118, 1013, 135);
    outtextxy(977, 130, "Set");
    rectangle(900, 160, 1013, 180);
    visualPiesa(957.5, 50, but);
    limba();
}
void buton()
{
    int gata;
    double x, y;
    while (true)
    {
        x = mousex();
        y = mousey();
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            if (ecranSchimbat == false)
            {
                if (y >= 0 && y <= 100)
                    but = x / 100 + 1;
            }
        }
        ///verific daca sunt pe ecranul principal sau ecranul de instructiuni
        if (ecranSchimbat == false)
            Piese(but);
    }

}
void MarimeFizica()
{
    int nr = 0, ok = 0, p = 977, aux;
    char m[8];
    char ch;
    setcolor(nr_culoare);
    setfillstyle(SOLID_FILL, BLACK);
    bar(976, 119, 1012, 134);
    do
    {
        if (ismouseclick(WM_LBUTTONDOWN))
            clearmouseclick(WM_LBUTTONDOWN);
        ch = getch();
        if (ok == 0 && ch != '0' && ch >= '0' && ch <= '9')
        {
            ok++;
            nr = nr * 10 + (ch - '0');
            aux = ch - '0';
            sprintf(m, "%d", aux);
            outtextxy(p, 119, m);
            m[0] = '\0';
            p = p + 8;
        }
        else if (ch >= '0' && ch <= '9' && ok < 4 && ok > 0)
        {
            ok++;
            nr = nr * 10 + (ch - '0');
            aux = ch - '0';
            sprintf(m, "%d", aux);
            outtextxy(p, 119, m);
            m[0] = '\0';
            p = p + 8;
        }
    }
    while (ch != '\r');
    v[Obiect_selectat].valoare = nr;
}
void Piese(int aux)
{
    int gata = 1,ch,crt,j;
    double x, y;
    if (selectat == 0)
    {
        setfillstyle(SOLID_FILL, BLACK);
        bar(976, 119, 1012, 134);
    }
    while (gata)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            x = mousex();
            y = mousey();
            if (ecranSchimbat == false)
            {
                ///butoanele de sus
                if (y >= 0 && y <= 100)
                {
                    val=0;
                    dim=0.7;
                    aux = x / 100 + 1;
                    if (aux != but && aux <= 9) ///daca sa zicem apas pe un alt buton, schimb figura si retin alta piesa
                    {
                        gata = 0, but = aux;
                        setfillstyle(SOLID_FILL, BLACK);
                        bar(903, 3, 1015, 100);
                        visualPiesa(957.5, 50, but);
                    }
                    if (selectat == 1)
                    {
                        ///deselectez o piesa, dar apas pe butoane
                        deselectare_Piesa(Obiect_selectat);
                    }
                }
                ///stergere
                if ((x > 900 && x < 1015) && (y > 170 && y < 200)&&but!=0)
                {
                    if(selectat==1)
                    {
                        stergere_PiesaTotal(Obiect_selectat);
                    }
                }
                else
                    ///rotatie minus
                    if ((x >= 930 && x <= 950) && (y >= 230 && y <= 245)&&but!=0)
                    {
                        if(!selectat)
                        {
                            val-=90;
                            setfillstyle(SOLID_FILL, BLACK);
                            bar(903, 3, 1015, 100);
                            visualPiesa(957.5, 50, but);
                        }
                        else  ///rotesc dupa ce am pus piesa
                        {
                            if(nu_are_linii(Obiect_selectat))
                            {
                                int care=Obiect_selectat;
                                int tipul=v[care].tip;
                                double x1=v[care].x,y1=v[care].y,copdim=v[care].dimm,copval=v[care].ang;
                                double val1=val,dim1=dim;
                                dim=copdim;
                                retinvaloarea=v[care].valoare;
                                stergere_Piesa(care);
                                copval-=90;
                                val=copval;
                                creazaPiesa(tipul,x1,y1,copdim);
                                if(!ook)
                                {
                                    val=copval+90;
                                    creazaPiesa(tipul,x1,y1,copdim);
                                }
                                v[contor-1].valoare=retinvaloarea;
                                selectare_Piesa(contor-1);
                                dim=dim1;
                                val=val1;
                            }
                        }
                    }
                ///rotatie plus
                    else if ((x >= 970 && x <= 990) && (y >= 230 && y <= 245)&&but!=0)
                    {
                        if(!selectat)
                        {
                            val+=90;
                            setfillstyle(SOLID_FILL, BLACK);
                            bar(903, 3, 1015, 100);
                            visualPiesa(957.5, 50, but);
                        }
                        else  ///rotesc dupa ce am pus piesa
                        {
                            if(nu_are_linii(Obiect_selectat))
                            {
                                int care=Obiect_selectat;
                                int tipul=v[care].tip;
                                double x1=v[care].x,y1=v[care].y,copdim=v[care].dimm,copval=v[care].ang;
                                double val1=val,dim1=dim;
                                dim=copdim;
                                retinvaloarea=v[care].valoare;
                                stergere_Piesa(care);
                                copval+=90;
                                val=copval;
                                creazaPiesa(tipul,x1,y1,copdim);
                                if(!ook)
                                {
                                    val=copval-90;
                                    creazaPiesa(tipul,x1,y1,copdim);
                                }
                                v[contor-1].valoare=retinvaloarea;
                                selectare_Piesa(contor-1);
                                dim=dim1;
                                val=val1;
                            }
                        }
                    }
                ///marire plus
                    else if (x >= 960 && x <= 990 && y >= 275 && y <= 295&&but!=0)
                    {

                        if(!selectat)
                        {
                            if(dim<1.3)
                            {
                                dim+=0.1;
                                setfillstyle(SOLID_FILL, BLACK);
                                bar(903, 3, 1015, 100);
                                visualPiesa(957.5, 50, but);
                            }
                        }
                        else///maresc dupa ce am pus piesa
                        {
                            if(nu_are_linii(Obiect_selectat))
                            {
                                int care=Obiect_selectat;
                                int tipul=v[care].tip;
                                double x1=v[care].x,y1=v[care].y,copdim=v[care].dimm,copval=v[care].ang;
                                double val1=val,dim1=dim;
                                val=copval;
                                retinvaloarea=v[care].valoare;
                                if(copdim<1.3)
                                {
                                    stergere_Piesa(care);
                                    copdim+=0.1;
                                    dim=copdim;
                                    creazaPiesa(tipul,x1,y1,copdim);
                                    if(!ook)
                                    {
                                        dim=copdim-0.1;
                                        creazaPiesa(tipul,x1,y1,copdim-0.1);
                                    }
                                    v[contor-1].valoare=retinvaloarea;
                                    selectare_Piesa(contor-1);
                                }
                                dim=dim1;
                                val=val1;
                            }
                        }
                    }
                ///marire minus
                    else if (x >= 930 && x <= 950 && y >= 275 && y <= 295&&but!=0)
                    {

                        if(!selectat)
                        {
                            if(dim>0.8)
                            {
                                dim-=0.1;
                                setfillstyle(SOLID_FILL, BLACK);
                                bar(903, 3, 1015, 100);
                                visualPiesa(957.5, 50, but);
                            }
                        }
                        else///maresc dupa ce am pus piesa
                        {
                            if(nu_are_linii(Obiect_selectat))
                            {
                                int care=Obiect_selectat;
                                int tipul=v[care].tip;
                                double x1=v[care].x,y1=v[care].y,copdim=v[care].dimm,copval=v[care].ang;
                                double val1=val,dim1=dim;
                                val=copval;
                                retinvaloarea=v[care].valoare;
                                if(copdim>0.8)
                                {
                                    stergere_Piesa(care);
                                    copdim-=0.1;
                                    dim=copdim;
                                    creazaPiesa(tipul,x1,y1,copdim);
                                    if(!ook)
                                    {
                                        dim=copdim+0.1;
                                        creazaPiesa(tipul,x1,y1,copdim+0.1);
                                    }
                                    v[contor-1].valoare=retinvaloarea;
                                    selectare_Piesa(contor-1);
                                }
                                dim=dim1;
                                val=val1;
                            }
                        }
                    }
                    else
                        ///butoanele de schimbat limba
                        if ((x >= 910 && x <= 930) && (y >= 575 && y <= 590))
                        {
                            nr_limba--;
                            if (nr_limba <= 0)
                                nr_limba = 4;
                            limba();
                        }
                        else if ((x >= 990 && x <= 1010) && (y >= 575 && y <= 590))
                        {
                            nr_limba++;
                            if (nr_limba > 4)
                                nr_limba = 1;
                            limba();
                        }
                ///(15 - alb, 9 - blue, 10 - green, 11 - cyan, 14 - yellow, 5 - magenta) ///pentru ecranul principal
                        else if (x >= 910 && x <= 935 && y >= 635 && y <= 660 && nr_culoare != 15) ///culoarea alb
                        {
                            nr_culoare = 15;
                            culoare();
                        }
                        else if (x >= 945 && x <= 970 && y >= 635 && y <= 660 && nr_culoare != 9) ///culoarea lighblue
                        {
                            nr_culoare = 9;
                            culoare();
                        }
                        else if (x >= 980 && x <= 1005 && y >= 635 && y <= 660 && nr_culoare != 10) ///culoarea lightgreen
                        {
                            nr_culoare = 10;
                            culoare();
                        }
                        else if (x >= 910 && x <= 935 && y >= 670 && y <= 695 && nr_culoare != 11) ///culoarea lightcyan
                        {
                            nr_culoare = 11;
                            culoare();
                        }
                        else if (x >= 945 && x <= 970 && y >= 670 && y <= 695 && nr_culoare != 15) ///culoarea yellow
                        {
                            nr_culoare = 14;
                            culoare();
                        }
                        else if (x >= 980 && x <= 1005 && y >= 670 && y <= 695 && nr_culoare != 5) ///culoarea magenta
                        {
                            nr_culoare = 5;
                            culoare();
                        }
                ///stergere ecran
                        else if (y > 700 && y < 750 && x > 900 && x < 1015)
                            StergereEcran();
                ///inchide ecran
                        else if (y > 750 && y < 800 && x > 900 && x < 1015)
                            exit(0);
                        else if (x > 977 && x < 1013 && y > 145 && y < 160 && selectat == 1)
                            MarimeFizica();
                ///creezi fisier nou
                        else if (y > 400 && y < 430 && x > 900 && x < 1015)
                        {
                            intrebareUtilizator();
                            if (nr_limba == 1)
                                cout << "S-a deschis un fisier nou si nu are nume.\n\n";
                            else if (nr_limba == 2)
                                cout << "A new file has been opened and has no name.\n\n";
                            else if (nr_limba == 3)
                                printf("Eine neue Datei wurde ge%cffnet und hat keinen Namen.\n\n", 153);
                            else if (nr_limba == 4)
                                printf("Un nouveau fichier a %ct%c ouvert et n'a pas de nom.\n\n", 130, 130);
                        }
                ///salvarea datelor fisierului
                        else if (y > 430 && y < 460 && x > 900 && x < 1015)
                        {
                            ///aici am creat fisierul
                            int lg, i;
                            lg = strlen(fisier);
                            if (lg == 1)  ///daca datele nu sunt salvate intr-un fisier, cream un fisier nou si le salvam acolo
                            {
                                if (nr_limba == 1)
                                    cout << "Datele trebuie salvate intr-un fisier. Introduceti numele fisierului cu extensia .txt: ";
                                else if (nr_limba == 2)
                                    cout << "The data must be saved in a file. Enter the file name with the .txt extension: ";
                                else if (nr_limba == 3)
                                    printf("Die Daten m%cssen in einer Datei gespeichert werden. Geben Sie den Dateinamen mit der Erweiterung .txt ein: ", 129);
                                else if (nr_limba == 4)
                                    printf("Les donn%ces doivent %ctre enregistr%ces dans un fichier. Entrez le nom du fichier avec l'extension .txt: ", 130, 136, 130);
                                while (true)
                                {
                                    cin >> fisier;
                                    ///aici verific daca fisierul nu exista deja ca sa nu suprapun datele
                                    strcat(folder, ".\\Fisiere\\");
                                    strcat(folder, fisier);
                                    if(fopen(folder,"r")!= NULL)
                                    {
                                        if (nr_limba == 1)
                                            cout << "Fisierul deja exista. Introduceti alt nume: ";
                                        else if (nr_limba == 2)
                                            cout << "The file already existed. Enter another name: ";
                                        else if (nr_limba == 3)
                                            printf("Die Datei war bereits vorhanden. Einen anderen Namen eingeben: ");
                                        else if (nr_limba == 4)
                                            printf("Le fichier existe d%cj%c. Entrez un autre nom: ", 130, 133);
                                        lg = strlen(folder);
                                        for (i = 0; i < lg; i++)
                                            folder[i] = 0;
                                    }
                                    else
                                    {
                                        lg = strlen(fisier);
                                        if (lg > 96)
                                        {
                                            if (nr_limba == 1)
                                                cout << "Numele este prea lung. Incercati inca o data. \n";
                                            else if (nr_limba == 2)
                                                cout << "The name is too long. Please try again. \n";
                                            else if (nr_limba == 3)
                                                printf("Der Name ist zu lang. Bitte versuche es erneut. \n");
                                            else if (nr_limba == 4)
                                                printf("Le nom est trop long. Veuillez r%cessayer. \n", 130);
                                        }
                                        else if (fisier[lg - 1] != 't' || fisier[lg - 2] != 'x' || fisier[lg - 3] != 't' || fisier[lg - 4] != '.')
                                        {
                                            if (nr_limba == 1)
                                                cout << "Extensia nu este corecta. Incercati inca o data.\n";
                                            else if (nr_limba == 2)
                                                cout << "The extension is incorrect. Please try again.\n";
                                            else if (nr_limba == 3)
                                                printf("Die Erweiterung ist falsch. Bitte versuchen Sie es erneut.\n");
                                            else if (nr_limba == 4)
                                                printf("L'extension est incorrecte. Veuillez r%cessayer.\n", 130);
                                        }
                                        else if (lg <= 4)
                                        {
                                            if (nr_limba == 1)
                                                cout << "Numele fisierului este prea scurt. Incercati inca o data. \n";
                                            else if (nr_limba == 2)
                                                cout << "File name is too short. Please try again. \n";
                                            else if (nr_limba == 3)
                                                printf("Dateiname ist zu kurz. Bitte versuchen Sie es erneut.\n");
                                            else if (nr_limba == 4)
                                                printf("Le nom du fichier est trop court. Veuillez r%cessayer.\n", 130);
                                        }
                                        else
                                        {
                                            if (nr_limba == 1)
                                                printf("Fisierul cu numele %s s-a salvat.\n\n", fisier);
                                            else if (nr_limba == 2)
                                                printf("The file named %s has been saved. \n\n", fisier);
                                            else if (nr_limba == 3)
                                                printf("Die Datei mit dem Namen %s wurde gespeichert. \n\n", fisier);
                                            else if (nr_limba == 4)
                                                printf("Le fichier nomm%c %s a %ct%c enregistr%c.\n\n", 130, fisier, 130, 130);
                                            numarFisiere++;
                                            break;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if (nr_limba == 1)
                                    printf("In fisierul %s au fost salvate modificarile.\n", fisier);
                                else if (nr_limba == 2)
                                    printf("Changes saved to file %s.\n", fisier);
                                else if (nr_limba == 3)
                                    printf("%cnderungen in Datei %s gespeichert.\n", 142, fisier);
                                else if (nr_limba == 4)
                                    printf("Modifications enregistr%ces dans le fichier %s.\n", 130, fisier);

                            }
                            salvareDate();
                        }
                ///deschizi un nou fisier
                        else if (y > 460 && y < 500 && x > 900 & x < 1015)
                        {
                            intrebareUtilizator(); ///intrebam utilizatorul daca vrea sa salveze datele curente
                            StergereEcran(); ///stergem spatiul de lucru pentru a pune noul spatiu de lucru
                            if (numarFisiere == 0)
                            {
                                if (nr_limba == 1)
                                    printf("Nu ati salvat un fisier nou. Salvati un fisier nou.\n\n");
                                else if (nr_limba == 2)
                                    printf("You did not save a new file. Save a new file.\n\n");
                                else if (nr_limba == 3)
                                    printf("Sie haben keine neue Datei gespeichert. Speichern Sie eine neue Datei.\n\n");
                                else if (nr_limba == 4)
                                    printf("Vous n'avez pas enregistre de nouveau fichier. Enregistrez un nouveau fichier.\n\n", 130);
                            }
                            else
                            {
                                if (nr_limba == 1)
                                    printf("Scrieti numele fisierului cu extensia .txt pe care vreti sa il deschideti: ");
                                else if (nr_limba == 2)
                                    printf("Type the name of the file with the .txt extension you want to open: ");
                                else if (nr_limba == 3)
                                    printf("Geben Sie den Namen der Datei mit der Erweiterung .txt ein, die Sie %cffnen m%cchten: ", 153, 153);
                                else if (nr_limba == 4)
                                    printf("Tapez le nom du fichier avec l'extension .txt que vous souhaitez ouvrir: ");
                                while (true)
                                {
                                    cin >> fisier;
                                    int lg = strlen(fisier);
                                    if (lg > 96)
                                    {
                                        if (nr_limba == 1)
                                            cout << "Numele este prea lung. Incercati inca o data. \n";
                                        else if (nr_limba == 2)
                                            cout << "The name is too long. Please try again. \n";
                                        else if (nr_limba == 3)
                                            printf("Der Name ist zu lang. Bitte versuche es erneut. \n");
                                        else if (nr_limba == 4)
                                            printf("Le nom est trop long. Veuillez r%cessayer. \n", 130);
                                    }
                                    else if (fisier[lg - 1] != 't' || fisier[lg - 2] != 'x' || fisier[lg - 3] != 't' || fisier[lg - 4] != '.')
                                    {
                                        if (nr_limba == 1)
                                            cout << "Extensia nu este corecta. Incercati inca o data.\n";
                                        else if (nr_limba == 2)
                                            cout << "The extension is incorrect. Please try again.\n";
                                        else if (nr_limba == 3)
                                            printf("Die Erweiterung ist falsch. Bitte versuchen Sie es erneut.\n");
                                        else if (nr_limba == 4)
                                            printf("L'extension est incorrecte. Veuillez r%cessayer.\n", 130);
                                    }
                                    else if (lg <= 4)
                                    {
                                        if (nr_limba == 1)
                                            cout << "Numele fisierului este prea scurt. Incercati inca o data. \n";
                                        else if (nr_limba == 2)
                                            cout << "File name is too short. Please try again. \n";
                                        else if (nr_limba == 3)
                                            printf("Dateiname ist zu kurz. Bitte versuchen Sie es erneut.\n");
                                        else if (nr_limba == 4)
                                            printf("Le nom du fichier est trop court. Veuillez r%cessayer.\n", 130);
                                    }
                                    else
                                    {
                                        strcat(folder, ".\\Fisiere\\"); ///aici am creat fisierul
                                        strcat(folder, fisier);
                                        FILE *fptr;
                                        if((fptr = fopen(folder,"r"))== NULL)
                                        {
                                            if (nr_limba == 1)
                                                printf("Fisierul nu exista. Scrieti numele fisierului pe care vreti sa il deschideti: ");
                                            else if (nr_limba == 2)
                                                printf("The file does not exist. Enter the name of the file you want to open: ");
                                            else if (nr_limba == 3)
                                                printf("Die Datei existiert nicht. Geben Sie den Namen der Datei ein, die Sie %cffnen m%cchten: ", 153, 153);
                                            else if (nr_limba == 4)
                                                printf("Le fichier n'existe pas. Tapez le nom du fichier que vous souhaitez ouvrir: ");
                                            int lg = strlen(folder);
                                            for (int i = 0; i < lg; i++)
                                            {
                                                folder[i] = 0;
                                                fisier[i] = 0;
                                            }
                                        }
                                        else
                                        {
                                            if (nr_limba == 1)
                                                printf("Fisierul exista. Piesele au fost puse pe masa de lucru.\n\n");
                                            else if (nr_limba == 2)
                                                printf("The file exists. The parts have been put on the desktop. \n\n");
                                            else if (nr_limba == 3)
                                                printf("Die Datei ist vorhanden. Die Teile wurden auf dem Desktop abgelegt. \n\n");
                                            else if (nr_limba == 4)
                                                printf("Le fichier existe. Les pièces ont été placées sur le bureau.\n\n");
                                            ifstream myfile;
                                            myfile.open(folder);
                                            myfile >> contor;
                                            for (int i = 0; i < contor; i++)
                                            {
                                                myfile >> v[i].valoare >> v[i].x >> v[i].y;
                                                myfile >> v[i].x_st >> v[i].y_st >> v[i].x_dr >> v[i].y_dr;
                                                myfile >> v[i].nr_noduri;
                                                for (int j = 0; j < v[i].nr_noduri; j++)
                                                    myfile >> v[i].nod[j].x_nod >> v[i].nod[j].y_nod>>v[i].nod[j].okk;
                                                myfile >> v[i].dimm >> v[i].ang >> v[i].tip;
                                            }
                                            myfile >> catte;
                                            for(int i=0; i<catte; i++)
                                                myfile>>pere[i].vst>> pere[i].vdr>>pere[i].nvst>>pere[i].nvdr;
                                            redesenarePiese();
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                ///manualul de instructiuni
                        else if (x > 900 && x < 1015 && y > 300 && y < 400 && ecranSchimbat == false)
                        {
                            ecranSchimbat = true;
                            deselectare_Piesa(Obiect_selectat);
                            Obiect_selectat = -1;
                            selectat = 0;
                            but = -1;
                            cleardevice();
                            readimagefile("left_arrow.jpg", 20, 20, 120, 120);
                            setcolor(nr_culoare);
                            if (nr_limba == 1)
                            {
                                outtextxy(60, 195, "Limba");
                                readimagefile("romana.jpg", 30, 220, 125, 280);
                                outtextxy(50, 360, "Culoare");
                            }
                            else if (nr_limba == 2)
                            {
                                outtextxy(60, 195, "Language");
                                readimagefile("engleza.jpg", 30, 220, 125, 280);
                                outtextxy(50, 360, "Colour");
                            }
                            else if (nr_limba == 3)
                            {
                                outtextxy(60, 195, "Sprache");
                                readimagefile("germana.jpg", 30, 220, 125, 280);
                                outtextxy(50, 360, "Farbe");
                            }
                            else if (nr_limba == 4)
                            {
                                outtextxy(60, 195, "Langue");
                                readimagefile("franceza.jpg", 30, 220, 125, 280);
                                outtextxy(50, 360, "Couleur");
                            }
                            readimagefile("left_arrow.jpg", 30, 295, 50, 320);
                            readimagefile("right_arrow.jpg", 110, 295, 130, 320);

                            setfillstyle(SOLID_FILL, WHITE);
                            bar(30, 390, 55, 415);
                            setfillstyle(SOLID_FILL, LIGHTBLUE);
                            bar(65, 390, 90, 415);
                            setfillstyle(SOLID_FILL, LIGHTGREEN);
                            bar(105, 390, 130, 415);
                            setfillstyle(SOLID_FILL, LIGHTCYAN);
                            bar(30, 425, 55, 450);
                            setfillstyle(SOLID_FILL, YELLOW);
                            bar(65, 425, 90, 450);
                            setfillstyle(SOLID_FILL, MAGENTA);
                            bar(105, 425, 130, 450);

                            outtextxy(40, 475, "Credits:");
                            outtextxy(20, 500, "Brezuleanu Alexandru");
                            outtextxy(20, 525, "Mihai");
                            outtextxy(20, 560, "Opariuc Rares Ioan");
                            line(173, 0, 173, 800);
                            instructiune(nr_culoare, nr_limba);
                        }
                        else if(x>10&&x<890&&y>110&&y<790)
                        {
                            x2=x1;
                            y2=y1;
                            x1=x;
                            y1=y;///retin coordonatele ultimelor doua click-uri
                            int Obiect;
                            Obiect = selectare(x, y);
                            if (Obiect >= 0)
                            {

                                if (selectat == 0)
                                    selectare_Piesa(Obiect);
                                else if (selectat == 1)
                                {

                                    if (Obiect == Obiect_selectat) ///daca apas inca odata pe obiectul selectat
                                    {
                                        continue;
                                    }
                                    else ///daca selectez o alta piesa o deselctez pe cea veche
                                    {
                                        golire();
                                        for(int i=0; i<contor; i++)
                                        {
                                            obstacol(v[i].x_st-10*v[i].dimm,v[i].y_st-10*v[i].dimm,v[i].x_dr+10*v[i].dimm,v[i].y_dr+10*v[i].dimm);
                                            for(int k=0; k<v[i].nr_noduri; k++)
                                                if(!v[i].nod[k].okk)
                                                    nu_obstacol(v[i].nod[k].x_nod-20*v[i].dimm,v[i].nod[k].y_nod-20*v[i].dimm,v[i].nod[k].x_nod+20*v[i].dimm,v[i].nod[k].y_nod+20*v[i].dimm);
                                        }
                                        unu=Obiect_selectat,doi=Obiect;///obiectele selectate
                                        deselectare_Piesa(Obiect_selectat);
                                        nodlegat=0;
                                        nodstart=selectarelegaturi(unu,x2,y2);///punctul de plecare
                                        nodend=selectarelegaturi(doi,x1,y1);///punctul de sosire
                                        if(nodlegat==2&&nodstart==-1&&nodend==-1)
                                        {
                                            sterglinie();
                                        }
                                        if(nodend>=0&&nodstart>=0)/// am doua puncte de legatura selectate
                                        {
                                            desenlinie(v[unu].nod[nodstart].x_nod,v[unu].nod[nodstart].y_nod,v[doi].nod[nodend].x_nod,v[doi].nod[nodend].y_nod);
                                        }
                                        else /// punctele de legatura sunt marcate ca nevizitate unde e cazul
                                        {
                                            if(nodstart!=-1)
                                                v[unu].nod[nodstart].okk=0;
                                            if(nodend!=-1)
                                                v[doi].nod[nodend].okk=0;
                                        }
                                        nodstart=nodend=-1;

                                    }
                                }
                            }
                            else
                            {
                                creazaPiesa(but,x,y,dim);
                                setfillstyle(SOLID_FILL, BLACK);
                                bar(976, 119, 1012, 134);
                            }
                        }
            }
            else if (ecranSchimbat == true)
            {
                ///cand apesi pe safeata din spate si te intorci la ecranul principal
                if (x > 20 && x < 120 && y > 20 && y < 120)
                {
                    cleardevice();
                    culoare();
                    redesenarePiese2();
                    ecranSchimbat = false;
                }///schimbi limba sau culoarea
                else if (x > 30 && x < 50 && y > 295 && y < 320)
                {
                    nr_limba--;
                    if (nr_limba == 0)
                        nr_limba = 4;
                    instructiune(nr_culoare, nr_limba);
                }
                else if (x > 110 && x < 130 && y > 295 && y < 320)
                {
                    nr_limba++;
                    if (nr_limba > 4)
                        nr_limba = 1;
                    instructiune(nr_culoare, nr_limba);
                }
                //(15 - alb, 9 - blue, 10 - green, 11 - cyan, 14 - yellow, 5 - magenta)
                else if (x > 30 && x < 55 && y > 390 && y < 415)
                {
                    nr_culoare = 15;
                    setcolor(nr_culoare);
                    line(173, 0, 173, 800);
                    instructiune(nr_culoare, nr_limba);
                }
                else if (x > 65 && x < 90 && y > 390 && y < 415)
                {
                    nr_culoare = 9;
                    setcolor(nr_culoare);
                    line(173, 0, 173, 800);
                    instructiune(nr_culoare, nr_limba);
                }
                else if (x > 105 && x < 130 && y > 390 && y < 415)
                {
                    nr_culoare = 10;
                    setcolor(nr_culoare);
                    line(173, 0, 173, 800);
                    instructiune(nr_culoare, nr_limba);
                }
                else if (x > 30 && x < 55 && y > 425 && y < 450)
                {
                    nr_culoare = 11;
                    setcolor(nr_culoare);
                    line(173, 0, 173, 800);
                    instructiune(nr_culoare, nr_limba);
                }
                else if (x > 65 && x < 90 && y > 425 && y < 450)
                {
                    nr_culoare = 14;
                    setcolor(nr_culoare);
                    line(173, 0, 173, 800);
                    instructiune(nr_culoare, nr_limba);
                }
                else if (x > 105 && x < 130 && y > 425 && y < 450)
                {
                    nr_culoare = 5;
                    setcolor(nr_culoare);
                    line(173, 0, 173, 800);
                    instructiune(nr_culoare, nr_limba);
                }
            }
        }
        ///cand apas tastele si sunt pe ecranul de instructiuni, sa nu le retin
        if (kbhit() && ecranSchimbat == true)
            ch=getch();
        else if(kbhit() && selectat == 1)
        {
            crt=Obiect_selectat;
            ch=getch();
            if(ch==80||ch==77||ch==75||ch==72)///tasta directie
            {
                fara_obstacol(v[crt].x_st-10*v[crt].dimm,v[crt].y_st-10*v[crt].dimm,v[crt].x_dr+10*v[crt].dimm,v[crt].y_dr+10*v[crt].dimm);
                for(int i=0; i<linn.size(); i++)
                    if(linn[i].st==crt)
                    {
                        restante.push_back(i);
                        v[crt].nod[linn[i].ndst].okk=0;
                        v[linn[i].dr].nod[linn[i].nddr].okk=0;
                    }
                    else if(linn[i].dr==crt)
                    {
                        restante.push_back(i);
                        v[crt].nod[linn[i].nddr].okk=0;
                        v[linn[i].st].nod[linn[i].ndst].okk=0;
                    }
                if(restante.size()>0)
                    stergerelegaturi();
                setcolor(nr_culoare);
                if(ch==72)///sus
                {
                    if(verificareMargineMutare(crt,0,-speed,0,-speed))
                    {
                        setfillstyle(SOLID_FILL, BLACK);
                        bar(v[crt].x_st,v[crt].y_st, v[crt].x_dr+1, v[crt].y_dr+1);
                        v[crt].y-=speed;
                        v[crt].y_st-=speed;
                        v[crt].y_dr-=speed;
                        for(int i=0; i<v[crt].nr_noduri; i++)
                            v[crt].nod[i].y_nod-=speed;
                        if(refacerelegaturi())
                        {
                            cautPiesa_si_Desen(crt);
                            setcolor(WHITE);
                            for(int i=0; i<v[crt].nr_noduri; i++)
                            {
                                circle(v[crt].nod[i].x_nod,v[crt].nod[i].y_nod,5*v[crt].dimm);
                            }
                            setcolor(nr_culoare);
                            selectare_Piesa(crt);
                        }
                        else
                        {
                            v[crt].y+=speed;
                            v[crt].y_st+=speed;
                            v[crt].y_dr+=speed;
                            for(int i=0; i<v[crt].nr_noduri; i++)
                                v[crt].nod[i].y_nod+=speed;
                            if(refacerelegaturi())
                            {
                                cautPiesa_si_Desen(crt);
                                setcolor(WHITE);
                                for(int i=0; i<v[crt].nr_noduri; i++)
                                {
                                    circle(v[crt].nod[i].x_nod,v[crt].nod[i].y_nod,5*v[crt].dimm);
                                }
                                setcolor(nr_culoare);
                                selectare_Piesa(crt);
                            }
                        }
                    }
                    else
                        refacerelegaturi2();
                }
                if(ch==80)///jos
                {
                    if(verificareMargineMutare(crt,0,speed,0,speed))
                    {
                        setfillstyle(SOLID_FILL, BLACK);
                        bar(v[crt].x_st,v[crt].y_st, v[crt].x_dr+1, v[crt].y_dr+1);
                        v[crt].y+=speed;
                        v[crt].y_st+=speed;
                        v[crt].y_dr+=speed;
                        for(int i=0; i<v[crt].nr_noduri; i++)
                            v[crt].nod[i].y_nod+=speed;
                        if(refacerelegaturi())
                        {
                            cautPiesa_si_Desen(crt);
                            setcolor(WHITE);
                            for(int i=0; i<v[crt].nr_noduri; i++)
                            {
                                circle(v[crt].nod[i].x_nod,v[crt].nod[i].y_nod,5*v[crt].dimm);
                            }
                            setcolor(nr_culoare);
                            selectare_Piesa(crt);
                        }
                        else
                        {
                            v[crt].y-=speed;
                            v[crt].y_st-=speed;
                            v[crt].y_dr-=speed;
                            for(int i=0; i<v[crt].nr_noduri; i++)
                                v[crt].nod[i].y_nod-=speed;
                            if(refacerelegaturi())
                            {
                                cautPiesa_si_Desen(crt);
                                setcolor(WHITE);
                                for(int i=0; i<v[crt].nr_noduri; i++)
                                {
                                    circle(v[crt].nod[i].x_nod,v[crt].nod[i].y_nod,5*v[crt].dimm);
                                }
                                setcolor(nr_culoare);
                                selectare_Piesa(crt);
                            }
                        }

                    }
                    else
                        refacerelegaturi2();
                }
                if(ch==75)///stanga
                {
                    if(verificareMargineMutare(crt,-speed,0,-speed,0))
                    {
                        setfillstyle(SOLID_FILL, BLACK);
                        bar(v[crt].x_st,v[crt].y_st, v[crt].x_dr+1, v[crt].y_dr+1);
                        v[crt].x-=speed;
                        v[crt].x_st-=speed;
                        v[crt].x_dr-=speed;
                        for(int i=0; i<v[crt].nr_noduri; i++)
                            v[crt].nod[i].x_nod-=speed;
                        if(refacerelegaturi())
                        {
                            cautPiesa_si_Desen(crt);
                            ///desenez si legaturile
                            setcolor(WHITE);
                            for(int i=0; i<v[crt].nr_noduri; i++)
                            {
                                circle(v[crt].nod[i].x_nod,v[crt].nod[i].y_nod,5*v[crt].dimm);
                            }
                            setcolor(nr_culoare);
                            selectare_Piesa(crt);
                        }
                        else
                        {
                            v[crt].x+=speed;
                            v[crt].x_st+=speed;
                            v[crt].x_dr+=speed;
                            for(int i=0; i<v[crt].nr_noduri; i++)
                                v[crt].nod[i].x_nod+=speed;
                            if(refacerelegaturi())
                            {
                                cautPiesa_si_Desen(crt);
                                ///desenez si legaturile
                                setcolor(WHITE);
                                for(int i=0; i<v[crt].nr_noduri; i++)
                                {
                                    circle(v[crt].nod[i].x_nod,v[crt].nod[i].y_nod,5*v[crt].dimm);
                                }
                                setcolor(nr_culoare);
                                selectare_Piesa(crt);
                            }
                        }
                    }
                    else
                        refacerelegaturi2();
                }
                if(ch==77)///dreapta
                {
                    if(verificareMargineMutare(crt,speed,0,speed,0))
                    {
                        setfillstyle(SOLID_FILL, BLACK);
                        bar(v[crt].x_st,v[crt].y_st, v[crt].x_dr+1, v[crt].y_dr+1);
                        v[crt].x+=speed;
                        v[crt].x_st+=speed;
                        v[crt].x_dr+=speed;
                        for(int i=0; i<v[crt].nr_noduri; i++)
                            v[crt].nod[i].x_nod+=speed;
                        if(refacerelegaturi())
                        {
                            cautPiesa_si_Desen(crt);
                            setcolor(WHITE);
                            for(int i=0; i<v[crt].nr_noduri; i++)
                            {
                                circle(v[crt].nod[i].x_nod,v[crt].nod[i].y_nod,5*v[crt].dimm);
                            }
                            setcolor(nr_culoare);
                            selectare_Piesa(crt);
                        }
                        else
                        {
                            v[crt].x-=speed;
                            v[crt].x_st-=speed;
                            v[crt].x_dr-=speed;
                            for(int i=0; i<v[crt].nr_noduri; i++)
                                v[crt].nod[i].x_nod-=speed;
                            if(refacerelegaturi())
                            {
                                cautPiesa_si_Desen(crt);
                                setcolor(WHITE);
                                for(int i=0; i<v[crt].nr_noduri; i++)
                                {
                                    circle(v[crt].nod[i].x_nod,v[crt].nod[i].y_nod,5*v[crt].dimm);
                                }
                                setcolor(nr_culoare);
                                selectare_Piesa(crt);
                            }
                        }
                    }
                    else
                        refacerelegaturi2();
                }

                restante.clear();
            }
        }
    }
}
void creazaPiesa(int but,double x,double y,double dim)
{
    ook=0;///daca am putut crea piesa 1 altfel 0

    switch(but)
    {
    case 1:
        if(verificareMargine(x, y, 35*dim, 26*dim))
        {
            ook=1;
            generator(x, y,dim);
            DateNoi(x, y, 35*dim, 26*dim,1);
            if (selectat == 1)
                deselectare_Piesa(Obiect_selectat);
        }
        break;
    case 2:
        if(verificareMargine(x, y, 35*dim, 30*dim))
        {
            ook=1;
            intrerupator(x, y,dim);
            DateNoi(x, y, 35*dim, 30*dim,2);
            if (selectat == 1)
                deselectare_Piesa(Obiect_selectat);
        }
        break;
    case 3:
        if(verificareMargine(x, y, 30*dim, 40*dim))
        {
            ook=1;
            tranzistor(x, y,dim);
            DateNoi(x, y, 30*dim, 40*dim,3);
            if (selectat == 1)
                deselectare_Piesa(Obiect_selectat);
        }
        break;
    case 4:
        if(verificareMargine(x, y, 45*dim, 35*dim))
        {
            ook=1;
            rezistor(x, y,dim);
            DateNoi(x, y, 45*dim, 35*dim,4);
            if (selectat == 1)
                deselectare_Piesa(Obiect_selectat);
        }
        break;
    case 5:
        if(verificareMargine(x, y, 45*dim, 30*dim))
        {
            ook=1;
            bec(x, y,dim);
            DateNoi(x, y, 45*dim, 30*dim,5);
            if (selectat == 1)
                deselectare_Piesa(Obiect_selectat);
        }
        break;
    case 6:
        if(verificareMargine(x, y, 35*dim, 26*dim))
        {
            ook=1;
            baterie(x, y,dim);
            DateNoi(x, y, 35*dim, 26*dim,6);
            if (selectat == 1)
                deselectare_Piesa(Obiect_selectat);
        }
        break;
    case 7:
        if(verificareMargine(x, y, 45*dim, 30*dim))
        {
            ook=1;
            voltmetru(x, y,dim);
            DateNoi(x, y, 45*dim, 30*dim,7);
            if (selectat == 1)
                deselectare_Piesa(Obiect_selectat);
        }
        break;
    case 8:
        if(verificareMargine(x, y, 45*dim, 30*dim))
        {
            ook=1;
            ampermetru(x, y,dim);
            DateNoi(x, y, 45*dim, 30*dim,8);
            if (selectat == 1)
                deselectare_Piesa(Obiect_selectat);
        }
        break;
    case 9:
        if(verificareMargine(x, y, 40*dim, 30*dim))
        {
            ook=1;
            difuzor(x, y,dim);
            DateNoi(x, y, 40*dim, 30*dim,9);
            if (selectat == 1)
                deselectare_Piesa(Obiect_selectat);
        }
        break;
    }
}
bool verificareMargine(double x, double y, double val_x, double val_y) ///cand pun piesa, sa nu se suprapuna cu alte butoane sau piese
{

    int i;
    double nx_st,nx_dr,ny_st,ny_dr;
    nx_st = min(min(newx(x - val_x,y-val_y,x,y),newx(x + val_x,y+val_y,x,y)),min(newx(x + val_x,y-val_y,x,y),newx(x - val_x,y+val_y,x,y)));
    ny_st = min(min(newy(x - val_x,y-val_y,x,y),newy(x + val_x,y+val_y,x,y)),min(newy(x + val_x,y-val_y,x,y),newy(x - val_x,y+val_y,x,y)));
    nx_dr = max(max(newx(x - val_x,y-val_y,x,y),newx(x + val_x,y+val_y,x,y)),max(newx(x + val_x,y-val_y,x,y),newx(x - val_x,y+val_y,x,y)));
    ny_dr =max(max(newy(x - val_x,y-val_y,x,y),newy(x + val_x,y+val_y,x,y)),max(newy(x + val_x,y-val_y,x,y),newy(x - val_x,y+val_y,x,y)));
    if(!verificareLegatura(nx_st,ny_st,nx_dr,ny_dr))
        return 0;
    for(i=0; i<contor; i++)
    {
        if(!(nx_st >= v[i].x_dr || ny_st>= v[i].y_dr ||nx_dr <= v[i].x_st || ny_dr <= v[i].y_st))
            return 0;
    }
    if(ny_st<120||nx_dr>880||nx_st<20||ny_dr>780)
        return 0;
    return 1;
}
bool verificareMargineMutare(int Obiect,double xst,double yst,double xdr,double ydr) ///cand mut o piesa, sa nu se suprapuna cu alte butoane sau piese
{

    int i;
    double nx_st,nx_dr,ny_st,ny_dr;
    nx_st = v[Obiect].x_st+xst;
    ny_st = v[Obiect].y_st+yst;
    nx_dr = v[Obiect].x_dr+xdr;
    ny_dr = v[Obiect].y_dr+ydr;
    for(i=0; i<Obiect; i++)
        if(!(nx_st >= v[i].x_dr || ny_st>= v[i].y_dr ||nx_dr <= v[i].x_st || ny_dr <= v[i].y_st))
            return 0;
    for(i=Obiect+1; i<contor; i++)
        if(!(nx_st >= v[i].x_dr || ny_st>= v[i].y_dr ||nx_dr <= v[i].x_st || ny_dr <= v[i].y_st))
            return 0;
    if(ny_st<120||nx_dr>880||nx_st<20||ny_dr>780)
        return 0;
    if(!verificareLegaturaMutare(Obiect,xst,yst))
        return 0;
    return 1;
}
int selectare(double x, double y) ///cand dau click, vad daca am selectat o piesa si o returnez sau nu
{
    int i;
    for (i = 0; i < contor; i++)
    {
        if (x >= v[i].x_st && x <= v[i].x_dr && y >= v[i].y_st && y <= v[i].y_dr)
            return i;
    }
    return -1;
}
int selectarelegaturi(int i,double x, double y)
{
    int j;
    bool ok=0;
    for(j=0; j<v[i].nr_noduri; j++)
        if (x >= v[i].nod[j].x_nod-v[i].dimm*10&& x <= v[i].nod[j].x_nod+v[i].dimm*10 && y >= v[i].nod[j].y_nod-v[i].dimm*10 && y <= v[i].nod[j].y_nod+v[i].dimm*10)
        {
            ok=1;
            if(v[i].nod[j].okk==0)
            {
                v[i].nod[j].okk=1;
                return j;
            }
        }
    if(ok)
        nodlegat++;
    return -1;
}
void selectare_Piesa(int Obiect)
{
    char m[8];
    ///afisare marime fizica
    setfillstyle(SOLID_FILL, BLACK);
    bar(976, 119, 1012, 134);
    if (v[Obiect].valoare == -1)
        outtextxy(977, 119, " - - - -");
    else
    {
        sprintf(m, "%d", v[Obiect].valoare);
        outtextxy(977, 119, m);
    }
    selectat = 1;
    Obiect_selectat = Obiect;
    setcolor(RED);
    /// in loc de rectangle am folosit 4 functii line pentru ca patrtul nu poate fi orientat dupa un unghi dat
    line(v[Obiect_selectat].x_st, v[Obiect_selectat].y_st,v[Obiect_selectat].x_dr, v[Obiect_selectat].y_st);
    line(v[Obiect_selectat].x_dr, v[Obiect_selectat].y_st,v[Obiect_selectat].x_dr, v[Obiect_selectat].y_dr);
    line(v[Obiect_selectat].x_dr, v[Obiect_selectat].y_dr,v[Obiect_selectat].x_st, v[Obiect_selectat].y_dr);
    line(v[Obiect_selectat].x_st, v[Obiect_selectat].y_dr,v[Obiect_selectat].x_st, v[Obiect_selectat].y_st);
    setcolor(nr_culoare);
}
void deselectare_Piesa(int Obiect)
{
    setcolor(BLACK);
    /// in loc de rectangle am folosit 4 functii line pentru ca patrtul nu poate fi orientat dupa un unghi dat
    line(v[Obiect_selectat].x_st, v[Obiect_selectat].y_st,v[Obiect_selectat].x_dr, v[Obiect_selectat].y_st);
    line(v[Obiect_selectat].x_dr, v[Obiect_selectat].y_st,v[Obiect_selectat].x_dr, v[Obiect_selectat].y_dr);
    line(v[Obiect_selectat].x_dr, v[Obiect_selectat].y_dr,v[Obiect_selectat].x_st, v[Obiect_selectat].y_dr);
    line(v[Obiect_selectat].x_st, v[Obiect_selectat].y_dr,v[Obiect_selectat].x_st, v[Obiect_selectat].y_st);
    setcolor(nr_culoare);
    Obiect_selectat = -1;
    selectat = 0;
    ///pentru stergerea valorii marimii fizice
    setfillstyle(SOLID_FILL, BLACK);
    bar(977, 125, 990, 135);
}
void stergere_Piesa(int Obiect)
{
    int i;
    setfillstyle(SOLID_FILL, BLACK);
    bar(v[Obiect].x_st,v[Obiect].y_st, v[Obiect].x_dr+1, v[Obiect].y_dr+1);
    ///sa sterg marimea fizica
    setfillstyle(SOLID_FILL, BLACK);
    bar(976, 119, 1012, 134);
    Obiect_selectat = -1;
    selectat = 0;
    for(i=Obiect; i<contor-1; i++)
        Schimb_Piesa(i,i+1);
    contor--;
    for(i=0; i<linn.size(); i++)
    {
        if(linn[i].st>Obiect)
            linn[i].st--;
        if(linn[i].dr>Obiect)
            linn[i].dr--;
    }
}
void stergere_PiesaTotal(int Obiect)
{
    int i;
    setfillstyle(SOLID_FILL, BLACK);
    bar(v[Obiect].x_st,v[Obiect].y_st, v[Obiect].x_dr+1, v[Obiect].y_dr+1);
    ///sa sterg marimea fizica
    setfillstyle(SOLID_FILL, BLACK);
    bar(976, 119, 1012, 134);
    Obiect_selectat = -1;
    selectat = 0;
    if(v[Obiect].nr_noduri>0)
    {
        for(i=0; i<linn.size(); i++)
            if(linn[i].st==Obiect||linn[i].dr==Obiect)
            {
                restante.push_back(i);
                v[linn[i].st].nod[linn[i].ndst].okk=0;
                v[linn[i].dr].nod[linn[i].nddr].okk=0;
            }
        stergerelegaturi();
        restante.clear();
        for(i=linn.size()-1; i>=0; i--)
            if(linn[i].st==Obiect||linn[i].dr==Obiect)
                linn.erase(linn.begin()+i);
    }
    for(i=Obiect; i<contor-1; i++)
        Schimb_Piesa(i,i+1);
    contor--;
    for(i=0; i<linn.size(); i++)
    {
        if(linn[i].st>Obiect)
            linn[i].st--;
        if(linn[i].dr>Obiect)
            linn[i].dr--;
    }
}
void Schimb_Piesa(int i,int j)
{
    int k;
    v[i].x=v[j].x;
    v[i].y=v[j].y;
    v[i].x_st=v[j].x_st;
    v[i].x_dr=v[j].x_dr;
    v[i].y_st=v[j].y_st;
    v[i].y_dr=v[j].y_dr;
    v[i].nr_noduri=v[j].nr_noduri;
    v[i].valoare = v[j].valoare;
    for(k=0; k<v[i].nr_noduri; k++)
    {
        v[i].nod[k].x_nod=v[j].nod[k].x_nod;
        v[i].nod[k].y_nod=v[j].nod[k].y_nod;
        v[i].nod[k].okk=v[j].nod[k].okk;
    }
    v[i].tip=v[j].tip;
    v[i].dimm=v[j].dimm;
    v[i].ang=v[j].ang;
}
void DateNoi(double x, double y, double val_x, double val_y,int carePiesa)///aici introduc datele problemei
{
    v[contor].valoare = -1;
    v[contor].x = x;
    v[contor].y = y;
    v[contor].x_st = min(min(newx(x - val_x,y-val_y,x,y),newx(x + val_x,y+val_y,x,y)),min(newx(x + val_x,y-val_y,x,y),newx(x - val_x,y+val_y,x,y)));
    v[contor].y_st = min(min(newy(x - val_x,y-val_y,x,y),newy(x + val_x,y+val_y,x,y)),min(newy(x + val_x,y-val_y,x,y),newy(x - val_x,y+val_y,x,y)));
    v[contor].x_dr = max(max(newx(x - val_x,y-val_y,x,y),newx(x + val_x,y+val_y,x,y)),max(newx(x + val_x,y-val_y,x,y),newx(x - val_x,y+val_y,x,y)));
    v[contor].y_dr =max(max(newy(x - val_x,y-val_y,x,y),newy(x + val_x,y+val_y,x,y)),max(newy(x + val_x,y-val_y,x,y),newy(x - val_x,y+val_y,x,y)));
    if(carePiesa==1)
    {
        v[contor].nr_noduri=2;
        v[contor].nod[0].x_nod=newx(x-28*dim,y,x,y);
        v[contor].nod[0].y_nod=newy(x-28*dim,y,x,y);
        v[contor].nod[1].x_nod=newx(x+28*dim,y,x,y);
        v[contor].nod[1].y_nod=newy(x+28*dim,y,x,y);
    }
    else if(carePiesa==2)
    {
        v[contor].nr_noduri=2;
        v[contor].nod[0].x_nod=newx(x-28*dim,y,x,y);
        v[contor].nod[0].y_nod=newy(x-28*dim,y,x,y);
        v[contor].nod[1].x_nod=newx(x+28*dim,y,x,y);
        v[contor].nod[1].y_nod=newy(x+28*dim,y,x,y);
    }
    else if(carePiesa==3)
    {
        v[contor].nr_noduri=3;
        v[contor].nod[0].x_nod=newx(x-23*dim,y,x,y);
        v[contor].nod[0].y_nod=newy(x-23*dim,y,x,y);
        v[contor].nod[1].x_nod=newx(x+23*dim,y-33*dim,x,y);
        v[contor].nod[1].y_nod=newy(x+23*dim,y-33*dim,x,y);
        v[contor].nod[2].x_nod=newx(x+23*dim,y+33*dim,x,y);
        v[contor].nod[2].y_nod=newy(x+23*dim,y+33*dim,x,y);
    }
    else if(carePiesa==4)
    {
        v[contor].nr_noduri=2;
        v[contor].nod[0].x_nod=newx(x-38*dim,y,x,y);
        v[contor].nod[0].y_nod=newy(x-38*dim,y,x,y);
        v[contor].nod[1].x_nod=newx(x+38*dim,y,x,y);
        v[contor].nod[1].y_nod=newy(x+38*dim,y,x,y);
    }
    else if(carePiesa==5)
    {
        v[contor].nr_noduri=2;
        v[contor].nod[0].x_nod=newx(x-38*dim,y,x,y);
        v[contor].nod[0].y_nod=newy(x-38*dim,y,x,y);
        v[contor].nod[1].x_nod=newx(x+38*dim,y,x,y);
        v[contor].nod[1].y_nod=newy(x+38*dim,y,x,y);
    }
    else if(carePiesa==6)
    {
        v[contor].nr_noduri=2;
        v[contor].nod[0].x_nod=newx(x-28*dim,y,x,y);
        v[contor].nod[0].y_nod=newy(x-28*dim,y,x,y);
        v[contor].nod[1].x_nod=newx(x+28*dim,y,x,y);
        v[contor].nod[1].y_nod=newy(x+28*dim,y,x,y);
    }
    else if(carePiesa==7)
    {
        v[contor].nr_noduri=2;
        v[contor].nod[0].x_nod=newx(x-38*dim,y,x,y);
        v[contor].nod[0].y_nod=newy(x-38*dim,y,x,y);
        v[contor].nod[1].x_nod=newx(x+38*dim,y,x,y);
        v[contor].nod[1].y_nod=newy(x+38*dim,y,x,y);
    }
    else if(carePiesa==8)
    {
        v[contor].nr_noduri=2;
        v[contor].nod[0].x_nod=newx(x-38*dim,y,x,y);
        v[contor].nod[0].y_nod=newy(x-38*dim,y,x,y);
        v[contor].nod[1].x_nod=newx(x+38*dim,y,x,y);
        v[contor].nod[1].y_nod=newy(x+38*dim,y,x,y);
    }
    else if(carePiesa==9)
    {
        v[contor].nr_noduri=2;
        v[contor].nod[0].x_nod=newx(x-33*dim,y,x,y);
        v[contor].nod[0].y_nod=newy(x-33*dim,y,x,y);
        v[contor].nod[1].x_nod=newx(x+33*dim,y,x,y);
        v[contor].nod[1].y_nod=newy(x+33*dim,y,x,y);
    }
    v[contor].dimm=dim;
    v[contor].ang=val;
    setcolor(WHITE);
    for(int i=0; i<v[contor].nr_noduri; i++)
    {
        circle(v[contor].nod[i].x_nod,v[contor].nod[i].y_nod,5*v[contor].dimm);
    }
    setcolor(nr_culoare);
    for(int j=0; j<v[contor].nr_noduri; j++)
        v[contor].nod[j].okk=0;
    v[contor++].tip = carePiesa;
}
void visualPiesa(double x, double y, int but) ///aici pun ce piesa a fost selectata in partea din dreapta
{
    switch(but)
    {
    case 1:
        generator(x, y,dim);
        break;
    case 2:
        intrerupator(x, y,dim);
        break;
    case 3:
        tranzistor(x, y,dim);
        break;
    case 4:
        rezistor(x, y,dim);
        break;
    case 5:
        bec(x, y,dim);
        break;
    case 6:
        baterie(x, y,dim);
        break;
    case 7:
        voltmetru(x, y,dim);
        break;
    case 8:
        ampermetru(x, y,dim);
        break;
    case 9:
        difuzor(x, y,dim);
        break;
    }
}
void cautPiesa_si_Desen(int crt)
{
    int tipp=v[crt].tip;
    double retinval=val;///retin valoarea globala a unghiului
    val=v[crt].ang;/// val ia valoarea unhgiului la care este rotita piesa curenta
    switch(tipp)
    {
    case 1:

        generator(v[crt].x, v[crt].y,v[crt].dimm);
        break;
    case 2:
        intrerupator(v[crt].x, v[crt].y,v[crt].dimm);
        break;
    case 3:
        tranzistor(v[crt].x, v[crt].y,v[crt].dimm);
        break;
    case 4:
        rezistor(v[crt].x, v[crt].y,v[crt].dimm);
        break;
    case 5:
        bec(v[crt].x, v[crt].y,v[crt].dimm);
        break;
    case 6:
        baterie(v[crt].x, v[crt].y,v[crt].dimm);
        break;
    case 7:
        voltmetru(v[crt].x, v[crt].y,v[crt].dimm);
        break;
    case 8:
        ampermetru(v[crt].x, v[crt].y,v[crt].dimm);
        break;
    case 9:
        difuzor(v[crt].x, v[crt].y,v[crt].dimm);
        break;
    }
    val=retinval;///la final val isi reia valoarea
}

void redesenarePiese()
{
    int i, tipPiesa,j,k,st,dr,st1,dr1,i1,j1,p,ii,kk;
    for(ii=0; ii<contor; ii++)
    {
        obstacol(v[ii].x_st-10*v[ii].dimm,v[ii].y_st-10*v[ii].dimm,v[ii].x_dr+10*v[ii].dimm,v[ii].y_dr+10*v[ii].dimm);
        for(kk=0; kk<v[ii].nr_noduri; kk++)
            nu_obstacol(v[ii].nod[kk].x_nod-20*v[ii].dimm,v[ii].nod[kk].y_nod-20*v[ii].dimm,v[ii].nod[kk].x_nod+20*v[ii].dimm,v[ii].nod[kk].y_nod+20*v[ii].dimm);
    }
    for (i = 0; i < contor; i++)
    {
        tipPiesa = v[i].tip;
        for(j=0; j<v[i].nr_noduri; j++)
            v[i].nod[j].okk=0;
        switch(tipPiesa)
        {
        case 1:
            generator(v[i].x, v[i].y, v[i].dimm);
            break;
        case 2:
            intrerupator(v[i].x, v[i].y, v[i].dimm);
            break;
        case 3:
            tranzistor(v[i].x, v[i].y, v[i].dimm);
            break;
        case 4:
            rezistor(v[i].x, v[i].y, v[i].dimm);
            break;
        case 5:
            bec(v[i].x, v[i].y, v[i].dimm);
            break;
        case 6:
            baterie(v[i].x, v[i].y, v[i].dimm);
            break;
        case 7:
            voltmetru(v[i].x, v[i].y, v[i].dimm);
            break;
        case 8:
            ampermetru(v[i].x, v[i].y, v[i].dimm);
            break;
        case 9:
            difuzor(v[i].x, v[i].y, v[i].dimm);
            break;
        }
        for(j=0; j<v[i].nr_noduri; j++)
        {
            circle(v[i].nod[j].x_nod,v[i].nod[j].y_nod,5*v[i].dimm);
        }
    }
    setcolor(WHITE);
    for (i = 0; i < catte; i++)
        linn.push_back({pere[i].vst,pere[i].vdr,pere[i].nvst,pere[i].nvdr});
    for(k=linn.size()-1; k>=0; k--)
    {
        st=v[linn[k].st].nod[linn[k].ndst].y_nod-109;
        st1=v[linn[k].dr].nod[linn[k].nddr].y_nod-109;
        dr=v[linn[k].st].nod[linn[k].ndst].x_nod-9;
        dr1=v[linn[k].dr].nod[linn[k].nddr].x_nod-9;
        if(lee(st,dr,st1,dr1))
        {
            prec[st][dr].l=st;
            prec[st][dr].c=dr;
            i=st1;
            j=dr1;
            while(i!=st||j!=dr)
            {
                line(j+10.5,i+110.5,prec[i][j].c+10.5,prec[i][j].l+110.5);
                bordarelinieleg(i,j);
                a[i][j]=-2;
                linn[k].pozz.push_back({i,j});
                i1=prec[i][j].l;
                j1=prec[i][j].c;
                i=i1;
                j=j1;
            }
            ///
            linn[k].pozz.push_back({st,dr});
            ///
            bordarelinieleg(st,dr);
            a[st][dr]=-2;
            v[linn[k].st].nod[linn[k].ndst].okk=1;
            v[linn[k].dr].nod[linn[k].ndst].okk=1;
        }
        golire();
        for(ii=0; ii<contor; ii++)
        {
            obstacol(v[ii].x_st-10*v[ii].dimm,v[ii].y_st-10*v[ii].dimm,v[ii].x_dr+10*v[ii].dimm,v[ii].y_dr+10*v[ii].dimm);
            for(kk=0; kk<v[ii].nr_noduri; kk++)
                if(!v[ii].nod[kk].okk)
                    nu_obstacol(v[ii].nod[kk].x_nod-20*v[ii].dimm,v[ii].nod[kk].y_nod-20*v[ii].dimm,v[ii].nod[kk].x_nod+20*v[ii].dimm,v[ii].nod[kk].y_nod+20*v[ii].dimm);
        }
    }
}
void redesenarePiese2()
{
    int i, tipPiesa,j,k,st,dr,st1,dr1,i1,j1,p,ii,kk;
    for (i = 0; i < contor; i++)
    {
        tipPiesa = v[i].tip;
        switch(tipPiesa)
        {
        case 1:
            generator(v[i].x, v[i].y, v[i].dimm);
            break;
        case 2:
            intrerupator(v[i].x, v[i].y, v[i].dimm);
            break;
        case 3:
            tranzistor(v[i].x, v[i].y, v[i].dimm);
            break;
        case 4:
            rezistor(v[i].x, v[i].y, v[i].dimm);
            break;
        case 5:
            bec(v[i].x, v[i].y, v[i].dimm);
            break;
        case 6:
            baterie(v[i].x, v[i].y, v[i].dimm);
            break;
        case 7:
            voltmetru(v[i].x, v[i].y, v[i].dimm);
            break;
        case 8:
            ampermetru(v[i].x, v[i].y, v[i].dimm);
            break;
        case 9:
            difuzor(v[i].x, v[i].y, v[i].dimm);
            break;
        }
        for(j=0; j<v[i].nr_noduri; j++)
        {
            circle(v[i].nod[j].x_nod,v[i].nod[j].y_nod,5*v[i].dimm);
        }
    }
    setcolor(WHITE);
    for(k=0; k<linn.size(); k++)
    {
        for(j=0;j<linn[k].pozz.size()-1;j++)
            line(linn[k].pozz[j].c+10.5,linn[k].pozz[j].l+110.5,linn[k].pozz[j+1].c+10.5,linn[k].pozz[j+1].l+110.5);

    }
    setcolor(nr_culoare);
}
void StergereEcran()
{
    int j;
    golirematrice();
    unu=doi=0;
    x1=y1=x2=y2=0;
    nodstart=nodend=-1;
    setfillstyle(SOLID_FILL, BLACK);
    bar(1, 101, 899, 800);///stergere spatiu de lucru
    bar(903, 3, 1015, 100); ///coltul dreapta sus
    bar(976, 119, 1012, 134); ///marimea fizica
    deselectare_Piesa(Obiect_selectat);
    for (j = 0; j < contor; j++)
    {
        v[j].valoare=-1;
        v[j].x_st = 0;
        v[j].y_st = 0;
        v[j].x_dr = 0;
        v[j].y_dr = 0;
        v[j].x = 0;
        v[j].y = 0;
        v[j].tip = 0;
        v[j].dimm=0;
        v[j].ang=0;
        for(int k=0; k<v[j].nr_noduri; k++)
        {
            v[j].nod[k].x_nod=0;
            v[j].nod[k].y_nod=0;
            v[j].nod[k].okk=0;
        }
        v[j].nr_noduri=0;
    }
    linn.clear();
    for (int i = 0; i < catte; i++)
    {
        pere[i].vdr = 0;
        pere[i].vst = 0;
        pere[i].nvdr = 0;
        pere[i].nvst = 0;
    }
    catte = 0;
    contor = 0;
    selectat = 0;
    Obiect_selectat = -1;
    but = -1;
    setcolor(nr_culoare);
}
void intrebareUtilizator() ///functia asta verifica daca utilizatorul vrea sa salveze datele noi introduse cand se deschide sau se creeaza un fisier nou
{
    ///datele curente trebuie salvate
    int lg = strlen(fisier);
    char ch[101];
    int ok, i;
    ///intrebam daca utilizatorul vrea sa salveze datele
    ///de facut si in alte limbi.
    if (nr_limba == 1)
        printf("Doriti sa salvati ultimele modificari? Apasati d/n. ");
    else if (nr_limba == 2)
        printf("Do you want to save the latest changes? Press y/n. ");
    else if (nr_limba == 3)
        printf("M%cchten Sie die letzten %cnderungen speichern? Dr%ccken Sie j/n. ", 153, 142, 129);
    else if (nr_limba == 4)
        printf("Voulez-vous enregistrer les derni%cres modifications? Appuyez sur o/n. ", 138);
    while(true)
    {
        cin >> ch;
        if (nr_limba == 1)
        {
            if (strcmp(ch, "d") == 0)
            {
                ok = 1;
                break;
            }
            else if (strcmp(ch, "n") == 0)
            {
                ok = 0;
                break;
            }
            else
                cout << "Nu ati tastat butonul potrivit. Doriti sa le salvati? Apasati d/n. ";
        }
        else if (nr_limba == 2)
        {
            if (strcmp(ch, "y") == 0)
            {
                ok = 1;
                break;
            }
            else if (strcmp(ch, "n") == 0)
            {
                ok = 0;
                break;
            }
            else
                printf("You didn't press the right button. Do you want to save them? Press y/n. ");
        }
        else if (nr_limba == 3)
        {
            if (strcmp(ch, "j") == 0)
            {
                ok = 1;
                break;
            }
            else if (strcmp(ch, "n") == 0)
            {
                ok = 0;
                break;
            }
            else
                printf("Sie haben nicht die richtige Taste gedr%cckt. M%cchten Sie sie speichern? Dr%ccken Sie j/n. ", 129, 148, 129);
        }
        else if (nr_limba == 4)
        {
            if (strcmp(ch, "o") == 0)
            {
                ok = 1;
                break;
            }
            else if (strcmp(ch, "n") == 0)
            {
                ok = 0;
                break;
            }
            else
                printf("Vous n'avez pas appuy%c sur le bouton droit. Voulez-vous les enregistrer? Appuyez sur o/n. ", 130);
        }
    }
    if (ok == 1)
    {
        if (lg == 1)  ///daca datele nu sunt salvate intr-un fisier, cream un fisier nou si le salvam acolo
        {
            if (nr_limba == 1)
                cout << "Datele trebuie salvate intr-un fisier. Introduceti numele fisierului cu extensia .txt: ";
            else if (nr_limba == 2)
                cout << "The data must be saved in a file. Enter the file name with the .txt extension: ";
            else if (nr_limba == 3)
                printf("Die Daten m%cssen in einer Datei gespeichert werden. Geben Sie den Dateinamen mit der Erweiterung .txt ein: ", 129);
            else if (nr_limba == 4)
                printf("Les donn%ces doivent %ctre enregistr%ces dans un fichier. Entrez le nom du fichier avec l'extension .txt: ", 130, 136, 130);
            while (true)
            {
                cin >> fisier;
                ///aici verific daca fisierul nu exista deja ca sa nu suprapun datele
                strcat(folder, ".\\Fisiere\\");
                strcat(folder, fisier);
                if(fopen(folder,"r")!= NULL)
                {
                    if (nr_limba == 1)
                        cout << "Fisierul deja exista. Introduceti alt nume: ";
                    else if (nr_limba == 2)
                        cout << "The file already existed. Enter another name: ";
                    else if (nr_limba == 3)
                        printf("Die Datei war bereits vorhanden. Einen anderen Namen eingeben: ");
                    else if (nr_limba == 4)
                        printf("Le fichier existe d%cj%c. Entrez un autre nom: ", 130, 133);
                    lg = strlen(folder);
                    for (i = 0; i < lg; i++)
                        folder[i] = 0;
                }
                else
                {
                    lg = strlen(fisier);
                    if (lg > 96)
                    {
                        if (nr_limba == 1)
                            cout << "Numele este prea lung. Incercati inca o data. \n";
                        else if (nr_limba == 2)
                            cout << "The name is too long. Please try again. \n";
                        else if (nr_limba == 3)
                            printf("Der Name ist zu lang. Bitte versuche es erneut. \n");
                        else if (nr_limba == 4)
                            printf("Le nom est trop long. Veuillez r%cessayer. \n", 130);
                    }
                    else if (fisier[lg - 1] != 't' || fisier[lg - 2] != 'x' || fisier[lg - 3] != 't' || fisier[lg - 4] != '.')
                    {
                        if (nr_limba == 1)
                            cout << "Extensia nu este corecta. Incercati inca o data.\n";
                        else if (nr_limba == 2)
                            cout << "The extension is incorrect. Please try again.\n";
                        else if (nr_limba == 3)
                            printf("Die Erweiterung ist falsch. Bitte versuchen Sie es erneut.\n");
                        else if (nr_limba == 4)
                            printf("L'extension est incorrecte. Veuillez r%cessayer.\n", 130);
                    }
                    else if (lg <= 4)
                    {
                        if (nr_limba == 1)
                            cout << "Numele fisierului este prea scurt. Incercati inca o data. \n";
                        else if (nr_limba == 2)
                            cout << "File name is too short. Please try again. \n";
                        else if (nr_limba == 3)
                            printf("Dateiname ist zu kurz. Bitte versuchen Sie es erneut.\n");
                        else if (nr_limba == 4)
                            printf("Le nom du fichier est trop court. Veuillez r%cessayer.\n", 130);
                    }
                    else
                    {
                        if (nr_limba == 1)
                            printf("Fisierul cu numele %s s-a salvat.\n\n", fisier);
                        else if (nr_limba == 2)
                            printf("The file named %s has been saved. \n\n", fisier);
                        else if (nr_limba == 3)
                            printf("Die Datei mit dem Namen %s wurde gespeichert. \n\n", fisier);
                        else if (nr_limba == 4)
                            printf("Le fichier nomm%c %s a %ct%c enregistr%c. \n\n", 130, 130, 130, 130);
                        numarFisiere++;
                        break;
                    }
                }
            }
        }
        else
        {
            if (nr_limba == 1)
                printf("In fisierul %s au fost salvate modificarile.\n", fisier);
            else if (nr_limba == 2)
                printf("Changes saved to file %s. \n", fisier);
            else if (nr_limba == 3)
                printf("%cnderungen in Datei %s gespeichert. \n", 142, fisier);
            else if (nr_limba == 4)
                printf("Modifications enregistr%ces dans le fichier %s. \n", 130, fisier);
        }
        ///stergem in fiserul folder ca sa nu se suprapuna cu alte informatii
        salvareDate();
    }
    ///stergem datele din fisier si din folder pentru ca utilizatorul va deschide un fisier nou fara nume
    for (i = 0; i < lg; i++)
    {
        folder[i] = 0;
        fisier[i] = 0;
    }
    StergereEcran();
    fisier[0] = '1';
}
void salvareDate()
///salvarea datelor in fisiere
{
    int lg, i, j;
    ///cream calea pana la folder
    lg = strlen(folder);
    for (i = 0; i < lg; i++)
        folder[i] = 0;
    strcat(folder, ".\\Fisiere\\");
    strcat(folder, fisier);
    ofstream Myfile(folder);
    Myfile << contor << '\n';
    for (i = 0; i < contor; i++)
    {
        Myfile << v[i].valoare << '\n' << v[i].x << ' ' << v[i].y << '\n';
        Myfile << v[i].x_st << ' ' << v[i].y_st << ' ' << v[i].x_dr << ' ' << v[i].y_dr << '\n';
        Myfile << v[i].nr_noduri << '\n';
        for (j = 0; j < v[i].nr_noduri; j++)
            Myfile << v[i].nod[j].x_nod << ' ' << v[i].nod[j].y_nod << ' ' << v[i].nod[j].okk << '\n';
        Myfile << v[i].dimm << ' ' << v[i].ang << ' ' << v[i].tip << '\n' << '\n';
    }
    catte=linn.size();
    Myfile << catte << '\n';
    for(i=0; i<catte; i++)
    {
        pere[i].vst = linn[i].st;
        pere[i].vdr = linn[i].dr;
        pere[i].nvst = linn[i].ndst;
        pere[i].nvdr = linn[i].nddr;
        Myfile<<pere[i].vst<< ' ' << pere[i].vdr << ' ' << pere[i].nvst << ' ' << pere[i].nvdr << '\n';
    }
    lg = strlen(folder);
    for (int i = 0; i < lg; i++)
        folder[i] = 0;
    ///inchidem fisierul pentru a curata memorie inutila
    Myfile.close();
}
void generator(double x,double y,double c)
{
    line(newx(x-25*c,y,x,y),newy(x-25*c,y,x,y),newx(x-5*c,y,x,y),newy(x-5*c,y,x,y));
    line(newx(x+25*c,y,x,y),newy(x+25*c,y,x,y),newx(x+5*c,y,x,y),newy(x+5*c,y,x,y));
    line(newx(x-2*c,y-10*c,x,y),newy(x-2*c,y-10*c,x,y),newx(x-2*c,y+10*c,x,y),newy(x-2*c,y+10*c,x,y));
    line(newx(x+5*c,y-16*c,x,y),newy(x+5*c,y-16*c,x,y),newx(x+5*c,y+16*c,x,y),newy(x+5*c,y+16*c,x,y));
    line(newx(x+10*c,y-5*c,x,y),newy(x+10*c,y-5*c,x,y),newx(x+10*c,y-12*c,x,y),newy(x+10*c,y-12*c,x,y));
    line(newx(x+13*c,y-8*c,x,y),newy(x+13*c,y-8*c,x,y),newx(x+7*c,y-8*c,x,y),newy(x+7*c,y-8*c,x,y));
    line(newx(x-10*c,y-8*c,x,y),newy(x-10*c,y-8*c,x,y),newx(x-13*c,y-8*c,x,y),newy(x-13*c,y-8*c,x,y));
}
void intrerupator(double x,double y,double c)
{
    line(newx(x-25*c,y,x,y),newy(x-25*c,y,x,y),newx(x-10*c,y,x,y),newy(x-10*c,y,x,y));
    line(newx(x+10,y-20*c,x,y),newy(x+10,y-20*c,x,y),newx(x-10*c,y,x,y),newy(x-10*c,y,x,y));
    line(newx(x+10,y,x,y),newy(x+10,y,x,y),newx(x+25*c,y,x,y),newy(x+25*c,y,x,y));
}
void tranzistor(double x,double y,double c)
{
    line(newx(x,y-20*c,x,y),newy(x,y-20*c,x,y),newx(x,y+20*c,x,y),newy(x,y+20*c,x,y));
    line(newx(x,y,x,y),newy(x,y,x,y),newx(x-20*c,y,x,y),newy(x-20*c,y,x,y));
    line(newx(x,y,x,y),newy(x,y,x,y),newx(x+20*c,y-20*c,x,y),newy(x+20*c,y-20*c,x,y));
    line(newx(x+20*c,y-30*c,x,y),newy(x+20*c,y-30*c,x,y),newx(x+20*c,y-20*c,x,y),newy(x+20*c,y-20*c,x,y));
    line(newx(x,y,x,y),newy(x,y,x,y),newx(x+20*c,y+20*c,x,y),newy(x+20*c,y+20*c,x,y));
    line(newx(x+20*c,y+30*c,x,y),newy(x+20*c,y+30*c,x,y),newx(x+20*c,y+20*c,x,y),newy(x+20*c,y+20*c,x,y));
}
void rezistor(double x,double y,double c)
{
    line(newx(x-25*c,y-15*c,x,y),newy(x-25*c,y-15*c,x,y),newx(x+25*c,y-15*c,x,y),newy(x+25*c,y-15*c,x,y));
    line(newx(x+25*c,y-15*c,x,y),newy(x+25*c,y-15*c,x,y),newx(x+25*c,y+15*c,x,y),newy(x+25*c,y+15*c,x,y));
    line(newx(x+25*c,y+15*c,x,y),newy(x+25*c,y+15*c,x,y),newx(x-25*c,y+15*c,x,y),newy(x-25*c,y+15*c,x,y));
    line(newx(x-25*c,y+15*c,x,y),newy(x-25*c,y+15*c,x,y),newx(x-25*c,y-15*c,x,y),newy(x-25*c,y-15*c,x,y));
    line(newx(x-25*c,y,x,y),newy(x-25*c,y,x,y),newx(x-35*c,y,x,y),newy(x-35*c,y,x,y));
    line(newx(x+25*c,y,x,y),newy(x+25*c,y,x,y),newx(x+35*c,y,x,y),newy(x+35*c,y,x,y));
}
void bec(double x,double y,double c)
{
    circle(x,y,20*c);
    line(newx(x+10*c,y+10*c,x,y),newy(x+10*c,y+10*c,x,y),newx(x-10*c,y-10*c,x,y),newy(x-10*c,y-10*c,x,y));
    line(newx(x-10*c,y+10*c,x,y),newy(x-10*c,y+10*c,x,y),newx(x+10*c,y-10*c,x,y),newy(x+10*c,y-10*c,x,y));
    line(newx(x+20*c,y,x,y),newy(x+20*c,y,x,y),newx(x+35*c,y,x,y),newy(x+35*c,y,x,y));
    line(newx(x-20*c,y,x,y),newy(x-20*c,y,x,y),newx(x-35*c,y,x,y),newy(x-35*c,y,x,y));
}
void baterie(double x,double y,double c)
{
    line(newx(x-25*c,y,x,y),newy(x-25*c,y,x,y),newx(x-10*c,y,x,y),newy(x-10*c,y,x,y));
    line(newx(x+25*c,y,x,y),newy(x+25*c,y,x,y),newx(x+10*c,y,x,y),newy(x+10*c,y,x,y));
    line(newx(x-10*c,y-16*c,x,y),newy(x-10*c,y-16*c,x,y),newx(x-10*c,y+16*c,x,y),newy(x-10*c,y+16*c,x,y));
    line(newx(x+10*c,y-10*c,x,y),newy(x+10*c,y-10*c,x,y),newx(x+10*c,y+10*c,x,y),newy(x+10*c,y+10*c,x,y));
}
void voltmetru(double x,double y,double c)
{
    circle(x,y,20*c);
    line(newx(x+20*c,y,x,y),newy(x+20*c,y,x,y),newx(x+35*c,y,x,y),newy(x+35*c,y,x,y));
    line(newx(x-20*c,y,x,y),newy(x-20*c,y,x,y),newx(x-35*c,y,x,y),newy(x-35*c,y,x,y));
    line(newx(x,y+10*c,x,y),newy(x,y+10*c,x,y),newx(x-10*c,y-10*c,x,y),newy(x-10*c,y-10*c,x,y));
    line(newx(x,y+10*c,x,y),newy(x,y+10*c,x,y),newx(x+10*c,y-10*c,x,y),newy(x+10*c,y-10*c,x,y));
}
void ampermetru(double x,double y,double c)
{
    circle(x,y,20*c);
    line(newx(x+20*c,y,x,y),newy(x+20*c,y,x,y),newx(x+35*c,y,x,y),newy(x+35*c,y,x,y));
    line(newx(x-20*c,y,x,y),newy(x-20*c,y,x,y),newx(x-35*c,y,x,y),newy(x-35*c,y,x,y));
    line(newx(x,y-10*c,x,y),newy(x,y-10*c,x,y),newx(x+5*c,y+5*c,x,y),newy(x+5*c,y+5*c,x,y));
    line(newx(x,y-10*c,x,y),newy(x,y-10*c,x,y),newx(x-5*c,y+5*c,x,y),newy(x-5*c,y+5*c,x,y));
    line(newx(x-2*c,y,x,y),newy(x-2*c,y,x,y),newx(x+2*c,y,x,y),newy(x+2*c,y,x,y));
}
void difuzor(double x,double y,double c)
{
    line(newx(x-10*c,y-10*c,x,y),newy(x-10*c,y-10*c,x,y),newx(x+10*c,y-10*c,x,y),newy(x+10*c,y-10*c,x,y));
    line(newx(x+10*c,y-10*c,x,y),newy(x+10*c,y-10*c,x,y),newx(x+10*c,y+10*c,x,y),newy(x+10*c,y+10*c,x,y));
    line(newx(x+10*c,y+10*c,x,y),newy(x+10*c,y+10*c,x,y),newx(x-10*c,y+10*c,x,y),newy(x-10*c,y+10*c,x,y));
    line(newx(x-10*c,y+10*c,x,y),newy(x-10*c,y+10*c,x,y),newx(x-10*c,y-10*c,x,y),newy(x-10*c,y-10*c,x,y));
    line(newx(x-10*c,y-20*c,x,y),newy(x-10*c,y-20*c,x,y),newx(x-10*c,y+20*c,x,y),newy(x-10*c,y+20*c,x,y));
    line(newx(x-10*c,y-10*c,x,y),newy(x-10*c,y-10*c,x,y),newx(x-20*c,y-20*c,x,y),newy(x-20*c,y-20*c,x,y));
    line(newx(x-10*c,y+10*c,x,y),newy(x-10*c,y+10*c,x,y),newx(x-20*c,y+20*c,x,y),newy(x-20*c,y+20*c,x,y));
    line(newx(x-20*c,y-20*c,x,y),newy(x-20*c,y-20*c,x,y),newx(x-20*c,y+20*c,x,y),newy(x-20*c,y+20*c,x,y));
    line(newx(x-20*c,y,x,y),newy(x-20*c,y,x,y),newx(x-30*c,y,x,y),newy(x-30*c,y,x,y));
    line(newx(x+10*c,y,x,y),newy(x+10*c,y,x,y),newx(x+30*c,y,x,y),newy(x+30*c,y,x,y));
}
void obstacol(double xst,double yst,double xdr,double ydr)
{
    int xst1=xst-9,yst1=yst-109,xdr1=xdr-9,ydr1=ydr-109,i,j;
    for(i=yst1; i<=ydr1; i++)
        for(j=xst1; j<=xdr1; j++)
            if(a[i][j]!=-2)
                a[i][j]=-1;
}
void nu_obstacol(double xst,double yst,double xdr,double ydr)
{
    int xst1=xst-9,yst1=yst-109,xdr1=xdr-9,ydr1=ydr-109,i,j;
    for(i=yst1; i<=ydr1; i++)
        for(j=xst1; j<=xdr1; j++)
            if(a[i][j]!=-2)
                a[i][j]=0;
}
void fara_obstacol(double xst,double yst,double xdr,double ydr)
{
    int xst1=xst-9,yst1=yst-109,xdr1=xdr-9,ydr1=ydr-109,i,j;
    for(i=yst1; i<=ydr1; i++)
        for(j=xst1; j<=xdr1; j++)
            a[i][j]=0;
}
void bordare()
{
    int i;
    for(i=0; i<=n+1; i++)
        a[i][m+1]=a[i][0]=-1;
    for(i=0; i<=m+1; i++)
        a[0][i]=a[n+1][i]=-1;
}
void desenlinie(double xa,double ya,double xb,double yb)
{
    int i1,j1,i,j,st,dr,st1,dr1;
    st=ya-109;
    st1=yb-109;
    dr=xa-9;
    dr1=xb-9;
    if(lee(st,dr,st1,dr1))
    {
        ///pun linia
        linn.push_back({unu,doi,nodstart,nodend});
        ///
        prec[st][dr].l=st;
        prec[st][dr].c=dr;
        i=st1;
        j=dr1;
        while(i!=st||j!=dr)
        {
            line(j+10.5,i+110.5,prec[i][j].c+10.5,prec[i][j].l+110.5);
            bordarelinieleg(i,j);
            a[i][j]=-2;
            linn[linn.size()-1].pozz.push_back({i,j});
            i1=prec[i][j].l;
            j1=prec[i][j].c;
            i=i1;
            j=j1;
        }
        ///
        linn[linn.size()-1].pozz.push_back({st,dr});
        ///
        bordarelinieleg(st,dr);
        a[st][dr]=-2;
    }
    else /// punctele de legatura sunt marcate ca nevizitate unde e cazul
    {
        if(nodstart!=-1)
            v[unu].nod[nodstart].okk=0;
        if(nodend!=-1)
            v[doi].nod[nodend].okk=0;
    }
}


bool lee(int i,int j,int i1,int j1)
{
    int lin,col,lin1,col1,k;
    double sl,sc,fl,fc;
    if(a[i][j]!=0)
        return 0;
    q.push({i,j});
    a[i][j]=1;
    prec[i][j].l=i;
    prec[i][j].c=j;
    while(!q.empty())
    {
        lin=q.front().first;
        col=q.front().second;
        if(lin==i1&&col==j1)
            return 1;
        q.pop();
        sl=prec[lin][col].l+0.5;
        sc=prec[lin][col].c+0.5;
        for(k=0; k<4; k++)
        {
            lin1=lin+dl[k];
            col1=col+dc[k];
            fl=lin1+0.5;
            fc=col1+0.5;
            if(verificapunct(lin,col,lin1,col1))///daca nu intersecteaza o linie
            {
                if(a[lin1][col1]==0)
                {
                    if(distt(sl,sc,fl,fc)==1||distt(sl,sc,fl,fc)==2)
                        cc[lin1][col1]=cc[lin][col];
                    else
                        cc[lin1][col1]=cc[lin][col]+1;
                    prec[lin1][col1].l=lin;
                    prec[lin1][col1].c=col;
                    a[lin1][col1]=a[lin][col]+1;
                    q.push({lin1,col1});
                }
                else if(a[lin1][col1]>0&&a[lin1][col1]==a[lin][col]+1)
                {
                    if(distt(sl,sc,fl,fc)==1||distt(sl,sc,fl,fc)==2)
                    {
                        if(cc[lin][col]<cc[lin1][col1])
                        {
                            cc[lin1][col1]=cc[lin][col];
                            prec[lin1][col1].l=lin;
                            prec[lin1][col1].c=col;
                        }
                    }
                    else
                    {
                        if(cc[lin][col]+1<cc[lin1][col1])
                        {
                            cc[lin1][col1]=cc[lin][col]+1;

                            prec[lin1][col1].l=lin;
                            prec[lin1][col1].c=col;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
double distt(double xa,double ya,double xb,double yb)
{
    return sqrt((xa-xb)*(xa-xb)+(ya-yb)*(ya-yb));
}
void golire()
{
    int i,j;
    for(i=1; i<=n; i++)
        for(j=1; j<=m; j++)
        {
            if(a[i][j]!=-2)
                a[i][j]=0;
            cc[i][j]=0;
            prec[i][j].l=prec[i][j].c=0;
        }
    while(!q.empty())
        q.pop();
}
void golirematrice()
{
    int i,j;
    for(i=1; i<=n; i++)
        for(j=1; j<=m; j++)
        {
            a[i][j]=0;
            cc[i][j]=0;
            prec[i][j].l=prec[i][j].c=0;
        }
    while(!q.empty())
        q.pop();
}
bool verificareLegatura(double xst,double yst,double xdr,double ydr)///verifica cand pun o piesa daca e pusa pe linie
{
    int xst1=xst-9,yst1=yst-109,xdr1=xdr-9,ydr1=ydr-109,i,j;
    for(i=yst1-6; i<=ydr1+6; i++)
        for(j=xst1-6; j<=xdr1+6; j++)
            if(a[i][j]==-2)
                return 0;
    return 1;
}
bool verificareLegaturaMutare(int crt,double xst,double yst)///verifica cand pun o piesa daca e pusa pe linie
{
    int xst1=v[crt].x_st-9,yst1=v[crt].y_st-109,xdr1=v[crt].x_dr-9,ydr1=v[crt].y_dr-109,i,j;
    if(xst>0)///dreapta
    {
        for(i=yst1; i<=ydr1; i++)
            for(j=xdr1+1; j<=xdr1+10; j++)
                if(a[i][j]==-2)
                    return 0;
    }
    else if(xst<0)///stanga
    {
        for(i=yst1; i<=ydr1; i++)
            for(j=xst1-1; j>=xst1-10; j--)
                if(a[i][j]==-2)
                    return 0;
    }
    else if(yst>0)///jos
    {
        for(i=ydr1+1; i<=ydr1+10; i++)
            for(j=xst1; j<=xdr1; j++)
                if(a[i][j]==-2)
                    return 0;
    }
    else if(yst<0)///sus
    {
        for(i=yst1-1; i>=yst1-10; i--)
            for(j=xst1; j<=xdr1; j++)
                if(a[i][j]==-2)
                    return 0;
    }
    return 1;
}
void bordarelinieleg(int x,int y)/// bordare in jurul liniei de legatura
{
    int i,j;
    for(i=x-5; i<=x+5; i++)
        for(j=y-5; j<=y+5; j++)
            if(a[i][j]>=0&&a[i][j]!=-1)
                a[i][j]=-2;
}
void NU_bordarelinieleg(int x,int y)/// debordare in jurul liniei de legatura
{
    int i,j;
    for(i=x-5; i<=x+5; i++)
        for(j=y-5; j<=y+5; j++)
            if(a[i][j]==-2)
                a[i][j]=0;
}
void sterglinie()
{
    int k,i,j,i2,j2,nod1,leg1,nod2,leg2;
    for(k=0; k<linn.size(); k++)
    {
        i=linn[k].st;
        j=linn[k].ndst;
        i2=linn[k].dr;
        j2=linn[k].nddr;
        if (x1 >= v[i].nod[j].x_nod-v[i].dimm*10&& x1 <= v[i].nod[j].x_nod+v[i].dimm*10 && y1 >= v[i].nod[j].y_nod-v[i].dimm*10 && y1 <= v[i].nod[j].y_nod+v[i].dimm*10
                &&x2 >= v[i2].nod[j2].x_nod-v[i2].dimm*10&& x2 <= v[i2].nod[j2].x_nod+v[i2].dimm*10 && y2 >= v[i2].nod[j2].y_nod-v[i2].dimm*10 && y2 <= v[i2].nod[j2].y_nod+v[i2].dimm*10)
        {
            nod2=j=linn[k].st;
            leg2=linn[k].ndst;
            nod1=j=linn[k].dr;
            leg1=linn[k].nddr;
            break;
        }
        else if (x2 >= v[i].nod[j].x_nod-v[i].dimm*10&& x2 <= v[i].nod[j].x_nod+v[i].dimm*10 && y2 >= v[i].nod[j].y_nod-v[i].dimm*10 && y2 <= v[i].nod[j].y_nod+v[i].dimm*10
                 &&x1 >= v[i2].nod[j2].x_nod-v[i2].dimm*10&& x1 <= v[i2].nod[j2].x_nod+v[i2].dimm*10 && y1 >= v[i2].nod[j2].y_nod-v[i2].dimm*10 && y1 <= v[i2].nod[j2].y_nod+v[i2].dimm*10)
        {
            nod1=j=linn[k].st;
            leg1=linn[k].ndst;
            nod2=j=linn[k].dr;
            leg2=linn[k].nddr;
            break;
        }
    }
    if(k<linn.size())
    {
        setcolor(BLACK);
        for(i=0; i<linn[k].pozz.size()-1; i++)
        {
            line(linn[k].pozz[i].c+10.5,linn[k].pozz[i].l+110.5,linn[k].pozz[i+1].c+10.5,linn[k].pozz[i+1].l+110.5);
            NU_bordarelinieleg(linn[k].pozz[i].l,linn[k].pozz[i].c);
        }
        NU_bordarelinieleg(linn[k].pozz[i].l,linn[k].pozz[i].c);
        setcolor(nr_culoare);
        v[nod1].nod[leg1].okk=v[nod2].nod[leg2].okk=0;
        linn.erase(linn.begin()+k);
    }
}
bool verificapunct(int x,int y,int x1,int y1)
{
    int i,j;
    if(x1==x+1&&y1==y)
    {
        for(j=y-5; j<=y+5; j++)
            if(a[x+6][j]==-2)
                return 0;
    }
    else if(x1==x-1&&y1==y)
    {
        for(j=y-5; j<=y+5; j++)
            if(a[x-6][j]==-2)
                return 0;
    }
    else if(x==x1&&y1==y+1)
    {
        for(i=x-5; i<=x+5; i++)
            if(a[i][y+6]==-2)
                return 0;
    }
    else if(x==x1&&y1==y-1)
    {
        for(i=x-5; i<=x+5; i++)
            if(a[i][y-6]==-2)
                return 0;
    }
    return 1;
}
void stergerelegaturi()
{
    int i,j,k;
    setcolor(BLACK);
    for(j=restante.size()-1; j>=0; j--)
    {
        k=restante[j];
        for(i=0; i<linn[k].pozz.size()-1; i++)
        {
            line(linn[k].pozz[i].c+10.5,linn[k].pozz[i].l+110.5,linn[k].pozz[i+1].c+10.5,linn[k].pozz[i+1].l+110.5);
            NU_bordarelinieleg(linn[k].pozz[i].l,linn[k].pozz[i].c);
        }
        NU_bordarelinieleg(linn[k].pozz[i].l,linn[k].pozz[i].c);
    }
    setcolor(nr_culoare);
}
bool refacerelegaturi()
{
    int rezultat=0,unu,doi,nodstart,nodend,st,dr,st1,dr1,i,j,k,i1,j1,ii,jj;
    for(j=0; j<restante.size(); j++)
    {
        int care=restante[j];
        golire();
        for(i=0; i<contor; i++)
        {
            obstacol(v[i].x_st-10*v[i].dimm,v[i].y_st-10*v[i].dimm,v[i].x_dr+10*v[i].dimm,v[i].y_dr+10*v[i].dimm);
            for(k=0; k<v[i].nr_noduri; k++)
                if(!v[i].nod[k].okk)
                    nu_obstacol(v[i].nod[k].x_nod-20*v[i].dimm,v[i].nod[k].y_nod-20*v[i].dimm,v[i].nod[k].x_nod+20*v[i].dimm,v[i].nod[k].y_nod+20*v[i].dimm);
        }
        if(lee(v[linn[care].st].nod[linn[care].ndst].y_nod-109,v[linn[care].st].nod[linn[care].ndst].x_nod-9,v[linn[care].dr].nod[linn[care].nddr].y_nod-109,v[linn[care].dr].nod[linn[care].nddr].x_nod-9))
            rezultat++;
    }
    if(rezultat==restante.size())
    {
        setcolor(WHITE);
        for( jj=0; jj<restante.size(); jj++)
        {
            int care=restante[jj];
            golire();
            for(ii=0; ii<contor; ii++)
            {
                obstacol(v[ii].x_st-10*v[ii].dimm,v[ii].y_st-10*v[ii].dimm,v[ii].x_dr+10*v[ii].dimm,v[ii].y_dr+10*v[ii].dimm);
                for(k=0; k<v[ii].nr_noduri; k++)
                    if(!v[ii].nod[k].okk)
                        nu_obstacol(v[ii].nod[k].x_nod-20*v[ii].dimm,v[ii].nod[k].y_nod-20*v[ii].dimm,v[ii].nod[k].x_nod+20*v[ii].dimm,v[ii].nod[k].y_nod+20*v[ii].dimm);
            }
            if(lee(v[linn[care].st].nod[linn[care].ndst].y_nod-109,v[linn[care].st].nod[linn[care].ndst].x_nod-9,v[linn[care].dr].nod[linn[care].nddr].y_nod-109,v[linn[care].dr].nod[linn[care].nddr].x_nod-9))
            {
                unu=linn[care].st;
                doi=linn[care].dr;
                nodstart=linn[care].ndst;
                nodend=linn[care].nddr;
                st=v[unu].nod[nodstart].y_nod-109;
                dr=v[unu].nod[nodstart].x_nod-9;
                st1=v[doi].nod[nodend].y_nod-109;
                dr1=v[doi].nod[nodend].x_nod-9;
                linn.push_back({unu,doi,nodstart,nodend});
                prec[st][dr].l=st;
                prec[st][dr].c=dr;
                i=st1;
                j=dr1;
                while(i!=st||j!=dr)
                {
                    line(j+10.5,i+110.5,prec[i][j].c+10.5,prec[i][j].l+110.5);
                    bordarelinieleg(i,j);
                    a[i][j]=-2;
                    linn[linn.size()-1].pozz.push_back({i,j});
                    i1=prec[i][j].l;
                    j1=prec[i][j].c;
                    i=i1;
                    j=j1;
                }
                linn[linn.size()-1].pozz.push_back({st,dr});
                bordarelinieleg(st,dr);
                a[st][dr]=-2;
                v[unu].nod[nodstart].okk=1;
                v[doi].nod[nodend].okk=1;
            }
        }
        for(j=restante.size()-1; j>=0; j--)
            linn.erase(linn.begin()+restante[j]);
        setcolor(nr_culoare);
        return 1;
    }
    else
        return 0;
}
void refacerelegaturi2()
{
    int rezultat=0,unu,doi,nodstart,nodend,st,dr,st1,dr1,i,j,k,i1,j1,ii,jj;
    for( jj=0; jj<restante.size(); jj++)
    {
        int care=restante[jj];
        golire();
        for(ii=0; ii<contor; ii++)
        {
            obstacol(v[ii].x_st-10*v[ii].dimm,v[ii].y_st-10*v[ii].dimm,v[ii].x_dr+10*v[ii].dimm,v[ii].y_dr+10*v[ii].dimm);
            for(k=0; k<v[ii].nr_noduri; k++)
                if(!v[ii].nod[k].okk)
                    nu_obstacol(v[ii].nod[k].x_nod-20*v[ii].dimm,v[ii].nod[k].y_nod-20*v[ii].dimm,v[ii].nod[k].x_nod+20*v[ii].dimm,v[ii].nod[k].y_nod+20*v[ii].dimm);
        }
        if(lee(v[linn[care].st].nod[linn[care].ndst].y_nod-109,v[linn[care].st].nod[linn[care].ndst].x_nod-9,v[linn[care].dr].nod[linn[care].nddr].y_nod-109,v[linn[care].dr].nod[linn[care].nddr].x_nod-9))
        {
            unu=linn[care].st;
            doi=linn[care].dr;
            nodstart=linn[care].ndst;
            nodend=linn[care].nddr;
            st=v[unu].nod[nodstart].y_nod-109;
            dr=v[unu].nod[nodstart].x_nod-9;
            st1=v[doi].nod[nodend].y_nod-109;
            dr1=v[doi].nod[nodend].x_nod-9;
            linn.push_back({unu,doi,nodstart,nodend});
            prec[st][dr].l=st;
            prec[st][dr].c=dr;
            i=st1;
            j=dr1;
            while(i!=st||j!=dr)
            {
                line(j+10.5,i+110.5,prec[i][j].c+10.5,prec[i][j].l+110.5);
                bordarelinieleg(i,j);
                a[i][j]=-2;
                linn[linn.size()-1].pozz.push_back({i,j});
                i1=prec[i][j].l;
                j1=prec[i][j].c;
                i=i1;
                j=j1;
            }
            linn[linn.size()-1].pozz.push_back({st,dr});
            bordarelinieleg(st,dr);
            a[st][dr]=-2;
            v[unu].nod[nodstart].okk=1;
            v[doi].nod[nodend].okk=1;
        }
    }
    for(j=restante.size()-1; j>=0; j--)
        linn.erase(linn.begin()+restante[j]);
}
bool nu_are_linii(int Obiect)
{
    int i;
    for(i=0; i<linn.size(); i++)
        if(linn[i].st==Obiect||linn[i].dr==Obiect)
            return 0;
    return 1;
}
