/***Date : 18.09.2016
	Project Name : CMS 1.0
	Author : Avik Sarkar
	Website : www.codexp.xyz
	Github : www.github.com/avistein/cprojects 
***/

#include<ncurses.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>

/***contacts.txt data***/
struct contact_name{
	char name[30],phone[30];
};
typedef struct contact_name CN;

/***specific contact data***/
struct contact_data{
	char org[50],email[100],add[200];
};
typedef struct contact_data CD;

/***node data***/
struct node{
	CN info;
	struct node *next;
};
typedef struct node ND;

int H,W; //height and width of the screen

/***function prototypes***/
void add_contact(ND**);
void add_to_file(ND*,char[]);
void ctlower(char[]);
void delete(ND**,char*,char[]);
void edit(char[],WINDOW*);
void int_menu(WINDOW*,int,char *choices[],int n_choices);
void print_menu(WINDOW*,int,char *choices[],int n_choices,int);
void read_data(ND**,char[]);
void remove_newLine(char[]);
void sort(ND*);
void vsf(ND**,ND**,int);

/***adding contacts to linked list***/
void add_contact(ND **ptr){
	WINDOW *menu_win;
	int height=10,width=40,starth,startw;
	starth=(H-height)/2;
	startw=(W-width)/2;
	menu_win=newwin(height,width,starth,startw);
	box(menu_win,0,0);
	FILE *fp;
	echo();
	curs_set(2);
  	ND *data;
	CD details;
	char fno[20],ch;
	int x=2,y=1;
	data=(ND*)malloc(sizeof(ND));
	move(starth+height+2,0);
	clrtoeol();
	mvwprintw(menu_win,y,x,"Enter Name: ");
	y++;
	wrefresh(menu_win);
	wgetnstr(menu_win,data->info.name,sizeof(data->info.name));
	mvwprintw(menu_win,y,x,"Enter Phone: ");
	y++;
	wrefresh(menu_win);
	wgetnstr(menu_win,data->info.phone,sizeof(data->info.phone));
	mvwprintw(menu_win,y,x,"Enter Organisation: ");
	y++;
	wrefresh(menu_win);
	wgetnstr(menu_win,details.org,sizeof(details.org));	
	mvwprintw(menu_win,y,x,"Enter Email: ");
	y++;
	wrefresh(menu_win);
	wgetnstr(menu_win,details.email,sizeof(details.email));
	mvwprintw(menu_win,y,x,"Enter Address: ");
	y+=3;
	wrefresh(menu_win);
	wgetnstr(menu_win,details.add,sizeof(details.add));
	strcpy(fno,data->info.phone);
	strcat(fno,".txt");
	fp=fopen(fno,"w");
	fputs(details.org,fp);
	fputs("\n",fp);
	fputs(details.email,fp);
	fputs("\n",fp);
	fputs(details.add,fp);
	fputs("\n",fp);
	fclose(fp);
	data->next=NULL;
	if(*ptr==NULL)
		*ptr=data;
	else{
		data->next=*ptr;
		*ptr=data;
	}
	curs_set(0);
	mvwprintw(menu_win,y,(width-strlen("Added Successfully"))/2,"Added Successfully");
	wrefresh(menu_win);
	sleep(1);
	wclear(menu_win);
	wrefresh(menu_win);
	noecho();
}

/***adding contacts to file***/
void add_to_file(ND *temp,char file_name[30]){
	FILE *fp;
	fp=fopen(file_name,"w");
	if(fp==NULL){
		printw("\nDatabase is not present or corrupted");
		refresh();
		return ;
	}
	while(temp){
		fputs(temp->info.name,fp);
		fputs("\n",fp);
		fputs(temp->info.phone,fp);
		fputs("\n",fp);
		temp=temp->next;
	}
	fclose(fp);	
}

/***convert a string to lower case***/
void ctlower(char name[30]){
	int i=0;
	while(name[i]!='\0'){
		name[i]=tolower(name[i]);
		i++;
	}
}

/***delete contact***/
void delete(ND **delete,char *delete_name,char file_name[30]){
	ND *temp,*temp1,*temp2;
	int found=0;
	temp=temp1=*delete;
		while(temp){
			if(strcmp(delete_name,temp->info.phone)==0){
				found=1;
				break;
			}
			temp=temp->next;
		}
		if(found==1){
			if(temp==*delete)
				*delete=temp->next;
			else{
				while(temp1->next&&temp1->next!=temp)
					temp1=temp1->next;
				temp1->next=temp->next;
			}
			temp2=*delete;
			add_to_file(temp2,file_name);
			free(temp);
		}
}

/***editing contacts***/
void edit(char file_name[30],WINDOW *menu_win){
	FILE *fp;
	wclear(menu_win);
	move(H-(H-10)/2,0);
	clrtoeol();
	move((H-10)/2+10+2,0);
	clrtoeol();
	refresh();
	box(menu_win,0,0);
	char add[50],email[50],org[50],fname[50];
	int x=2,y=2,choice;
	echo();
	curs_set(2);
	mvwprintw(menu_win,y,x,"Enter new Organisation: ");
	y++;
	wrefresh(menu_win);
	wgetnstr(menu_win,org,sizeof(org));	
	mvwprintw(menu_win,y,x,"Enter new Email: ");
	y++;
	wrefresh(menu_win);
	wgetnstr(menu_win,email,sizeof(email));
	mvwprintw(menu_win,y,x,"Enter new Address: ");
	y+=2;
	wrefresh(menu_win);
	wgetnstr(menu_win,add,sizeof(add));
	noecho();
	mvwprintw(menu_win,y,x,"Enter enter to confirm");
	choice=wgetch(menu_win);
	curs_set(0);
	if(choice==10){
		strcpy(fname,file_name);
		strcat(fname,".txt");
		remove(fname);
		fp=fopen(fname,"w");
		fputs(org,fp);
		fputs("\n",fp);
		fputs(email,fp);
		fputs("\n",fp);
		fputs(add,fp);
		fputs("\n",fp);
		fclose(fp);
	}
	wclear(menu_win);
}
	

/***print sub-menu***/
void int_menu(WINDOW *menu_win,int highlight,char *choices[],int n_choices){
	wmove(menu_win,8,2);
	wclrtoeol(menu_win);
	box(menu_win,0,0);
	int x, y, i;	
	x = 2;
	y = 8;
	for(i = 0; i < n_choices; ++i){	
		if(highlight == i + 1) {	// Highlight the present choice 
			wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		x+=10;
	}
	wrefresh(menu_win);
}

/***print main menu***/
void print_menu(WINDOW *menu_win,int highlight,char *choices[],int n_choices,int menu_no){
	wclear(menu_win);
	int x, y, i,start,loop_end,end,count,count_max,starth;	
	x = 2;
	y = 2;
	starth=(H-10)/2;
	box(menu_win, 0, 0);
	//display 5 contacts per page
	count=(highlight-1)/5+1;          
	count_max=n_choices/5;           
	if(n_choices%5||n_choices<5)
		count_max++;
	if(highlight%5!=0)
		start=highlight-highlight%5;
	else
		start=highlight-5;
	loop_end=n_choices-(start+5);
	if(loop_end>0)
		end=start+5;
	else
		end=n_choices;
	
	for(i = start; i < end; ++i){	
		if(highlight == i + 1) {	//Highlight the present choice 
			wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	if(menu_no==2){
		y=(H-starth);
		mvprintw(y,(W-strlen("List 1 of 1"))/2,"List %d of %d",count,count_max);
	}
	refresh();
	wrefresh(menu_win);
}		

/***read data from files***/
void read_data(ND **start,char file_name[30]){
	ND *data,*current;
	FILE *fp;
	fp=fopen(file_name,"a+");
	char names[30],phones[30];
	if(fp==NULL)
	{
		clear();
		mvprintw(1,0,"Error 404:Database is not present or corrupted");
		mvprintw(2,0,"Enter any key to exit");
		refresh();
		getch();
		endwin();
		exit(1);
	}
	while(fgets(names,sizeof(names),fp)){
		fgets(phones,sizeof(phones),fp);
		data=malloc(sizeof(ND));
		strcpy(data->info.name,names);
		strcpy(data->info.phone,phones);
		remove_newLine(data->info.name);
		remove_newLine(data->info.phone);
		if(!*start)
			*start=data;
		else
			current->next=data;
		current=data;
	}
	if(*start)
		current->next=NULL;
}

/***removing newline from input obtained via fgets***/
void remove_newLine(char last[]){
	last[strlen(last)-1]='\0';
}
			
/***sorting alphabetically***/
void sort(ND *sorting){
	ND *temp1,*temp2,*min;
	char temp[30],temp_phn[30];
	temp1=sorting;
	while(temp1->next){
		min=temp1;
		temp2=temp1->next;
		while(temp2){
			if(strcmp(min->info.name,temp2->info.name)>0)
				min=temp2;
			temp2=temp2->next;
		}
		strcpy(temp,temp1->info.name);
		strcpy(temp_phn,temp1->info.phone);
		strcpy(temp1->info.name,min->info.name);
		strcpy(temp1->info.phone,min->info.phone);
		strcpy(min->info.name,temp);
		strcpy(min->info.phone,temp_phn);
		temp1=temp1->next;
	}
}

/***view,search & favourite***/
void vsf(ND **ptr1,ND **ptr2,int select){  //select ->1:Search 2:View All 3:Favourites
	char *choices_int[] = { 
			"Delete",
			"Back",
			"Edit",
			"Star"
	};
	if(select==3)
		choices_int[3]="Unstar";
	int n_choices_int=sizeof(choices_int)/sizeof(char*);
	WINDOW *menu_win;
	FILE *fp;
	int height=10,width=40,starth,startw;
	starth=(H-height)/2;
	startw=(W-width)/2;
	menu_win=newwin(height,width,starth,startw);
	ND *temp1,*temp2,*data;
	temp1=*ptr1;
	keypad(menu_win,TRUE);
	char **aPtr,**aPtr1,filename[30],i=1,j=2,k=2,read[30],fav[30],cmp1[30],cmp2[30];
	int check=0,len=0,highlight=1,c,c_int,highlight_int=1,m=1,l,r=1;
	aPtr=malloc(sizeof(char*)*len);
	aPtr1=malloc(sizeof(char*)*len);
	box(menu_win,0,0);
	if(select==1){
		mvwprintw(menu_win,j,k,"Enter name to search: ");
		wrefresh(menu_win);
		echo();
		curs_set(2);
		wgetnstr(menu_win,cmp2,sizeof(cmp2));
		noecho();
		curs_set(0);
		ctlower(cmp2);
		while(temp1){
			memset(cmp1,'\0',sizeof(cmp1));
			strcpy(cmp1,temp1->info.name);
			ctlower(cmp1);
			if(strstr(cmp1,cmp2)){
				len++;
				aPtr=(char**)realloc(aPtr,len*sizeof(char*));
				aPtr1=(char**)realloc(aPtr1,len*sizeof(char*));
				aPtr[len-1]=(char*)malloc(30*sizeof(char));
				aPtr1[len-1]=(char*)malloc(30*sizeof(char));
				strcpy(aPtr[len-1],temp1->info.name);
				strcpy(aPtr1[len-1],temp1->info.phone);
				check++;
			}
			temp1=temp1->next;
		}
	}
	else {
		while(temp1){
			len++;
			aPtr=(char**)realloc(aPtr,len*sizeof(char*));
			aPtr1=(char**)realloc(aPtr1,len*sizeof(char*));
			aPtr[len-1]=(char*)malloc(30*sizeof(char));
			aPtr1[len-1]=(char*)malloc(30*sizeof(char));
			strcpy(aPtr[len-1],temp1->info.name);
			strcpy(aPtr1[len-1],temp1->info.phone);
			check++;
			temp1=temp1->next;
		}
	}
	if(check==0){	
		wclear(menu_win);
		box(menu_win,0,0);
		mvwprintw(menu_win,height/2,(width-strlen("No contact found"))/2,"No contact found");
		mvprintw(starth+height+2,(W-sizeof("Press <- to back"))/2,"Press <- to back");
		refresh();
		wrefresh(menu_win);
		while(r){
			if(getch()==127){
				move(starth+height+2,0);
				clrtoeol();
				wclear(menu_win);
				wrefresh(menu_win);
				return;
			}	
		}
	}
	wclear(menu_win);
	wrefresh(menu_win);
	print_menu(menu_win,highlight,aPtr,len,2);
	while(i){
		if(select!=3)	
			temp2=*ptr2;
		mvprintw(starth+height+2,(W-sizeof("Press <- to back"))/2,"Press <- to back");
		refresh();
		c = wgetch(menu_win);
		switch(c){	
			case KEY_UP:
				if(highlight == 1)
					highlight = len;
				else
					--highlight;
			break;
			case KEY_DOWN:
				if(highlight == len)
					highlight = 1;
				else 
					++highlight;
			break;
			case 10:
				move(starth+height+2,0);
				clrtoeol();
				move(starth+height+1,0);
				clrtoeol();
				refresh();
				m=1;
				memset(filename,'\0',sizeof(filename));
				strcpy(filename,aPtr1[highlight-1]);
				strcat(filename,".txt");
				fp=fopen(filename,"r");
				if(fp==NULL){
					clear();
					mvprintw(1,0,"Can't open %s file",filename);
					mvprintw(2,0,"Enter any key to exit");
					refresh();
					getch();
					endwin();
					free(aPtr);
					free(aPtr1);
					exit(1);
				}
				wclear(menu_win);
				wrefresh(menu_win);
				box(menu_win,0,0);
				j=2;
				k=2;
				mvwprintw(menu_win,j,k,"Name: %s",aPtr[highlight-1]);
				j++;
				mvwprintw(menu_win,j,k,"Phone: %s",aPtr1[highlight-1]);
				j++;
				fgets(read,sizeof(read),fp);
				remove_newLine(read);
				mvwprintw(menu_win,j,k,"Organization: %s",read);
				j++;
				fgets(read,sizeof(read),fp);
				remove_newLine(read);
				mvwprintw(menu_win,j,k,"Email: %s",read);
				j++;
				fgets(read,sizeof(read),fp);
				fclose(fp);
				remove_newLine(read);
				mvwprintw(menu_win,j,k,"Address: %s",read);
				if(select!=3){
					while(temp2){
						if(strcmp(temp2->info.phone,aPtr1[highlight-1])==0){
							choices_int[3]="Unstar";
							break;		
						}
						else
							choices_int[3]="Star";
						temp2=temp2->next;
					}
				}
				int_menu(menu_win,highlight_int,choices_int,n_choices_int);
				while(m){
					c_int=wgetch(menu_win);
					switch(c_int){
						case KEY_LEFT:
							if(highlight_int == 1)
								highlight_int = n_choices_int;
							else
								--highlight_int;
						break;
						case KEY_RIGHT:
							if(highlight_int == n_choices_int)
								highlight_int = 1;
							else 
								++highlight_int;
						break;
						case 10:
							if(select==3)
								m=0;
							if(highlight_int==1){
								if(highlight_int==1&&len){
									delete(ptr1,aPtr1[highlight-1],"contacts.txt");
									delete(ptr2,aPtr1[highlight-1],"favourites.txt");
									choices_int[3]="Star";
									remove(filename);
									if(len!=highlight){
										len--;
										for(l=highlight-1;l<len;l++){
											aPtr[l]=aPtr[l+1];
											aPtr1[l]=aPtr1[l+1];
										}
									}
									else
										len--;
									aPtr=(char**)realloc(aPtr,len*sizeof(char*));
									aPtr1=(char**)realloc(aPtr1,len*sizeof(char*));
									highlight=1;
								}
								wclear(menu_win);
								box(menu_win,0,0);
								mvwprintw(menu_win,height/2,(width-strlen("Contact deleted successfully"))/2,"Contact deleted successfully");
								wrefresh(menu_win);
								sleep(1);
								wclear(menu_win);
								if(select!=3)
									m=0;
							}
							else if(highlight_int==4){
								if(select!=3){
									if(strcmp(choices_int[3],"Star")==0){
										choices_int[3]="Unstar";
										data=(ND*)malloc(sizeof(ND));
										strcpy(data->info.name,aPtr[highlight-1]);
										strcpy(data->info.phone,aPtr1[highlight-1]);
										data->next=NULL;
										if(*ptr2==NULL)
											*ptr2=data;
										else{
											data->next=*ptr2;
											*ptr2=data;
										}
										sort(*ptr2);
										add_to_file(*ptr2,"favourites.txt");	
									}
									else{
										delete(ptr2,aPtr1[highlight-1],"favourites.txt");
										choices_int[3]="Star";
									}
								}
								else{
									delete(ptr1,aPtr1[highlight-1],"favourites.txt");
										if(len!=highlight){
										len--;
										for(l=highlight-1;l<len;l++){
											aPtr[l]=aPtr[l+1];
											aPtr1[l]=aPtr1[l+1];
										}
									}
									else
										len--;
									aPtr=(char**)realloc(aPtr,len*sizeof(char*));
									aPtr1=(char**)realloc(aPtr1,len*sizeof(char*));
									highlight=1;
								}
							}
							else if(highlight_int==3){
								edit(aPtr1[highlight-1],menu_win);
								m=0;
								highlight=1;
							}
							else
								m=0;
							if(len==0)
								i=0;
						break;	
					}
					if(m)
						int_menu(menu_win,highlight_int,choices_int,n_choices_int);
				}
				if(i){	
					wclear(menu_win);
					print_menu(menu_win,highlight,aPtr,len,2);
				}
			break;
			case KEY_BACKSPACE:	
				i=0;
				move(starth+height+2,0);
				clrtoeol();
				move(starth+height+1,0);
				clrtoeol();
			break;
		}
		if(c!=KEY_BACKSPACE&&i)
		print_menu(menu_win, highlight,aPtr,len,2);
	}	
		wclear(menu_win);
		wrefresh(menu_win);
			
		free(aPtr);
		free(aPtr1);		
}

/***main***/
int main(){
	WINDOW *menu_win,*menu_win2;
	initscr();
	clear();
	noecho();
	cbreak();
	curs_set(0);
	getmaxyx(stdscr,H,W);
	ND *start,*startf;
	int choice_1,check = 1,a = 0,i = 1,j = 1,choice,choice2,n_choices,n_choices2,highlight = 1,highlight_2 = 1,startx,starty,height=10,width=30;
	char *choices[] = { 
		"1.Contacts",
		"2.Favourites",
		"3.Exit"
	},
	*choices2[] = {
		"1.Add to Contacts",
		"2.Search",
		"3.View All",
		"4.Back"
	};
	n_choices = sizeof(choices) / sizeof(char *);
	n_choices2 = sizeof(choices2) / sizeof(char *);	
	startx = (W-width)/2;
	starty = (H-height)/2;
	menu_win = newwin(height,width,starty,startx);
	keypad(menu_win,TRUE);
	menu_win2 = newwin(height,width,starty,startx);
	keypad(menu_win2,TRUE);
	print_menu(menu_win,highlight,choices,n_choices,1);
	start = startf = NULL;
	read_data(&start,"contacts.txt");
	read_data(&startf,"favourites.txt");
	attron(A_UNDERLINE);
	attron(A_BOLD);
	mvprintw(15,(W-strlen("CMS 1.0"))/2,"CMS 1.0");
	refresh();
	attroff(A_UNDERLINE);
	attroff(A_BOLD);
	while(i){
			choice = wgetch(menu_win);
			switch(choice){
				case KEY_UP:
					if(highlight == 1)
						highlight=n_choices;
					else 
						--highlight;
				break;
				case KEY_DOWN:
					if(highlight == n_choices)
						highlight = 1;
					else
						++highlight;
				break;
				case 10:
					if(highlight == 1){
						attron(A_BOLD);
						mvprintw(15,(W-strlen("Contacts"))/2,"Contacts");
						refresh();
						attroff(A_BOLD);
						print_menu(menu_win2,highlight_2,choices2,n_choices2,1);
						j = 1;
						while(j){
							choice2 = wgetch(menu_win2);
							switch(choice2){
								case KEY_UP:
									if(highlight_2 == 1)
										highlight_2 = n_choices2;
									else
										--highlight_2;
								break;
								case KEY_DOWN:
									if(highlight_2 == n_choices2)
										highlight_2 = 1;
									else
										++highlight_2;
								break;
								case 10:
									if(highlight_2 == 1){
										wclear(menu_win2);
										wrefresh(menu_win2);
										add_contact(&start);
										sort(start);
										add_to_file(start,"contacts.txt");			
									}
									else if(highlight_2 == 2){
										wclear(menu_win2);
										wrefresh(menu_win2);
										vsf(&start,&startf,1);
									}
									else if(highlight_2 == 3){
										wclear(menu_win2);
										wrefresh(menu_win2);
										vsf(&start,&startf,2);
										}
									else{
										wclear(menu_win2);
										print_menu(menu_win,highlight,choices,n_choices,1);
										j = 0;
										highlight = highlight_2 =1;
										clear();
										refresh();
									}
								break;
							}
							if(j)
								print_menu(menu_win2, highlight_2,choices2,n_choices2,1);
						}
					}
					else if(highlight==2){
						attron(A_BOLD);
						mvprintw(15,(W-strlen("Favourites"))/2,"Favourites");
						refresh();
						attroff(A_BOLD);
						vsf(&startf,&start,3);
					}
					else{
						clear();
						refresh();
						wclear(menu_win);
						box(menu_win,0,0);
						wattron( menu_win,A_UNDERLINE);
						mvwprintw(menu_win,1,(width-strlen("Author"))/2,"Author");
						wattroff(menu_win, A_UNDERLINE);
						wattron( menu_win,A_BOLD);
						mvwprintw(menu_win,4,(width-strlen("Avik Sarkar"))/2,"Avik Sarkar");
						wattroff( menu_win,A_BOLD);
						mvwprintw(menu_win,6,(width-strlen("Website:www.codexp.xyz"))/2,"Website:www.codexp.xyz");
						mvwprintw(menu_win,8,(width-strlen("Github:/avistein/cprojects"))/2,"Github:/avistein/cprojects");
						wrefresh(menu_win);
						sleep(3);
						endwin();
						exit(0);
					}
				break;
		}
		move(15,0);
		clrtoeol();
		attron(A_UNDERLINE);
		attron(A_BOLD);
		mvprintw(15,(W-strlen("CMS 1.0"))/2,"CMS 1.0");
		refresh();
		attroff(A_UNDERLINE);
		attroff(A_BOLD);
		print_menu(menu_win, highlight,choices,n_choices,1);						
	}
}
