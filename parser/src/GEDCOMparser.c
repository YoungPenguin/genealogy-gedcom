//import lib
#include "GEDCOMutilities.h"
#include "GEDCOMparser.h"
#include "LinkedListAPI.h"

//main
GEDCOMerror createGEDCOM(char* fileName, GEDCOMobject** obj){
    if(DEBUG_PARSER)printf("********************************************************* INVOKING CREATE OBJECT\n");
    debug("*************************************** %s INVOKING CREATE OBJECT\n", fileName);
    *obj = NULL;
    //check the file name
    GEDCOMerror error = initError(OK, -1);
    error = checkFile(fileName);
    if(error.type != OK){
        return error;
    }//end if

    //dec vars
    int lineSize = 0;
    char** line = readFileByChar(fileName, &lineSize/*, 256*/);
    HelperVars vars = initVars(fileName, line, lineSize);//dont forget to free the object after

    //check the gedcom
    error = checkGedcom(&vars);
    if(error.type != OK){
        clearList(&vars.indiRef);
        clearList(&vars.famRef);
        clearList(&vars.garbageFamily);
        freeStringArray(line, lineSize);
        deleteGEDCOM(vars.object);
        *obj = NULL;
        if(DEBUG_PARSER)printf("CHECK GEDCOM\n%s\n", printError(error));
        return error;
    }//end if

    //check the header
    error = checkHeader(&vars);
    if(error.type != OK){
        clearList(&vars.indiRef);
        clearList(&vars.famRef);
        clearList(&vars.garbageFamily);
        freeStringArray(line, lineSize);
        deleteGEDCOM(vars.object);
        *obj = NULL;
        if(DEBUG_PARSER)printf("CHECK HEADER\n%s\n", printError(error));
        return error;
    }//end if*/

    error = checkIfLineNumbers(&vars);
    if(error.type != OK){
        clearList(&vars.indiRef);
        clearList(&vars.famRef);
        clearList(&vars.garbageFamily);
        freeStringArray(line, lineSize);
        deleteGEDCOM(vars.object);
        if(DEBUG_PARSER)printf("CHECK LINE\n%s\n", printError(error));
        return error;
    }//end if

    //check the records
    error = checkRecord(&vars);
    if(error.type != OK){
        clearList(&vars.indiRef);
        clearList(&vars.famRef);
        clearList(&vars.garbageFamily);
        freeStringArray(line, lineSize);
        deleteGEDCOM(vars.object);
        *obj = NULL;
        if(DEBUG_PARSER)printf("CHECK RECORD\n%s\n", printError(error));
        return error;
    }//end if*/

    //save the struct to the object
    *obj = vars.object;

    //return
    freeStringArray(line, lineSize);
    clearList(&vars.indiRef);
    clearList(&vars.famRef);
    clearList(&vars.garbageFamily);
    return error;
}//end parser func

char* printGEDCOM(const GEDCOMobject* obj){//its done
    //print if there is no object
    char* stringToPrint = setString("There is nothing to print\n");
    int stringSize = 399;
    int memSize = 999;
    char versionString[256];
    static char* encodingType[] = {
        "ANSEL", "UTF-8", "UNICODE", "ASCII"
    };//end array

    //check if object is null
    if(obj != NULL){
        //reset the stringtoprint so it doesnt show the prev string
        free(stringToPrint);
        stringToPrint = calloc(1, sizeof(char) * stringSize * memSize);

        //print the header
        strcat(stringToPrint, "********************((( HEADER )))********************\n");
        strcat(stringToPrint, "Header source: |");
        strcat(stringToPrint, obj->header->source);
        strcat(stringToPrint, "|\nHeader version: ");
        sprintf(versionString, "%0.2f", obj->header->gedcVersion);
        strcat(stringToPrint, versionString);
        strcat(stringToPrint, "\nEncoding: ");
        strcat(stringToPrint, encodingType[obj->header->encoding]);

        //print the other field of the header
        strcat(stringToPrint, "\nHeader other field:\n");
        char* headerString = toString(obj->header->otherFields);
        memSize = memSize + (strlen(headerString) * stringSize);
        stringToPrint = realloc(stringToPrint, sizeof(char*) * memSize);
        char* otherFieldString = toString(obj->header->otherFields);
        strcat(stringToPrint, otherFieldString);

        //print the family and the individual
        char* indiString = toString(obj->individuals);
        char* famString = toString(obj->families);
        memSize = memSize + strlen(indiString) + strlen(famString) + ((getLength(obj->individuals)*9) + (getLength(obj->families)*9));
        stringToPrint = realloc(stringToPrint, sizeof(char*)* memSize);
        strcat(stringToPrint, "********************((( INDIVIDUAL )))********************\n");
        //strcat(stringToPrint, "List indivdual:\n");
        strcat(stringToPrint, indiString);
        strcat(stringToPrint, "\n********************((( FAMILIES )))********************\n");
        //strcat(stringToPrint, "List families:\n");
        strcat(stringToPrint, famString);
        strcat(stringToPrint, "\n********************((( SUBMITTER )))********************\n");
        char* subString = printSubmitter(obj->submitter);
        strcat(stringToPrint, subString);
        strcat(stringToPrint, "********************((( TRAILER )))********************\n\0");

        //free
        free(headerString);
        free(otherFieldString);
        free(indiString);
        free(famString);
        free(subString);
    }//end if

    //return
    return stringToPrint;
}//end parser func

char* printError(GEDCOMerror err){
    //array for tags and enums etc
    static char* errorType[] = {
        "OK", "INV_FILE", "INV_GEDCOM",
        "INV_HEADER", "INV_RECORD", "OTHER"
    };//end array
    //print the error if there is error
    char* stringToPrint = calloc(1, sizeof(char)* (strlen(errorType[err.type])+99) );
    sprintf(stringToPrint, "Error code: %s\nError line: %d\n", errorType[err.type], err.line);
    strcat(stringToPrint, "\0");
    //return the string
    return stringToPrint;
}//end func

void deleteGEDCOM(GEDCOMobject* obj){
    if(obj == NULL){
        return;
    }//end if
    deleteFamilyGarbageWrapper();
    deleteHeader(obj->header);
    clearList(&obj->families);
    clearList(&obj->individuals);
    free(obj);
}//end parser func*/

Individual* findPerson(const GEDCOMobject* familyRecord, bool (*compare)(const void* first, const void* second), const void* person){
    //error check
    if(familyRecord == NULL || person == NULL){
        return NULL;
    }//end if
    //find thea person
    Individual* persongBeingSearched = NULL;
    persongBeingSearched =  findElement(familyRecord->individuals, &comparePerson, person);
    return persongBeingSearched;
}//end parser func

List getDescendants(const GEDCOMobject* familyRecord, const Individual* person){//dundo
    List listOfDescendant;
    //check if its is null and valid
    if(familyRecord == NULL || person == NULL){
        listOfDescendant = initializeList(printDescendants, dummyDelete, compareIndividuals);
        return listOfDescendant;
    }//end if
    listOfDescendant = initializeList(printDescendants, dummyDelete, compareIndividuals);

    //find the person and pass to the func to return the list
    Individual* searchPerson = findPerson(familyRecord, comparePerson, person);
    if(searchPerson != NULL){
        listOfDescendant = getChildren(searchPerson);
    }else{
        if(DEBUG_PARSER)printf("searchPerson: NULL\n");
    }//end if
    return listOfDescendant;
}//end parser func

// write GEDCOM file //

GEDCOMerror writeGEDCOM(char* fileName, const GEDCOMobject* obj){//done
    //check the file name
    GEDCOMerror error = initError(OK, -1);

    //check if object is null
    if(obj == NULL){
        fprintf(stderr, "Object is NULL.\n");
        return initError(WRITE_ERROR, -1);
    }//end if

    //check if the file name is empty
    if(fileName == NULL){
        fprintf(stderr, "File name cannot be empty string.\n");
        return initError(WRITE_ERROR, -1);
    }//end if

    //check if the file names end with ".ged"
    if (strstr(fileName, ".ged") == NULL) {
        fprintf(stderr, "File extension must be a (.ged).\n");
        return initError(WRITE_ERROR, -1);
    }//end if

    //validate the object
    error.type = validateGEDCOM(obj);
    if(error.type != OK){
        return error;
    }//end if

    //create a list of family and indi
    List famList = initializeList(printReference, deleteFamRef, compareFamRef);
    List indiList = initializeList(printReference, deleteIndiRef, compareIndiRef);
    //set the reference pointer
    setReferences(&famList, &indiList, obj);
    linkReferences(&famList, &indiList);

    //write to the file
    char* fileOutput = getFileString(&famList, &indiList, obj);
    writeFile(fileName, fileOutput);
    clearList(&famList);
    clearList(&indiList);
    free(fileOutput);
    return error;
}//func

ErrorCode validateGEDCOM(const GEDCOMobject* obj){//comlpleted, need to check indi otherfield
    //dec vars
    ErrorCode error = OK;

    //checking the invalid gedcom
    if(obj == NULL){
        debug("object is null\n");
        return INV_GEDCOM;
    }//end if
    if(obj->header == NULL || obj->submitter == NULL){
        debug("header and subm is null\n");
        return INV_GEDCOM;
    }//end if

    //checking the invalid header !!! not sure if source is mandatory
    Header* header = obj->header;
    if(strlen(header->source) == 0 || strlen(header->source) > 200 || (header->gedcVersion < 1) || header->submitter == NULL){
        debug("invalid header struct\n");
        return INV_HEADER;
    }//end if

    //checking the submitter name
    Submitter* submitter = obj->submitter;
    if(strlen(submitter->submitterName) == 0 || strlen(submitter->submitterName) > 60){
        debug("invalid subm name\n");
        return INV_RECORD;
    }//end if

    //check indi records
    ListIterator indiIterator = createIterator(obj->individuals);
    for(int x=0; x<getLength(obj->individuals) ;x++){
        Individual* currentIndi = nextElement(&indiIterator);

        //check if indi is null
        if(currentIndi == NULL){
            debug("indi is null\n");
            return INV_RECORD;
        }//end if

        //check for the first name
        if(currentIndi->givenName == NULL){
            debug("given name is null\n");
            return INV_RECORD;
        }//end if
        if(strlen(currentIndi->givenName) > 200){
            debug("given name is over 200\n");
            return INV_RECORD;
        }//end if

        //check for the last name
        if(currentIndi->surname == NULL){
            debug("surname is null\n");
            return INV_RECORD;
        }//end if
        if(strlen(currentIndi->surname) > 200){
            debug("surname is over 200\n");
            return INV_RECORD;
        }//end if

        //check for the events
        error = validateEvents(&currentIndi->events);
        if(error != OK){
            debug("invalid indi events\n");
            return INV_RECORD;
        }//end if

        //check families?
        ListIterator famIter = createIterator(currentIndi->families);
        for(int y=0; y<getLength(currentIndi->families); y++){
            Family* currentFam = nextElement(&famIter);
            if(currentFam == NULL){
                debug("one of the indi fam is null\n");
                return INV_RECORD;
            }//end if
        }//end for
    }//end for

    //check indi records
    ListIterator famIterator = createIterator(obj->families);
    for(int x=0; x<getLength(obj->families) ;x++){
        Family* currentFam = nextElement(&famIterator);

        //check if fam is null
        if(currentFam == NULL){
            debug("fam is null\n");
            return INV_RECORD;
        }//end if

        //check children?
        ListIterator childIter = createIterator(currentFam->children);
        for(int y=0; y<getLength(currentFam->children); y++){
            Individual* currentChild = nextElement(&childIter);
            if(currentChild == NULL){
                debug("one of fam children is null\n");
                return INV_RECORD;
            }//end if
        }//end for

        //check the events
        error = validateEvents(&currentFam->events);
        if(error != OK){
            debug("invalid fam events\n");
            return INV_RECORD;
        }//end if
    }//end for

    return error;
}//end func

List getDescendantListN(const GEDCOMobject* familyRecord, const Individual* person, unsigned int maxGen){
    //dec vars
    List listOfDescendant;
    List listOfGeneration;

    //check if its is null and valid
    if(familyRecord == NULL || person == NULL){
        listOfDescendant = initializeList(dummyPrint, dummyDelete, dummyCompare);
        return listOfDescendant;
    }//end if

    //find the person and pass to the func to return the list
    Individual* searchPerson = findPerson(familyRecord, comparePerson, person);
    if(searchPerson == NULL){
        listOfDescendant = initializeList(dummyPrint, dummyDelete, dummyCompare);
        return listOfDescendant;
    }//end if

    if(searchPerson != NULL){
        int counter = 0;
        int maxDepth = 0;
        listOfDescendant = initializeList(dummyPrint, deleteIndiRef, dummyCompare);
        List listToBeDeleted = getGeneration(searchPerson, &counter, &maxDepth, &listOfDescendant);
        debug("Max depth: %d\n\n", maxDepth);

        //init a list of a list of generation
        listOfGeneration = initializeList(printGeneration, deleteGeneration, dummyCompare);

        //to make sure it go above the max depth
        if(maxGen == 0){
            maxGen = maxDepth;
        }else if(maxGen >= maxDepth){
            maxGen = maxDepth;
        }//end if

        //loop through the depth and assign all the generatuion to the list
        for(int depthCounter=1; depthCounter<=maxGen; depthCounter++){
            //init vars
            ListIterator descIterator = createIterator(listOfDescendant);
            List* tempSubListOfGen = initializeListPointer(printDescendants, dummyDelete, compareGenerations);
            //loop to assign to the sub list
            for(int x=0; x<getLength(listOfDescendant); x++){
                IndividualReference* currentIndi = nextElement(&descIterator);

                //assign the indi to proper gen
                if(currentIndi->depth == depthCounter){
                    insertSorted(tempSubListOfGen, currentIndi->individual);
                }//end if
            }//end for
            insertBack(&listOfGeneration, tempSubListOfGen);
        }//end for

        clearList(&listOfDescendant);
        clearList(&listToBeDeleted);
    }else{
        if(DEBUG_PARSER)printf("searchPerson: NULL\n");
    }//end if

    return listOfGeneration;
}//end func

List getAncestorListN(const GEDCOMobject* familyRecord, const Individual* person, int maxGen){
    debug("!!!!!!!!!!!!!!!!!!!!!!! CALLING GET ANCESTOR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    //dec vars
    List listOfAscendance;
    List listOfGeneration;

    //check if its is null and valid
    if(familyRecord == NULL || person == NULL){
        listOfAscendance = initializeList(dummyPrint, dummyDelete, dummyCompare);
        return listOfAscendance;
    }//end if

    //find the person and pass to the func to return the list
    Individual* searchPerson = findPerson(familyRecord, comparePerson, person);
    if(searchPerson == NULL){
        listOfAscendance = initializeList(dummyPrint, dummyDelete, dummyCompare);
        return listOfAscendance;
    }//end if

    if(searchPerson != NULL){
        int counter = 0;
        int maxDepth = 0;
        listOfAscendance = initializeList(dummyPrint, deleteIndiRef, dummyCompare);
        List listToBeDeleted = getAncestor(searchPerson, &counter, &maxDepth, &listOfAscendance);
        debug("Max depth: %d\n\n", maxDepth);

        //init a list of a list of generation
        listOfGeneration = initializeList(printGeneration, deleteGeneration, dummyCompare);

        //to make sure it go above the max depth
        if(maxGen == 0){
            maxGen = maxDepth;
        }else if(maxGen >= maxDepth){
            maxGen = maxDepth;
        }//end if

        //loop through the depth and assign all the generatuion to the list
        for(int depthCounter=1; depthCounter<=maxGen; depthCounter++){
            //init vars
            ListIterator descIterator = createIterator(listOfAscendance);
            List* tempSubListOfGen = initializeListPointer(printDescendants, dummyDelete, compareGenerations);
            //loop to assign to the sub list
            for(int x=0; x<getLength(listOfAscendance); x++){
                IndividualReference* currentIndi = nextElement(&descIterator);

                //assign the indi to proper gen
                if(currentIndi->depth == depthCounter){
                    insertSorted(tempSubListOfGen, currentIndi->individual);
                }//end if
            }//end for
            insertBack(&listOfGeneration, tempSubListOfGen);
        }//end for

        clearList(&listOfAscendance);
        clearList(&listToBeDeleted);
    }else{
        if(DEBUG_PARSER)printf("searchPerson: NULL\n");
    }//end if

    return listOfGeneration;
}//end func

char* indToJSON(const Individual* ind){
    //check if indi is null
    if(ind == NULL){
        return setString("");
    }//end if

    char sex[256] = {"NA"};
    ListIterator fieldIter = createIterator(ind->otherFields);
    for(int x=0; x<getLength(ind->otherFields) ;x++){
        Field* curField = nextElement(&fieldIter);
        if(strcmp(curField->tag, "SEX") == 0){
            if(strcmp(curField->value, "F") == 0){
                strcpy(sex, "F");
            }else{
                strcpy(sex, "M");
            }//end if
        }//end if
    }//end for
    int famNum = getLength(ind->families);

    //assign the string
    int memSize = 256*2;
    char* string = calloc(1, sizeof(char)*memSize);
    sprintf(string, "{\"givenName\":\"%s\",\"surname\":\"%s\",\"sex\":\"%s\",\"famNum\":\"%d\"}", ind->givenName, ind->surname, sex, famNum);
    return string;
}//end func

Individual* JSONtoInd(const char* str){
    //error check
    if(str == NULL){
        return NULL;
    }//end if

    //dec vars
    char* stringToBeSplit = setString((char*)str);
    int tokenSize = 0;
    char** token = split(stringToBeSplit, "[]{}:,", &tokenSize);
    free(stringToBeSplit);

    //error check
    if(tokenSize < 4){
        freeStringArray(token, tokenSize);
        return NULL;
    }//end if
    Individual* newIndi = initIndividual();

    //check the given name if its null and assign the given name
    for(int x=0; x<tokenSize; x++){
        if(strcmp("\"givenName\"", token[x]) == 0){
            if(strcmp(token[x+1],"\"\"") == 0){
                newIndi->givenName = setString("");
            }else{
                int valueSize;
                char** value = split(token[x+1], "\"", &valueSize);
                newIndi->givenName = setString(value[0]);
                freeStringArray(value, valueSize);
            }//end if
        }//end if
    }//end for

    //check the surname if its null and assign the surname
    for(int x=0; x<tokenSize; x++){
        if(strcmp("\"surname\"", token[x]) == 0){
            if(strcmp(token[x+1],"\"\"") == 0){
                newIndi->surname = setString("");
            }else{
                int valueSize;
                char** value = split(token[x+1], "\"", &valueSize);
                newIndi->surname = setString(value[0]);
                freeStringArray(value, valueSize);
            }//end if
        }//end if
    }//end for

    freeStringArray(token, tokenSize);
    return newIndi;
}//end func

GEDCOMobject* JSONtoGEDCOM(const char* str){
    //error check
    if(str == NULL){
        return NULL;
    }//end if

    //dec vars
    char* stringToBeSplit = setString((char*)str);
    int tokenSize = 0;
    char** token = split(stringToBeSplit, "[]{}:,", &tokenSize);
    free(stringToBeSplit);

    //error check
    if(tokenSize < 10){
        freeStringArray(token, tokenSize);
        return NULL;
    }//end if
    GEDCOMobject* newObject = initObject();
    Submitter* newSub = initSubmitter();
    newObject->submitter = newSub;
    newObject->header->submitter = newSub;

    //check the source if its null and assign the source
    for(int x=0; x<tokenSize; x++){
        if(strcmp("\"source\"", token[x]) == 0){
            if(strcmp(token[x+1],"\"\"") == 0){
                newObject->header->source[0] = '\0';
            }else{
                int valueSize;
                char** value = split(token[x+1], "\"", &valueSize);
                strcpy(newObject->header->source, value[0]);
                freeStringArray(value, valueSize);
            }//end if
        }//end if
    }//end for

    //check the version if its null and assign the version
    for(int x=0; x<tokenSize; x++){
        if(strcmp("\"gedcVersion\"", token[x]) == 0){
            if(strcmp(token[x+1],"\"\"") == 0){
                newObject->header->gedcVersion = 5.5;
            }else{
                int valueSize;
                char** value = split(token[x+1], "\"", &valueSize);
                newObject->header->gedcVersion = atof(value[0]);
                freeStringArray(value, valueSize);
            }//end if
        }//end if
    }//end for

    //check the encoding
    for(int x=0; x<tokenSize; x++){
        if(strcmp("\"encoding\"", token[x]) == 0){
            if(strcmp(token[x+1],"\"\"") == 0){
                newObject->header->encoding = ASCII;
            }else{
                int valueSize;
                char** value = split(token[x+1], "\"", &valueSize);
                if(strcmp(value[0], "ANSEL") == 0){
                    newObject->header->encoding = ANSEL;
                }else if(strcmp(value[0], "UTF8") == 0 || strcmp(value[0], "UTF-8") == 0){
                    newObject->header->encoding = UTF8;
                }else if(strcmp(value[0], "UNICODE") == 0){
                    newObject->header->encoding = UNICODE;
                }else{
                    newObject->header->encoding = ASCII;
                }//end if
                freeStringArray(value, valueSize);
            }//end if
        }//end if
    }//end for

    //check the subname if its null and assign the subname
    for(int x=0; x<tokenSize; x++){
        if(strcmp("\"subName\"", token[x]) == 0){
            if(strcmp(token[x+1],"\"\"") == 0){
                newObject->submitter->submitterName[0] = '\0';
            }else{
                int valueSize;
                char** value = split(token[x+1], "\"", &valueSize);
                strcpy(newObject->submitter->submitterName, value[0]);
                freeStringArray(value, valueSize);
            }//end if
        }//end if
    }//end for

    //check the subname if its null and assign the address
    for(int x=0; x<tokenSize; x++){
        if(strcmp("\"subAddress\"", token[x]) == 0){
            if(strcmp(token[x+1],"\"\"") == 0){
                newObject->submitter->address[0] = '\0';
            }else{
                int valueSize;
                char** value = split(token[x+1], "\"", &valueSize);
                strcpy(newObject->submitter->address, value[0]);
                freeStringArray(value, valueSize);
            }//end if
        }//end if
    }//end for

    freeStringArray(token, tokenSize);
    return newObject;
}//end func

void addIndividual(GEDCOMobject* obj, const Individual* toBeAdded){
    //error check
    if(obj == NULL || toBeAdded == NULL ){
        return;
    }//end if
    //insert the indi
    insertBack(&obj->individuals, (void*)toBeAdded);
}//end func

char* iListToJSON(List iList){
    //dec vars
    int mem = 256;
    char* string = calloc(1, sizeof(char)*mem);

    //set the bracket
    strcat(string, "[");
    ListIterator indiIter = createIterator(iList);
    for(int y=0; y<getLength(iList); y++){
        Individual* tempIndi = nextElement(&indiIter);
        char* tempIndiString = indToJSON(tempIndi);

        // debug("debug indToJSON:\n %s\n", tempIndiString);
        mem = mem+ 256;
        string = realloc(string, sizeof(char)*mem);
        strcat(string , tempIndiString);
        free(tempIndiString);

        //set comma
        if( y < getLength(iList)-1 ){
            strcat(string , ",");
        }//end if

    }//end for
    strcat(string, "]");

    //debug("debug iListToJSON:\n%s\n", string);CHECK;
    return string;
}//end func

char* gListToJSON(List gList){
    //dec vars
    int mem = 256;
    char* string = calloc(1, sizeof(char)*mem);

    //loop through list of list
    ListIterator listIter = createIterator(gList);
    strcat(string, "[");
    for(int x=0; x<getLength(gList) ;x++){
        List* indiList = nextElement(&listIter);

        //loop trough the list of indi
        strcat(string, "[");
        ListIterator indiIter = createIterator(*indiList);
        for(int y=0; y<getLength(*indiList); y++){
            Individual* tempIndi = nextElement(&indiIter);
            char* tempIndiString = indToJSON(tempIndi);

            // debug("debug indToJSON:\n %s\n", tempIndiString);
            mem = mem+ 256;
            string = realloc(string, sizeof(char)*mem);
            strcat(string , tempIndiString);
            free(tempIndiString);

            if( y < getLength(*indiList)-1 ){
                strcat(string , ",");
            }//end if
        }//end for
        strcat(string, "]");

        if( x < getLength(gList)-1 ){
            strcat(string , ",");
        }//end if
    }//end for
    strcat(string, "]");

    //debug("debug gListToJSON:\n%s\n", string);CHECK;
    return string;
}//end func

// stuff 4 later

void deleteGeneration(void* toBeDeleted){
    //dec vars
    List* list = (List*)toBeDeleted;
    clearList(list);
    free(list);
    return;
}//end func

int compareGenerations(const void* first,const void* second){
    //check if its null
    if(first == NULL || second == NULL){
        return 0;
    }//end if

    //dec vars
    Individual* object1 = (Individual*)first;
    Individual* object2 = (Individual*)second;
    if(object1->givenName == NULL || object1->surname == NULL || object2->givenName == NULL || object2->givenName == NULL){
        return 0;
    }//end if
    char indiSurname1[256];
    char indiSurname2[256];
    char indiGivenName1[256];
    char indiGivenName2[256];
    sprintf(indiSurname1, "%s", object1->surname);
    sprintf(indiSurname2, "%s", object2->surname);
    sprintf(indiGivenName1, "%s", object1->givenName);
    sprintf(indiGivenName2, "%s", object2->givenName);

    //return
    if(compareString(indiSurname1, indiSurname2) != 0){
        return compareString(indiSurname1, indiSurname2);
    }//end if
    return compareString(indiGivenName1, indiGivenName2);
}//end funcc

char* printGeneration(void* toBePrinted){
    //check if its null
    if(toBePrinted == NULL){
        return NULL;
    }//end dif

    //dec vars and return
    List* list = (List*)toBePrinted;
    char* listString = toString(*list);
    int memSize = 999;
    char* string = calloc(1 , sizeof(char)* (memSize+strlen(listString)));
    sprintf(string, "-----<<< GENERATION >>>-----\n");
    strcat(string, listString);
    strcat(string, "\n");
    free(listString);
    return string;
}//end func

void deleteEvent(void* toBeDeleted){//checked
    //check if is null
    if(toBeDeleted == NULL){
        return;
    }//end if

    //delete the event
    Event* object = (Event*)toBeDeleted;
    if(object->date != NULL ){
        free(object->date);
    }//end if
    if(object->place != NULL){
        free(object->place);
    }//end if
    clearList(&object->otherFields);
    free(object);
}//end func

int compareEvents(const void* first,const void* second){//checked
    //check if null
    if(first == NULL || second == NULL){
        return 0;
    }//end if

    //dec vars
    int result = 0;
    Event* object1 = (Event*)first;
    Event* object2 = (Event*)second;
    //check if there is a null
    if(object1->date == NULL || object2->date == NULL){
        return compareString(object1->type, object2->type);//type is mandatory
    }//end if
    return compareString(object1->type, object2->type);

    //dec vars
    int tokenSize1 = 0;
    char** token1 = split(object1->date, " ", &tokenSize1);
    int tokenSize2 = 0;
    char** token2 = split(object2->date, " ", &tokenSize2);

    //check the year
    if(tokenSize1 > 1 && tokenSize2 > 1){
        //if(DEBUG_PARSER)printf("comparing year %s = %s\n", token1[tokenSize1-1], token2[tokenSize2-1]);
        if(checkStringIfNumbers(token1[tokenSize1-1]) && checkStringIfNumbers(token2[tokenSize2-1])){
            if( atoi(token1[tokenSize1-1]) < atoi(token2[tokenSize2-1]) ){
                freeStringArray(token1, tokenSize1);
                freeStringArray(token2, tokenSize2);
                return -1;
            }else if( atoi(token1[tokenSize1-1]) > atoi(token2[tokenSize2-1]) ){
                freeStringArray(token1, tokenSize1);
                freeStringArray(token2, tokenSize2);
                return 1;
            }else if( atoi(token1[tokenSize1-1]) == atoi(token2[tokenSize2-1]) ){
                freeStringArray(token1, tokenSize1);
                freeStringArray(token2, tokenSize2);
                return strcmp(object1->type, object2->type);
            }//end if
        }//end if
    }//end if

    //return
    freeStringArray(token1, tokenSize1);
    freeStringArray(token2, tokenSize2);
    return result;
}//end func

char* printEvent(void* toBePrinted){//checked
    //check if null
    if(toBePrinted == NULL){
        return setString("");
    }//end if

    //return
    Event* object = (Event*)toBePrinted;
    char* fieldString = toString(object->otherFields);
    char* stringToPrint = calloc(1, sizeof(char)* (99 + 99 + 256 + 256 + strlen(fieldString)));
    sprintf(stringToPrint, "|%s| |%s| |%s|\n", object->type, object->date, object->place);

    //free and return
    free(fieldString);
    return stringToPrint;
}//end func


void deleteIndividual(void* toBeDeleted){//checked
    //check if null
    if(toBeDeleted == NULL){
        return;
    }//end if

    //delete indi
    Individual* object = (Individual*)toBeDeleted;
    if(object->givenName != NULL){
        free(object->givenName);
    }//end if
    if(object->surname != NULL){
        free(object->surname);
    }//end if
    clearList(&object->events);
    clearList(&object->families);
    clearList(&object->otherFields);
    free(object);
}//end func

int compareIndividuals(const void* first,const void* second){//checked
    //check if its null
    if(first == NULL || second == NULL){
        return 0;
    }//end if

    //dec vars
    Individual* object1 = (Individual*)first;
    Individual* object2 = (Individual*)second;
    char individual1[256];
    char individual2[256];
    sprintf(individual1, "%s,%s", object1->surname, object1->givenName);
    sprintf(individual2, "%s,%s", object2->surname, object2->givenName);
    return compareString(individual1, individual2);
}//end func

char* printIndividual(void* toBePrinted){//checked
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
    strcat(stringToPrint, "\n\t---< Individual >---\nName: |");
    if(object->givenName != NULL){
        strcat(stringToPrint, object->givenName);
        strcat(stringToPrint, "| |");
    }//end if
    if(object->surname != NULL){
        strcat(stringToPrint, object->surname);
    }//end if
    strcat(stringToPrint, "|\nEvents: \n");
    strcat(stringToPrint, eventString);
    // strcat(stringToPrint, "Families: \n");
    // strcat(stringToPrint, famString);
    strcat(stringToPrint, "Other fields: \n");
    strcat(stringToPrint, fieldString);
    strcat(stringToPrint, "\0");

    /*free*/
    free(eventString);
    free(famString);
    free(fieldString);
    return stringToPrint;
}//end func

void deleteFamily(void* toBeDeleted){//checked
    //check if null
    if(toBeDeleted == NULL){
        return;
    }//end if

    //delete family
    Family* object = (Family*)toBeDeleted;
    clearList(&object->events);
    clearList(&object->children);
    clearList(&object->otherFields);
    free(object);
}//end func

int compareFamilies(const void* first,const void* second){//checked
    //dec vars
    Family* object1 = (Family*)first;
    Family* object2 = (Family*)second;
    int countFamily1 = 0;
    int countFamily2 = 0;

    //count the number of indi in the family for the first one
    if(object1->wife != NULL){
        countFamily1++;
    }//end if
    if(object1->husband != NULL){
        countFamily1++;
    }//end if
    countFamily1 = countFamily1 + getLength(object1->children);

    //check the number of person in the second family
    if(object2->wife != NULL){
        countFamily2++;
    }//end if
    if(object2->husband != NULL){
        countFamily2++;
    }//end if
    countFamily2 = countFamily2 + getLength(object2->children);

    //return the result
    if(countFamily1 < countFamily2){
        return -1;
    }else if(countFamily1 > countFamily2){
        return 1;
    }else{
        return 0;//means it is the same
    }//end if
}//end func

char* printFamily(void* toBePrinted){//checked
    //check if its null
    if(toBePrinted == NULL){
        return setString("");
    }//end if

    //return
    Family* object = (Family*)toBePrinted;
    char* stringEvent = toString(object->events);
    char* stringChildren = toString(object->children);
    int mallocSize = strlen(stringChildren) + strlen(stringEvent) + 99;
    char stringHusband[256];
    char stringWife[256];
    char* stringToPrint = calloc(1, sizeof(char) * (mallocSize+500));
    //set the husband
    if(object->husband != NULL){
        sprintf(stringHusband, "|%s| |%s|", object->husband->givenName, object->husband->surname);
    }else{
        strcpy(stringHusband, "");
    }//end if
    //set the wife
    if(object->wife != NULL){
        sprintf(stringWife, "|%s| |%s|", object->wife->givenName, object->wife->surname);
    }else{
        strcpy(stringWife, "");
    }//end if
    strcat(stringToPrint, "\n\t---< Family >---\nHusband: ");
    strcat(stringToPrint, stringHusband);
    strcat(stringToPrint, "\nWife: ");
    strcat(stringToPrint, stringWife);
    strcat(stringToPrint, "\nEvent: \n");
    strcat(stringToPrint, stringEvent);
    strcat(stringToPrint, "Chilren: \n");
    strcat(stringToPrint, stringChildren);
    strcat(stringToPrint, "\0");
    /*free*/
    free(stringEvent);
    free(stringChildren);
    return stringToPrint;
}//end func

/***********************************************************/

void deleteField(void* toBeDeleted){//checked
    //check if its null
    if(toBeDeleted == NULL){
        return;
    }//end if

    //delete field
    Field* object = (Field*)toBeDeleted;
    if(object->tag != NULL){
        free(object->tag);
    }//end if
    if(object->value != NULL){
        free(object->value);
    }//end if
    free(object);
}//end func

int compareFields(const void* first,const void* second){//checked
    //check if null
    if(first == NULL || second == NULL){
        return 0;
    }//end if

    Field* object1 = (Field*)first;
    Field* object2 = (Field*)second;
    char* field1 = calloc(1, sizeof(char)* (strlen(object1->tag) + strlen(object1->value) + 99) );
    char* field2 = calloc(1, sizeof(char)* (strlen(object2->tag) + strlen(object2->value) + 99) );
    //set the string
    sprintf(field1, "%s %s", object1->tag, object1->value);
    sprintf(field2, "%s %s", object2->tag, object2->value);
    //comapre the too string
    int compareResult = strcmp(field1, field2);
    free(field1);
    free(field2);
    if(compareResult < 0){
        return -1;
    }else if(compareResult > 0){
        return 1;
    }else{
        return 0;
    }//end if
}//end func

char* printField(void* toBePrinted){
    if(toBePrinted == NULL){
        return setString("");
    }//end if

    //cast the object
    Field* object = (Field*)toBePrinted;

    //check the if its empty
    if(object->value == NULL){
        object->value = setString("");
    }//end

    //return
    char* stringToPrint = calloc(1, sizeof(char)* (strlen(object->tag) + strlen(object->value) + 99) );
    sprintf(stringToPrint, "\t|%s| |%s|\n", object->tag, object->value);
    return stringToPrint;
}//end func
