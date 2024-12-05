#include "gererparking1.h"
#include <stdio.h>
#include <string.h>

int modifier_agent( char * filename, char ID[], full_agent nouv )
{
    int tr=0;
    full_agent a;
    FILE * f=fopen(filename, "r");
    FILE * f2=fopen("nouv.txt", "w");
    if(f!=NULL && f2!=NULL)
    {
        while(fscanf(f,"%s %s %s %d %s %s %d %s\n", a.nom, a.prenom ,a.id_agent, &(a.telephone), a.email, a.zone, &(a.etat), a.evaluation)!=EOF)
        {
            if(strcmp(a.id_agent, ID) == 0)
            {
                fprintf(f2,"%s %s %s %d %s %s %d %s\n", nouv.nom, nouv.prenom, nouv.id_agent, nouv.telephone, nouv.email, nouv.zone, nouv.etat, nouv.evaluation);
                tr=1;
            }
            else
                fprintf(f2,"%s %s %s %d %s %s %d %s\n", a.nom, a.prenom ,a.id_agent, a.telephone, a.email, a.zone, a.etat, a.evaluation);
        }
    }
    fclose(f);
    fclose(f2);
    remove(filename);
    rename("nouv.txt", filename);
    return tr;
}



int ajouter(char * filename, park p )
{
    FILE * f=fopen(filename, "a");
    if(f!=NULL)
    {
        fprintf(f,"%s %s %s %s %d %d %f %s %s %s\n", p.id_p, p.nomp, p.gouv, p.munic, p.nb_p_t, p.nb_p_d, p.tarif,p.agent.nom, p.agent.prenom, p.agent.id_agent);
        fclose(f);
        return 1;
    }
    else return 0;
}




int modifier(char *filename, char id[], park nouv) 
{
    int tr = 0;
    park p;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("nouv.txt", "w");
    
    if (f != NULL && f2 != NULL) 
    {
        while (fscanf(f, "%s %s %s %s %d %d %f %s %s %s\n", 
                      p.id_p, p.nomp, p.gouv, p.munic, 
                      &(p.nb_p_t), &(p.nb_p_d), &(p.tarif), 
                      p.agent.nom, p.agent.prenom, p.agent.id_agent) != EOF) 
        {
            if (strcmp(p.id_p, id) == 0)
            {
                fprintf(f2, "%s %s %s %s %d %d %.2f %s %s %s\n", 
                        nouv.id_p, nouv.nomp, nouv.gouv, nouv.munic, 
                        nouv.nb_p_t, nouv.nb_p_d, nouv.tarif, 
                        nouv.agent.nom, nouv.agent.prenom, nouv.agent.id_agent);
                tr = 1; 
            } 
            else 
            {
                fprintf(f2, "%s %s %s %s %d %d %.2f %s %s %s\n", 
                        p.id_p, p.nomp, p.gouv, p.munic, 
                        p.nb_p_t, p.nb_p_d, p.tarif, 
                        p.agent.nom, p.agent.prenom, p.agent.id_agent);
            }
        }
        fclose(f);
        fclose(f2);

        // Replace the old file with the new one
        remove(filename);
        rename("nouv.txt", filename);
    } 
    else 
    {
        // Handle file opening errors
        if (f != NULL) fclose(f);
        if (f2 != NULL) fclose(f2);
        return -1; // Error code for file opening failure
    }
    return tr;
}



int supprimer(char * filename, char id[] )
{
    int tr=0;
    park p;
    FILE * f=fopen(filename, "r");
    FILE * f2=fopen("nouv.txt", "w");
    if(f!=NULL && f2!=NULL)
    {while(fscanf(f,"%s %s %s %s %d %d %f %s %s %s\n",p.id_p, p.nomp, p.gouv, p.munic, &(p.nb_p_t), &(p.nb_p_d), &(p.tarif), p.agent.nom, p.agent.prenom, p.agent.id_agent)!=EOF) 
    {
        if(strcmp(p.id_p, id) == 0)
                tr=1;
            else
                fprintf(f2,"%s %s %s %s %d %d %f %s %s %s\n", p.id_p, p.nomp, p.gouv, p.munic, p.nb_p_t, p.nb_p_d, p.tarif,p.agent.nom, p.agent.prenom, p.agent.id_agent);
    }
    }
    fclose(f);
    fclose(f2);
    remove(filename);
    rename("nouv.txt", filename);
    return tr;
}




park recherche(char * filename, char id[] )
{
    park p;
    int tr=0;
    FILE * f=fopen(filename, "r");
    if(f!=NULL)
    {
        while((tr==0) && (fscanf(f,"%s %s %s %s %d %d %f %s %s %s\n", p.id_p, p.nomp, p.gouv, p.munic, &(p.nb_p_t), &(p.nb_p_d), &(p.tarif), p.agent.nom, p.agent.prenom, p.agent.id_agent)!=EOF))
        {
            if(strcmp(p.id_p, id) == 0)
                tr=1;
        }
            fclose(f);
    }

    if (tr == 0) 
        strcpy(p.id_p, "-1"); // Use a string "-1" to indicate not found not 
    return p;
}


//affichage
void afficher(char * filename)
{
	park p;
	FILE * f=fopen(filename, "r");
	if(f!=NULL)
	{
		while(fscanf(f,"%s %s %s %s %d %d %f %s %s %s\n",p.id_p, p.nomp, p.gouv, p.munic, &(p.nb_p_t), &(p.nb_p_d), &(p.tarif), p.agent.nom, p.agent.prenom, p.agent.id_agent)!=EOF)
		  printf("%s %s %s %s %d %d %f %s %s %s\n",p.id_p, p.nomp, p.gouv, p.munic, p.nb_p_t, p.nb_p_d, p.tarif, p.agent.nom, p.agent.prenom, p.agent.id_agent);
		fclose(f);
	}
	else
                printf("Error opening file: %s\n", filename); 
}

	

//affecter 2.0
int affecter_agent(char* fichier, char* fichier1, char idagentpouraffecter[], char idp[]) 
{
    FILE *f1 = fopen("agents.txt", "r");
    FILE *f = fopen(fichier, "r");

    if (f == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return 0;
    }
    if (f1 == NULL) {
        printf("Erreur d'ouverture du fichier agent.\n");
        fclose(f); 
        return 0;
    }

    park pnew;  
    park p;
    full_agent a;
    int modif = 0;
    while (fscanf(f1, "%s %s %s %d %s %s %d %s\n", 
                  a.nom, a.prenom, a.id_agent, &(a.telephone), 
                  a.email, a.zone, &(a.etat), a.evaluation) != EOF) {
        if ((strcmp(idagentpouraffecter, a.id_agent) == 0) && (a.etat == 0)) {
           break;
        }
    }

    if (strcmp(idagentpouraffecter, a.id_agent) != 0 || a.etat != 0) {
        printf("Agent non existant ou déjà affecté.\n");
        fclose(f);
        fclose(f1);
        return 0;
    }

    while (fscanf(f, "%s %s %s %s %d %d %f %s %s %s\n", 
                  p.id_p, p.nomp, p.gouv, p.munic, 
                  &p.nb_p_t, &p.nb_p_d, &p.tarif, 
                  p.agent.nom, p.agent.prenom, p.agent.id_agent) != EOF) {
        if (strcmp(p.id_p, idp) == 0) {
            pnew = p; // Copy the existing park
            strcpy(pnew.agent.nom, a.nom);
            strcpy(pnew.agent.prenom, a.prenom);
            strcpy(pnew.agent.id_agent, a.id_agent);

            int x = modifier(fichier, pnew.id_p, pnew);
            if (x == 1) {
                a.etat = 1;
                int y = modifier_agent("agents.txt", a.id_agent, a);

                if (y == 1) {
                    modif = 1;
                }
            }

            break;
        }
    }

    if (modif == 0) {
        printf("Echec d'affectation.\n");
    }

    fclose(f);
    fclose(f1);

    return modif;
}
