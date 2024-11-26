#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <gtk/gtk.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "tache4.h"
citoyen c;
char cinid[20];
int sexe,x;
void
on_buttonvalidinscrip_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
citoyen c;
GtkWidget *input_name, *input_pren, *input_cin, *input_password, *input_email, *input_num, *input_age, *fail_label, *succ_label;
// Lookup widgets
    input_name = lookup_widget(objet, "signup_name");
    input_pren = lookup_widget(objet, "signup_prename");
    input_cin = lookup_widget(objet, "signup_cin");
    input_password = lookup_widget(objet, "signup_password");
    input_email = lookup_widget(objet, "signup_email");
    input_num = lookup_widget(objet, "signup_num");
    input_adr = lookup_widget(objet, "signup_adr"); 
    input_pwd = lookup_widget(objet, "signup_password");
    fail_label = lookup_widget(objet, "signup_fail");
    succ_label = lookup_widget(objet, "signup_succ");
    // Retrieve text from entries
    strcpy(c.nom, gtk_entry_get_text(GTK_ENTRY(input_name)));
    strcpy(c.prenom, gtk_entry_get_text(GTK_ENTRY(input_pren)));
    strcpy(c.cin, gtk_entry_get_text(GTK_ENTRY(input_cin)));
    strcpy(c.email, gtk_entry_get_text(GTK_ENTRY(entry_email)));
    strcpy(c.numTel, gtk_entry_get_text(GTK_ENTRY(input_num)));
    strcpy(c.adresse, gtk_entry_get_text(GTK_ENTRY(input_adr)));
    strcpy(c.mdp, gtk_entry_get_text(GTK_ENTRY(entry_pwd)));
    // Retrieve selected gender
    if(sexe==1){
        strcpy(c.sexe,"Homme");
    }else{
        strcpy(c.sexe,"Femme");
    }
    c.birthDate.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_jour));
    c.birthDate.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_mois));
    c.birthDate.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_annee));
  // Check if CIN already exist
    FILE *f = fopen("accounts.txt", "r");
    citizen temp;
    int exist = 0;
    while (fscanf(f, "%s %s", temp.cin, temp.password) != EOF) {
        if (strcmp(c.cin, temp.cin) == 0) {
            exist = 1;
            break;
        }
    }
    fclose(f);
    if (exist) {
        // CIN already exist
        gtk_label_set_text(GTK_LABEL(fail_label), "CIN déjà utilisé !");
        gtk_widget_show(fail_label);
    } else {
        const char *password1 = gtk_entry_get_text(GTK_ENTRY(input_password));
        const char *password2 = gtk_entry_get_text(GTK_ENTRY(input_confirm_password));

if (strcmp(password1, password2) != 0) {
    gtk_label_set_text(GTK_LABEL(fail_label), "Les mots de passe ne correspondent pas !");
    gtk_widget_show(fail_label);
} else {
    // Proceed with password modification logic
}

        if(strcmp(c.nom,"")!=0&&strcmp(c.prenom,"")!=0&&strcmp(c.cin,"")!=0&&strcmp(c.sexe,"")!=0&& (c.age>=18) && (c.num!=0) && strcmp(c.email,"")!=0&&strcmp(c.poste,"")!=0){
        addCitoyen(c);
        gtk_widget_hide(fail_label);
        gtk_widget_show(succ_label);
        // Open EspaceCitoyen window
        espace_citoyen_window = create_espacecitoyen_window(); // Generated function by Glade
        gtk_widget_show(espace_citoyen_window);
    }
    }
}

void
on_buttonConnexion_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{

    GtkWidget *input_id, *input_password, *login_fail_label;
    char entered_id[20], entered_password[20];
    int is_valid = 0, is_admin = 0;

    // Lookup the widgets
    input_id = lookup_widget(GTK_WIDGET(button), "login_id");
    input_password = lookup_widget(GTK_WIDGET(button), "login_password");
    login_fail_label = lookup_widget(GTK_WIDGET(button), "login_fail");

    // Retrieve user input
    strcpy(entered_id, gtk_entry_get_text(GTK_ENTRY(input_id)));
    strcpy(entered_password, gtk_entry_get_text(GTK_ENTRY(input_password)));

    // Open files for validation
    FILE *admin_file = fopen("admin.txt", "r");
    FILE *citoyen_file = fopen("accounts.txt", "r");

    // Validate against admin credentials
    if (admin_file) {
        char admin_id[20], admin_password[20];
        while (fscanf(admin_file, "%s %s", admin_id, admin_password) != EOF) {
            if (strcmp(entered_id, admin_id) == 0 && strcmp(entered_password, admin_password) == 0) {
                is_valid = 1;
                is_admin = 1;
                break;
            }
        }
        fclose(admin_file);
    }

    // Validate against citoyen credentials if not admin
    if (!is_valid && citoyen_file) {
        citoyen temp;
        while (fscanf(citoyen_file, "%s %s %s %s %d %d %d %s %s %s %s\n",
                      temp.cin, temp.nom, temp.prenom, temp.sexe,
                      &temp.birthDate.jour, &temp.birthDate.mois, &temp.birthDate.annee,
                      temp.adresse, temp.numTel, temp.email, temp.mdp) != EOF) {
            if (strcmp(entered_id, temp.cin) == 0 && strcmp(entered_password, temp.mdp) == 0) {
                is_valid = 1;
                break;
            }
        }
        fclose(citoyen_file);
    }

    // Redirect based on validation result
    if (is_valid) {
        if (is_admin) {
            printf("Login successful as admin.\n");
            GtkWidget *admin_window = create_EspaceAdmin();
            gtk_widget_show(admin_window);
        } else {
            printf("Login successful as citoyen.\n");
            GtkWidget *citoyen_window = create_EspaceCitoyen();
            gtk_widget_show(citoyen_window);
        }
        GtkWidget *login_window = lookup_widget(GTK_WIDGET(button), "login_window");
        gtk_widget_destroy(login_window);
    } else {
        gtk_label_set_text(GTK_LABEL(login_fail_label), "Invalid ID or password. Please try again.");
    }

}
void
on_radiobuttonF4_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_RADIO_BUTTON(togglebutton)))
    sexe=1;
}


void
on_radiobuttonH4_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_RADIO_BUTTON(togglebutton)))
    sexe=2;
}
////////////////////
void
on_buttonAojutCit_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{

}

void
on_parametre_toggled                   (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data)
{

}


void
on_buttonDeconnecter_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_buttonSuppCpt_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window_show, *window_destroy;
window_destroy=lookup_widget(button,"w4espacecitoyen");
gtk_widget_destroy(window_destroy);
window_show=authentification  ();
gtk_widget_show (window_show);
}

//////////profil///////////////////
void
on_buttonGestionInfo_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window_show, *window_destroy;
window_destroy=lookup_widget(button,"w4espacecitoyen");
gtk_widget_destroy(window_destroy);
window_show=create_w4  ();
gtk_widget_show (window_show);
}


void
on_buttonGestionRev_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window_show, *window_destroy;
window_destroy=lookup_widget(button,"w4espacecitoyen");
gtk_widget_destroy(window_destroy);
window_show=create_w5  ();
gtk_widget_show (window_show);
}


void
on_buttonGestionAvis_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window_show, *window_destroy;
window_destroy=lookup_widget(button,"w4espacecitoyen");
gtk_widget_destroy(window_destroy);
window_show=create_w6  ();
gtk_widget_show (window_show);
}
//////////admin ///////////////
void
on_buttonListeCit_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window_show, *window_destroy;
window_destroy=lookup_widget(button,"create_w1espaceadmin");
gtk_widget_destroy(window_destroy);
window_show=create_w4listeCit ();
gtk_widget_show (window_show);
}


void
on_buttonGestionAg_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window_show, *window_destroy;
window_destroy=lookup_widget(button,"create_w1espaceadmin");
gtk_widget_destroy(window_destroy);
window_show=create_w2 ();
gtk_widget_show (window_show);
}


void
on_buttonGestionServ_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window_show, *window_destroy;
window_destroy=lookup_widget(button,"create_w1espaceadmin");
gtk_widget_destroy(window_destroy);
window_show=create_w3 ();
gtk_widget_show (window_show);
}


void
on_buttonGestionPark_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window_show, *window_destroy;
window_destroy=lookup_widget(button,"create_w1espaceadmin");
gtk_widget_destroy(window_destroy);
window_show=create_w1 ();
gtk_widget_show (window_show);
}

on_button4modifphoto_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{

}

////////facture//////////////
void
on_buttonFiltrer_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button4calf_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_buttonPDF_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

}