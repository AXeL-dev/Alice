//=============================================================================
// Projet : Alice (Assistante)
// Version : 0.1
// Fichier : main.c
// Auteur : AXeL
// Date de création : 11/11/2013
// Date de la dernière modification : 17/02/2014
// Idées d'amélioration :
//                        - Au lancement empécher le gestionnaire de tâches de 
//                          fermer le programme (en le supprimant de System32, ...)
//                        - Connexion à la boite email + lecture des nvs messages
//                          (? "Imap" : "Pop3")
//                        - Gestion du language arabe dans le RichEdit
//                        - Meilleur protection des fichiers logs (car SYSTEM & HIDDEN
//                          ne sert a rien avec la majorité des fonctions comme
//                          CreateDirectory(), openfile(), ...
//                        - Changer le curseur (voir dossier Windows pour les curseurs)
//                        - Gérer les erreurs dû au mode administrateur
//                        - Diminuer l'appel de la fonction GetDlgItem() chaque fois
//                          (surtout dans les DialogBox) en l'associant a un handle au début
//                          , après on utilisera ce handle au lieu de réappeler cette fonction
//                        - Gérer les conflits de minutes dans la configuration des horaires
//                        - Ajouter otpion d'horloge parlante
//                        - Gestion des dates/heures dépassés lors de la création d'un rappel
//                        - Envoie des logs par mail en cas de vol ou cas similaire
//                        - Ajout de la possibilité d'effacer automatiquement les logs datant
//                          d'un mois ou plus
//                        - Eteindre/mettre en veille l'ordi. à une condition ou à un temp
//                          donné
//                        - Copier/absorber les données des clés usb inconnus ;)
//                        - Optimiser l'algorithme de coloration du bilan
//                        - Pouvoir jouer du son avec les rappels
//                        - Activer caméra si désactivée (simuler l'appui sur Fn + F10)
// Mise à jour :
//                - 25/01/2014 (v 0.1):
//                   - Changement du nom de l'application de "IA" à "Alice"
//                - 26/01/2014:
//                   - Gestion de la multi instance
//                   - Création des threads et fonctions
//                   - version BETA 0.1 fini ! (ou presque)
//                - 27/01/2014:
//                   - Mineurs modifications par içi et par là
//                   - Modification des options de la configuration
//                - 28/01/2014:
//                   - Gestion des conflits entre les heures dans la configuration
//                     (Jour, Nuit, Sommeil)
//                   - Ajout des fonctions chargerConfig() et sauvegarderConfig()
//                   - Ajout de la coloration du texte dans le RichEdit
//                - 29/01/2014:
//                   - Ajout d'une interface graphique pour 'Bilan'
//                   - Réglage d'une petite lacune au niveau des variables static
//                     (ne jamais initialisé une var. static à sa déclaration
//                     car elle peut ne pas se rénitialiser, ex: après un 2ème
//                     appel de la fonction qui la contient la var. ne va pas
//                     se rénitialiser)
//                - 30/01/2014:
//                   - Modification du nom de l'éxé d' "Alice" à "winmanager" pour
//                     que le processus n'aie pas le même nom que celui de la fenêtre
//                   - Reconfiguration du timer en charge du changement du temp
//                   - Suppression de la fonction recupererTemp(), on s'en sert plus
//                   - Ajout d'une interface graphique pour 'Rappel' (Enfin ! :D)
//                   - Ajout de la possibilité de changer le mot de passe depuis
//                     la configuration
//                - 31/01/2014:
//                   - Finalisation de l'interface graphique de 'Rappel'
//                - 01/02/2014:
//                   - Les rappels sont maintenant mis au point (à marche)
//                   - Gestion des erreurs de l'inéxistence du dossier "logs"
//                   - PATH_TO_LOG contient maintenant l'emplacement actuel du programme
//                     non pas celui par défaut (grace à la fct. getMyDirectory())
//                   - Ajout du thread AntiVoyou() et comme son nom l'indique il
//                     enregistre les identifiants des inconnus/voyous
//                   - Ajout des tabs 4 et 5 dans 'Configuration'
//                   - version BETA 0.1 fermé (closed !) à 17:13 (enfin !)
//                - 02/02/2014:
//                   - Sauvegarde des rappels activés dans le fichier log
//                   - Quelques optimisations au niveau du thread AntiVoyou()
//                   - Maintenant la GUI de 'Bilan' commence par afficher le fichier
//                     log d'aujourd'hui
//                - 03/02/2014: (Une journée à chier :/)
//                   - Ajout de la possibilité de répéter les rappels automatiquement
//                     + gestion de quelques erreurs au niveau de la fonction
//                     verifierRappel()
//                   - Ajout de la possibilité de modifier un rappel
//                - 05/02/2014:
//                   - Réglage du décalage du temp après mise en veille de l'ordi.
//                - 06/02/2014:
//                   - Maintenant l'AntiVoyou capture les identifiants passés par
//                     copier/coller (CTRL+C => CTRL+V)
//                - 16/02/2014:
//                   - Modification du nom de la focntion capturerEcran() à callProgram()
//                     car elle ne sert qu'à appeler un programme en lui passant des paramétres
//                     en ligne de commandes
//                   - Maintenant l'AntiVoyou capture la photo de l'intru/voyou à l'aide
//                     de la webcam mais il faut toujours que cette dernière soit activée
//                   - Ajout du chemin WEBCAM_EXE (chemin du programme WebCamImageSave)
//                - 17/02/2014:
//                   - Ajout du chemin WEBSITEBLOCKER_EXE
//                   - Ajout du Blockage/débloquage des sites web en utilisant le programme
//                     WebSiteBlocker, par contre vu que je n'ai pas trouvé assez de place
//                     pour mettre un boutton sur la GUI, j'ai utilisé le boutton JOUROUNUIT
//                     qui n'avait pas encore d'utilité (c'est juste temporaire et puis c'est
//                     de la classe nn ? un boutton misterieux hein :p)
// Beugs/lacunes connus :
//                   - Parfois l'AntiVoyou peut récupérer n'importe koi ><
//                   - Dans certaines circonstances les rappels peuvent ne pas
//                     s'afficher (ex : s'ils ne sont décalés que par une minute
//                     de leurs précédents)
//
//=============================================================================

#include "resources.h"


//=============================================================================
//                          Fonction principale
//=============================================================================

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrecedente, LPSTR lpCmdLine, int nCmdShow)
{
   /* Gestion de double instance */
   HANDLE hMutex = CreateMutex (NULL, FALSE, NOM_SOLO); /* Création d'instance */
   if (GetLastError() == ERROR_ALREADY_EXISTS) /* Si double instance */
      return 0;
      
   HINSTANCE hInstRich32;
   HWND hwnd;
   MSG msg;
   WNDCLASS wc;
   
   InitCommonControls(); /* Initialisation des contrôles communs */
    
   hInstRich32 = LoadLibrary("RICHED32.DLL"); /* Chargement de la DLL pour les Rich Edit */
   
   if(!hInstRich32)
   {
      MessageBox(NULL, "Impossible de charger RICHEDIT32.DLL !", NOM_APP, MB_OK | MB_ICONWARNING);
      return 0;	
   }
	
   hInst = hInstance;
   
   wc.style = 0; 
   wc.lpfnWndProc = MainWndProc; 
   wc.cbClsExtra = 0; 
   wc.cbWndExtra = 0; 
   wc.hInstance = hInstance; 
   wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ALICE32)); 
   wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
   wc.hbrBackground = (HBRUSH)(1 + COLOR_BTNFACE); 
   wc.lpszMenuName =  NULL; 
   wc.lpszClassName = "MaWinClass";
   
   if(!RegisterClass(&wc)) return FALSE;
   
   /* Création de la fenêtre */
   hwnd = CreateWindow("MaWinClass", NOM_APP, WS_POPUP | WS_BORDER | WS_CAPTION 
                                     | WS_MINIMIZEBOX | WS_SYSMENU | WS_OVERLAPPED, 
                                     PX_ECRAN, PY_ECRAN, L_FENETRE, H_FENETRE
                                     , NULL, NULL, hInstance, NULL);
   if (!hwnd) return FALSE;
   
   //ShowWindow(hwnd, nCmdShow); /* Affichage de la fenêtre après création */
   //UpdateWindow(hwnd); 
   
   while (GetMessage(&msg, NULL, 0, 0)) 
   { 
       TranslateMessage(&msg); 
       DispatchMessage(&msg); 
   } 
   
   /* Libération de la DLL pour les Rich Edit */
   if(!FreeLibrary (hInstRich32))
	   MessageBox(NULL, "Bibliothèque RICHEDIT non libérée !", NOM_APP, MB_OK | MB_ICONWARNING);
   
   return msg.wParam;
}

//=============================================================================
//                     Procédure de la fonction principale
//=============================================================================

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static BOOL REFRESH_ICONES, SET_TIMER_TIME;
    static HWND hJourOuNuit;
    static HWND hTTip;
    static TOOLINFO ti;
    static RECT rect;
    static HANDLE hThread[2];
    static DWORD dwThreadId[2];
    static HWND hRichEdit;
    static SYSTEMTIME Time;
    static char info1[] = "Alice";
    static char info2[] = "vous assiste ! :p";
    static char version[] = APP_VERSION;
    static char jourL[3];
    static char jour[2];
    static char mois[2];
    static char annee[4];
    static char heure[2];
    static char minutes[2];
    static char dateHeureRappel[16];
    
    switch (uMsg)
    {
        case WM_CREATE:
             /* Création de l'icone de notification */
             TrayIcon.cbSize = sizeof(NOTIFYICONDATA); 
             TrayIcon.hWnd = hwnd; 
             TrayIcon.uID = 0; 
             TrayIcon.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ALICE32)); 
             TrayIcon.uCallbackMessage = MY_WM_NOTIFYICON; 
             TrayIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO; 
             lstrcpy(TrayIcon.szTip, NOM_APP); 
             Shell_NotifyIcon(NIM_ADD, &TrayIcon);
             /* Création du RichEdit (!@! pour mettre une bordure => ES_SUNKEN) */
             hRichEdit = CreateWindowEx(WS_EX_CLIENTEDGE, RICHEDIT_CLASS, ""
             , WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_DISABLENOSCROLL | WS_VSCROLL | ES_READONLY
             , 10, 120, 280, 240, hwnd, NULL, hInst, NULL);
             /* Création de l'image d'Alice */
             HWND hAliceImage = CreateWindow("STATIC", "IDB_ALICE96"
             , WS_CHILD | WS_VISIBLE | SS_BITMAP
             ,10, 10, 0, 0, hwnd, NULL, hInst, NULL);
             /* Création d'un groupbox */
             HWND hGroupBox = CreateWindow("BUTTON", ""
             , WS_CHILD | WS_VISIBLE | BS_GROUPBOX
             , 310, 20, 150, 100, hwnd, NULL, hInst, NULL);
             /* Création du bitmap date (image) */
             HWND hBmpDate = CreateWindow("STATIC", "IDB_DATE"
             , WS_CHILD | WS_VISIBLE | SS_BITMAP
             ,320, 40, 0, 0, hwnd, NULL, hInst, NULL);
             /* Création du bitmap heure */
             HWND hBmpHeure = CreateWindow("STATIC", "IDB_HEURE"
             , WS_CHILD | WS_VISIBLE | SS_BITMAP
             ,320, 80, 0, 0, hwnd, NULL, hInst, NULL);
             /* Création du button jourOuNuit */
             hJourOuNuit = CreateWindow("BUTTON", " "
             , WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BOTTOM
             , 410, 77, 36, 24, hwnd, (HMENU)IDM_JOUROUNUIT, hInst, NULL);
             /* Création d'un tooltips (qui affiche des bulbes) */
             hTTip = CreateWindow(TOOLTIPS_CLASS, NULL 
             , WS_POPUP | TTS_NOPREFIX | TTS_BALLOON 
             , CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT 
             , hwnd, NULL, hInst, NULL);
             
             SendMessage(hTTip, TTM_SETTITLE, TTI_INFO, (LPARAM)NOM_SOLO); /* Titre des infos bulbes */
             
             GetClientRect (hwnd, &rect);
             ti.cbSize = sizeof(TOOLINFO);
             ti.uFlags = TTF_SUBCLASS;
             ti.hwnd = hJourOuNuit; /* Pour l'instant il y'a que ce boutton qui en bénéfice */
             ti.hinst = hInst;
             ti.uId = 0;
             ti.rect.left = rect.left; 
             ti.rect.top = rect.top; 
             ti.rect.right = rect.right; 
             ti.rect.bottom = rect.bottom;
             /* Création des buttons d'options */
                /* (1) À propos */
                HWND hApropos = CreateWindow("BUTTON", " À propos"
                , WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
                , 310, 330, 150, 30, hwnd, (HMENU)IDM_APROPOS, hInst, NULL);
                /* Bitmap À propos */
                SendMessage(hApropos, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP
                , (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_APROPOS)));
                /* (2) Configuration */
                HWND hConfig = CreateWindow("BUTTON", " Configuration"
                , WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
                , 310, 290, 150, 30, hwnd, (HMENU)IDM_CONFIG, hInst, NULL);
                /* Bitmap Configuration */
                SendMessage(hConfig, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP
                , (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CONFIG)));
                /* (3) Calculatrice */
                HWND hCalculatrice = CreateWindow("BUTTON", " Calculatrice"
                , WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
                , 310, 250, 150, 30, hwnd, (HMENU)IDM_CALCULATRICE, hInst, NULL);
                /* Bitmap Calculatrice */
                SendMessage(hCalculatrice, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP
                , (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CALCULATRICE)));
                /* (4) Rappel */
                HWND hRappel = CreateWindow("BUTTON", " Rappel"
                , WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
                , 310, 210, 150, 30, hwnd, (HMENU)IDM_RAPPEL, hInst, NULL);
                /* Bitmap Rappel */
                SendMessage(hRappel, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP
                , (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RAPPEL)));
                /* (5) Screenshot */
                HWND hScreenshot = CreateWindow("BUTTON", " Screenshot"
                , WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
                , 310, 170, 150, 30, hwnd, (HMENU)IDM_SCREENSHOT, hInst, NULL);
                /* Bitmap Screenshot */
                SendMessage(hScreenshot, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP
                , (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SCREENSHOT)));
                /* (6) Bilan */
                HWND hBilan = CreateWindow("BUTTON", " Bilan"
                , WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
                , 310, 130, 150, 30, hwnd, (HMENU)IDM_BILAN, hInst, NULL);
                /* Bitmap Bilan */
                SendMessage(hBilan, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP
                , (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BILAN)));
             /* Police des buttons d'options */
             HFONT policeButton = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Forte");
             SendMessage(hApropos, WM_SETFONT, (long)policeButton, 0);
             SendMessage(hConfig, WM_SETFONT, (long)policeButton, 0);
             SendMessage(hCalculatrice, WM_SETFONT, (long)policeButton, 0);
             SendMessage(hRappel, WM_SETFONT, (long)policeButton, 0);
             SendMessage(hScreenshot, WM_SETFONT, (long)policeButton, 0);
             SendMessage(hBilan, WM_SETFONT, (long)policeButton, 0);
             /* Récupération du chemin du programme + ses fichiers */
             getMyDirectory(PATH_TO_LOG);
                /* chemins des programmes nécessaires au fonctionnement */
             sprintf(CALCULTRICE_EXE, "%s\\calculatrice.exe", PATH_TO_LOG);
             sprintf(SCREENSHOT_EXE, "%s\\impscreen.exe", PATH_TO_LOG);
             sprintf(WEBCAM_EXE, "%s\\WebCamImageSave.exe", PATH_TO_LOG);
             sprintf(WEBSITEBLOCKER_EXE, "%s\\WebSiteBlocker.exe", PATH_TO_LOG);
                /* Chemin du dossier qui contient les logs */
             lstrcat(PATH_TO_LOG, "\\logs");
             sprintf(FICHIER_CONFIG, "%s\\alice.cfg", PATH_TO_LOG); /* Chemin du fichier de configuration */
             sprintf(FICHIER_RAPPEL, "%s\\alice.rpl", PATH_TO_LOG); /* Chemin du fichier des rappels */
             sprintf(WHITE_LISTE, "%s\\alice.wlt", PATH_TO_LOG); /* Chemin de la white liste */
             sprintf(FICHIER_FCIBLE, "%s\\alice.cib", PATH_TO_LOG); /* Chemin du fichier qui contient les fenêtres ciblés */
             /* Chargement des paramétres (de configuration et par défaut) */
             REFRESH_ICONES = TRUE;
             SET_TIMER_TIME = TRUE;
             ACTUALISER_CONFIG = FALSE;
             ACTIVE_MENU = TRUE;
             ACTIVE_WINDOW = FALSE;
             IS_DLG_RAPPEL = FALSE;
             chargerConfig();
             /* Configuration du timer du temp || TIMER_TIME */
             GetSystemTime(&Time); /* On récupère le temp (Time est déclaré tout en haut en étant static) */
             SetTimer(hwnd, ID_TIMER_TIME, (60-Time.wSecond)*1000, NULL); /* On active le timer (il se déclenchera à la minute suivante) */
             /* Création/Lancement des Threads */
             hThread[0] = CreateThread(NULL, 0, Controleur, hRichEdit, 0, &dwThreadId[0]);
             if (ACTIVE_ANTI_VOYOU)
                hThread[1] = CreateThread(NULL, 0, AntiVoyou, hRichEdit, 0, &dwThreadId[1]);
             break;
        case WM_TIMER:
             if(wParam == ID_TIMER_TIME) /* Si il est temp de changer l'heure */
             {
                if (SET_TIMER_TIME)
                {
                   SetTimer(hwnd, ID_TIMER_TIME, 60000, NULL); /* On reconfigure une dernière fois le timer sur 1 minute (60 secondes) */
                   SET_TIMER_TIME = FALSE;
                }
                GetSystemTime(&Time); /* On récupère le temp */
                /* Si la différence est + qu'une minute (à cause d'une mise en veille par exemple) */
                if (differenceDeTemp(Time.wHour, Time.wMinute, strtol(heure, NULL, 10), strtol(minutes, NULL, 10)) > 1)
                {
                   SetTimer(hwnd, ID_TIMER_TIME, (60-Time.wSecond)*1000, NULL);
                   SET_TIMER_TIME = TRUE;
                }
                REFRESH_ICONES = TRUE; /* On indique qu'on doit redessiner les icones (dans WM_PAINT) */
                InvalidateRect(hwnd, NULL, TRUE); /* On force le redessinage de l'écran */
                sprintf(dateHeureRappel, "%02d/%02d/%d %02d:%02d", Time.wDay, Time.wMonth, Time.wYear, Time.wHour, Time.wMinute);
                if (!verifierRappel(FICHIER_RAPPEL, dateHeureRappel, TRUE)) /* Si rappel trouvé et activé */
                   lireApartirDeLog((LPVOID)&hRichEdit, FICHIER_LOG, TRUE, TRUE); /* On raffraichis l'écran du RichEdit */
             }
             break;
        case WM_PAINT: /* Affichage/dessinage de la date/heure, ... */
             {
                HFONT NewFont, OldFont;
                LOGFONT lf;
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                SetBkMode(hdc, TRANSPARENT);
                
                /* Info1 */
                ZeroMemory(&lf, sizeof(LOGFONT));
                lstrcpy(lf.lfFaceName, "Forte");
                lf.lfHeight = 38;
                NewFont = CreateFontIndirect(&lf);
                OldFont = SelectObject(hdc, NewFont);
                SetTextColor(hdc, RGB(3, 19, 53));
                TextOut(hdc, 145, 30, info1, strlen(info1));
                
                /* Info2 */
                lf.lfHeight = 26;
                NewFont = CreateFontIndirect(&lf);
                OldFont = SelectObject(hdc, NewFont);
                SetTextColor(hdc, RGB(225, 55, 56));
                TextOut(hdc, 110, 65, info2, strlen(info2));
                
                /* Version */
                lstrcpy(lf.lfFaceName, "Eras Demi ITC");
                lf.lfHeight = 14;
                NewFont = CreateFontIndirect(&lf);
                OldFont = SelectObject(hdc, NewFont);
                SetTextColor(hdc, RGB (0, 0, 0));
                TextOut(hdc, 430, 5, version, strlen(version));
                
                /* On classe la date et l'heure dans leur variables (variables déclarés au début de la procédure) */
                switch (Time.wDayOfWeek)
                {
                   case 0: sprintf(jourL, "Dim"); break;
                   case 1: sprintf(jourL, "Lun"); break;
                   case 2: sprintf(jourL, "Mar"); break;
                   case 3: sprintf(jourL, "Mer"); break;
                   case 4: sprintf(jourL, "Jeu"); break;
                   case 5: sprintf(jourL, "Ven"); break;
                   case 6: sprintf(jourL, "Sam"); break;
                }
                sprintf(jour, "%02d", Time.wDay);
                sprintf(mois, "%02d", Time.wMonth);
                sprintf(annee, "%d", Time.wYear);
                sprintf(heure, "%02d", Time.wHour);
                sprintf(minutes, "%02d", Time.wMinute);
                
                /* Date (jourL, jour, mois, annee) */
                lf.lfHeight = 15;
                NewFont = CreateFontIndirect(&lf);
                OldFont = SelectObject(hdc, NewFont);
                SetTextColor(hdc, RGB (225, 55, 56));
                TextOut(hdc, 350, 40, jourL, strlen(jourL));
                TextOut(hdc, 397, 40, ".", 1);
                TextOut(hdc, 416, 40, ".", 1);
                TextOut(hdc, 369, 80, ":", 1);
                SetTextColor(hdc, RGB (0, 0, 0));
                TextOut(hdc, 340, 40, ":", 1);
                TextOut(hdc, 380, 40, jour, strlen(jour));
                TextOut(hdc, 400, 40, mois, strlen(mois));
                TextOut(hdc, 420, 40, annee, strlen(annee));
                
                /* Heure (heure, minutes) */
                TextOut(hdc, 340, 80, ":", 1);
                TextOut(hdc, 350, 80, heure, strlen(heure));
                TextOut(hdc, 374, 80, minutes, strlen(minutes));
                   
                SelectObject(hdc, OldFont);
                DeleteObject(NewFont);
                EndPaint(hwnd, &ps);
                   
                /* On raffraichis/change les icones */
                if (REFRESH_ICONES || ACTUALISER_CONFIG)
                {
                   int heureChiffres = strtol(heure, NULL, 10); /* conversion de la chaine en entier */
                   int minuteChiffres = strtol(minutes, NULL, 10);
                   /* Si (heure == jourH et minute >= jourM) ou (heure > jourH et heure < nuitH) ou (heure == nuitH et minute < nuitM) */
                   if ((heureChiffres == jourHeure && minuteChiffres >= jourMinute) || (heureChiffres > jourHeure && heureChiffres < nuitHeure) || (heureChiffres == nuitHeure && minuteChiffres < nuitMinute))
                   {
                      /* Bitmap jour */
                      SendMessage(hJourOuNuit, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP
                      , (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_JOUR)));
                      ti.lpszText = "Fait-il beau dehors ?"; 
                      SendMessage(hTTip, TTM_ADDTOOL, 0, (LPARAM) &ti);
                   }
                   /* Si nn si (heure == nuitH et minute >= nuitM) ou (heure > nuitH et heure != sommeilH) ou (heure == sommeilH et minute < sommeilM) */
                   else if ((heureChiffres == nuitHeure && minuteChiffres >= nuitMinute) || (heureChiffres > nuitHeure && heureChiffres != sommeilHeure) || (heureChiffres == sommeilHeure && minuteChiffres < sommeilMinute))
                   {
                      /* Bitmap nuit */
                      SendMessage(hJourOuNuit, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP
                      , (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NUIT)));
                      ti.lpszText = "Y'a t'il des étoiles dans le ciel ?";
                      SendMessage(hTTip, TTM_ADDTOOL, 0, (LPARAM) &ti);
                   }
                   else /* Si nn */
                   {
                      /* Bitmap nuit2 (sommeil) */
                      SendMessage(hJourOuNuit, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP
                      , (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NUIT2)));
                      ti.lpszText = "C'est trop tard, il faut aller se coucher !";
                      SendMessage(hTTip, TTM_ADDTOOL, 0, (LPARAM) &ti);
                   }
                   REFRESH_ICONES = FALSE;
                }
             }
             break;
        case WM_CLOSE:
             ACTIVE_WINDOW = FALSE;
             PASSWORD_OK = FALSE;
             ShowWindow(hwnd, SW_HIDE); /* On cache la fenêtre */
             break;
        case MY_WM_NOTIFYICON:
             switch (lParam)
             {
                 case WM_LBUTTONUP:
                      if (!ACTIVE_WINDOW)
                      {
                         ACTIVE_WINDOW = TRUE;
                         if (ACTIVE_PASSWORD)
                         {
                            PASSWORD_OK = FALSE;
                            ACTIVE_MENU = FALSE;
                            DialogBox(hInst, "DLGPASSWORD", hwnd, (DLGPROC)PasswordDlgProc);
                            if (!PASSWORD_OK) /* Si faux mot de passe */
                            {
                               ACTIVE_WINDOW = FALSE;
                               break;
                            }
                         }
                         ShowWindow(hwnd, SW_SHOW & SW_MAXIMIZE); /* Affichage de la fenêtre */
                      }
                      break;
                 case WM_RBUTTONUP:
                      if (ACTIVE_MENU)
                      {
                         /* Création du menu de notification */
                         HMENU hmenu; 
                         HMENU hpopup; 
                         POINT pos; 
                         GetCursorPos(&pos); 
                         hmenu = LoadMenu(hInst, "TRAYMENU"); 
                         hpopup = GetSubMenu(hmenu, 0);
                         /* Affichage des images bitmap */
                            /* IDM_QUITTER */
                            SetMenuItemBitmaps(hpopup, IDM_QUITTER, MF_BYCOMMAND
                            , LoadBitmap(hInst, MAKEINTRESOURCE(IDB_QUITTER)) /* Bitmap si item non sélectionné */
                            , NULL);                                         /* Bitmap si item sélectionné */
                            /* IDM_APROPOS */
                            SetMenuItemBitmaps(hpopup, IDM_APROPOS, MF_BYCOMMAND
                            , LoadBitmap(hInst, MAKEINTRESOURCE(IDB_APROPOS)), NULL);
                            /* IDM_CONFIG */
                            SetMenuItemBitmaps(hpopup, IDM_CONFIG, MF_BYCOMMAND
                            , LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CONFIG)), NULL);
                            /* IDM_CALCULATRICE */
                            SetMenuItemBitmaps(hpopup, IDM_CALCULATRICE, MF_BYCOMMAND
                            , LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CALCULATRICE)), NULL);
                            /* IDM_RAPPEL */
                            SetMenuItemBitmaps(hpopup, IDM_RAPPEL, MF_BYCOMMAND
                            , LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RAPPEL)), NULL);
                            /* IDM_SCREENSHOT */
                            SetMenuItemBitmaps(hpopup, IDM_SCREENSHOT, MF_BYCOMMAND
                            , LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SCREENSHOT)), NULL);
                            /* IDM_BILAN */
                            SetMenuItemBitmaps(hpopup, IDM_BILAN, MF_BYCOMMAND
                            , LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BILAN)), NULL);
                            /* IDM_USER_INTERFACE */
                            SetMenuItemBitmaps(hpopup, IDM_USER_INTERFACE, MF_BYCOMMAND
                            , LoadBitmap(hInst, MAKEINTRESOURCE(IDB_USER_INTERFACE)), NULL);
                         /* Affichage du menu */
                         SetMenuDefaultItem(hpopup, IDM_USER_INTERFACE, FALSE); /* Menu item par défaut */
                         if (!ACTIVE_QUITTER)
                            EnableMenuItem(hpopup, IDM_QUITTER, MF_GRAYED); /* On met en gris le boutton quitter */
                         //HiliteMenuItem(hwnd, hpopup, IDM_USER_INTERFACE, MF_BYCOMMAND | MF_HILITE); /* Item séléctionné à l'ouverture du menu */
                         SetForegroundWindow(hwnd); 
                         TrackPopupMenuEx(hpopup, 0, pos.x, pos.y, hwnd, NULL);               
                         DestroyMenu(hmenu);
                      }
                      break;
             }
             break;
        case WM_COMMAND:
             switch (LOWORD(wParam))
             {
                 case IDM_JOUROUNUIT:
                      /* Vu que je n'ai pas trouvé quoi mettre içi, je vais mettre quelque chose temporairement */
                      ShellExecute(NULL, "open", WEBSITEBLOCKER_EXE, NULL, NULL, SW_SHOWNORMAL);
                      break;
                 case IDM_USER_INTERFACE:
                      if (!ACTIVE_WINDOW)
                      {
                         ACTIVE_WINDOW = TRUE;
                         if (ACTIVE_PASSWORD)
                         {
                            PASSWORD_OK = FALSE;
                            ACTIVE_MENU = FALSE;
                            DialogBox(hInst, "DLGPASSWORD", hwnd, (DLGPROC)PasswordDlgProc);
                            if (!PASSWORD_OK) /* Si faux mot de passe */
                            {
                               ACTIVE_WINDOW = FALSE;
                               break;
                            }
                         }
                         ShowWindow(hwnd, SW_SHOW & SW_MAXIMIZE); /* Affichage de la fenêtre */
                      }
                      break;
                 case IDM_BILAN:
                      //PASSWORD_OK = FALSE;
                      ACTIVE_MENU = FALSE;
                      if (!PASSWORD_OK)
                         DialogBox(hInst, "DLGPASSWORD", hwnd, (DLGPROC)PasswordDlgProc);
                      if (PASSWORD_OK)
                      {
                         if (ACTIVE_BILAN_DLG)
                         {
                            ACTIVE_MENU = FALSE;
                            DialogBox(hInst, "BILAN", hwnd, (DLGPROC)BilanDlgProc);
                            if (!ACTIVE_WINDOW)
                               PASSWORD_OK = FALSE;
                         }
                         else
                            ShellExecute(NULL, "open", PATH_TO_LOG, NULL, NULL, SW_SHOWNORMAL);
                      }
                      break;
                 case IDM_SCREENSHOT:
                      ACTIVE_MENU = FALSE;
                      screenShot(hwnd);
                      ACTIVE_MENU = TRUE;
                      break;
                 case IDM_RAPPEL:
                      //PASSWORD_OK = FALSE;
                      ACTIVE_MENU = FALSE;
                      if (!PASSWORD_OK)
                         DialogBox(hInst, "DLGPASSWORD", hwnd, (DLGPROC)PasswordDlgProc);
                      if (PASSWORD_OK)
                      {
                         ACTIVE_MENU = FALSE;
                         DialogBox(hInst, "RAPPEL", hwnd, (DLGPROC)RappelDlgProc);
                         if (!ACTIVE_WINDOW)
                            PASSWORD_OK = FALSE;
                      }
                      break;
                 case IDM_CALCULATRICE:
                      ShellExecute(NULL, "open", CALCULTRICE_EXE, NULL, NULL, SW_SHOWNORMAL);
                      break;
                 case IDM_CONFIG:
                      //PASSWORD_OK = FALSE;
                      ACTIVE_MENU = FALSE;
                      if (!PASSWORD_OK)
                         DialogBox(hInst, "DLGPASSWORD", hwnd, (DLGPROC)PasswordDlgProc);
                      if (PASSWORD_OK)
                      {
                         ACTIVE_MENU = FALSE;
                         DialogBox(hInst, "CONFIG", hwnd, (DLGPROC)ConfigDlgProc);
                         if (ACTUALISER_CONFIG)
                            InvalidateRect(hwnd, NULL, TRUE); /* On force le redessinage de l'ecran */
                         if (!ACTIVE_WINDOW)
                            PASSWORD_OK = FALSE;
                      }
                      break;
                 case IDM_APROPOS:
                      ACTIVE_MENU = FALSE;
                      DialogBox(hInst, "APROPOS", hwnd, (DLGPROC)AproposDlgProc);
                      break;
                 case IDM_QUITTER:
                      DestroyWindow(hwnd);
                      break;
             }
             break;
        case WM_DESTROY:
             KillTimer(hwnd, ID_TIMER_TIME); /* On arrête le timer du temp */
             CloseHandle(hThread[0]); /* On arrête le 1er thread */
             if (ACTIVE_ANTI_VOYOU)
                CloseHandle(hThread[1]); /* On arrête le 2ème thread */
             Shell_NotifyIcon(NIM_DELETE, &TrayIcon); /* Suppression de la notification */
             PostQuitMessage(0);
             break;
        default:
             return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

//=============================================================================
//                             AproposDialogBox
//=============================================================================

BOOL APIENTRY AproposDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg)
   {
       case WM_INITDIALOG:
            /* Affichage de l'icone */
            SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ALICE32)));
            /* Affichage de l'email */
            SendDlgItemMessage(hDlg, BT_EMAIL, BM_SETIMAGE, (WPARAM)IMAGE_ICON
            , (LPARAM)LoadImage(hInst, MAKEINTRESOURCE(IDI_EMAIL), IMAGE_ICON, 20, 16, 0));
            /* Application du fond pour TXT_NOM_APP */
            HFONT PoliceTahoma = CreateFont(24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Forte");
            SendMessage(GetDlgItem(hDlg, TXT_NOM_APP), WM_SETFONT, (long)PoliceTahoma, 0);
            break;
       case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                /* Gestion des clics buttons (OK - FERMER) */
                case BT_EMAIL:
                     ShellExecute(NULL, "open", "https://mail.google.com/", NULL, NULL, SW_SHOWNORMAL);
                     break;
                case IDOK:
                case IDCANCEL:
                     ACTIVE_MENU = TRUE;
                     EndDialog(hDlg, 0);
                     break;
            }
            break;
   }

return 0;
}

//=============================================================================
//                             ConfigDialogBox
//=============================================================================

BOOL APIENTRY ConfigDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   static BOOL QUITTER_STAT, PASSWORD_STAT, MSG_ACCUEIL_STAT, BILAN_DLG_STAT, AFFICHER_MDP;
   static unsigned short jourHeureSave, jourMinuteSave, nuitHeureSave, nuitMinuteSave, sommeilHeureSave, sommeilMinuteSave;
   static HANDLE hIdeMdp, hWhiteListe, hFenetresCibles;
   
   switch (uMsg)
   {
       case WM_INITDIALOG:
            /* Affichage de l'icone */
            SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ALICE32)));
            /* Création des Tabs */
            TC_ITEM tie;
            tie.mask = TCIF_TEXT;
               /* Tab 1 */
               tie.pszText = "Options"; 
               TabCtrl_InsertItem(GetDlgItem(hDlg, TAB_CONTROL), 1, &tie);
               /* Tab 2 */
               tie.pszText = "Horaires"; 
               TabCtrl_InsertItem(GetDlgItem(hDlg, TAB_CONTROL), 2, &tie);
               /* Tab 3 */
               tie.pszText = "Plus"; 
               TabCtrl_InsertItem(GetDlgItem(hDlg, TAB_CONTROL), 3, &tie);
               /* Tab 4 */
               tie.pszText = "White liste"; 
               TabCtrl_InsertItem(GetDlgItem(hDlg, TAB_CONTROL), 4, &tie);
               /* Tab 5 */
               tie.pszText = "Fenêtres ciblés"; 
               TabCtrl_InsertItem(GetDlgItem(hDlg, TAB_CONTROL), 5, &tie);
            /* On cache l'onglet/Tab 2, 3, 4 et 5 (car c'est le 1er tab qui s'affichera) */
            cacherTab(hDlg, 2);
            cacherTab(hDlg, 3);
            cacherTab(hDlg, 4);
            cacherTab(hDlg, 5);
            /* On coche les CheckBox */
            if (ACTIVE_QUITTER)
               CheckDlgButton(hDlg, IDC_ACTIVE_QUITTER, BST_CHECKED);
            if (ACTIVE_PASSWORD)
               CheckDlgButton(hDlg, IDC_ACTIVE_PASSWORD, BST_CHECKED);
            if (ACTIVE_MSG_ACCUEIL)
               CheckDlgButton(hDlg, IDC_ACTIVE_ACCUEIL, BST_CHECKED);
            if (ACTIVE_BILAN_DLG)
               CheckDlgButton(hDlg, IDC_ACTIVE_BILANDLG, BST_CHECKED);
            /* On remplie les edits */
               /* Jour */
               SetDlgItemInt(hDlg, IDE_JOURH, jourHeure, FALSE);
               SetDlgItemInt(hDlg, IDE_JOURM, jourMinute, FALSE);
               /* Nuit */
               SetDlgItemInt(hDlg, IDE_NUITH, nuitHeure, FALSE);
               SetDlgItemInt(hDlg, IDE_NUITM, nuitMinute, FALSE);
               /* Sommeil */
               SetDlgItemInt(hDlg, IDE_SOMMEILH, sommeilHeure, FALSE);
               SetDlgItemInt(hDlg, IDE_SOMMEILM, sommeilMinute, FALSE);
               /* Mot de passe */
               hIdeMdp = GetDlgItem(hDlg, IDE_MDP);
               SetWindowText(hIdeMdp, PASSWORD);
               /* White liste */
               hWhiteListe = GetDlgItem(hDlg, IDE_WHITE_LISTE);
               lireApartirDeLog(&hWhiteListe, WHITE_LISTE, FALSE, FALSE);
               /* Fenêtres cibles */
               hFenetresCibles = GetDlgItem(hDlg, IDE_FENETRES_CIBLES);
               lireApartirDeLog(&hFenetresCibles, FICHIER_FCIBLE, FALSE, FALSE);
            /* On limite l'edit du mot de passe */
            SendMessage(hIdeMdp, EM_LIMITTEXT, MAX_PASSWORD, 0);
            /* On sauvegarde tout au début */
            QUITTER_STAT = ACTIVE_QUITTER;
            PASSWORD_STAT = ACTIVE_PASSWORD;
            MSG_ACCUEIL_STAT = ACTIVE_MSG_ACCUEIL;
            jourHeureSave = jourHeure;
            jourMinuteSave = jourMinute;
            nuitHeureSave = nuitHeure;
            nuitMinuteSave = nuitMinute;
            sommeilHeureSave = sommeilHeure;
            sommeilMinuteSave = sommeilMinute;
            BILAN_DLG_STAT = ACTIVE_BILAN_DLG;
            /* Initialisation a zero */
            ACTUALISER_CONFIG = FALSE;
            AFFICHER_MDP = FALSE;
            break;
        case WM_CTLCOLORSTATIC:
            {
               HBRUSH hbr = (HBRUSH) RGB(255, 255, 255); /* Couleur blanche */
               SetBkMode((HDC) wParam, TRANSPARENT); /* On assure la transparence des Statics */
               return (BOOL) hbr;
            }
         case WM_NOTIFY:
            {
               LPNMHDR pnmhdr = (LPNMHDR)lParam;
               switch (pnmhdr->code)
               {
                   case TCN_SELCHANGE:
                        switch (TabCtrl_GetCurSel(GetDlgItem(hDlg, TAB_CONTROL)))
                        {
                            case 0: /* Tab/onglet 1 */
                                 cacherTab(hDlg, 2);
                                 cacherTab(hDlg, 3);
                                 cacherTab(hDlg, 4);
                                 cacherTab(hDlg, 5);
                                 afficherTab(hDlg, 1);
                                 break;
                            case 1: /* Tab/onglet 2 */
                                 cacherTab(hDlg, 1);
                                 cacherTab(hDlg, 3);
                                 cacherTab(hDlg, 4);
                                 cacherTab(hDlg, 5);
                                 afficherTab(hDlg, 2);
                                 break;
                            case 2: /* Tab/onglet 3 */
                                 cacherTab(hDlg, 1);
                                 cacherTab(hDlg, 2);
                                 cacherTab(hDlg, 4);
                                 cacherTab(hDlg, 5);
                                 afficherTab(hDlg, 3);
                                 break;
                            case 3: /* Tab/onglet 4 */
                                 cacherTab(hDlg, 1);
                                 cacherTab(hDlg, 2);
                                 cacherTab(hDlg, 3);
                                 cacherTab(hDlg, 5);
                                 afficherTab(hDlg, 4);
                                 break;
                            case 4: /* Tab/onglet 5 */
                                 cacherTab(hDlg, 1);
                                 cacherTab(hDlg, 2);
                                 cacherTab(hDlg, 3);
                                 cacherTab(hDlg, 4);
                                 afficherTab(hDlg, 5);
                                 break;
                        }
                        break;              
               }
            }
            break;
       case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                /* Gestion des clics buttons */
                /* Tab 1 */
                case IDC_ACTIVE_QUITTER:
                     ACTIVE_QUITTER =! ACTIVE_QUITTER;
                     break;
                case IDC_ACTIVE_PASSWORD:
                     ACTIVE_PASSWORD =! ACTIVE_PASSWORD;
                     break;
                case IDC_ACTIVE_ACCUEIL:
                     ACTIVE_MSG_ACCUEIL =! ACTIVE_MSG_ACCUEIL;
                     break;
                /* Tab 2 */
                /* Jour */
                case BT_PLUS_JOURH:
                     if (jourHeure < 23)
                        jourHeure++;
                     else
                        jourHeure = 0;
                     SetDlgItemInt(hDlg, IDE_JOURH, jourHeure, FALSE);
                     break;
                case BT_MOIN_JOURH:
                     if (jourHeure > 0)
                        jourHeure--;
                     else
                        jourHeure = 23;
                     SetDlgItemInt(hDlg, IDE_JOURH, jourHeure, FALSE);
                     break;
                 case BT_PLUS_JOURM:
                     if (jourMinute < 59)
                        jourMinute++;
                     else
                        jourMinute = 0;
                     SetDlgItemInt(hDlg, IDE_JOURM, jourMinute, FALSE);
                     break;
                case BT_MOIN_JOURM:
                     if (jourMinute > 0)
                        jourMinute--;
                     else
                        jourMinute = 59;
                     SetDlgItemInt(hDlg, IDE_JOURM, jourMinute, FALSE);
                     break;
                /* Nuit */
                case BT_PLUS_NUITH:
                     if (nuitHeure < 23)
                        nuitHeure++;
                     else
                        nuitHeure = 0;
                     SetDlgItemInt(hDlg, IDE_NUITH, nuitHeure, FALSE);
                     break;
                case BT_MOIN_NUITH:
                     if (nuitHeure > 0)
                        nuitHeure--;
                     else
                        nuitHeure = 23;
                     SetDlgItemInt(hDlg, IDE_NUITH, nuitHeure, FALSE);
                     break;
                case BT_PLUS_NUITM:
                     if (nuitMinute < 59)
                        nuitMinute++;
                     else
                        nuitMinute = 0;
                     SetDlgItemInt(hDlg, IDE_NUITM, nuitMinute, FALSE);
                     break;
                case BT_MOIN_NUITM:
                     if (nuitMinute > 0)
                        nuitMinute--;
                     else
                        nuitMinute = 59;
                     SetDlgItemInt(hDlg, IDE_NUITM, nuitMinute, FALSE);
                     break;
                /* Sommeil */
                case BT_PLUS_SOMMEILH:
                     if (sommeilHeure < 23)
                        sommeilHeure++;
                     else
                        sommeilHeure = 0;
                     SetDlgItemInt(hDlg, IDE_SOMMEILH, sommeilHeure, FALSE);
                     break;
                case BT_MOIN_SOMMEILH:
                     if (sommeilHeure > 0)
                        sommeilHeure--;
                     else
                        sommeilHeure = 23;
                     SetDlgItemInt(hDlg, IDE_SOMMEILH, sommeilHeure, FALSE);
                     break;
                case BT_PLUS_SOMMEILM:
                     if (sommeilMinute < 59)
                        sommeilMinute++;
                     else
                        sommeilMinute = 0;
                     SetDlgItemInt(hDlg, IDE_SOMMEILM, sommeilMinute, FALSE);
                     break;
                case BT_MOIN_SOMMEILM:
                     if (sommeilMinute > 0)
                        sommeilMinute--;
                     else
                        sommeilMinute = 59;
                     SetDlgItemInt(hDlg, IDE_SOMMEILM, sommeilMinute, FALSE);
                     break;
                /* Tab 3 */
                case IDC_ACTIVE_BILANDLG:
                     ACTIVE_BILAN_DLG =! ACTIVE_BILAN_DLG;
                     break;
                case IDC_AFFICHER_MDP:
                     if (!AFFICHER_MDP)
                     {
                        SendMessage(hIdeMdp, EM_SETPASSWORDCHAR, 0, 0); /* On affiche le mdp */
                        AFFICHER_MDP = TRUE;
                     }
                     else
                     {
                        SendMessage(hIdeMdp, EM_SETPASSWORDCHAR, 7, 0); /* On cache le mdp ( 7 == 'o' petit cercle noir) */
                        AFFICHER_MDP = FALSE;
                     }
                     SetFocus(hIdeMdp); /* On focus l'edit de l' mdp */
                     break;
                /* Public */
                case IDCANCEL:
                     /* On restaure tout */
                     ACTIVE_PASSWORD = PASSWORD_STAT;
                     ACTIVE_QUITTER = QUITTER_STAT;
                     ACTIVE_MSG_ACCUEIL = MSG_ACCUEIL_STAT;
                     jourHeure = jourHeureSave;
                     jourMinute = jourMinuteSave;
                     nuitHeure = nuitHeureSave;
                     nuitMinute = nuitMinuteSave;
                     sommeilHeure = sommeilHeureSave;
                     sommeilMinute = sommeilMinuteSave;
                     ACTIVE_BILAN_DLG = BILAN_DLG_STAT;
                     ACTIVE_MENU = TRUE;
                     EndDialog(hDlg, 0);
                     break;
                case IDOK:
                     if (jourHeure == nuitHeure)
                        MessageBox(hDlg, "Attention, Jour heures = Nuit heures !", NOM_SOLO, MB_OK | MB_ICONWARNING);
                     else if (jourHeure == sommeilHeure)
                        MessageBox(hDlg, "Attention, Jour heures = Sommeil heures !", NOM_SOLO, MB_OK | MB_ICONWARNING);
                     else if (nuitHeure == sommeilHeure)
                        MessageBox(hDlg, "Attention, Nuit heures = Sommeil heures !", NOM_SOLO, MB_OK | MB_ICONWARNING);
                     else if (jourHeure > nuitHeure)
                        MessageBox(hDlg, "Attention, Jour heures > Nuit heures !", NOM_SOLO, MB_OK | MB_ICONWARNING);
                     else if (sommeilHeure > jourHeure && sommeilHeure < nuitHeure)
                        MessageBox(hDlg, "Attention, Jour heures < Sommeil heures < Nuit heures !", NOM_SOLO, MB_OK | MB_ICONWARNING);
                     else if (!GetWindowTextLength(hIdeMdp))
                        MessageBox(hDlg, "Veuillez saisir un mot de passe !", NOM_SOLO, MB_OK | MB_ICONWARNING);
                     else /* Si tout est bien */
                     {
                        GetWindowText(hIdeMdp, PASSWORD, MAX_PASSWORD); /* On récupère le nv mdp */
                        if (sauvegarderConfig()) /* Si sauvegarde réussi */
                        {
                           char *buffer, *buffer2;
                           /* On enregistre la White liste */
                           int taille = GetWindowTextLength(hWhiteListe)+1;
                           buffer = malloc(taille);
                           GetWindowText(hWhiteListe, buffer, taille);
                           enregistrerDansLog(buffer, WHITE_LISTE, TRUE);
                           /* On enregistre les fenêtres cibles */ 
                           taille = GetWindowTextLength(hFenetresCibles)+1;
                           buffer2 = malloc(taille);
                           GetWindowText(hFenetresCibles, buffer2, taille);
                           enregistrerDansLog(buffer2, FICHIER_FCIBLE, TRUE);
                           /* On vide les chaines et on ferme la dialogBox */
                           free(buffer);
                           free(buffer2);
                           ACTUALISER_CONFIG = TRUE;
                           ACTIVE_MENU = TRUE;
                           EndDialog(hDlg, 0);
                        }
                        else
                           MessageBox(hDlg, "Erreur de sauvegarde !", NOM_SOLO, MB_OK | MB_ICONSTOP);
                     }
                     break;
            }
            break;
   }

return 0;
}

//=============================================================================
//                             PasswordDialogBox
//=============================================================================

BOOL APIENTRY PasswordDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg)
   {
       case WM_INITDIALOG:
            /* Affichage de l'icone */
            SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ALICE32)));
            /* Bitmap OK */
            SendMessage(GetDlgItem(hDlg, IDOK), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP
            , (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GO)));
            break;
       case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                /* Gestion des clics buttons */
                case IDOK:
                     {
                        char tmp[32];
                        GetDlgItemText(hDlg, IDE_PASSWORD, tmp, 32);
                        if (lstrcmp(tmp, PASSWORD))
                        {
                           MessageBox(hDlg, "Accès refusé !", NOM_SOLO, MB_OK | MB_ICONSTOP);
                           break;
                        }
                        else
                           PASSWORD_OK = TRUE;
                     }
                case IDCANCEL:
                     ACTIVE_MENU = TRUE;
                     EndDialog(hDlg, 0);
                     break;
            }
            break;
   }

return 0;
}

//=============================================================================
//                              BilanDialogBox
//=============================================================================

BOOL APIENTRY BilanDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   static LPVOID hRichEdit;
   static HANDLE hZoneTitre;
   static HANDLE hBtRechercher;
   static HANDLE hIdeRechercher;
   static char nomFichier[11]; /* "xx-xx-xxxx" (10) + (1) */
   static char cheminFichier[100]; /* minimum(56): lstrlen(PATH_TO_LOG) == (29) + "\\xx-xx-xxxx\\xx-xx-xxxx.txt" (26) + (1) */
   static unsigned int nbrDossier, nbrFichier;
   
   switch (uMsg)
   {
       case WM_INITDIALOG:
            /* Affichage de l'icone */
            SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ALICE32)));
            /* Bitmap des bouttons */
               /* Precedent */
               SendMessage(GetDlgItem(hDlg, BT_PRECEDENT), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP
               , (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PRECEDENT)));
               /* Suivant */
               SendMessage(GetDlgItem(hDlg, BT_SUIVANT), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP
               , (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SUIVANT)));
               /* Rechercher */
               SendMessage(GetDlgItem(hDlg, BT_RECHERCHER), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP
               , (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RECHERCHER)));
               /* Explorer */
               SendMessage(GetDlgItem(hDlg, BT_EXPLORER), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP
               , (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_EXPLORER)));
            /* Lecture du fichier log */
            /* On récupère le nombre de dossier + le nom du dernier */
            nbrDossier = 0; /* Initialisation à zéro */
            explorerDossier(PATH_TO_LOG, nomFichier, &nbrDossier, TRUE);
            nbrFichier = nbrDossier;
            sprintf(cheminFichier, "%s\\%s\\%s.txt", PATH_TO_LOG, nomFichier, nomFichier); /* Contient le chemin du dernier fichier log */
            while (lstrcmp(cheminFichier, FICHIER_LOG)) /* Tant qu'on ne trouve pas le fichier log d'ajourd'hui */
            {
               /* On cherche le fichier suivant */
               if (nbrFichier+1 > nbrDossier) /* Si on arrive au dernier fichier on recommence du premier */
                  nbrFichier = 1;
               else
                  nbrFichier++;
               explorerDossier(PATH_TO_LOG, nomFichier, &nbrFichier, FALSE);
               sprintf(cheminFichier, "%s\\%s\\%s.txt", PATH_TO_LOG, nomFichier, nomFichier);     
            }
            /* Une fois le fichier log d'aujourd'hui trouvé */
            hRichEdit = GetDlgItem(hDlg, IDE_RICHEDIT); /* On récupère le handle du RichEdit */
            lireApartirDeLog(&hRichEdit, cheminFichier, FALSE, TRUE); /* On affiche le fichier log */
            /* Changement de police pour TXT_NOM_LOG */
            hZoneTitre = GetDlgItem(hDlg, TXT_NOM_LOG);
            HFONT PoliceTahoma = CreateFont(24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Forte");
            SendMessage(hZoneTitre, WM_SETFONT, (long)PoliceTahoma, 0);
            SendMessage(hZoneTitre, WM_SETTEXT, 0, (LPARAM)nomFichier); /* On y met le nom du fichier log */
            /* Désactivation du boutton Rechercher */
            hIdeRechercher = GetDlgItem(hDlg, IDE_RECHERCHER);
            hBtRechercher = GetDlgItem(hDlg, BT_RECHERCHER);
            EnableWindow(hBtRechercher, FALSE);
            break;
       case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                /* Gestion des clics buttons */
                case BT_PRECEDENT:
                     if (nbrDossier) /* S'il y'a des dossiers/fichiers logs à lire (je sais qu'il y'en aura au moin 1, mais ce test est plus sécurisé) */
                     {
                        /* On cherche le fichier précedent */
                        if (nbrFichier-1 == 0) /* Si on ne peut plus revenir en arrière on recommence du dernier */
                           nbrFichier = nbrDossier;
                        else
                           nbrFichier--;
                        explorerDossier(PATH_TO_LOG, nomFichier, &nbrFichier, FALSE);
                        sprintf(cheminFichier, "%s\\%s\\%s.txt", PATH_TO_LOG, nomFichier, nomFichier);
                        /* On l'affiche à l'ecran */
                        lireApartirDeLog(&hRichEdit, cheminFichier, FALSE, TRUE);
                        /* On affiche son nom dans la zone de titre */
                        SendMessage(hZoneTitre, WM_SETTEXT, 0, (LPARAM)nomFichier);
                     }
                     break;
                case BT_SUIVANT:
                     if (nbrDossier)
                     {
                        /* On cherche le fichier suivant */
                        if (nbrFichier+1 > nbrDossier) /* Si on arrive au dernier fichier on recommence du premier */
                           nbrFichier = 1;
                        else
                           nbrFichier++;
                        explorerDossier(PATH_TO_LOG, nomFichier, &nbrFichier, FALSE);
                        sprintf(cheminFichier, "%s\\%s\\%s.txt", PATH_TO_LOG, nomFichier, nomFichier);
                        /* On l'affiche à l'ecran */
                        lireApartirDeLog(&hRichEdit, cheminFichier, FALSE, TRUE);
                        /* On affiche son nom dans la zone de titre */
                        SendMessage(hZoneTitre, WM_SETTEXT, 0, (LPARAM)nomFichier);
                     }
                     break;
                case BT_RECHERCHER:
                     if(!rechercherTexteEdit(&hRichEdit, &hIdeRechercher))
                        MessageBox(hDlg, "Recherche nulle !", NOM_SOLO, MB_OK | MB_ICONINFORMATION);
                     break;
                case BT_EXPLORER:
                     //SetFileAttributes(PATH_TO_LOG, FILE_ATTRIBUTE_NORMAL); /* Déverouillage du dossier qui contient les logs */
                     sprintf(cheminFichier, "%s\\%s", PATH_TO_LOG, nomFichier);
                     ShellExecute(NULL, "open", cheminFichier, NULL, NULL, SW_SHOWNORMAL);
                     //SetFileAttributes(PATH_TO_LOG, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM); /* Vérouillage du dossier */
                     break;
                case IDCANCEL:
                     ACTIVE_MENU = TRUE;
                     EndDialog(hDlg, 0);
                     break;
            }
            switch (HIWORD(wParam))
            {
                case EN_CHANGE:     /* Si Changement dans les edits */
                     switch (LOWORD(wParam))
                     {
                         case IDE_RECHERCHER: /* Si le changement conserne l'IDE_RECHERCHER */
                              if (GetWindowTextLength(hIdeRechercher)) /* S'il y'a au moin 1 caractère dans l'edit */
                                 EnableWindow(hBtRechercher, TRUE); /* Activation du boutton Rechercher */
                              else
                                 EnableWindow(hBtRechercher, FALSE); /* Désactivation du boutton Rechercher */
                              break;
                     }
                     break;
            }   
            break;
   }

return 0;
}

//=============================================================================
//                             RappelDialogBox
//=============================================================================

BOOL APIENTRY RappelDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{   
   switch (uMsg)
   {
       case WM_INITDIALOG:
            /* Affichage de l'icone */
            SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ALICE32)));
            /* Bitmap de 'Nouveau' (relié a IDOK) */
            SendMessage(GetDlgItem(hDlg, IDOK), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP
            , (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RAPPEL)));
            /* Création des colones de la ListView */
            hListView = GetDlgItem(hDlg, LISTE_RAPPEL);
            creerListViewColone(&hListView, 90, "Date", 0);
            creerListViewColone(&hListView, 50, "Heure", 1);
            creerListViewColone(&hListView, 135, "Rappel", 2);
            SendMessage(hListView, LVM_SETEXTENDEDLISTVIEWSTYLE , 0, LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP); /* Gestion des styles */
            /* On remplie la ListView (le dernier rappel s'affichera en haut) */
            lireEtAfficherRappel(&hListView, FICHIER_RAPPEL);
            /* On indique que la dialogBox du rappel est activé */
            IS_DLG_RAPPEL = TRUE;
            break;
       case WM_NOTIFY:
            {
               /* Changement de l'item actuel de la ListView */
               LPNM_LISTVIEW pnlv = (LPNM_LISTVIEW)lParam;
               if (pnlv->hdr.code == LVN_ITEMCHANGED)
                  lvItemActuel = pnlv->iItem;
            }
            break;
       case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                /* Gestion des clics buttons */
                case IDOK:
                     ACTIVE_MODIFIER_RAPPEL = FALSE;
                     DialogBox(hInst, "NVRAPPEL", hDlg, (DLGPROC)NvRappelDlgProc);
                     break;
                case BT_MODIFIER:
                     if (lvItemActuel < ListView_GetItemCount(hListView)) /* Si l'item actuel existe dans la listView */
                     {
                        ACTIVE_MODIFIER_RAPPEL = TRUE;
                        DialogBox(hInst, "NVRAPPEL", hDlg, (DLGPROC)NvRappelDlgProc);
                     }
                     else
                        MessageBox(hDlg, "Veuillez sélectionner un rappel !", NOM_SOLO, MB_OK | MB_ICONWARNING);
                     break;
                case BT_SUPPRIMER:
                     if (lvItemActuel < ListView_GetItemCount(hListView)) /* Si l'item actuel existe dans la listView */
                     {
                        supprimerRappel(&hListView, FICHIER_RAPPEL, lvItemActuel, FALSE, "");
                        ListView_DeleteItem(hListView, lvItemActuel); /* On efface l'élément de la ListView */
                     }
                     else
                        MessageBox(hDlg, "Veuillez sélectionner un rappel !", NOM_SOLO, MB_OK | MB_ICONWARNING);
                     break;
                case IDCANCEL:
                     ACTIVE_MENU = TRUE;
                     IS_DLG_RAPPEL = FALSE;
                     EndDialog(hDlg, 0);
                     break;
            }
            break;
   }

return 0;
}

//=============================================================================
//                             NvRappelDialogBox
//=============================================================================

BOOL APIENTRY NvRappelDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   static SYSTEMTIME Time;
   static unsigned int cYear;
   static char ancienRappel[100];
   static BOOL ACTIVE_DATE_BUTTONS;
   
   switch (uMsg)
   {
       case WM_INITDIALOG:
            /* Affichage de l'icone */
            SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ALICE32)));
            /* Initialisation */
            ACTIVE_DATE_BUTTONS = TRUE;
            /* On récupère le temp actuel + gestion des collisions */
            GetSystemTime(&Time);
            if (ACTIVE_MODIFIER_RAPPEL)
            {
               char date[11], heure[6], sujetRappel[MAX_SRAPPEL+1];
               int day, month, year, hour, minute;
               SendMessage(hDlg, WM_SETTEXT, 0, (LPARAM)"Modifier rappel"); /* On change le nom de la fenêtre */
               SendDlgItemMessage(hDlg, IDOK, WM_SETTEXT, 0, (LPARAM)"Modifier"); /* On change le nom du boutton OK (qui était 'Créer') */
               /* On récupère la ligne/rappel à supprimer depuis la listView */
               ListView_GetItemText(hListView, lvItemActuel, 0, date, 11); /* Partie1 (Date) */
               ListView_GetItemText(hListView, lvItemActuel, 1, heure, 6); /* Partie2 (Heure) */
               ListView_GetItemText(hListView, lvItemActuel, 2, sujetRappel, MAX_SRAPPEL+1); /* Partie3 (Rappel) */
               sprintf(ancienRappel, "%s %s %s", date, heure, sujetRappel);
               if (date[0] == 'T')
               {
                  CheckDlgButton(hDlg, IDC_FOR_EVER, BST_CHECKED); /* On coche le checkBox de répétition pour toujours */
                  SetDlgItemText(hDlg, IDE_JOUR, "xx"); /* Jour */
                  SetDlgItemText(hDlg, IDE_MOIS, "xx"); /* Mois */
                  SetDlgItemText(hDlg, IDE_ANNEE, "xxxx"); /* Année */
                  /* On désactive les bouttons de gestion de la date */
                  ACTIVE_DATE_BUTTONS = FALSE;
                  EnableWindow(GetDlgItem(hDlg, BT_PLUS_JOUR), ACTIVE_DATE_BUTTONS);
                  EnableWindow(GetDlgItem(hDlg, BT_MOIN_JOUR), ACTIVE_DATE_BUTTONS);
                  EnableWindow(GetDlgItem(hDlg, BT_PLUS_MOIS), ACTIVE_DATE_BUTTONS);
                  EnableWindow(GetDlgItem(hDlg, BT_MOIN_MOIS), ACTIVE_DATE_BUTTONS);
                  EnableWindow(GetDlgItem(hDlg, BT_PLUS_ANNEE), ACTIVE_DATE_BUTTONS);
                  EnableWindow(GetDlgItem(hDlg, BT_MOIN_ANNEE), ACTIVE_DATE_BUTTONS);
               }
               else
               {
                  /* On remplace les '/' de la date par ' ' pour la fonction sscanf() */
                  date[2] = ' ';
                  date[5] = ' ';
                  sscanf(date, "%d%d%d", &day, &month, &year);
                  Time.wDay = day;
                  Time.wMonth = month;
                  Time.wYear = year;
                  SetDlgItemInt(hDlg, IDE_JOUR, Time.wDay, FALSE); /* Jour */
                  SetDlgItemInt(hDlg, IDE_MOIS, Time.wMonth, FALSE); /* Mois */
                  SetDlgItemInt(hDlg, IDE_ANNEE, Time.wYear, FALSE); /* Année */
               }
               
               /* On remplace les ':' de l'heure par ' ' */
               heure[2] = ' ';
               sscanf(heure, "%d%d", &hour, &minute);
               Time.wHour = hour;
               Time.wMinute = minute;
               
               SetDlgItemInt(hDlg, IDE_HEURE, Time.wHour, FALSE); /* Heure */
               SetDlgItemInt(hDlg, IDE_MINUTE, Time.wMinute, FALSE); /* Minute */
               sujetRappel[lstrlen(sujetRappel)-1] = '\0'; /* On enlève le '\n' car on va rajouter un après */
               SetDlgItemText(hDlg, IDE_RAPPEL, sujetRappel); /* Rappel */
            }
            else
            {
               /* On remplie les edits par la date et l'heure d'aujourd'hui */
               SetDlgItemInt(hDlg, IDE_JOUR, Time.wDay, FALSE); /* Jour */
               SetDlgItemInt(hDlg, IDE_MOIS, Time.wMonth, FALSE); /* Mois */
               SetDlgItemInt(hDlg, IDE_ANNEE, Time.wYear, FALSE); /* Année */
               cYear = Time.wYear;
               SetDlgItemInt(hDlg, IDE_HEURE, Time.wHour, FALSE); /* Heure */
               if (Time.wMinute < 59) /* Si minutes < 59 */
                  Time.wMinute++;
               else
               {
                  Time.wMinute = 0;
                  if (Time.wHour < 23) /* Si heure < 23 */
                     Time.wHour++;
                  else
                  {
                     Time.wHour = 0;
                     if (Time.wDay < 31) /* Si jour < 31 */
                        Time.wDay++;
                     else
                     {
                        Time.wDay = 1;
                        if (Time.wMonth < 12) /* Si mois < 12 */
                           Time.wMonth++;
                        else
                        {
                           Time.wMonth = 1;
                           Time.wYear++; /* On ne testera pas l'année :D, vu qu'on ne peut revenir au passé :p */
                           SetDlgItemInt(hDlg, IDE_JOUR, Time.wYear, FALSE);
                        }
                        SetDlgItemInt(hDlg, IDE_JOUR, Time.wMonth, FALSE);
                     }
                     SetDlgItemInt(hDlg, IDE_JOUR, Time.wDay, FALSE);
                  }
                  SetDlgItemInt(hDlg, IDE_HEURE, Time.wHour, FALSE); /* Heure +1 */
               }
               SetDlgItemInt(hDlg, IDE_MINUTE, Time.wMinute, FALSE); /* Minutes (+1 pour que le rappel soit à la prochaine minute) */
            }
            /* On limite l'edit du sujet du rappel à 50 caractères */
            SendDlgItemMessage(hDlg, IDE_RAPPEL, EM_LIMITTEXT, MAX_SRAPPEL, 0);
            break;
       case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                /* Gestion des clics buttons */
                /* Jour */
                case BT_PLUS_JOUR:
                     if (Time.wDay == 31)
                        Time.wDay = 1;
                     else
                        Time.wDay++;
                     SetDlgItemInt(hDlg, IDE_JOUR, Time.wDay, FALSE);
                     break;
                case BT_MOIN_JOUR:
                     if (Time.wDay == 1)
                        Time.wDay = 31;
                     else
                        Time.wDay--;
                     SetDlgItemInt(hDlg, IDE_JOUR, Time.wDay, FALSE);
                     break;
                /* Mois */
                case BT_PLUS_MOIS:
                     if (Time.wMonth == 12)
                        Time.wMonth = 1;
                     else
                        Time.wMonth++;
                     SetDlgItemInt(hDlg, IDE_MOIS, Time.wMonth, FALSE);
                     break;
                case BT_MOIN_MOIS:
                     if (Time.wMonth == 1)
                        Time.wMonth = 12;
                     else
                        Time.wMonth--;
                     SetDlgItemInt(hDlg, IDE_MOIS, Time.wMonth, FALSE);
                     break;
                /* Année */
                case BT_PLUS_ANNEE:
                     Time.wYear++;
                     SetDlgItemInt(hDlg, IDE_ANNEE, Time.wYear, FALSE);
                     break;
                case BT_MOIN_ANNEE:
                     if (Time.wYear-1 >= cYear) /* Si on ne descend pas au dessous de l'an actuel */
                     {
                        Time.wYear--;
                        SetDlgItemInt(hDlg, IDE_ANNEE, Time.wYear, FALSE);
                     }
                     break;
                /* Heure */
                case BT_PLUS_HEURE:
                     if (Time.wHour == 23)
                        Time.wHour = 0;
                     else
                        Time.wHour++;
                     SetDlgItemInt(hDlg, IDE_HEURE, Time.wHour, FALSE);
                     break;
                case BT_MOIN_HEURE:
                     if (Time.wHour == 0)
                        Time.wHour = 23;
                     else
                        Time.wHour--;
                     SetDlgItemInt(hDlg, IDE_HEURE, Time.wHour, FALSE);
                     break;
                /* Minutes */
                case BT_PLUS_MINUTE:
                     if (Time.wMinute == 59)
                        Time.wMinute = 0;
                     else
                        Time.wMinute++;
                     SetDlgItemInt(hDlg, IDE_MINUTE, Time.wMinute, FALSE);
                     break;
                case BT_MOIN_MINUTE:
                     if (Time.wMinute == 0)
                        Time.wMinute = 59;
                     else
                        Time.wMinute--;
                     SetDlgItemInt(hDlg, IDE_MINUTE, Time.wMinute, FALSE);
                     break;
                /* Répéter pour toujours */
                case IDC_FOR_EVER:
                     ACTIVE_DATE_BUTTONS =! ACTIVE_DATE_BUTTONS;
                     EnableWindow(GetDlgItem(hDlg, BT_PLUS_JOUR), ACTIVE_DATE_BUTTONS);
                     EnableWindow(GetDlgItem(hDlg, BT_MOIN_JOUR), ACTIVE_DATE_BUTTONS);
                     EnableWindow(GetDlgItem(hDlg, BT_PLUS_MOIS), ACTIVE_DATE_BUTTONS);
                     EnableWindow(GetDlgItem(hDlg, BT_MOIN_MOIS), ACTIVE_DATE_BUTTONS);
                     EnableWindow(GetDlgItem(hDlg, BT_PLUS_ANNEE), ACTIVE_DATE_BUTTONS);
                     EnableWindow(GetDlgItem(hDlg, BT_MOIN_ANNEE), ACTIVE_DATE_BUTTONS);
                     if (!ACTIVE_DATE_BUTTONS)
                     {
                        SetDlgItemText(hDlg, IDE_JOUR, "xx"); /* Jour */
                        SetDlgItemText(hDlg, IDE_MOIS, "xx"); /* Mois */
                        SetDlgItemText(hDlg, IDE_ANNEE, "xxxx"); /* Année */                        
                     }
                     else
                     {
                        SetDlgItemInt(hDlg, IDE_JOUR, Time.wDay, FALSE); /* Jour */
                        SetDlgItemInt(hDlg, IDE_MOIS, Time.wMonth, FALSE); /* Mois */
                        SetDlgItemInt(hDlg, IDE_ANNEE, Time.wYear, FALSE); /* Année */   
                     }
                     break;
                /* Créer/Modifier */
                case IDOK:
                     if (!GetWindowTextLength(GetDlgItem(hDlg, IDE_RAPPEL)))
                     {
                        MessageBox(hDlg, "Veuillez entrer le sujet du rappel !", NOM_SOLO, MB_OK | MB_ICONWARNING);
                        break;
                     }
                     else
                     {
                        char rappel[100];
                        char sujetRappel[MAX_SRAPPEL];
                        GetDlgItemText(hDlg, IDE_RAPPEL, sujetRappel, MAX_SRAPPEL); /* On récupère le sujet du rappel */
                        if (IsDlgButtonChecked(hDlg, IDC_FOR_EVER) == BST_CHECKED) /* Si checkBox coché */
                           sprintf(rappel, "Toujours %02d:%02d %s\n", Time.wHour, Time.wMinute, sujetRappel);
                        else
                           sprintf(rappel, "%02d/%02d/%d %02d:%02d %s\n", Time.wDay, Time.wMonth, Time.wYear, Time.wHour, Time.wMinute, sujetRappel);
                        if (verifierRappel(FICHIER_RAPPEL, rappel, FALSE)) /* Si le rappel n'existe pas déjà (juste la même date et heure) */
                        {
                           if (ACTIVE_MODIFIER_RAPPEL)
                              modifierRappel(&hListView, FICHIER_RAPPEL, lvItemActuel, ancienRappel, rappel);
                           else
                           {
                              enregistrerDansLog(rappel, FICHIER_RAPPEL, FALSE); /* On enregistre tout */
                              ajouterALaListView(&hListView, rappel, lstrlen(rappel), FALSE, 0); /* On ajoute le rappel en bas de la liste */
                           }
                        }
                        else
                        {
                           MessageBox(hDlg, "Cette date/heure existe déjà !", NOM_SOLO, MB_OK | MB_ICONWARNING);
                           break;
                        }
                     }
                /* Annuler/Fermer */
                case IDCANCEL:
                     EndDialog(hDlg, 0);
                     break;
            }
            break;
   }

return 0;
}
