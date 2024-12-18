#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <gtk/gtk.h>
#include "callbacks.h"
#include <hpdf.h>
#include "interface.h"
#include "support.h"
#include "tache4.h"
citoyen c;
char cinid[20];
int sexe,modifmdp=0;
/////// inscription  //////////
/////// AJOUT   //////////////
void
on_buttonvalidinscrip_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *inscription_window; GtkWidget *espace_cit_window;
citoyen c;
GtkWidget * name, * pre, * cin, * password,* adr,* email,* pwd,* num,* gov, *spinbutton_jour, *spinbutton_mois, *spinbutton_annee;
GtkWidget *fail_label, *succ_label;
const gchar *governorat_text = NULL;
name = lookup_widget(GTK_WIDGET(button), "entryNom");
pre = lookup_widget(GTK_WIDGET(button), "entryPrenom");
cin = lookup_widget(GTK_WIDGET(button), "entryCin");
password = lookup_widget(GTK_WIDGET(button), "entrymdp4");
email = lookup_widget(GTK_WIDGET(button), "entrymail4");
num = lookup_widget(GTK_WIDGET(button), "entrynum4");
adr = lookup_widget(GTK_WIDGET(button), "entryadr4"); 
pwd = lookup_widget(GTK_WIDGET(button), "entrymdpconfirm");
gov = lookup_widget(GTK_WIDGET(button), "comboboxgov4");
spinbutton_jour = lookup_widget(GTK_WIDGET(button), "spinbuttonBDj");
spinbutton_mois = lookup_widget(GTK_WIDGET(button), "spinbuttonBDm");
spinbutton_annee = lookup_widget(GTK_WIDGET(button), "spinbuttonBDy");
fail_label = lookup_widget(GTK_WIDGET(button), "labelFail");
succ_label = lookup_widget(GTK_WIDGET(button), "labelsucces");
// Retrieve text from entries
strcpy(c.nom, gtk_entry_get_text(GTK_ENTRY( name)));
strcpy(c.prenom, gtk_entry_get_text(GTK_ENTRY( pre)));
strcpy(c.cin, gtk_entry_get_text(GTK_ENTRY( cin)));
strcpy(c.email, gtk_entry_get_text(GTK_ENTRY( email)));
strcpy(c.numTel, gtk_entry_get_text(GTK_ENTRY( num)));
strcpy(c.adresse, gtk_entry_get_text(GTK_ENTRY( adr)));
strcpy(c.mdp, gtk_entry_get_text(GTK_ENTRY( password)));
strcpy(c.governorat, gtk_combo_box_get_active_text(GTK_COMBO_BOX(gov)));
c.birthDate.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_jour));
c.birthDate.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_mois));
c.birthDate.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_annee));
if(sexe==1){strcpy(c.sexe,"Homme");}
else{strcpy(c.sexe,"Femme");}
if (verifySignup(c)) {
    addCitoyen(c);
    gtk_widget_show(succ_label);
    inscription_window =  lookup_widget(GTK_WIDGET(button), "inscription");
    espace_cit_window =  create_authentification ();
    gtk_widget_destroy(inscription_window);
    gtk_widget_show(espace_cit_window);
    }
else{
    gtk_widget_show(fail_label);
    gtk_widget_hide(succ_label);
}
}
void
on_radiobuttonmf_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
    sexe=2;}
}
void
on_radiobuttonmh_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
    sexe=1;}
}
////////////////////////////////////////
///////   authentification    /////////
////////////////////////////////////
void
on_buttonAojutCit_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{  
GtkWidget *authentification;GtkWidget *inscription;
authentification = lookup_widget(GTK_WIDGET(button), "authentification"); 
if (authentification != NULL) {
     gtk_widget_destroy(authentification);
    }
    inscription = create_inscription();
    gtk_widget_show(inscription);
}
////////////////////////////////////////////////////////////////////////////////
void prefill_labels_after_login(GtkWidget *login_window, const char *cin) {
    GtkWidget *labelNespc, *LabelPreespc;
    citoyen current_citizen;
    int found = 0; 
    FILE *accounts_file = fopen("accounts.txt", "r");
    if (accounts_file == NULL) {
        printf("Error: Unable to open accounts.txt\n");
        return;
    }
    while (fscanf(accounts_file, "%s %s %s %s %d %d %d %s %s %s %s %s\n",
                current_citizen.cin, current_citizen.nom, current_citizen.prenom, current_citizen.sexe,
                &current_citizen.birthDate.jour, &current_citizen.birthDate.mois, &current_citizen.birthDate.annee,
                current_citizen.adresse, current_citizen.numTel, current_citizen.email, current_citizen.governorat, current_citizen.mdp) != EOF) {
        if (strcmp(current_citizen.cin, cin) == 0) {
            found = 1;break;
        }
    }
    fclose(accounts_file);
    if (!found) {
        printf("Error: Citizen not found in accounts.txt\n");
        return;
    }
    labelNespc = lookup_widget(login_window, "labelNespc");
    LabelPreespc = lookup_widget(login_window, "LabelPreespc");
    if (labelNespc && LabelPreespc) {
        gtk_label_set_text(GTK_LABEL(labelNespc), current_citizen.nom);
        gtk_label_set_text(GTK_LABEL(LabelPreespc), current_citizen.prenom);
    } else {
        printf("Error: Labels not found in the window.\n");
    }
}
void
on_buttonConnexion_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{ 
GtkWidget * id, * password, *login_fail_label;
GtkWidget *admin_window, *citoyen_window, *login_window;
int loginResult;
char entered_id[20], entered_password[20];
int is_valid = 0, is_admin = 0;
id = lookup_widget(GTK_WIDGET(button), "entryAuthID");
password = lookup_widget(GTK_WIDGET(button), "entryAuthMdp");
login_fail_label = lookup_widget(GTK_WIDGET(button), "labelverifauth");
strcpy(entered_id, gtk_entry_get_text(GTK_ENTRY( id)));
strcpy(entered_password, gtk_entry_get_text(GTK_ENTRY( password)));
loginResult = verifyLogin(entered_id, entered_password);
//char str[30]; snprintf( str, 30, "%d", loginResult );gtk_label_set_text(GTK_LABEL(login_fail_label), str);
    if (loginResult == 2) {  
            admin_window = create_w1espaceadmin();
            gtk_widget_show(admin_window);
            login_window = lookup_widget(GTK_WIDGET(button), "authentification");
            gtk_widget_destroy(login_window);
        } else if (loginResult == 1) { // Citizen login
        citoyen_window = create_w4espacecitoyen();
        gtk_widget_show(citoyen_window);
        login_window = lookup_widget(GTK_WIDGET(button), "authentification");
        gtk_widget_destroy(login_window);
        // Pass the CIN to prefill the labels after login
        prefill_labels_after_login(citoyen_window, entered_id);
    } else { 
       // gtk_label_set_text(GTK_LABEL(login_fail_label), "Invalid ID or password. Please try again.");
        gtk_widget_show(login_fail_label);
    }
    if (loginResult != 0) {
        FILE *f = fopen("session.txt", "w");
        if (f != NULL) {
            fprintf(f, "%s", entered_id);
            fclose(f);
        } else {printf("Unable to open file for writing.\n");
        }
    }
}
/////////////////////////////////////////////////////////////////////// 
////////////////////modif//////////////////////////////////////////
/////////////////////////////////////////////
void
on_buttonmodif4_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{

GtkWidget *name, *pre, *cin, *password,*adr,*email,*old_pwd,*new_pwd,*confirm_pwd,*togglebtn_mdp,*gov,*num,
 *spinbutton_jour, *spinbutton_mois, *spinbutton_annee;
 citoyen c;
     const gchar *oldPassword, *newPassword, *confirmPassword;

name = lookup_widget(GTK_WIDGET(button), "entrymnn");
pre = lookup_widget(GTK_WIDGET(button), "entrympre");
cin = lookup_widget(GTK_WIDGET(button), "entrymcin");
adr = lookup_widget(GTK_WIDGET(button), "entrymadr");
email = lookup_widget(GTK_WIDGET(button), "entrymmail");
num = lookup_widget(GTK_WIDGET(button), "entrymtel");
old_pwd = lookup_widget(GTK_WIDGET(button), "entryoldpass"); 
new_pwd = lookup_widget(GTK_WIDGET(button), "entrympass");
confirm_pwd = lookup_widget(GTK_WIDGET(button), "entrycheckpass");
togglebtn_mdp=lookup_widget(GTK_WIDGET(button), "checkbuttonmodifpwd");
//gov = lookup_widget(GTK_WIDGET(button), "comboboxmgov4");
spinbutton_jour = lookup_widget(GTK_WIDGET(button), "spinbuttonmj");
spinbutton_mois = lookup_widget(GTK_WIDGET(button), "spinbuttonmm");
spinbutton_annee = lookup_widget(GTK_WIDGET(button), "spinbuttonmy");
// Retrieve text from entries
strcpy(c.nom, gtk_entry_get_text(GTK_ENTRY(name)));
strcpy(c.prenom, gtk_entry_get_text(GTK_ENTRY(pre)));
strcpy(c.cin, gtk_entry_get_text(GTK_ENTRY(cin)));
strcpy(c.email, gtk_entry_get_text(GTK_ENTRY(email)));
strcpy(c.numTel, gtk_entry_get_text(GTK_ENTRY(num)));
strcpy(c.adresse, gtk_entry_get_text(GTK_ENTRY(adr)));
// strcpy(c.mdp, gtk_entry_get_text(GTK_ENTRY(confirm_pwd)));
// strcpy(c.mdp, gtk_entry_get_text(GTK_ENTRY(new_pwd)));
// strcpy(c.mdp, gtk_entry_get_text(GTK_ENTRY(old_pwd)));
// strcpy(c.governorat, gtk_combo_box_get_active_text(GTK_COMBO_BOX( gov)));
c.birthDate.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_jour));
c.birthDate.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_mois));
c.birthDate.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_annee));
if(sexe==1){
    strcpy(c.sexe,"Homme");
    }else{
    strcpy(c.sexe,"Femme");
    }
    if (modifmdp == 1) { 
      oldPassword = gtk_entry_get_text(GTK_ENTRY(old_pwd));
        newPassword = gtk_entry_get_text(GTK_ENTRY(new_pwd));
        confirmPassword = gtk_entry_get_text(GTK_ENTRY(confirm_pwd));
    }
// Verify and modify the citizen info
if (modifCitoyen(c.cin, c)) {
    printf("Citizen information updated successfully.\n");

    // Update session file with the new CIN
    FILE *session_file = fopen("session.txt", "w");
    if (session_file != NULL) {
        fprintf(session_file, "%s", c.cin);
        fclose(session_file);
    } else {
        printf("Error: Unable to update session file.\n");
    }

    // Optionally, refresh the interface to display updated information
} else {
    printf("Failed to update citizen information.\n");
}

}
void
on_checkbuttonmodifpwd_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
 GtkWidget *vboxm = lookup_widget(GTK_WIDGET(togglebutton), "hbox31");  // Retrieve the GtkVBox pointer passed as user data

     if (gtk_toggle_button_get_active(togglebutton)) {
         modifmdp=1;
        gtk_widget_show(vboxm);  
    } else {
        gtk_widget_hide(vboxm);  
        modifmdp = 0;
    }
}

///////////////////////////////////////
///////   espace citoyen    /////
////////////////////////////////////
void
on_parametre_toggled                   (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data)
{
GtkWidget *vbox = lookup_widget(GTK_WIDGET(toggletoolbutton), "vbox7"); 
    if (gtk_toggle_tool_button_get_active(toggletoolbutton))
     {
        gtk_widget_show(vbox);  
    } else {
        gtk_widget_hide(vbox);  
    }
     
}
void
on_buttonDeconnecter_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *current_window, *auth_window;
    FILE *session_file = fopen("session.txt", "w");
    if (session_file != NULL) {
        fclose(session_file);
    }
    current_window = lookup_widget(GTK_WIDGET(button), "w4espacecitoyen"); // Replace with your current window's name
    gtk_widget_destroy(current_window);
    auth_window = create_authentification (); 
    gtk_widget_show(auth_window);
}
/////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void
prefill_modifier_infos(GtkWidget *modifier_window)
{
    GtkWidget *name_entry, *prenom_entry, *cin_entry, *adresse_entry, *email_entry, *tel_entry;
    GtkWidget *spin_jour, *spin_mois, *spin_annee;
    GtkToggleButton *radio_male, *radio_female;
    citoyen current_citizen;
    char cin[20];
    int found = 0;
    // Retrieve CIN from session file
    FILE *session_file = fopen("session.txt", "r");
    if (session_file == NULL || fscanf(session_file, "%s", cin) != 1) {
        printf("Error: Failed to read CIN from session file.\n");
        if (session_file != NULL) fclose(session_file);
        return;
    }
    fclose(session_file);
    // Retrieve citizen info from accounts.txt
    FILE *accounts_file = fopen("accounts.txt", "r");
    if (accounts_file == NULL) {
        printf("Error: Unable to open accounts.txt\n");
        return;
    }
    while (fscanf(accounts_file, "%s %s %s %s %d %d %d %s %s %s %s %s\n",
                  current_citizen.cin, current_citizen.nom, current_citizen.prenom, current_citizen.sexe,
                  &current_citizen.birthDate.jour, &current_citizen.birthDate.mois, &current_citizen.birthDate.annee,
                  current_citizen.adresse, current_citizen.numTel, current_citizen.email, current_citizen.governorat, current_citizen.mdp) != EOF) {
        if (strcmp(current_citizen.cin, cin) == 0) {
            found = 1;
            break;
        }
    }
    fclose(accounts_file);
    if (!found) {
        printf("Error: Citizen not found in accounts.txt\n");
        return;
    }
    // Map widgets
    name_entry = lookup_widget(modifier_window, "entrymnn");
    prenom_entry = lookup_widget(modifier_window, "entrympre");
    cin_entry = lookup_widget(modifier_window, "entrymcin");
    adresse_entry = lookup_widget(modifier_window, "entrymadr");
    email_entry = lookup_widget(modifier_window, "entrymmail");
    tel_entry = lookup_widget(modifier_window, "entrymtel");
    spin_jour = lookup_widget(modifier_window, "spinbuttonmj");
    spin_mois = lookup_widget(modifier_window, "spinbuttonmm");
    spin_annee = lookup_widget(modifier_window, "spinbuttonmy");
    radio_male = GTK_TOGGLE_BUTTON(lookup_widget(modifier_window, "radiobuttonmh"));
    radio_female = GTK_TOGGLE_BUTTON(lookup_widget(modifier_window, "radiobuttonmf"));
    // Prefill data
    gtk_entry_set_text(GTK_ENTRY(name_entry), current_citizen.nom);
    gtk_entry_set_text(GTK_ENTRY(prenom_entry), current_citizen.prenom);
    gtk_entry_set_text(GTK_ENTRY(cin_entry), current_citizen.cin);
    gtk_entry_set_text(GTK_ENTRY(adresse_entry), current_citizen.adresse);
    gtk_entry_set_text(GTK_ENTRY(email_entry), current_citizen.email);
    gtk_entry_set_text(GTK_ENTRY(tel_entry), current_citizen.numTel);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_jour), current_citizen.birthDate.jour);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_mois), current_citizen.birthDate.mois);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_annee), current_citizen.birthDate.annee);
    if (strcmp(current_citizen.sexe, "Homme") == 0) {
        gtk_toggle_button_set_active(radio_male, TRUE);
    } else {
        gtk_toggle_button_set_active(radio_female, TRUE);
    }
}
void
on_buttonGestionInfo_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window_show, *window_destroy;
    GtkWidget *label_nn, *label_pr, *label_id, *label_birth;
    GtkWidget *label_gender, *label_mail, *label_phone, *label_adress, *label_govv;
    GtkWidget *gestion_info_window;
    citoyen current_citizen;
    char cin[20];
    int found = 0;
      // Open session file to retrieve the current citizen's CIN
    FILE *session_file = fopen("session.txt", "r");
    if (session_file != NULL) {
        if (fscanf(session_file, "%s", cin) != 1) {
            printf("Error: Failed to read CIN from session.txt\n");
            fclose(session_file);
            return;
        }
        fclose(session_file);
    } else {
        printf("Error: Unable to open session.txt\n");
        return;
    }

    // Open accounts file to search for the citizen's details
    FILE *accounts_file = fopen("accounts.txt", "r");
    if (accounts_file != NULL) {
        citoyen temp;
        while (fscanf(accounts_file, "%s %s %s %s %d %d %d %s %s %s %s %s\n",
                      temp.cin, temp.nom, temp.prenom, temp.sexe,
                      &temp.birthDate.jour, &temp.birthDate.mois, &temp.birthDate.annee,
                      temp.adresse, temp.numTel, temp.email, temp.governorat, temp.mdp) != EOF) {
            if (strcmp(temp.cin, cin) == 0) {
                current_citizen = temp;
                found = 1;
                break;
            }
        }
        fclose(accounts_file);
    } else {
        printf("Error: Unable to open accounts.txt\n");
        return;
    }

    if (!found) {
        printf("Error: Citizen not found in accounts.txt\n");
        return;
    }
    // Close the current citizen's window and show the information management window
    window_destroy = lookup_widget(GTK_WIDGET(button), "w4espacecitoyen");
    gtk_widget_destroy(window_destroy);
    gestion_info_window = create_w4(); // Replace with the actual function to create the info window
    gtk_widget_show(gestion_info_window);
    prefill_modifier_infos(gestion_info_window);
    // Update the labels in the "gestion info" window with citizen's information
    label_nn = lookup_widget(gestion_info_window, "labeln41");
    label_pr = lookup_widget(gestion_info_window, "labelpr41");
    label_id = lookup_widget(gestion_info_window, "labelcin41");
    label_birth = lookup_widget(gestion_info_window, "labelbirth41");
    label_gender = lookup_widget(gestion_info_window, "labels41");
    label_mail = lookup_widget(gestion_info_window, "labelmail41");
    label_phone = lookup_widget(gestion_info_window, "labeltel41");
    label_adress = lookup_widget(gestion_info_window, "labeladr41");
    label_govv = lookup_widget(gestion_info_window, "labelgov41");
gtk_label_set_text(GTK_LABEL(label_nn), g_strconcat(gtk_label_get_text(GTK_LABEL(label_nn)), " ",  current_citizen.nom, NULL));
gtk_label_set_text(GTK_LABEL(label_nn), g_strconcat(gtk_label_get_text(GTK_LABEL(label_pr)), " ", current_citizen.prenom, NULL));
gtk_label_set_text(GTK_LABEL(label_id), g_strconcat(gtk_label_get_text(GTK_LABEL(label_id)), " ", current_citizen.cin, NULL));
char birth_date[50];
sprintf(birth_date, "%d/%d/%d", current_citizen.birthDate.jour, current_citizen.birthDate.mois, current_citizen.birthDate.annee);
gtk_label_set_text(GTK_LABEL(label_birth), g_strconcat(gtk_label_get_text(GTK_LABEL(label_birth)), " ", birth_date, NULL));
gtk_label_set_text(GTK_LABEL(label_gender), g_strconcat(gtk_label_get_text(GTK_LABEL(label_gender)), " ", current_citizen.sexe, NULL));
gtk_label_set_text(GTK_LABEL(label_mail), g_strconcat(gtk_label_get_text(GTK_LABEL(label_mail)), " ", current_citizen.email, NULL));
gtk_label_set_text(GTK_LABEL(label_phone), g_strconcat(gtk_label_get_text(GTK_LABEL(label_phone)), " ", current_citizen.numTel, NULL));
gtk_label_set_text(GTK_LABEL(label_adress), g_strconcat(gtk_label_get_text(GTK_LABEL(label_adress)), " ", current_citizen.adresse, NULL));
gtk_label_set_text(GTK_LABEL(label_govv), g_strconcat(gtk_label_get_text(GTK_LABEL(label_govv)), " ", current_citizen.governorat, NULL));
   
}

void
on_buttonGestionRev_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window_show, *window_destroy;
    window_destroy = lookup_widget(GTK_WIDGET(button), "w4espacecitoyen");
    gtk_widget_destroy(window_destroy);
    window_show = create_w5();
    gtk_widget_show(window_show);
}


void
on_buttonGestionAvis_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window_show, *window_destroy;
    window_destroy = lookup_widget(GTK_WIDGET(button), "w4espacecitoyen");
    gtk_widget_destroy(window_destroy);
    window_show = create_w6();
    gtk_widget_show(window_show);
}
///////////////////////////////
////////// espace admin ///////////
/////////////////////////////
void
on_buttonReturnad_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *gestion_park_window, *gestion_serv_window, *gestion_ag_window,*gestion_cit_window,*espace_admin_window;
    gestion_park_window = lookup_widget(GTK_WIDGET(button), "w1");
    gestion_serv_window = lookup_widget(GTK_WIDGET(button), "w3");
    gestion_ag_window = lookup_widget(GTK_WIDGET(button), "w2");
    gestion_cit_window = lookup_widget(GTK_WIDGET(button), "w4");
    if (gestion_park_window && GTK_WIDGET_VISIBLE(gestion_park_window)) {
        gtk_widget_destroy(gestion_park_window);
    }
    if (gestion_serv_window && GTK_WIDGET_VISIBLE(gestion_serv_window)) {
        gtk_widget_destroy(gestion_serv_window);
    }
    if (gestion_ag_window && GTK_WIDGET_VISIBLE(gestion_ag_window)) {
        gtk_widget_destroy(gestion_ag_window);
    }
   if (gestion_cit_window && GTK_WIDGET_VISIBLE(gestion_cit_window)) {
        gtk_widget_destroy(gestion_cit_window);
    }
    espace_admin_window = create_w1espaceadmin();
    gtk_widget_show(espace_admin_window);
}
void
on_buttonGestionAg_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window_show, *window_destroy;
    window_destroy = lookup_widget(GTK_WIDGET(button), "w1espaceadmin");
    gtk_widget_destroy(window_destroy);
    window_show = create_w2();
    gtk_widget_show(window_show);
    

}
void
on_buttonGestionServ_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window_show, *window_destroy;
    window_destroy = lookup_widget(GTK_WIDGET(button), "w1espaceadmin");
    gtk_widget_destroy(window_destroy);
    window_show = create_w3();
    gtk_widget_show(window_show);
}
void
on_buttonGestionPark_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window_show, *window_destroy;
    window_destroy = lookup_widget(GTK_WIDGET(button), "w1espaceadmin");
    gtk_widget_destroy(window_destroy);
    window_show = create_w1();
    gtk_widget_show(window_show);
  
}

////////facture//////////////


////////////////////////
/////// dialog supp compte /////
///////////////////////////////




void
on_buttonReturncit_clicked(GtkButton *button, gpointer user_data)
{GtkWidget *gestion_reserv_window, *gestion_info_window, *espace_citoyen_window,*gestion_avis_window;
char cin[20];
    
    // Retrieve the citizen's CIN from the session file
    FILE *session_file = fopen("session.txt", "r");
    if (session_file) {
        fscanf(session_file, "%s", cin);
        fclose(session_file);
    } else {
        printf("Error: Unable to open session.txt\n");
        return;
    }

    // Try to find the windows if they are open
    gestion_reserv_window = lookup_widget(GTK_WIDGET(button), "w5");
    gestion_info_window = lookup_widget(GTK_WIDGET(button), "w4");
    gestion_avis_window = lookup_widget(GTK_WIDGET(button), "w6");
    // Check if "Gestion des Réservations" window is open and destroy it
    if (gestion_reserv_window && GTK_WIDGET_VISIBLE(gestion_reserv_window)) {
        gtk_widget_destroy(gestion_reserv_window);
    }
    // Check if "Gestion des Informations" window is open and destroy it
    if (gestion_info_window && GTK_WIDGET_VISIBLE(gestion_info_window)) {
        gtk_widget_destroy(gestion_info_window);
    }
 // Check if "Gestion des Informations" window is open and destroy it
    if (gestion_avis_window && GTK_WIDGET_VISIBLE(gestion_avis_window)) {
        gtk_widget_destroy(gestion_avis_window);
    }
    // Create and show the "Espace Citoyen" window
    espace_citoyen_window = create_w4espacecitoyen();
    gtk_widget_show(espace_citoyen_window);
    prefill_labels_after_login(espace_citoyen_window, cin);
}


void
on_buttoncomfirmsupp_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *current_window, *auth_window, *dialog_confirm;
    char cin[20];
    int delete_success;
    dialog_confirm = lookup_widget(GTK_WIDGET(button), "dialogSuppCpt");
    FILE *session_file = fopen("session.txt", "r");
    if (session_file != NULL) {
        if (fscanf(session_file, "%s", cin) != 1) {
            fclose(session_file);
            printf("Error: Failed to read CIN from session file.\n");
            gtk_widget_hide(dialog_confirm); // Close dialog on error
            return;
        }
        fclose(session_file);
    } else {
        printf("Error: Unable to open session file.\n");
        gtk_widget_hide(dialog_confirm); // Close dialog on error
        return;
    }
    delete_success = deleteCitoyen(cin);
    if (delete_success) {
        printf("Account successfully deleted for CIN: %s\n", cin);
        session_file = fopen("session.txt", "w");
        if (session_file != NULL) {
            fclose(session_file);
        }
        current_window = lookup_widget(GTK_WIDGET(button), "w4espacecitoyen");
        if (current_window != NULL) {
            gtk_widget_destroy(current_window);
        }
        auth_window = create_authentification();
        gtk_widget_show(auth_window);
    } else {
        printf("Failed to delete account for CIN: %s\n", cin);
    }

    // Hide the confirmation dialog
    gtk_widget_hide(dialog_confirm);
}
void
on_buttoncancelsupp_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *dialog_confirm;

    // Retrieve the confirmation dialog widget
    dialog_confirm = lookup_widget(GTK_WIDGET(button), "dialogSuppCpt");

    if (dialog_confirm != NULL) {
        // Simply hide the confirmation dialog
        gtk_widget_hide(dialog_confirm);
    }
}
void
on_buttonSuppCpt_clicked(GtkButton *button, gpointer user_data)
{ GtkWidget *dialog_confirm;



   dialog_confirm = create_dialogSuppCpt ();
    if (dialog_confirm != NULL) {
        gtk_widget_show(dialog_confirm);
    } else {
        printf("Error: Confirmation dialog not found.\n");
    }
}

void
prefill_treeview_listeCit(GtkWidget *treeview)
{
    GtkListStore *list_store;
    GtkTreeIter iter;
    FILE *accounts_file;
    citoyen temp;
    // Define the columns for the ListStore (CIN, Nom, Prenom, etc.)
    list_store = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    // Open the accounts file and populate the ListStore
    accounts_file = fopen("accounts.txt", "r");
    if (accounts_file == NULL) {
        printf("Error: Unable to open accounts.txt\n");
        return;
    }
    while (fscanf(accounts_file, "%s %s %s %s %d %d %d %s %s %s %s %s\n",
                  temp.cin, temp.nom, temp.prenom, temp.sexe,
                  &temp.birthDate.jour, &temp.birthDate.mois, &temp.birthDate.annee,
                  temp.adresse, temp.numTel, temp.email, temp.governorat, temp.mdp) != EOF) {
        // Add each citizen's data to the ListStore
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter,
                            0, temp.cin,
                           1, temp.nom,
                           2, temp.prenom,
                           3, temp.sexe,
                           4, g_strdup_printf("%02d/%02d/%04d", temp.birthDate.jour, temp.birthDate.mois, temp.birthDate.annee),
                           5, temp.governorat,
                           -1);
    }

    fclose(accounts_file);
    // Set the model for the TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(list_store));
    // Unreference the ListStore
    g_object_unref(list_store);
}
void
setup_treeview_columns(GtkWidget *treeview)
{
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    // Create and attach "CIN" column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("CIN", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    // Create and attach "Nom" column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    // Create and attach "Prenom" column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Prenom", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
      // Create and attach "Sexe" column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Sexe", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Create and attach "Date de Naissance" column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Date de Naissance", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Create and attach "Gouvernorat" column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Gouvernorat", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
}

void
on_buttonListeCit_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *liste_cit_window, *current_window, *treeview;
    // Get the current admin window (if any) and close it
    current_window = lookup_widget(GTK_WIDGET(button), "w1espaceadmin");
    if (current_window != NULL) {
        gtk_widget_destroy(current_window);
    }
    // Create the "Liste des Citoyens" window
    liste_cit_window = create_w4listeCit();
    // Locate the TreeView widget in the new window
    treeview = lookup_widget(liste_cit_window, "treeview11");
    // Set up the TreeView columns (if not pre-defined in Glade)
    setup_treeview_columns(treeview);
    // Prefill the TreeView with citizen data
    prefill_treeview_listeCit(treeview);
    // Show the "Liste des Citoyens" window
    gtk_widget_show(liste_cit_window);
}
void
on_buttonHelp_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
}

void
on_buttonQuit_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{ GtkWidget *current_window, *auth_window;
  current_window = lookup_widget(GTK_WIDGET(button), "w1espaceadmin"); // Replace with your current window's name
    gtk_widget_destroy(current_window);
    auth_window = create_authentification (); 
    gtk_widget_show(auth_window);
}

void
on_buttonAbout_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget  *about_window;
 
    about_window = create_aboutdialog1 (); 
    gtk_widget_show(about_window);

}




void populate_treeview_reservations(GtkWidget *treeview, int selected_month)
{
    GtkListStore *list_store;
    GtkTreeIter iter;
    FILE *reservationsFile, *servicesFile, *parkingFile;
    int id_citoyen, duration, number_places, service_id;
    char nom_parking[50], date_reservation[15], heure[10], commentaires[100];
    char service_name[50];
    int service_cost = 0;
    float parking_cost_per_hour = 0.0, total_cost = 0.0;

    // Create the ListStore (columns: Citizen ID, Parking Name, Date, Duration, Places, Service Name, Cost)
    list_store = gtk_list_store_new(7, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, 
                                    G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);

    // Open reservations file
    reservationsFile = fopen("reservation.txt", "r");
    if (!reservationsFile) {
        printf("Error: Unable to open reservation.txt\n");
        return;
    }

    // Read reservation data
    while (fscanf(reservationsFile, "%d %s %s %s %d %d %d %s\n",
                  &id_citoyen, nom_parking, date_reservation, heure, 
                  &duration, &number_places, &service_id, commentaires) != EOF) {
        int jour, mois, annee;

        // Extract day, month, and year from the date
        sscanf(date_reservation, "%d-%d-%d", &jour, &mois, &annee);

        // Check if the reservation matches the selected month
        if (mois == selected_month) {
            // Find parking cost
            parking_cost_per_hour = 0.0;
            parkingFile = fopen("parking.txt", "r");
            if (parkingFile) {
                char park_id[20], park_name[50], park_gouv[50], park_munic[50];
                int nb_p_t, nb_p_d;
                float tarif;

                while (fscanf(parkingFile, "%s %s %s %s %d %d %f %*s %*s %*s\n",
                              park_id, park_name, park_gouv, park_munic,
                              &nb_p_t, &nb_p_d, &tarif) != EOF) {
                    if (strcmp(nom_parking, park_name) == 0) {
                        parking_cost_per_hour = tarif;
                        break;
                    }
                }
                fclose(parkingFile);
            }

            // Find service name and cost
            strcpy(service_name, "Unknown");
            service_cost = 0;
            servicesFile = fopen("services.txt", "r");
            if (servicesFile) {
                int srv_id;
                char srv_desc[100], srv_dispo[20], srv_type[20];

                while (fscanf(servicesFile, "%d %s %s %d %s %s\n",
                              &srv_id, service_name, srv_desc, &service_cost, srv_dispo, srv_type) != EOF) {
                    if (service_id == srv_id) {
                        break;
                    }
                }
                fclose(servicesFile);
            }

            // Append the data to the TreeView's ListStore
            gtk_list_store_append(list_store, &iter);
            gtk_list_store_set(list_store, &iter,
                               0, id_citoyen,            // Citizen ID
                               1, nom_parking,           // Parking Name
                               2, date_reservation,      // Date
                               3, duration,              // Duration
                               4, number_places,         // Number of Places
                               5, service_name,          // Service Name
                               6, service_cost,          // Service Cost
                               -1);
        }
    }

    fclose(reservationsFile);

    // Set the model to the TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(list_store));

    // Unreference ListStore
    g_object_unref(list_store);
}

void setup_treeview10_columns(GtkWidget *treeview)
{
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // Column 1: ID Citizen
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID Citoyen", renderer, "text", 0, NULL);
    gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_FIXED);
    gtk_tree_view_column_set_fixed_width(column, 100); // Set a fixed width for GTK2
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Column 2: Parking Name
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Parking", renderer, "text", 1, NULL);
    gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_FIXED);
    gtk_tree_view_column_set_fixed_width(column, 150);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Column 3: Date
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Date", renderer, "text", 2, NULL);
    gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_FIXED);
    gtk_tree_view_column_set_fixed_width(column, 100);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Column 4: Duration
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Durée (h)", renderer, "text", 3, NULL);
    gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_FIXED);
    gtk_tree_view_column_set_fixed_width(column, 80);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Column 5: Number of Places
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Places", renderer, "text", 4, NULL);
    gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_FIXED);
    gtk_tree_view_column_set_fixed_width(column, 80);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Service", renderer, "text", 5, NULL);
    gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_FIXED);
    gtk_tree_view_column_set_fixed_width(column, 150);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Column 6: Service Cost
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Cost", renderer, "text", 6, NULL);
    gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_FIXED);
    gtk_tree_view_column_set_fixed_width(column, 80);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
}

void
on_buttonFiltrer_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *treeview, *spinbutton_month;
    static gboolean columns_setup = FALSE; // Flag to check if columns are already set
    int selected_month;

    // Locate widgets
    spinbutton_month = lookup_widget(GTK_WIDGET(button), "spinbuttonselectmonth");
    treeview = lookup_widget(GTK_WIDGET(button), "treeview10");

    // Get the selected month from the spinbutton
    selected_month = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_month));

    // Set up columns for the TreeView only once
    if (!columns_setup) {
        setup_treeview10_columns(treeview);
        columns_setup = TRUE;
    }

    setup_treeview10_columns(treeview);
    populate_treeview_reservations(treeview, selected_month);
}
void
on_button4calf_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *spinbutton_month, *label_total;
    char cin[20];
    int selected_month;
    float total_cost;
    char total_text[50];

    // Get logged-in CIN from session file
    FILE *session_file = fopen("session.txt", "r");
    if (!session_file || fscanf(session_file, "%s", cin) != 1) {
        printf("Error: Unable to read session file.\n");
        if (session_file) fclose(session_file);
        return;
    }
    fclose(session_file);

    // Locate widgets
    spinbutton_month = lookup_widget(GTK_WIDGET(button), "spinbuttonselectmonth");
    label_total = lookup_widget(GTK_WIDGET(button), "label341");

    // Get selected month
    selected_month = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_month));

    // Calculate total facture
    total_cost = calculateFacture_for_UI(cin, selected_month);

    // Update label
    snprintf(total_text, sizeof(total_text), "Total Facture: %.2f", total_cost);
    gtk_label_set_text(GTK_LABEL(label_total), total_text);
}


void
on_button4pdf_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
}
