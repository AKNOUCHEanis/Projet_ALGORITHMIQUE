#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define CDEL 2
#define CINS 2
#define INFINI pow(10,100)


int CSUB(char,char);
void afficherCouple(char*,int,char*,int);
int DIST_NAIF_REC(char*,int,char*,int,int,int,int,long int);
void concatener (char **,char *);
void aligne_lettre_mot (char** ,char** );
char* concat_2 (char *,char *);

int DIST_NAIF(char* x,int tailleX,char* y,int tailleY)
{
    return DIST_NAIF_REC(x,tailleX,y,tailleY,0,0,0,INFINI);
}

int DIST_NAIF_REC(char* x,int t1,char* y,int t2,int i,int j,int c,long int dist)
{
    if((i==t1)&&(j==t2))
    {
        if(c<dist){
            dist=c;
        }
    }else{
        if((i<t1)&&(j<t2))
        {
            dist=DIST_NAIF_REC(x,t1,y,t2,i+1,j+1,c+CSUB(x[i],y[j]),dist);
        }
        if(i<t1)
        {
            dist=DIST_NAIF_REC(x,t1,y,t2,i+1,j,c+CDEL,dist);
        }
        if(j<t2)
        {
            dist=DIST_NAIF_REC(x,t1,y,t2,i,j+1,c+CINS,dist);
        }
    }
    return dist;
}

int Min(int x,int y,int z)
{
    int min=y;
    if (x<y) min=x;
    if (z<min) min=z;
    return min;
}


int DIST_1(char* x,int tailleX,char* y,int tailleY,int *D[])
{

    int i,j;
    for( i=0;i<tailleX+1;i++)
    {
        D[i][0]=i*CDEL;
    }


    for( i=0;i<tailleY+1;i++)
    {
        D[0][i]=i*CINS;
    }
    for( i=1;i<tailleX+1;i++)
    {
        for( j=1;j<tailleY+1;j++)
        {
            D[i][j]=Min(D[i-1][j]+CDEL,D[i][j-1]+CINS,D[i-1][j-1]+CSUB(x[i-1],y[j-1]));
        }
    }
        return D[tailleX][tailleY];
}

int DIST_2 (char* x,int tailleX,char *y, int tailleY)
{
    int D[2][tailleY+1];
    int cpt=1,j;
    /******* INITIALISATION **********/
    for(j=0;j<=tailleY;j++)
    {
        D[0][j]=j*CINS;
    }
    D[1][0]=CDEL;
    /*********************************/
    while(cpt<=tailleX)
    {
        for(j=1;j<=tailleY;j++){
            D[1][j]=Min(D[0][j-1]+CSUB(x[cpt-1],y[j-1]),D[0][j]+CDEL,D[1][j-1]+CINS);
        }
        /*********Copie de la ligne 1 dans la ligne 0**********/
        cpt++;
        for(j=0;j<=tailleY;j++){
            D[0][j]=D[1][j];
        }
        D[1][0]=D[0][0]+CDEL;
        /******************************************************/
    }
    return D[0][tailleY];
}

void inversion (char* X,int size){

    char* xi=malloc(sizeof(char)*size);
    int i=0;

    for (i=size-1;i>=0;i--){
        xi[size-1-i]=X[i];
    }
    for (i=size-1;i>=0;i--){
        X[i]=xi[i];
    }
    free(xi);

}

void Sol1(char* x,int tailleX,char* y,int tailleY,char* Xb,char* Yb,int **D,int *taille)
{
    int i=tailleX,j=tailleY;
    int min;
    *taille=0;

    while(i>0 && j>0)
    {   min=Min(D[i-1][j],D[i][j-1],D[i-1][j-1]);
        if(min==D[i-1][j])
        {   (Xb)[*taille]=x[i-1];
            (Yb)[*taille]='-';
            i--;
            }
        else if(min==D[i][j-1])
        {
            (Xb)[*taille]='-';
            (Yb)[*taille]=y[j-1];
            j--;
            }
        else if(min==D[i-1][j-1])
        {   (Xb)[*taille]=x[i-1];
            (Yb)[*taille]=y[j-1];
            i--;
            j--;
            }
            (*taille)++;
    }
    /**********Inversion************/
    inversion(Xb,*taille);  free(x);
    inversion(Yb,*taille);  free(y);
}

void PROG_DYN(char** x,int tailleX,char** y,int tailleY,char* Xb,char* Yb,int **D)
{

    int dist=DIST_1(*x,tailleX,*y,tailleY,D);
    int taille=0;                                /** taille est la Taille de Xb et Yb**/
    Xb=malloc(sizeof(char)*(tailleX+tailleY));
    Yb=malloc(sizeof(char)*(tailleX+tailleY));

    Sol1(*x,tailleX,*y,tailleY,Xb,Yb,D,&taille); /** Xb et Yb sont les mots de l'alignement **/
    printf("La distance d'edition = %d\n",dist);
    printf("L'alignement optimal :\n");
    afficherCouple(Xb,taille,Yb,taille);
}

int CSUB(char a,char b)
{
    if(a==b)
    {
        return 0;
    }else
    {
        if(((a=='A')&&(b='T'))||((a=='T')&&(b=='A'))||((a=='G')&&(b=='C'))||((a=='C')&&(b=='G')))
        {
            return 3;
        }
        else{
            return 4;
        }
    }
}

void afficherCouple(char* x,int t1,char* y,int t2)
{   int i=0;
    printf("(X=");
    for(i=0;i<t1;i++)
    {
        printf("%c",x[i]);
    }
    printf(" , Y=");
    for( i=0;i<t2;i++)
    {
        printf("%c",y[i]);
    }
    printf(") \n");

}

void AfficherMatrice(int **D,int x,int y)
{
    int i,j;
    printf("Afficher la matrice D : \n");
    for(i=0;i<=x;i++)
    {
        for(j=0;j<=y;j++)
        {
            printf("%d ",D[i][j]);
        }
        printf("\n");
    }
}

char* mot_gaps (int k){
    char *x=malloc(sizeof(char)*(k+1));
    int j;
    for(j=0;j<k;j++)
    {
        x[j]='-';
    }
    x[k]='\0';
    return x;
}

int contientLettre (char *y,char* x){
    /** retourn  sa position si la lettre de x est dans y,0 sinon**/
    int i;
    for (i=0;i<strlen(y);i++){
        if (y[i]==x[0]) return i;
    }
    return -1;
}



int lettreMemefamille (char* y,char* x){
    int i;
    if (x[0]=='T'){
        for (i=0;i<strlen(y);i++){
            if (y[i]=='A') return i;
        }
    }
    else if (x[0]=='A'){
        for (i=0;i<strlen(y);i++){
            if (y[i]=='T') return i;
        }
    }
    else if (x[0]=='C'){
        for (i=0;i<strlen(y);i++){
            if (y[i]=='G') return i;
        }
    }
    else if (x[0]=='G'){
        for (i=0;i<strlen(y);i++){
            if (y[i]=='C') return i;
        }
    }
    return -1;
}

int coupure (char* x,char *y,int i){
    int tailleX=strlen(x);
    int tailleY=strlen(y);
    int D[2][tailleY+1];
    int I[2][tailleY+1];
    int cpt=1,j;
    /******* INITIALISATION **********/
    for(j=0;j<=tailleY;j++)
    {
        D[0][j]=j*CINS;
        I[0][j]=j;
    }
    D[1][0]=CDEL;
    I[1][0]=I[0][0];
    /*********************************/
    while(cpt<=tailleX)
    {
        for(j=1;j<=tailleY;j++){
            D[1][j]=Min(D[0][j-1]+CSUB(x[cpt-1],y[j-1]),D[0][j]+CDEL,D[1][j-1]+CINS);
            if (cpt>i){
                if (D[1][j]==D[0][j-1]+CSUB(x[cpt-1],y[j-1])) {
                    I[1][j]=I[0][j-1];
                }
                else if(D[1][j]==D[1][j-1]+CINS){
                    I[1][j]=I[1][j-1];
                }
                else if(D[1][j]==D[0][j]+CDEL){
                    I[1][j]=I[0][j];
                }
            }
        }
        /*********Copie de la ligne 1 dans la ligne 0**********/
        for(j=0;j<=tailleY;j++){
            D[0][j]=D[1][j];
        }
        if (cpt>i){
            for(j=0;j<=tailleY;j++){
                I[0][j]=I[1][j];
            }
        }
        cpt++;
        I[1][0]=I[0][0];
        D[1][0]=D[0][0]+CDEL;
        /******************************************************/
    }
    return I[1][tailleY];
}

void diviser(char *x,char ** x1,char ** x2,int pos){
    int j;
    for (j=0;j<pos;j++){
        (*x1)[j]=x[j];
    }
    for (j=pos;j<strlen(x);j++){
        (*x2)[j-pos]=x[j];
    }
    (*x1)[pos]='\0';
    (*x2)[strlen(x)-pos]='\0';

}

void SOL_2 (char* x,int tailleX,char* y,int tailleY,char** Xb,char** Yb){

    if (tailleY==0){
        concatener(Xb,x);
        (*Yb)=mot_gaps(tailleX);
    }
    else if (tailleX==1){
        concatener(Xb,x);
        concatener(Yb,y);
        aligne_lettre_mot(Xb,Yb);
    }
    else{
        int j=coupure(x,y,tailleX/2);

        char *x1=malloc(sizeof(char)*(tailleX/2+2));

        char** X1b=malloc(sizeof(int));
        (*X1b)=malloc(sizeof(char)*(tailleX+tailleY));
        (*X1b)[0]='\0';

        char *y1=malloc(sizeof(char)*(j+1));

        char **Y1b=malloc(sizeof(int));
        (*Y1b)=malloc(sizeof(char)*(tailleX+tailleY));
        (*Y1b)[0]='\0';

        char *x2=malloc(sizeof(char)*(tailleX/2+2));

        char **X2b=malloc(sizeof(int));
        (*X2b)=malloc(sizeof(char)*(tailleX+tailleY));
        (*X2b)[0]='\0';

        char *y2=malloc(sizeof(char)*(tailleY-j+1));

        char **Y2b=malloc(sizeof(int));
        (*Y2b)=malloc(sizeof(char)*(tailleX+tailleY));
        (*Y2b)[0]='\0';

        diviser(x,&x1,&x2,(tailleX/2));
        diviser(y,&y1,&y2,j);

        SOL_2(x1,strlen(x1),y1,strlen(y1),X1b,Y1b);
        SOL_2(x2,strlen(x2),y2,strlen(y2),X2b,Y2b);

        (*Xb)=concat_2(*X1b,*X2b);
        (*Yb)=concat_2(*Y1b,*Y2b);

        free(x1);
        free(y1);
        free(x2);
        free(y2);
        free(*X1b);
        free(*Y1b);
        free(*X2b);
        free(*Y2b);
    }

}

void aligne_lettre_mot (char** x,char** y){
    char * xi=malloc(sizeof(char)*(strlen((*y)+1)));
    int j;
    for (j=0;j<strlen(*y);j++){
        xi[j]='\0';
    }
    int m=strlen(*y);
    int i=contientLettre(*y,*x);
    if (i!=-1){
        concatener(&xi,mot_gaps(i));
        xi[i]=(*x)[0];
        concatener(&xi,mot_gaps(m-i-1));
        xi[m]='\0';
        (*x)=xi;
    }
    else{
        i=lettreMemefamille(*y,*x);
        if (i!=-1){
            concatener(&xi,mot_gaps(i));
            xi[i]=(*x)[0];
            concatener(&xi,mot_gaps(m-i-1));
            xi[m]='\0';
            (*x)=xi;
        }
        else{
            xi[0]=(*x)[0];
            concatener(&xi,mot_gaps(m-1));
            xi[m]='\0';
            (*x)=xi;
        }
    }

}

void concatener (char **x,char *y){
    int m=strlen(*x),j;
    for (j=m;j<m+strlen(y);j++){
        (*x)[j]=y[j-m];
    }
    (*x)[m+strlen(y)]='\0';
}


char* concat_2 (char *x,char *y){
    char* z=malloc(sizeof(char)*(strlen(x)+strlen(y)+1));
    int m=strlen(x),j;
    for (j=0;j<m;j++){
        z[j]=x[j];
    }
    for (j=m;j<m+strlen(y);j++){
        z[j]=y[j-m];
    }
    z[m+strlen(y)]='\0';
    return z;
}


void LIRE_FICHIER(char* nomFichier,char** x,int* tailleX,char** y,int* tailleY)
{   FILE* fichier=NULL;
    fichier=fopen(nomFichier,"r");
    char* chaine1=malloc(10*sizeof(char));
    char* chaine2=malloc(10*sizeof(char));

    if(fichier!=NULL)
    {
        fgets(chaine1,9,fichier);
        *tailleX=atoi(chaine1);
        fgets(chaine2,9,fichier);
        *tailleY=atoi(chaine2);
        int i=0;
        *x=malloc((*tailleX)*sizeof(char));
        *y=malloc((*tailleY)*sizeof(char));
        char c;

        while(i<(*tailleX))        /** RECUPERER LE PREMIER MOT X **/
        {   c=fgetc(fichier);
            if(c!=' ' && c!=EOF)
            {   (*x)[i]=c;
                i++;
            }
        }
       while(fgetc(fichier)!='\n')
       {   }
       i=0;

       while(i<(*tailleY))      /** RECUPERER DEUXIEME MOT Y **/
        {   c=fgetc(fichier);
            if(c!=' ' && c!=EOF)
            {   (*y)[i]=c;
                i++;
            }
        }
        free(chaine1);
        free(chaine2);
    }else{
    printf("Impossible d'ouvrire le fichier\n");
    }
    fclose(fichier);
}


void main(int arg,char *argv[])
{
/**************** test DIST_NAIF*********************** */
/*
int tailleX=0;
int tailleY=0;
char** x=malloc(sizeof(int));
char** y=malloc(sizeof(int));
*x=NULL; *y=NULL;

LIRE_FICHIER("Instances_genome/Inst_0000015_4.adnInst_0000010_88.adn",x,&tailleX,y,&tailleY);
printf("** CALCUL DE LA DISTANCE D'EDITION DU COUPLE DE MOTS (X,Y) **\n");
afficherCouple(*x,tailleX,*y,tailleY);

clock_t start,end;
double elapsed;

start=clock();

int dist=DIST_NAIF(*x,tailleX,*y,tailleY);
end=clock();             //** FIN DE L'EXECUTION DE PROG_DYN **

elapsed = ((double)end - start) / CLOCKS_PER_SEC;    //** CALCUL DU TEMPS D'EXECUTION **

printf("La distance d'edition =%d \n",dist);
printf("Temps d'exectution de DIST_NAIF = %2f \n",elapsed);

   //system("ps aux | grep main");  //commande sous linux

*/
/**************** test DIST_1*********************** */
/*
int tailleX=0;
int tailleY=0;
char** x=malloc(sizeof(int));
char** y=malloc(sizeof(int));
*x=NULL; *y=NULL;

LIRE_FICHIER("Instances_genome/Inst_0000015_4.adn",x,&tailleX,y,&tailleY);
printf("** CALCUL DE LA DISTANCE D'EDITION DU COUPLE DE MOTS (X,Y) **\n");
afficherCouple(*x,tailleX,*y,tailleY);
int** D=(int**)malloc((tailleX+1)*sizeof(int*));
    int i=0;
    for(i=0;i<tailleX+1;i++)
    {
        D[i]=(int*)malloc((tailleY+1)*sizeof(int));
    }

    clock_t start,end;
    double elapsed;

    start=clock();

int dist=DIST_1(*x,tailleX,*y,tailleY,D);

    end=clock();             //** FIN DE L'EXECUTION DE PROG_DYN **

    elapsed = ((double)end - start) / CLOCKS_PER_SEC;    //** CALCUL DU TEMPS D'EXECUTION **


printf("La distance d'edition =%d \n",dist);
AfficherMatrice(D,tailleX,tailleY);
printf("Temps d'exectution de DIST_1 = %2f \n",elapsed);
   //system("ps aux | grep main");  //commande sous linux


*/
/***** Test PROG_DYN ******************************************/
/*
    int tailleX=0;
    int tailleY=0;
    char** x=malloc(sizeof(int));
    char** y=malloc(sizeof(int));
    *x=NULL; *y=NULL;

    LIRE_FICHIER("Instances_genome/Inst_0050000_6.adn",x,&tailleX,y,&tailleY);
    printf("** CALCUL DE LA DISTANCE D'EDITION DU COUPLE DE MOTS (X , Y) **\n");
    afficherCouple(*x,tailleX,*y,tailleY);
    printf("De Taille :(%d, %d)\n",tailleX,tailleY);

    int** D=(int**)malloc((tailleX+1)*sizeof(int*));
    int i=0;
    for(i=0;i<tailleX+1;i++)
    {
        D[i]=(int*)malloc((tailleY+1)*sizeof(int));
    }

    char* Xb=NULL;
    char* Yb=NULL;

    clock_t start,end;
    double elapsed;

    start=clock();           //** DEBUT DE L'EXECUTION DE PROG_DYN **

    PROG_DYN(x,tailleX,y,tailleY,Xb,Yb,D);

    end=clock();             //** FIN DE L'EXECUTION DE PROG_DYN **

    elapsed = ((double)end - start) / CLOCKS_PER_SEC;    //** CALCUL DU TEMPS D'EXECUTION **
    printf("Temps d'exectution de PROG_DYN = %2f \n",elapsed);

    //system("ps aux | grep main");  //commande sous linux
*/

/***** Test DIST_2 ***************/
/*
int tailleX=0;
int tailleY=0;
char** x=malloc(sizeof(int));
char** y=malloc(sizeof(int));
*x=NULL; *y=NULL;

clock_t start,end;
double elapsed;

LIRE_FICHIER("Instances_genome/Inst_0020000_5.adn",x,&tailleX,y,&tailleY);
printf("** CALCUL DE LA DISTANCE D'EDITION DU COUPLE DE MOTS (X,Y) **\n");
afficherCouple(*x,tailleX,*y,tailleY);

start=clock();

int dist=DIST_2(*x,tailleX,*y,tailleY);

end=clock();             //** FIN DE L'EXECUTION DE DIST_2 **

elapsed = ((double)end - start) / CLOCKS_PER_SEC;    //** CALCUL DU TEMPS D'EXECUTION **

printf("La distance d'edition =%d \n",dist);
printf("Temps d'exectution de DYST_2 = %2f \n",elapsed);

//system("ps aux | grep main");  //commande sous linux
*/
/******************* Test SOL_2 *************************/

int tailleX=0;
int tailleY=0;
char** x=malloc(sizeof(int));
char** y=malloc(sizeof(int));
*x=NULL; *y=NULL;
int i;

clock_t start,end;
double elapsed;

LIRE_FICHIER("Instances_genome/Inst_0000015_4.adn",x,&tailleX,y,&tailleY);

char * xb=malloc(sizeof(char)*(strlen(*y)+strlen(*x)));
char * yb=malloc(sizeof(char)*(strlen(*y)+strlen(*x)));

for (i=0;i<strlen(*x);i++){
    xb[i]='\0';
}
for (i=0;i<strlen(*y);i++){
    yb[i]='\0';
}

printf("** ALIGNEMENT OPTIMAL DU COUPLE DE MOTS (X,Y) **\n");
afficherCouple(*x,tailleX,*y,tailleY);
(*x)[tailleX]='\0';
(*y)[tailleY]='\0';
printf("LA COUPURE EST DE : %d\n",coupure(*x,*y,strlen(*x)/2));

start=clock();

SOL_2(*x,strlen(*x),*y,strlen(*y),&xb,&yb);

end=clock();
             //** FIN DE L'EXECUTION DE DIST_2 **
elapsed = ((double)end - start) / CLOCKS_PER_SEC;    //** CALCUL DU TEMPS D'EXECUTION **

printf("L'ALIGNEMENT OPTIMAL EST : ( %s , %s )\n",xb,yb);
printf("Temps d'exectution de SOL_2 = %2f \n",elapsed);

//system("ps aux | grep main");  //commande sous linux

}



