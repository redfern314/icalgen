#include <form.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <ncurses.h>
#include <string.h>
#include "funcs.h"

FILE *openFile(){
    FIELD *field[3];
    FORM  *my_form;
    int ch;

    int contLoop = 1;
    FILE *file;
    char *fname = (char*)malloc(50*sizeof(char));;

    while(contLoop){

        /* Initialize the fields */
        field[0] = new_field(1, 30, 5, 1, 0, 0); //filename
        field[1] = new_field(1, 1, 7, 33, 0, 0); //overwrite?
        field[2] = NULL;

        /* Set field options */
        set_field_back(field[0], A_UNDERLINE);  /* Print a line for the option  */
        field_opts_off(field[0], O_AUTOSKIP);   /* Don't go to next field when this */
                            /* Field is filled up       */
        //set_field_back(field[1], A_UNDERLINE); 
        field_opts_off(field[1], O_AUTOSKIP);
        field_opts_off(field[1], O_VISIBLE);

        /* Create the form and post it */
        my_form = new_form(field);
        post_form(my_form);
        refresh();
        mvprintw(0,0,"iCal Generator\nWritten by Derek Redfern");
        mvprintw(4, 1, "Path and name of iCal file to create (without extension):");
        
        refresh();
        form_driver(my_form, REQ_FIRST_FIELD);
        /* Loop through to get user requests */
        while((ch = getch()) != '\n')
        {   switch(ch)
            {   case KEY_BACKSPACE:
                    form_driver(my_form, REQ_LEFT_CHAR);
                    form_driver(my_form, REQ_DEL_CHAR);
                    break;
                case KEY_LEFT:
                    form_driver(my_form, REQ_LEFT_CHAR);
                    break;
                case KEY_RIGHT:
                    form_driver(my_form, REQ_RIGHT_CHAR);
                    break;
                default:
                    /* If this is a normal character, it gets */
                    /* Printed                */    
                    form_driver(my_form, ch);
                    break;
            }
        }

        form_driver(my_form,REQ_VALIDATION);
        char *temp = field_buffer(field[0],0);
        int length = strlen(temp);
        strncpy(fname,temp,length);
        trimTrailingWhitespace(fname);
        strcat(fname,".ics");
        file = fopen(fname,"r");
        if (!(file==NULL)){
            mvprintw(7, 1, "Overwrite existing file? [y/n]: ");
            field_opts_on(field[1], O_VISIBLE);
            form_driver(my_form, REQ_NEXT_FIELD);
            field_opts_off(field[0], O_ACTIVE);

            while((ch = getch()) != '\n')
            {   switch(ch)
                {   case KEY_BACKSPACE:
                        form_driver(my_form, REQ_DEL_PREV);
                        break;
                    case 'y':   
                        form_driver(my_form, ch);
                        break;
                    case 'n':
                        form_driver(my_form, ch);
                        break;
                }
            }
            form_driver(my_form,REQ_VALIDATION);
            char *confirm = field_buffer(field[1],0);
            if (confirm[0]=='y'){ //confirm overwrite
                contLoop=0;
            } else {
                fclose(file); //close test file
            }
        } else {
            contLoop=0; //no file exists, proceed with write
        }

        /* Un post form and free the memory */
        unpost_form(my_form);
        free_form(my_form);
        free_field(field[0]);
        free_field(field[1]); 
    }
    file = fopen(fname,"w");
    return file;
}

void getSummary(char *title, char *desc, char *loc){
    FIELD *field[3];
    FORM  *my_form;
    int ch;
    int contLoop=1;
    char *temp = (char*)malloc(50*sizeof(char));

    /* Initialize the fields */
    field[0] = new_field(1, 50, 5, 25, 0, 0); //title
    field[1] = new_field(4, 50, 7, 25, 0, 0); //description
    field[2] = new_field(1, 30, 12, 25, 0, 0); //location

    /* Set field options */
    set_field_back(field[0], A_UNDERLINE);
    field_opts_off(field[0], O_AUTOSKIP); 
                        
    set_field_back(field[1], A_UNDERLINE); 
    field_opts_off(field[1], O_AUTOSKIP);

    set_field_back(field[2], A_UNDERLINE); 
    field_opts_off(field[2], O_AUTOSKIP);

    /* Create the form and post it */
    my_form = new_form(field);
    post_form(my_form);
    mvprintw(0,0,"iCal Generator\nWritten by Derek Redfern");
    mvprintw(3,1,"Use arrow keys to navigate; press Enter to proceed\n");
    mvprintw(5, 1, "Event title (required): ");
    mvprintw(7, 1, "Event description: ");
    mvprintw(12, 1, "Event location: ");
    refresh();
    //getch();
    form_driver(my_form, REQ_FIRST_FIELD);
    /* Loop through to get user requests */
    while(contLoop)
    {   ch = getch();
        switch(ch)
        {   case KEY_BACKSPACE:
                form_driver(my_form, REQ_DEL_PREV);
                break;
            case KEY_DC:
                form_driver(my_form, REQ_DEL_CHAR);
                break;
            case KEY_LEFT:
                if (form_driver(my_form, REQ_LEFT_CHAR)==E_REQUEST_DENIED){
                    form_driver(my_form, REQ_PREV_LINE);
                }
                break;
            case KEY_RIGHT:
                if (form_driver(my_form, REQ_RIGHT_CHAR)==E_REQUEST_DENIED){
                    form_driver(my_form, REQ_NEXT_LINE);
                }
                break;
            case KEY_UP:
                if (form_driver(my_form, REQ_PREV_LINE)==E_REQUEST_DENIED){
                    form_driver(my_form, REQ_PREV_FIELD);
                }
                break;
            case KEY_DOWN:
                if (form_driver(my_form, REQ_NEXT_LINE)==E_REQUEST_DENIED){
                    form_driver(my_form, REQ_NEXT_FIELD);
                }
                break;
            case '\n':
                form_driver(my_form,REQ_VALIDATION);
                strcpy(temp,field_buffer(field[0],0));
                trimTrailingWhitespace(temp);
                if (strlen(temp)>0) contLoop=0;
            default:
                form_driver(my_form, ch);
                break;
        }
    }
    title = field_buffer(field[0],0);
    desc = field_buffer(field[1],0);
    loc = field_buffer(field[2],0);
    unpost_form(my_form);
    free_form(my_form);
    free_field(field[0]);
    free_field(field[1]); 
    free_field(field[2]); 
}

void getDateTime(char *title, char *desc, char *loc){
    FIELD *field[5];
    FORM  *my_form;
    int ch;
    int contLoop=1;
    char *temp = (char*)malloc(50*sizeof(char));

    /* Initialize the fields */
    field[0] = new_field(1, 2, 6, 5, 0, 0);
    field[1] = new_field(1, 2, 6, 8, 0, 0);
    field[2] = new_field(1, 4, 6, 11, 0, 0);
    field[3] = new_field(1, 2, 6, 26, 0, 0);
    field[4] = new_field(1, 2, 6, 29, 0, 0);

    /* Set field options */
    set_field_back(field[0], A_UNDERLINE);
    field_opts_off(field[0], O_NULLOK+O_BLANK); 
    set_field_type(field[0], TYPE_INTEGER,2,1,12);

    set_field_back(field[1], A_UNDERLINE); 
    field_opts_off(field[1], O_NULLOK+O_BLANK);
    set_field_type(field[1], TYPE_INTEGER,2,1,31);

    set_field_back(field[2], A_UNDERLINE); 
    field_opts_off(field[2], O_NULLOK+O_BLANK);
    set_field_type(field[2], TYPE_INTEGER,4,0,9999);

    set_field_back(field[3], A_UNDERLINE);
    field_opts_off(field[3], O_NULLOK+O_BLANK); 
    set_field_type(field[3], TYPE_INTEGER,2,0,23);
                        
    set_field_back(field[4], A_UNDERLINE); 
    field_opts_off(field[4], O_NULLOK+O_BLANK);
    set_field_type(field[4], TYPE_INTEGER,2,0,59);

    /* Create the form and post it */
    my_form = new_form(field);
    post_form(my_form);
    mvprintw(0,0,"iCal Generator\nWritten by Derek Redfern");
    mvprintw(3,1,"Use arrow keys to navigate; press Enter to proceed\n");
    mvprintw(5,5,"Start date:");
    mvprintw(5,26,"Start time:");
    mvprintw(6,7,"/");
    mvprintw(6,10,"/");
    mvprintw(6,28,":");

    refresh();
    //getch();
    form_driver(my_form, REQ_FIRST_FIELD);
    
    get_input(my_form,field);
    move_field(field[0],9,5);
    move_field(field[1],9,8);
    move_field(field[2],9,11);
    move_field(field[3],9,26);
    move_field(field[4],9,29);
    int i;
    for(i=0;i<5;i++){
        form_driver(my_form, REQ_CLR_FIELD);
        form_driver(my_form, REQ_NEXT_FIELD);
    }
    refresh();
    get_input(my_form,field);
    title = field_buffer(field[0],0);
    desc = field_buffer(field[1],0);
    loc = field_buffer(field[2],0);
    unpost_form(my_form);
    free_form(my_form);
    free_field(field[0]);
    free_field(field[1]); 
    free_field(field[2]); 
}

void get_input(FORM *my_form, FIELD **field){
    int ch;
    int contLoop=1;
    char *temp = (char*)malloc(50*sizeof(char));
    while(contLoop)
    {   ch = getch();
        switch(ch)
        {   case KEY_BACKSPACE:
                form_driver(my_form, REQ_DEL_PREV);
                break;
            case KEY_DC:
                form_driver(my_form, REQ_DEL_CHAR);
                break;
            case KEY_LEFT:
                if (form_driver(my_form, REQ_LEFT_CHAR)==E_REQUEST_DENIED){
                    form_driver(my_form, REQ_PREV_FIELD);
                }
                break;
            case KEY_RIGHT:
                if (form_driver(my_form, REQ_RIGHT_CHAR)==E_REQUEST_DENIED){
                    form_driver(my_form, REQ_NEXT_FIELD);
                }
                break;
            case KEY_UP:
                if (form_driver(my_form, REQ_PREV_LINE)==E_REQUEST_DENIED){
                    form_driver(my_form, REQ_PREV_FIELD);
                }
                break;
            case KEY_DOWN:
                if (form_driver(my_form, REQ_NEXT_LINE)==E_REQUEST_DENIED){
                    form_driver(my_form, REQ_NEXT_FIELD);
                }
                break;
            case '\n':
                if (form_driver(my_form,REQ_VALIDATION)==E_OK){
                    strcpy(temp,field_buffer(field[0],0));
                    trimTrailingWhitespace(temp);
                    if (strlen(temp)>0) contLoop=0;
                }
            default:
                form_driver(my_form, ch);
                break;
        }
    }
}


int main()
{   
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    //open file and write intro data to file
    FILE *file;
    file = openFile();

    //collect summary data from user and write to file
    char *title = "";
    char *desc = "";
    char *loc = "";
    char temp[3] = "";
    //sprintf(temp,"a");
    getSummary(title, desc, loc);

    //collect data/time data from user and write to file
    char *startdatetime = "";
    char *enddatetime = "";
    getDateTime(startdatetime,enddatetime,title);
    
    //!!!!!!remember: [date]T[time]!!!!!!!!!!!!!

    //clean up
    getch();
    endwin();

    //print data to file
//     printIntro(file);
//     printCurrentTime(file);
//     fprintf(file,"ORGANIZER;CN=Derek Redfern:mailto:redfern.derek@gmail.com\n\
// DESCRIPTION:%s\nSUMMARY;LANGUAGE=en-us:%s\nLOCATION:%s\n",\
// desc,title,loc);
//     fprintf(file,"DTSTART;TZID=\"Eastern Standard Time\":%s\n",startdatetime);
//     fprintf(file,"DTEND;TZID=\"Eastern Standard Time\":%s\n",enddatetime);
//     fprintf(file,"END:VEVENT\nEND:VCALENDAR\n");
    fclose(file);
    return 0;
}
