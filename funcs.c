#include "funcs.h"

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

void trimTrailingWhitespace(char *string){
    char *end = string + strlen(string) - 1;
    while(end >= string && isspace(*end)) end--;
    *(end+1)='\0';
}

void printIntro(FILE *file){
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
}

char *dateToDay(char *datestr, char *timestr) {
    //takes a date string of format YYYYmmdd and a time string of format HHMMSS
    
    struct tm tm;
    char *temp=(char*)malloc(sizeof(char)*50);
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