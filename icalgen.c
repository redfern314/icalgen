#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//TODO: implement organizer name and email?

void printIntro(){
    puts("iCal Generator for Linux");
    puts("Written by Derek Redfern\n\n");
}

FILE *openFile(){
    int contLoop = 1;
    FILE *file;
    char input[51];
    char *fname = (char*)malloc(50*sizeof(char));
    int length;
    while(contLoop){
        puts("Path and name of iCal file to create (without extension):");
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

char *dateToDay(char *datestr, char *timestr) {
    //takes a date string of format YYYYmmdd and a time string of format HHMMSS
    
    struct tm tm;
    char *temp=(char*)malloc(sizeof(char)*5);
    strncpy(temp,datestr,4);
    temp[4]='\0';
    tm.tm_year=atoi(temp)-1900;
    strncpy(temp,datestr+4,2);
    temp[2]='\0';
    tm.tm_mon=atoi(temp)-1;
    strncpy(temp,datestr+6,2);
    temp[2]='\0';
    tm.tm_mday=atoi(temp);
    strncpy(temp,timestr,2);
    temp[2]='\0';
    tm.tm_hour=atoi(temp);
    strncpy(temp,timestr+2,2);
    temp[2]='\0';
    tm.tm_min=atoi(temp);
    strncpy(temp,timestr+4,2);
    temp[2]='\0';
    tm.tm_sec=atoi(temp);
    tm.tm_isdst=-1;
    int day=-1;

    time_t t = mktime(&tm);
    day = localtime(&t)->tm_wday; // Sunday=0, Monday=1, etc.
    if (day==0) {
    return("SU");
    } else if (day==1) {
    return("MO");
    } else if (day==2) {
    return("TU");
    } else if (day==3) {
    return("WE");
    } else if (day==4) {
    return("TH");
    } else if (day==5) {
    return("FR");
    } else if (day==6) {
    return("SA");
    }
    return("");
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

    puts("\nEvent title:");
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
    //int startdate = atoi(startdatestr);
    puts("\nStart time (hhmmss):");
    char starttimestr[7];
    fgets(starttimestr,7,stdin);
    if (strstr(starttimestr,"\n")==NULL) {
        while ((flush=getchar()) != '\n' && flush != EOF); //flush stdin
    }
    //int starttime = atoi(starttimestr);

    puts("\nEnd date (yyyymmdd):");
    char enddatestr[9];
    fgets(enddatestr,9,stdin);
    if (strstr(enddatestr,"\n")==NULL) {
        while ((flush=getchar()) != '\n' && flush != EOF); //flush stdin
    }
    //int enddate = atoi(enddatestr);
    puts("\nEnd time (hhmmss):");
    char endtimestr[7];
    fgets(endtimestr,7,stdin);
    if (strstr(endtimestr,"\n")==NULL) {
        while ((flush=getchar()) != '\n' && flush != EOF); //flush stdin
    }
    //int endtime = atoi(endtimestr);

    fprintf(file,"DTEND;TZID=\"Eastern Standard Time\":%sT%s\n",enddatestr,endtimestr);
    fprintf(file,"DTSTART;TZID=\"Eastern Standard Time\":%sT%s\n",startdatestr,starttimestr);
    
    puts("\nDo you want this event to recur? [y/n]");
    char confirm='a';
    while(!((confirm=='y')||(confirm=='n'))){ //wait for valid input
        confirm=getchar(); //get first character from stdin
        while ((flush=getchar()) != '\n' && flush != EOF); //flush stdin
    }
    if(confirm=='y'){
        puts("\nHow often do you want this event to recur?");
        puts("Daily/weekly/monthly/yearly? [d/w/m/y]");
        confirm='a';
        while(!((confirm=='d')||(confirm=='w')||(confirm=='m')||(confirm=='y'))){ //wait for valid input
            confirm=getchar(); //get first character from stdin
            while ((flush=getchar()) != '\n' && flush != EOF); //flush stdin
        }
        char *freqstr;
        if (confirm=='d'){
            freqstr="day";
            fprintf(file, "RRULE:FREQ=DAILY;");
        } else if (confirm=='w') {
            freqstr="week";
            fprintf(file, "RRULE:FREQ=WEEKLY;");
            puts("\nEnter days of the week to repeat on.");
            puts("0=Su,1=M,2=Tu,3=W,4=Th,5=F,6=Sa");
            puts("Example: 12345 to repeat on every weekday.");
            char weekday[8];
            fgets(weekday,8,stdin);
            if (strstr(weekday,"\n")==NULL) {
                while ((flush=getchar()) != '\n' && flush != EOF); //flush stdin
            }
            int weekdaybool[7] = {0,0,0,0,0,0,0};
            char weekdaynames[15]="";
            int i;
            for (i=0;i<strlen(weekday);i++){
                if(weekday[i]=='0'&&!weekdaybool[0]){
                    strcat(weekdaynames,"SU,");
                    weekdaybool[0]=1;
                } else if(weekday[i]=='1'&&!weekdaybool[1]){
                    strcat(weekdaynames,"MO,");
                    weekdaybool[1]=1;
                } else if(weekday[i]=='2'&&!weekdaybool[2]){
                    strcat(weekdaynames,"TU,");
                    weekdaybool[2]=1;
                } else if(weekday[i]=='3'&&!weekdaybool[3]){
                    strcat(weekdaynames,"WE,");
                    weekdaybool[3]=1;
                } else if(weekday[i]=='4'&&!weekdaybool[4]){
                    strcat(weekdaynames,"TH,");
                    weekdaybool[4]=1;
                } else if(weekday[i]=='5'&&!weekdaybool[5]){
                    strcat(weekdaynames,"FR,");
                    weekdaybool[5]=1;
                } else if(weekday[i]=='6'&&!weekdaybool[6]){
                    strcat(weekdaynames,"SA,");
                    weekdaybool[6]=1;
                }
            }
            weekdaynames[strlen(weekdaynames)-1]=';';
            weekdaynames[strlen(weekdaynames)]='\0';
            fprintf(file,"BYDAY=%sWKST=SU;",weekdaynames);
        } else if (confirm=='m') {
            freqstr="month";
            fprintf(file, "RRULE:FREQ=MONTHLY;");
            puts("\nRecur by day (M-Su) or by date (1-31)? [day/date]");
            char daydate[5]="";
            while (strcmp(daydate,"day")&&strcmp(daydate,"date")) {
                fgets(daydate,5,stdin);
                if (strstr(daydate,"\n")==NULL) {
                    while ((flush=getchar()) != '\n' && flush != EOF); //flush stdin
                }
                daydate[strlen(daydate)-1]='\0';
            }
            if (!strcmp(daydate,"day")){
                int weeknum = ((startdatestr[6]-48)*10+(startdatestr[7]-48))/7+1;
                char *weekday=dateToDay(startdatestr,starttimestr);
                fprintf(file,"BYDAY=%i%s\n",weeknum,weekday);
            } else {
                fprintf(file,"BYMONTHDAY:%c%c",startdatestr[6],startdatestr[7]);
            }
        } else if (confirm=='y') {
            freqstr="year";
            fprintf(file, "RRULE:FREQ=YEARLY;BYMONTH=%c%c;",startdatestr[4],startdatestr[5]);
            puts("\nRecur by day (M-Su) or by date (1-31)? [day/date]");
            char daydate[5]="";
            while (strcmp(daydate,"day")&&strcmp(daydate,"date")) {
                fgets(daydate,5,stdin);
                if (strstr(daydate,"\n")==NULL) {
                    while ((flush=getchar()) != '\n' && flush != EOF); //flush stdin
                }
                daydate[strlen(daydate)-1]='\0';
            }
            if (!strcmp(daydate,"day")){
                int weeknum = ((startdatestr[6]-48)*10+(startdatestr[7]-48))/7+1;
                char *weekday=dateToDay(startdatestr,starttimestr);
                fprintf(file,"BYDAY=%i%s\n",weeknum,weekday);
            } else {
                fprintf(file,"BYMONTHDAY:%c%c",startdatestr[6],startdatestr[7]);
            }
        } else {
            puts("Something went wrong. Exiting.");
            exit(-1);
        }
        printf("\nRecur at what frequency? (1 for every %s, 2 for every other %s, etc)\n",freqstr,freqstr);
        char intervalstr[3];
        fgets(intervalstr,3,stdin);
        if (strstr(intervalstr,"\n")==NULL) {
            while ((flush=getchar()) != '\n' && flush != EOF); //flush stdin
        }
        int interval = atoi(intervalstr);

        puts("\nHow many recurrences do you want (NOT including the original)?");
        char numrecurstr[5];
        int numrecur;
        fgets(numrecurstr,5,stdin);
        numrecur=atoi(numrecurstr)+1; //TODO: check num of recurrences!
        fprintf(file,"INTERVAL=%i;COUNT=%i\n",interval,numrecur);
    }    

    fprintf(file,"END:VEVENT\nEND:VCALENDAR\n");

    fclose(file);
    return(0);
}