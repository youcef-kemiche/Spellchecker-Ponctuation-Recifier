#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#define N 500
#define M 3
//AREZKI KARIM
//KEMICHE YOUCEF


//declaration des structures
typedef struct _CHAINE
{
    char chaine[N];
}CHAINE;

typedef struct _MOT
{
    CHAINE word;
    struct _MOT *next;
}MOT;

typedef struct _CANDIDAT
{
    int numero_candidat;
    CHAINE word;
    struct _CANDIDAT *next;
}CANDIDAT;


typedef struct _ENTETE
{
    MOT *head;
    MOT *tail;
}ENTETE;


typedef struct _ENTETE_CANDIDAT
{
    CANDIDAT*head;
    CANDIDAT*tail;
}ENTETE_CANDIDAT;

//decalration des fonctions

void ajouterdebut(ENTETE *liste, char pch[N]);
ENTETE texte_liste(char chaine[N]);
ENTETE lecture_fichier(FILE *f);
void afficherliste(ENTETE liste);
int chercher_mot(char chercher[N], ENTETE liste);
int minimum(int a, int b, int c);
void min_maj(char chaine[N]);
void maj_min(char chaine[N]);
void maj_lettre_1(CHAINE*cible);
void maj_lettre_1_mot_1(ENTETE liste);
ENTETE_CANDIDAT candidats(CHAINE motinsere, ENTETE dict);
int distance(char chaine1[N], char chaine2[N]);
void ajouterdebut_pour_candidat(ENTETE_CANDIDAT *liste, char pch[N], int i);
void afficherliste_candidat(ENTETE_CANDIDAT *liste);
void affichertexte(ENTETE liste);
CHAINE choix_parmi_candidats(CHAINE mot_a_acorriger, ENTETE dictionaire);
ENTETE correction(ENTETE texte_a_corriger, ENTETE dictionaire);
void enlever_retour_ligne(char chaine[N]);
ENTETE inverser_liste(ENTETE a_inverser);
void ecrire_fichier(ENTETE liste);
CHAINE mot_suivant(CHAINE*actuel, ENTETE*liste);
void ajouter_fin(ENTETE *liste, char nouv[N]);
ENTETE  ponctuation(ENTETE*a_ponctuer);
void point_finale(ENTETE *liste);
void affichertexte_cours(ENTETE liste);
int pnt_colle_mot(char chaine[N]);
char*enlever_pnt(char chaine[N]);
char*ajouter_pnt(char chaine[N], char point);
char get_pnt(char chaine[N]);
char get_pnt_frst(char chaine[N]);
CHAINE mot_precedent(CHAINE*actuel, ENTETE*liste);
ENTETE *espace_version_anglais(ENTETE *liste);
void modifier_precedent(CHAINE*actuel, ENTETE*liste, CHAINE nv);
void esp_retour_ligne(char chaine[N]);






int main(int argc, char*argv[])
{


    printf("******************************************************************\n");
    printf("*                                                                *\n");
    printf("*                                                                *\n");
    printf("*                        PROJET LANGAGE C                        *\n");
    printf("*                     CORRECTEUR D'ORTHOGRAPHE                   *\n");
    printf("*                                                                *\n");
    printf("*                                                                *\n");
    printf("******************************************************************\n");



    printf("\n");
    // printf("                  saisissez votre texte\n");

    char chaine[N];
    char ch[N];
    char temp[N];
    if (argc >0)
    {


        FILE*f = fopen("dict.txt", "r");
        if (f == NULL)
        {
            printf("impossible d'ouvrir le fichier");
        }
        ENTETE dictio_liste = lecture_fichier(f);

        FILE*a_corriger = fopen("text.txt", "r");
        FILE *unie = fopen("uni.txt", "w");

        while (!feof(a_corriger))
        {
            fgets(ch, N, a_corriger);
            printf("%s", ch);
            ENTETE tempo = texte_liste(ch);
            
            MOT *ptr = tempo.head;
            
            
            enlever_retour_ligne(ptr->word.chaine);
            printf("%s", ptr->word.chaine);
            ENTETE tempo_inverse = inverser_liste(tempo);
            MOT *ptre = tempo_inverse.head;
            while (ptre!=NULL)
            {
                fprintf(unie, "%s ", ptre->word.chaine);
                ptre = ptre->next;
            }
            
        }
        fclose(unie);
        FILE *principale = fopen("uni.txt", "r");

        
        
            fgets(chaine, N, principale);
           




            ENTETE liste_from_chaine = texte_liste(chaine);
            ENTETE liste_bon_ordre = inverser_liste(liste_from_chaine);
            ENTETE version_corrigee = correction(liste_bon_ordre, dictio_liste);
            ENTETE version_corrigee_bon_ordre = inverser_liste(version_corrigee);
            ENTETE ponctuee = ponctuation(&version_corrigee_bon_ordre);
            maj_lettre_1_mot_1(ponctuee);
            ENTETE*version_ponctuee_finale = espace_version_anglais(&ponctuee);
            ecrire_fichier(*version_ponctuee_finale);
            



        

        
    }





    return 0;
}







// fichier==>liste
ENTETE lecture_fichier(FILE *f)
{
    char chaine[N];
    ENTETE tete = { NULL,NULL };

    while (!feof(f))
    {

        fgets(chaine, N, f);
        enlever_retour_ligne(chaine);
        ajouterdebut(&tete, chaine);

    }
    return tete;
}

//affichage de la liste
void afficherliste(ENTETE liste)
{
    int i = 1;
    MOT *parcours = liste.head;
    while (parcours != NULL)
    {
        printf("%d-%s \n", i, parcours->word.chaine);
        parcours = parcours->next;
        i++;
    }


}

// afficher les element de la liste contenant  des structures CANDIDAT
void afficherliste_candidat(ENTETE_CANDIDAT*liste)
{
    CANDIDAT *parcours = liste->head;
    printf("%d-IGNORER\n", 0);
    while (parcours != NULL)
    {
        printf("%d-%s \n", parcours->numero_candidat, parcours->word.chaine);
        parcours = parcours->next;

    }

}

// fonction qui cherche un mot dans une liste , retourne 1 si le mot existe
int chercher_mot(char chercher[N], ENTETE liste)
{

    MOT *p = liste.head;
    while (p != NULL)
    {
        if (strcmp(p->word.chaine, chercher) == 0)
        {
            return 1;
        }
        p = p->next;
    }
    return 0;
}


ENTETE texte_liste(char chaine[N])
{
    ENTETE chinsere = { NULL,NULL };
    char delim[] = " ";
    char *ptr = strtok(chaine, delim);
    int i = 0;

    while (ptr != NULL)
    {

        ajouterdebut(&chinsere, ptr);
        ptr = strtok(NULL, delim);
    }
    return chinsere;
}


//fonction qui prend une chaine de caractere et renvoi une chaine qui contient que ddes minuscules 
void maj_min(char chaine[N])
{


    int i = 0;
    while (chaine[i] != '\0')
    {
        if (chaine[i] >= 65 && chaine[i] <= 90)
        {
            chaine[i] = chaine[i] + 32;
        }
        i++;
    }

}

//fonction qui enleve le retour a la ligne  d'un mot
void enlever_retour_ligne(char chaine[N])
{

    int i = 0;

    while (chaine[i] != '\0')
    {
        if (chaine[i] == '\n')
        {
            chaine[i] = '\0';

        }

        i++;

    }

}
//fonction qui transforme le retour a la ligne  d'un mot en espace 
void esp_retour_ligne(char chaine[N])
{

    int i = 0;

    while (chaine[i] != '\0')
    {
        if (chaine[i] == '\n')
        {
            chaine[i] = ' ';

        }

        i++;

    }

}

//miniscule en majuscule
void min_maj(char chaine[N])
{
    int i = 0;
    while (chaine[i] != '\0')
    {
        if (chaine[i] >= 97 && chaine[i] <= 122)
        {
            chaine[i] = chaine[i] - 32;
        }
        i++;
    }

}


//ajouter un element au debut de la liste
void ajouterdebut(ENTETE *liste, char pch[N])
{

    MOT *element = malloc(sizeof(MOT));
    strcpy(element->word.chaine, pch);
    element->next = liste->head;
    if (liste->tail == NULL)
    {
        liste->tail = element;
    }

    liste->head = element;
}

// fonction qui transforme la premiere lettre d'un mot en majuscule
void maj_lettre_1(CHAINE*cible)
{

    if (cible->chaine[0] >= 97 && cible->chaine[0] <= 122)
    {
        cible->chaine[0] = cible->chaine[0] - 32;
    }
}


void maj_lettre_1_mot_1(ENTETE liste)
{
    MOT*parcours = liste.head;
    maj_lettre_1(&parcours->word);


}


// ajout d'element au debut (ca concerne les structures CANDIDAT)
void ajouterdebut_pour_candidat(ENTETE_CANDIDAT *liste, char pch[N], int i)
{

    CANDIDAT *element = malloc(sizeof(CANDIDAT));
    strcpy(element->word.chaine, pch);
    element->numero_candidat = i;
    element->next = liste->head;
    if (liste->tail == NULL)
    {
        liste->tail = element;
    }
    liste->head = element;
}

//distance de levenshtein entre deux mot
int distance(char chaine1[N], char chaine2[N])
{
    int lgch1 = strlen(chaine1);
    int lgch2 = strlen(chaine2);

    int mat[N][N];
    int i, j, cout;
    for (i = 0; i <= lgch1; i++) {
        mat[i][0] = i;
    }
    for (j = 0; j <= lgch2; j++) {
        mat[0][j] = j;
    }

    for (i = 1; i <= lgch1; i++) {
        for (j = 1; j <= lgch2; j++) {
            if (chaine1[i - 1] == chaine2[j - 1]) {
                cout = 0;
            }
            else
            {
                cout = 1;
            }
            mat[i][j] = minimum(mat[i - 1][j] + 1, mat[i][j - 1] + 1, mat[i - 1][j - 1] + cout);
        }

    }
    return mat[lgch1][lgch2];
}

int minimum(int a, int b, int c)
{
    int min = a;
    if (b <= a)
    {
        min = b;
    }
    if (c <= min) { min = c; }
    return min;
}


// la fonction qui renvoie la liste des candidats du mot à corriger 
ENTETE_CANDIDAT candidats(CHAINE motinsere, ENTETE dict)
{
    int min = 0;
    ENTETE_CANDIDAT liste_candidats = { NULL,NULL };
    MOT*parcours = dict.head;

    int lg_acorrige = strlen(motinsere.chaine);
    int lg_dictio;

    min = distance(motinsere.chaine, parcours->word.chaine);
    while (parcours != NULL)
    {
        lg_dictio = strlen(parcours->word.chaine);

        if (lg_dictio <= lg_acorrige + 2 && lg_dictio >= lg_acorrige - 1) // pour limiter la liste des candiddats 
        {

            if (distance(motinsere.chaine, parcours->word.chaine) < min)
            {
                min = distance(motinsere.chaine, parcours->word.chaine); // pour avoir le minimum des distances de Levenshtein
            }

        }
        parcours = parcours->next;
    }

    parcours = dict.head;

    int i = 0;
    while (parcours != NULL)
    {


        if (distance(motinsere.chaine, parcours->word.chaine) == min)
        {
            i++;

        }
        parcours = parcours->next;
    }
    //cette repitition est dans le but d'attribuer les bons numeros car à chaque fois qu'on ajoute au debut on lui attribue le dernier numero
    parcours = dict.head;

    int j = 0;
    while (parcours != NULL)
    {


        if (distance(motinsere.chaine, parcours->word.chaine) == min)
        {
            ajouterdebut_pour_candidat(&liste_candidats, parcours->word.chaine, i - j);
            j++;

        }
        parcours = parcours->next;
    }


    return liste_candidats;

}


//fonction qui laisse à l'utilisateur le choix des candidats du mot à corriger et valide son choix 
CHAINE choix_parmi_candidats(CHAINE mot_a_acorriger, ENTETE dictionaire)
{

    CHAINE correction;
    int choix;
    ENTETE_CANDIDAT can = { NULL,NULL };

    maj_min(mot_a_acorriger.chaine);  // pour transformer le mot en minuscule si il est en majuscule 
    CHAINE finale;
    CHAINE temp;
    temp = mot_a_acorriger;
    strcpy(finale.chaine, enlever_pnt(temp.chaine));
    can = candidats(finale, dictionaire);
    printf("********************* FAUTE ! ******************************\n");
    printf("choisissez une suggestion pour<%s>\n", finale.chaine);
    printf("---------------------------------------------------\n");
    printf("        taper 0 pour ignorer    \n");
    printf("        --------------------\n");
    afficherliste_candidat(&can);
    scanf("%d", &choix);

    CANDIDAT*parcours_candidat = can.head;
    int compteur = 0;
    while (parcours_candidat != NULL)
    {

        if (choix == parcours_candidat->numero_candidat || choix == 0)
        {
            compteur++;
        }

        parcours_candidat = parcours_candidat->next;
    }
    // forcer l'utilisateur  s'il donne un choix hors ceux de la liste des candidats
    while (compteur == 0)
    {
        parcours_candidat = can.head;
        printf("choisissez le candidat par son numero \n");
        scanf("%d", &choix);
        while (parcours_candidat != NULL)
        {

            if (choix == parcours_candidat->numero_candidat || choix == 0)
            {
                compteur++;
            }

            parcours_candidat = parcours_candidat->next;
        }


    }



    // la on est sur que le choix donné par l'utilisateur est bien sur la liste des candidats 


    int trouve = 0;
    parcours_candidat = can.head;


    if (pnt_colle_mot(mot_a_acorriger.chaine) == 1)
    {


        while (parcours_candidat != NULL && trouve == 0)
        {


            if (choix == parcours_candidat->numero_candidat)
            {
                char point;
                CHAINE avc_pt;
                point = get_pnt(mot_a_acorriger.chaine);
                strcpy(avc_pt.chaine, ajouter_pnt(parcours_candidat->word.chaine, point));
                correction = avc_pt;

                trouve = 1;

            }


            if (choix == 0)
            {
                correction = mot_a_acorriger;
            }

            parcours_candidat = parcours_candidat->next;
        }
    }

    else
    {


        while (parcours_candidat != NULL && trouve == 0)
        {


            if (choix == parcours_candidat->numero_candidat)
            {

                correction = parcours_candidat->word;
                trouve = 1;

            }

            if (choix == 0)
            {
                correction = mot_a_acorriger;
            }

            parcours_candidat = parcours_candidat->next;
        }
    }


    maj_min(correction.chaine); // pour transformer le mot selectioné par l'utilisateur en minuscule ,car le dictionaire contient des mots comme English ! 
    return correction;

}


// fonction qui affiche le texte
void affichertexte(ENTETE liste)
{



    MOT*par = liste.head;
    printf("***********************texte corrige************\n");
    while (par != NULL)
    {
        printf("%s ", par->word.chaine);
        par = par->next;
    }


}


// fonction qui renvoit la liste qui contient le texte corrigé
ENTETE correction(ENTETE texte_a_corriger, ENTETE dictionaire)
{

    ENTETE liste_corrige = { NULL,NULL };
    ENTETE texte_en_cours = texte_a_corriger;
    MOT*parcou_from_stdin = texte_a_corriger.head;
    MOT*parcou_cours = texte_en_cours.head;




    while (parcou_from_stdin != NULL)
    {


        maj_min(parcou_from_stdin->word.chaine);
        if (chercher_mot(parcou_from_stdin->word.chaine, dictionaire) == 0 && strcmp(parcou_from_stdin->word.chaine, "?") != 0 && strcmp(parcou_from_stdin->word.chaine, "!") != 0 && strcmp(parcou_from_stdin->word.chaine, ".") != 0)
        {
            printf("\n");
            affichertexte_cours(texte_en_cours);
            printf("\n");
            printf("\n");
            printf("\n");
            CHAINE corrige = choix_parmi_candidats(parcou_from_stdin->word, dictionaire);
            ajouterdebut(&liste_corrige, corrige.chaine);


            parcou_cours = texte_en_cours.head;
            while (parcou_cours != NULL)
            {
                if (strcmp(parcou_cours->word.chaine, parcou_from_stdin->word.chaine) == 0)
                {
                    parcou_cours->word = corrige;


                }


                parcou_cours = parcou_cours->next;

            }
        }

        else  if (chercher_mot(parcou_from_stdin->word.chaine, dictionaire) == 1 || strcmp(parcou_from_stdin->word.chaine, "?") != 0 || strcmp(parcou_from_stdin->word.chaine, "!") != 0 || strcmp(parcou_from_stdin->word.chaine, ".") != 0)
        {
            ajouterdebut(&liste_corrige, parcou_from_stdin->word.chaine);
        }



        parcou_from_stdin = parcou_from_stdin->next;
    }

    return  liste_corrige;



}

// fonction qui inverse une liste chainée
ENTETE inverser_liste(ENTETE a_inverser)
{
    ENTETE resultat = { NULL,NULL };
    MOT*parcours = a_inverser.head;

    while (parcours != NULL)
    {

        ajouterdebut(&resultat, parcours->word.chaine);
        parcours = parcours->next;

    }

    return resultat;

}


//fonction qui ecrit dans un fichier 
void ecrire_fichier(ENTETE liste)
{
    char filename[N] = "correction.txt";
    FILE *f = fopen(filename, "w");
    fprintf(f, "%s ", "  "); // pour laisser l'espace dans le debut de la phrase 
    MOT *parcours = liste.head;
    while (parcours != NULL)
    {
        fprintf(f, "%s ", parcours->word.chaine);
        parcours = parcours->next;
    }
    fclose(f);
}


//fonction qui retourne la chaine qui suit une chaine donné par l'utilisateur 
CHAINE mot_suivant(CHAINE*actuel, ENTETE*liste)
{
    MOT*parcours = liste->head;

    while (parcours != NULL)
    {
        if (strcmp(parcours->word.chaine, actuel->chaine) == 0)
        {
            parcours = parcours->next;
            return parcours->word;

        }

        parcours = parcours->next;
    }

}







// fonction qui ajoute un element à la fin 
void ajouter_fin(ENTETE *liste, char nouv[N])
{
    MOT*parcours = liste->head;
    MOT*element = malloc(sizeof(MOT));
    strcpy(element->word.chaine, nouv);
    element->next = NULL;

    while (parcours->next != NULL)
    {
        parcours = parcours->next;

    }

    parcours->next = element;
    liste->tail = element;

}

// fonction qui ajoute un point final s'il n'existe pas 
void point_finale(ENTETE *liste)
{

    MOT*parcours = liste->tail;

    if (strcmp(parcours->word.chaine, ".") != 0)
    {
        ajouter_fin(liste, ".");
    }
}


// fonction quimet la premiere lettre du mot qui viens juste apres ? ou ! ou . en majuscule
ENTETE ponctuation(ENTETE*a_ponctuer)
{

    MOT*parcours = a_ponctuer->head;
    ENTETE version_ponct = { NULL,NULL };

    int test = 0;
    while (parcours != NULL)
    {
        if (strcmp(parcours->word.chaine, "?") == 0 || strcmp(parcours->word.chaine, "!") == 0 || strcmp(parcours->word.chaine, ".") == 0 || pnt_colle_mot(parcours->word.chaine) == 1)
        {

            CHAINE resultat;
            resultat = mot_suivant(&parcours->word, a_ponctuer);
            maj_lettre_1(&resultat);
            ajouterdebut(&version_ponct, parcours->word.chaine);
            ajouterdebut(&version_ponct, resultat.chaine);
            test = 1;

        }

        else if (test == 1)
        {
            test = 0;  // pour eviter d'ajouter de nouveau le mot qui suit ?!. car c'est deja fait juste au dessus 
        }

        else
        {
            ajouterdebut(&version_ponct, parcours->word.chaine);

        }

        parcours = parcours->next;

    }


    ENTETE  version_ponct_ordonee = inverser_liste(version_ponct);
    point_finale(&version_ponct_ordonee); //rajouter le point final s'il n'existe pas
    return version_ponct_ordonee;


}

// fonction qui affiche le texte en cours de modification
void affichertexte_cours(ENTETE liste)
{



    MOT*par = liste.head;
    printf("°°°°°°°°°°°°°°°°° texte en cours °°°°°°°°°°°°°°°°°\n");
    while (par != NULL)
    {
        printf("%s ", par->word.chaine);
        par = par->next;
    }


}



//fonction qui retourne 1 si le mot contient ?ou!ou.  ,0 sinon
int pnt_colle_mot(char chaine[N])
{
    int i = 0;
    while (chaine[i] != '\0')
    {
        i++;
    }
    i--;
    if (chaine[i] == '.' || chaine[i] == '!' || chaine[i] == '?')return 1;
    return 0;

}


//fonction qui enleve ?!. dans le mot s'il contiens un de ces trois derniers
char*enlever_pnt(char chaine[N])
{

    int i = 0;
    while (chaine[i] != '\0')
    {
        i++;
    }
    i--;
    if (chaine[i] == '.' || chaine[i] == '!' || chaine[i] == '?')
    {
        chaine[i] = '\0';

    }

    return chaine;

}

char*ajouter_pnt(char chaine[N], char point)
{


    int i = 0;
    while (chaine[i] != '\0')
    {
        i++;
    }
    chaine[i] = point;
    chaine[i + 1] = '\0';
    return chaine;

}

char get_pnt(char chaine[N])
{

    int i = 0;
    while (chaine[i] != '\0')
    {
        i++;
    }
    return chaine[i - 1];

}



//fonction qui retourne la chaine qui precede une chaine donné par l'utilisateur 
CHAINE mot_precedent(CHAINE*actuel, ENTETE*liste)
{
    MOT*parcours = liste->head;
    int cpt = 0;
    int arret = 0;
    while (parcours != NULL && arret == 0)
    {
        if (strcmp(parcours->word.chaine, actuel->chaine) == 0)
        {
            arret = 1;

        }
        cpt++;
        parcours = parcours->next;
    }

    int avc = 0;
    parcours = liste->head;
    while (avc != cpt - 2)
    {

        parcours = parcours->next;
        avc++;

    }

    return parcours->word;


}


//fonction qui modofie la chaine qui precede une chaine donné par l'utilisateur 
void modifier_precedent(CHAINE*actuel, ENTETE*liste, CHAINE nv)
{
    MOT*parcours = liste->head;
    int cpt = 0;
    int arret = 0;
    while (parcours != NULL && arret == 0)
    {
        if (strcmp(parcours->word.chaine, actuel->chaine) == 0)
        {
            arret = 1;

        }
        cpt++;
        parcours = parcours->next;
    }

    int avc = 0;
    parcours = liste->head;
    while (avc != cpt - 2)
    {

        parcours = parcours->next;
        avc++;

    }

    parcours->word = nv;


}

ENTETE*espace_version_anglais(ENTETE *liste)
{

    ENTETE*finale;
    finale = liste;
    MOT*parcours = finale->head;


    while (parcours != NULL)
    {

        if (strcmp(parcours->word.chaine, "?") == 0 || strcmp(parcours->word.chaine, "!") == 0 || strcmp(parcours->word.chaine, ".") == 0)
        {



            CHAINE resultat;
            char point = get_pnt(parcours->word.chaine);

            resultat = mot_precedent(&parcours->word, finale);

            CHAINE fnl;
            strcpy(fnl.chaine, ajouter_pnt(resultat.chaine, point));
            modifier_precedent(&parcours->word, finale, fnl);
            strcpy(parcours->word.chaine, "");

        }


        parcours = parcours->next;



    }


    return finale;




}
