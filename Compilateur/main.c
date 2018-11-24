#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>

#define NB_CLE 15
#define NB_PONC 5
#define NB_OPERL 6
#define NB_OPADD 3
#define NB_OPMUL 4

const char* cle[]={"if","then","else","while","do","read","write","readln","writeln","var","integer","char","program","begin","end"};
const char* ponc[]={"(",")",":=",",",":"};
const char* oprel[]={"==","<>","<=",">=","<",">"};
const char* opadd[]={"||","+","-"};
const char* opmul[]={"&&","/","*","%"};

typedef struct id{
    char id[256];
    int num;
    int* type; //not_declared 0 char 1 integer 2 name_prog 3
    int val;
} id;

typedef struct Symbole{
    char unit[256];
    int num_Id;
    int num_lig;
    int num_col;
    int val;
    char op[2];
} Symbole;

id List_Id[10000];
Symbole List_Sym[10000];
int Size_Sym=0,Size_Id=0,Num_Lig=1,Num_Col=1;

int est_id(char* lex){
    int i;
    if (!isalpha(lex[0])) return 0;
    for(i=1;i<strlen(lex);i++){
       if (!isalnum(lex[i])) return 0;
    }
    return 1;
}

int est_nb(char* lex){
    int i;
    for(i=0;i<strlen(lex);i++){
       if (!isdigit(lex[i])) return 0;
    }
    return 1;
}

int est_unit_Lex(char* lex, const char* T[], int nb){
    int i;
    for(i=0;i<nb;i++){
        if (!strcmp(lex,T[i])){
            return 1;
        }
    }
    return 0;
}

int est_cle(char* lex){est_unit_Lex(lex,cle,NB_CLE);}

int est_ponc(char* lex){est_unit_Lex(lex,ponc,NB_PONC);}

int est_oprel(char* lex){est_unit_Lex(lex,oprel,NB_OPERL);}

int est_opadd(char* lex){est_unit_Lex(lex,opadd,NB_OPADD);}

int est_opmul(char* lex){est_unit_Lex(lex,opmul,NB_OPMUL);}

int est_sep(char sep){
    if (sep==' ' || sep == '\n' ||sep == '.' || sep=='\t' || sep == ';' ){
        return 1;
    }
    else return 0;
}

int trait_sep(char sep){
    switch (sep){
        case ' ' : return 1;
        case '\t' : return 1;
        case '.' :{
            ajouter_Sym(".");
            return 1;
        }
        case '\n':{
            Num_Lig++;
            Num_Col=1;
            return 1;
        }
        case ';':{
            ajouter_Sym(";");
            return 1;
        }
        default :{
            return 0;
        }
    }
}

int trouver_Id(id List_Id[1000],char* lex,int Size_Id){
    int i;
    for(i=0;i<=Size_Id;i++){
        if(!strcmp(List_Id[i].id,lex)){
           return List_Id[i].num;
        }
    }
    return 0;
}

void ajouter_Sym(char* lex){
    strcpy(List_Sym[Size_Sym].unit,lex);
    List_Sym[Size_Sym].num_lig=Num_Lig;
    List_Sym[Size_Sym].num_col=Num_Col;
    Size_Sym++;
    Num_Col++;
}

void remplir_table_alpha_Sym(char* lex ,char sep){
    if (!strcmp(lex,""));

    else if (est_cle(lex)&& est_sep(sep)){
        ajouter_Sym(lex);
    }

    else if (est_cle(lex)&& !est_sep(sep)){
        printf("ERREUR : MOT CLE DOIVENT ETRE ENTOURES PAR DES BLANCS, DES FIN DE LIGNES, LE DEBUT DU PROGRAMME OU LE POINT FINAL. LIGNE %d. ORDRE %d.\n",Num_Lig,Num_Col);
        ajouter_Sym("ERREUR");
    }

    else if(est_nb(lex)){
        ajouter_Sym("nb");
        List_Sym[Size_Sym-1].val=atoi(lex);
    }

    else if (est_id(lex)){
        int Pos;
        Pos=trouver_Id(List_Id, lex, Size_Id);
        if(Pos){
            List_Sym[Size_Sym].num_Id=Pos;
            ajouter_Sym("id");
        }
        else{
            strcpy(List_Id[Size_Id].id,lex);
            List_Id[Size_Id].num=Size_Id+1;
            List_Id[Size_Id].type=malloc(sizeof(int));
            *(List_Id[Size_Id].type)=0;
            List_Sym[Size_Sym].num_Id=Size_Id+1;
            ajouter_Sym("id");
            Size_Id++;
        }
    }

    else {
        printf("ERREUR : ID INVALIDE. LIGNE %d. ORDRE %d.\n",Num_Lig,Num_Col);
        ajouter_Sym("ERREUR");
    }
}

int remplir_table_nonalpha_Sym(char* lex){
    if(est_ponc(lex)){
        ajouter_Sym(lex);
        strcpy(List_Sym[Size_Sym-1].op,lex);
        return 1;
    }

    else if(est_opadd(lex)){
        ajouter_Sym("opadd");
        strcpy(List_Sym[Size_Sym-1].op,lex);
        return 1;
    }

    else if(est_oprel(lex)){
        ajouter_Sym("oprel");
        strcpy(List_Sym[Size_Sym-1].op,lex);
        return 1;
    }

    else if(est_opmul(lex)){
        ajouter_Sym("opmul");
        strcpy(List_Sym[Size_Sym-1].op,lex);
        return 1;
    }

    return 0;
}

void afficher_table_Id(id List_Id[1000],int Size_Id){
    int i;
    printf("\nTable Des Ids:\n ------------------\n");
    for(i=0;i<Size_Id;i++){
        printf(" | %3d | %8s |\n ------------------ \n",List_Id[i].num,List_Id[i].id);
    }
    printf("\n");
}

void afficher_table_Sym(Symbole List_Sym[1000],int Size_Sym){
    int i;
    printf("\nTable Des Symboles:\n --------------------------------\n");
    printf(" |%4s | %10s |%5s|%5s|\n --------------------------------\n","N_id","Symboles ","N_lig","N_Col");
    for(i=0;i<Size_Sym;i++){
        printf(" | %3d | %10s | %3d | %3d |\n --------------------------------\n",List_Sym[i].num_Id,List_Sym[i].unit,List_Sym[i].num_lig,List_Sym[i].num_col);
    }
    printf("\n");
}

void message_invalide_fin_commentaire(){
    printf("ERREUR: *) MANQUANTE. LIGNE %d.\n",Num_Lig);
};

//L'analyse lexical.
void analyseLexicale(char* fileName){
    FILE *fp;
    fp = freopen(fileName, "r", stdin);
    char c;
    char lex[256];
    int size_lex=0;
    while(scanf("%c",&c)!= EOF){
        if (!isalnum(c)){
            lex[size_lex]='\0';
            size_lex=0;
            remplir_table_alpha_Sym(lex,c);
            if ( !trait_sep(c) ){
                char lex_int[2];
                lex_int[0]=c;
                Ret :
                if (scanf("%c",&lex_int[1])!= EOF){
                    if (est_sep(lex_int[1])){
                        char sep =lex_int[1];
                        lex_int[1]='\0';
                        if (!remplir_table_nonalpha_Sym(lex_int)){
                            ajouter_Sym("ERREUR");
                            printf("ERREUR: SYMBOLE INVALIDE. LIGNE %d. ORDRE %d.\n",Num_Lig,Size_Sym);
                        }
                        trait_sep(sep);
                    }
                    else{
                        lex_int[2]='\0';
                        if (!strcmp(lex_int,"(*")){
                            while(1){
                                if (scanf("%c",&c)!=EOF){
                                    if (c=='\n'){
                                        Num_Lig++;
                                        Num_Col=1;
                                    }
                                    else if (c=='*'){
                                        if (scanf("%c",&c)!=EOF){
                                            if (c==')'){
                                                break;
                                            }
                                        }
                                        else {
                                            message_invalide_fin_commentaire();
                                            break;
                                        }
                                    }
                                }
                                else {
                                    message_invalide_fin_commentaire();
                                    break;
                                }
                            }
                        }
                        else if (!remplir_table_nonalpha_Sym(lex_int)){
                            c=lex_int[1];
                            lex_int[1]='\0';
                            if (!remplir_table_nonalpha_Sym(lex_int)){
                                ajouter_Sym("ERREUR");
                                printf("ERREUR: SYMBOLE INVALIDE. LIGNE %d. ORDRE %d.\n",Num_Lig,Num_Col);
                            }
                            if (isalnum(c)){
                                lex[size_lex]=c;
                                size_lex++;
                                lex_int[0]='\0';
                            }
                            else {
                                lex_int[0]=c;
                                goto Ret;
                            }
                        }
                    }
                }
                else {
                    lex_int[1]='\0';
                    if (!remplir_table_nonalpha_Sym(lex_int)){
                        ajouter_Sym("ERREUR");
                        printf("ERREUR: SYMBOLE INVALIDE. LIGNE %d. ORDRE %d.\n",Num_Lig,Num_Col);
                    }
                }
            }
        }
        else {
            lex[size_lex]=c;
            size_lex++;
        }
    }

    lex[size_lex]='\0';
    remplir_table_alpha_Sym(lex,' ');

    //afficher_table_Id(List_Id,Size_Id);
    //afficher_table_Sym(List_Sym,Size_Sym);
    fclose(fp);
}

int TailleInstruction[] = {7,7,2,3,4,2,2,2,4,2,2,3,4,2,4,7,5,5,5,5,5,3,3,2,3,4,2,3,4,2,2,2,4};

#define tNT 17
#define tT 27
#define tInstr 33
#define octet 25
#define tMaxInst 8

const char* nonTerminaux[]={"P","Dcl","Inst_composee","Liste_id","Liste_in","Type","Inst","Liste_inst","Liste_I","I","Exp_simple","Exp","Exp_I","Terme","Exp_S","Facteur","Terme_I"};
const char* terminaux[]={"id","nb","oprel","opadd","opmul","if","then","else","while","do",";",",",":","read","write","readln","writeln","var","integer","char","program","begin","end",".","(",")",":=","$"};

const char* Inst[tInstr][tMaxInst] ={{"P","program","id",";","Dcl","Inst_composee","."},
{"Dcl","var","Liste_id",":","Type",";","Dcl"},
{"Dcl","epsilon"},
{"Liste_id","id","Liste_in"},
{"Liste_in",",","id","Liste_in"},
{"Liste_in","epsilon"},
{"Type","integer"},
{"Type","char"},
{"Inst_composee","begin","Inst","end"},
{"Inst","Liste_inst"},
{"Inst","epsilon"},
{"Liste_inst","I","Liste_I"},
{"Liste_I",";","I","Liste_I"},
{"Liste_I","epsilon"},
{"I","id",":=","Exp_simple"},
{"I","if","Exp","then","I","else","I"},
{"I","while","Exp","do","I"},
{"I","read","(","id",")"},
{"I","write","(","id",")"},
{"I","readln","(","id",")"},
{"I","writeln","(","id",")"},
{"Exp","Exp_simple","Exp_I"},
{"Exp_I","oprel","Exp_simple"},
{"Exp_I","epsilon"},
{"Exp_simple","Terme","Exp_S"},
{"Exp_S","opadd","Terme","Exp_S"},
{"Exp_S","epsilon"},
{"Terme","Facteur","Terme_I"},
{"Terme_I","opmul","Facteur","Terme_I"},
{"Terme_I","epsilon"},
{"Facteur","id"},
{"Facteur","nb"},
{"Facteur","(","Exp_simple",")"}};

char tablePremier[tNT][30][octet];
int index_tablePremier[tNT];

char tableSuivant[tNT][30][octet];
int index_tableSuivant[tNT];

int Est_Terminal(char* mot){
    int i;
    for(i=0;i<tT;i++){
        if(!strcmp(mot,terminaux[i])){
            return 1;
        }
    }
    return 0;
}

int Est_NonTerminal(char* mot){
    int i;
    for(i=0;i<tNT;i++){
        if(!strcmp(mot,nonTerminaux[i])){
            return 1;
        }
    }
    return 0;
}

int posNonTerminal(char* mot){
    int i;
    for(i=0;i<tNT;i++){
        if(!strcmp(mot,nonTerminaux[i])){
            return i;
        }
    }
    return -1;
}

int posTerminal(char* mot){
    int i;
    for(i=0;i<tT;i++){
        if(!strcmp(mot,terminaux[i])){
            return i;
        }
    }
    return -1;
}

int dejaExist(char* nonTerm,char* term){
    int i;
    int j=posNonTerminal(nonTerm);
    for(i=0;i<index_tablePremier[j];i++){
        if(!strcmp(term,tablePremier[j][i])){
            return 1;
        }
    }
    return 0;
}

int dejaExistS(char* nonTerm,char* term){
    int i;
    int j=posNonTerminal(nonTerm);
    for(i=0;i<index_tableSuivant[j];i++){
        if(!strcmp(term,tableSuivant[j][i])){
            return 1;
        }
    }
    return 0;
}

int visiteNonTerminal[tNT];

void premier(char* mot){
    int j;
    int i= posNonTerminal(mot);
    if (visiteNonTerminal[i]==1) return;
    else visiteNonTerminal[i]=1;
    for(j=0;j<tInstr;j++){
        if (!strcmp(Inst[j][0],mot)){
            if (!strcmp(Inst[j][1],"epsilon")){
                if (!dejaExist(mot,"epsilon")){
                    strcpy(tablePremier[i][index_tablePremier[i]],"epsilon");
                    index_tablePremier[i]++;
                }
            }
            else if (Est_Terminal(Inst[j][1])){
                if (!dejaExist(mot,Inst[j][1])){
                    strcpy(tablePremier[i][index_tablePremier[i]],Inst[j][1]);
                    index_tablePremier[i]++;
                }
            }
            else if (Est_NonTerminal(Inst[j][1])){
                int k=1,compteur;
                REFAIRE :
                if (k<TailleInstruction[j]) premier(Inst[j][k]);
                int pos = posNonTerminal(Inst[j][k]);

                for(compteur=0;compteur<index_tablePremier[pos];compteur++){
                    if (strcmp(tablePremier[pos][compteur],"epsilon")){
                        if (!dejaExist(mot,tablePremier[pos][compteur])){
                            strcpy(tablePremier[i][index_tablePremier[i]],tablePremier[pos][compteur]);
                            index_tablePremier[i]++;
                        }
                    }
                    else {
                        k++;
                        if (k>=TailleInstruction[j]){
                            if (!dejaExist(mot,"epsilon")){
                                strcpy(tablePremier[i][index_tablePremier[i]],"epsilon");
                                index_tablePremier[i]++;
                            }
                        }
                        else{
                            goto REFAIRE;
                        }
                    }
                }
            }
        }
    }
}

int posNonTerDansInst(char* mot,int j,int k){
    int i;

    for(i=k;i<TailleInstruction[j];i++){
        if (!strcmp(mot,Inst[j][i])){
            return i;
        }
    }
    return -1;
}

void ReintivisiteNonTerminal(){
    int i;
    for(i=0;i<tNT;i++){
        visiteNonTerminal[i]=0;
    }
}

void suivant(char* mot){
    int j;
    int i= posNonTerminal(mot);
    if (visiteNonTerminal[i]==1) return;
    else visiteNonTerminal[i]=1;

    for(j=0;j<tInstr;j++){
        int k=1;
        while(k<TailleInstruction[j]){
            int posNTDI = posNonTerDansInst(mot,j,k);
            k=posNTDI+1;
            if (posNTDI!=-1){
                if (posNTDI<TailleInstruction[j]-1){
                    char proch[255];
                    strcpy(proch,Inst[j][posNTDI+1]);
                    if (Est_Terminal(proch)){
                        if (!dejaExistS(mot,proch)){
                            strcpy(tableSuivant[i][index_tableSuivant[i]],proch);
                            index_tableSuivant[i]++;
                        }
                    }
                    else {
                        int compteur;
                        for(compteur=0;compteur<index_tablePremier[posNonTerminal(proch)];compteur++){
                            if(strcmp(tablePremier[posNonTerminal(proch)][compteur],"epsilon")){
                                if (!dejaExistS(mot,tablePremier[posNonTerminal(proch)][compteur])){
                                    strcpy(tableSuivant[i][index_tableSuivant[i]],tablePremier[posNonTerminal(proch)][compteur]);
                                    index_tableSuivant[i]++;
                                }
                            }
                            else {
                                if (!visiteNonTerminal[posNonTerminal(Inst[j][0])]) suivant(Inst[j][0]);
                                int compteurI;
                                for(compteurI=0;compteurI<index_tableSuivant[posNonTerminal(Inst[j][0])];compteurI++){
                                    if (!dejaExistS(mot,tableSuivant[posNonTerminal(Inst[j][0])][compteurI])){
                                        strcpy(tableSuivant[i][index_tableSuivant[i]],tableSuivant[posNonTerminal(Inst[j][0])][compteurI]);
                                        index_tableSuivant[i]++;
                                    }
                                }
                            }
                        }
                    }
                }
                else if (posNTDI==TailleInstruction[j]-1){
                    if (!visiteNonTerminal[posNonTerminal(Inst[j][0])]) suivant(Inst[j][0]);
                    int compteur;
                    for(compteur=0;compteur<index_tableSuivant[posNonTerminal(Inst[j][0])];compteur++){
                        if (strcmp(mot,Inst[j][0])){
                            if (!dejaExistS(mot,tableSuivant[posNonTerminal(Inst[j][0])][compteur])){
                                strcpy(tableSuivant[i][index_tableSuivant[i]],tableSuivant[posNonTerminal(Inst[j][0])][compteur]);
                                index_tableSuivant[i]++;
                            }
                        }
                    }
                }
            }
            else {
                break;
            }
        }
    }
}

void affichagePremier(){

    printf("TABLE DES PREMIERS: \n-----------------------------------------------------------------------------\n");
    int i,j;
    for(i=0;i<tNT;i++){
        printf("|%15s| ", nonTerminaux[i]);
        char S[60]="";
        for(j=0;j<index_tablePremier[i];j++){
            strcat(S,tablePremier[i][j]);
            strcat(S,"| ");
        }
        printf("%60s",S);
        printf("\n-----------------------------------------------------------------------------\n");
    }
}

void affichageSuivant(){
    printf("TABLE DES SUIVANTS: \n-----------------------------------------------------------------------------\n");
    int i,j;
    for(i=0;i<tNT;i++){
        printf("|%15s| ", nonTerminaux[i]);
        char S[60]="";
        for(j=0;j<index_tableSuivant[i];j++){
            strcat(S,tableSuivant[i][j]);
            strcat(S,"| ");
        }
        printf("%60s",S);
        printf("\n-----------------------------------------------------------------------------\n");
    }
}

char TableM[tNT][tT+1][tMaxInst][octet];
int index_TableM[tNT][tT+1];

int appartientPremier(char* nterm,char* term){
    int i;
    int j=posNonTerminal(nterm);
    for(i=0;i<index_tablePremier[j];i++){
        if(!strcmp(tablePremier[j][i],term)){
            return 1;
        }
    }
    return 0;
}

void RemplirTableM(){
    int i,j,k,l,posNT,posT;
    for(i=0;i<tInstr;i++){
        posNT=posNonTerminal(Inst[i][0]);
        for(j=0;j<index_tablePremier[posNT];j++){
            char motpremier[octet];
            strcpy(motpremier,tablePremier[posNT][j]);
            if (strcmp(motpremier,"epsilon")){
                posT=posTerminal(motpremier);
                if((Est_Terminal(Inst[i][1])&&!strcmp(motpremier,Inst[i][1]))||(Est_NonTerminal(Inst[i][1])&&appartientPremier(Inst[i][1],motpremier))){
                    for(k=0;k<TailleInstruction[i];k++){
                        strcpy(TableM[posNT][posT][index_TableM[posNT][posT]],Inst[i][k]);
                        index_TableM[posNT][posT]++;
                    }
                }
            }
            else {
                for(l=0;l<index_tableSuivant[posNT];l++){
                    posT=posTerminal(tableSuivant[posNT][l]);
                    if (!strcmp(Inst[i][1],"epsilon")){
                        for(k=0;k<TailleInstruction[i];k++){
                            strcpy(TableM[posNT][posT][index_TableM[posNT][posT]],Inst[i][k]);
                            index_TableM[posNT][posT]++;
                        }
                    }
                }
            }
        }
    }
}

//Pour Tester
/*void affichierTableM2(){
    int i,j,k,l,m;
    for(i=0;i<tNT;i++){
        printf("%s \t",nonTerminaux[i]);
        j=2;
            for(k=0;k<index_TableM[i][j];k++){
                printf("%s ",TableM[i][j][k]);
            }

        printf("\n");
    }
}*/

#define lig "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------"


void affichierTableM(){
    int i,j,k,l=0;
    char* ligne[500];
    strcpy(ligne,lig);
    while(l<tT+1){
        printf("%s \n",ligne);
        printf("|%13s|"," ");
        for(j=l;j<l+4&&j<tT+1;j++){
            printf("%37s|",terminaux[j]);
        }
        printf("\n");
        printf("%s \n",ligne);
        for(i=0;i<tNT;i++){
            printf("|%13s|",nonTerminaux[i]);
            for(j=l;j<l+4&&j<tT+1;j++){
                if(index_TableM[i][j]!=0){
                    char S[40]="";
                    strcat(S,TableM[i][j][0]);
                    strcat(S," ->");
                    for(k=1;k<index_TableM[i][j];k++){
                        strcat(S," ");
                        strcat(S,TableM[i][j][k]);
                    }
                    printf("%37s|",S);
                }
                else printf("%37s|","");
            }
            printf("\n");
            printf("%s \n",ligne);
        }
        printf("\n\n\n");
        l+=4;
    }
}

char pile[10000][14];
int pointsommet=1;
int indexParcoursTableSymbole=0;
char arbreSyntaxique[10000][tMaxInst][14];
int indexColoneArbre[10000];
int indexArbreSyntaxique=0;

void afficher_pile(){
    int i;
    char S[50];
    for(i=pointsommet;i>=0;i--){
        printf("%s ",pile[i]);
    }
    printf("\n");
}

int VerifierAnalyseSyntaxique=1;

void analyseSyntaxique(){
    strcpy(pile[0],"$");
    strcpy(pile[1],"P");
    strcpy(List_Sym[Size_Sym].unit,"$");
    Size_Sym++;
    //afficher_table_Sym(List_Sym,Size_Sym);
    int i=0;

    while(pointsommet){
        if(Est_Terminal(pile[pointsommet])){
            if (strcmp(pile[pointsommet],List_Sym[indexParcoursTableSymbole].unit)){
                printf("ERREUR SYMBOLE ATTENDU: %s, SYMBOLE TROUVEE: %s. Ligne %d\n",List_Sym[indexParcoursTableSymbole].unit,pile[pointsommet],List_Sym[indexParcoursTableSymbole].num_lig);
                VerifierAnalyseSyntaxique=0;
                break;
            }
            pointsommet--;
            indexParcoursTableSymbole++;
        }
        else if(Est_NonTerminal(pile[pointsommet])){
            int posNTe,posTe;
            posNTe = posNonTerminal(pile[pointsommet]);
            posTe = posTerminal(List_Sym[indexParcoursTableSymbole].unit);

            if(index_TableM[posNTe][posTe]){
                int j;
                pointsommet--;
                for(j=index_TableM[posNTe][posTe]-1;j>0;j--){
                    if (strcmp(TableM[posNTe][posTe][j],"epsilon")){
                        pointsommet++;
                        strcpy(pile[pointsommet],TableM[posNTe][posTe][j]);
                    }

                }
                indexColoneArbre[indexArbreSyntaxique]=index_TableM[posNTe][posTe];
                for(j=0;j<index_TableM[posNTe][posTe];j++){
                    strcpy(arbreSyntaxique[indexArbreSyntaxique][j],TableM[posNTe][posTe][j]);
                }
                indexArbreSyntaxique++;
            }
            else{
                pointsommet--;
                printf("ERREUR TABLE DE SYMBOLE [%s, %s] EST VIDE. LIGNE: %d\n",List_Sym[indexParcoursTableSymbole].unit,pile[pointsommet],List_Sym[indexParcoursTableSymbole].num_lig);
                VerifierAnalyseSyntaxique=0;
                break;
            }
        }
        i++;
    }
}

void afficherArbreSyntaxique(){
    int i,j;
    printf("\nARBRE SYNTAXIQUE:\n");
    for(i=0;i<indexArbreSyntaxique;i++){
        printf("\t-%s -> ",arbreSyntaxique[i][0]);
        for(j=1;j<indexColoneArbre[i];j++){
            printf("%s ",arbreSyntaxique[i][j]);
        }
        printf("\n");
    }
}

int CompteurTableSyntaxique=0;

int* Liste_id;
int* Type;
id* Exp_simple;
id* Exp;
id* Exp_I;
id* Terme;
id* Exp_S;
int p=0;
int CodeInterI=1;
int tempo=0;

void analyseSemantiqueNonTerm(int j){
    int i;
    if (!strcmp(arbreSyntaxique[j][0],"P")){
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                if(!strcmp(List_Sym[CompteurTableSyntaxique].unit,"id")){
                    int x=trouver_Id(List_Id,List_Sym[CompteurTableSyntaxique].unit,Size_Id);
                    *(List_Id[x].type)=3;
                }
                CompteurTableSyntaxique++;
            }
        }
    }

    else if (!strcmp(arbreSyntaxique[j][0],"Dcl")&&!strcmp(arbreSyntaxique[j][1],"var")){
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                if (!strcmp(arbreSyntaxique[j][i],"Liste_id")){
                    Liste_id=Type;
                }
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                CompteurTableSyntaxique++;
            }
        }
    }

    else if (!strcmp(arbreSyntaxique[j][0],"Liste_id")&&!strcmp(arbreSyntaxique[j][1],"id")){
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                if(!strcmp(List_Sym[CompteurTableSyntaxique].unit,"id")){
                    if(*List_Id[List_Sym[CompteurTableSyntaxique].num_Id-1].type==0){
                        List_Id[List_Sym[CompteurTableSyntaxique].num_Id-1].type=Liste_id;
                    }
                    else{
                        printf("ERREUR: VARIABLE DEJA DECLARE.LIGNE %d ORDRE %d\n",List_Sym[CompteurTableSyntaxique].num_lig,List_Sym[CompteurTableSyntaxique].num_col);
                    }
                }
                CompteurTableSyntaxique++;
            }
        }
    }
    else if ((!strcmp(arbreSyntaxique[j][0],"Liste_in"))&&(!strcmp(arbreSyntaxique[j][1],","))){
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                if(!strcmp(List_Sym[CompteurTableSyntaxique].unit,"id")){
                    if(*List_Id[List_Sym[CompteurTableSyntaxique].num_Id-1].type==0){
                        List_Id[List_Sym[CompteurTableSyntaxique].num_Id-1].type=Liste_id;
                    }
                    else{
                        printf("ERREUR: VARIABLE DEJA DECLARE.LIGNE %d ORDRE %d\n",List_Sym[CompteurTableSyntaxique].num_lig,List_Sym[CompteurTableSyntaxique].num_col);
                    }
                }
                CompteurTableSyntaxique++;
            }
        }
    }
    else if (!strcmp(arbreSyntaxique[j][0],"Type")&&!strcmp(arbreSyntaxique[j][1],"integer")){
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                    *Type=2;
                    Type=malloc(sizeof(int));
                CompteurTableSyntaxique++;
            }
        }
    }
    else if (!strcmp(arbreSyntaxique[j][0],"Type")&&!strcmp(arbreSyntaxique[j][1],"char")){
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                    *Type=1;
                    Type=malloc(sizeof(int));
                CompteurTableSyntaxique++;
            }
        }
    }
    else if (!strcmp(arbreSyntaxique[j][0],"Inst_composee")&&!strcmp(arbreSyntaxique[j][1],"begin")){
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                CompteurTableSyntaxique++;
            }
        }
    }
    else if (!strcmp(arbreSyntaxique[j][0],"Inst")&&!strcmp(arbreSyntaxique[j][1],"Liste_inst")){
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                CompteurTableSyntaxique++;
            }
        }
    }
    else if (!strcmp(arbreSyntaxique[j][0],"Liste_inst")&&!strcmp(arbreSyntaxique[j][1],"I")){
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                CompteurTableSyntaxique++;
            }
        }
    }
    else if (!strcmp(arbreSyntaxique[j][0],"Liste_I")&&!strcmp(arbreSyntaxique[j][1],";")){
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                CompteurTableSyntaxique++;
            }
        }
    }

     else if(!strcmp(arbreSyntaxique[j][0],"I")&&!strcmp(arbreSyntaxique[j][1],"if")){
        FILE* F;
        F = fopen("Code Intermédaire.txt","a");
        fprintf(F,"%d. IF\n",CodeInterI);
        CodeInterI++;
        fclose(F);

        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                if(!strcmp(List_Sym[CompteurTableSyntaxique].unit,"else")){
                    F = fopen("Code Intermédaire.txt","a");
                    fprintf(F,"%d. ELSE\n",CodeInterI);
                    CodeInterI++;
                    fclose(F);
                }
                CompteurTableSyntaxique++;
            }
        }
        F = fopen("Code Intermédaire.txt","a");
        fprintf(F,"%d. END IF\n",CodeInterI);
        CodeInterI++;
        fclose(F);
    }

    else if(!strcmp(arbreSyntaxique[j][0],"I")&&!strcmp(arbreSyntaxique[j][1],"while")){
        FILE* F;
        F = fopen("Code Intermédaire.txt","a");
        fprintf(F,"%d. LOOP\n",CodeInterI);
        CodeInterI++;
        fclose(F);

        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                if (!strcmp(arbreSyntaxique[j][i],"Exp")){
                    F = fopen("Code Intermédaire.txt","a");
                    fprintf(F,"%d. IF NOT\n",CodeInterI);
                    CodeInterI++;
                    fclose(F);
                }
                p++;
                analyseSemantiqueNonTerm(p);
                if (!strcmp(arbreSyntaxique[j][i],"Exp")){
                    F = fopen("Code Intermédaire.txt","a");
                    fprintf(F,"%d. GO END LOOP\n",CodeInterI);
                    CodeInterI++;
                    fclose(F);
                }
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                CompteurTableSyntaxique++;
            }
        }
        F = fopen("Code Intermédaire.txt","a");
        fprintf(F,"%d. RET LOOP\n",CodeInterI);
        CodeInterI++;
        fprintf(F,"%d. END LOOP\n",CodeInterI);
        CodeInterI++;
        fclose(F);
    }

    else if(!strcmp(arbreSyntaxique[j][0],"I")&&strcmp(arbreSyntaxique[j][1],"id")){
        FILE * pFile;
        pFile = fopen ("Code Intermédaire.txt","a");
        fprintf(pFile,"%d. ",CodeInterI);
        CodeInterI++;

        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                if(!strcmp(List_Sym[CompteurTableSyntaxique].unit,"id")){
                    fprintf(pFile,"%s ",List_Id[List_Sym[CompteurTableSyntaxique].num_Id-1].id);
                }
                else {
                    fprintf(pFile,"%s ",List_Sym[CompteurTableSyntaxique].unit);
                }
                CompteurTableSyntaxique++;
            }
        }
        fprintf(pFile,"\n");
        fclose (pFile);
    }

    else if(!strcmp(arbreSyntaxique[j][0],"I")&&!strcmp(arbreSyntaxique[j][1],"id")){
        FILE * pFile;
        pFile = fopen ("Code Intermédaire.txt","a");
        char aux[256];

        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                if(!strcmp(List_Sym[CompteurTableSyntaxique].unit,"id")){
                    if(*List_Id[List_Sym[CompteurTableSyntaxique].num_Id-1].type!=0){
                        Exp_simple=&List_Id[List_Sym[CompteurTableSyntaxique].num_Id-1];
                        sprintf(aux,"%s",List_Id[List_Sym[CompteurTableSyntaxique].num_Id-1].id);
                    }
                    else{
                        printf("ERREUR: VARIABLE NON DECLARE.LIGNE %d ORDRE %d\n",List_Sym[CompteurTableSyntaxique].num_lig,List_Sym[CompteurTableSyntaxique].num_col);
                    }
                }
                CompteurTableSyntaxique++;
            }
        }

        fprintf(pFile, "%d. ",CodeInterI);
        fprintf(pFile, "%s ",aux);
        fprintf(pFile, ":= TEMPO%d \n",tempo);
        CodeInterI++;
        fclose(pFile);
    }

    else if(!strcmp(arbreSyntaxique[j][0],"Exp_simple")&&!strcmp(arbreSyntaxique[j][1],"Terme")){
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                CompteurTableSyntaxique++;
            }
        }
        /*FILE * pFile;
        pFile = fopen ("Code Intermédaire.txt","a");
        fprintf(pFile,"%d. Terme := Exp_S\n",CodeInterI);
        CodeInterI++;
        fprintf(pFile,"%d. Exp_simple := Terme\n",CodeInterI);
        CodeInterI++;
        fclose (pFile);*/
    }

    else if(!strcmp(arbreSyntaxique[j][0],"Exp_S")&&!strcmp(arbreSyntaxique[j][1],"opadd")){
        char op[2];
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                if(!strcmp(List_Sym[CompteurTableSyntaxique].unit,"opadd")){
                    strcpy(op, List_Sym[CompteurTableSyntaxique].op);
                }
                CompteurTableSyntaxique++;
            }
        }
        FILE * pFile;
        pFile = fopen ("Code Intermédaire.txt","a");
        /*fprintf(pFile,"%d. Terme := Exp_S1\n",CodeInterI);
        CodeInterI++;*/
        tempo++;
        fprintf(pFile,"%d. TEMPO%d := TEMPO%d %s TEMPO%d\n",CodeInterI,tempo,tempo-1,op,tempo-2);
        CodeInterI++;
        fclose (pFile);
    }

    else if(!strcmp(arbreSyntaxique[j][0],"Terme")&&!strcmp(arbreSyntaxique[j][1],"Facteur")){

        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                CompteurTableSyntaxique++;
            }
        }
        /*FILE * pFile;
        pFile = fopen ("Code Intermédaire.txt","a");
        fprintf(pFile,"%d. Terme := Facteur\n",CodeInterI);
        CodeInterI++;
        fprintf(pFile,"%d. Facteur := Terme_I\n",CodeInterI);
        CodeInterI++;
        fclose (pFile);*/
    }

    else if(!strcmp(arbreSyntaxique[j][0],"Terme_I")&&!strcmp(arbreSyntaxique[j][1],"opmul")){
        char op[2];
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                if(!strcmp(List_Sym[CompteurTableSyntaxique].unit,"opmul")){
                    strcpy(op, List_Sym[CompteurTableSyntaxique].op);
                }
                CompteurTableSyntaxique++;
            }
        }

        FILE * pFile;
        pFile = fopen ("Code Intermédaire.txt","a");
        /*fprintf(pFile,"%d. Facteur := Terme_I\n",CodeInterI);
        CodeInterI++;*/
        tempo++;
        fprintf(pFile,"%d. TEMPO%d := TEMPO%d %s TEMPO%d\n",CodeInterI,tempo,tempo-1,op,tempo-2);
        CodeInterI++;
        fclose (pFile);
    }

    else if(!strcmp(arbreSyntaxique[j][0],"Facteur")&&!strcmp(arbreSyntaxique[j][1],"id")){
        char aux[256];
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                if(!strcmp(List_Sym[CompteurTableSyntaxique].unit,"id")){
                    if(*List_Id[List_Sym[CompteurTableSyntaxique].num_Id-1].type!=0){
                        Exp_simple=&List_Id[List_Sym[CompteurTableSyntaxique].num_Id-1];
                        sprintf(aux,"%s",List_Id[List_Sym[CompteurTableSyntaxique].num_Id-1].id);
                    }
                    else{
                        printf("ERREUR: VARIABLE NON DECLARE.LIGNE %d ORDRE %d\n",List_Sym[CompteurTableSyntaxique].num_lig,List_Sym[CompteurTableSyntaxique].num_col);
                    }
                }
                CompteurTableSyntaxique++;
            }
        }
        FILE * pFile;
        pFile = fopen ("Code Intermédaire.txt","a");
        tempo++;
        fprintf(pFile,"%d. TEMPO%d := %s\n",CodeInterI,tempo,aux);
        CodeInterI++;
        fclose (pFile);
    }

     else if(!strcmp(arbreSyntaxique[j][0],"Facteur")&&!strcmp(arbreSyntaxique[j][1],"nb")){
        int val;
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                if(!strcmp(List_Sym[CompteurTableSyntaxique].unit,"nb")){
                    val = List_Sym[CompteurTableSyntaxique].val;
                }
                CompteurTableSyntaxique++;
            }
        }

        FILE * pFile;
        pFile = fopen ("Code Intermédaire.txt","a");
        tempo++;
        fprintf(pFile,"%d. TEMPO%d := %d\n",CodeInterI,tempo,val);
        CodeInterI++;
        fclose (pFile);
    }

    else if(!strcmp(arbreSyntaxique[j][0],"Facteur")&&!strcmp(arbreSyntaxique[j][1],"(")){
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                CompteurTableSyntaxique++;
            }
        }
        /*FILE * pFile;
        pFile = fopen ("Code Intermédaire.txt","a");
        fprintf(pFile,"%d. Factor:= Exp_simple\n",CodeInterI);
        CodeInterI++;
        fclose (pFile);*/
    }
    else if(!strcmp(arbreSyntaxique[j][0],"Exp")&&!strcmp(arbreSyntaxique[j][1],"Exp_simple")){
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                CompteurTableSyntaxique++;
            }
        }
       /* FILE * pFile;
        pFile = fopen ("Code Intermédaire.txt","a");
        fprintf(pFile,"%d. Exp_simple := Exp\n",CodeInterI);
        CodeInterI++;
        fprintf(pFile,"%d. Exp_simple := Exp_I\n",CodeInterI);
        CodeInterI++;
        fclose (pFile);*/
    }

    else if(!strcmp(arbreSyntaxique[j][0],"Exp_I")&&!strcmp(arbreSyntaxique[j][1],"oprel")){
    char op[2];
        for(i=1;i<indexColoneArbre[j];i++){
            if(Est_NonTerminal(arbreSyntaxique[j][i])){
                p++;
                analyseSemantiqueNonTerm(p);
            }
            else if (Est_Terminal(arbreSyntaxique[j][i])){
                if(!strcmp(List_Sym[CompteurTableSyntaxique].unit,"oprel")){
                    strcpy(op, List_Sym[CompteurTableSyntaxique].op);
                }
                CompteurTableSyntaxique++;
            }
        }
        FILE* pFile;
        pFile = fopen("Code Intermédaire.txt","a");
        fprintf(pFile,"%d. TEMPO%d %s TEMPO%d\n",CodeInterI,tempo,op,tempo-1);
        CodeInterI++;
        fclose(pFile);
    }
}

void afficher_table_Id_Apres_AnalyseSemantique(id List_Id[1000],int Size_Id){
    int i;
    printf("\nTable Des Ids:\n -----------------------\n");
    printf(" | %3s| %8s |%4s|\n -----------------------\n","N°Id","Id","Type");
    for(i=0;i<Size_Id;i++){
        printf(" | %3d | %8s |%4d|\n ----------------------- \n",List_Id[i].num,List_Id[i].id,*(List_Id[i].type));
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    analyseLexicale(argv[1]);
    afficher_table_Sym(List_Sym,Size_Sym);
    afficher_table_Id(List_Id,Size_Id);
    int i;
    for(i=0;i<tNT;i++){
        premier(nonTerminaux[i]);
    }

    affichagePremier();

    ReintivisiteNonTerminal();

    strcpy(tableSuivant[0][0],"$");
    index_tableSuivant[0]++;
    for(i=0;i<tNT;i++){
       suivant(nonTerminaux[i]);
    }
    printf("\n");
    affichageSuivant();
    RemplirTableM();
    affichierTableM();

    analyseSyntaxique();
    if (VerifierAnalyseSyntaxique){
        afficherArbreSyntaxique();
    }

    FILE * pFile;
    pFile = fopen ("Code Intermédaire.txt","w");
    fclose (pFile);

    Type= malloc(sizeof(int));
    analyseSemantiqueNonTerm(0);
    afficher_table_Id_Apres_AnalyseSemantique(List_Id,Size_Id);
    return 0;
}
