#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <hpdf.h>
#include "tache4.h"
#include "reservation.h"
#include "projetilef.h"
#include "parking.h"
void verifySignup(citoyen c) {
    if (strlen(c.cin) != 8) {
        printf("CIN must be 8 digits.\n");
        exit(1);
    }
    if (c.birthDate.jour < 1 || c.birthDate.jour > 31 ||
        c.birthDate.mois < 1 || c.birthDate.mois > 12 ||
        c.birthDate.annee < 1900 || c.birthDate.annee > 2024) {
        printf("Invalid birth date.\n");
        exit(1);
    }
    if (strlen(c.numTel) < 8 || strlen(c.email) < 5) {
        printf("Invalid phone number or email.\n");
        exit(1);
    }
}
void formatCitoyenInfo(citoyen c, char output[]) {
    strcpy(output, "");
    sprintf(output, "Nom: %s\nPrenom: %s\nCIN: %s\nSexe: %s\nDate de naissance: %02d/%02d/%04d\nAdresse: %s\nNum Tel: %s\nEmail: %s\n",
            c.nom, c.prenom, c.cin, c.sexe, 
            c.birthDate.jour, c.birthDate.mois, c.birthDate.annee, 
            c.adresse, c.numTel, c.email);
}
void addCitoyen(citoyen c) {
    // verifySignup(c);
    FILE *f = fopen("accounts.txt", "a+");
    if (f != NULL) {
        fprintf(f, "%s %s %s %s %d %d %d %s %s %s %s\n", 
            c.cin, c.nom, c.prenom, c.sexe, 
            c.birthDate.jour, c.birthDate.mois, c.birthDate.annee, 
            c.adresse, c.numTel, c.email, c.mdp);
        fclose(f);
    } else {
        printf("Unable to open file for writing.\n");
    }
}
int modifCitoyen(const char* cin, citoyen nouv) {
    int found = 0;
    citoyen temp;
    FILE *f = fopen("accounts.txt", "r");
    FILE *f2 = fopen("temp.txt", "w");
    if (f != NULL && f2 != NULL) {
        while (fscanf(f, "%s %s %s %s %d %d %d %s %s %s %s\n", 
            temp.cin, temp.nom, temp.prenom, temp.sexe, 
            &temp.birthDate.jour, &temp.birthDate.mois, &temp.birthDate.annee, 
            temp.adresse, temp.numTel, temp.email, temp.mdp) != EOF) {
            printf(temp.cin, cin);
            if (strcmp(temp.cin, cin) == 0) {
                fprintf(f2, "%s %s %s %s %d %d %d %s %s %s %s\n",
                    nouv.cin, nouv.nom, nouv.prenom, nouv.sexe, 
                    nouv.birthDate.jour, nouv.birthDate.mois, nouv.birthDate.annee, 
                    nouv.adresse, nouv.numTel, nouv.email, nouv.mdp);
                found = 1;
            } else {
                fprintf(f2, "%s %s %s %s %d %d %d %s %s %s %s\n", 
                    temp.cin, temp.nom, temp.prenom, temp.sexe, 
                    temp.birthDate.jour, temp.birthDate.mois, temp.birthDate.annee, 
                    temp.adresse, temp.numTel, temp.email, temp.mdp);
            }
        }
        fclose(f);
        fclose(f2);
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");
    }
    return found;
}
int deleteCitoyen(const char *cin) {
    int found = 0;
    citoyen c2;
    FILE *f = fopen("accounts.txt", "r");
    FILE *f2 = fopen("nouv.txt", "w");
    if (f != NULL && f2 != NULL) {
        while (fscanf(f, "%s %s %s %s %d %d %d %s %s %s %s\n",
            c2.cin, c2.nom, c2.prenom, c2.sexe,
            &c2.birthDate.jour, &c2.birthDate.mois, &c2.birthDate.annee,
            c2.adresse, c2.numTel, c2.email, c2.mdp) != EOF) {
            if (strcmp(c2.cin,cin) != 0 ) {
                fprintf(f2, "%s %s %s %s %d %d %d %s %s %s %s\n",
                    c2.cin, c2.nom, c2.prenom, c2.sexe,
                    c2.birthDate.jour, c2.birthDate.mois, c2.birthDate.annee,
                    c2.adresse, c2.numTel, c2.email, c2.mdp);
               
            }else {
 found = 1;
            }
        }
        fclose(f);
        fclose(f2);
        remove("accounts.txt");
        rename("nouv.txt", "accounts.txt");
    }return found; 
  
}
void verifLogin(char log[], char pw[]) {
    citoyen temp;
    int found = 0;
    FILE *f = fopen("auth.txt", "r");

    if (f != NULL) {
        while (fscanf(f, "%s %s\n", temp.cin, temp.mdp) != EOF) {
            if (strcmp(temp.cin, log) == 0 && strcmp(temp.mdp, pw) == 0) {
                printf("Login successful.\n");
                found = 1;
                if (strcmp(log, "admin_cin_value") == 0) {
                    printf("Opening admin interface...\n"); // Admin interface code
                } else {
                    printf("Opening citoyen interface...\n"); // Citoyen interface code
                }
                break;
            }
        }
        fclose(f);
    }
}
void searchCitoyen( char *cin){   
    citoyen c2;
    int found = 0;
    FILE * f=fopen("accounts.txt", "r");
    if(f!=NULL) {
        while (fscanf(f, "%s %s %s %s %d %d %d %s %s %s %s\n",
            c2.cin, c2.nom, c2.prenom, c2.sexe,
            &c2.birthDate.jour, &c2.birthDate.mois, &c2.birthDate.annee,
            c2.adresse, c2.numTel, c2.email, c2.mdp) != EOF) {
                if (strcmp(c2.cin, cin) == 0 ){
                    printf("citizen found: %s , %s, %s \n", c2.cin, c2.nom, c2.prenom);
                    found = 1;
                    break;
                } 
            }
    }
    fclose(f);
    if(found == 0) {
        printf("citizen not found");
    }  
}
/*void formatCitoyenInfo(citoyen c, char output[]) {
    strcpy(output, "");
    sprintf(output, "Nom: %s\nPrenom: %s\nCIN: %s\nSexe: %s\nDate de naissance: %02d/%02d/%04d\nAdresse: %s\nNum Tel: %s\nEmail: %s\n",
            c.nom, c.prenom, c.cin, c.sexe, 
            c.birthDate.jour, c.birthDate.mois, c.birthDate.annee, 
            c.adresse, c.numTel, c.email);
}*/

void calculateFacture(const char *cin, int moisChoisi) {  
Reservation res;
    service srv;
    park park;
    float totalCost = 0.0;
    FILE *factureFile = fopen("facture.txt", "w");
    if (!factureFile) {
        printf("Error: Unable to open facture.txt for writing.\n");
        return;
    }
    FILE *reservFile = fopen("reservation.txt", "r");
    if (!reservFile) {
        printf("Error: Unable to open reservation.txt for reading.\n");
        fclose(factureFile);
        return;
    }
    while (fscanf(reservFile, "%d %s %s %s %d %d %d %s\n",
                  &res.id_citoyen, res.nom_parking, res.date_reservation, res.heure,
                  &res.duree, &res.nombre_place, &res.id_service,
                   res.commentaires) != EOF) {
        int jour, mois, annee;
        sscanf(res.date_reservation, "%d-%d-%d", &jour, &mois, &annee);

        if (res.id_citoyen == atoi(cin) && mois == moisChoisi) {
            // Process reservation costs
            float parkingCostPerHour = 0.0;
            FILE *parkingFile = fopen("parking.txt", "r");
            if (parkingFile) {
                while (fscanf(parkingFile, "%s %s %s %s %d %d %f %s %s %s\n",
                              park.id_p, park.nomp, park.gouv, park.munic,
                              &park.nb_p_t, &park.nb_p_d, &park.tarif,
                              park.ag.nom, park.ag.prenom, park.ag.id_agent) != EOF) {
                    if (strcmp(res.nom_parking, park.nomp) == 0) {
                        parkingCostPerHour = park.tarif;
                        break;
                    }
                }
                fclose(parkingFile);
            }
            float reservationCost = res.duree * parkingCostPerHour;
            totalCost += reservationCost;
            fprintf(factureFile, "- Reservation: Parking Name: %s, Duration: %d hours, Cost: %.2f\n",
                    res.nom_parking, res.duree, reservationCost);
            // Process additional services
            FILE *servicesFile = fopen("services.txt", "r");
            if (servicesFile) {
                while (fscanf(servicesFile, "%d %s %s %d %s %s\n",
                              &srv.id, srv.nom, srv.description, &srv.offre,
                              srv.disponibilite, srv.type) != EOF) {
                    if (res.id_service == srv.id) {
                        fprintf(factureFile, "  - Service: Name: %s, Cost: %d\n",
                                srv.nom, srv.offre);
                        totalCost += srv.offre;
                    }
                }
                fclose(servicesFile);
            }
        }
    
    }
    fclose(reservFile);

    fprintf(factureFile, "\nTotal Cost: %.2f\n", totalCost);
    fclose(factureFile);
    printf("Facture generated successfully in facture.txt\n");
}
void FactureToPDF(const char *factureFile, const char *outputPDF) {
    FILE *f = fopen(factureFile, "r");

    }

int modifPassword(const char *cin, const char *newPassword) {
    int found = 0;
    citoyen temp;
    FILE *f = fopen("accounts.txt", "r");
    FILE *f2 = fopen("temp.txt", "w");
    if (f != NULL && f2 != NULL) {
        while (fscanf(f, "%s %s %s %s %d %d %d %s %s %s %s\n", 
            temp.cin, temp.nom, temp.prenom, temp.sexe, 
            &temp.birthDate.jour, &temp.birthDate.mois, &temp.birthDate.annee, 
            temp.adresse, temp.numTel, temp.email, temp.mdp) != EOF) {
            
                if (strcmp(temp.cin, cin) == 0) {
                strcpy(temp.mdp, newPassword);
                found = 1;
            }
            fprintf(f2, "%s %s %s %s %d %d %d %s %s %s %s\n", 
                    temp.cin, temp.nom, temp.prenom, temp.sexe, 
                    temp.birthDate.jour, temp.birthDate.mois, temp.birthDate.annee, 
                    temp.adresse, temp.numTel, temp.email, temp.mdp);
        }
        fclose(f);
        fclose(f2);
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");
    }
    return found;
}


