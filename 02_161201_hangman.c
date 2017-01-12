/* !* Hangman with german words
 *
 * \description a simple Hangman game, where you can play against computer or
 *              different player. Two different gamemodes, player vs player
 *              or player vs computer, aswell HARDCORE or NORMAL, difference is
 *              only 10 attempts, instead of 15 with longer words and aswell,
 *              counting down, if player hits a right letter.
 *
 * \author Sebastian Dichler <el16b032@technikum-wien.at>
 *
 * \version Rev.: 01, 01.12.2016 - Created
 *          Rev.: 02, 01.12.2016 - Added File input, Reading words and
 *                                 add them into array, choose word randomly,
 *                                 print "_" characters of word lenght
 *          Rev.: 03, 01.12.2016 - Added helpdesk, better user interface
 *          Rev.: 04, 02.12.2016 - Added red writeout for debug messages
 *          Rev.: 05, 16.12.2016 - Added guessing process, but having problems
 *                                 with next game, that strings keep saved (word_check)
 *                                 keeps saved but resetted everything with "\0"
 *                                 after game and problem that programm stops reducing
 *                                 attempts after writing a right letter
 *          Rev.: 06, 16.12.2016 - Problem with stop reducing attempts now fixed,
 *                                 other problem still there
 *          Rev.: 07, 16.12.2016 - Formating for handin
 *          Rev.: 08, 16.12.2016 - Everything is done, only replay function
 *                                 is missing.
 *          Rev.: 09, 17.12.2016 - programm fixxes, shorten the code
 *
 * \information - Programm tested on linux (tested on ubuntu14.04) and macos
 *                (tested on os sierra 10.12.2), somehow windows prints the word
 *                into console, without telling him to do that
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define DEBUG 0
#define LINECOUNT 110 // Amount of rows (+1 in file for \n)
#define WORDMAX 20 // Amount of chars in word
#define ONELETTER 100 // Amount of chars for oneletter
#define HARDCORE 0      //must be the negaive of NORMAL
#define NORMAL 1
#define WITHAGAIN 0     //must be the negative of WITHOUTAGAIN, (is not working)
#define WITHOUTAGAIN 1

/*------------------------------------------------------------------*/
/* S T A R T   O F   P R O G R A M                                  */
/*------------------------------------------------------------------*/

#if defined(__APPLE__) && defined(__MACH__) || defined(__linux__)

/* ---- ASCII FORMAT FOR TERMINAL ---- */

#define RESET   "\033[0m"
#define BLACK   "\033[30m"                 /* Black */
#define RED     "\033[31m"                 /* Red */
#define GREEN   "\033[32m"                 /* Green */
#define CYAN    "\033[36m"                 /* Cyan */
#define BOLD    "\033[1m"                  /* Bold */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#endif

#if defined(_WIN32) || defined(_WIN64)

/* ---- ASCII FORMAT NOT SUPPORTED IN WINDOWS ---- */

#define RESET   ""
#define BLACK   ""
#define RED     ""
#define GREEN   ""
#define CYAN    ""
#define BOLD    ""
#define BOLDRED     ""
#define BOLDGREEN   ""
#define BOLDBLACK   ""
#define BOLDCYAN    ""
#define BOLDWHITE   ""

#endif


void helpdesk_1(void);
void helpdesk_2(void);
void gallows(int i);
void clear(void);
void clearNoHelp(void);
unsigned int catchClearString(char *input);

int main(int argc, char *argv[])
{
    FILE* pFin = NULL;
    char word_string[LINECOUNT][WORDMAX];
    char word_check[LINECOUNT];
    char word[WORDMAX];
    char oneletter[ONELETTER];
    char letter[WORDMAX];
    
    char playagain;
    
    int i;
    int random_word;
    int counter = 0;
    int counter2 = 0;
    int attempts = 10;
    int error = 0;
    int gamestatus = 0;
    
/*------------------------------------------------------------------*/
/* S T A R T S C R E E N                                            */
/*------------------------------------------------------------------*/
    
    clear();
    
/*------------------------------------------------------------------*/
/* E R R O R M E S S A G E S                                        */
/*------------------------------------------------------------------*/
    
    if (argc == 1)
    {
        printf("\n");
        printf(BOLD"ERROR: YOU HAVE TO CHOSE BETWEEN PLAYER VS PLAYER \"P\" OR"
               " PLAYER VS COMPUTER \"C\"\n"RESET);
        helpdesk_2();
        return -1;
    }
    
    if (argv[1][0] != 'P' && argv[1][0] != 'p' &&
        argv[1][0] != 'C' && argv[1][0] != 'c' &&
        argv[1][0] != 'H' && argv[1][0] != 'h' &&
        argv[1][0] != '?')
    {
        printf(BOLD"ERROR: UNKNOWN PARAMETER\n"RESET);
        helpdesk_2();
        return -1;
    }
    
/*------------------------------------------------------------------*/
/* H E L P D E S K                                                  */
/*------------------------------------------------------------------*/
    
    if (argv[1][0] == '?' || argv[1][0] == 'H' || argv[1][0] == 'h')
    {
        helpdesk_2();
        return 0;
    }

/*------------------------------------------------------------------*/
/* S T A R T   O F   P R O G R A M                                  */
/*------------------------------------------------------------------*/
    while (1)
    {
        
    if (argc >= 2)
    {
        
/*------------------------------------------------------------------*/
/* P L A Y   A G A I N S T   P L A Y E R                            */
/*------------------------------------------------------------------*/
        if (argv[1][0] == 'P' || argv[1][0] == 'p')
        {

/* ---- ASK FOR INPUT ---- */
            
            printf("WHAT IS THE CHOOSEN WORD (less than 20 characters)?\n");
            scanf("%19s", word);
            //fgets(word,WORDMAX-1,stdin);
            
            catchClearString(word);
            
/* ---- CAP LETTERS TO LOWER ---- */
            
            for (i = 0; i < strlen(word); i++)
            {
                word[i] = tolower(word[i]);
            }
#if DEBUG
            printf(RED"Stringcheck: %s\n"RESET, word);
            printf(RED"First letter should be lower character now\n"RESET);
#endif
            
/* ---- CHECK FOR DIGITS ---- */
            
            for (i = 0; i < strlen(word); i++)
            {
                if (isdigit(word[i]) == 1)
                {
                    printf(BOLD"ERROR: ONLY WORDS NO DIGITS\n"RESET);
                    return -1;
                }
            }
            
#if DEBUG
            printf(RED"Stringcheck: %s\n"RESET, word);
            printf(RED"i = %d\n"RESET, i);
#endif
            fseek(stdin,0,SEEK_END);
            fflush(stdin);
        }
        
/*------------------------------------------------------------------*/
/* P L A Y   A G A I N S T   C O M P U T E R                        */
/*------------------------------------------------------------------*/
        
        if (argv[1][0] == 'C' || argv[1][0] == 'c')
        {
            
/* ---- OPEN FILE ---- */
            
            pFin = fopen("words.txt", "r");
            
            if (pFin == NULL)
            {
                printf(BOLD"ERRO: COULD NOT OPEN FILE, OPEN IT MANULLY...\n"RESET);
                printf("E.G. ./a.out C filename.txt\n");
                pFin = fopen(argv[2], "r");
                {
                    if (pFin == NULL)
                    {
                        printf(BOLD"ERROR: COULD NOT OPEN FILE FOR READING.\n"RESET);
                        return -1;
                    }
                }
            }
            
/* ---- READ WORDS FROM FILE ---- */

            for (i = 0; i < LINECOUNT; i++)
            {
                fgets(word_string[i], WORDMAX, pFin);
#if DEBUG
                printf(RED"%3d %s"RESET, i + 1, word_string[i]);
#endif
            }

            fclose(pFin);
            
#if DEBUG
            printf("\n\n");
#endif
            
/* ---- RANDOM WORD (from Array to word-String) ---- */
            
            srand(time(NULL));
            random_word = rand()%LINECOUNT;
            
            strcpy(word, word_string[random_word]);
            
#if DEBUG
#if defined(__APPLE__) && defined(__MACH__)
            printf(RED"Wordlenght: %lu\n"RESET, strlen(word));
#endif
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
			printf(RED"Wordlenght: %d\n"RESET, strlen(word));
#endif
			
#endif
            
/* ---- REMOVE \n FROM STRING WITH \0 ---- */
            
            //word[strlen(word)-1] = '\0';
            catchClearString(word);
            
/* ---- CHECK IF WORD AND WORDSTRING IS THE SAME ---- */
            
#if DEBUG
            printf(RED"Word_String: %s"RESET, word_string[random_word]);
            printf(RED"Word:        %s\n"RESET, word);
#endif

/* ---- CAP TO LOWER LETTERS ---- */
            
            for (i = 0; i < strlen(word); i++)
            {
                word[i] = tolower(word[i]);
            }
#if DEBUG
            printf(RED"Stringcheck: %s\n"RESET, word);
			
#if defined(__APPLE__) && defined(__MACH__)
            printf(RED"Wordlenght after formation: %lu\n"RESET, strlen(word));
#endif
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
            printf(RED"Wordlenght after formation: %d\n"RESET, strlen(word));
#endif

            printf("\n");
            printf(RED"i = %d\n"RESET, i);
#endif
        }
        
/*------------------------------------------------------------------*/
/* G U E S S I N G   T A S K                                        */
/*------------------------------------------------------------------*/
        
        /* ---- MORE TRIES IF WORD IS ABOVE 15 CHARACTERS LONG ---- */
        
        if (strlen(word) >= 15)
        {
            attempts = 15;
        }
        
        /* ---- LESS TRIES IF WORD IS LESS THAN 15 CHARACTERS LONG ---- */
#if NORMAL
        if (strlen(word) < 15)
        {
            attempts = 10;
        }
#endif
		memset(oneletter,0,sizeof(oneletter));
		
        for (i = 0; i < ONELETTER; i++)
        {
            oneletter[i] = ' ';
        }
        catchClearString(oneletter);
        
/* ---- WRITE "_" CHARACTERS INTO WORD_CHECK STRING ---- */
        
        memset(word_check,0,sizeof(word_check));
        
        for (i = 0; i < strlen(word); i++)
        {
            word_check[i] = '_';
        }
        
        //word_check[i] = '\0';
        catchClearString(word_check);

/* ---- CLEAR CONSOLE / TEMINAL ---- */
        
        clear();
       
#if defined(__APPLE__) && defined(__MACH__)
        printf("You can guess a single character or the whole word"
               " ("BOLD"%lu"RESET" letters)\n", strlen(word));
#endif
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
		printf("You can guess a single character or the whole word"
               " ("BOLD"%d"RESET" letters)\n", strlen(word));
#endif
        printf("Punishment for a wrong letter: \t"BOLD"-1 Attempt"RESET"\n");
        printf("Punishment for a wrong word: \t"BOLD"-2 Attempts"RESET"\n\n");
		
#if defined(__APPLE__) && defined(__MACH__)
        printf("The word is "BOLD"%lu"RESET" letters long.\n", strlen(word));
 #endif
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
		printf("The word is "BOLD"%d"RESET" letters long.\n", strlen(word));
#endif
		
        while (1)
        {
            
/* ---- ASKING FOR INPUT ---- */

            printf("\n");
            printf("You have "BOLD"%d"RESET" attempts left.\n\n", attempts);
            printf("Please guess a letter or word > ");
            
            scanf("%19s", letter);
            //fgets(letter, WORDMAX-1, stdin);
            
/* ---- INPUT TO LOWER CHARACTERS ---- */
            
            for (i = 0; i < strlen(letter); i++)
            {
                letter[i] = tolower(letter[i]);
            }
            
            //letter[strlen(letter)-1] = '\0';
            catchClearString(letter);
            
            fseek(stdin,0,SEEK_END);
            fflush(stdin);
       
/* ---- CHECK FOR INVALID INPUT ---- */
            
            error = 0;
            
            while(error == 0)
            {
                error = 0;
                
                for (i = 0; i < strlen(letter); i++)
                {
                    if (isalpha(letter[i]) == 0)
                    {
                        printf(BOLD"ERROR: Letters only!\n"RESET);
                        //return -1;
                        error = 1;
                        break;
                    }

                }
                
/* ---- INPUT IS JUST 1 CHARACTER ---- */
                
                if (strlen(letter) == 1)
                {
                    oneletter[counter] = letter[0];
                    counter++;
                    
                    while(1)
                    {
                        counter2 = 0;
                        for (i = 0; i < strlen(word); i++)
                        {
                            if (letter[0] == word[i])
                            {
                                word_check[i] = word[i];
                                counter2++;
                            }
                        }
                        
/* ---- IF PLAYER INPUT IS SAME AS WORD ---- */
                        
                        if (strcmp(word_check, word) == 0)
                        {
                            gamestatus = 1;
                        }
                        
                        break;
                    }
                    
/* ---- GO OUT OF LOOP IF PLAYER WON ---- */
                    
#if NORMAL
                    if (gamestatus == 1 || counter2 != 0)
                    {
                        break;
                    }
#endif
                    
/* ---- WE REMOVE COUNTER TO REDUCE ATTEMPTS ALSO IF PLAYER GETS RIGHT CHARACTER ---- */
                    
#if HARDCORE
                    if (gamestatus == 1)
                    {
                        break;
                    }
#endif
                    

                    
/* ---- PLAYER WROTE WRONG LETTER ---- */
                    
                    printf("Wrong letter: \t"BOLD"-1 Attempt\n"RESET);

                    attempts = attempts - 1;
                    break;
                }
                
/* ---- PLAYER WROTE WORD WITH SAME CHARACTER AMMOUNT OF WORD ---- */
                
                if (strlen(letter) == strlen(word))
                {
                    
/* ---- AND INPUT IS THE SAME AS WORD ---- */
                    
                    if (strcmp(letter, word) == 0)
                    {
                        gamestatus = 1;
                        break;
                    }
                    
/* ---- PLAYER WROTE WRONG WORD ----- */
                    
                    printf("Wrong word: \t"BOLD"-2 Attempts\n"RESET);

                    gamestatus = 2;
                    attempts = attempts -2;
                    break;
                }
                
/* ---- IF INPUT IS NOT 1 CHARACTER OR STRLEN OF WORD ---- */
                
#if defined(__APPLE__) && defined(__MACH__)
                printf(BOLD"ERROR: 1 or %lu characters!\n"RESET, strlen(word));
#endif
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
                printf(BOLD"ERROR: 1 or %d characters!\n"RESET, strlen(word));
#endif
                break;
            }
            
/* ---- CLEAR CONSOLE FOR GAMEINFORMATION WITHOUT HELPDESK ---- */
            
            clearNoHelp();
            
/* ---- MAIN INFORMATIONS FOR USER ---- */
            
            printf("\n");
            printf("Guessed letters: \t\t\t");
            printf("%s", oneletter);
            printf("\n");
            printf("Word to be guessed: \t\t\t%s\n\n", word_check);
            
/* ---- PRINT GALLOWS FOR EVERY WRONG LETTER OR WORD ---- */
            
            while(1)
            {
                gallows(attempts);
                break;
            }
            
/* ---- PRINT WIN MESSAGE ---- */
            
            if (gamestatus == 1)
            {
                printf(BOLD"You won :-)\n\n"RESET);
                break;
            }
            
/* ---- PRINT LOSE MESSAGE ---- */
            
            if (attempts < 1)
            {
                printf(BOLD"You lost :-C\n\n"RESET);
                break;
            }
        }
        
        printf("The correct word is\t\t\t"BOLD"%s"RESET"\n\n", word);
        
/* ---- DEBUG INFORMATION OF EVERY STRING ---- */
        
#if DEBUG
#if defined(__APPLE__) && defined(__MACH__)
        printf(RED"sizeof word_check:\t%lu\n"RESET, sizeof(word_check));
        printf(RED"sizeof oneletter:\t%lu\n"RESET, sizeof(oneletter));
        printf(RED"sizeof letter:\t\t%lu\n"RESET, sizeof(letter));
        printf(RED"sizeof word:\t\t%lu\n"RESET, sizeof(word));
#endif
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
        printf(RED"sizeof word_check:\t%d\n"RESET, sizeof(word_check));
        printf(RED"sizeof oneletter:\t%d\n"RESET, sizeof(oneletter));
        printf(RED"sizeof letter:\t\t%d\n"RESET, sizeof(letter));
        printf(RED"sizeof word:\t\t%d\n"RESET, sizeof(word));
#endif
#endif
        
/* ---- RESET STRINGS, SOMEHOW PROGRAMM SAVES OLD CHARACTERS IF YOU RERUN IT ---- */
        
        memset(word_check,0,sizeof(word_check));
        memset(oneletter,0,sizeof(oneletter));
        memset(letter,0,sizeof(letter));
        memset(word,0,sizeof(word));
        
/* ---- DEBUG INFORMATION FOR EVERY STRING AFTER MEMSET ---- */
        
#if DEBUG
        printf(RED"wordcheck:%s.\n"RESET, word_check);
        printf(RED"oneletter:%s.\n"RESET, oneletter);
        printf(RED"letter:%s.\n"RESET, letter);
        printf(RED"word:%s.\n"RESET, word);
        printf(RED"sizeof oneletter: "BOLDRED"%lu\n"RESET, sizeof(oneletter));
#endif

/*------------------------------------------------------------------*/
/* Q U I T   M E S S A G E                                          */
/*------------------------------------------------------------------*/
        
#if WITHAGAIN
        printf("To restart press any button, If you want to quit press 'Q' and confirm with backspace > ");
        
        fseek(stdin,0,SEEK_END);

#if defined(__APPLE__) && defined(__MACH__) || defined(_WIN32) || defined (_WIN64)
        scanf("%c", &playagain);
#endif
        
#if defined(__linux__)
        scanf(" %c", &playagain);
#endif
        
        if (playagain == 'Q' || playagain == 'q')
        {
            break;
        }
#endif
        
#if WITHOUTAGAIN
        printf("Press any button and confirm with backspace to quit > ");
        fseek(stdin,0,SEEK_END);
        
#if defined(__APPLE__) && defined(__MACH__) || defined(_WIN32) || defined (_WIN64)
        scanf("%c", &playagain);
#endif
        
#if defined(__linux__)
        scanf(" %c", &playagain);
#endif

        break;
#endif
        
        clear();
        
        fseek(stdin,0,SEEK_END);
        fflush(stdin);
    }
    }
    
    return 0;
}

/*------------------------------------------------------------------*/
/* S U B R O U T I N E   L I N E S                                  */
/*------------------------------------------------------------------*/

void helpdesk_1(void)
{
    printf("\n");
    printf(BOLD"HANGMAN @ v2.0\n"RESET);
    printf(BOLD"Created by Sebastian Dichler, 2016\n"RESET);
    printf("For more information use \"h\", \"H\" or \"?\"as parameter\n\n");
#if DEBUG
    printf(BOLDRED"!!!! DEBUG MODE ACTIVE !!!!\n"RESET);
    printf(BOLDRED"EVERYTHING APPEARING RED IS A DEBUGMESSAGE (not with windows)\n\n"RESET);
#endif
}

void helpdesk_2(void)
{
    printf("\n");
    printf(BOLD"HANGMAN @ v2.0\n"RESET);
    printf(BOLD"Created by Sebastian Dichler, 2016\n"RESET);
    printf("Correct writing of startup:\n");
    printf(BOLD"Against Computer "RESET"./PROGRAMNAME.out \"C\"\n");
    printf(BOLD"Against Player\t "RESET"./PROGRAMNAME.out \"P\"\n\n");
    printf("If you play against a player your word must be a string without");
    printf(" any digits and\nno vowels.\n");
    printf("If you play PvP please make sure that the word youre searching for");
    printf(" has less than 20 characters\n");
#if DEBUG
    printf(BOLDRED"!!!! DEBUG MODE ACTIVE !!!!\n"RESET);
    printf(BOLDRED"EVERYTHING APPEARING RED IS A DEBUGMESSAGE (not with windows)\n\n"RESET);
#endif
}

void gallows(int i)
{
    switch (i)
    {
        case 15:
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);
            break;
            
        case 14:
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);
            break;
            
        case 13:
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);
            break;
            
        case 12:
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);
            break;
            
        case 11:
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);
            break;
            
        case 10:
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);
            break;
            
        case 9:
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);
            break;
            
        case 8:
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|__|__________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);

            break;
            
        case 7:
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|  _____        |\n"RESET);
            printf(BOLD"|  |/           |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|__|__________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);
            break;
            
        case 6:
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|  _____        |\n"RESET);
            printf(BOLD"|  |/  |        |\n"RESET);
            printf(BOLD"|  |   O        |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|__|__________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);
            break;
            
        case 5:
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|  _____        |\n"RESET);
            printf(BOLD"|  |/  |        |\n"RESET);
            printf(BOLD"|  |   O        |\n"RESET);
            printf(BOLD"|  |   |        |\n"RESET);
            printf(BOLD"|  |   |        |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|__|__________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);
            break;
            
        case 4:
            
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|  _____        |\n"RESET);
            printf(BOLD"|  |/  |        |\n"RESET);
            printf(BOLD"|  |   O        |\n"RESET);
            printf(BOLD"|  |  \\|/       |\n"RESET);
            printf(BOLD"|  |   |        |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|__|__________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);
            break;
            
        case 3:
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|  _____        |\n"RESET);
            printf(BOLD"|  |/  |        |\n"RESET);
            printf(BOLD"|  |   O        |\n"RESET);
            printf(BOLD"|  |  \\|/       |\n"RESET);
            printf(BOLD"|  |   |        |\n"RESET);
            printf(BOLD"|  |  /         |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|__|__________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);
            break;
            
        case 2:
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|  _____        |\n"RESET);
            printf(BOLD"|  |/  |        |\n"RESET);
            printf(BOLD"|  |   O        |\n"RESET);
            printf(BOLD"|  |  \\|/       |\n"RESET);
            printf(BOLD"|  |   |        |\n"RESET);
            printf(BOLD"|  |  / \\       |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|__|__________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);
            break;
            
        case 1:
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|  _____        |\n"RESET);
            printf(BOLD"|  |/  |        |\n"RESET);
            printf(BOLD"|  |   O        |\n"RESET);
            printf(BOLD"|  |  /|\\       |\n"RESET);
            printf(BOLD"|  |   |        |\n"RESET);
            printf(BOLD"|  |  / \\       |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|__|__________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);
            break;
            
        case 0:
            printf(BOLD"#################\n"RESET);
            printf(BOLD"|  _____        |\n"RESET);
            printf(BOLD"|  |/  |        |\n"RESET);
            printf(BOLD"|  |   X        |\n"RESET);
            printf(BOLD"|  |  /|\\       |\n"RESET);
            printf(BOLD"|  |   |        |\n"RESET);
            printf(BOLD"|  |  / \\       |\n"RESET);
            printf(BOLD"|  |            |\n"RESET);
            printf(BOLD"|__|__________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|_____________  |\n"RESET);
            printf(BOLD"|               |\n"RESET);
            printf(BOLD"#################\n\n"RESET);
            break;
            
        default:
            break;
    }
}

void clear(void)
{
#if defined(__APPLE__) && defined(__MACH__) || defined(__linux__)
    system("clear");
#endif
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
    helpdesk_1();
}

void clearNoHelp(void)
{
#if defined(__APPLE__) && defined(__MACH__) || defined(__linux__)
    system("clear");
#endif
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}


unsigned int catchClearString(char *input)
{
    int j;
    
    for (j = 0; j < strlen(input); j++)
    {
        if (input[j] == '\r')
        {
            input[j] = '\0';
        }
        if (input[j] == '\n')
        {
            input[j] = '\0';
        }
        if (input[j] == '\t')
        {
            input[j] = '\0';
        }
    }
    
    return 0;
}
