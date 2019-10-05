#ifndef GEDCOMPARSER_H
#define GEDCOMPARSER_H

#include "LinkedListAPI.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum cSet {ANSEL, UTF8, UNICODE, ASCII} CharSet;

typedef enum eCode {OK, INV_FILE, INV_GEDCOM, INV_HEADER, INV_RECORD, OTHER_ERROR, WRITE_ERROR} ErrorCode;

/*

We start by defining the different structures of bouth the encoding and layout
of the gedcom file

*/

typedef struct {
    char type[5];
    char* date;
    char* place;
    List    otherFields;
} Event;

typedef struct {
    char* tag;
    char* value;
} Field;

typedef struct {
    char    submitterName[61];
    List    otherFields;
    char    address[];
} Submitter;

typedef struct {
    char        source[249];
    float       gedcVersion;
    CharSet     encoding;
    Submitter*  submitter;
    List        otherFields;
} Header;

typedef struct {
    char*    givenName;
    char*    surname;
    List    events;
    List    families;
    List    otherFields;
} Individual;

typedef struct {
    Individual* wife;
    Individual* husband;
    List        children;
    List        events;
    List        otherFields;
} Family;

typedef struct {
    Header*     header;
    List        families;
    List        individuals; //Must contain type Family
    Submitter*  submitter;
} GEDCOMobject;


typedef struct {
    ErrorCode   type;
    int         line;
} GEDCOMerror;


GEDCOMerror createGEDCOM(char* fileName, GEDCOMobject** obj);
char* printGEDCOM(const GEDCOMobject* obj);
void deleteGEDCOM(GEDCOMobject* obj);
char* printError(GEDCOMerror err);
Individual* findPerson(const GEDCOMobject* familyRecord, bool (*compare)(const void* first, const void* second), const void* person);
List getDescendants(const GEDCOMobject* familyRecord, const Individual* person);
GEDCOMerror writeGEDCOM(char* fileName, const GEDCOMobject* obj);
ErrorCode validateGEDCOM(const GEDCOMobject* obj);
List getDescendantListN(const GEDCOMobject* familyRecord, const Individual* person, unsigned int maxGen);
List getAncestorListN(const GEDCOMobject* familyRecord, const Individual* person, int maxGen);
char* indToJSON(const Individual* ind);

Individual* JSONtoInd(const char* str);

GEDCOMobject* JSONtoGEDCOM(const char* str);

void addIndividual(GEDCOMobject* obj, const Individual* toBeAdded);

char* iListToJSON(List iList);

char* gListToJSON(List gList);

void deleteGeneration(void* toBeDeleted);
int compareGenerations(const void* first,const void* second);
char* printGeneration(void* toBePrinted);

//************************************************************************************************************
void deleteField(void* toBeDeleted);
int compareFields(const void* first,const void* second);
char* printField(void* toBePrinted);

void deleteEvent(void* toBeDeleted);
int compareEvents(const void* first,const void* second);
char* printEvent(void* toBePrinted);

void deleteIndividual(void* toBeDeleted);
int compareIndividuals(const void* first,const void* second);
char* printIndividual(void* toBePrinted);

void deleteFamily(void* toBeDeleted);
int compareFamilies(const void* first,const void* second);
char* printFamily(void* toBePrinted);


//************************************************************************************************************

#endif
