#include "GEDCOMutilities.h"
#include "GEDCOMparser.h"
#include "LinkedListAPI.h"

char* setLowerCase(char* string){
    //loop through the char change to lower case
    for(int x=0; x <strlen(string)+1;x++){
        //to prevent conversion when its not letter
        if(UPPERCASE_RANGE(string[x]) || LOWERCASE_RANGE(string[x])){
            string[x] = tolower(string[x]);
        }//end if
    }//end if
    //return
    return string;
}//end func

char* setUpperCase(char* string){
    //loop through the char change to upper case
    for(int x=0; x <strlen(string)+1;x++){
        //to prevent conversion when its not letter
        if(UPPERCASE_RANGE(string[x]) || LOWERCASE_RANGE(string[x])){
            string[x] = toupper(string[x]);
        }//end if
    }//end if
    //return
    return string;
}//end func

char* setString(char* string){
    char* newString = calloc(1 ,sizeof(char)*(strlen(string)+9));
    strcpy(newString, string);
    strcat(newString, "\0");
    return newString;
}//end func

char** split(char* stringToBeSplit, const char* delimiter, int* arraySize){
    //create string array
    int numOfSplits = 0;
    char** stringArray = calloc(1 ,sizeof(stringArray)*(numOfSplits+1));
    //tokenize through thw code
    char *token;
    char* stringBuffer = setString(stringToBeSplit);
    token = strtok(stringBuffer, delimiter);
    //iterate through the tokem
    while(token != NULL) {
        //copy the split token
        stringArray[numOfSplits] = setString(token);
        //keep going
        token = strtok(NULL, delimiter);
        //realloc more memory
        numOfSplits++;
        stringArray = realloc(stringArray, sizeof(stringArray)*(numOfSplits+1));
    }//end if
    //return
    *arraySize = numOfSplits;
    free(stringBuffer);
    return stringArray;
}//end func

char** readFileByLine(char* fileName, int* arraySize, const int lineSize){
    //dec var
    FILE* filePointer = fopen(fileName, "r");
    char line[lineSize];
    char** stringArray = calloc(1 ,sizeof(stringArray));
    int numberOfLines = 0;

    //if file doesnt exist
    if(filePointer == NULL) {
        if(DEBUG_HELPER)printf("No such file.\n");
        free(stringArray);
        return NULL;
    }//end if

    if(DEBUG_HELPER)printf("Loading string...\n");
    //scan until end of file
    while (fgets(line, sizeof(line), filePointer) != NULL) {
        //copy string
        if(DEBUG_HELPER)printf("line %d: %s", numberOfLines, line);
        removeLineEnding(line);
        line[strcspn(line, "\r\n")] = '\0';
        stringArray[numberOfLines] = setString(line);
        numberOfLines++;
        //create string array from each line
        stringArray = realloc(stringArray, sizeof(stringArray)*(numberOfLines+1));
        //go to the next
    }//end loop
    if(DEBUG_HELPER)printf("\nString loaded...\n");

    //return
    *arraySize = numberOfLines;
    fclose(filePointer);
    return stringArray;
}//end func

char** readFileByChar(char* fileName, int* arraySize){//wierd char fix
    //dec vars
    FILE* filePointer = fopen(fileName, "r");
    char** stringArray = calloc(1, sizeof(stringArray));
    char* stringBuffer = calloc(1, sizeof(stringBuffer));
    int charBuffer;
    int stringSize = 0;
    int lineSize = 0;

    //read the entire file to the string
    while(true){
        charBuffer = getc(filePointer);
        if(charBuffer == EOF){
            break;
        }//end if
        stringBuffer[stringSize] = (char)charBuffer;
        stringSize++;
        stringBuffer = realloc(stringBuffer, sizeof(stringBuffer)*(stringSize+1));
        stringBuffer[stringSize] = '\0';
    }//end if*/

    //remove the \r\n by tokenizing and adding to array
    char* token = strtok(stringBuffer, "\r\n");
    while(token != NULL){
        stringArray[lineSize] = setString(token);
        strcpy(stringArray[lineSize], token);
        //debug("Tokenized: %s\n", stringArray[lineSize]);
        lineSize++;
        stringArray = realloc(stringArray, sizeof(stringArray)*(lineSize+1));
        token = strtok(NULL, "\r\n");
    }//end while

    //set the size and return
    *arraySize = lineSize;
    free(stringBuffer);
    fclose(filePointer);
    return stringArray;
}//end func

void freeStringArray(char** array, int arraySize){
    for(int x = 0; x < arraySize; x++) {
        free(array[x]);
    }//end if
    free(array);
}//end func

bool checkEmail(char* string){
    //dec vars
    int foundAtSign = 0;
    int foundPeriod = 0;

    //if string size is greater than 5 it is impossible to be an email
    int stringLength = strlen(string);
    if(DEBUG_HELPER)printf("String length: %d\n", stringLength);
    if(stringLength < 5){
        return false;
    }//end if

    //go through the string and check
    for(int x=1; x<strlen(string)-1;x++){
        if (string[x-1] != '@' && string[x] == '@' && string[x+1] != '@'){
            foundAtSign++;
            if(DEBUG_HELPER)printf("Found '@'.\n");
        }//end if
        if( (string[x-1] != '@' && string[x-1] != '.') && string[x] == '.' && ALPHABET_RANGE(string[x+1]) && foundAtSign == 1 ){
            foundPeriod++;
            if(DEBUG_HELPER)printf("Found '.'.\n");
        }//end if
    }//end for loop

    //return
    if (foundAtSign == 1 && foundPeriod >= 1){
        return true;
    }//end if
    return false;
}//end func

int compareString(char* string1, char* string2){
    //set temp string for the strings to not be manipulated
    char* tempString1 = setString(string1);
    char* tempString2 = setString(string2);
    //compare with having both lower case
    int result = strcmp(setLowerCase(tempString1), setLowerCase(tempString2));
    //free and return
    free(tempString1);
    free(tempString2);
    return result;
}//end func

int searchStringArray(char* stringArray[], char* stringToBeSearched, int arraySize, char* type){
    //dec vars
    int index = -999;
    int foundString = 0;
    //loop through to find the string
    for (int x = 0; x < arraySize; x++){
        //if the string to be searched is found returned the size
        if (compareString(stringArray[x], stringToBeSearched) == 0){
            index = x;
            foundString++;
        }//end if
    }//for loop
    //return based on the searched type
    if(compareString(type, "index") == 0){
        return index;
    }else{
        return foundString;
    }//end if
}//end func

char* combinedToken(char** token, int tokenSize, int tokenStartIndex){//untested = so far it works
    //prevent memory leaked
    if(tokenStartIndex > tokenSize){
        return NULL;
    }//end if
    //dec vars
    int mallocSize = 256 + strlen(token[tokenStartIndex]);
    char* combinedString = calloc(1 ,sizeof(char)*(mallocSize));
    int x = 0;

    //loop through and combined the string to one
    for(x=tokenStartIndex; x<tokenSize; x++){
        //start to combined the string
        char* newString = setString(token[x]);
        strcat(combinedString, newString);
        free(newString);
        if(x == tokenSize){
            strcat(combinedString, "\0");
            break;
        }//end if
        strcat(combinedString, " ");
        mallocSize = mallocSize + strlen(token[x]);
        combinedString = realloc(combinedString, sizeof(char)*mallocSize);
    }//end if
    combinedString[strlen(combinedString)-1] = '\0';

    if(DEBUG_UTILITIES)printf("combinedToken: %s\n", combinedString);
    return combinedString;
}//end func

void removeSpace(char* string){
    //remove the space of the white spaces of the string
    if(string == NULL){
        return;
    }//end if
    if(strlen(string) < 3){
        return;
    }//end if
    //right to left
    for(int x = strlen(string)-1; x > 0; x--){
        if(string[x] == ' '){
            string[x] = '\0';
        }else if(string[x] != ' '){
            break;
        }//end
    }//end if
    //left to right
    for(int x = 0; x < strlen(string)-1; x++){
        if(string[x] == ' '){
            string[x] = '\0';
        }else if(string[x] != ' '){
            break;
        }//end
    }//end if
}//end func

void removeLineEnding(char* string){
    for(int x=0; x<255; x++){
        if(string[x] == '\r' || string[x] == '\n'){
            string[x] = '\0';
            break;
        }else if(string[x] == '\0'){
            return;
        }//end if
    }//end for
}//end func

List* initializeListPointer(char* (*printFunction)(void* toBePrinted),void (*deleteFunction)(void* toBeDeleted),int (*compareFunction)(const void* first,const void* second)){
    //init all the instance vars of the list
    List* newList = malloc(sizeof(List));
    newList->head = NULL;
    newList->tail = NULL;
    newList->length = 0;
    //assign the pointer functions
    newList->deleteData = deleteFunction;
    newList->compare = compareFunction;
    newList->printData = printFunction;
    //return the new list
    return newList;
}//end constructor

Event* initEvent(){
    Event* newEvent = calloc(1, sizeof(Event));
    newEvent->type[0] = '\0';
    newEvent->date = NULL;
    newEvent->place = NULL;
    newEvent->otherFields = initializeList(printField, deleteField, compareFields);
    return newEvent;
}//end constructor

Field* initField(){
    Field* newField = calloc(1, sizeof(Field));
    newField->tag = NULL;
    newField->value = NULL;
    return newField;
}//end construtor

Submitter* initSubmitter(){
    Submitter* newSub = calloc(1, sizeof(Submitter) + sizeof(char [999]));
    newSub->address[0] = '\0';
    newSub->submitterName[0] = '\0';
    newSub->otherFields = initializeList(printField, deleteField, compareFields);
    return newSub;
}//end constructor

Header* initHeader(){
    Header* newHeader = calloc(1, sizeof(Header));
    newHeader->source[0] = '\0';
    newHeader->gedcVersion = -1;
    newHeader->encoding = ASCII;//not sure if its ascii
    newHeader->submitter = NULL;//need to be init
    newHeader->otherFields = initializeList(printField, deleteField, compareFields);
    return newHeader;
}//end constructor

Individual* initIndividual(){
    Individual* newIndividual = calloc(1, sizeof(Individual));;
    newIndividual->givenName = NULL;
    newIndividual->surname = NULL;
    newIndividual->events = initializeList(printEvent, deleteEvent, compareEvents);
    newIndividual->families = initializeList(printFamily, dummyDelete, compareFamilies);
    newIndividual->otherFields = initializeList(printField, deleteField, compareFields);
    return newIndividual;
}//end constructor

Family* initFamily(){
    Family* newFamily = calloc(1, sizeof(Family));
    newFamily->wife = NULL;
    newFamily->husband =NULL;
    newFamily->children = initializeList(printIndividualName, dummyDelete, compareIndividuals);
    newFamily->events = initializeList(printEvent, deleteEvent, compareEvents);
    newFamily->otherFields = initializeList(printField, deleteField, compareFields);
    return newFamily;
}//end construstor

GEDCOMobject* initObject(){
    GEDCOMobject* newObject = calloc(1, sizeof(GEDCOMobject));
    newObject->header = initHeader();
    newObject->families = initializeList(printFamily, deleteFamily, compareFamilies);
    newObject->individuals = initializeList(printIndividual, deleteIndividual, compareIndividuals);
    newObject->submitter = NULL;//need to be init
    return newObject;
}//end constructor

GEDCOMerror initError(ErrorCode errorCode, int line){
    #define DEBUG_ERROR false
    GEDCOMerror newError;
    newError.type = errorCode;
    newError.line = line;//not sure if it start with 1 or 0
    return newError;
}//end constructor

HelperVars initVars(char* fileName, char** stringArrayOfFile, int lineSize){
    //dec vars
    HelperVars newVars;
    newVars.object = initObject();
    newVars.indiRef = initializeList(printReference, deleteIndiRef, compareIndiRef);
    newVars.famRef = initializeList(printReference, deleteFamRef, compareFamRef);
    newVars.garbageFamily = initializeList(printFamily, deleteGarbageFamily, compareFamilies);
    strcpy(newVars.fileName, fileName);
    //vars for the file
    newVars.lineLocation = 0;
    newVars.lineSize = lineSize;
    newVars.lineString = stringArrayOfFile;
    //dec found vars for GED_COM
    newVars.foundHead = 0;
    newVars.foundTrailer = 0;
    newVars.foundSubmitter = 0;
    newVars.foundRecord = 0;
    //dec vars for the header tags
    newVars.foundSourceTag = 0;
    newVars.foundGedcVersionTag = 0;
    newVars.foundGedcTag = 0;
    newVars.foundEncodingTag = 0;
    newVars.foundSubmitterTag = 0;
    newVars.foundEndHeader = 0;
    newVars.endHeaderLineLocation = 0;
    //for individual
    newVars.foundStartIndividual = 0;
    newVars.foundIndividual = 0;
    newVars.foundStartNameTag = 0;
    newVars.foundNameTag = 0;
    newVars.foundGivenNameTag = 0;
    newVars.foundSurnameTag = 0;
    //for family
    newVars.foundStartFamily = 0;
    newVars.foundFamily = 0;
    newVars.foundHusband = 0;
    newVars.foundWife = 0;
    newVars.foundChild = 0;
    //return the init helper vars
    newVars.counter = 0;
    newVars.foundGarbageFamily = 0;
    return newVars;
}//end constructor

IndividualReference* initIndiRef(char* ref, Individual* indi){
    IndividualReference* newIndi = calloc(1, sizeof(IndividualReference));
    newIndi->reference = setString(ref);
    newIndi->individual = indi;
    newIndi->found = false;
    newIndi->foundFAMS = false;
    newIndi->foundFAMC = false;
    newIndi->famcRef = initializeList(dummyPrint, freeObject, dummyCompare);
    newIndi->famsRef = initializeList(dummyPrint, freeObject, dummyCompare);
    newIndi->depth = 0;
    return newIndi;
}//end constructor

FamilyReference* initFamRef(char* ref, Family* fam){
    FamilyReference* newFam = calloc(1, sizeof(FamilyReference));
    newFam->reference = setString(ref);
    newFam->family = fam;
    newFam->found = false;
    newFam->husbRef = NULL;
    newFam->wifeRef = NULL;
    newFam->childRef = initializeList(dummyPrint, freeObject, dummyCompare);
    return newFam;
}//end constructor

IndividualReference* initIndividualReference(){
    IndividualReference* newIndi = calloc(1, sizeof(IndividualReference));
    newIndi->reference = NULL;
    newIndi->individual = NULL;
    newIndi->found = false;
    newIndi->foundFAMS = false;
    newIndi->foundFAMC = false;
    newIndi->famcRef = initializeList(dummyPrint, freeObject, dummyCompare);
    newIndi->famsRef = initializeList(dummyPrint, freeObject, dummyCompare);
    newIndi->depth = 0;
    return newIndi;
}//end constructor

FamilyReference* initFamilyReference(){
    FamilyReference* newFam = calloc(1, sizeof(FamilyReference));
    newFam->reference = NULL;
    newFam->family = NULL;
    newFam->found = false;
    newFam->husbRef = NULL;
    newFam->wifeRef = NULL;
    newFam->childRef = initializeList(dummyPrint, freeObject, dummyCompare);
    return newFam;
}//end constructor

// check main
GEDCOMerror checkFile(char* fileName){//checked
    if(DEBUG_UTILITIES)printf("\n******Invoking checkFile function*****\n");

    //check if the file name is empty
    if(fileName == NULL){
        fprintf(stderr, "File name cannot be empty string.\n");
        return initError(INV_FILE, -1);
    }//end if

    //check if the file names end with ".ged"
    if (strstr(fileName, ".ged") == NULL) {
        fprintf(stderr, "File extension must be a (.ged).\n");
        return initError(INV_FILE, -1);
    }//end if

    //check if the file can be opened
    FILE* fileCheck = fopen(fileName, "r");
    if(fileCheck == NULL){
        fprintf(stderr, "File cannot be opened.\n");
        return initError(INV_FILE, -1);;
    }//end if
    fclose(fileCheck);

    //return
    return initError(OK, -1);
}//end file check

GEDCOMerror checkGedcom(HelperVars* vars){//worked, no leak
    LIST_OF_FAMILY_GARBAGE = initializeList(printFamily, deleteGarbageFamily, compareFamilies);
    if(DEBUG_UTILITIES)printf("\n******Invoking checkGedcom function*****\n");

    //check if there is error with gedcom
    for(int x=0; x<vars->lineSize;x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
        static char* recordList[] = {
            //index 0
            "INDI", "FAM", "SUBM",
            //the for the main GEDCOM record
            "TRLR", "HEAD"
        };//end array
        printToken(token, tokenSize);

        //make sure that the token is at least 2
        if(tokenSize > 1){
            //check the head
            if(tokenSize == 2){
                if(strcmp(token[1], "HEAD") == 0){
                    vars->foundHead++;
                    if(DEBUG_UTILITIES)printf("foundHead: %d\n", vars->foundHead);
                }//end if
            }//end if

            //check if record exist
            if(tokenSize == 3){
                if(searchStringArray(recordList, token[2], RECORD_SIZE, "FOUND") > 0){
                    vars->foundRecord++;
                    if(DEBUG_UTILITIES)printf("foundRecord: %d\n", vars->foundRecord);
                    //check if it is SUBM record
                    if(strcmp(token[2], "SUBM") == 0){
                        vars->foundSubmitter++;
                        if(DEBUG_UTILITIES)printf("foundSubmitter: %d\n", vars->foundSubmitter);
                    }//end if
                }//end if
            }//end func

            //check for TRLR
            if(vars->foundHead == 1 && vars->foundSubmitter == 1 && vars->foundRecord >= 1 ){
                //make sure the token size is 2
                if(tokenSize == 2){
                    if(strcmp(token[1], "TRLR") == 0){
                        vars->foundTrailer++;
                        if(DEBUG_UTILITIES)printf("foundTrailer: %d\n", vars->foundTrailer);
                        freeStringArray(token, tokenSize);
                        break;
                    }//end if
                }//end if
            }else if(vars->foundHead > 1){
                if(DEBUG_UTILITIES)printf("Found duplicate heads\n");
                freeStringArray(token, tokenSize);
                return initError(INV_GEDCOM, -1);
            }//end if
        }//end if

        //free the token
        freeStringArray(token, tokenSize);
    }//end for

    //return the result
    debug("foundHead: %d, foundTrailer: %d, foundSubmitter: %d\n", vars->foundHead, vars->foundTrailer, vars->foundSubmitter);
    if(vars->foundHead == 1 && vars->foundTrailer == 1 && vars->foundSubmitter == 1){
        return initError(OK, -1);
    }else{
        return initError(INV_GEDCOM, -1);
    }//end if
}//end func

GEDCOMerror checkHeader(HelperVars* vars){//worked no leak
    if(DEBUG_UTILITIES)printf("\n*****Invoking checkHeader function*****\n");
    //dec vars
    int lastLine = 0;
    bool otherField = true;
    bool foundHead = false;

    //check if the is an error in header
    for(int x=0; x<vars->lineSize; x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
        otherField = true;
        printToken(token, tokenSize);

        //check if there is a new record *****!!!!! not sure if it can end if "O TRLR"
        if(tokenSize == 2){
            if(strcmp(token[1], "HEAD") == 0){
                foundHead = true;
                otherField = false;
                if(DEBUG_UTILITIES)printf("foundHead: %d\n", vars->foundHead);
                //check the correct line number
                if(strcmp(token[0], "0") != 0){
                    freeStringArray(token, tokenSize);
                    return initError(INV_HEADER, x+1);
                }//end if
            }//end if
        }else if(strcmp(token[0], "0") == 0){
            if(tokenSize == 3 && foundHead == true){
                vars->foundEndHeader++;
                vars->endHeaderLineLocation = x+1;
                lastLine = x;
                if(DEBUG_UTILITIES)printf("lastLine: %d\n", lastLine);
                freeStringArray(token, tokenSize);
                break;
            }//end if
        }//end if

        //check 4 grammar mistakes
        if(checkIfValidLine(vars->lineString[x]) == false){
            freeStringArray(token, tokenSize);
            return initError(INV_RECORD, x+1);
        }//end if

        //make sure that the token is greater 1
        if(tokenSize > 1 && foundHead == true){
            //check the source
            if(tokenSize >= 2){
                if(strcmp(token[1], "SOUR") == 0){
                    otherField = false;
                    vars->foundSourceTag++;
                    if(DEBUG_UTILITIES)printf("foundSourceTag: %d\n", vars->foundSourceTag);
                    //check the correct line number
                    if(strcmp(token[0], "1") != 0 || tokenSize == 2){
                        freeStringArray(token, tokenSize);
                        return initError(INV_HEADER, x+1);
                    }//end if
                    //assign the source to the object
                    char* combinedString = combinedToken(token, tokenSize, 2);
                    strcpy(vars->object->header->source, combinedString);
                    free(combinedString);
                    if(DEBUG_UTILITIES)printf("source: %s\n", vars->object->header->source);
                }//end if
            }//end if

            //check the gedc version
            if(tokenSize == 2){
                if(strcmp(token[1], "GEDC") == 0){
                    otherField = false;
                    vars->foundGedcTag++;
                    if(DEBUG_UTILITIES)printf("foundGedcTag: %d\n", vars->foundGedcTag);
                    //check the correct line number
                    if(strcmp(token[0], "1") != 0){
                        freeStringArray(token, tokenSize);
                        return initError(INV_HEADER, x+1);
                    }//end if
                }//end if
            }//end if

            //check the important data in GEDCOM
            if(tokenSize >= 2){
                //found vers inside gedc
                if(vars->foundGedcTag == 1 && strcmp(token[1], "VERS") == 0){
                    otherField = false;
                    vars->foundGedcVersionTag++;
                    if(DEBUG_UTILITIES)printf("foundGedcVersionTag: %d\n", vars->foundGedcVersionTag);
                    //check the correct line number
                    if(strcmp(token[0], "2") != 0 || tokenSize != 3){
                        freeStringArray(token, tokenSize);
                        return initError(INV_HEADER, x+1);
                    }//end if
                    //set the gedc version
                    vars->object->header->gedcVersion = atof(token[2]);
                    if(DEBUG_UTILITIES)printf("gedcVersion: %f\n", vars->object->header->gedcVersion);
                }//end if
            }//end if

            //check the endcoding
            if(tokenSize >= 2){
                if(strcmp(token[1], "CHAR") == 0){
                    otherField = false;
                    vars->foundEncodingTag++;
                    if(DEBUG_UTILITIES)printf("foundEncodingTag: %d\n", vars->foundEncodingTag);
                    //check the correct line number
                    if(strcmp(token[0], "1") != 0 || tokenSize != 3){
                        freeStringArray(token, tokenSize);
                        return initError(INV_HEADER, x+1);
                    }//end if
                    //check the encoding
                    CharSet encoding = ASCII;
                    if(strcmp("ANSEL", token[2]) == 0){
                        encoding = ANSEL;
                    }else if(strcmp("UTF-8", token[2]) == 0 || strcmp("UTF8", token[2]) == 0){
                        encoding = UTF8;
                    }else if(strcmp("UNICODE", token[2]) == 0){
                        encoding = UNICODE;
                    }//end if
                    //set the encoding
                    vars->object->header->encoding = encoding;
                    if(DEBUG_UTILITIES){
                        static char* encodingType[] = {
                            "ANSEL", "UTF-8", "UNICODE", "ASCII"
                        };//end array
                        printf("encoding: %s\n", encodingType[vars->object->header->encoding]);
                    }//end debug
                }//end if
            }//end if

            //check the reference for the submitter exist
            if(tokenSize >= 2){
                if(strcmp(token[1], "SUBM") == 0){
                    otherField = false;
                    vars->foundSubmitterTag++;
                    if(DEBUG_UTILITIES)printf("foundSubmitterTag: %d\n", vars->foundSubmitterTag);
                    //check the correct line number
                    if(strcmp(token[0], "1") != 0 /*|| tokenSize != 3*/){
                        freeStringArray(token, tokenSize);
                        return initError(INV_HEADER, x+1);
                    }//end if
                    //check if it is reference proper grammar
                    if(tokenSize == 3){
                        if(checkIfReference(token[2]) == false){
                            freeStringArray(token, tokenSize);
                            return initError(INV_HEADER, x+1);
                        }//end if
                    }//end if
                    //add submitter to the struct
                    getSubmitter(token[2], vars);
                }//end if
            }//end if

            //for the other field
            if(tokenSize > 1 && otherField == true){
                if(checkStringIfCaps(token[1])){
                    //set the other field
                    Field* tempField = initField();
                    tempField->tag = setString(token[1]);
                    if(tokenSize > 2){
                        tempField->value = combinedToken(token, tokenSize, 2);
                    }else{
                        tempField->value = setString("");
                    }//end if
                    insertBack(&vars->object->header->otherFields, (void*)tempField);
                    if(DEBUG_UTILITIES)printf("%s", printField(tempField));
                }//end if
            }//end if
        }//end if
        freeStringArray(token, tokenSize);
    }//end for

    //return
    if(vars->foundSourceTag != 1 || vars->foundGedcVersionTag != 1 || vars->foundEncodingTag != 1 || vars->foundSubmitterTag != 1){
        return initError(INV_HEADER, lastLine);
    }else{
        return initError(OK, -1);
    }//end if
}//end func*/

GEDCOMerror checkRecord(HelperVars* vars){//no leak
    if(DEBUG_UTILITIES)printf("\n*****Invoking checkRecord function*****\n");
    //dec vars
    GEDCOMerror error = initError(OK, -1);
    if(DEBUG_UTILITIES)printf("endHeaderLineLocation: %d\n", vars->endHeaderLineLocation-1);

    //check for the line
    for(int x=vars->endHeaderLineLocation-1; x<vars->lineSize; x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
        //check if line valid
        if(checkIfValidLine(vars->lineString[x]) == false){
            freeStringArray(token, tokenSize);
            return initError(INV_RECORD, x+1);
        }//end if
        freeStringArray(token, tokenSize);
    }//end for

    //check if the is an error in record
    //IMPORTANT this loop may be using at the start of endHeader location!!!***
    for(int x=vars->endHeaderLineLocation-1; x<vars->lineSize; x++){
        //dec needed vars
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
        printToken(token, tokenSize);

        //make sure that the token is greater 1
        if(tokenSize > 1){
            //check the grammar for the record
            if(checkIfStartRecord(vars->lineString[x]) == true){

                //check if it is an individual
                if(strcmp(token[0], "0") == 0 && strcmp(token[2], "INDI") == 0){
                    vars->foundStartIndividual++;
                    if(DEBUG_UTILITIES)printf("foundStartIndividual: %d\n", vars->foundStartIndividual);
                    //error check the ref
                    if(checkIfReference(token[1]) == false || tokenSize != 3){
                        freeStringArray(token, tokenSize);
                        return initError(INV_RECORD, x+1);
                    }//end if
                    //set the ref
                    insertSorted(&vars->indiRef, initIndiRef(token[1], NULL));
                }//end if

                //check if it is a family
                if(strcmp(token[0], "0") == 0 && strcmp(token[2], "FAM") == 0){
                    vars->foundStartFamily++;
                    if(DEBUG_UTILITIES)printf("foundStartFamily: %d\n", vars->foundStartFamily);
                    //error hec the ref
                    if(checkIfReference(token[1]) == false || tokenSize != 3){
                        freeStringArray(token, tokenSize);
                        return initError(INV_RECORD, x+1);
                    }//end if

                    insertSorted(&vars->famRef, initFamRef(token[1], NULL));
                }//end if

            }else if(checkIfValidLine(vars->lineString[x]) != true){
                //error if there is no number and less then 2 token
                freeStringArray(token, tokenSize);
                return initError(INV_RECORD, x+1);
            }else if(strcmp(token[0], "0") == 0 && strcmp(token[1], "TRLR") == 0){
                //stop loop when the program end
                freeStringArray(token, tokenSize);
                if(DEBUG_UTILITIES)printf("End of line from checkRecord\n");
                break;
            }//end if
        }//end if
        freeStringArray(token, tokenSize);
    }//end for

    //get the indi
    if(DEBUG_UTILITIES)printf("\n*****Invoking get function*****\n");
    for(int x=vars->endHeaderLineLocation-1; x<vars->lineSize; x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
        if(tokenSize == 3){
            if(strcmp(token[0], "0") == 0 && strcmp(token[2], "INDI") == 0){
                if(DEBUG_PARSER)printf("FOUND INDI: %s\n", token[1]);
                error = getIndividual(token[1], vars);
                if(error.type != OK){
                    freeStringArray(token, tokenSize);
                    return error;
                }//end if
            }//end if
        }//end if
        freeStringArray(token, tokenSize);
    }//end if

    //get the fam
    for(int x=vars->endHeaderLineLocation-1; x<vars->lineSize; x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
        if(tokenSize == 3){
            if(strcmp(token[0], "0") == 0 && strcmp(token[2], "FAM") == 0){
                //if(DEBUG_PARSER)printf("FOUND FAM: %s\n", token[1]);
                error = getFamily(token[1], vars);
                if(error.type != OK){
                    freeStringArray(token, tokenSize);
                    return error;
                }//end if
            }//end if
        }//end if
        freeStringArray(token, tokenSize);
    }//end if

    //return
    return error;
}//end func

// parser functions
GEDCOMerror getSubmitter(char* reference, HelperVars* vars){//worked, called sub free, no leak
    //dec vars
    Submitter* tempSubmitter = initSubmitter();
    int foundAddress = 0;
    int foundName = 0;
    bool foundStart = false;

    //loop through and get the submitter data
    for(int x=0; x<vars->lineSize; x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " /", &tokenSize);

        //check where the start of the reference
        if(tokenSize > 1 && strcmp(token[1], reference) == 0){
            if(strcmp(token[0], "0") == 0 && strcmp(token[1], reference) == 0){
                foundStart = true;
                if(tokenSize != 3){
                    deleteSubmitter(tempSubmitter);
                    freeStringArray(token, tokenSize);
                    return initError(INV_RECORD, x+1);
                }//end if
            }//end if
        }else if(strcmp(token[0], "0") == 0 && foundStart == true){
            //set to the object and break
            vars->object->header->submitter = tempSubmitter;
            vars->object->submitter = tempSubmitter;
            if(DEBUG_UTILITIES)printf("%s", printSubmitter(tempSubmitter));
            //name is a must in submitter
            if(foundName != 1){
                deleteSubmitter(tempSubmitter);
                freeStringArray(token, tokenSize);
                return initError(INV_RECORD, x+1);
            }//end
            freeStringArray(token, tokenSize);
            break;
        }//end if

        //check the body of the record
        if(foundStart == true){
            if(strcmp(token[0], "1") == 0 && strcmp(token[1], "NAME") == 0){
                //get the name
                foundName++;
                if(tokenSize <= 2){
                    deleteSubmitter(tempSubmitter);
                    freeStringArray(token, tokenSize);
                    return initError(INV_RECORD, x+1);
                }//end if
                char* name = combinedToken(token, tokenSize, 2);
                strcpy(tempSubmitter->submitterName, name);
                free(name);
                if(DEBUG_UTILITIES)printf("!!!!! submitterName: %s = %s\n", name, tempSubmitter->submitterName);
            }else if(strcmp(token[0], "1") == 0 && strcmp(token[1], "ADDR") == 0){
                //get the address
                foundAddress++;
                char* address = combinedToken(token, tokenSize, 2);
                if(address != NULL){
                    strcpy(tempSubmitter->address, address);
                }//end if
                free(address);
            }else if(strcmp(token[0], "0") != 0){
                //get the other field
                Field* otherField = initField();
                otherField->tag = setString(token[1]);
                //get the value for the field
                if(tokenSize > 2){
                    otherField->value = combinedToken(token, tokenSize, 2);
                }else{
                    otherField->value = setString("");
                }//end if
                insertSorted(&tempSubmitter->otherFields, otherField);
            }//end if
        }//end if
        freeStringArray(token, tokenSize);
    }//end for

    return initError(OK, -1);
}//end func

GEDCOMerror getIndividual(char* reference, HelperVars* vars){//need some work
    //dec vars
    Individual* tempIndividual = initIndividual();
    Event* tempEvent = NULL;
    GEDCOMerror error = initError(OK, -1);
    char pointerFAMS[256] = {"\0"};
    char pointerFAMC[256] = {"\0"};
    char pointerIndi[256] = {"\0"};
    bool foundFAMS = false;
    bool foundFAMC = false;
    //bool male = false;
    bool foundEvent = false;
    bool foundStart = false;
    int eventSize = 23;
    static char* eventTypes[] = {"BIRT", "CHR", "DEAT", "BURI", "CREM", "ADOP",
                               "BAPM", "BARM", "BASM", "BLES", "CHRA", "CONF", "FCOM", "ORDN",
                               "NATU", "EMIG", "IMMI", "CENS", "PROB", "WILL", "GRAD", "RETI", "EVEN"};

    //loop and get the data of the indi
    for(int x=0; x<vars->lineSize; x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
        printToken(token, tokenSize);

        //start and the end of record
        if(tokenSize > 1){
             //at the end
            if(strcmp(token[0], "0") == 0 && foundStart == true){
                //set the main to the indi struct
                IndividualReference* tempSearch = initIndiRef(pointerIndi, NULL);
                IndividualReference* indiLocation = findElement(vars->indiRef, findIndiRef, tempSearch);
                if(indiLocation != NULL){
                    if(foundFAMS == true){
                        indiLocation->foundFAMS = true;
                    }//end if
                    if(foundFAMC == true){
                        indiLocation->foundFAMC = true;
                    }//end if
                    indiLocation->individual = tempIndividual;
                    insertSorted(&vars->object->individuals, tempIndividual);
                }//end if
                deleteIndiRef(tempSearch);
                freeStringArray(token, tokenSize);
                break;
            }//end i

            //start
            if(strcmp(token[0], "0") == 0 && strcmp(token[1], reference) == 0){
               foundStart = true;
               strcpy(pointerIndi, token[1]);
            }//end if
        }//end if

        //check the body of the record
        if(foundStart == true && tokenSize > 1){
            //find the name
            if(strcmp(token[0], "1") == 0 && strcmp(token[1], "NAME") == 0){
                if(tokenSize > 2){
                    //get the string of the first name
                    char* nameValue = combinedToken(token, tokenSize, 2);
                    if(nameValue != NULL){
                        //split the string to get the first and last name
                        int nameTokenSize = 0;
                        char** nameToken = split(nameValue, "/", &nameTokenSize);
                        if(nameTokenSize < 2){
                            if(token[2][0] == '/'){
                                removeSpace(nameToken[0]);
                                tempIndividual->surname = setString(nameToken[0]);
                                tempIndividual->givenName = setString("");
                            }else if(nameTokenSize == 1){
                                removeSpace(nameToken[0]);
                                tempIndividual->surname = setString("");
                                tempIndividual->givenName = setString(nameToken[0]);
                            }//end if
                        }else if(nameTokenSize > 1){
                            removeSpace(nameToken[0]);
                            removeSpace(nameToken[1]);
                            tempIndividual->givenName = setString(nameToken[0]);
                            tempIndividual->surname = setString(nameToken[1]);
                        }//end if
                        freeStringArray(nameToken, nameTokenSize);
                    }//end if
                    free(nameValue);
                }//end if
            }else if(strcmp(token[0], "1") == 0 && strcmp(token[1], "SEX") == 0){
                //get the sex
                if(tokenSize > 2){
                    if(strcmp(token[2], "M")){
                        //male = true;
                    }//end if
                    if(tokenSize != 3){
                        error = initError(INV_RECORD, x+1);
                        freeStringArray(token, tokenSize);
                        deleteIndividual(tempIndividual);
                        deleteEvent(tempEvent);
                        break;
                    }//end if
                    Field* otherField = initField();
                    otherField->tag = setString(token[1]);
                    otherField->value = setString(token[2]);
                    insertSorted(&tempIndividual->otherFields, otherField);
                }else{
                    if(DEBUG_UTILITIES)printf("FOUND NO SEX VALUE!!!!!");
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteIndividual(tempIndividual);
                    deleteEvent(tempEvent);
                    break;
                }//end if
            }else if(strcmp(token[0], "1") == 0 && strcmp(token[1], "FAMS") == 0){
                //check the fam tags
                if(tokenSize != 3){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteIndividual(tempIndividual);
                    deleteEvent(tempEvent);
                    break;
                }//end if
                if(checkIfReference(token[2]) != true){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteIndividual(tempIndividual);
                    deleteEvent(tempEvent);
                    break;
                }//end if
                foundFAMS = true;
                strcpy(pointerFAMS, token[2]);
            }else if(strcmp(token[0], "1") == 0 && strcmp(token[1], "FAMC") == 0){
                //check the fam tags
                if(tokenSize != 3){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteIndividual(tempIndividual);
                    deleteEvent(tempEvent);
                    break;
                }//end if
                if(checkIfReference(token[2]) != true){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteIndividual(tempIndividual);
                    deleteEvent(tempEvent);
                    break;
                }//end if
                foundFAMC = true;
                strcpy(pointerFAMC, token[2]);
            }else if(strcmp(token[0], "1") == 0 && searchStringArray(eventTypes, token[1], eventSize, "FOUND") > 0){
                //with temp indi assign the event
                foundEvent = true;
                tempEvent = initEvent();
                error = getEvent(tempEvent, tempIndividual, x, "Individual", vars);
                if(error.type != OK){
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempIndividual);
                    deleteEvent(tempEvent);
                    return error;
                }//end for
                strcpy(tempEvent->type, token[1]);
            }else if(strcmp(token[0], "0") != 0 && foundEvent == false && !(strcmp(token[1], "SURN") == 0 || strcmp(token[1], "GIVN") == 0)){
                Field* otherField = initField();
                otherField->tag = setString(token[1]);
                //get the value for the field
                if(tokenSize > 2){
                    otherField->value = combinedToken(token, tokenSize, 2);
                }else{
                    otherField->value = setString("");
                }//end if
                insertSorted(&tempIndividual->otherFields, otherField);
            }else if( (strcmp(token[0], "0") == 0 || strcmp(token[0], "1") == 0 ) && foundEvent == true){
                foundEvent = false;
            }//end if

        }//end if
        freeStringArray(token, tokenSize);
    }//end for
    return error;
}//end func

GEDCOMerror getFamily(char* reference, HelperVars* vars){//need some work
    #define DEBUG_FAMILY false
    if(DEBUG_FAMILY)printf("\n*****Invoking checkFamily function*****\n");
    GEDCOMerror error = initError(OK, -1);
    Event* tempEvent = NULL;
    Family* tempFamily = initFamily();
    //dec vars
    char pointerFam[256] = {"\0"};
    char pointerHusband[256] = {"\0"};
    char pointerWife[265] = {"\0"};
    bool foundHusband = false;
    bool foundWife = false;
    bool foundEvent = false;
    bool foundStart = false;
    int foundChild = 0;
    int eventSize = 12;
    static char* eventTypes[] = {"ANUL", "CENS", "DIV", "DIVF", "ENGA", "MARB", "MARC",
                        "MARR", "MARL", "MARS", "RESI", "EVEN"};

    for(int x=0; x<vars->lineSize; x++){
        //dec vars
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);

        //start of the records
        if(tokenSize > 1){
            //at the end of the record
            if(strcmp(token[0], "0") == 0 && foundStart == true){
                FamilyReference* tempSearch = initFamRef(pointerFam, NULL);
                FamilyReference* famLocation = findElement(vars->famRef, findFamRef, tempSearch);
                if(DEBUG_UTILITIES)printf("end of family: \t\t\t\t%s %d\n", pointerFam, vars->counter);
                famLocation->family = tempFamily;
                insertSorted(&vars->object->families, tempFamily);
                //if(DEBUG_UTILITIES)printf("\nfamLocation: %s\n\n",  printFamily( tempFamily));
                freeStringArray(token, tokenSize);
                deleteFamRef(tempSearch);
                break;
            }//end if

            //check where the start of the reference
            if(strcmp(token[0], "0") == 0 && strcmp(token[1], reference) == 0){
                foundStart = true;
                strcpy(pointerFam, token[1]);
                vars->counter++;
                if(DEBUG_UTILITIES)printf("initFamily: %d\n", vars->counter);
            }//end if
        }//end if

        //body of the record
        if(foundStart == true && tokenSize > 1){
            if(strcmp(token[0], "1") == 0 && strcmp(token[1], "HUSB") == 0){
                //check husband
                vars->foundHusband++;
                //foundHusband = true;
                strcpy(pointerHusband, token[2]);
                if(DEBUG_FAMILY)printf("foundHusband: %d\n", vars->foundHusband);
                //error checked
                if(tokenSize != 3){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempFamily);
                    deleteEvent(tempEvent);
                    break;
                }//end
                if(checkIfReference(token[2]) != true){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempFamily);
                    deleteEvent(tempEvent);
                    break;
                }//end if

                // link the husband
                IndividualReference* tempSearch = initIndiRef(token[2], NULL);
                IndividualReference* indiLocation = findElement(vars->indiRef, findIndiRef, tempSearch);
                if(indiLocation != NULL){
                    ///if(indiLocation->foundFAMS == true && indiLocation->foundFAMC == false /*&& indiLocation->found == false*/){
                        if(DEBUG_UTILITIES)printf("HUSBAND NAME:\t%s %s\t\t%s\t%d\n", indiLocation->individual->givenName, indiLocation->individual->surname, token[2], vars->counter);
                        foundHusband = true;
                        indiLocation->found = true;
                        indiLocation->foundFAMS = true;
                        tempFamily->husband = indiLocation->individual;
                        insertSorted(&indiLocation->individual->families, tempFamily);
                    //}//end if
                }//end if
                deleteIndiRef(tempSearch);

            }else if(strcmp(token[0], "1") == 0 && strcmp(token[1], "WIFE") == 0){
                //check wife
                vars->foundWife++;
                //foundWife = true;
                strcpy(pointerWife, token[2]);
                //error check
                if(DEBUG_FAMILY)printf("foundWife: %d\n", vars->foundWife);
                if(tokenSize != 3){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempFamily);
                    deleteEvent(tempEvent);
                    break;
                }//end if
                if(checkIfReference(token[2]) != true){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempFamily);
                    deleteEvent(tempEvent);
                    break;
                }//end if

                // link the wife
                IndividualReference* tempSearch = initIndiRef(token[2], NULL);
                IndividualReference* indiLocation = findElement(vars->indiRef, findIndiRef, tempSearch);
                if(indiLocation != NULL){
                    //if(indiLocation->foundFAMS == true && indiLocation->foundFAMC == false /*&& indiLocation->found == false*/){
                        if(DEBUG_UTILITIES)printf("WIFE NAME:\t%s %s\t\t%s\t%d\n", indiLocation->individual->givenName, indiLocation->individual->surname,token[2], vars->counter);
                        foundWife = true;
                        indiLocation->found = true;
                        indiLocation->foundFAMS = true;
                        tempFamily->wife = indiLocation->individual;
                        insertSorted(&indiLocation->individual->families, tempFamily);
                    //}//end if
                }//end if
                deleteIndiRef(tempSearch);

            }else if(strcmp(token[0], "1") == 0 && strcmp(token[1], "CHIL") == 0){
                //check chil
                vars->foundChild++;
                foundChild++;
                //error check
                if(DEBUG_FAMILY)printf("foundChild: %d\n", vars->foundChild);
                if(tokenSize != 3){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempFamily);
                    deleteEvent(tempEvent);
                    break;
                }//end if
                if(checkIfReference(token[2]) != true){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempFamily);
                    deleteEvent(tempEvent);
                    break;
                }//end if

                //line the child
                IndividualReference* tempSearch = initIndiRef(token[2], NULL);
                IndividualReference* indiLocation = findElement(vars->indiRef, findIndiRef, tempSearch);
                if(indiLocation != NULL){

                            vars->foundGarbageFamily++;
                            Family* famCopy = initFamily(printFamily, dummyDelete, compareFamilies);

                            famCopy->events = tempFamily->events;
                            famCopy->otherFields = tempFamily->otherFields;
                            if(foundHusband == true){
                                famCopy->husband = tempFamily->husband;
                            }//end if
                            if(foundWife == true){
                                famCopy->wife = tempFamily->wife;
                            }//end if

                            insertSorted(&LIST_OF_FAMILY_GARBAGE, famCopy);
                            insertSorted(&indiLocation->individual->families, famCopy);

                }//end if
                deleteIndiRef(tempSearch);
                insertSorted(&tempFamily->children, indiLocation->individual);

            }else if(strcmp(token[0], "1") == 0 && searchStringArray(eventTypes, token[1], eventSize, "FOUND") > 0){
                //with temp indi assign the event
                foundEvent = true;
                tempEvent = initEvent();
                error = getEvent(tempEvent, tempFamily, x, "Family", vars);
                if(error.type != OK){
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempFamily);
                    deleteEvent(tempEvent);
                    return error;
                }//end for
                strcpy(tempEvent->type, token[1]);
            }else if(strcmp(token[0], "0") != 0 && foundEvent == false){
                Field* otherField = initField();
                otherField->tag = setString(token[1]);
                //get the value for the field
                if(tokenSize > 2){
                    otherField->value = combinedToken(token, tokenSize, 2);
                }else{
                    otherField->value = setString("");
                }//end if
                insertSorted(&tempFamily->otherFields, otherField);
            }else if( (strcmp(token[0], "0") == 0 || strcmp(token[0], "1") == 0 ) && foundEvent == true){
                foundEvent = false;
            }//end if
        }//end if

        freeStringArray(token, tokenSize);
    }//end for

    //return
    return error;
}//end func

GEDCOMerror getEvent(Event* tempEvent, void* object, int lineStartNumber, char* type, HelperVars* vars){
    //dec vars
    GEDCOMerror error = initError(OK, -1);
    bool foundDate = false;
    bool foundPlace = false;
    bool foundEvent = true;
    bool foundEventBody = false;
    Family* tempFamily = NULL;
    Individual* tempIndividual = NULL;

    //cast based on type
    if(compareString(type, "Family") == 0){
        tempFamily = (Family*)object;
    }else{
        tempIndividual = (Individual*)object;
    }//end if

    for(int x=lineStartNumber; x<vars->lineSize; x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);

        //when found event
        if(tokenSize > 1){
            //when found event
            if(DEBUG_UTILITIES)printf("found event\n");
            //check if there is the data for event
            if(strcmp(token[0], "2") == 0 && tokenSize > 2){
                foundEventBody = true;
                if(strcmp(token[1], "DATE") == 0){
                    foundDate = true;
                    tempEvent->date = combinedToken(token, tokenSize, 2);
                }else if(strcmp(token[1], "PLAC") == 0){
                    foundPlace = true;
                    tempEvent->place = combinedToken(token, tokenSize, 2);
                }else{
                    Field* tempField = initField();
                    tempField->tag = setString(token[1]);
                    char* tempValue = combinedToken(token, tokenSize, 2);
                    if(tempValue == NULL){
                        tempField->value = setString("");
                        free(tempValue);
                    }else{
                        tempField->value = tempValue;
                    }//end if
                    insertSorted(&tempEvent->otherFields, tempField);
                }//end if
            }else if(foundEvent == true && foundEventBody == true && (strcmp(token[0], "0") == 0 || strcmp(token[0], "1") == 0 )){
                //end of event
                if(foundPlace == false){
                    tempEvent->place = setString("");
                }else if(foundDate == false){
                    tempEvent->date = setString("");
                }//end if
                foundDate = false;
                foundPlace = false;
                foundEvent = false;
                foundEventBody = false;
                //cast based on type
                if(compareString(type, "Family") == 0){
                    insertSorted(&tempFamily->events, tempEvent);
                }else{
                    insertSorted(&tempIndividual->events, tempEvent);
                }//end if
                //at the end of event
                freeStringArray(token, tokenSize);
                break;
            }//end if
        }//end if*/

        freeStringArray(token, tokenSize);
    }//end for
    return error;
}//end func

char* getNameOfIndividual(Individual* individual){
    static char string[256];
    sprintf(string, "%s %s", individual->givenName, individual->surname);
    return string;
}//end func

void setErrorCode(GEDCOMerror* object, ErrorCode errorCode, int line){
    object->type = errorCode;
    object->line = line;
}//end func

/***********************************************************/

void printToken(char** token, int tokenSize){
    for(int x=0; x<tokenSize; x++){
        if(DEBUG_UTILITIES)printf("\tToken %d: %s\n", x, token[x]);
    }//end if
}//end func

bool checkStringIfNumbers(char* string){
    //make sure it doesnt start at 0
    if(strlen(string) > 1 && string[0] == '0'){
        return false;
    }//end if

    //loop through each char and check if its number
    for(int x=0; x<strlen(string); x++){
        if(!NUMBER_RANGE(string[x])){
            return false;
        }//end if
    }//end for
    return true;
}//end func

bool checkStringIfCaps(char* string){
    //loop through each char and check if its upper case
    for(int x=0; x<strlen(string); x++){
        if(!UPPERCASE_RANGE(string[x])){
            return false;
        }//end if
    }//end for
    return true;
}//end func

bool checkIfReference(char* string){
    #define DEBUG_FUNC false
    //chec if the @ at the start
    if(string[0] != '@' || string[strlen(string)-1] != '@'){
        return false;
    }//end if
    if(DEBUG_FUNC)printf("Ref1: %c, Ref2: %c\n", string[0], string[strlen(string)-1]);

    //check if inside @ is not alpha or number
    if(DEBUG_FUNC)printf("Inside the ref: ");
    for(int x=1; x<strlen(string)-1; x++){
        if(DEBUG_FUNC)printf("%c", string[x]);
        if( !(ALPHABET_RANGE(string[x]) || NUMBER_RANGE(string[x])) ){
            return false;
        }//end if
    }//end for
    if(DEBUG_FUNC)printf("\n");

    //check pass
    return true;
}//end func

bool checkIfValidLine(char* stringLine){
    //dec vars
    int tokenSize = 0;
    char** token = split(stringLine, " ", &tokenSize);
    bool validTag = false;

    //chec for line length
    if(strlen(stringLine) > 200){
        return false;
    }//end if

    //check if token is greater than 2
    if(tokenSize < 2){
        freeStringArray(token, tokenSize);
        return false;
    }//end if

    //check if it is a number and a valid tag
    if(checkStringIfNumbers(token[0]) == true /*&& checkStringIfCaps(token[1]) == true*/){
        validTag = true;
    }//end if

    //check if the tag is valid
    if(validTag == false){
        freeStringArray(token, tokenSize);
        return false;
    }//end if

    //valid line
    freeStringArray(token, tokenSize);
    return true;
}//end func

bool checkIfStartRecord(char* stringLine){
    //dec vars
    int tokenSize = 0;
    char** token = split(stringLine, " ", &tokenSize);

    //check if it is exactly 3 token
    if(tokenSize != 3){
        freeStringArray(token, tokenSize);
        return false;
    }//end if

    //check the if is record format
    if(strcmp(token[0], "0") != 0){
        freeStringArray(token, tokenSize);
        return false;
    }//end if

    //free and return
    freeStringArray(token, tokenSize);
    return true;
}//end fund

bool checkIfLastName(char* string){
    //check if it is the right format for the last name
    if(strlen(string) < 2){
        return false;
    }//end if

    if(string[0] != '/' || string[strlen(string)-1] != '/'){
        return false;
    }//end if

    //return
    return true;
}//end func

GEDCOMerror checkIfLineNumbers(HelperVars* vars){
    //loop until pass or found error
    for(int x=0; x < vars->lineSize-1; x++){
        if(checkIfValidLine(vars->lineString[x]) == false){
            return initError(INV_RECORD, x+1);
        }//end if

        //dec vars
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
        int postTokenSize = 0;
        char** postToken = split(vars->lineString[x+1], " ", &postTokenSize);

        //make sure that line number exist before converting
        if(checkStringIfNumbers(token[0]) == false){
            freeStringArray(token, tokenSize);
            freeStringArray(postToken, postTokenSize);
            return initError(INV_RECORD, x+1);
        }//end if

        //convert to number
        int lineNum = atoi(token[0]);
        int postLineNum = atoi(postToken[0]);
        int lineDistance = postLineNum - lineNum;

        //check if line numbers is correct
        if(lineDistance > 1){
            freeStringArray(token, tokenSize);
            freeStringArray(postToken, postTokenSize);
            return initError(INV_RECORD, x+2);
        }//end if

        //free
        freeStringArray(token, tokenSize);
        freeStringArray(postToken, postTokenSize);
    }//end for
    return initError(OK, -1);
}//end func



List combinedIndividualList(List* list1, List* list2){
    //dec vars
    List combinedList = initializeList(printDescendants, dummyDelete, compareIndividuals);
    ListIterator iterList1 = createIterator(*list1);
    ListIterator iterList2 = createIterator(*list2);
    //loop through the firsst list
    for(int x = 0; x < getLength(*list1); x++){
        Individual* tempIndi = nextElement(&iterList1);
        insertSorted(&combinedList, tempIndi);
    }//end for
    //loop through the second list
    for(int x = 0; x < getLength(*list2); x++){
        Individual* tempIndi = nextElement(&iterList2);
        insertSorted(&combinedList, tempIndi);
    }//end for
    clearList(list1);
    clearList(list2);
    return combinedList;
}//end func

List getChildren(Individual* person){
    if(DEBUG_UTILITIES)printf("calling get children\n");
    //dec vars
    List descendantsList = initializeList(printDescendants, dummyDelete, compareIndividuals);
    //check if its empty
    if(person != NULL){
        ListIterator iterateFam = createIterator(person->families);
        if(DEBUG_UTILITIES)printf("getLength: %d\n", getLength(person->families));
        for(int x = 0; x < getLength(person->families); x++){
            Family* family = nextElement(&iterateFam);
            if(family == NULL){
                continue;
            }//end if
            char* printFam = printFamily(family);
            if(DEBUG_UTILITIES)printf("printFam: %s\n", printFam);
            free(printFam);

            //get the each child
            ListIterator iterateChild = createIterator(family->children);
            for(int i = 0; i < getLength(family->children); i++){
                if(DEBUG_UTILITIES)printf("in children loop: %d\n", i);
                Individual* individual = nextElement(&iterateChild);
                if(individual == NULL){
                    break;
                }//end if

                //create a break if the search person is one of the same
                if(person == individual){
                    break;
                }//end if

                insertSorted(&descendantsList, individual);
                List tempChildList = getChildren(individual);
                descendantsList = combinedIndividualList(&descendantsList, &tempChildList);
            }//end for
        }//end for
    }else{
        if(DEBUG_UTILITIES)printf("person is null\n");
    }//end if
    //return
    if(DEBUG_UTILITIES)printf("return the list of children\n");
    return descendantsList;
}//end funcs

void dummyDelete(void* toBeDeleted){
    //does nothing
}//end func

int dummyCompare(const void* first,const void* second){
    return 0;
}//end func

char* dummyPrint(void* toBePrinted){
    return setString("");
}//end func

void freeObject(void* toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }//end if
    //check if null
    free(toBeDeleted);
}//end func


bool comparePerson(const void* first, const void* second){
    //declare vars
    Individual* person1 = (Individual*)first;
    Individual* person2 = (Individual*)second;
    if(compareIndividuals(person1, person2) == 0){
        return true;
    }else{
        return false;
    }//end if
}//end func

char* printSubmitter(void* toBePrinted){
    if(toBePrinted == NULL){
        return setString("");
    }//end if

    //print the object
    Submitter* object = (Submitter*)toBePrinted;
    char* stringField = toString(object->otherFields);
    int stringSize = 99 + 256 + strlen(object->submitterName) + strlen(stringField);
    char* stringToPrint = calloc(1, sizeof(char)*stringSize);
    //assign the print
    strcat(stringToPrint, "submiter name: |");
    strcat(stringToPrint, object->submitterName);
    strcat(stringToPrint, "|\nOther fields:\n");
    strcat(stringToPrint, stringField);
    strcat(stringToPrint, "Address: |");
    strcat(stringToPrint, object->address);
    strcat(stringToPrint, "|\n\0");
    //free
    free(stringField);
    return stringToPrint;
}//end func

char* printIndividualName(void* toBePrinted){//checked
    //check if null
    if(toBePrinted == NULL){
        return setString("");
    }//end if

    Individual* object = (Individual*)toBePrinted;
    char* eventString = toString(object->events);
    char* famString = toString(object->families);
    char* fieldString = toString(object->otherFields);
    int mallocSize = 99 + strlen(eventString) + strlen(famString) + strlen(fieldString);
    char* stringToPrint = calloc(1, sizeof(char)* mallocSize );
    //make sure you dont combine the
    strcat(stringToPrint, "Name: |");
    if(object->givenName != NULL){
        strcat(stringToPrint, object->givenName);
        strcat(stringToPrint, "| |");
    }//end if
    if(object->surname != NULL){
        strcat(stringToPrint, object->surname);
    }//end if

    strcat(stringToPrint, "|\n\0");

    /*free*/
    free(eventString);
    free(famString);
    free(fieldString);
    return stringToPrint;
}//end func

void deleteIndiRef(void* toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }//end if

    //free mem
    IndividualReference* object = (IndividualReference*)toBeDeleted;
    //clearList(&object->individual->families);
    free(object->reference);
    clearList(&object->famcRef);
    clearList(&object->famsRef);
    free(object);
}//end func

void deleteFamRef(void* toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }//end if

    //free mem
    FamilyReference* object = (FamilyReference*)toBeDeleted;
    free(object->reference);
    if(object->husbRef != NULL){
        free(object->husbRef);
    }//end if
    if(object->wifeRef != NULL){
        free(object->wifeRef);
    }//end if
    clearList(&object->childRef);
    free(object);
}//end func

void deleteGarbageFamily(void* toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }//end if
    Family* object = (Family*)toBeDeleted;
    //clearList(&object->events);
    //clearList(&object->otherFields);
    free(object);
}//end func

char* printReference(void* toBePrinted){
    //just a dummy for now
    return setString("printReference has not been setup\n");
}//end func

int compareIndiRef(const void* first,const void* second){//checked
    //check if its null
    if(first == NULL || second == NULL){
        return 0;
    }//end if

    //dec vars
    IndividualReference* object1 = (IndividualReference*)first;
    IndividualReference* object2 = (IndividualReference*)second;
    char individual1[256];
    char individual2[256];
    strcpy(individual1, object1->reference);
    strcpy(individual2, object2->reference);
    return strcmp(individual1, individual2);
}//end func

int compareFamRef(const void* first,const void* second){//checked
    //check if its null
    if(first == NULL || second == NULL){
        return 0;
    }//end if

    //dec vars
    FamilyReference* object1 = (FamilyReference*)first;
    FamilyReference* object2 = (FamilyReference*)second;
    char family1[256];
    char family2[256];
    strcpy(family1, object1->reference);
    strcpy(family2, object2->reference);
    return strcmp(family1, family2);
}//end func

bool findIndiRef(const void* first, const void* second){
    //declare vars
    IndividualReference* person1 = (IndividualReference*)first;
    IndividualReference* person2 = (IndividualReference*)second;
    if(strcmp(person1->reference, person2->reference) == 0){
        //if(DEBUG_UTILITIES)printf("FOUND INDI REF\n");
        return true;
    }else{
        return false;
    }//end if
}//end func

bool findFamRef(const void* first, const void* second){
    //declare vars
    FamilyReference* fam1 = (FamilyReference*)first;
    FamilyReference* fam2 = (FamilyReference*)second;
    if(strcmp(fam1->reference, fam2->reference) == 0){
        //if(DEBUG_UTILITIES)printf("FOUND FAM REF\n");
        return true;
    }else{
        return false;
    }//end if
}//end func

void deleteSubmitter(Submitter* toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }//end if
    //free the field
    clearList(&toBeDeleted->otherFields);
    free(toBeDeleted);
}//end func

void deleteHeader(Header* toBeDeleted){
    deleteSubmitter(toBeDeleted->submitter);
    clearList(&toBeDeleted->otherFields);
    free(toBeDeleted);
}//end func

char* printDescendants(void* toBePrinted){
    //check if null
    if(toBePrinted == NULL){
        return setString("");
    }//end if

    Individual* object = (Individual*)toBePrinted;
    char* eventString = toString(object->events);
    char* famString = toString(object->families);
    char* fieldString = toString(object->otherFields);
    int mallocSize = 999 + strlen(eventString) + strlen(famString) + strlen(fieldString);
    char* stringToPrint = calloc(1, sizeof(char)* mallocSize );
    //make sure you dont combine the
    strcat(stringToPrint, "Name: ");
    if(object->givenName != NULL){
        strcat(stringToPrint, object->givenName);
        strcat(stringToPrint, " ");
    }//end if
    if(object->surname != NULL){
        strcat(stringToPrint, object->surname);
    }//end if

    strcat(stringToPrint, "\n\0");

    /*free*/
    free(eventString);
    free(famString);
    free(fieldString);
    return stringToPrint;
}//end func

char* printEncoding(CharSet encoding){
    //array for tags and enums etc
    static char* charType[] = {
        "ANSEL", "UTF-8", "UNICODE", "ASCII"
    };//end array
    //print the error if there is error
    char* stringToPrint = calloc(1, sizeof(char)* 99);
    sprintf(stringToPrint, "%s", charType[encoding]);
    strcat(stringToPrint, "\0");
    //return the string
    return stringToPrint;
}//end func

void deleteFamilyGarbageWrapper(){
    clearList(&LIST_OF_FAMILY_GARBAGE);
}//end func

void writeFile(const char* fileName, char* string){
    FILE* filePointer = fopen(fileName, "w");
    //check if there is an error opening a file
    if(filePointer == NULL){
        debug("Error opening a file for writing\n");
    }//end if
    fprintf(filePointer, "%s", string);
    fclose(filePointer);
}//end func

char* getFileString(List* famList, List* indiList, const GEDCOMobject* object){
    //dec vars
    char* headerString = getFileHeader(object->header);
    char* submitterString = getFileSubmitter(object->submitter);
    char* individualString = getFileIndividual(indiList);
    char* familyString = getFileFamily(famList);
    char* string = calloc(1, sizeof(char)* (strlen(headerString)+strlen(submitterString)+strlen(individualString)+strlen(familyString)+256) );
    strcat(string, "0 HEAD\n");
    strcat(string, headerString);
    strcat(string, submitterString);
    strcat(string, individualString);
    strcat(string, familyString);
    strcat(string, "0 TRLR\n\0");
    //return
    free(headerString);
    free(submitterString);
    free(individualString);
    free(familyString);
    return string;
}//end func

char* getFileHeader(void* toBePrinted){
    if(toBePrinted == NULL){
        return setString("");
    }//end if

    //setup the mem and vars
    Header* object = (Header*)toBePrinted;
    char* string = calloc(1, sizeof(char)*999);
    char* encodingPointer = printEncoding(object->encoding);

    //set the string
    sprintf(string, "1 SOUR %s\n1 CHAR %s\n1 GEDC\n2 VERS %0.2lf\n2 FORM LINEAGE-LINKED\n",
        object->source, encodingPointer, object->gedcVersion);
    strcat(string, "1 SUBM @SUB1@\n");
    free(encodingPointer);
    return string;
}//end func

char* getFileSubmitter(void* toBePrinted){
    if(toBePrinted == NULL){
        return setString("");
    }//end if

    //print the object
    Submitter* object = (Submitter*)toBePrinted;
    int stringSize = 99 + 256 + 999 + strlen(object->submitterName);
    char* stringToPrint = calloc(1, sizeof(char)*stringSize);

    //name and address
    char* name = calloc(1, sizeof(char)* (strlen(object->submitterName)+99));
    char* address = calloc(1, sizeof(char)* (strlen(object->address)+99) );
    sprintf(name, "0 @SUB1@ SUBM\n1 NAME %s\n", object->submitterName);
    strcat(stringToPrint, name);
    if(strlen(object->address) > 1){
        sprintf(address, "1 ADDR %s\n", object->address);
        strcat(stringToPrint, address);
    }//end if

    //assign the print
    free(name);
    free(address);
    return stringToPrint;
}//end func

char* getFileIndividual(List* indiList){
    //dec vars
    ListIterator indiIterator = createIterator(*indiList);
    int memSize = 256*3*INDI_EVENT_SIZE*getLength(*indiList);
    char surname[265];
    char givenName[265];
    char refBuffer[265];
    char* stringToBeReturned = calloc(1, sizeof(char)*memSize);

    //for loop to to the whole indi list
    for(int x=0; x<getLength(*indiList); x++){
        //set the vars
        IndividualReference* tempIndi = nextElement(&indiIterator);
        memSize = memSize + (256*3) + (256*3 * getLength(tempIndi->individual->events) );
        char* stringBuffer = calloc(1, sizeof(char)*(memSize));

        //for the name tag
        sprintf(stringBuffer, "0 %s INDI\n1 NAME %s /%s/\n", tempIndi->reference, tempIndi->individual->givenName, tempIndi->individual->surname);

        //for givenname tag
        if(strlen(tempIndi->individual->givenName) > 1){
            sprintf(givenName, "2 GIVN %s\n", tempIndi->individual->givenName);
            strcat(stringBuffer, givenName);
        }//end if

        //for the surname tag
        if(strlen(tempIndi->individual->surname) > 1){
            sprintf(surname, "2 SURN %s\n", tempIndi->individual->surname);
            strcat(stringBuffer, surname);
        }//end if

        //for the indi events
        if(getLength(tempIndi->individual->events) != 0){
            char* eventBuffer = getFileEvent(&tempIndi->individual->events);
            strcat(stringBuffer, eventBuffer);
            free(eventBuffer);
        }//end if

        //for the init otherfield
        if(getLength(tempIndi->individual->otherFields) != 0){
            ListIterator fieldIter = createIterator(tempIndi->individual->otherFields);
            for(int y=0; y<getLength(tempIndi->individual->otherFields); y++){
                Field* currentField = nextElement(&fieldIter);
                //check if the tag has giv and sur and dont print it
                if( !(strcmp("GIVN", currentField->tag) == 0 || strcmp("SURN", currentField->tag) == 0) ){
                    memSize = memSize + 256;
                    stringBuffer = realloc(stringBuffer, sizeof(char)*memSize);
                    char fieldBuffer[256];
                    sprintf(fieldBuffer, "1 %s %s\n", currentField->tag, currentField->value);
                    strcat(stringBuffer, fieldBuffer);
                }//end if
            }//end if
        }//end if

        //for the fams ref
        ListIterator famsIterator = createIterator(tempIndi->famsRef);
        for(int y=0; y<getLength(tempIndi->famsRef) ;y++){
            char* famsString = nextElement(&famsIterator);
            sprintf(refBuffer, "1 FAMS %s\n", famsString);
            strcat(stringBuffer, refBuffer);
        }//end for

        //for the famc ref
        ListIterator famcIterator = createIterator(tempIndi->famcRef);
        for(int y=0; y<getLength(tempIndi->famcRef) ;y++){
            char* famcString = nextElement(&famcIterator);
            sprintf(refBuffer, "1 FAMC %s\n", famcString);
            strcat(stringBuffer, refBuffer);
        }//end for

        strcat(stringToBeReturned, stringBuffer);
        free(stringBuffer);
    }//end for

    return stringToBeReturned;
}//end func

char* getFileFamily(List* famList){
    //dec vars
    ListIterator famIterator = createIterator(*famList);
    int memSize = 256*3;
    char refBuffer[256];
    char* stringToBeReturned = calloc(1, sizeof(char)*( (memSize*FAM_EVENT_SIZE)*getLength(*famList)) );

    //loop through the whole fam list
    for(int x=0; x<getLength(*famList); x++){
        //set the vars
        FamilyReference* tempFam = nextElement(&famIterator);
        memSize = memSize + 256 + (256 * getLength(*famList) + getLength(tempFam->family->events));
        char* stringBuffer = calloc(1, sizeof(char)*memSize);

        //for the record fam tag
        sprintf(stringBuffer, "0 %s FAM\n", tempFam->reference);

        //for the husb tag
        if(tempFam->husbRef != NULL){
            sprintf(refBuffer, "1 HUSB %s\n", tempFam->husbRef);
            strcat(stringBuffer, refBuffer);
        }//end if

        //for the wife tag
        if(tempFam->wifeRef != NULL){
            sprintf(refBuffer, "1 WIFE %s\n", tempFam->wifeRef);
            strcat(stringBuffer, refBuffer);
        }//end if

        if(getLength(tempFam->family->events) != 0){
            char* eventBuffer = getFileEvent(&tempFam->family->events);
            strcat(stringBuffer, eventBuffer);
            free(eventBuffer);
        }//end if

        //for the child tags
        ListIterator childIterator = createIterator(tempFam->childRef);
        for(int y=0; y<getLength(tempFam->childRef) ;y++){
            char* childString = nextElement(&childIterator);
            sprintf(refBuffer, "1 CHIL %s\n", childString);
            strcat(stringBuffer, refBuffer);
        }//end for

        strcat(stringToBeReturned, stringBuffer);
        free(stringBuffer);
    }//end for

    return stringToBeReturned;
}//end func

char* getFileEvent(List* eventList){
    //dec vars
    ListIterator eventIterator = createIterator(*eventList);
    int memSize = 256*3;
    char dateString[256];
    char placeString[256];
    char* stringToBeReturned = calloc(1, sizeof(char)*(memSize*getLength(*eventList)));
    //for loop event list unti
    for(int x=0; x<getLength(*eventList) ;x++){
        //set the vars
        Event* tempEvent = nextElement(&eventIterator);
        memSize = memSize + 256 + (256 * getLength(*eventList));
        char* stringBuffer = calloc(1, sizeof(char)*memSize);

        //for the event type tag
        sprintf(stringBuffer, "1 %s\n", tempEvent->type);

        if(strlen(tempEvent->date) > 1 ){
            sprintf(dateString, "2 DATE %s\n", tempEvent->date);
            strcat(stringBuffer, dateString);
        }//end if

        if(strlen(tempEvent->place) > 1 ){
            sprintf(placeString, "2 PLAC %s\n", tempEvent->place);
            strcat(stringBuffer, placeString);
        }//end if

        strcat(stringToBeReturned, stringBuffer);
        free(stringBuffer);
    }//end for

    return stringToBeReturned;
}//end func

void setReferences(List* famList, List* indiList, const GEDCOMobject* object){
    //create iterator for the list of family and indi
    ListIterator famIterator = createIterator(object->families);
    ListIterator indiIterator = createIterator(object->individuals);

    //creata a vars for counter the code
    for(int x=1; x<=getLength(object->individuals); x++){
        char referenceCode[99];
        sprintf(referenceCode, "@I%d@", x);
        IndividualReference* indiRef = initIndiRef(referenceCode, nextElement(&indiIterator));
        insertBack(indiList, indiRef);
    }//end for

    for(int x=1; x<=getLength(object->families); x++){
        char referenceCode[99];
        sprintf(referenceCode, "@F%d@", x);
        FamilyReference* famRef = initFamRef(referenceCode, nextElement(&famIterator));
        insertBack(famList, famRef);
    }//end for
}//end func

void linkReferences(List* famList, List* indiList){
    //loop through the all the fam and link with the indi
    ListIterator famIterator = createIterator(*famList);
    for(int x=0; x<getLength(*famList); x++){
        FamilyReference* famRef = nextElement(&famIterator);
        IndividualReference* searchIndividual = initIndividualReference();

        //searched for the husband for the family pointer
        searchIndividual->individual = famRef->family->husband;
        IndividualReference* husbSearchedResult = findElement(*indiList, findIndividualPointer, searchIndividual);
        if(husbSearchedResult != NULL){
            //if(husbSearchedResult->famsRef != NULL){
                insertBack(&husbSearchedResult->famsRef, setString(famRef->reference));
                famRef->husbRef = setString(husbSearchedResult->reference);
            //}//end if
        }//end if

        //searched for the wife for the family pointer
        searchIndividual->individual = famRef->family->wife;
        IndividualReference* wifeSearchedResult = findElement(*indiList, findIndividualPointer, searchIndividual);
        if(wifeSearchedResult != NULL){
            //if(wifeSearchedResult->famsRef != NULL){
                insertBack(&wifeSearchedResult->famsRef, setString(famRef->reference));
                famRef->wifeRef = setString(wifeSearchedResult->reference);
            //}//end if
        }//end if

        //searched for the child for familly pointer
        ListIterator childIterator = createIterator(famRef->family->children);
        for(int y=0; y<getLength(famRef->family->children); y++){
            Individual* currentChildren = nextElement(&childIterator);

            //search the child
            searchIndividual->individual = currentChildren;
            IndividualReference* childSearchedResult = findElement(*indiList, findIndividualPointer, searchIndividual);
            if(childSearchedResult != NULL){
                // if(childSearchedResult->famcRef != NULL){
                    insertBack(&childSearchedResult->famcRef, setString(famRef->reference));
                    insertBack(&famRef->childRef, setString(childSearchedResult->reference));
                //}//end if
            }//end if
        }//end for

        deleteIndiRef(searchIndividual);
    }//end for
}//end func

bool findIndividualPointer(const void* first, const void* second){
    //error check
    if(first == NULL || second == NULL){
        return false;
    }//end if
    //cast objects
    IndividualReference* object1 = (IndividualReference*)first;
    IndividualReference* object2 = (IndividualReference*)second;
    //declare vars
    if(object1->individual == object2->individual){
        return true;
    }else{
        return false;
    }//end if
}//end func

ErrorCode validateEvents(List* event){
    ErrorCode error = OK;

    //check event
    ListIterator eventIterator = createIterator(*event);
    for(int y=0; y<getLength(*event); y++){
        Event* currentEvent = nextElement(&eventIterator);

        //check if the event is not null
        if(currentEvent == NULL){
            return INV_RECORD;
        }//end if

        //check the type
        if(strlen(currentEvent->type) > 4){
            return INV_RECORD;
        }//end if

        //check the date
        if(currentEvent->date == NULL){
            return INV_RECORD;
        }//end if
        if(strlen(currentEvent->date) > 200){
            return INV_RECORD;
        }//end if

        //check the place
        if(currentEvent->place == NULL){
            return INV_RECORD;
        }//end if
        if(strlen(currentEvent->place) > 200){
            return INV_RECORD;
        }//end if

    }//end for
    return error;
}//end func


Individual* copyIndividual(Individual* toBeCopied){
    Individual* newIndi = initIndividual();
    if(toBeCopied->givenName != NULL){
        newIndi->givenName = setString(toBeCopied->givenName);
    }else{
        newIndi->givenName = setString("");
    }//end if
    if(toBeCopied->surname){
        newIndi->surname = setString(toBeCopied->surname);
    }else{
        newIndi->surname = setString("");
    }//end if
    newIndi->families = toBeCopied->families;
    newIndi->events =  toBeCopied->events;
    newIndi->otherFields = toBeCopied->otherFields;
    return newIndi;
}//end func

List copyIndividualList(List* list){
    //dec vars
    List copyList = initializeList(printDescendants, dummyDelete, compareGenerations);
    ListIterator iterList1 = createIterator(*list);
    //loop through the firsst list
    for(int x = 0; x < getLength(*list); x++){
        Individual* tempIndi = nextElement(&iterList1);
        insertSorted(&copyList, tempIndi);
    }//end for
    return copyList;
}//end func

List getGeneration(Individual* person, int* counter, int* maxDepth, List* listOfGeneration){
    if(DEBUG_UTILITIES)printf("calling get children\n");
    //dec vars
    List descendantsList = initializeList(dummyPrint, dummyDelete, dummyCompare);
    //check if its empty
    if(person != NULL){

        //iterate the person families
        ListIterator iterateFam = createIterator(person->families);
        if(DEBUG_UTILITIES)printf("getLength: %d\n", getLength(person->families));
        for(int x = 0; x < getLength(person->families); x++){
            Family* family = nextElement(&iterateFam);
            if(family == NULL){
                break;
            }//end if

            //set the current depth
            int currentDepth = *counter;
            if(currentDepth >= *maxDepth){
                *maxDepth = currentDepth;
            }//end if

            //get the each child
            ListIterator iterateChild = createIterator(family->children);
            for(int i = 0; i < getLength(family->children); i++){
                if(DEBUG_UTILITIES)printf("in children loop: %d\n", i);
                Individual* individual = nextElement(&iterateChild);
                if(individual == NULL){
                    break;
                }//end if

                //create a break if the search person is one of the same
                if(person == individual){
                    break;
                }//end if

                //create a break prevention for parent
                IndividualReference* searchChild = initIndividualReference();
                searchChild->individual = individual;
                IndividualReference* searchResult = findElement(*listOfGeneration, findIndividualPointer, searchChild);
                if(searchResult != NULL){
                    deleteIndiRef(searchChild);
                    break;
                }//end if
                deleteIndiRef(searchChild);

                //go to the next depth
                *counter = *counter + 1;

                //set the individual with the depth and insert it to dif struct
                IndividualReference* tempIndi = initIndividualReference();
                tempIndi->individual = individual;
                tempIndi->depth = *counter;
                insertBack(listOfGeneration, tempIndi);

                //insert the to the list and recurse
                insertSorted(&descendantsList, individual);
                List tempChildList = getGeneration(individual, counter, maxDepth, listOfGeneration);
                descendantsList = combinedIndividualList(&descendantsList, &tempChildList);

                //reset depth
                *counter = currentDepth;
            }//end for

        }//end for
    }else{
        if(DEBUG_UTILITIES)printf("person is null\n");
    }//end if

    //return
    if(DEBUG_UTILITIES)printf("return the list of children\n");
    return descendantsList;
}//end funcs

List getAncestor(Individual* person, int* counter, int* maxDepth, List* listOfGeneration){
    //dec vars
    List anscestorList = initializeList(printDescendants, dummyDelete, dummyCompare);
    //check if its empty
    if(person != NULL){

        //loop through through the fam
        ListIterator iterateFam = createIterator(person->families);
        for(int x = 0; x < getLength(person->families); x++){
            Family* family = nextElement(&iterateFam);

            if(family == NULL){
                continue;
            }//end if

            //set the current depth
            int currentDepth = *counter;
            if(currentDepth >= *maxDepth){
                *maxDepth = currentDepth;
                //debug for fixing more than 2 parents
                if(strcmp("Mary", person->givenName) == 0 || strcmp("Fred", person->givenName) == 0){
                    if(family != NULL && getLength(person->families) != x+1 && *counter == 1 && x == 0 && *maxDepth == 1){
                        family = nextElement(&iterateFam);
                    }//end if
                }//end if
            }//end if

            if(family == NULL){
                continue;
            }//end if

            //use new family var for better coding
            Family* currentFam = family;

            //check if the parrent has the person you are passing
            if(currentFam->husband != NULL){
                if(currentFam->husband == person){
                    continue;
                }//end if
            }//end if
            if(currentFam->wife != NULL){
                if(currentFam->wife == person){
                    continue;
                }//end if
            }//end if

            for(int i = 0; i < 2; i++){
                //change between husb and wife
                Individual* individual = NULL;
                if(i == 0){
                     individual = currentFam->husband;
                     if(currentFam->husband == NULL){
                        continue;
                    }//end if
                }//end if
                if(i == 1){
                    individual = currentFam->wife;
                     if(currentFam->wife == NULL){
                        continue;
                    }//end if
                }//end if

                //create a break if the search person is one of the husb/wife
                if(person == individual){
                    break;
                }//end if

                //create a continue prevention for parent
                IndividualReference* searchParents = initIndividualReference();
                searchParents->individual = individual;
                IndividualReference* searchResult = findElement(*listOfGeneration, findIndividualPointer, searchParents);
                if(searchResult != NULL){
                    deleteIndiRef(searchParents);
                    break;
                }//end if
                deleteIndiRef(searchParents);

                //go to the next depth
                *counter = *counter + 1;

                //set the individual with the depth and insert it to dif struct
                IndividualReference* tempIndi = initIndividualReference();
                tempIndi->individual = individual;
                tempIndi->depth = *counter;
                insertBack(listOfGeneration, tempIndi);

                // //debug
                debug("%d: name: %s %s, fam: %d\n", tempIndi->depth, tempIndi->individual->givenName, tempIndi->individual->surname, getLength(tempIndi->individual->families));
                if(strcmp(tempIndi->individual->givenName, "John") == 0){
                    debug("YO\n");
                    ListIterator testFamIter = createIterator(tempIndi->individual->families);
                    for(int t=0; t<getLength(tempIndi->individual->families); t++){
                        Family* testFam = nextElement(&testFamIter);
                        if(testFam->husband != NULL){
                            debug("\t%d: hub name: %s %s\n", tempIndi->depth, testFam->husband->givenName, testFam->husband->surname);
                        }//end if
                        if(testFam->wife != NULL){
                            debug("\t%d: ife name: %s %s\n", tempIndi->depth, testFam->wife->givenName, testFam->wife->surname);
                        }//end if
                    }//end for
                }//end if

                //insert the to the list and recurse
                insertSorted(&anscestorList, individual);
                List tempChildList = getAncestor(individual, counter, maxDepth, listOfGeneration);
                anscestorList = combinedIndividualList(&anscestorList, &tempChildList);

                //reset depth
                *counter = currentDepth;
            }//end for

        }//end for
    }else{
        if(DEBUG_UTILITIES)printf("person is null\n");
    }//end if
    //return
    if(DEBUG_UTILITIES)printf("return the list of children\n");
    return anscestorList;
}//end funcs

List getParent(Individual* person){
    //dec vars
    List listOfAscendance = initializeList(printDescendants, dummyDelete, compareIndividuals);
    //check if its empty
    if(person != NULL){
        ListIterator iterateFam = createIterator(person->families);
        for(int x = 0; x < getLength(person->families); x++){
            Family* family = nextElement(&iterateFam);
            if(family == NULL){
                continue;
            }//end if
            char* printFam = printFamily(family);
            free(printFam);

            //get the each child
            Family* currentFam = family;
            for(int i = 0; i < 2; i++){
                //change between husb and wife
                Individual* currentParent = NULL;
                if(i == 0){
                     currentParent = currentFam->husband;
                     if(currentFam->husband == NULL){
                        continue;
                    }//end if
                }else if(i == 1){
                    currentParent = currentFam->wife;
                     if(currentFam->wife == NULL){
                        continue;
                    }//end if
                }//end if
                insertSorted(&listOfAscendance, currentParent);
                List tempParentList = getParent(currentParent);
                listOfAscendance = combinedIndividualList(&listOfAscendance, &tempParentList);
            }//end for
        }//end for
    }else{
        if(DEBUG_UTILITIES)printf("person is null\n");
    }//end if
    //return
    return listOfAscendance;
}//end funcs

GEDCOMobject* createGEDCOMWrapper(char* fileName){
    GEDCOMobject* object = NULL;
    createGEDCOM(fileName, &object);
    return object;
}//end func

void writeGEDCOMWrapper(char* fileName, GEDCOMobject* object){
    GEDCOMerror error = writeGEDCOM(fileName, object);
    if(error.type != OK){
        fprintf(stderr, "failed to write the gedcom file\n");
    }//end if
}//end func

char* GEDCOMtoJSON(char* fileName){
    //creating the object
    GEDCOMobject* object = NULL;
    GEDCOMerror error = createGEDCOM(fileName, &object);
    if(error.type != OK){
        deleteGEDCOM(object);
        fprintf(stderr, "failed to create object for GEDCOMtoJSON\n");
        return setString("{}");
    }//end if

    //dec vars
    int memSize = 256*7;
    char* string = calloc(memSize, sizeof(char));
    char source[256];
    char version[256];
    char encoding[256];
    char subName[256];
    char subAddress[256];
    char indiNum[256];
    char famNum[256];

    strcpy(source, object->header->source);
    sprintf(version, "%.2f", object->header->gedcVersion);

    if(object->header->encoding == ANSEL){
        strcpy(encoding, "ANSEL");
    }else if(object->header->encoding == UTF8){
        strcpy(encoding, "UTF-8");
    }else if(object->header->encoding == UNICODE){
        strcpy(encoding, "UNICODE");
    }else{
        strcpy(encoding, "ASCII");
    }//end if

    strcpy(subName, object->submitter->submitterName);
    strcpy(subAddress, object->submitter->address);
    sprintf(indiNum, "%d", getLength(object->individuals));
    sprintf(famNum, "%d", getLength(object->families));

    //"{"source":"Blah","gedcVersion":"5.5","encoding":"ASCII", "subName":"Some dude”,"subAddress":"nowhere"}"
    sprintf(string, "{\"fileName\":\"%s\",\"source\":\"%s\",\"gedcVersion\":\"%s\",\"encoding\":\"%s\",\"subName\":\"%s\",\"subAddress\":\"%s\",\"indiNum\":\"%s\",\"famNum\":\"%s\"}",
        fileName, source, version, encoding, subName, subAddress, indiNum, famNum);
    deleteGEDCOM(object);
    return string;
}//end func

char* descToJSON(char* fileName, char* firstName, char* lastName, int numGen){
    //creating the object
    GEDCOMobject* object = NULL;
    GEDCOMerror error = createGEDCOM(fileName, &object);
    if(error.type != OK){
        deleteGEDCOM(object);
        fprintf(stderr, "failed to create obejct for descToJSON\n");
        return setString("[]");
    }//end if

    //dec vars for searching
    Individual* searching = initIndividual();
    searching->givenName = setString(firstName);
    searching->surname = setString(lastName);

    //get the desc
    Individual* personSearched = findPerson(object, comparePerson, searching);
    List desc = getDescendantListN(object, personSearched, numGen);
    char* descJSON = gListToJSON(desc);

    return descJSON;
}//end func

char* anceToJSON(char* fileName, char* firstName, char* lastName, int numGen){
    //creating the object
    GEDCOMobject* object = NULL;
    GEDCOMerror error = createGEDCOM(fileName, &object);
    if(error.type != OK){
        deleteGEDCOM(object);
                fprintf(stderr, "failed to create obejct for anceToJSON\n");

        return setString("[]");
    }//end if

    //dec vars for searching
    Individual* searching = initIndividual();
    searching->givenName = setString(firstName);
    searching->surname = setString(lastName);

    //get the desc
    Individual* personSearched = findPerson(object, comparePerson, searching);
    List desc = getAncestorListN(object, personSearched, numGen);
    char* anceJSON = gListToJSON(desc);

    return anceJSON;
}//end func

char* getIndiListJSON(char* fileName){
    //create object
    GEDCOMobject* object = NULL;
    GEDCOMerror error = createGEDCOM(fileName, &object);
    if(error.type != OK){
        fprintf(stderr, "failed to create obejct for getIndiListJSON\n");
    }//end if
    char* list = iListToJSON(object->individuals);
    return list;
}//end func

void addIndiJSON(char* fileName, char* firstName, char* lastName){
    //create object
    GEDCOMobject* object = NULL;
    GEDCOMerror error = createGEDCOM(fileName, &object);
    if(error.type != OK){
        fprintf(stderr, "failed to create obejct for addIndiJSON\n");
        return;
    }//end if
    //adding a new indi to the object
    Individual* newIndi = initIndividual();
    newIndi->givenName = firstName;
    newIndi->surname = lastName;
    addIndividual(object, newIndi);
    writeGEDCOM(fileName, object);
}//end func

void writeString(char* fileName, char* outputString){
    writeFile(fileName, outputString);
}//end func

char* getJSONString(char* fileName){
    //dec vars
    FILE* filePointer = fopen(fileName, "r");
    if(filePointer == NULL){
        return setString("[]");
    }//end if
    char* stringBuffer = calloc(1, sizeof(stringBuffer));
    int charBuffer;
    int stringSize = 0;
    //read the entire file to the string
    while(true){
        charBuffer = getc(filePointer);
        if(charBuffer == EOF){
            break;
        }//end if
        stringBuffer[stringSize] = (char)charBuffer;
        stringSize++;
        stringBuffer = realloc(stringBuffer, sizeof(stringBuffer)*(stringSize+1));
        stringBuffer[stringSize] = '\0';
    }//end if*/
    fclose(filePointer);
    if(strlen(stringBuffer) < 2){
        return setString("[]");
    }//end if
    return stringBuffer;
}//end func

GEDCOMobject* newGEDCOM(char* source, char* subName, char* subAddress){
    GEDCOMobject* ged = initObject();
    Submitter* sub = initSubmitter();
    strcpy(sub->submitterName, subName);
    strcpy(sub->address, subAddress);
    ged->header->submitter = sub;
    ged->submitter = sub;
    strcpy(ged->header->source, source);
    ged->header->gedcVersion = 5.5;
    return ged;
}//edn func
