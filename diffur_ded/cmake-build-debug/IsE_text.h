#ifndef ISE_TEXT_H_INCLUDED
#define ISE_TEXT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys\stat.h>
#include <ctype.h>

const char INPU[]    = "input.txt";
const char SOUTPU[]  = "straight_output.txt";
const char ROUTPU[]  = "reverse_output.txt";
const char FULLTEX[] = "full_text.txt";

struct String
{
    char* chLine;
    int length;
};

struct FileNames
{
    char* finput   = (char*) INPU;
    char* fsoutput = (char*) SOUTPU;
    char* froutput = (char*) ROUTPU;
    char* fstorage = (char*) FULLTEX;
};

enum StringErrors
{
    NO_ERRORS = 0,
    NULL_ARGUMENT = 1,

};

//!---------------------------------------------------
//! gets size of file in bytes
//!
//! @param [in] fileName-name of file or filepath
//!
//! @return size of file in bytes
//!
//! @note size of file in bytes often is bigger than
//!       number of char symbols in file
//!----------------------------------------------------

int getFileSize (const char* fileName);

//!-------------------------------------------------------
//!         reads data from file to storage
//!
//! @param  [in] fileName-name of file or filepath
//! @param  [in] storage-buffer of char
//!
//! @return error code
//!
//! @note   returns string that is not normal for c
//!         format of string is
//!-------------------------------------------------------

int readToStorage (const char* fileName, char** storage);

//!---------------------------------------------------------------------------
//!         counts number of input symbols in input string
//!
//! @param  [in] storage-pointer to buffer consisting string
//! @param  [in] symb-symbol which we find
//!
//! @return number of string
//!
//!---------------------------------------------------------------------------

int countSymb (const char* storage, const char symb);

//!---------------------------------------------------------------------------
//!         deletes every double symbol from input and returns size of new storage without double symb;
//!
//! @param  [in] storage-buffer consisting string
//! @param  [in] symb-symbol which we want to delete
//!
//! @return size of new storage without double symb;
//!
//!---------------------------------------------------------------------------

int deleteDoubleSymb (const char* storage, const char symb);

//!---------------------------------------------------------------------------
//!         deletes every double symbol from input and returns size of new storage without double symb;
//!
//! @param  [in] storage-buffer consisting string
//! @param  [in] symb-symbol which we want to delete
//!
//! @return size of new storage without double symb;
//!
//!---------------------------------------------------------------------------

int deleteSymb (const char* storage, const char symb);

//!---------------------------------------------------------------------------
//!         deletes every double symbol from input and returns size of new storage without double symb;
//!
//! @param  [in] storage-buffer consisting string
//! @param  [in] symbOld-symbol which we want to substitute
//! @param  [in] symbNew-symbol which we want to add
//!
//! @return number of sustituted symbols;
//!
//!---------------------------------------------------------------------------

int substituteSymb (const char* storage, const char symbOld, const char symbNew);

//!--------------------------------------------------------------------------
//!         converts sting to array of Strings
//!
//! @param  [in] storage-buffer consisting string
//! @param  [in] nStr-number of string
//! @param  [out] lines-array of lines
//!
//! @return error code
//!
//!---------------------------------------------------------------------------

int splitTextIntoLines (const char* storage, const int nStr, struct String** lines);

//!---------------------------------------------------------------------------
//!         compares strings from beginning
//!
//! @param  [in] str1 first string
//! @param  [in] str2 second string
//!
//! @return negative number if str1 <  str2
//!         positive number if str1 >  str2
//!         zero            if str1 == str2
//!---------------------------------------------------------------------------

int strCompareStraight (const char* str1, const char* str2);

//!---------------------------------------------------------------------------
//!         compares strings from ending
//!
//! @param  [in] str1 first string
//! @param  [in] str2 second string
//!
//! @return negative number if str1 <  str2
//!         positive number if str1 >  str2
//!         zero            if str1 == str2
//!---------------------------------------------------------------------------

int strCompareReverse (const struct String* str1, const struct String* str2);

//!---------------------------------------------------------------------------
//!         comparator for sorting using strCompareStraight
//!
//! @param  [in] first  member for comparing
//! @param  [in] second member for comparing
//!
//! @return negative number if str1 <  str2
//!         positive number if str1 >  str2
//!         zero            if str1 == str2
//!----------------------------------------------------------------------------

int straightComparator (const void* member1, const void* member2);

//!---------------------------------------------------------------------------
//!         comparator for sorting using strCompareReverse
//!
//! @param  [in] first  member for comparing
//! @param  [in] second member for comparing
//!
//! @return negative number if str1 <  str2
//!         positive number if str1 >  str2
//!         zero            if str1 == str2
//!---------------------------------------------------------------------------

int reverseComporator  (const void* member1, const void* member2);

//!--------------------------------------------------------------------------
//!        writes text from storage to file
//!
//! @param [in] storage buffer with text
//! @param [in] nStr number of strings in buffer
//! @param [in] fileName name of file where text will be written
//!
//!--------------------------------------------------------------------------

void storageOutput (const char* storage, const size_t nCh, const char* fileName);

//!--------------------------------------------------------------------------
//!        writes text from text to file from beginning of every string
//!
//! @param [in] text array of strings
//! @param [in] nCh number of strings in buffer
//! @param [in] fileName name of file where text will be written
//!
//!--------------------------------------------------------------------------

void textOutputStraight (struct String* text,const size_t nStr, const char* fileName);

//!--------------------------------------------------------------------------
//!        writes text from text to file from ending of every string
//!
//! @param [in] text array of strings
//! @param [in] nStr number of strings in buffer
//! @param [in] fileName name of file where text will be written
//!
//!--------------------------------------------------------------------------

void textOutputReverse  (struct String* text,const int nStr, const char* fileName);

//!-----------------------------------------------------------------------------------
//!         reads string from file and deletes every '\r' and double '\n' from it
//!
//! @param  [in] fileName-name of file  from which we read
//! @param  [out] nCh-number of characters
//! @param  [out] nStr-number of lines
//! @param  [out] storage-initial text
//!
//! @return error code
//!------------------------------------------------------------------------------------

int prepareStorage (const char* fileName, size_t* nCh, size_t* nStr, char** storage);

//!-----------------------------------------------------------------------------------------
//!        sorts array of Strings
//! @param [in] text-pointer to string array
//! @param [in] beg-beginning index
//! @param [in] en-ending index
//!-----------------------------------------------------------------------------------------------

void quickSort (struct String* text, int beg, int en, int (*compare) (const void *, const void *));

//!-------------------------------------------------------------------------------------
//!        swaps strings
//! @param [in] text-array of Strings
//! @param [in] l-left index
//! @param [in] f-right index
//!--------------------------------------------------------------------------------------

void sw (struct String* text, int l, int r);

//!--------------------------------------------------------------------------------------
//!        gets strings which are filenames
//!
//! @param [in] args - number of arguments
//! @param [in] argv - array of strings
//! @param [in] filenames - pointer to structure with filenames
//!
//!---------------------------------------------------------------------------------------

void getFileNames (int args, char* argv[], FileNames* filenames);

//!-----------------------------------------------------------------------------------------
//!        destructs fileNames
//!
//! @param [in] filenames-what we want to destruct
//!-----------------------------------------------------------------------------------------

void FileNamesDestruct (FileNames* filenames);

int myisalpha (char symb);

//!-----------------------------------------------
//! unit tests
//!-----------------------------------------------

void testGetFileSize (const char* fileName, const int exampleSize, int testN);
void testReadToStorage (const char* fileName, const char* exampleStr, int nTest);
void testStrCompareStraight (const char* str1, const char* str2, int res, int nTest);
void testStrCompareReverse (const struct String* str1, const struct String* str2, int res, int nTest);
void testQuickSort (struct String* text, int beg, int en, int (*compare) (const void *, const void *), struct String* example, int nTest);
void tests();

#endif // ISE_TEXT_H_INCLUDED

