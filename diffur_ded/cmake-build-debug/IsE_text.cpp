#include "IsE_text.h"


int getFileSize (const char* fileName)
{
    assert (fileName != NULL);

    struct stat buff = {};
    stat (fileName, &buff);

    return buff.st_size;
}

//----------------------------------------------------------------------

int readToStorage (const char* fileName, char** storage)
{
    assert (fileName != NULL);

    FILE* input = fopen (fileName, "rt");

    if (input == NULL)
       return NULL_ARGUMENT;

    if (storage == NULL)
        return NULL_ARGUMENT;


    int fileSz = getFileSize (fileName);

    *storage = (char*) calloc (fileSz + 2, sizeof (*storage));
    assert (*storage);

    int nCh = fread (*storage, sizeof(char), fileSz, input);
    *(*storage + nCh)     = '\n';
    *(*storage + nCh + 1) = '\0';

    fclose (input);

    return NO_ERRORS;
}

//------------------------------------------------------------------------

int countSymb (const char* storage, const char symb)
{

    assert (storage != NULL);

    int nStr = 0;

    //int ind = -1;

    char* pointerToSymb = (char*)storage - 1;
    while ( (pointerToSymb = strchr(pointerToSymb + 1, symb)) != NULL)
        nStr++;
    return nStr;

}

//-------------------------------------------------------------------------

int deleteDoubleSymb (const char* storage, const char symb)
{
    assert (storage != NULL);

    char* read  = (char*) storage;
    char* write = (char*) storage;
    int nSymb = 0;

    if (symb == '\0')
        while (*read != '\0')
            nSymb++;

    else

        while (*read != '\0')
        {
            while (*read == symb && *(read+1) == symb )
                read++;

            *write++ = *read++;
            nSymb++;
        }

        *write = '\0';

    return nSymb;
}

//------------------------------------------------------------------------

int substituteSymb (const char* storage, const char symbOld, const char symbNew)
{
    assert (storage != NULL);

    int nStr = 0;

    char* pointerToSymb = (char*) storage - 1;
    while ( (pointerToSymb = strchr(pointerToSymb + 1, symbOld)) != NULL)
    {
        nStr++;
        *pointerToSymb = symbNew;
    }

    return nStr;
}

//------------------------------------------------------------------------

int deleteSymb (const char* storage,const char symb)
{
    assert (storage);

    char* read  = (char*) storage;
    char* write = (char*) storage;
    int nSymb = 0;

    if (symb == '\0')
        while (*read != '\0')
            nSymb++;

    else

        while (*read != '\0')
        {
            while (*read == symb)
                read++;

            *write++ = *read++;
            nSymb++;
        }
        *write = '\0';

    return nSymb + 1;
}


//-------------------------------------------------------------------------

int strCompareStraight (const char* str1, const char* str2)
{
    assert (str1);
    assert (str2);

    const char* currSymbInStr1 = str1;
    const char* currSymbInStr2 = str2;

    while (*currSymbInStr1 == ' ' || ispunct (*currSymbInStr1) || (*currSymbInStr1 >= '0' && *currSymbInStr1 <= '9'))
        currSymbInStr1++;

    while (*currSymbInStr2 == ' ' || ispunct (*currSymbInStr2) || (*currSymbInStr2 >= '0' && *currSymbInStr2 <= '9'))
        currSymbInStr2++;

    while (1)
    {
        while (!(isalpha((int)(unsigned char)*(currSymbInStr1))) && (*(currSymbInStr1) != '\n') && !(isspace(*currSymbInStr1)))
            currSymbInStr1++;

        while (!(isalpha((int)(unsigned char)*(currSymbInStr2))) && (*(currSymbInStr2) != '\n') && !(isspace(*currSymbInStr2)))
            currSymbInStr2++;

        unsigned char tempSymb1 = (unsigned char) tolower (*currSymbInStr1);
        unsigned char tempSymb2 = (unsigned char) tolower (*currSymbInStr2);

        if (tempSymb1 == '\n' || tempSymb2 == '\n')
            return tempSymb1 - tempSymb2;

        if ((tempSymb1) == (tempSymb2))
        {
            currSymbInStr1++;
            currSymbInStr2++;
            continue;
        }

        return tempSymb1 - tempSymb2;
    }
}

//-------------------------------------------------------------------------------------

int strCompareReverse (const String* str1, const String* str2)
{

    assert (str1);
    assert (str2);

    int nCh1 = str1->length;
    int nCh2 = str2->length;

    const char* currSymbInStr1 = str1->chLine + nCh1 - 2;
    const char* currSymbInStr2 = str2->chLine + nCh2 - 2;

    while (*currSymbInStr1 == ' ' || ispunct (*currSymbInStr1) || (*currSymbInStr1 >= '0' && *currSymbInStr1 <= '9'))
        currSymbInStr1--;

    while (*currSymbInStr2 == ' ' || ispunct (*currSymbInStr2) || (*currSymbInStr2 >= '0' && *currSymbInStr2 <= '9'))
        currSymbInStr2--;

    while (1)
    {
        while (!(isalpha((int)(unsigned char)*(currSymbInStr1))) && (nCh1 > 1) && !(isspace(*currSymbInStr1)))
        {
            currSymbInStr1--;
            nCh1--;
        }

        while (!(isalpha((int)(unsigned char)*(currSymbInStr2))) && (nCh2 > 1) && !(isspace(*currSymbInStr2)))
        {
            currSymbInStr2--;
            nCh2--;
        }

        unsigned char tempSymb1 = (unsigned char) tolower (*currSymbInStr1);
        unsigned char tempSymb2 = (unsigned char) tolower (*currSymbInStr2);

        if ((nCh1 == 1) || (nCh2 == 1))
            return tempSymb1 - tempSymb2;

        if ((tempSymb1) == (tempSymb2))
        {
            currSymbInStr1--;
            currSymbInStr2--;
            continue;
        }

        return tempSymb1 - tempSymb2;
    }
}

//-----------------------------------------------------

int splitTextIntoLines (const char* storage, const int nStr, struct String** lines)
{
    assert (storage);
    assert (lines);

    char* read = (char*) storage;

    *lines = (struct String*) calloc (nStr, sizeof(**lines));
    assert (*lines);

    int strInd = 0;

    char* beginOfString = (char*) storage;
    char* endOfString   = (char*) storage;

    while (endOfString = strchr (beginOfString, '\n'))
    {
        (*lines + strInd)->chLine = beginOfString;
        (*lines + strInd)->length = endOfString - beginOfString + 1;
        strInd++;
        beginOfString = endOfString + 1;
    }

    return 0;
}

//---------------------------------------------------------------------------------------------

int straightComparator (const void* member1, const void* member2)
{
    assert (member1 != NULL);
    assert (member2 != NULL);

    return strCompareStraight (((struct String*) member1)->chLine, ((struct String*) member2)->chLine);
}

//----------------------------------------------------------------------------------------------

int reverseComporator  (const void* member1, const void* member2)
{
    assert (member1 != NULL);
    assert (member2 != NULL);

    return strCompareReverse ((struct String*) member1, (struct String*) member2);
}

//------------------------------------------------------------------------------------------------

void storageOutput (const char* storage, const size_t nCh, const char* fileName)
{
    assert (storage);
    assert (fileName);

    FILE* output = fopen (fileName, "wt");

    if (output == NULL)
    {
        printf ("[Error] Unable to write to file \"%s\" \n", fileName);
        exit (EXIT_FAILURE);
    }

    fwrite (storage, sizeof(char), nCh, output);

    fclose (output);
}

//--------------------------------------------------------------------------------------------------

void textOutputStraight (struct String* text, const size_t nStr, const char* fileName)
{
    assert (text);
    assert (fileName);

    FILE* output = fopen (fileName, "wb");

    if (output == NULL)
    {
        printf ("[Error] Unable to write to file \"%s\" \n", fileName);
        exit (EXIT_FAILURE);
    }

    for( int i = 0; i < nStr; i++)
    {
        char* temp = (text + i)->chLine;
        fwrite ((text + i)->chLine, sizeof(char), (text + i)->length, output);
    }
    fclose (output);
}

//---------------------------------------------------------------------------------------------------

int prepareStorage (const char* fileName, size_t* nCh, size_t* nStr, char** storage)
{
    assert (storage);
    assert (fileName);
    assert (nCh);
    assert (nStr);

    int errNum = readToStorage (fileName, storage);
    if (!errNum)
    {
        *nCh = deleteDoubleSymb (*storage, '\n');
        *nStr = countSymb (*storage, '\n');
    }

    return errNum;
}

//------------------------------------------------------------------------------------------------------

void testGetFileSize (const char* fileName, const int exampleSize, int testN)
{
    int Sz = getFileSize (fileName);
    (exampleSize == Sz) ? :
        printf ("test number %d:read of file failed\n", testN);
}

//----------------------------------------------------------------------

void sw (struct String* text, int l, int r) {

    assert (text);
    struct String t = text[l];
    text[l] = text[r];
    text[r] = t;
}

//-----------------------------------------------------------------------

void quickSort(String v[], int left, int right, int (*compare) (const void *, const void *))
{
    assert (v);
    assert (compare);

    if (left >= right)
        return;

    sw (v, left, (left+right)/2);
    int last = left;

    for(int i = left + 1; i <= right; i++)

        if (compare(v + i, v + left) < 0)
            sw (v, ++last, i);

    sw (v, left, last);

    quickSort (v, left    , last - 1, compare);
    quickSort (v, last + 1, right   , compare);
}

//----------------------------------------------------------------------------------------

void testReadToStorage (const char* fileName, const char* exampleStr, int nTest)
{
    char* storage = "";
    readToStorage (fileName, &storage);

    (!strcmp(storage, exampleStr)) ? :
        printf ("reading failed test number: %d\n", nTest);
}

//-----------------------------------------------------------------------------------------

void testStrCompareStraight (const char* str1, const char* str2, int res, int nTest)
{
    int res1 = strCompareStraight (str1, str2);
    if (res1 != 0)
        res1 = res1 / abs (res1);
    (!(res1 - res) ) ? :
        printf ("compairing failed test number: %d\n", nTest);
}

//-------------------------------------------------------------------------------------------

void testStrCompareReverse (const struct String* str1, const struct String* str2, int res, int nTest)
{
    int res1 = strCompareReverse(str1, str2);
    if (res1 != 0)
        res1 = res1 / abs (res1);
    (!(res1 - res) ) ? :
        printf ("compairing failed test number: %d\n", nTest);
}

//-------------------------------------------------------------------------------------------------

void testQuickSort (struct String* text, int beg, int en, int (*compare) (const void *, const void *), struct String* exampleText, int nTest)
{
    quickSort (text, beg, en, compare);
    int i = beg;

    while (compare(text + i,exampleText + i))
        i++;

    (!(i-en - 1) ) ? :
        printf ("sorting failed test number: %d\n", nTest);
}

//-----------------------------------------------------------------------------------------------

void getFileNames (int args, char* argv[], FileNames* filenames)
{
    switch (args)
    {
        case 5:
            filenames->fstorage = argv[4];

        case 4:
            filenames->froutput = argv[3];

        case 3:
            filenames->fsoutput = argv[2];

        case 2:
            filenames->finput   = argv[1];
    }
}

//-------------------------------------------------------------------------------

void tests()
{
    testGetFileSize("ex1.txt", 3 , 1);
    testGetFileSize("ex2.txt", 7 , 2);
    testGetFileSize("ex3.txt", 10, 3);

    testReadToStorage("ex1.txt", "aaa\n", 4);
    testReadToStorage("ex2.txt", "aaa\naa\n", 5);

    testStrCompareStraight("aaaa", "abaa", -1, 6);
    testStrCompareStraight("aaa a c", "aaaa", -1, 7);
}

//---------------------------------------------------------------------------

void FileNamesDestruct (FileNames* files)
{
    free(files->finput);
    free(files->fsoutput);
    free(files->froutput);
    free(files->fstorage);

    files->finput = NULL;
    files->fsoutput = NULL;
    files->froutput = NULL;
    files->fstorage = NULL;

}
