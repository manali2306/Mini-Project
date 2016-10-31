/***************************************************************************
* Copyright (C) Manali Jadhav jmanali27@gmail.com
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of he GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1of the License, or 
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program; if not, write to Free Software Foundation,
* Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
***************************************************************************/








#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <menu.h>
#include <form.h>
#include <ctype.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))


int add_itm1(FORM *form, FIELD **field);
int new_item1(WINDOW *w); 
int del_item(WINDOW *w);
int del_from_file(FORM *form, FIELD **field);
int make1(WINDOW *w, int m);
int bill(WINDOW *w);
double fetch_price(FORM *form, FIELD **field);
void print_in_middle(WINDOW *w, int starty, int startx, int width, char *string, chtype color);
char *string(char *ch);
int rows, cols;

typedef struct item
{
	char item_code[10];
	char item_name[20];
	float item_rate;
	
}item;


int main()
{
	int ret1 = 0;
	ITEM **i;
	MENU *m;
	WINDOW *w;
	int ch;
	char *choices[] = { "1. ADD ITEM", "2. DELETE ITEM", "3. BILL", (char *)NULL };

	int n, j, c;
	initscr();
	start_color();
	getmaxyx(stdscr, rows, cols);
	cbreak();
	noecho();
	init_pair(1, COLOR_MAGENTA, COLOR_WHITE);
	init_pair(2, COLOR_CYAN, COLOR_WHITE);
	init_pair(3, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	w = newwin(100,100,0,0);
	keypad(w, TRUE);
	
	n = ARRAY_SIZE(choices);
        i = (ITEM **)calloc(n + 1, sizeof(ITEM *));
        for(j = 0; j < n; ++j)
                i[j] = new_item(choices[j], choices[j]);
        i[j] = (ITEM *)NULL;
        m = new_menu((ITEM **)i);
        wattron(w, COLOR_PAIR(2));
	mvwprintw(w, 1, 15, "****************WELCOME****************");
	wattroff(w, COLOR_PAIR(2));
	wattron(w, COLOR_PAIR(3));
	mvwprintw(w, 3, 15, "-----------SELECT ANY OPTION-----------");
	wattroff(w, COLOR_PAIR(3));
	wattron(w, COLOR_PAIR(1));
	mvwprintw(w, 5, 31, "MY MENU");
	wattroff(w, COLOR_PAIR(1));
	set_menu_win(m, w);
        set_menu_sub(m, derwin(w, 6, 18, 7, 25));
        set_menu_mark(m, " * ");
	post_menu(m);
	refresh();
	wrefresh(w);
	while((c = wgetch(w)) != KEY_HOME)
	{       switch(c)
	        {	
	        	case KEY_DOWN: 
				menu_driver(m, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(m, REQ_UP_ITEM);
				break;
			case 10:
				if(strcmp(item_name(current_item(m)),"1. ADD ITEM") == 0 ) 
		
				{
					ret1 = make1(w, 1);
					if(ret1 == 1){
						unpost_menu(m);
        					free_menu(m);
        					for(j = 0; j < n; ++j)
        	        				free_item(i[j]);
						
					}
					main(); 			
				}
				if(strcmp(item_name(current_item(m)),"2. DELETE ITEM") == 0 ) 
				{
					ret1 = make1(w, 0);
					if(ret1 == 1){
						unpost_menu(m);
        					free_menu(m);
        					for(j = 0; j < n; ++j)
        	        				free_item(i[j]);
						
					}
					main(); 		
				}
				if(strcmp(item_name(current_item(m)),"3. BILL") == 0 ) 
				{
					ret1 = make1(w, 2);
					if(ret1 == 1){
						unpost_menu(m);
        					free_menu(m);
        					for(j = 0; j < n; ++j)
        	        				free_item(i[j]);
						
					}
					main(); 	
				}
				break;
				
		}
                wrefresh(w);
	}	
	getch();
	endwin();
		
}





int make1(WINDOW *w, int m) {
	int k = 0;
	w = newwin(100, 100, 0, 0);
	box(w, 0 , 0);	
	wbkgd(w, COLOR_PAIR(4));	
	wrefresh(w);
	if(m == 0)
		k = del_item(w);
	else if(m == 1)		
		k = new_item1(w);
	else if(m == 2)
		k = bill(w);
	wclear(w);
	wbkgd(w,COLOR_PAIR(4));
	wrefresh(w);
	delwin(w);
	return k;
}






void print_in_middle(WINDOW *w, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
	float temp;

	if(w == NULL)
		w = stdscr;
	getyx(w, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(w, color);
	mvwprintw(w, y, x, "%s", string);
	wattroff(w, color);
	refresh();
}







int add_itm1(FORM *form, FIELD **field)
{
	FILE *fp;
	item itm;
	int i = 0, j = 0, k = 0;
	int len = field_count(form);
	char **pt;
	char *pt1;
	pt = (char **)malloc(sizeof(char *) * len);
	fp = fopen("additem.txt", "a+");
	while(i < len)
	{
		pt[i] = field_buffer(field[i], 0);
		i++;
	}
	pt1 = string(pt[0]);
	strcpy(itm.item_code, pt1);
	pt1 = string(pt[1]);
	strcpy(itm.item_name, pt1);
	itm.item_rate = atof(pt[2]);
	fprintf(fp, "%s \t %s \t %f \n", itm.item_code, itm.item_name, itm.item_rate);
	free(pt);
	free(pt1);
	fclose(fp);
	return 1;
}






int new_item1(WINDOW *w)
{
	int ret = 0;
	FIELD **field;
	FORM *form;
	char *s;
	int j = 0;
	s = (char *)malloc(sizeof(char) * 31);
	while(j< 30){
		s[j] = ' ';
		j++;
	}
	s[j] = '\0';
	int ch = 0, size, i = 0, count = 0, x, y;
	char *field_names[] = {"ITEM CODE","ITEM NAME", "PRICE"};
	size = sizeof(field_names) / sizeof(field_names[0]);
	init_pair(5, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(6, COLOR_BLUE, COLOR_WHITE);
	cbreak();
	keypad(w, TRUE);
	field = (FIELD**)malloc((size + 2) * sizeof(FIELD*));
	for(i = 0; i < 3; i++) {			
		field[i] = new_field(1, 30, 5 + 2*i, 50, 0, 0);
	}
	
	i = 0;
	while(i < size) {
		field_opts_off(field[i], O_AUTOSKIP );
		set_field_back(field[i], COLOR_PAIR(5));
		i++;
	} 
	field[3] = new_field(1, 10, 17, 30, 0, 0);
	field_opts_off(field[3], O_AUTOSKIP | O_EDIT);
	set_field_back(field[3], COLOR_PAIR(5));
	set_field_fore(field[3], COLOR_PAIR(5));
	set_field_buffer(field[3], 0, " ADD ITEM ");

	field[4] = new_field(1, 6, 17, 50, 0, 0);
	field_opts_off(field[4], O_AUTOSKIP | O_EDIT);
	set_field_back(field[4], COLOR_PAIR(5));
	set_field_fore(field[4], COLOR_PAIR(5));
	set_field_buffer(field[4], 0, " EXIT ");

	field[5] = NULL;
	form = new_form(field);	
	scale_form(form, &y, &x);

	set_form_win(form, w);
	set_form_sub(form, derwin(w, y, x, 2, 40));
	post_form(form);
	
	for(i = 0; i < 3; i++)
		mvwprintw(w,5 + 2 * i, 20, "%s :",field_names[i]);
	mvwprintw(w, 1, 35, "ADD NEW ITEM", A_BOLD | COLOR_PAIR(6) | A_REVERSE);
	form_driver(form, REQ_FIRST_FIELD);	
	wrefresh(w);
	while(i = wgetch(w)) {
		if(isprint(i) || (i == ' ') || (i == '\t') )
			form_driver(form, i);
		else {
			switch(i) {
					case KEY_DOWN:
						form_driver(form, REQ_NEXT_FIELD);
						form_driver(form, REQ_END_LINE);
						break;
					case KEY_UP:
						form_driver(form, REQ_PREV_FIELD);
						form_driver(form, REQ_END_LINE);
						break;
					case KEY_LEFT:
						form_driver(form, REQ_PREV_CHAR);
						break;
					case KEY_RIGHT:
						form_driver(form, REQ_NEXT_CHAR);
						break;
					case 10:
							if(field[3] == current_field(form)) {
								count = 0;
								i = 0;
								while(i < 3) {
									if(strcmp(field_buffer(field[i], 0), s) == 0 ) {
										print_in_middle(w, 23, 0, 120,"     FIELD CAN'T BE EMPTY!    ", COLOR_PAIR(3));
										count = 1;
										break;
									}
									i++;
								}
								if(count == 0) {
									ret = add_itm1(form, field);
									if(ret == 1)
										print_in_middle(w, 23, 0, 120,"ITEM ADDED SUCCESFULLY", COLOR_PAIR(3));
									else if(ret == 0)
										print_in_middle(w, 23, 0, 120,"ITEM ALREADY ADDED", COLOR_PAIR(3));
									i = 0;
									while( i < size ) 
										set_field_buffer(field[i++], 0, "");
								}
									form_driver(form, REQ_FIRST_FIELD);
							}
							else if(field[4] == current_field(form)) {
								main();
							}
							else {
								form_driver(form, REQ_NEXT_FIELD);
								form_driver(form, REQ_END_LINE);
							}
						break;									
						case KEY_BACKSPACE: case 127:
						form_driver(form, REQ_DEL_PREV);
						break;
				  				
			}
		}
	}
	end:
	unpost_form(form);
	free_form(form);
	wclear(w);
	i = 0;
	while(i < size) {
		wmove(w, 3 + 2 * i, 1);
		wclrtoeol(w);
		free_field(field[i]);
		i++;
	}
	free(field);
	box(w, 0, 0);
	wrefresh(w);
	return ret;
}





int del_item(WINDOW *w)
{
	int ret = 0;
	FIELD **field;
	FORM *form;
	char *s;
	int j = 0;
	s = (char *)malloc(sizeof(char) * 31);
	while(j < 30){
		s[j] = ' ';
		j++;
	}
	s[j] = '\0';
	int ch = 0, size, i = 0, count = 0, x, y;
	char *field_names[] = {"ITEM CODE","ITEM NAME"};
	size = sizeof(field_names) / sizeof(field_names[0]);
	init_pair(7, COLOR_WHITE, COLOR_BLUE);
	cbreak();
	keypad(w, TRUE);
	field = (FIELD**)malloc((size + 2) * sizeof(FIELD*));
	for(i = 0; i < 2; i++) {			
		field[i] = new_field(1, 30, 9 + 2*i, 50, 0, 0);
	}
	i = 0;
	while(i < size) {
		field_opts_off(field[i], O_AUTOSKIP );
		set_field_back(field[i], COLOR_PAIR(7));
		i++;
	} 
	field[2] = new_field(1, 15, 17, 50, 0, 0);
	field_opts_off(field[2], O_AUTOSKIP | O_EDIT);
	set_field_back(field[2], COLOR_PAIR(7));
	set_field_fore(field[2], COLOR_PAIR(7));
	set_field_buffer(field[2], 0, " DELETE ITEM ");

	field[3] = new_field(1, 8, 17, 72, 0, 0);
	field_opts_off(field[3], O_AUTOSKIP | O_EDIT);
	set_field_back(field[3], COLOR_PAIR(7));
	set_field_fore(field[3], COLOR_PAIR(7));
	set_field_buffer(field[3], 0, " EXIT ");

	field[4] = NULL;
	form = new_form(field);	
	scale_form(form, &y, &x);

	set_form_win(form, w);
	set_form_sub(form, derwin(w, y, x, 1, 60));
	post_form(form);
	for(i = 0; i < 2; i++)
		mvwprintw(w, 9 + 2*i, 35, "%s: ", field_names[i]);
	print_in_middle(w, 28, 0, 120,"USE UP AND DOWN KEYS TO MOVE BETWEEN FIELDS", COLOR_PAIR(7));
	
	form_driver(form, REQ_FIRST_FIELD);	
	wrefresh(w);
	while(i = wgetch(w)) {
		if(isprint(i) || (i == ' ') || (i == '\t') )
			form_driver(form, i);
		else {
			switch(i) {
					case KEY_DOWN:
						form_driver(form, REQ_NEXT_FIELD);
						form_driver(form, REQ_END_LINE);
						break;
					case KEY_UP:
						form_driver(form, REQ_PREV_FIELD);
						form_driver(form, REQ_END_LINE);
						break;
					case KEY_LEFT:
						form_driver(form, REQ_PREV_CHAR);
						break;
					case KEY_RIGHT:
						form_driver(form, REQ_NEXT_CHAR);
						break;
						case 10:
							if(field[2] == current_field(form)) {
								i = 0;
								while(i < size) {
									if(strcmp(field_buffer(field[i], 0), s) == 0 ) {
										print_in_middle(w, 23, 0, 120,"     FIELD CAN'T BE EMPTY!    ", COLOR_PAIR(3));
										count++;
										break;
									}
									i++;
								}
								if(count == 0) {
									ret = del_from_file(form, field);
									if(ret == 1)
										print_in_middle(w, 23, 0, 120,"ITEM DELETED SUCCESFULLY", COLOR_PAIR(3));
									else if(ret == 0)
										print_in_middle(w, 23, 0, 120,"ITEM NOT FOUND", COLOR_PAIR(3));
									i = 0;
									while( i < size ) 
										set_field_buffer(field[i++], 0, "");
								}
									form_driver(form, REQ_FIRST_FIELD);
							}
							else if(field[3] == current_field(form)) {
								main();
							}
							else {
								form_driver(form, REQ_NEXT_FIELD);
								form_driver(form, REQ_END_LINE);
							}
						break;									
						case KEY_BACKSPACE: case 127:
						form_driver(form, REQ_DEL_PREV);
						break;
				  				
			}
		}
	}
	end:
	unpost_form(form);
	free_form(form);
	i = 0;
	while(i < size) {
		wmove(w, 3 + 2 * i, 1);
		wclrtoeol(w);
		free_field(field[i]);
		i++;
	}
	free(field);
	box(w, 0, 0);
	wrefresh(w);
	return ret;
}





char *string(char *ch)
{
	int i = 0;
	char *s;
	s = (char *)malloc(sizeof(char) * 30);
	while(ch[i] != 32)
	{
		s[i] = ch[i];
		i++;
	}
	s[i] = '\0';
	return s;
}





int del_from_file(FORM *form, FIELD **field)
{
	FILE *fp, *fp1;
	int y = 0;
	int j = 0;
	int l = field_count(form), l2;
	item it1, it2;
	fp = fopen("additem.txt", "r");
	fp1 = fopen("temp.txt", "w+");
	char **itm;
	char *itm1;	
	itm = (char **)malloc(sizeof(char *) * l);
	while(y < l) {
		itm[y] = field_buffer(field[y], 0);
		y++;
	}
	itm1 = string(itm[0]);
	strcpy(it1.item_code, itm1);
	l2 = strlen(it1.item_code);
	while(fscanf(fp, "%s \t %s \t %f \n", it2.item_code, it2.item_name, &it2.item_rate) != EOF) {
		
		if((strncmp(it1.item_code, it2.item_code, l2) != 0))
			fprintf(fp1, "%s \t %s \t %f \n", it2.item_code, it2.item_name, it2.item_rate);
		else if(strncmp(it1.item_code, it2.item_code, l2) == 0)
			j++;
		else if(feof(fp))
			break;
	}
	remove("additem.txt");
	rename("temp.txt", "additem.txt");
	free(itm);
	free(itm1);
	fclose(fp);
	fclose(fp1);
	if (j == 0)
		return 0;
	return 1;
}





int bill(WINDOW *w)
{
	int ret = 0;
	FIELD **field;
	FORM *form;
	char *s;
	int j = 0;
	s = (char *)malloc(sizeof(char) * 31);
	while(j < 30){
		s[j] = ' ';
		j++;
	}
	s[j] = '\0';
	int ch = 0, size, i = 0, count = 0, x, y;
	char *field_names[] = {"ITEM CODE","ITEM NAME"};
	size = sizeof(field_names) / sizeof(field_names[0]);
	init_pair(8, COLOR_WHITE, COLOR_BLUE);
	cbreak();
	keypad(w, TRUE);
	field = (FIELD**)malloc((size + 2) * sizeof(FIELD*));
	for(i = 0; i < 2; i++) {			
		field[i] = new_field(1, 30, 9 + 2*i, 50, 0, 0);
	}
	i = 0;
	while(i < size) {
		field_opts_off(field[i], O_AUTOSKIP );
		set_field_back(field[i], COLOR_PAIR(8));
		i++;
	} 
	field[2] = new_field(1, 15, 17, 50, 0, 0);
	field_opts_off(field[2], O_AUTOSKIP | O_EDIT);
	set_field_back(field[2], COLOR_PAIR(8));
	set_field_fore(field[2], COLOR_PAIR(8));
	set_field_buffer(field[2], 0, " ENTER ");
	
	field[3] = new_field(1, 15, 17, 65, 0, 0);
	field_opts_off(field[3], O_AUTOSKIP | O_EDIT);
	set_field_back(field[3], COLOR_PAIR(8));
	set_field_fore(field[3], COLOR_PAIR(8));
	set_field_buffer(field[3], 0, " FIND BILL ");


	field[4] = new_field(1, 8, 17, 80, 0, 0);
	field_opts_off(field[4], O_AUTOSKIP | O_EDIT);
	set_field_back(field[4], COLOR_PAIR(8));
	set_field_fore(field[4], COLOR_PAIR(8));
	set_field_buffer(field[4], 0, " EXIT ");

	field[5] = NULL;
	form = new_form(field);	
	scale_form(form, &y, &x);

	set_form_win(form, w);
	set_form_sub(form, derwin(w, y, x, 1, 60));
	post_form(form);
	for(i = 0; i < 2; i++)
		mvwprintw(w, 9 + 2*i, 35, "%s: ", field_names[i]);
	print_in_middle(w, 28, 0, 120,"USE UP AND DOWN KEYS TO MOVE BETWEEN FIELDS", COLOR_PAIR(8));
	
	form_driver(form, REQ_FIRST_FIELD);	
	wrefresh(w);
	while(i = wgetch(w)) {
		if(isprint(i) || (i == ' ') || (i == '\t') )
			form_driver(form, i);
		else {
			switch(i) {
					case KEY_DOWN:
						form_driver(form, REQ_NEXT_FIELD);
						form_driver(form, REQ_END_LINE);
						break;
					case KEY_UP:
						form_driver(form, REQ_PREV_FIELD);
						form_driver(form, REQ_END_LINE);
						break;
					case KEY_LEFT:
						form_driver(form, REQ_PREV_CHAR);
						break;
					case KEY_RIGHT:
						form_driver(form, REQ_NEXT_CHAR);
						break;
						case 10:
							if(field[2] == current_field(form)) {
								i = 0;
								while(i < size) {
									if(strcmp(field_buffer(field[i], 0), s) == 0 ) {
										print_in_middle(w, 23, 0, 120,"     FIELD CAN'T BE EMPTY!    ", COLOR_PAIR(3));
										count++;
										break;
									}
									i++;
								}
								if(count == 0) {
									ret = fetch_price(form, field);
									if(ret == 1)
										print_in_middle(w, 23, 0, 120, "ERROR", COLOR_PAIR(8));
									else
										mvwprintw(w, 25, 55, "%lf", ret);
									i = 0;
									while( i < size ) 
										set_field_buffer(field[i++], 0, "");
								}
									form_driver(form, REQ_FIRST_FIELD);
							}
							else if(field[4] == current_field(form)) {
								main();
							}
							else {
								form_driver(form, REQ_NEXT_FIELD);
								form_driver(form, REQ_END_LINE);
							}
						break;									
						case KEY_BACKSPACE: case 127:
						form_driver(form, REQ_DEL_PREV);
						break;
				  				
			}
		}
	}
	end:
	unpost_form(form);
	free_form(form);
	i = 0;
	while(i < size) {
		wmove(w, 3 + 2 * i, 1);
		wclrtoeol(w);
		free_field(field[i]);
		i++;
	}
	free(field);
	box(w, 0, 0);
	wrefresh(w);
	return ret;
	
}



double fetch_price(FORM *form, FIELD **field)
{
	int i = 0;
	int j = 0;
	double rate = 0;
	int l = field_count(form), l2;
	item st1, st2, st3;
	FILE *fp, *fp1;
	fp = fopen("additem.txt", "r");
	fp1 = fopen("bill.txt", "w");
	char **stf;
	char *stf1;	
	stf = (char **)malloc(sizeof(char *) * l);
	while(i < l) {
		stf[i] = field_buffer(field[i], 0);
		i++;
	}
	stf1 = string(stf[1]);
	strcpy(st1.item_name, stf1);
	while(fscanf(fp, "%s \t %s \t %f \n", st2.item_code, st2.item_name, &st2.item_rate) != EOF) 
	{	
		if((strcmp(st1.item_name, st2.item_name) == 0))
			fprintf(fp1, "%s \t %s \t %f \n", st2.item_code, st2.item_name, st2.item_rate);
		else if(strncmp(st1.item_code, st2.item_name, l2) != 0)
			j++;
		else if(feof(fp))
			break;
	}
	stf1 = string(stf[0]);
	strcpy(st1.item_code, stf1);
	while(fscanf(fp1, "%s \t %s \t %f \n", st3.item_code, st3.item_name, &st3.item_rate) != EOF)
	{
		rate = rate + st3.item_rate;
	}
	free(stf);
	fclose(fp);
	fclose(fp1);
	if (j == 0)
		return rate;
	else
		return 1;	
}
