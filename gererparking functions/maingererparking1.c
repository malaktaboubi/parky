#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gererparking1.h"

void display_menu() {
    printf("\n--- Gestion des Parkings ---\n");
    printf("1. Ajouter un parking\n");
    printf("2. Modifier un parking\n");
    printf("3. Supprimer un parking\n");
    printf("4. Rechercher un parking\n");
    printf("5. Affecter un agent à un parking\n");
    printf("6. Afficher les parkings\n");
    printf("7. Quitter\n");
    printf("Choisissez une option: ");
}

int main() {
    char filename_parks[] = "parkings.txt";
    char filename_agents[] = "agents.txt";
    int choix;
    char id[10];
    park p;
    full_agent agent;
    int result;
    char ida[10];
    do {
        display_menu();
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                printf("\n--- Ajouter un parking ---\n");
                printf("ID: ");
                scanf("%s", p.id_p);
                printf("Nom: ");
                scanf("%s", p.nomp);
                printf("Gouvernorat: ");
                scanf("%s", p.gouv);
                printf("Municipalité: ");
                scanf("%s", p.munic);
                printf("Nombre total de places: ");
                scanf("%d", &p.nb_p_t);
                printf("Nombre de places disponibles: ");
                scanf("%d", &p.nb_p_d);
                printf("Tarif: ");
                scanf("%f", &p.tarif);

                // Placeholder agent details
                strcpy(p.agent.nom, "0");
                strcpy(p.agent.prenom, "0");
                strcpy(p.agent.id_agent, "0");

                result = ajouter(filename_parks, p);
                if (result)
                    printf("Parking ajouté avec succès.\n");
                else
                    printf("Erreur lors de l'ajout du parking.\n");
                break;

            case 2:
                printf("\n--- Modifier un parking ---\n");
                printf("ID du parking à modifier: ");
                scanf("%s", id);
                printf("Nouveaux détails:\n");
                printf("Nom: ");
                scanf("%s", p.nomp);
                printf("Gouvernorat: ");
                scanf("%s", p.gouv);
                printf("Municipalité: ");
                scanf("%s", p.munic);
                printf("Nombre total de places: ");
                scanf("%d", &p.nb_p_t);
                printf("Nombre de places disponibles: ");
                scanf("%d", &p.nb_p_d);
                printf("Tarif: ");
                scanf("%f", &p.tarif);

                result = modifier(filename_parks, id, p);
                if (result)
                    printf("Parking modifié avec succès.\n");
                else
                    printf("Erreur lors de la modification du parking.\n");
                break;

            case 3:
                printf("\n--- Supprimer un parking ---\n");
                printf("ID du parking à supprimer: ");
                scanf("%s", id);

                result = supprimer(filename_parks, id);
                if (result)
                    printf("Parking supprimé avec succès.\n");
                else
                    printf("Erreur lors de la suppression du parking.\n");
                break;

            case 4:
                printf("\n--- Rechercher un parking ---\n");
                printf("ID du parking à rechercher: ");
                scanf("%s", id);

                p = recherche(filename_parks, id);
                if (strcmp(p.id_p, "-1") != 0) {
                    printf("Parking trouvé:\n");
                    printf("ID: %s\nNom: %s\nGouvernorat: %s\nMunicipalité: %s\n", 
                           p.id_p, p.nomp, p.gouv, p.munic);
                    printf("Places Totales: %d, Disponibles: %d\n", p.nb_p_t, p.nb_p_d);
                    printf("Tarif: %.2f\n", p.tarif);
                    printf("Agent Assigné: %s %s (ID: %s)\n", 
                           p.agent.nom, p.agent.prenom, p.agent.id_agent);
                } else {
                    printf("Parking non trouvé.\n");
                }
                break;

            case 5:
                printf("\n--- Affecter un agent à un parking ---\n");
                printf("ID de l'agent à affecter: ");
                scanf("%s", ida);
                printf("ID du parking: ");
                scanf("%s", id);

                result = affecter_agent(filename_parks, filename_agents, ida, id);
                if (result)
                    printf("Agent affecté avec succès.\n");
                else
                    printf("Erreur lors de l'affectation de l'agent.\n");
                break;

            case 6:
                afficher(filename_parks);
                break;

            default:
                printf("Option invalide. Veuillez réessayer.\n");
        }
    } while (choix != 7);

    return 0;
}
