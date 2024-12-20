
typedef struct{
	int jour;
	int mois;
	int annee;
} Date;
typedef struct
{
	char cin[20]; //==id
	char nom[20];
	char prenom[20];
	char sexe[20];
	Date birthDate;
	char numTel[20];
	char email[50];
	char adresse[50];
	char mdp[20];
    char governorat[20];
}citoyen;

void addCitoyen(citoyen c);
//void affichCit(GtkWidget *liste);
int modifCitoyen(const char *cin, citoyen nouv);
int deleteCitoyen(const char* cin);
void searchCitoyen( char * cin);
int verifyLogin(const char *log, const char *pw);
int verifySignup(citoyen c);
void calculateFacture(const char *cin, int moisChoisi) ;
void update_session(const char *cin);
float calculateFacture_for_UI(const char *cin, int moisChoisi);
