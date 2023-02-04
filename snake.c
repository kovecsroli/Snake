/*///////////////////////////////
| Snake game (with multiplayer) |
|	Only for windows			|
|								|
|	Kövecs Roland - 2020.11-12. |
///////////////////////////////*/

//szukseges header fileok includalasa
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

//hdir(head direction) tipusu irany valtozo
typedef enum hdir { Stop = 0, L, R, U, D } hdir;

//kigyo lancolt lista node-janak strukturaja
typedef struct snake {
	int x;
	int y;
	struct snake *next;
}snake;

//uj jatek inditasakor megjeleno menu fuggvenye
int mainmenu(int *height, int *width, bool *dsize) {
	int be; //jatekmod valasztas bemenet

	do{printf("Kerlek valassz jatekmodot! (1-single | 2-multiplayer)");//jatekmod beallitasa
		scanf("%d", &be);
	} while (be != 1 && be != 2);

	char def; //alapertelmezett palyameret bemenet
	do {
		printf("Az alapertelmezett palyamerettel jatszol? (y/n)");//palyameret beallitasa
		scanf(" %c", &def);
	} while (def != 'y' && def != 'n');
	if (def == 'n') {

		*dsize = false;
		do {
			printf("Add meg a palyameretet!\n hossz(>4): ");
			scanf("%d", width);
		} while (*width<5);
		
		do {
			printf("magassag(>4): ");
			scanf("%d", height);
		} while (*height < 5);
		}

		
	//tajekoztato uzenetek
	if (be==2) {
		system("cls"); printf("Valasztott jatekmod: ket jatekos(2)\n\n  Iranyitas:\n -1. jatekos: a-bal,w-fel,s-le,d-jobb\n -2. jatekos: j-bal,i-fel,k-le,l-jobb\n");
		printf("\n\nNyomj barmilyen gombot a jatek kezdesehez"); 
	}

	if (be == 1) {
		system("cls"); printf("Valasztott jatekmod: egy jatekos(1)\n\n  Iranyitas: a-bal,w-fel,s-le,d-jobb\n");
		printf("\n\nNyomj barmilyen gombot a jatek kezdesehez"); 
	}

	return be;
}

//pontszam elmentese nevvel rekord tablaba fuggveny
void save(int *scorebe)
{
	printf("\n\n |:(|   Meghaltal!!! \nA tovabblepeshez nyomj entert!");
	_getch();
	system("clear");

	system("cls");
	char mente;
	do {
		printf("El szeretned menteni az eredmenyed? (y/n)");
		scanf(" %c", &mente);
	} while (mente != 'y' && mente != 'n');
	if (mente == 'y') {
		printf("\n\nIrd be a neved: ");
		char nev[11];
		int score =*scorebe;  

		//jatekos nevenek bekerese
		scanf("%10s", nev);

		//egy jatekos adatainak a strukturaja 
		typedef struct player {
			int score;
			char name[11];
		}player;

		FILE *fp;

		//file megnyitasa, ha nem letezik akkor csinal egyet olyan nevvel az eszkozon
		fp = fopen("scoreboard.txt", "r");
		if (fp == NULL) {
			fp = fopen("scoreboard.txt", "w");
		}
		else {
			fclose(fp);

			fp = fopen("scoreboard.txt", "a");
		}
		fprintf(fp, "%s %d ", nev, score); //nev es pontszam beirasa a fileba

		fclose(fp);

		fp = fopen("scoreboard.txt", "r");


		int i = 0; //kesobbi ciklus valtozo


		//emberek megszamlalasa a tablaban
		int numofplayers = 0;
		player szam;

		int c;
		char be1[11];
		c = fgetc(fp);
		rewind(fp);
		while (c!= EOF)
		{
			fscanf(fp, "%s %d", be1, &szam.score);
			strcpy(szam.name, be1);
			numofplayers++;
			c = fgetc(fp);
		}

		fclose(fp);
		fp = fopen("scoreboard.txt", "r");

		//emberek lementese a tablabol egy dinamikus tombbe
		player* arr = (player*)malloc(numofplayers * sizeof(player));

		char be[11];
		c = fgetc(fp);
		rewind(fp);
		while (c!= EOF)
		{
			fscanf(fp, "%s %d", be, &arr[i].score);
			strcpy(arr[i].name, be);
			i++;
			c = fgetc(fp);
		}

		//adatok rendezese bubble sort algo-val
		player temp;
		for (int i = 0; i < numofplayers-2; i++)
		{
			for (int j = 0; j < numofplayers-i-2; j++)
			{
				if (arr[j].score < arr[j + 1].score)
				{
					temp = arr[j];
					arr[j] = arr[j + 1];
					arr[j + 1] = temp;
				}
			}
		}

		fclose(fp);
		remove("scoreboard.txt");//regi adatok torlese a file torlesevel egyutt

		//rendezett adatok visszairasa es rekordtabla konzolra irasa
		printf("-----------------------------------\n");
		fp = fopen("scoreboard.txt", "w");
		for (int i = 0; i < numofplayers-1; i++)
		{

			fprintf(fp, "%s %d ", arr[i].name, arr[i].score);

			if (i + 1 < 10) { printf("%d.  name: %s, \t points: %d", i + 1, arr[i].name, arr[i].score); }
			else {
				printf("%d. name: %s, \t points: %d", i + 1, arr[i].name, arr[i].score);
			}
			if (strcmp(arr[i].name, nev) == 0) { printf("  <--\n"); }
			else { printf("\n"); }

		}free(arr); //lementett rekordtabla torlese

		fclose(fp);
	}
	else { return; }

}

//uj jatekkor a kezdeti ertekek beallitasa
void setup(bool *multi, bool *game_end, bool *eaten, bool *eaten2, int *score, int *score2, int *x, int *y, int *x2, int *y2, int *fx, int *fy, hdir *dir, hdir *dir2, bool *dsize, int width, int height, snake *head, snake *head2) {
	*score = 0; //lokalis ponttabla


	*x = width / 2; //fej x koordinataja
	*y = height / 2; //fej y koordinataja
	*fy = rand() % (height - 2); //kaja y
	*fx = rand() % (width - 2); //kaja x koordinataja
	*dir = Stop; //kezdetkor all
	*game_end = false; //jatek vege bool
	*eaten = false; //evett-e ebben a ciklusban kajat
	head->x = *x; //fej kezdeti x koordinatajanak beallitasa
	head->y = *y; //fej kezdeti y koordinatajanak beallitasa
	head->next = NULL;

	if (*multi) {
		
		
		*x2 = width / 4; //masodik jatekos x koordinataja
		*y2 = height / 4; //masodik jatekos y koordinataja
		head2->x = *x2; //fej kezdeti x koordinatajanak beallitasa
		head2->y = *y2; //fej kezdeti y koordinatajanak beallitasa
		head2->next = NULL;
		*eaten2 = false; //masodik jatekos evett-e ebben a ciklusban kajat
		*dir2 = Stop; //masodik jatekos is all kezdetkor
		*score2 = 0;
	}
}

//kurzort attallitja a frissiteni kivant pixelre
void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

//iranyitas a hdir tipusu valtozo ertekenek atallitasaval
void control(hdir *dir) {

	bool pressed = false;
	if (GetAsyncKeyState('A')) {
		*dir = L;
		pressed = true;
	}
	if (GetAsyncKeyState('D')) {
		*dir = R;
		pressed = true;
	}

	if (GetAsyncKeyState('W')) {
		*dir = U;
		pressed = true;
	}

	if (GetAsyncKeyState('S')) {
		*dir = D;
		pressed = true;
	}
	if (!pressed)*dir = *dir;

}

//kettes jatekos iranyitasa
void control2(hdir *dir2) {

	bool pressed = false;
	if (GetAsyncKeyState('J')) {
		*dir2 = L;
		pressed = true;
	}
	if (GetAsyncKeyState('L')) {
		*dir2 = R;
		pressed = true;
	}

	if (GetAsyncKeyState('I')) {
		*dir2 = U;
		pressed = true;
	}

	if (GetAsyncKeyState('K')) {
		*dir2 = D;
		pressed = true;
	}
	if (!pressed)*dir2 = *dir2;
}

//a jatek magja, itt tortennek a fobb valtozo modositasok
void logic(void(*gotoxy)(int*, int*), bool *multi, int *score, hdir *dir, int *x, int *y, int *fx, int *fy, bool *game_end, bool *eaten, int width, int height, snake* head) {
	snake* tmp = head; //kigyo lancszemeinek a pivotja

		if (*x == *fx && *y == *fy) { *eaten = true; *score = *score + 1; }
		else { *eaten = false; }
	

	//kaja kiirasa, firssites ha megettek
	if (*eaten) {

		bool inbody = false;

		do {
			*fy = rand() % (height - 2);
			*fx = rand() % (width - 2);

			if (*score > 4 ||(height*width)<160) { //4 testresz alatt tul keves az eselye hogy testbe spawnoljon
				//tmp = head->next;
				int a = 1;
				tmp = head;
				while (a<*score-1) { // azert -1 mert test elemet meg nem addolt
					if (tmp->x == *fx && tmp->y == *fy) { inbody = true; break; }
					else {
						inbody = false;
					}//lista vegigjarasa es x,y ertekek vizsgalata, hogy ne a testbe spawnoljon a kaja
					tmp = tmp->next;
					a++;
				}
			}

		} while (((*fx == 0 || *fx == width) || (*fy == 0 || *fy == height)) || inbody==true);
		gotoxy(*fx, *fy);
		printf("X");//letorolni azert nem kell mert bekapta
	}
	 
	//ha a fej evett akkor hozzaad egy test elemet a vegehez
	if (*eaten) {

		//memoria foglalas az uj elemnek
		snake* uj = (snake*)malloc(sizeof(snake));
		uj->next = NULL;

		snake *last = head;
		//hozzafuzes az elejetol kezdve
		while (last->next != NULL) {
			last = last->next;
		}last->next = uj;

	}


	snake* cur1 = (snake*)malloc(sizeof(snake));//segedelemek a kovetkezo lepesben megvaltoztatott adatok jelenlegi ertekenek tarolasahoz
	snake* cur2 = (snake*)malloc(sizeof(snake));
	
	//pivot reset
	tmp=head;
	
	//a test elemek koordinatainak leptetese egyet elore 
	cur2->x = tmp->x; //ertekadas a legelso lefutashoz
	cur2->y = tmp->y;

	int par = 0;//elem helyenek paritasa az adatok csusztatasanal kell

	if (*score > 0) {

		int a = 0;//while cv-ja
		while (a < *score) {
			
			if (par % 2 == 0) {//paros elem a 0. headdel kezdve
				if (a < *score) { //score db test eleme van a fejen kivul
				
					cur1->x = tmp->next->x;
					cur1->y = tmp->next->y;
				}//kovetkezo lepesben valtoztatott elem adatainak eltarolasa

				gotoxy(tmp->next->x, tmp->next->y); { printf(" "); }//valtoztatas elott letoroljuk mert nem tudjuk mi lesz utana helyette

				tmp->next->x = cur2->x;
				tmp->next->y = cur2->y;
				gotoxy(tmp->next->x, tmp->next->y); printf("o"); //leptetett farok kiirasa

				tmp = tmp->next;

			}

			//paratlanadik elemek leptetese
			if (par % 2 == 1) {
				if (a < *score) {
				
					cur2->x = tmp->next->x;
					cur2->y = tmp->next->y;
				}
				gotoxy(tmp->next->x, tmp->next->y); { printf(" "); }//valtoztatas elott letoroljuk mert nem tudjuk mi lesz utana helyette

				tmp->next->x = cur1->x;
				tmp->next->y = cur1->y;
				gotoxy(tmp->next->x, tmp->next->y); printf("o");//farok kiiarasa helyben

				tmp = tmp->next;

			}par = par + 1;
			a++;

		}

	}
	free(cur1); //lefoglalt seged elemek torlese
	free(cur2);

	//a controlba beolvasott dir alapjan fej x;y koordinatak valtoztatasa azert itt kell mert elobb leptetjuk az adatokat a fej adataval egyutt es utana allitjuk at a fej koordinatajat
		switch (*dir) {

		case L:
			*x = *x - 1;
			break;

		case R:
			*x = *x + 1;
			break;

		case U:
			*y = *y - 1;
			break;

		case D:
			*y = *y + 1;
			break;

		default:
			break;

		}

		//ha van test elem ezt nem kell mert a fej helyere a test elso eleme kerul
		if (*score == 0) { gotoxy(head->x, head->y); printf(" "); }

		//head koordinatak frissitese
		head->x = *x;
		head->y = *y;

		gotoxy(head->x, head->y); 
		
		if (*multi) {
			printf("M");
		}
		else { printf("O"); }//frissitett fej kiirasa az egyes vagy kettes jatekosra


	//falnak utkozes viszgalata
	if (*x == 0 || *x == width-1 || *y == 0 || *y == height - 1) {
		*game_end = true;
	}

	//farokba utkozes vizsgalata
	if (*score > 1) {
		tmp = head->next;
		while (tmp->next != NULL) {
			if (tmp->next->x == *x && tmp->next->y == *y) { *game_end = true; }//lista vegigjarasa es x,y ertekek vizsgalata
			tmp = tmp->next;
		}
	}
}//end logic

//multinal kigyok utkozeset vizsgalo fv
void crash(bool *game_end, int score, int score2, int *x, int *y, int *x2, int *y2, snake* head, snake* head2) {
	
	//pivot
	snake* tmp; 

	tmp = head;
	int a =0;//ciklus valtozo, hany elemet nezzen meg
	while (a<score+1) {
		if (tmp->x == *x2 && tmp->y == *y2) { *game_end = true; }//lista vegigjarasa es x,y ertekek vizsgalata
		tmp = tmp->next;
		a++;
	}

	//Crash b, (2->1)
	tmp = head2;
	a = 0;
	while (a<score2+1) {
		if (tmp->x == *x && tmp->y == *y) { *game_end = true; }
		tmp = tmp->next;
		a++;
	}
}

//lefoglalt kigyo memoria felszabaditasa fontos, hogy a fejet nem torli az uj jatek miatt ezert csak score>0-nal mukodik
void wipesnake(snake *head) {
	snake *seg1 = head->next;
	snake *seg2 = seg1->next; //if score>0 ez letezik (ha 1 akkor null lesz a seg2)

	do{
		free(seg1);
		seg1 = seg2;
		if (seg2 != NULL) { seg2 = seg2->next; }
	}
	while (seg2 != NULL);
}


//palya kirajzolasa a jatek elejen plusz legelso kaja
void draw(void(*gotoxy)(int *x, int *y), int *fx, int *fy, int height, int width, bool *multi) {

	system("cls");

	//fal
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (j == 0 || j == width-1) { printf("|"); }
			else {
				if (i == 0 || i == height-1) { printf("-"); }
				else printf(" ");
			}
		}printf("\n");
	}

	//legelso kaja generalasa es kiirasa
	do {
		*fy = rand() % (height - 2);
		*fx = rand() % (width - 2);

	} while (((*fx == 0 || *fx == width) || (*fy == 0 || *fy == height)));
	gotoxy(*fx, *fy);
	printf("X");//letorolni ugye azert nem kell mert bekapta;

	gotoxy(width / 2, height / 2); printf("O");
	if(*multi){ gotoxy(width / 4, height / 4); printf("M"); }


}

/////////////////////////////////////////////////////eddig tartott a fuggvenyek definialasa

int main() {

	srand(time(NULL)); //random kaja generalashoz kell

	//alap valtozok deklaralasa a mainben
	bool game_end = false, eaten = false, dsize=true;
	int x = 0, y = 0, fx = 0, fy = 0, score = 0;
	int height = 20, width = 40;
	hdir dir = 0;
	bool multi = false;

	snake *head = (snake*)malloc(sizeof(snake));//kigyo feje
	head->next = NULL;

	//a masodik jatekos valtozoinak deklaralasa

	bool eaten2 = false;
	int x2 = 0, y2 = 0, score2 = 0;
	hdir dir2 = 0;

	snake *head2 = (snake*)malloc(sizeof(snake));//kigyo feje a masodik jatekosra
	head2->next = NULL;

	bool restart = false; // uj jatek inditasahoz bool

	printf("Kigyojatek \n\n");
	do{//legfelso fuggveny - restart

		//palyameret beallitasa alapertelmezettre, hogy ne az elozo beallitas legyen a default
		height = 20; 
		width = 40;

		//uj jatek inditasakor megjeleno menu
		int be = mainmenu(&height, &width, &dsize);

	if (be == 1) {
		//single player mod futtatasa

			setup(&multi, &game_end, &eaten, &eaten2, &score, &score2, &x, &y, &x2, &y2, &fx, &fy, &dir, &dir2, &dsize, width, height, head, head2);
			_getch();
			draw(gotoxy, &fx, &fy, height, width, &multi);
			_getch();

			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));//a kosza inputok takaritasa vegett
			//egy menet alatt futo fuggveny
			while (!game_end) {
				control(&dir);
				logic(gotoxy, &multi, &score, &dir, &x, &y, &fx, &fy, &game_end, &eaten, width, height, head);
				Sleep(100);//futast lassitja, megall a futas (ms)
				gotoxy(0, height); printf("\n pont: %d", score);//pontszam kiirasa

			}
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //az iranyitas soran lenyomott billenytuk torlese a pufferbol
			
			if(score>0)wipesnake(head);//torli a lefoglalt kigyot a fej nelkul!!
			save(&score);
			
	}

	if (be == 2) {
		//multiplayer mod futtatasa

		multi = true; //seged bool a multi fuggvenyek lefutasahoz
		setup(&multi, &game_end, &eaten, &eaten2, &score, &score2, &x, &y, &x2, &y2, &fx, &fy, &dir, &dir2, &dsize, width, height, head, head2);
		_getch();
		draw(gotoxy, &fx, &fy, height, width, &multi);
		_getch();
		multi = false;

		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));//a kosza inputok takaritasa vegett
		dir = 0;
		dir2 = 0;

		while (!game_end) {
			control(&dir);
			control2(&dir2);
			logic(gotoxy, &multi, &score, &dir, &x, &y, &fx, &fy, &game_end, &eaten, width, height, head);//logic elso jatekosra hivasa
			crash(&game_end, score, score2, &x, &y, &x2, &y2, head, head2);
			multi = true;//hogy a logicban lefussanak a multis reszek(fej kiiras; switch(dir))
			logic(gotoxy, &multi, &score2, &dir2, &x2, &y2, &fx, &fy, &game_end, &eaten2, width, height, head2);//masodik jatekosra hivas
			multi = false;//hogy megint az elsore nezze majd a logic
			crash(&game_end, score, score2, &x, &y, &x2, &y2, head, head2);
			gotoxy(0, height); printf("\n pont1: %d", score);
			gotoxy(0, height + 1); printf("\n pont2: %d", score2);
			Sleep(150);
		}
		Sleep(700);//hogy ne villanjon el a kep ha veletlen nyom egy gombot a halal utan
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //az iranyitas soran lenyomott billenytuk torlese a pufferbol

		if(score>0)wipesnake(head);//torli a lefoglalt elso kigyot
		if(score2>0)wipesnake(head2);//torli a lefoglalt masodik kigyot
	}

	printf("\nA jatek vegetert!!\n\nA tovabblepeshez nyomj entert!");
	_getch();

	system("cls");
	
	//ujrainditas 
	char re;
	do {
		printf("Ujra szeretned inditani a jatekot? (y/n)");
		scanf(" %c", &re);
	} while (re != 'y' && re != 'n');
	if (re == 'y') { restart = true; }
	else { restart = false; }

	} while (restart == true);
	
	return 0;
}
