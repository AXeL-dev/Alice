//=============================================================================
// Projet : Alice (Assistante)
// Fichier : resources.h
//
//=============================================================================

#define _WIN32_IE 0x0500 // Pour la structure NOTIFYICONDATA
#include <stdio.h>
#include <windows.h>
#include <commctrl.h> // -lcomctl32
#include <wininet.h>  // -lwininet
#include <richedit.h>
#define TTS_BALLOON 0x40 // Pour le tooltips
#define TTM_SETTITLE (WM_USER + 32)
#define TTI_INFO 1


//=============================================================================
//                               Constantes
//=============================================================================

#define ACTIVE_ANTI_VOYOU          1
#define NOM_APP                    "Alice (Assistante)"
#define NOM_SOLO                   "Alice"

#define APP_VERSION                " v 0.1"

#define DEFAULT_PASSWORD           "shiku"
#define CLE_CRYPTAGE               "Assistante"
#define MAX_PASSWORD               20

#define MY_WM_NOTIFYICON           WM_USER+1

#define L_FENETRE                  480
#define H_FENETRE                  400

#define PX_ECRAN                   (GetSystemMetrics(SM_CXSCREEN)/2) - (L_FENETRE/2)
#define PY_ECRAN                   (GetSystemMetrics(SM_CYSCREEN)/2) - (H_FENETRE/2)

/* Timers */
#define ID_TIMER_TIME              1

/* Icones/Bitmaps */
#define IDI_ALICE32                100
#define IDB_QUITTER                101
#define IDB_APROPOS                102
#define IDB_USER_INTERFACE         103
#define IDB_BILAN                  104
#define IDB_SCREENSHOT             105
#define IDB_RAPPEL                 106
#define IDB_CALCULATRICE           107
#define IDB_CONFIG                 108
#define IDI_EMAIL                  109
#define IDB_JOUR                   110
#define IDB_NUIT                   111
#define IDB_NUIT2                  112
#define IDI_INTEROGATION           113
#define IDB_GO                     114
#define IDB_PRECEDENT              115
#define IDB_SUIVANT                116
#define IDB_RECHERCHER             117
#define IDB_EXPLORER               118

/* Tray Menu */
#define IDM_USER_INTERFACE         200
#define IDM_QUITTER                201
#define IDM_APROPOS                202
#define IDM_CONFIG                 203
#define IDM_CALCULATRICE           204
#define IDM_RAPPEL                 205
#define IDM_SCREENSHOT             206
#define IDM_BILAN                  207
#define IDM_JOUROUNUIT             208

/* Apropos DialogBox */
#define BT_EMAIL                   210
#define TXT_NOM_APP                211

/* Config DialogBox */
#define TAB_CONTROL                212
#define IDC_ACTIVE_QUITTER         213
#define IDC_ACTIVE_PASSWORD        214
#define IDC_ACTIVE_ACCUEIL         215
#define TXT_1                      216
#define TXT_2                      217
#define TXT_3                      218
   /* Jour */
   #define IDE_JOURH               219
   #define BT_PLUS_JOURH           220
   #define BT_MOIN_JOURH           221
   #define TXT_JOURH               222
   #define IDE_JOURM               223
   #define BT_PLUS_JOURM           224
   #define BT_MOIN_JOURM           225
   #define TXT_JOURM               226
   /* Nuit */
   #define IDE_NUITH               227
   #define BT_PLUS_NUITH           228
   #define BT_MOIN_NUITH           229
   #define TXT_NUITH               230
   #define IDE_NUITM               231
   #define BT_PLUS_NUITM           232
   #define BT_MOIN_NUITM           233
   #define TXT_NUITM               234
   /* Sommeil */
   #define IDE_SOMMEILH            235
   #define BT_PLUS_SOMMEILH        236
   #define BT_MOIN_SOMMEILH        237
   #define TXT_SOMMEILH            238
   #define IDE_SOMMEILM            239
   #define BT_PLUS_SOMMEILM        240
   #define BT_MOIN_SOMMEILM        241
   #define TXT_SOMMEILM            242
#define IDC_ACTIVE_BILANDLG        243
#define TXT_MDP                    244
#define IDE_MDP                    245
#define IDC_AFFICHER_MDP           246
#define IDE_WHITE_LISTE            247
#define IDE_FENETRES_CIBLES        248

/* Password DialogBox */
#define IDE_PASSWORD               249

/* Bilan DialogBox */
#define IDE_RICHEDIT               250
#define BT_PRECEDENT               251
#define BT_SUIVANT                 252
#define IDE_RECHERCHER             253
#define BT_RECHERCHER              254
#define BT_EXPLORER                255
#define TXT_NOM_LOG                256

/* Rappel DialogBox */
#define BT_SUPPRIMER               257
#define LISTE_RAPPEL               258
#define BT_MODIFIER                259

/* NvRappel DialogBox */
#define IDE_JOUR                   260
#define IDE_MOIS                   261
#define IDE_ANNEE                  262
#define IDE_HEURE                  263
#define IDE_MINUTE                 264
#define IDE_RAPPEL                 265
#define BT_PLUS_JOUR               266
#define BT_MOIN_JOUR               267
#define BT_PLUS_MOIS               268
#define BT_MOIN_MOIS               269
#define BT_PLUS_ANNEE              270
#define BT_MOIN_ANNEE              271
#define BT_PLUS_HEURE              272
#define BT_MOIN_HEURE              273
#define BT_PLUS_MINUTE             274
#define BT_MOIN_MINUTE             275
#define IDC_FOR_EVER               276

#define MAX_SRAPPEL                50

//#define PATH_TO_LOG                "F:\\prv\\[new]\\[API]\\Alice\\logs"
//#define FICHIER_CONFIG             "F:\\prv\\[new]\\[API]\\Alice\\logs\\alice.cfg"
//#define FICHIER_RAPPEL             "F:\\prv\\[new]\\[API]\\Alice\\logs\\alice.rpl"
//#define WHITE_LISTE                "F:\\prv\\[new]\\[API]\\Alice\\logs\\alice.wlt"
//#define FICHIER_FCIBLE             "F:\\prv\\[new]\\[API]\\Alice\\logs\\alice.cib"

#define MAX_CHEMIN                 200
#define MAX_FENETRE_TAILLE         256
#define MAX_TMP                    512

//#define CALCULTRICE_EXE            "calculatrice.exe"
//#define SCREENSHOT_EXE             "impscreen.exe"
//#define WEBCAM_EXE                 "WebCamImageSave.exe"
//#define WEBSITEBLOCKER_EXE         "WebSiteBlocker.exe"
//#define CALCULTRICE_EXE            "D:\\prv\\[new]\\[API]\\Alice\\calculatrice.exe"
//#define SCREENSHOT_EXE             "D:\\prv\\[new]\\[API]\\Alice\\impscreen.exe"
//#define WEBCAM_EXE                 "D:\\prv\\[new]\\[API]\\Alice\\WebCamImageSave.exe"
//#define WEBSITEBLOCKER_EXE         "D:\\prv\\[new]\\[API]\\Alice\\WebSiteBlocker.exe"

#define DEFAULT_IMPR_QUALITY       75
#define MAX_IMPR_QUALITY           100

#define MINI_LEN_USER              4
#define MINI_LEN_PASS              4

//=============================================================================
//                           Variables Globales
//=============================================================================

HINSTANCE hInst;

NOTIFYICONDATA TrayIcon;

BOOL ACTIVE_MENU, ACTIVE_QUITTER, PASSWORD_OK, ACTIVE_PASSWORD, ACTIVE_WINDOW, ACTUALISER_CONFIG, ACTIVE_MSG_ACCUEIL, ACTIVE_BILAN_DLG, IS_DLG_RAPPEL, ACTIVE_MODIFIER_RAPPEL;

unsigned short jourHeure, jourMinute, nuitHeure, nuitMinute, sommeilHeure, sommeilMinute;

char dossier[MAX_CHEMIN];

char PASSWORD[MAX_PASSWORD], PATH_TO_LOG[MAX_CHEMIN], FICHIER_LOG[MAX_CHEMIN], FICHIER_CONFIG[MAX_CHEMIN], FICHIER_RAPPEL[MAX_CHEMIN], WHITE_LISTE[MAX_CHEMIN], FICHIER_FCIBLE[MAX_CHEMIN];

char CALCULTRICE_EXE[MAX_CHEMIN], SCREENSHOT_EXE[MAX_CHEMIN], WEBCAM_EXE[MAX_CHEMIN], WEBSITEBLOCKER_EXE[MAX_CHEMIN];

HANDLE hListView;

int lvItemActuel;

//=============================================================================
//                              Prototypes
//=============================================================================

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void afficherTrayIconBallon(char *titre, char *contenu, unsigned int temp, BOOL icone);
BOOL APIENTRY AproposDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL APIENTRY ConfigDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL APIENTRY PasswordDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL APIENTRY BilanDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL APIENTRY RappelDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL APIENTRY NvRappelDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL enregistrerDansLog(char *log, char *fichier, BOOL ecraserAncien);
BOOL lireApartirDeLog(LPVOID *hwnd, char *fichier, BOOL posBottom, BOOL colorier);
BOOL enregistrerEtLireLog(char *log, LPVOID *ptLParam, char *fichier, BOOL posBottom, BOOL ecraserAncien, BOOL colorier);
BOOL callProgram(char *programme, char *parametres);
BOOL screenShot(HWND hwnd);
void getMyDirectory(char *path);
BOOL afficherTab(HWND hwnd, unsigned short tab);
BOOL cacherTab(HWND hwnd, unsigned short tab);
BOOL chargerConfig();
BOOL sauvegarderConfig();
int colorierTexte(LPVOID *hwnd, char cDebut, char cFin, char *texte, int texteSize, COLORREF couleur);
int colorierCaractere(LPVOID *hwnd, char c, char *texte, int texteSize, COLORREF couleur);
BOOL explorerDossier(char *path, char *buffer, unsigned int *nbrDossier, BOOL compterDossier);
int rechercherTexteEdit(HANDLE *hEdit, HANDLE *hEditTexte);
void changerCouleurSelection(HANDLE *hEdit, CHARRANGE Selection, COLORREF couleur);
void creerListViewColone(HANDLE *handle, UINT largeurColone, char *nomColone, UINT idColone);
void ajouterALaListView(HANDLE *hListView, char *texte, int tailleTexte, BOOL modifier, int posModification);
BOOL verifierRappel(char *cheminFichier, char *rappel, BOOL activerRappel);
BOOL modifierRappel(HANDLE *hListView, char *cheminFichier, int itemPos, char *ancienRappel, char *nvRappel);
BOOL supprimerRappel(HANDLE *hListView, char *cheminFichier, int itemPos, BOOL jaiUneChaine, char *chaine);
BOOL lireEtAfficherRappel(HANDLE *hListView, char *cheminFichier);
short alphabet(char lettre);
void crypterVigenere(char *phrase, char *cle);
void decrypterVigenere(char *phrase, char *cle);
BOOL etatConnexion();
void recupererIdentifiants(LPVOID *hRichEdit, char *fenetre);
BOOL verifierWhiteListe(char *chaine);
BOOL rechercherChaineDansChaine(char *chaine1, char *chaine2);
int differenceDeTemp(int hrC, int minC, int hrP, int minP);

//=============================================================================
//                               Threads
//=============================================================================

DWORD  WINAPI Controleur(LPVOID lParam);
DWORD  WINAPI AntiVoyou(LPVOID lParam);
