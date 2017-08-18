#include <termios.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#define LARGEUR_ECRAN 160
#define HAUTEUR_ECRAN 40
#define TEMPS 40000000

typedef struct curseur curseur;
struct curseur {
	int x;
	int y;
	int prev;
};

void    game_of_life()
{
    /*
    printf(" _______    ___     __   __   _____\n");
    printf("/######/   /###\   /##\_/##\ /####/\n");
    printf("|#|  __   /#/_\#\  |#|\#/|#| |#|_  \n");
    printf("|#| |##\ |#######| |#|   |#| |###| \n");
    printf("|#\__|#| |#|   |#| |#|   |#| |#|__ \n");
    printf("\######/ |#|   |#| |#|   |#| \####\\n");

    printf("   ___     _____\n");
    printf("  /###\   /####/\n");
    printf(" /#/ \#\  |#|_  \n");
    printf("|#|   |#| |###| \n");
    printf(" \#\_/#/  |#|   \n");
    printf("  \###/   |/    \n");
    
    printf(" __      _   _____  _____\n");
    printf("|#|     /#\ /####/ /####/\n");
    printf("|#|     |#| |#|_   |#|_  \n");
    printf("|#|     |#| |###|  |###| \n");
    printf("|#\___  |#| |#|    |#|__ \n");
    printf("\#####\ \#/ |/     \####\\n");*/

    printf(" _______    ___     __   __   _____\n");
    printf("/######/   /###\\   /##\\_/##\\ /####/\n");
    printf("|#|  __   /#/_\\#\\  |#|\\#/|#| |#|_  \n");
    printf("|#| |##\\ |#######| |#|   |#| |###| \n");
    printf("|#\\__|#| |#|   |#| |#|   |#| |#|__ \n");
    printf("\\######/ |#|   |#| |#|   |#| \\####\\\n");

    printf("   ___     _____\n");
    printf("  /###\\   /####/\n");
    printf(" /#/ \\#\\  |#|_  \n");
    printf("|#|   |#| |###| \n");
    printf(" \\#\\_/#/  |#|   \n");
    printf("  \\###/   |/    \n");
    
    printf(" __      _   _____  _____\n");
    printf("|#|     /#\\ /####/ /####/\n");
    printf("|#|     |#| |#|_   |#|_  \n");
    printf("|#|     |#| |###|  |###| \n");
    printf("|#\\___  |#| |#|    |#|__ \n");
    printf("\\#####\\ \\#/ |/     \\####\\\n");

}

void    clear_screen(int nbr)
{
    int i = 0;

    while(i < nbr)
    {
        printf("\n");
        i++;
    }
}

void 	count_cells(int grid[LARGEUR_ECRAN][HAUTEUR_ECRAN])
{
	int y = 1;
	int x = 1;
	int count = 0;

	while(y < HAUTEUR_ECRAN - 1)
	{
		while(x < LARGEUR_ECRAN - 2)
		{
			if(grid[x][y] == 3)
				count++;
			x++;
		}
		x = 1;
		y++;
	}
	printf("Nombre de cellules = %d", count);
}

void    mode_raw(int activer) 
{ 
    static struct termios cooked; 
    static int raw_actif = 0; 
  
    if (raw_actif == activer) 
        return; 
  
    if (activer) 
    { 
        struct termios raw; 
  
        tcgetattr(STDIN_FILENO, &cooked); 
  
        raw = cooked; 
        cfmakeraw(&raw); 
        tcsetattr(STDIN_FILENO, TCSANOW, &raw); 
    } 
    else 
        tcsetattr(STDIN_FILENO, TCSANOW, &cooked); 
  
    raw_actif = activer; 
}

int unix_text_kbhit(void) 
{ 
    struct timeval tv = { 0, 0 }; 
    fd_set readfds; 
  
    FD_ZERO(&readfds); 
    FD_SET(STDIN_FILENO, &readfds); 
  
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) == 1; 
}

void	new_generation(int grid[LARGEUR_ECRAN][HAUTEUR_ECRAN], int mortMax, int mortMin, int nait)
{
	count_cells(grid);
	int gridPatch[LARGEUR_ECRAN][HAUTEUR_ECRAN];
	int x = 1;
	int y = 1;
	int count = 0;

	while(y < HAUTEUR_ECRAN - 1)
	{
		while(x < LARGEUR_ECRAN - 1)
		{
			if(grid[x - 1][y] == 3)
				count++;

			if(grid[x - 1][y - 1] == 3)
				count++;

			if(grid[x][y - 1] == 3)
				count++;

			if(grid[x + 1][y - 1] == 3)
				count++;

			if(grid[x + 1][y] == 3)
				count++;

			if(grid[x + 1][y + 1] == 3)
				count++;

			if(grid[x][y + 1] == 3)
				count++;

			if(grid[x - 1][y + 1] == 3)
				count++;

			if(count == nait && grid[x][y] == 2)
			{
				gridPatch[x][y] = 3;
			}
			if(count > mortMax || count < mortMin)
			{
				gridPatch[x][y] = 2;
			}
			// if(count == 2 && grid[x][y] == 2)
			// {
			// 	gridPatch[x][y] = 2;
			// }
			// if((count == 2 || count == 3) && grid[x][y] == 3)
			// {
			// 	gridPatch[x][y] = 3;
			// }
			count = 0;
			x++;
		}
		x = 1;
		y++;
	}
	x = 1;
	y = 1;
	count = 0;
	while(y < HAUTEUR_ECRAN - 1)
	{
		while(x < LARGEUR_ECRAN - 1)
		{
			if(gridPatch[x][y] == 3 || gridPatch[x][y] == 2)
				grid[x][y] = gridPatch[x][y];
			//printf("%d\n", gridPatch[x][y]);
			count++;
			x++;
		}
		x = 1;
		y++;
	}
	// printf("count = %d\n", count);
	// printf("%d\n", (LARGEUR_ECRAN - 2) * (HAUTEUR_ECRAN - 1));
	// exit(EXIT_SUCCESS);
}

void	init_patern(int grid[LARGEUR_ECRAN][HAUTEUR_ECRAN], curseur joueur[1], char c)
{
	if(c == 'z' && joueur[0].y > 1)
	{
		grid[joueur[0].x][joueur[0].y] = joueur[0].prev;
		joueur[0].y -= 1;
		joueur[0].prev = grid[joueur[0].x][joueur[0].y];
		grid[joueur[0].x][joueur[0].y] = 4;
	}
	if(c == 's' && joueur[0].y < HAUTEUR_ECRAN - 2)
	{
		grid[joueur[0].x][joueur[0].y] = joueur[0].prev;
		joueur[0].y += 1;
		joueur[0].prev = grid[joueur[0].x][joueur[0].y];
		grid[joueur[0].x][joueur[0].y] = 4;
	}
	if(c == 'q' && joueur[0].x > 1)
	{
		grid[joueur[0].x][joueur[0].y] = joueur[0].prev;
		joueur[0].x -= 1;
		joueur[0].prev = grid[joueur[0].x][joueur[0].y];
		grid[joueur[0].x][joueur[0].y] = 4;
	}
	if(c == 'd' && joueur[0].x < LARGEUR_ECRAN - 2)
	{
		grid[joueur[0].x][joueur[0].y] = joueur[0].prev;
		joueur[0].x += 1;
		joueur[0].prev = grid[joueur[0].x][joueur[0].y];
		grid[joueur[0].x][joueur[0].y] = 4;
	}
	if(c == 'm')
	{
		if(grid[joueur[0].x][joueur[0].y] == 3)
			grid[joueur[0].x][joueur[0].y] = 2;
		else
			grid[joueur[0].x][joueur[0].y] = 3;
		joueur[0].prev = grid[joueur[0].x][joueur[0].y];
	}
}

void    init_grid(int grid[LARGEUR_ECRAN][HAUTEUR_ECRAN])
{
    int x = 0;
    int y = 0;
    while(y < HAUTEUR_ECRAN)
    {
        while(x < LARGEUR_ECRAN)
        {
            grid[x][y] = 2;
            x++;
        }
        y++;
        x = 0;
    }
    y = 0;
    x = 0;
    while(x < LARGEUR_ECRAN)
    {
        grid[x][0] = 0;
        grid[x][HAUTEUR_ECRAN - 1] = 0;
        x++;
    }
    while(y < HAUTEUR_ECRAN)
    {
        grid[0][y] = 1;
        grid[LARGEUR_ECRAN - 1][y] = 1;
        y++;
    }
    grid[1][1] = 4;
}

void    print_grid(int grid[LARGEUR_ECRAN][HAUTEUR_ECRAN])
{
    int x = 0;
    int y = 0;
    printf("\n=== GAME OF LIFE ===\n");
    while(y < HAUTEUR_ECRAN)
    {
        while(x < LARGEUR_ECRAN)
        {
            if (grid[x][y] == 0)
                printf("-");
            else if (grid[x][y] == 1)
                printf("|");
            else if (grid[x][y] == 2)
                printf(" ");
            else if (grid[x][y] == 3)
                printf("O");
            else if (grid[x][y] == 4)
                printf(".");
            x++;
        }
        y++;
        x = 0;
        //if (y < HAUTEUR_ECRAN)
        printf("\n");   
    }
}

int		moteur_jeu(int regles)
{
	int mortMin = 2;
	int mortMax = 3;
	int nait = 3;

	int over = 0;
	int wait = 0;
	int time = 0;
	int grid[LARGEUR_ECRAN][HAUTEUR_ECRAN];
	char move[2];
	char c;
	curseur joueur[1];
	joueur[0].x = 1;
	joueur[0].y = 1;
	joueur[0].prev = 2;

	time = TEMPS / 2;
	c = 'a';
	init_grid(grid);
	while(!over && regles == 3)
	{
		while(c < '0' || c > '9')
		{
			printf("=== GAME OF LIFE ===\n");
			printf("Règles de la vie\n");
			clear_screen(42 - 2);
			printf("Entrez le nombre de cellules vivantes adjacentes nécéssaire à la naissance d'une nouvelle cellule : ");
			c = getchar();
			if(c < '0' || c > '9')
				printf("Saisie innapropriée.\n");
		}
		nait = c - 48;
		c = 'a';
		while(c < '0' || c > '9')
		{
			printf("=== GAME OF LIFE ===\n");
			printf("Règles de la vie\n");
			clear_screen(42 - 2);
			printf("Entrez le nombre minimum de cellules vivantes adjacentes nécéssaire à la survie d'une cellule : ");
			c = getchar();
			if(c < '0' || c > '9')
				printf("Saisie innapropriée.\n");
		}
		mortMin = c - 48;
		c = 'a';
		while(c < '0' || c > '9')
		{
			printf("=== GAME OF LIFE ===\n");
			printf("Règles de la vie\n");
			clear_screen(42 - 2);
			printf("Entrez le nombre maximum de cellules vivantes adjacentes permettant la survie d'une cellule : ");
			c = getchar();
			if(c < '0' || c > '9')
				printf("Saisie innapropriée.\n");
		}
		mortMax = c - 48;
		c = 'a';
		over = 1;
		regles = 0;
	}
	over = 0;
	while(!over)
	{
		print_grid(grid);
		//count_cells(grid);
		printf("Entrez un patern de base (z, q, s, d, m, & f pour valider): \n");
		mode_raw(1);
		if(unix_text_kbhit() == 1)
			fgets(move, 2, stdin);
		mode_raw(0);
		if(move[0] == 'z' || move[0] == 'q' || move[0] == 's' || move[0] == 'd' || move[0] == 'm')
			init_patern(grid, joueur, move[0]);
		if(move[0] == 'f')
			over = 1;
		move[0] = '\n';
		while(wait < time)
			wait++;
		wait = 0;
	}
	//count_cells(grid);
	over = 0;
	time = TEMPS;
	while(!over)
	{
		//count_cells(grid);
		new_generation(grid, mortMax, mortMin, nait);
		print_grid(grid);
		printf("Entrez 'q' pour quitter la simulation.\n");
		mode_raw(1);
		if(unix_text_kbhit() == 1)
			fgets(move, 2, stdin);
		mode_raw(0);
		if(move[0] == 'q')
			over = 1;
		if(move[0] == 'u')
			time = TEMPS / 2;
		if(move[0] == 'i')
			time = TEMPS;
		if(move[0] == 'o')
			time = TEMPS * 2;
		if(move[0] == 'p')
			time = TEMPS * 4;
		while(wait < time)
			wait++;
		wait = 0;
	}
	//sprintf("c", stdin);
	return(0);
}

int ft_str_compare(char str[])
{
    int i = 0;
    char oui[] = "oui\0";
    char non[] = "non\0";
    char vie[] = "vie\0";

    while(str[i] != '\0' && (str[i] == oui[i] || str[i] == non[i] || str[i] == vie[i]))
        i++;
    if(i != 3)
        return(2);
    else
    {
        i = 0;
        while(str[i] == oui[i] && str[i] != '\0')
            i++;
        if(str[i] == '\0')
            return (1);
        i = 0;
        while(str[i] == vie[i] && str[i] != '\0')
            i++;
        if(str[i] == '\0')
            return (3);
        else
            return (0);
    }
    return(0);
}

int intro(void)
{
    char str[4];
    int i = 1;
    char c;

    str[3] = '\0';
    printf("=== GAME OF LIFE ===\n");
    game_of_life();
    printf("\n\n(par Idriss Cornuau)\n\n");
    printf("Nouvelle simulation ?\n");
    clear_screen(42 - 25);
    printf("oui / non / (ou vie si vous voulez modifier les Règles de la Vie)\n");
    while(i)
    {
        fgets(str, sizeof str, stdin);
        while((c = getchar()) != '\n' && c != EOF)
        {}
        if(ft_str_compare(str) == 0)
        {
            i = 0;
            return(0);
        }
        else if(ft_str_compare(str) == 1)
        {
            i = 0;
            return(1);
        }
        else if(ft_str_compare(str) == 3)
        {
        	i = 0;
        	return(3);
        }
        else
            printf("Mauvaise saisie, recommencez.\n");
    }
    return (0);
}

int		main(int argc, char **argv)
{
	int over = 0;
	int intr;
	while(!over)
	{
		intr = intro();
		if(intr != 0)
			moteur_jeu(intr);
		else
			over = 1;
	}
	return(0);
}