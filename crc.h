#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

string crcmaker(string input, string key) {
    	int i,j,keylen,msglen;
	string finaldata;
    	keylen=key.length();
    	msglen=input.length();
	char temp[msglen+100],quot[msglen+100],rem[msglen],key1[keylen];
    	strcpy(key1,key.c_str());
    	for (i=0;i<keylen-1;i++) {
    		input[msglen+i]='0';
    	}
    	for (i=0;i<keylen;i++)
    	 temp[i]=input[i];
    	for (i=0;i<msglen;i++) {
    		quot[i]=temp[0];
    		if(quot[i]=='0')
    		 for (j=0;j<keylen;j++)
    		 key[j]='0'; else
    		 for (j=0;j<keylen;j++)
    		 key[j]=key1[j];
    		for (j=keylen-1;j>0;j--) {
    			if(temp[j]==key[j])
    			 rem[j-1]='0'; else
    			 rem[j-1]='1';
    		}
    		rem[keylen-1]=input[i+keylen];
    		strcpy(temp,rem);
    	}
    	strcpy(rem,temp);
  /*  	printf("\nQuotient is ");
    	for (i=0;i<msglen;i++)
    	 printf("%c",quot[i]);
    	printf("\nRemainder is ");*/
    	for (i=0;i<keylen-1;i++){
    	 //printf("%c",rem[i]);
	     finaldata+=rem[i];	
	}
 /*   	printf("\nFinal data is: ");
    	for (i=0;i<msglen;i++){
	 finaldata+=input[i];
    	 printf("%c",input[i]);}
    	for (i=0;i<keylen-1;i++){
	 finaldata+=rem[i];
    	 printf("%c",rem[i]);}
        cout<<"finaldata "<<finaldata<<endl;*/
   	return finaldata;
}
