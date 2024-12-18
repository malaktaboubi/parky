#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <hpdf.h>
#include <gtk/gtk.h>
#include "tache4.h"
/////////////////////////////////////////////////////
int cin_exists(const char *cin) {
    FILE *f = fopen("accounts.txt", "r");
    citoyen temp;
    int exist = 0;
    while (fscanf(f, "%s %s", temp.cin, temp.mdp) != EOF) {
        if (strcmp(cin, temp.cin) == 0) {
            exist = 1;
            break;
        }
    }fclose(f);
    return exist;
}
int verifySignup(citoyen c) {
    int isValid = 1;
    // Check CIN length
    if (strlen(c.cin) != 8) {
        printf("CIN must be 8 digits.\n");
        isValid = 0;
    }
    // Check birth date validity
    if (c.birthDate.jour < 1 || c.birthDate.jour > 31 ||
        c.birthDate.mois < 1 || c.birthDate.mois > 12 ||
        c.birthDate.annee < 1900 || c.birthDate.annee > 2024) {
        printf("Invalid birth date.\n");
        isValid = 0;
    }
    // Check phone number and email
    if (strlen(c.numTel) < 8 || !strchr(c.email, '@')) {
        printf("Invalid phone number or email.\n");
        isValid = 0;
    }
    // Check if CIN already exists
    if (cin_exists(c.cin)) {
        printf("CIN already exists. Please use a unique CIN.\n");
        isValid = 0;
    }return isValid;
}

int verifyLogin(const char *log, const char *pw) {
    FILE *admin_file = fopen("auth.txt", "r");
    FILE *citoyen_file = fopen("accounts.txt", "r");
    char admin_id[20], admin_pw[20];
    int found = 0; // 0 = not found, 1 = citizen login, 2 = admin login
    if (admin_file != NULL) {
        while (fscanf(admin_file, "%s %s",admin_id, admin_pw) != EOF) {
            if (strcmp(admin_id, log) == 0 && strcmp(admin_pw, pw) == 0) {
                found = 2; // Admin login
                break;
            }
        }fclose(admin_file);
    }
    if (citoyen_file != NULL) {
        citoyen temp;
        while (fscanf(citoyen_file, "%s %s %s %s %d %d %d %s %s %s %s %s\n", 
            temp.cin, temp.nom, temp.prenom, temp.sexe, 
            &temp.birthDate.jour, &temp.birthDate.mois, &temp.birthDate.annee, 
            temp.adresse, temp.numTel, temp.email, temp.governorat,temp.mdp) != EOF) {
            if (strcmp(temp.cin, log) == 0 && strcmp(temp.mdp, pw) == 0) {
                found = 1; // Citizen login
                break;
            }
        }fclose(citoyen_file);
    }return found;
}
void update_session(const char *cin) {
    FILE *f = fopen("session.txt", "w");
    if (f != NULL) {
        fprintf(f, "%s", cin); // Store the CIN of the logged-in user
        fclose(f);
    } else {
        printf("Error: Unable to open session file for writing.\n");
    }
}

//////////////////////////////////////////////////////////////////////
void addCitoyen(citoyen c) {
    FILE *f = fopen("accounts.txt", "a+");
    if (f != NULL) {
        fprintf(f, "%s %s %s %s %d %d %d %s %s %s %s %s\n", 
            c.cin, c.nom, c.prenom, c.sexe, 
            c.birthDate.jour, c.birthDate.mois, c.birthDate.annee, 
            c.adresse, c.numTel, c.email,c.governorat, c.mdp);
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
        while (fscanf(f, "%s %s %s %s %d %d %d %s %s %s %s %s\n", 
            temp.cin, temp.nom, temp.prenom, temp.sexe, 
            &temp.birthDate.jour, &temp.birthDate.mois, &temp.birthDate.annee, 
            temp.adresse, temp.numTel, temp.email, temp.governorat,temp.mdp) != EOF) {
            printf(temp.cin, cin);
            if (strcmp(temp.cin, cin) == 0) {
            
                fprintf(f2, "%s %s %s %s %d %d %d %s %s %s %s %s\n",
                    nouv.cin, nouv.nom, nouv.prenom, nouv.sexe, 
                    nouv.birthDate.jour, nouv.birthDate.mois, nouv.birthDate.annee, 
                    nouv.adresse, nouv.numTel, nouv.email,temp.governorat, temp.mdp);
                found = 1;
            } else {
                fprintf(f2, "%s %s %s %s %d %d %d %s %s %s %s %s\n", 
                    temp.cin, temp.nom, temp.prenom, temp.sexe, 
                    temp.birthDate.jour, temp.birthDate.mois, temp.birthDate.annee, 
                    temp.adresse, temp.numTel, temp.email, temp.governorat,temp.mdp);
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
        while (fscanf(f, "%s %s %s %s %d %d %d %s %s %s %s %s\n",
            c2.cin, c2.nom, c2.prenom, c2.sexe,
            &c2.birthDate.jour, &c2.birthDate.mois, &c2.birthDate.annee,
            c2.adresse, c2.numTel, c2.email, c2.governorat,c2.mdp) != EOF) {
            if (strcmp(c2.cin,cin) != 0 ) {
                fprintf(f2, "%s %s %s %s %d %d %d %s %s %s %s %s\n",
                    c2.cin, c2.nom, c2.prenom, c2.sexe,
                    c2.birthDate.jour, c2.birthDate.mois, c2.birthDate.annee,
                    c2.adresse, c2.numTel, c2.email,c2.governorat, c2.mdp);
               
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
void searchCitoyen( char *cin){   
    citoyen c2;
    int found = 0;
    FILE * f=fopen("accounts.txt", "r");
    if(f!=NULL) {
        while (fscanf(f, "%s %s %s %s %d %d %d %s %s %s %s %s\n",
            c2.cin, c2.nom, c2.prenom, c2.sexe,
            &c2.birthDate.jour, &c2.birthDate.mois, &c2.birthDate.annee,
            c2.adresse, c2.numTel, c2.email, c2.governorat,c2.mdp) != EOF) {
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
// void formatCitoyenInfo(citoyen c, char output[]) {
//     strcpy(output, "");
//     sprintf(output, "Nom: %s\nPrenom: %s\nCIN: %s\nSexe: %s\nDate de naissance: %02d/%02d/%04d\nAdresse: %s\nNum Tel: %s\nEmail: %s\n",
//             c.nom, c.prenom, c.cin, c.sexe, 
//             c.birthDate.jour, c.birthDate.mois, c.birthDate.annee, 
//             c.adresse, c.numTel, c.email);
// }
// Function to update labels with citizen's name and surname from the file
/*
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
*/
int modifPassword(const char *cin, const char *newPassword) {
    int found = 0;
    citoyen temp;
    FILE *f = fopen("accounts.txt", "r");
    FILE *f2 = fopen("temp.txt", "w");
    if (f != NULL && f2 != NULL) {
        while (fscanf(f, "%s %s %s %s %d %d %d %s %s %s %s %s\n", 
            temp.cin, temp.nom, temp.prenom, temp.sexe, 
            &temp.birthDate.jour, &temp.birthDate.mois, &temp.birthDate.annee, 
            temp.adresse, temp.numTel, temp.email,temp.governorat, temp.mdp) != EOF) {
            
                if (strcmp(temp.cin, cin) == 0) {
                strcpy(temp.mdp, newPassword);
                found = 1;
            }
            fprintf(f2, "%s %s %s %s %d %d %d %s %s %s %s %s\n", 
                    temp.cin, temp.nom, temp.prenom, temp.sexe, 
                    temp.birthDate.jour, temp.birthDate.mois, temp.birthDate.annee, 
                    temp.adresse, temp.numTel, temp.email, temp.governorat,temp.mdp);
        }
        fclose(f);
        fclose(f2);
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");
    }
    return found;
}
///////////////////////////////
/*
void generateFacturePDF(const char *cin, int moisChoisi, float totalCost, const char *details) {
        HPDF_Doc pdf;
    HPDF_Page page;
    char fileName[100];
    sprintf(fileName, "Facture_%s_%02d.pdf", cin, moisChoisi);

    // Create a new PDF document
    pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        printf("Error: Unable to create PDF document.\n");
        return;
    }

    // Add a page to the document
    page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    // Set up font
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 12);

    // Title of the PDF
    HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, 50, 800);
    HPDF_Page_ShowText(page, "Facture Parking - PARKY");
    HPDF_Page_MoveTextPos(page, 0, -30);
    HPDF_Page_ShowText(page, "---------------------------------------");

    // Add CIN and Month
    char header[200];
    sprintf(header, "CIN: %s | Mois: %02d", cin, moisChoisi);
    HPDF_Page_MoveTextPos(page, 0, -30);
    HPDF_Page_ShowText(page, header);

    // Add Total Cost
    char costLine[100];
    sprintf(costLine, "Montant Total: %.2f TND", totalCost);
    HPDF_Page_MoveTextPos(page, 0, -30);
    HPDF_Page_ShowText(page, costLine);

    // Add Details
    HPDF_Page_MoveTextPos(page, 0, -30);
    HPDF_Page_ShowText(page, "Details:");
    HPDF_Page_MoveTextPos(page, 0, -20);
    HPDF_Page_ShowText(page, details);

    HPDF_Page_EndText(page);

    // Save the PDF file
    HPDF_SaveToFile(pdf, fileName);
    HPDF_Free(pdf);

    printf("Facture saved as %s\n", fileName);
}*/

/////////////////
float calculateFacture_for_UI(const char *cin, int moisChoisi) {
    FILE *reservFile, *servicesFile, *parkingFile;
    int id_citoyen, duration, number_places, service_id;
    char nom_parking[50], date_reservation[15], heure[10], commentaires[100];
    float totalCost = 00.0, parkingCostPerHour = 0.0;
    int jour, mois, annee;

    reservFile = fopen("reservation.txt", "r");
    if (!reservFile) {
        printf("Error: Unable to open reservation.txt\n");
        return 0.0;
    }

    // Read reservation file line by line
    while (fscanf(reservFile, "%d %s %s %s %d %d %d %s\n",
                  &id_citoyen, nom_parking, date_reservation, heure,
                  &duration, &number_places, &service_id, commentaires) != EOF) {

        // Extract day, month, and year from the reservation date
        sscanf(date_reservation, "%d-%d-%d", &jour, &mois, &annee);

        if ((id_citoyen == atoi(cin)) && ( mois == moisChoisi)) {
            parkingCostPerHour = 0.0;

            // Open parking file to find parking cost
            parkingFile = fopen("parking.txt", "r");
            if (parkingFile) {
                char park_id[20], park_name[50], park_gouv[50], park_munic[50];
                int nb_p_t, nb_p_d;
                float tarif;

                while (fscanf(parkingFile, "%s %s %s %s %d %d %f %*s %*s %*s\n",
                              park_id, park_name, park_gouv, park_munic,
                              &nb_p_t, &nb_p_d, &tarif) != EOF) {
                    if (strcmp(nom_parking, park_name) == 0) {
                        parkingCostPerHour = tarif;
                        break;
                    }
                }
                fclose(parkingFile);
            }

            // Add parking cost to total
            totalCost += duration * parkingCostPerHour;

            // Open services file to find service cost
            servicesFile = fopen("services.txt", "r");
            if (servicesFile) {
                int srv_id, srv_offre;
                char srv_name[50], srv_desc[100], srv_dispo[20], srv_type[20];

                while (fscanf(servicesFile, "%d %s %s %d %s %s\n",
                              &srv_id, srv_name, srv_desc, &srv_offre, srv_dispo, srv_type) != EOF) {
                    if (service_id == srv_id) {
                        totalCost += srv_offre;
                        break;
                    }
                }
                fclose(servicesFile);
            }
        }
    }
    fclose(reservFile);
    return totalCost;
}


