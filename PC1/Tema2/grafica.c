#include "info.h"
void fa_spanzuratoare_meniu();

/*
	Acesta este meniul principal al jocului. Toate functiile
	de tip meniu au 'k' care tine minte pe ce pozitie se afla
	jucatorul pentru o afisare distinctiva.
*/
void afisare_meniu(int k)
{

	int x, y;
	
	char new[] = "New Game";
	char resume[] = "Resume Game";
	char quit[] = "Quit";
	char settings[] = "Settings";

	WINDOW *WIN;
	WINDOW *New_Game;
	WINDOW *Resume_Game;
	WINDOW *Quit;
	WINDOW *Settings;
	
	clear();
	getmaxyx(stdscr, x, y);
	
	WIN = newwin(x/2, y/2, x/2, 0);
	wborder(WIN, '-', '-', '=', '=', 'O', 'O', 'O', 'O');

	wbkgd(stdscr, COLOR_PAIR(2));
	wbkgd(WIN, COLOR_PAIR(2));

	New_Game = newwin(1, 13,  3*x/4 - 1, y/4-6);
	Resume_Game = newwin(1, 13, 3*x/4, y/4-6);
	Settings = newwin(1,13,3*x/4+1, y/4-6);
	Quit = newwin(1, 13, 3*x/4 + 2, y/4-6);

	wbkgd(New_Game, COLOR_PAIR(2));
	wbkgd(Resume_Game, COLOR_PAIR(2));
	wbkgd(Settings, COLOR_PAIR(2));
	wbkgd(Quit, COLOR_PAIR(2));
        
	if( k == 0 )
	{
		wattron(New_Game,COLOR_PAIR(1));
		wattron(New_Game, A_BOLD);
		
		mvwprintw(New_Game, 0, 2, "%s", new);
		mvwprintw(Resume_Game, 0, 1, "%s", resume);
		mvwprintw(Settings, 0, 2, "%s", settings);
		mvwprintw(Quit, 0, 4, "%s", quit);
	}

	else if( k == 1 )
	{
		wattron(Resume_Game, COLOR_PAIR(1));
		wattron(Resume_Game, A_BOLD);

		mvwprintw(New_Game, 0, 2, "%s", new);
		mvwprintw(Resume_Game, 0, 1, "%s", resume);
		mvwprintw(Settings, 0, 2, "%s", settings);
		mvwprintw(Quit, 0, 4, "%s", quit);
	}
	
	else if( k == 2 )
	{
		wattron(Settings, COLOR_PAIR(1));
		wattron(Settings, A_BOLD);
		
		mvwprintw(New_Game, 0, 2, "%s", new);
		mvwprintw(Resume_Game, 0, 1, "%s", resume);
		mvwprintw(Settings, 0, 2, "%s", settings);
		mvwprintw(Quit, 0, 4, "%s", quit);
	}
	
	else
	{
		wattron(Quit, COLOR_PAIR(1));
		wattron(Quit, A_BOLD);
		
		mvwprintw(New_Game, 0, 2, "%s", new);
		mvwprintw(Resume_Game, 0, 1, "%s", resume);
		mvwprintw(Settings, 0, 2, "%s", settings);
		mvwprintw(Quit, 0, 4, "%s", quit);
	}

	fa_spanzuratoare_meniu();

	refresh();
	wrefresh(WIN);
	wrefresh(New_Game);
	wrefresh(Resume_Game);
	wrefresh(Quit);
	wrefresh(Settings);

	delwin(WIN);
	delwin(New_Game);
	delwin(Resume_Game);
	delwin(Quit);
	delwin(Settings);

}

/*
	Realizeaza un cerc.
	(Folosit pentru spanzuratoarea din meniul principal.)
*/
void cerc(WINDOW *fereastra, int a, int b, int h)
{
	int x, y;
	float unghi;

	for(unghi = 0.0f; unghi < 360.f; unghi += 1.0f)
	{
		x = h + (int)(h * cos(rad(unghi)) );
		y = h + (int)(h * sin(rad(unghi)) );
		mvwaddch(fereastra, a + x, b+y, 'O');
	}
	
	refresh();
	wrefresh(fereastra);	
}

/*
	Spanzuratoarea din meniul principal.
	Succesiunea de x si y este pentru aranjarea in fereastra meniului.
*/
void fa_spanzuratoare_meniu()
{
	int x, y;
	int i,j;
	int x_spanz, y_spanz;
	WINDOW *spanzuratoare;

	getmaxyx(stdscr, x, y);
	spanzuratoare = newwin(3*x/4, y/2, x/4, y/2);
	wbkgd(spanzuratoare, COLOR_PAIR(2));

	getmaxyx(spanzuratoare, x_spanz, y_spanz);
	
	for(i = 0; i < 3*x/12; i++)
	{
		for(j = 0; j < 3*x/8; j++)
		{
			mvwprintw(spanzuratoare,x_spanz-i,
			y_spanz/2 - y/16 + j, "=");
		}
	}

	for(i = 0; i < 3*x/4 - 1 - 3*x/13; i++)
	{
		for(j = 0; j < 3; j++)
		{
			mvwprintw(spanzuratoare,x_spanz-i-3*x/12,
			y_spanz/2 - y/16 + j + 3*x/16, "|");
		}
	}

	for(i = 0; i < y/4; i++)
	{
		mvwprintw(spanzuratoare,0,
		y_spanz/2 - 3*y/16 + i + 3*x/16, "_");
	}

	for(i = 0; i < 3*x/32; i++)
	{
		mvwprintw(spanzuratoare, 1 + i,
		y_spanz/2 - 3*y/16 + 3*x/16, "|");

		mvwprintw(spanzuratoare, 1 + i,
		y_spanz/2 - 3*y/16 + 3*x/16 + y/4, "|");
	}
	
	cerc(spanzuratoare, 1 + 3*x/32,
	y_spanz/2 + (5*x - 9*y)/48, x/12);

	for(i = 0; i < 3*x/64+2; i++)
	{
		mvwprintw(spanzuratoare, 1 + i + x/6 + 3*x/32,
		y_spanz/2 - 3*y/16 + 3*x/16, "|");
		
		mvwprintw(spanzuratoare, 1 + i + x/6 + 3*x/32,
		y_spanz/2 - 3*y/16 + 3*x/16 + 1 + i, "\\");
		
		mvwprintw(spanzuratoare, 1 + i + x/6 + 3*x/32,
		y_spanz/2 - 3*y/16 + 3*x/16 - 1 - i, "/");

			
		mvwprintw(spanzuratoare,  i + 3*x/12 + 3*x/32,
		y_spanz/2 - 3*y/16 + 3*x/16 + 1 + i, "\\");
		
		mvwprintw(spanzuratoare,  i + 3*x/12 + 3*x/32,
		y_spanz/2 - 3*y/16 + 3*x/16 - 1 - i, "/");
	}

	cerc(spanzuratoare, 1 + 3*x/32,
	y_spanz/2 + (5*x + 3*y)/48,  x/12);

	for(i = 0; i < 3*x/64+2; i++)
	{
		mvwprintw(spanzuratoare, 1 + i + x/6 + 3*x/32,
		y_spanz/2 + (3*x + y)/16, "|");
		
		mvwprintw(spanzuratoare, 1 + i + x/6 + 3*x/32,
		y_spanz/2 + (3*x + y)/16 + 1 + i, "\\");

		mvwprintw(spanzuratoare, 1 + i + x/6 + 3*x/32,
		y_spanz/2 + (3*x + y)/16 - 1 - i, "/");
		

		mvwprintw(spanzuratoare,  i + 3*x/12 + 3*x/32,
		y_spanz/2 + (3*x + y)/16 + 1 + i, "\\");
		
		mvwprintw(spanzuratoare,  i + 3*x/12 + 3*x/32,
		y_spanz/2 + (3*x + y)/16 - 1 - i, "/");
	}

		 

	refresh();
	wrefresh(spanzuratoare);
	refresh();

	delwin(spanzuratoare);
}

/*
	Este panoul specificat in enunt.
*/
void afisare_panou_control(WINDOW *fereastra, int x, int y, propozitie hangman)
{
	time_t t = time(NULL);
	struct tm *data;
	int i;
	
	wbkgd(fereastra, COLOR_PAIR(2));
	data = localtime(&t);
	
	wprintw(fereastra, "%sVieti: %d\n\"Q\" - parasire joc\n\"N\" - inceperea unei noi sesiuni\n\n",
	asctime(data), hangman.vieti);

	wprintw(fereastra, "Fraze jucate: %d\n", hangman.numar_fraze_jucate);
	wprintw(fereastra, "Numar caractere: %d\n\n", hangman.numar_total_caractere);

	wprintw(fereastra, "Caractere epuizate:\n");
	
	for(i = 0; i < 10; i++)
	{
		if(hangman.litere[i] == TRUE)
		{
			wprintw(fereastra, "%c ", i + 48);
		}
	}
	
	for(i = 0; i < 26; i++)
	{
		if(hangman.litere[i + 10] == TRUE)
		{
			wprintw(fereastra, "%c ", i + 97);
		}
	}
	
	for(i = 0; i < 6; i++)
	{
		if(hangman.litere[i + 36] == TRUE)
		{
			wprintw(fereastra, "%c ", hangman.caractere_speciale[i]);
		}
	}
	

}

/*
	Are loc unificarea cuvintelor pentru a putea fi afisat textul.
*/
void cuvinte_de_afisat(char *sir, propozitie hangman)
{
	int i,j;
	int aux = 0;
	
	for(i = 0; i < hangman.numar_cuvinte; i++)
	{
		for(j = 0; j < strlen(hangman.cuvinte[i]); j++)
		{
			if(hangman.ascunse[i][j] == 1)
			{
				sir[aux++] = hangman.cuvinte[i][j];
			}
			
			else
			{
				sir[aux++] = '_';
			}	
		}

		sir[aux++] = ' ';
	}
	
	sir[aux] = '\0';
}

/*
	Partea de sub spanzuratoare din timpul jocului este destinata
	textului. Acesta incepe imediat sub spanzuratoare pentru a avea loc
	un numar de 2000 de caractere introduse.
*/
void afisare_stadiu(propozitie hangman)
{
	int x, y;
	char pt_afisare[2000];

	WINDOW *fereastra_joc;
	WINDOW *panou_control;

        clear();
	getmaxyx(stdscr, x, y);

	panou_control = newwin(x/2, y/4 + 10, 0, 3*y/4 - 10);
	afisare_panou_control(panou_control, x, y, hangman);

	fereastra_joc=newwin(x/2-1, y, x/2 + 1, 0);
	wbkgd(fereastra_joc, COLOR_PAIR(2));

	cuvinte_de_afisat(pt_afisare,hangman);
	
	wprintw(fereastra_joc, "%s", pt_afisare);
	
	refresh();
	wrefresh(fereastra_joc);
	wrefresh(panou_control);

	delwin(fereastra_joc);
	delwin(panou_control);
}

/*
	'k' reprezinta numarul de vieti. In functie de acesta, se afiseaza
	mai mult sau mai putin din corpul spanzuratului.
*/
void spanzuratoare_joc(int k)
{
	int x,y;
	int i;
	
	WINDOW *spanzuratoare;

	getmaxyx(stdscr,x,y);
	spanzuratoare = newwin(x/2, y/2,0,0);
	wbkgd(spanzuratoare, COLOR_PAIR(3));

	mvwprintw(spanzuratoare,x/2 - 1,2,"========================");
	mvwprintw(spanzuratoare,x/2 - 2,2,"========================");

	for(i = 0; i < 20; i++)
	{
		mvwprintw(spanzuratoare,x/2 - 3 - i, 2 + 11,"||");
	}
	
	for(i = 0; i < 30; i++)
	{
		mvwprintw(spanzuratoare, x/2 - 23, 2 + 7 + i, "_");
	}
	
	for(i = 0; i < 10; i++)
	{
		mvwprintw(spanzuratoare, x/2 - 2 - 20 + i ,  7 + 30, "|");
	}
	
	if(k < 6)
	{
		mvwprintw(spanzuratoare,x/2 - 12 , 7 + 29 , "___");
		mvwprintw(spanzuratoare,x/2 - 11 , 7 + 28 , "/. .\\");
		mvwprintw(spanzuratoare,x/2 - 10 , 7 + 28 , "\\_-_/");
		
	}
	
	if(k < 5)
	{
		mvwprintw(spanzuratoare, x/2 - 9, 7 + 30, "|");
		mvwprintw(spanzuratoare, x/2 - 8, 7 + 30, "|");
		mvwprintw(spanzuratoare, x/2 - 7, 7 + 30, "|");
		mvwprintw(spanzuratoare, x/2 - 6, 7 + 30, "|");
	}
	
	if(k<4)
	{
		mvwprintw(spanzuratoare,x/2 - 8,7+31,"\\");
		mvwprintw(spanzuratoare,x/2 - 7,7+32,"\\");
		mvwprintw(spanzuratoare,x/2 - 6,7+33,"\\");
	}
	
	if(k<3)
	{
		mvwprintw(spanzuratoare,x/2 - 8,7+29,"/");
		mvwprintw(spanzuratoare,x/2 - 7,7+28,"/");
		mvwprintw(spanzuratoare,x/2 - 6,7+27,"/");
	}
	
	if(k<2)
	{
		mvwprintw(spanzuratoare,x/2 - 5,7+31,"\\");
		mvwprintw(spanzuratoare,x/2 - 4,7+32,"\\");
		mvwprintw(spanzuratoare,x/2 - 3,7+33,"\\");
	}
	
	if(k<1)
	{
		mvwprintw(spanzuratoare,x/2 - 5,7+29,"/");
		mvwprintw(spanzuratoare,x/2 - 4,7+28,"/");
		mvwprintw(spanzuratoare,x/2 - 3,7+27,"/");
	}
	
	refresh();
	wrefresh(spanzuratoare);

	delwin(spanzuratoare);
}

/*
	[BONUS]
	La fiecare deschidere a programului sau modificare a setarilor
	se apeleaza aceasta functie pentru a modifica perechile 2 si 3
	de culori.
*/
void initializare_culori()
{
	FILE *in;
	culori asta;
	
	asta.spanzuratoare = 3;
	asta.fundal = 0;
	asta.scris = 7;
	
	init_pair(1, COLOR_CYAN, COLOR_BLUE);	
	
	in = fopen("settings", "rb");
	
	if(in == NULL)	
        {
		init_pair(2, asta.scris, asta.fundal);
		init_pair(3, asta.spanzuratoare, asta.fundal);
		
		in = fopen("settings", "wb");
		
		fwrite(&asta, sizeof(culori), 1,in);
		

	}
	else
	{

		fread(&asta, sizeof(culori), 1, in);

		init_pair(2, asta.scris, asta.fundal);
		init_pair(3, asta.spanzuratoare, asta.fundal);


	}

	fclose(in);
}

/*
	La selectarea butonului 'Settings' din meniu se realizeaza
	un alt meniu pentru a selecta ce culoare sa se modifice
	(fundal, text sau spanzuratoare).
*/
void afisare_setari(int k)
{
	int x,y;
	char fundal[] = "Background";
	char scris[] = "Text";
	char spanzuratoare[] = "Gibbet";
	char back[] = "Back";

	WINDOW *meniu;
	WINDOW *Fundal;
	WINDOW *Scris;
	WINDOW *Spanzuratoare;
	WINDOW *Back;
	
	getmaxyx(stdscr,x,y);

	meniu = newwin(x/2,y/4, x/4, y/4);
	wborder(meniu, '-', '-', '=', '=', 'O', 'O', 'O', 'O');
	wbkgd(meniu, COLOR_PAIR(2));

	clear();
	refresh();
	wrefresh(meniu);

	Fundal = newwin(1, 10, x/2 - 1, y/2-y/8-5);
	Scris = newwin(1, 4, x/2, y/2-y/8-2);
	Spanzuratoare = newwin(1, 6, x/2 + 1, y/2-y/8-3);
	Back = newwin(1,4,x/2+2,y/2-y/8-2);
	
	wbkgd(Fundal, COLOR_PAIR(2));
	wbkgd(Scris, COLOR_PAIR(2));
	wbkgd(Spanzuratoare, COLOR_PAIR(2));
	wbkgd(Back, COLOR_PAIR(2));

	if( k == 0 )
	{
		wattron(Fundal,COLOR_PAIR(1));
		wattron(Fundal, A_BOLD);
		
		mvwprintw(Fundal, 0, 0, "%s", fundal);
		mvwprintw(Scris, 0, 0, "%s", scris);
		mvwprintw(Spanzuratoare, 0, 0, "%s", spanzuratoare);
		mvwprintw(Back, 0, 0, "%s", back);
	}
	
	else if( k == 1 )
	{
		wattron(Scris, COLOR_PAIR(1));
		wattron(Scris, A_BOLD);
		
		mvwprintw(Fundal, 0, 0, "%s", fundal);
		mvwprintw(Scris, 0, 0, "%s", scris);
		mvwprintw(Spanzuratoare, 0, 0, "%s", spanzuratoare);
		mvwprintw(Back, 0, 0, "%s", back);
	}

	else if( k == 2 )
	{
		wattron(Spanzuratoare, COLOR_PAIR(1));
		wattron(Spanzuratoare, A_BOLD);
                
		mvwprintw(Fundal, 0, 0, "%s", fundal);
		mvwprintw(Scris, 0, 0, "%s", scris);
		mvwprintw(Spanzuratoare, 0, 0, "%s", spanzuratoare);
		mvwprintw(Back, 0, 0, "%s", back);
        }

	else
	{
		wattron(Back, COLOR_PAIR(1));
		wattron(Back, A_BOLD);
                
		mvwprintw(Fundal, 0, 0, "%s", fundal);
		mvwprintw(Scris, 0, 0, "%s", scris);
		mvwprintw(Spanzuratoare, 0, 0, "%s", spanzuratoare);
		mvwprintw(Back, 0, 0, "%s", back);
	}
	
	refresh();
	wrefresh(Fundal);
	wrefresh(Scris);
	wrefresh(Spanzuratoare);
	wrefresh(Back);
	
	delwin(Fundal);
	delwin(Scris);
	delwin(Spanzuratoare);
	delwin(Back);
	delwin(meniu);
}

/*
	Dupa ce am ales ce vrem sa modificam, apare un alt meniu
	in care sunt afisate toate posibilitatile de culori.
*/
void afisare_culoare(int k)
{
	int x,y;
	int i;
	char afisare[9][7] = {"Black", "Red", "Green", "Yellow", "Blue",
				"Magenta", "Cyan", "White", "Back"};
	
	WINDOW *fereastra;
	WINDOW *afisari[9];

	getmaxyx(stdscr,x,y);
	
	fereastra = newwin(x/2, y/4, x/4, y/2);
	wborder(fereastra, '-', '-', '=', '=', 'O', 'O', 'O', 'O');
	wbkgd(fereastra, COLOR_PAIR(2));

	for(i = 0; i < 9; i++)
	{
		afisari[i] = newwin(1, 7, x/2 - 5 + i, y/2 + y/8);
		wbkgd(afisari[i], COLOR_PAIR(2));
	}
	
	wattron(afisari[k], COLOR_PAIR(1));
	wattron(afisari[k], A_BOLD);

	for(i = 0; i < 9; i++)
	{
		wprintw(afisari[i], "%s", afisare[i]);
	}

	refresh();
	wrefresh(fereastra);

	for(i = 0; i < 9; i++)
	{
		wrefresh(afisari[i]);
		delwin(afisari[i]);
	}
			
	delwin(fereastra);


}


