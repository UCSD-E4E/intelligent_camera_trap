/*
 * write.h
 *
 * Created: 12/11/2012 11:03:59 PM
 *  Author: Samuel
 */ 

#ifndef MESSAGE_PRINT_H_
#define MESSAGE_PRINT_H_

extern unsigned char Length(char Temp[40]);
extern void Print(char Text[40]);
extern void PrintLn(char Text[40]);
extern void PrintLine();
extern void Print_Welcome();
extern void Print_Error();

extern char printTxt[40];

#endif /* MESSAGE_PRINT_H_ */
