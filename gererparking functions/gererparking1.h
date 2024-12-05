#ifndef GERERPARKING1_H_INCLUDED
#define GERERPARKING1_H_INCLUDED
#include <stdio.h>

typedef struct agent 
{
	char nom[10];
	char prenom[10];
	char id_agent[10];
} agent;

typedef struct full_agent {
    char nom[10];
    char prenom[10];
    char id_agent [10];
    int telephone;
    char email[50];
    char zone[50];
    int etat;
    char evaluation[20];
} full_agent;

typedef struct park
{
	char id_p[10];
	char nomp[10];
	char gouv[10];
	char munic[10];
	int nb_p_t;
	int nb_p_d;
	float tarif;
	agent agent;
}park;


int ajouter(char *, park nouv );
int modifier( char *,char id[], park nouv );
int supprimer(char *, char id[] );
park recherche(char *, char id[]);
int affecter_agent(char* , char* , char idagentpouraffecter[], char idp[]);

#endif 
