#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//TODO: make sure that filename ends in .ics
//      implement organizer name and email?

//Credit: maerics on stackoverflow for date to day algorithm

void printIntro(){
    puts("iCal Generator for Linux");
    puts("Written by Derek Redfern\n");
    puts("Note: DST may or may not work correctly! Has not been been tested yet.\n\n");
}

FILE *openFile(){
    int contLoop = 1;
    FILE *file;
    char input[51];
    char *fname = (char*)malloc(50*sizeof(char));
    int length;
    while(contLoop){
        puts("Name of iCal file to create (extension will be appended automatically):");
        fgets(input,51,stdin);
        length = strlen(input);
        strncpy(fname,input,length-1); //get rid of \n in input
        strcat(fname,".ics");
        file = fopen(fname,"r");
        if (!(file==NULL)){
            puts("Overwrite existing file? [y/n]");
            char confirm='a';
            char flush;
            while(!((confirm=='y')||(confirm=='n'))){ //wait for valid input
                confirm=getchar(); //get first character from stdin
                while ((flush=getchar()) != '\n' && flush != EOF); //flush stdin
            }
            if (confirm=='y'){ //confirm overwrite
                contLoop=0;
            }
            fclose(file); //close test file
        } else {
            contLoop=0; //no file exists, proceed with write
        }

    }
    printf("Writing to file %s\n",fname);
    file = fopen(fname,"w");
    return file;
}

void printCurrentTime(FILE *file){
    time_t timesinceepoch = time(NULL);
    struct tm *utctime;
    utctime = gmtime(&timesinceepoch);
    //printf("%s\n",asctime(utctime)); //asctime deprecated? use strftime?
    int year = utctime->tm_year+1900;
    int month = utctime->tm_mon+1;
    int day = utctime->tm_mday;
    int hr = utctime->tm_hour;
    char hrstr[3]="";
    if (hr<10){
        sprintf(hrstr,"0%i",hr);
    } else {
        sprintf(hrstr,"%i",hr);
    }
    int min = utctime->tm_min;
    char minstr[3]="";
    if (min<10){
        sprintf(minstr,"0%i",min);
    } else {
        sprintf(minstr,"%i",min);
    }
    int sec = utctime->tm_sec;
    char secstr[3]="";
    if (sec<10){
        sprintf(secstr,"0%i",sec);
    } else {
        sprintf(secstr,"%i",sec);
    }
    fprintf(file,"CREATED:%i%i%iT%s%s%sZ\n",year,month,day,hrstr,minstr,secstr);
    fprintf(file,"DTSTAMP:%i%i%iT%s%s%sZ\n",year,month,day,hrstr,minstr,secstr);
    fprintf(file,"LAST-MODIFIED:%i%i%iT%s%s%sZ\n",year,month,day,hrstr,minstr,secstr);
}

int main(int argc,char*argv[]){
    system("clear");
    printIntro();
    FILE *file = openFile();
    
    //write intro static iCal code
    fprintf(file,"BEGIN:VCALENDAR\nPRODID:-//Derek Redfern//iCal Generator//EN\nVERSION:2.0\n\
CALSCALE:GREGORIAN\nMETHOD:PUBLISH\nX-MS-OLK-FORCEINSPECTOROPEN:TRUE\nBEGIN:VTIMEZONE\n\
TZID:Eastern Standard Time\nBEGIN:STANDARD\nDTSTART:16011104T020000\n\
RRULE:FREQ=YEARLY;BYDAY=1SU;BYMONTH=11\nTZOFFSETFROM:-0400\nTZOFFSETTO:-0500\nEND:STANDARD\n\
BEGIN:DAYLIGHT\nDTSTART:16010311T020000\nRRULE:FREQ=YEARLY;BYDAY=2SU;BYMONTH=3\n\
TZOFFSETFROM:-0500\nTZOFFSETTO:-0400\nEND:DAYLIGHT\nEND:VTIMEZONE\nBEGIN:VEVENT\nCLASS:PUBLIC\n\
PRIORITY:5\nTRANSP:OPAQUE\n");
    srand(time(NULL)); //initialize randomizer
    fprintf(file,"UID:thisisarandomuid%i\n",rand());
    printCurrentTime(file);

    puts("Event title:");
    char title[201];
    fgets(title,201,stdin);
    puts("\nEvent description:");
    char description[801];
    fgets(description,801,stdin);
    puts("\nEvent location:");
    char location[151];
    fgets(location,151,stdin);
    fprintf(file,"ORGANIZER;CN=Derek Redfern:mailto:redfern.derek@gmail.com\n\
DESCRIPTION:%sSUMMARY;LANGUAGE=en-us:%sLOCATION:%s",description,title,location); //assuming all strings end in \n

    char flush;
    puts("\nStart date (yyyymmdd):");
    char startdatestr[9];
    fgets(startdatestr,9,stdin);
    if (strstr(startdatestr,"\n")==NULL) {
        while ((flush=getchar()) != '\n' && flush != EOF); //flush stdin
    }
    int startdate = atoi(startdatestr);
    puts("\nStart time (hhmmss):");
    char starttimestr[7];
    fgets(starttimestr,7,stdin);
    if (strstr(starttimestr,"\n")==NULL) {
        while ((flush=getchar()) != '\n' && flush != EOF); //flush stdin
    }
    int starttime = atoi(starttimestr);

    puts("\nEnd date (yyyymmdd):");
    char enddatestr[9];
    fgets(enddatestr,9,stdin);
    if (strstr(enddatestr,"\n")==NULL) {
        while ((flush=getchar()) != '\n' && flush != EOF); //flush stdin
    }
    int enddate = atoi(enddatestr);
    puts("\nEnd time (hhmmss):");
    char endtimestr[7];
    fgets(endtimestr,7,stdin);
    if (strstr(endtimestr,"\n")==NULL) {
        while ((flush=getchar()) != '\n' && flush != EOF); //flush stdin
    }
    int endtime = atoi(endtimestr);

    fprintf(file,"DTEND;TZID=\"Eastern Standard Time\":%sT%s\n",enddatestr,endtimestr);
    fprintf(file,"DTSTART;TZID=\"Eastern Standard Time\":%sT%s\n",startdatestr,starttimestr);    

    fprintf(file,"END:VEVENT\nEND:VCALENDAR\n");

    fclose(file);
    return(0);
}