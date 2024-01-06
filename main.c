#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 50


struct Data{
    /*
     * O structura pt a determina usor ziua,luna si anul unei date calendaristice
     */
    int zi;
    int luna;
    int an;
};
 struct Tranzactie{
     /*
      * Structura principala folosita pt tranzactii
      */
    struct Data data;
    int amount;
    char description[255];
    char type[10];
};
typedef struct Tranzactie Tranzactie;
typedef struct  Data  Data;
struct Cont{
    char password[255];
    int numarTranzactii;
    Tranzactie Lista[MAXSIZE];
};
typedef struct Cont Cont;
void ClearBuffer(){
    /*
     * Aceasta functie curata bufferul de orice input ramas in el, in situatiile cand schimbam metoda de citire de la tastatura
     * (Pt cand programul se schimba de scanf la fgets si vice versa)
     */
    int c;
    while ( (c = getchar()) != '\n' && c != EOF ) { }
}
Data NewData(char newdata[]){
    /*
     * Constructor pt variabile de tip Data
     */
    Data d;
    char tempdata[10];
    strcpy(tempdata,newdata);
    char* splitter,*remaining;
    splitter = strtok(tempdata,"/");
    int i=1;
    while(splitter!=NULL) {
        if (i == 1){
            d.zi = strtol(splitter,&remaining,10);
            i++;
        }else if (i==2){
            d.luna = strtol(splitter,&remaining,10);
            i++;
        }else if(i==3){
            d.an = strtol(splitter,&remaining,10);
        }
        splitter = strtok(NULL,"/");
    }
    return d;
}
Tranzactie NewTranzactie(char newdata[],int newamount,char newdesc[],char newtype[]){
    /*
     * Constructor pt variabile de tip tranzactie
     */
    Tranzactie t;
    t.amount=newamount;
    t.data = NewData(newdata);
    strcpy(t.description,newdesc);
    strcpy(t.type,newtype);
    return t;
}
Cont NewCont(char password[],int nr,Tranzactie L[MAXSIZE]){
    Cont c;
    strcpy(c.password,password);
    c.numarTranzactii = nr;
    for(int i=0;i<nr;i++){
        c.Lista[i]=L[i];
    }
    return c;
}
int AddTranzactie(Tranzactie t,Tranzactie L[],int numberT){
    /*
     * Adauga o noua tranzactie in cont, actualizand numarul de tranzactii din cont, verificand intai daca mai este spatiu
     */
    if(numberT < MAXSIZE) {
        L[numberT] = t;
        numberT++;
    }
    return numberT;
}
int AddCont(Cont c,Cont L[],int numberC){
    if(numberC < MAXSIZE) {
        L[numberC] = c;
        numberC++;
    }
    return numberC;
}
int Balance(Tranzactie L[],int ID){
    /*
     * Calculam totalul din cont uitandu-ne la parametrul type pt a determina daca suma respectiva se adauga sau scade din total
     */
    int Sum=0;
    for(int i=0;i<ID;i++) {
        if (strcmp("Venit",L[i].type)==0) {
            Sum += L[i].amount;

        } else {
            Sum -= L[i].amount;
        }
    }
    return Sum;
}
void ShowTranzactie(Tranzactie t){
    /*
     * Afiseaza datele unei tranzactii
     */
    printf("Data: %d/%d/%d\n",t.data.zi,t.data.luna,t.data.an);
    printf("Suma: %d\n",t.amount);
    printf("Descriere: %s\n",t.description);
    printf("Tip: %s\n",t.type);
    printf("\n");
}
void ShowListaTranzactii(Tranzactie L[],int ID) {
    /*
     * Afiseaza toate tranzactiile curente din cont
     */
    for (int i = 0; i < ID; i++){
        printf("Tranzactie Nr. %d\n", i+1);
        ShowTranzactie(L[i]);
    }
}
void ShowCont(Cont C){
    for(int i=0;i<C.numarTranzactii;i++) {
        printf("Tranzactie Nr. %d\n", i+1);
        ShowTranzactie(C.Lista[i]);
    }
}
void ShowListaConturi(Cont L[],int ID){

    for(int i=0;i<ID;i++) {
        printf("%d\n",L[i].numarTranzactii);
        printf("Cont Nr. %d\n", i+1);
        ShowCont(L[i]);
    }
}
void ShowPeriod(Tranzactie L[],int ID){
    /*
     * Afiseaza toate tranzactiile dintr-o perioada de timp specificata
     */
    Data DataInit,DataEnd;
    char newdata[10];
    printf("Specificati data initiala a perioadei:\n ");
    scanf("%s",newdata);
    DataInit = NewData(newdata);
    printf("Specificati data finala a perioadei:\n ");
    scanf("%s",newdata);
    DataEnd = NewData(newdata);
    int ok=0;
    for(int i=0;i<ID;i++){
        if((L[i].data.an<=DataEnd.an) && (L[i].data.an>=DataInit.an))
              if(((L[i].data.luna<=DataEnd.luna) && (L[i].data.luna>=DataInit.luna)) || (((L[i].data.luna>=DataEnd.luna) && (L[i].data.luna<=DataInit.luna))))
                  if(((L[i].data.zi<=DataEnd.zi) && (L[i].data.zi>=DataInit.zi)) || ((L[i].data.zi>=DataEnd.zi) && (L[i].data.zi<=DataInit.zi)) || ((L[i].data.zi<=DataEnd.zi) && (L[i].data.zi<=DataInit.zi) && (L[i].data.luna!=DataInit.luna)) || ((L[i].data.zi>=DataEnd.zi) && (L[i].data.zi>=DataInit.zi) && (L[i].data.luna!=DataEnd.luna)) ) {
                      ShowTranzactie(L[i]);
                      ok=1;
                  }
    }
    if(ok==0)
        printf("Nu exista tranzactii in acea perioada!\n");
}
void SaveToFile(Tranzactie L[],int ID){
    /*
     * Salveaza tranzactiile curente din cont intr-un fisier text pt a fi reincarcate eventual in alta sesiune
     */
    FILE *f = fopen("Tranzactie.txt","w");
    if(f == NULL){
        printf("Nu s-a putut deschide fisier.");
        exit(1);
    }
    for(int i=0;i<ID;i++) {
        fprintf(f, "Data: %d/%d/%d\n", L[i].data.zi, L[i].data.luna, L[i].data.an);
        fprintf(f, "Suma: %d\n", L[i].amount);
        fprintf(f, "Descriere: %s\n", L[i].description);
        fprintf(f, "Tip: %s\n", L[i].type);
        fprintf(f, "\n");
    }
    printf("Am salvat lista curenta de tranzactii.\n");
    fclose(f);
}
int LoadFromFile(Tranzactie *L){
    /*
     * Incarca o lista de tranzactii salvata intr-un fisier text
     */
    FILE *f  = fopen("Tranzactie.txt","r");
    int tempNumber=0;
    char temp[255],newdata[10],newdesc[255],newtype[10];
    char* splitter,*remaining;
    int i=1,newamount;
    while(!feof(f)){
        fgets(temp,255,f);
        if(i%5==1){
            splitter = strtok(temp," ");
            splitter = strtok(NULL," ");
            strcpy(newdata,splitter);
        }else if(i%5==2){
            splitter = strtok(temp," ");
            splitter = strtok(NULL," ");
            newamount = strtol(splitter,&remaining,10);
        }else if(i%5==3){
            splitter = strtok(temp," ");
            splitter = strtok(NULL," ");
            strcpy(newdesc,splitter);
        }else if(i%5==4){
            splitter = strtok(temp," ");
            splitter = strtok(NULL," ");
            strcpy(newtype,splitter);
        }else{
            tempNumber = AddTranzactie(NewTranzactie(newdata,newamount,newdesc,newtype),L,tempNumber);
        }
        i++;
    }
    fclose(f);
    return tempNumber;
}
int ValidareTranzactie(char newdata[],int newamount,char newdesc[],char newtype[],Tranzactie L[],int ID) {
    /*
     * Verificam datele unei noi tranzactii pt a determina daca acestea sunt valide pt adaugare in lista
     * Ne uitam la data,suma si tip, deoarece descrierea este la discretia utilizatorului
     * Mai si verificam dacaa cumva tranzactia noua adauagata ar fi inaintea celei mai recente tranzactii din lista, in sens cronologic
     */
    newdata[strcspn(newdata,"\n")] = 0;
    newdesc[strcspn(newdesc,"\n")] = 0;
    newtype[strcspn(newtype,"\n")] = 0;
    Data temp = NewData(newdata);
    /*
    if(L[ID-1].data.an>=temp.an)
        if(L[ID-1].data.luna>=temp.luna)
            if(L[ID-1].data.zi>temp.zi)
                return 0;
    */
    if ((temp.zi <= 0 || temp.zi > 31) || (temp.luna <= 0 || temp.luna > 12) || (temp.an <= 0)) {
        return 0;
    }
    if (newamount <= 0) {
        return 0;
    }
    if (strcmp(newtype, "Cheltuiala") != 0){
        if (strcmp(newtype, "Venit") != 0) {
            return 0;
        }
    }
    return 1;
}
int AddListaTranzactii(Cont C,Tranzactie *L){
    int nr;
    char newdata[10];
    int newamount;
    char newdesc[255];
    char newtype[10];
    printf("Introduceti data tranzactiei:\n");
    fgets(newdata,11,stdin);
    ClearBuffer();
    printf("Introduceti suma:\n");
    scanf("%d",&newamount);
    ClearBuffer();
    printf("Introduceti motivul tranzactiei:\n");
    fgets(newdesc,255,stdin);
    printf("Introduceti tipul tranzactiei:\n");
    fgets(newtype,11,stdin);
    if(ValidareTranzactie(newdata,newamount,newdesc,newtype,L,C.numarTranzactii)==0) {
        printf("S-au introdus date invalide!\n");
    }else{
        Tranzactie temp = NewTranzactie(newdata, newamount, newdesc, newtype);
        int currentfirstaccount = AddTranzactie(temp,L,C.numarTranzactii);
        if(currentfirstaccount == C.numarTranzactii)
            printf("Nu s-a putut adauga.\n");
        else
           nr = currentfirstaccount;
    }
    return nr;
}
void Menu(){
    /*
     * Meniul pt interfata contului
     */
    printf("Alegeti optiunea: \n");
    printf("1.Adaugare \n");
    printf("2.Afisare \n");
    printf("3.Calcul Total \n");
    printf("4.Afisare pt o perioada \n");
    printf("5.Salvare tranzactii \n");
    printf("6.Incarcare de tranzactii \n");
    printf("0.Iesire \n");
}
void MenuPrincipal(){
    printf("Alegeti optiunea: \n");
    printf("1.Afisare conturi \n");
    printf("2.Selectare cont \n");
    printf("3.Adaugare cont \n");
    printf("0.Iesire \n");
}

int MenuTranzactii(Cont C,Tranzactie *L){
    int option;
    Menu();
    scanf("%d",&option);
    ClearBuffer();
    while(option!=0){
        if(option==1){
            //Adaugare
            C.numarTranzactii = AddListaTranzactii(C,L);
        }else if(option==2){
            //Afisare
            ShowListaTranzactii(C.Lista,C.numarTranzactii);
        }else if(option==3){
            //Calcul Total
            int total = Balance(C.Lista,C.numarTranzactii);
            printf("Totalul din cont este %d \n",total);
        }else if(option == 4){
            //Afisare tranzactii dintr-o perioada data
            ShowPeriod(C.Lista,C.numarTranzactii);
            ClearBuffer();
        }else if(option == 5){
            //Salvare de tranzactii intr-un fisier txt
            SaveToFile(C.Lista,C.numarTranzactii);
        }else if(option == 6){
            //Incarcare de tranzactii dintr-un fisier txt
            C.numarTranzactii = LoadFromFile(C.Lista);
        }else{
            printf("Optiunea nu exista.\n");
        }
        Menu();
        scanf("%d",&option);
        ClearBuffer();
    }
    return C.numarTranzactii;
}
int main() {
    struct Tranzactie TransLista1[MAXSIZE],TransLista2[MAXSIZE];
    int firsttrans=0,secondtrans=0;
    firsttrans = AddTranzactie(NewTranzactie("5/9/2023",250,"Cumparaturi","Cheltuiala"),TransLista1,firsttrans);
    firsttrans = AddTranzactie(NewTranzactie("28/10/2023",800,"Salariu","Venit"),TransLista1,firsttrans);
    firsttrans = AddTranzactie(NewTranzactie("15/12/2023",700,"Chirie ","Cheltuiala"),TransLista1,firsttrans);
    secondtrans = AddTranzactie(NewTranzactie("17/11/2023",350,"Factura Lumina","Cheltuiala"),TransLista2,secondtrans);
    secondtrans = AddTranzactie(NewTranzactie("23/11/2023",950,"Cadou de la Familie ","Venit"),TransLista2,secondtrans);
    struct Cont Cont1 = NewCont("exemplu\n",firsttrans,TransLista1);
    struct Cont Cont2 = NewCont("argd3\n",secondtrans,TransLista2);
    struct Cont ContList[MAXSIZE];
    int nraccount =0;
    nraccount = AddCont(Cont1,ContList,nraccount);
    nraccount = AddCont(Cont2,ContList,nraccount);
    int option;
    MenuPrincipal();
    scanf("%d",&option);
    ClearBuffer();
    while (option!=0){
        if(option==1){
            ShowListaConturi(ContList,nraccount);
        }else if(option == 2){
            int select;
            printf("Specificati ID-ul contului pe care il doriti \n");
            scanf("%d",&select);
            ClearBuffer();
            if(select > nraccount){
                printf("Acel cont nu exista! \n");
            }else{
                char pass[255];
                printf("Introduceti parola pt contul %d \n",select);
                fgets(pass,255,stdin);
                if(strcmp(ContList[select-1].password,pass)==0){
                    ContList[select-1].numarTranzactii= MenuTranzactii(ContList[select-1],ContList[select-1].Lista);
                }else{
                    printf("Ati introdus parola gresita! \n");
                }
            }
        }else if(option == 3){
            int nrT=0;
            Tranzactie NewContList[MAXSIZE];
            char pass[255];
            printf("Introduceti parola pt contul nou \n");
            fgets(pass,255,stdin);
            nraccount = AddCont(NewCont(pass,nrT,NewContList),ContList,nraccount);
        }
        MenuPrincipal();
        scanf("%d",&option);
        ClearBuffer();
    }
    return 0;
}
