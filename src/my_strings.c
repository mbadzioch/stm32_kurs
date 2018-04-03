#include "my_strings.h"
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------

int my_strcmp (char* str1 , char* str2) { // compare two strings 
    unsigned int t_i = 0;
    int cmp_ok = 1;
    while  ( (*(str1+t_i) != 0)  && (*(str2+t_i) != 0) ) {
        if (*(str1+t_i)  != *(str2+t_i))  cmp_ok = 0;
        t_i ++;
    }
    if (t_i == 0) cmp_ok = 0;
    if (*(str2+t_i) != 0 )  cmp_ok = 0;
    return (cmp_ok);
}

//----------------------------------------------------------------------------------------------------------------------


void my_strcut (char* str_in , char delimiter , unsigned int delim_num , char* str_out , int max_len ) { // cut substring using given delimiter and given delimiter number
    unsigned int t_i = 0 , t_i2 = 0 , delim_count = 0;
    while ( *(str_in + t_i) != 0 )  {
        if ( ( delim_count == delim_num ) && ( *(str_in + t_i) != delimiter) ) {
            *(str_out + t_i2) = *(str_in + t_i);
            t_i2 ++;
            *(str_out + t_i2) = 0;
            if (t_i2 >= max_len - 1) break;
        }
        if (*(str_in + t_i)  == delimiter ) delim_count ++;
        if (delim_count > delim_num) return;
        t_i ++;
    } 
}

//----------------------------------------------------------------------------------------------------------------------

void my_strcat (char* str_1 , char* str_2 , char* str_out , int max_len ) { // concatenate two strings
    unsigned int t_i = 0 , t_i2 = 0;
    while (*(str_1 + t_i) != 0)  {
	    *(str_out + t_i2) =  *(str_1 + t_i);
	    t_i++;
	    t_i2++;
	    *(str_out + t_i2) = 0;
            if (t_i2 >= max_len - 1) return;
    }
    
    t_i = 0;
    while (*(str_2 + t_i) != 0)  {
	    *(str_out + t_i2) =  *(str_2 + t_i);
	    t_i++;
	    t_i2++;
	    *(str_out + t_i2) = 0;
            if (t_i2 >= max_len - 1) return;
    }

}

//----------------------------------------------------------------------------------------------------------------------

void str_append (char* str_out , char* str_in , int max_len) {
    unsigned int t_i = 0 ,t_i2 = 0;
    while (*(str_out + t_i) != 0) t_i ++;
    while (*(str_in + t_i2) != 0) {
	*(str_out + t_i) = *(str_in + t_i2);
	t_i ++;
	t_i2 ++;
        if (t_i >= max_len - 1) break;
    }
    *(str_out + t_i) = 0;
}

//----------------------------------------------------------------------------------------------------------------------

void my_itoa (int i , char* str_out ) {  // convert signed int to string
    unsigned int t_i = 0 , u_i , u_i2 , mask; 
    if (i < 0) {
	*(str_out + t_i) = '-';
	t_i ++;
	u_i = - i;
    } else u_i = i;
    u_i2 = u_i; 
    
    for (mask = 1000000000 ; mask > 0 ; mask /= 10) {
	if (u_i >= mask) {
	    *(str_out + t_i) = '0' + u_i2 / mask;
	    t_i++;
	}
	u_i2 = u_i2 % mask;
    }
    
    if (u_i == 0) {
	    *(str_out + t_i) = '0';
	    t_i++;
    }
    *(str_out + t_i) = 0;
}


//----------------------------------------------------------------------------------------------------------------------

uint32_t my_atoi (char* str_in ) {
    uint32_t t_i = 0, t_i2 = 0 , sign = 0;
    if ( *str_in == '-' ) {t_i2 ++; sign = 1; }
    while ((*(str_in + t_i2) >= '0') && (*(str_in + t_i2) <= '9')) {
	t_i = t_i * 10 + *(str_in + t_i2) - '0';
	t_i2++;
    }
    
    if (sign) return (-t_i) ; else return (t_i);
}

//----------------------------------------------------------------------------------------------------------------------
