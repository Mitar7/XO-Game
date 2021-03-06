// Mitar Milovanovic NRT-2/18
// Igra IKS-OKS 
// Jezik C

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 // Definisanje Strukture Igrac
typedef struct igrac {
    char ime[25];
    int broj_pobeda;
} IGRAC;
 //Definisanje Linked liste
typedef struct node {
    IGRAC data;
    struct node *next;
} NODE;
 
//Prikaz svih igraca ali iz liste
void prikaziIgrace(NODE *head){
    NODE *crnt = head; //CURRENT da pokazuje na pocetak
   
    printf("\n------------------- REZULTATI: -------------------\n");
    while(crnt != NULL){ //Ovo ide sve dok postoji odnosno dok nije NULL
        printf("%23s - %02d\n", crnt->data.ime, crnt->data.broj_pobeda);
        crnt = crnt->next; //ovo je kao i++ samo sto CURRENT prelazi na sledeci
    }
    printf("--------------------------------------------------\n");
}
 
//Provera da li postoji vec igrac u listi
int daLiVecPostojiIgrac(NODE *head, IGRAC data){
    NODE *crnt = head; //CURRENT da pokazuje na pocetak
    while(crnt != NULL){
        if(strcmp(crnt->data.ime, data.ime) == 0){ //String COMPARE
            return 1;
        }
       
        crnt = crnt->next;
    }
   
    return 0;
}
 
void dodajPobedu(NODE *head, IGRAC data){
    NODE *crnt = head;
    while(crnt != NULL){
        if(strcmp(crnt->data.ime, data.ime) == 0){
            crnt->data.broj_pobeda += 1; //Dodaje jednu pobedu na datog igraca
        }
       
        crnt = crnt->next;
    }
}
 
//Dodavanje elemenata u niz ali uvek na kraj se dodaje novi
void dodajNaKraj(NODE **head, NODE **tail, IGRAC data){
    NODE *node = (struct node*)malloc(sizeof(NODE)); //Pravljenje novog node-a
    node->data = data; //podaci za node
    node->next = NULL; //posto je taj node poslednji sad nema sledeci element iza njega i zato null
   
    if(daLiVecPostojiIgrac(*head, data) == 0){
        if(*head == NULL){ //ako je head null znaci da nema ni jedan element i zato je pocetak i kraj isti
            *head = node;
            *tail = node;
        } else { //ako postoji barem jedan element
            (*tail)->next = node; //uzme poslednji element i upise da je adresa sledeces adresa tog novod noda
            *tail = node; //sada je na tom poslednjem nodu jer ovo iznad je kao i++ odnosno vrati i ali posle ga poveca i zato si onda na tom novom poslednjem i tu upisujes ceo novi node
        }
    } else {
        printf("Igrac vec postoji u listi sto znaci da ce se igrati sa njim!\n"); //ako postoji igrac ne treba da ga dodajes
    }
}
 
//Proveravanje da li na tabli postoji pobednik
char daLiPostojiPobednik(char tabla[3][3]) {
    int i;
   
    //horizontale
    for(i = 0; i < 3; i++){
        if(tabla[i][0] == tabla[i][1] && tabla[i][1] == tabla[i][2]){
            return tabla[i][0];
        }
    }
   
    //vertikale
    for(i = 0; i < 3; i++){
        if(tabla[0][i] == tabla[1][i] && tabla[1][i] == tabla[2][i]){
            return tabla[0][i];
        }
    }
   
    //dijagonale
    if(tabla[0][0] == tabla[1][1] && tabla[1][1] == tabla[2][2]){
        return tabla[0][0];
    } else if (tabla[0][2] == tabla[1][1] && tabla[1][1] == tabla[2][0]){
        return tabla[0][2];
    }
   
    return ' ';
}
 
//Provera da li postoji na tabli vec x/o da se ne bi upisivalo preko x - o ili obratno
int daLiPostojiNaTabli(char tabla[3][3], int x, int y){
    if(tabla[x][y] == 'x' || tabla[x][y] == 'o'){
        return 1;
    }
   
    return 0;
}
// Prikazuje Tabelu 
void prikazTable(char tabla[3][3]){
    int i, j;
   
    printf("   | a | b | c |\n");
    printf("---+---+---+---+---\n");
    for(i = 0; i < 3; i++){
        printf(" %d |", i + 1);
        for(j = 0; j < 3; j++){
            printf("%2c |", tabla[i][j]); //%2c znaci da ima 2 karaktera ali posto mi imamo samo jedan (X/O) onda stavi spejs ispred
        }
        printf("\n---+---+---+---+---\n");
    }
    printf("   |   |   |   |\n");
}
 //Sortiranje Niza
void sortiranjeNiza(NODE **head){
    IGRAC tmp;
    NODE *t1 = *head;
    NODE *t2 = t1;
 
    while(t1 != NULL){
        t2 = t1;
        while(t2 != NULL){
            if(t2->data.broj_pobeda > t1->data.broj_pobeda){
                tmp = t1->data;
                t1->data = t2->data;
                t2->data = tmp;
            }
           
            t2 = t2->next;
        }
   
        t1 = t1->next;
    }
}
 
void upisUDatoteku(NODE *head, NODE *tail){
    //Prvo sortitamo Linked List-u
    sortiranjeNiza(&head);
   
    FILE *dat;
    dat = fopen("rezultati.bin", "wb");
    if (dat == NULL){
        printf("\nNije moguce kreirati novi direktorijum!");
    } else {
        NODE *crnt = head;
        while(crnt != NULL){ //Ovo ide sve dok postoji odnosno dok nije NULL
            fwrite(&crnt->data, sizeof(IGRAC), 1, dat);
            crnt = crnt->next;
        }
        fclose(dat);
    }
}
 
//Ovo sluzi da doda sve igrace iz one datoteke u listu da imas kao istoriju
void dodajIgraceIzListe(NODE **head, NODE **tail, IGRAC data, int broj_pobeda){
    NODE *node = malloc(sizeof(NODE));
    node->data = data;
    node->next = NULL;
   
    if(daLiVecPostojiIgrac(*head, data) == 0){
        if(*head == NULL){
            *head = node;
            *tail = node;
        } else {
            (*tail)->next = node;
            *tail = node;
        }
    } else { //ako postoji igrac dodjes do njega i upises da je broj pobeda onaj iz fajla (NE KORISTI SE OVO TRENUTNO ALI JE RADILO KADA SAM IMO TEST PODATKE)
        NODE *crnt = *head;
        while(crnt != NULL){
            if(strcmp(crnt->data.ime, data.ime) == 0){
                crnt->data.broj_pobeda = broj_pobeda;
            }
           
            crnt = crnt->next;
        }
    }
}
 
//Prikazivanje istorije odnosno rezultata iz direktorijuma
void prikazIzDatoteke(NODE *head, NODE *tail){
    IGRAC p;
    FILE *dat;
    dat = fopen("rezultati.bin", "rb");
    if (dat == NULL){
        printf("\nNe postoji direktorijum ili postoji neka greska pri otvaranju!");
    } else {
        printf("\n------------------- REZULTATI: -------------------\n");
        while(fread(&p, sizeof(IGRAC), 1, dat)){
            printf("%23s - %02d\n", p.ime, p.broj_pobeda);
        }
        printf("--------------------------------------------------\n");
        fclose(dat);
    }
}
 
//Ucitavanje rezultata u listu cita svaki objekat IGRAC i salje dodajIgraceIzListe da ga doda na kraj
void ucitavanjeRezultata(NODE **head, NODE **tail){
    IGRAC p;
    FILE *dat;
    dat = fopen("rezultati.bin", "rb");
    if (dat == NULL){
        printf("\nNe postoji direktorijum ili postoji neka greska pri otvaranju!");
    } else {
        while(fread(&p, sizeof(IGRAC), 1, dat)){
            dodajIgraceIzListe(head, tail, p, p.broj_pobeda);
        }
       
        printf("\n>>> Uspesno su ucitani prethodni rezultati u niz!\n\n");
        fclose(dat);
    }
}
 
void postaviNaPoslednjeSlobodno(char tabla[3][3]){
    int i, j;
   
    //Ako je 8. potez i nema pobednika odma dodaj x na poslednju praznu poziciju jer nema gde drugo da se stavi
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            if(tabla[i][j] == ' '){
                tabla[i][j] = 'x';
            }
        }
    }
}
 
//Logika za igricu
void pokreniNovuIgru(NODE **head, NODE **tail){
    IGRAC p1;
    IGRAC p2;
    char tabla[3][3] = {{' ',' ',' '}, {' ',' ',' '}, {' ',' ',' '}}; //Tabla sa praznim poljima
   
    //Unos podataka za Igraca 1
    printf("\nUnesite ime za 1. igraca (X igrac): ");
    scanf("%s", p1.ime);
    p1.broj_pobeda = 0;
    dodajNaKraj(head, tail, p1); //Dodaje na kraj ako ne postoji
   
    //Unos podataka za Igraca 2
    printf("Unesite ime za 2. igraca (O igrac): ");
    scanf("%s", p2.ime);
    p2.broj_pobeda = 0;
    dodajNaKraj(head, tail, p2); //Dodaje na kraj ako ne postoji
   
    printf("\n\n>>> Pritisnite enter da pocnete sa igrom...");
    getchar();
    getchar();
 
    //Obrisi unos korisnika / Ocisti konzolu
    system("CLS");
   
    int potez;
    for(potez = 0; potez < 8; potez++){
        char z;
        int x, y;
       
        if (potez%2 == 0){
            prikazTable(tabla);
            printf("\n(Trenutni potez: %d) Na redu je: %s (X)\n", potez+1, p1.ime);
            printf("Unesite polje na koje zelite da stavite X [FORMAT: a1, b2]: ");
            scanf("%c%d", &z, &x);
            getchar();
            y = z - 'a'; //z je char a ovo -a jer je to pocetak ce da stavi da je y iz niza {0,1,2} -> 0 kad je a, 1 kad je b
            x--; //posto unosimo 1/2/3 a nama treba 0/1/2
           
            //Ako je uneto npr. A4, D1, 00 itd. (GRESKA U FORMATU POLJA)
            if (x < 0 || x > 2 || y < 0 || y > 2){
                do {
                    printf("\nNista uneli u dobrom formatu polje!\n");
                    printf("Unesite polje na koje zelite da stavite X [FORMAT: a1, b2]: ");
                    scanf("%c%d", &z, &x);
                    getchar();
                    y = z - 'a';
                    x--;
                } while(x < 0 || x > 2 || y < 0 || y > 2);
            }
           
            //Ako vec postoji nesto na tom mestu
            if(daLiPostojiNaTabli(tabla, x, y) == 1){
                do {
                    printf("\nNa toj poziciji se vec nalazi neki element!\n");
                    printf("Unesite polje na koje zelite da stavite X [FORMAT: a1, b2]: ");
                    scanf("%c%d", &z, &x);
                    getchar();
                    y = z - 'a';
                    x--;   
                } while((z < 'a' || z > 'c' || y < 0 || y > 2) || daLiPostojiNaTabli(tabla, x, y) == 1);
                tabla[x][y] = 'x';
            } else if (daLiPostojiNaTabli(tabla, x, y) == 0){
                tabla[x][y] = 'x';
            }
        } else {
            prikazTable(tabla);
            printf("\n(Trenutni potez: %d) Na redu je: %s (O)\n", potez+1, p2.ime);
            printf("Unesite polje na koje zelite da stavite O [FORMAT: a1, b2]: ");
            scanf("%c%d", &z, &x);
            getchar();
            y = z - 'a';
            x--;
           
            //Ako je uneto npr. A4, D1, 00 itd. (GRESKA U FORMATU POLJA)
            if (x < 0 || x > 2 || y < 0 || y > 2){
                do {
                    printf("\nNista uneli u dobrom formatu polje!\n");
                    printf("Unesite polje na koje zelite da stavite X [FORMAT: a1, b2]: ");
                    scanf("%c%d", &z, &x);
                    getchar();
                    y = z - 'a';
                    x--;
                } while(x < 0 || x > 2 || y < 0 || y > 2);
            }
           
            //Ako vec postoji nesto na tom mestu
            if(daLiPostojiNaTabli(tabla, x, y) == 1){
                y = 0, x = 0;
                do {
                    printf("\nNa toj poziciji se vec nalazi neki element!\n");
                    printf("Unesite polje na koje zelite da stavite O [FORMAT: a1, b2]: ");
                    scanf("%c%d", &z, &x);
                    getchar();
                    y = z - 'a';   
                    x--;
                } while((z < 'a' || z > 'c' || y < 0 || y > 2) || daLiPostojiNaTabli(tabla, x, y) == 1);
                tabla[x][y] = 'o';
            } else {
                tabla[x][y] = 'o';
            }
        }
       
        system("CLS");
        char pobednik = daLiPostojiPobednik(tabla);
        if (pobednik == 'x'){
            prikazTable(tabla);
            printf("\n\nPOBEDNIK JE %s ODNOSNO X\n\n", p1.ime);
            dodajPobedu(*head, p1);
            sortiranjeNiza(head);
            prikaziIgrace(*head);
            upisUDatoteku(*head, *tail);
            printf("\n\nPritisnite bilo sta da nastavite...");
            break;
        } else if (pobednik == 'o') {
            prikazTable(tabla);
            printf("\n\nPOBEDNIK JE %s ODNOSNO O\n\n", p2.ime);
            dodajPobedu(*head, p2);
            sortiranjeNiza(head);
            prikaziIgrace(*head);
            upisUDatoteku(*head, *tail);
            printf("\n\nPritisnite bilo sta da nastavite...");
            break;
        }
       
        //Automatski postavi x na posledje slobodno u 8. potezu
        if(potez == 7){
            postaviNaPoslednjeSlobodno(tabla);
            char pobednik = daLiPostojiPobednik(tabla);
            if (pobednik == 'x'){
                prikazTable(tabla);
                printf("\n\nPOBEDNIK JE %s ODNOSNO X\n\n", p1.ime);
                dodajPobedu(*head, p1);
                sortiranjeNiza(head);
                prikaziIgrace(*head);
                upisUDatoteku(*head, *tail);
                printf("\n\nPritisnite bilo sta da nastavite...");
                break;
            } else if (pobednik == 'o') {
                prikazTable(tabla);
                printf("\n\nPOBEDNIK JE %s ODNOSNO O\n\n", p2.ime);
                dodajPobedu(*head, p2);
                sortiranjeNiza(head);
                prikaziIgrace(*head);
                upisUDatoteku(*head, *tail);
                printf("\n\nPritisnite bilo sta da nastavite...");
                break;
            } else {
                prikazTable(tabla);
                printf("\n\nNERESENO!\n\n");
                prikaziIgrace(*head);
                upisUDatoteku(*head, *tail);
                printf("\n\nPritisnite bilo sta da nastavite...");
            }
        }
    }
}
//Sporedni meni koji sluzi za prikaz podataka
void sporedniMeni(NODE **head, NODE **tail){
    int opt;
    do {
        printf("----------------- SPOREDNI MENI: -----------------\n\
1) Prikaz rezultata iz direktorijuma\n\
2) Prikaz trenutnih rezultata iz niza\n\
3) Ucitati sve rezultati iz direktorijuma u listu\n\
4) Obrisati istoriju iz direktorijuma\n\
5) Sortiranje rezultata\n\
6) Izlaz iz sporednog menija\n\
Vas izbor (1-6): ");
 
        scanf("%d", &opt);
        switch(opt){
            case 1:
                getchar();
                prikazIzDatoteke(*head, *tail);
                printf("\nPritisnite enter da obrisete konzolu...");
                getchar();
                system("CLS");
                break;
            case 2:
                getchar();
                prikaziIgrace(*head);
                printf("\nPritisnite enter da obrisete konzolu...");
                getchar();
                system("CLS");
                break;
            case 3:
                getchar();
                ucitavanjeRezultata(head, tail);
                printf("\nPritisnite enter da obrisete konzolu...");
                getchar();
                system("CLS");
                break;
            case 4:
                getchar();
                FILE *dat;
                dat = fopen("rezultati.bin", "wb");
                printf("\n>>> Obrisana sva istorija!\n\n");
                printf("\nPritisnite enter da obrisete konzolu...");
                getchar();
                system("CLS");
                break;
            case 5:
                getchar();
                sortiranjeNiza(head);
                printf("\n>>> Sortiranje je zavrseno!\n\n");
                printf("\nPritisnite enter da obrisete konzolu...");
                getchar();
                system("CLS");
                break;
            case 6:
                system("CLS");
                break;
            default:
                printf("\nMorate uneti vrednost od 1-3!\n\n\n");
                break;
        }
    } while (opt != 6);
}
// Glavni meni programa
void glavniMeni(void) {
    NODE *head = NULL;
    NODE *tail = NULL;
   
    char opt;
    do {
        printf("------------------ GLAVNI MENI: ------------------\n\
a) Pokreni novu igru\n\
b) Prikaz pobeda\n\
c) Izlaz iz programa\n\
Vas izbor (a-c): ");
 
        scanf("%c", &opt);
        switch (opt) {
        case 'a':
            pokreniNovuIgru(&head, &tail);
            getchar();
            system("CLS");
            break;
        case 'b':
            system("CLS");
            sporedniMeni(&head, &tail);
            getchar();
            break;
        case 'c':
            printf("\n\nIZLAZ IZ PROGRAMA...");
            break;
        default:
            getchar();
            printf("\nMorate uneti vrednost od a-c!\n\n\n");
            break;
        }
    } while (opt != 'c');
}
// U glavnoj funkciji poziva se samo Glavni meni preko kojeg se pozivaju i sve ostale funkcije
int main(void){
    glavniMeni();
    return 0;
}
