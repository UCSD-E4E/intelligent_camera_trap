/*
 * write.h
 *
 * Created: 12/11/2012 11:03:59 PM
 *  Author: Samuel
 */ 

#ifndef MESSAGE_PRINT_H_
#define MESSAGE_PRINT_H_

extern unsigned char Length(char Temp[80]);
extern void Print(char Text[80]);
extern void PrintLn(char Text[80]);
extern void PrintLine();
extern void Print_Welcome();
extern void Print_Error();

extern char printTxt[80];

#endif /* MESSAGE_PRINT_H_ */
