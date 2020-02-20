#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <iostream>
#include <random>
#include <map>
#include <iterator>
#include "crc.h"
 
using namespace std;

#define M 8
vector<bitset<8> > bits;
vector<string> stringbit;
vector<string> stringbit2;
vector<string> stringbit3;
vector<string> stringbit4;
string serialstr;
string serialstr2;
string serialstr3;
string serialstr4;
string recvstr;
string recvstr2;
string recvstr3;
string recvstr4;
vector<string> recvbit;
vector<string> recvbit2;
vector<string> recvbit3;
vector<string> recvbit4;
string r1,r2,r3,r4,r5,r6;

vector<string> strtoint(string str){
vector<string> strbit;
	for(size_t i=0;i<str.size();i++){
		bitset<8> b(str.c_str()[i]);
		strbit.push_back(b.to_string());
	}
return strbit;
}

string padding(string str, int m){
	int strsz=str.length();
	if(strsz%m != 0){
		while (strsz%m != 0){
			str.resize(strsz+1,'~');
			strsz=str.length();
		}
	}
    return str;
}

string randomGen(char s,double p){
	string ranNum;
	random_device rd;
    	mt19937 gen(rd());
    	double weights[2];
	if(s=='1'){
    		weights[0] = {p};
    		weights[1]={1-p};
	}
	if(s=='0'){
    		weights[0] = {1-p};
    		weights[1]={p};
	}
    	discrete_distribution<> d(std::begin(weights), std::end(weights));
	ranNum=to_string(d(gen));

	return ranNum;
}

string bittostr(string data){
    	stringstream sstream(data);
    	string output;
    	while(sstream.good())
    	{
        	bitset<8> bits;
        	sstream >> bits;
        	char c = char(bits.to_ulong());
        	output += c;
    	}
	return output;
}

int main(){
	string str;
	int m;
	double p;
	int poly;
	cout<<"enter data string:";
	getline(cin,str);
	cout<<"enter number of data bytes in a row (m):";
	cin>>m;
	cout<<"enter probability(p):";
	cin>>p;
	cout<<"enter generator polynomial:";
	cin>>poly;
	str=padding(str,m);
	cout<<"\n\ndata string after padding: "<<str<<endl;
	for(size_t i=0;i<str.size();++i){
		bitset<M> bitval(str.c_str()[i]);
		bits.push_back(bitval);
        }
	cout<<"\ndata block (ascii code of m characters per row):"<<endl;
	if(m==1){
		stringbit.clear();
		for(int i=0;i<bits.size();i++){
			stringbit.push_back(bits[i].to_string());
			cout<<bits[i]<<endl;
		}
		cout<<"\ndata block after adding check bits:"<<endl;		
		for(int i=0;i<stringbit.size();i++){
			if((stringbit[i].at(0)-'0'+stringbit[i].at(2)-'0'+stringbit[i].at(4)-'0'+stringbit[i].at(6)-'0')%2==0){
				stringbit[i].insert(0,"0");
			}
			else {
				stringbit[i].insert(0,"1");
			}
			if((stringbit[i].at(1)-'0'+stringbit[i].at(2)-'0'+stringbit[i].at(5)-'0'+stringbit[i].at(6)-'0')%2==0){
				stringbit[i].insert(1,"0");
			}
			else {
				stringbit[i].insert(1,"1");
			}
			if((stringbit[i].at(4)-'0'+stringbit[i].at(5)-'0'+stringbit[i].at(6)-'0'+stringbit[i].at(7)-'0')%2==0){
				stringbit[i].insert(3,"0");
			}
			else {
				stringbit[i].insert(3,"1");
			}
			if((stringbit[i].at(10)-'0')%2==0){
				stringbit[i].insert(7,"0");
			}
			else {
				stringbit[i].insert(7,"1");
			}
			cout<<endl;
			for(int j=0;j<12;j++){
				if(j==0 || j==1 || j==3 || j==7){
					cout << "\033[1;32m"<<stringbit[i].at(j)<<"\033[0m";
				}
				else {
					cout<<stringbit[i].at(j);
				}
			}	
		}
		cout<<"\ndata bit after column wise serialization:"<<endl;
		for(int j=0;j<12;j++){
			for(int i=0;i<stringbit.size();i++){
				//cout<<stringbit[i].at(j);
				serialstr+=stringbit[i].at(j);
			}
		}
		cout<<serialstr<<endl;
		cout<<"\ndata bit after appending CRC checksum (sent frame):"<<endl;
		
		string result=crcmaker(serialstr,to_string(poly));
		cout<<serialstr;
		for(int i=0;i<result.size();i++){
			cout << "\033[1;36m"<<result[i]<<"\033[0m";
		}
		cout<<endl;
		serialstr=serialstr+result;
		cout<<"received frame:"<<endl;
		for(int i=0;i<serialstr.size();i++){
			//cout<<serialstr[i]<<endl;
			string bit=randomGen(serialstr[i],p);
			recvstr.insert(i,bit);
		}
		
		for(int i=0;i<recvstr.size();i++){
			if(recvstr[i] != serialstr[i]){
				cout << "\033[1;31m"<<recvstr[i]<<"\033[0m";				
			}
			else {
				cout<<recvstr[i];
			}
		}
		cout<<"\nresult of CRC checksum matching:";
		string rem=crcmaker(recvstr,to_string(poly));
		if(atoi(rem.c_str()) == 0){
			cout<<" no error detected"<<endl;
		}
		else {
			cout<<" error detected"<<endl;
		}
		cout<<"\ndata block after removing CRC checksum bits:"<<endl;
		recvstr.erase(recvstr.size()-result.size(),result.size());
		char temparr[stringbit.size()][12];int r=0;
		for(int j=0;j<12;j++){
			for(int i=0;i<stringbit.size();i++,r++){
				temparr[i][j]=recvstr[r];
			}
		}
		for(int i=0;i<stringbit.size();i++){
			string arr;
			for(int j=0;j<12;j++){
				arr+=temparr[i][j];
			}
			recvbit.push_back(arr);
			arr.clear();
		}
		int count[recvbit.size()]={0};
		for(int i=0;i<recvbit.size();i++){
			//cout<<recvbit[i]<<endl;
			for(int j=0;j<12;j++){
				if(recvbit[i].at(j) != stringbit[i].at(j)){
					cout << "\033[1;31m"<<recvbit[i].at(j)<<"\033[0m";
					count[i]++;
				}
				else {
					cout<<recvbit[i].at(j);
				}
			}
			cout<<endl;
		}
		for(int i=0;i<recvbit.size();i++){
			if(count[i]==1)
			{
				if((recvbit[i].at(0)-'0'+recvbit[i].at(2)-'0'+recvbit[i].at(4)-'0'+recvbit[i].at(6)-'0')%2==0){
					r1="0";
				}
				else {
					r1="1";
				}
				if((recvbit[i].at(1)-'0'+recvbit[i].at(2)-'0'+recvbit[i].at(5)-'0'+recvbit[i].at(6)-'0')%2==0){
					r2="0";
				}
				else {
					r2="1";
				}
				if((recvbit[i].at(4)-'0'+recvbit[i].at(5)-'0'+recvbit[i].at(6)-'0'+recvbit[i].at(7)-'0')%2==0){
					r3="0";
				}
				else {
					r3="1";
				}
				if((recvbit[i].at(10)-'0')%2==0){
					r4="0";
				}
				else {
					r4="1";
				}
			string num=r1+r2+r3+r4;
			char * ptr;
    			long parsed = strtol(num.c_str(), & ptr, 2);
			if(parsed<=recvbit2.size()){
			if(recvbit[i].at(parsed-1)==0){
				recvbit[i].replace(parsed-1,1,"1");
			}
			else {
				recvbit[i].replace(parsed-1,1,"0");
			}
			}}
		}
		cout<<"\ndata block after removing check bits:"<<endl;
		for(int i=0;i<recvbit.size();i++){
			recvbit[i].erase(7,1);
			recvbit[i].erase(3,1);
			recvbit[i].erase(1,1);
			recvbit[i].erase(0,1);
		}
		string frame;
		for(int i=0;i<recvbit.size();i++){
			cout<<recvbit[i]<<endl;
			frame+=recvbit[i];
		}
		cout<<"\noutput frame: ";

		cout<<bittostr(frame)<<endl;
	}
	if(m==2){
		stringbit2.clear();
		for(int i=0;i<bits.size();i=i+m){
			string result=bits[i].to_string();
			string result2=bits[i+1].to_string();
			string cat=result+result2;
			stringbit2.push_back(cat);
			//cout<<"result "<<cat<<endl;
			cout<<bits[i]<<bits[i+1]<<endl;
		}
		cout<<"\ndata block after adding check bits:"<<endl;
		for(int i=0;i<stringbit2.size();i++){
			if((stringbit2[i].at(0)-'0'+stringbit2[i].at(2)-'0'+stringbit2[i].at(4)-'0'+stringbit2[i].at(6)-'0'+
stringbit2[i].at(8)-'0'+stringbit2[i].at(10)-'0'+stringbit2[i].at(12)-'0'+stringbit2[i].at(14)-'0')%2==0){
				stringbit2[i].insert(0,"0");
			}
			else {
				stringbit2[i].insert(0,"1");
			}
			if((stringbit2[i].at(1)-'0'+stringbit2[i].at(2)-'0'+stringbit2[i].at(5)-'0'+stringbit2[i].at(6)-'0'
+stringbit2[i].at(9)-'0'+stringbit2[i].at(10)-'0'+stringbit2[i].at(13)-'0'+stringbit2[i].at(14)-'0')%2==0){
				stringbit2[i].insert(1,"0");
			}
			else {
				stringbit2[i].insert(1,"1");
			}
			if((stringbit2[i].at(3)-'0'+stringbit2[i].at(4)-'0'+stringbit2[i].at(5)-'0'+stringbit2[i].at(6)-'0'
+stringbit2[i].at(11)-'0'+stringbit2[i].at(12)-'0'+stringbit2[i].at(13)-'0'+stringbit2[i].at(14)-'0')%2==0){
				stringbit2[i].insert(3,"0");
			}
			else {
				stringbit2[i].insert(3,"1");
			}
			if((stringbit2[i].at(7)-'0'+stringbit2[i].at(8)-'0'+stringbit2[i].at(9)-'0'+stringbit2[i].at(10)-'0'
+stringbit2[i].at(11)-'0'+stringbit2[i].at(12)-'0'+stringbit2[i].at(13)-'0'+stringbit2[i].at(14)-'0')%2==0){
				stringbit2[i].insert(7,"0");
			}
			else {
				stringbit2[i].insert(7,"1");
			}
			if((stringbit2[i].at(15)-'0'+stringbit2[i].at(16)-'0'+stringbit2[i].at(17)-'0'+stringbit2[i].at(18)-'0'
+stringbit2[i].at(19)-'0')%2==0){
				stringbit2[i].insert(15,"0");
			}
			else {
				stringbit2[i].insert(15,"1");
			}
			cout<<endl;
			for(int j=0;j<21;j++){
				if(j==0 || j==1 || j==3 || j==7 || j==15){
					cout << "\033[1;32m"<<stringbit2[i].at(j)<<"\033[0m";
				}
				else {
					cout<<stringbit2[i].at(j);
				}
			}		
		}
		cout<<"\ndata bit after column wise serialization:"<<endl;
		for(int j=0;j<21;j++){
			for(int i=0;i<stringbit2.size();i++){
				//cout<<stringbit2[i].at(j);
				serialstr2+=stringbit2[i].at(j);
			}
		}
		cout<<serialstr2<<endl;
		cout<<"\ndata bit after appending CRC checksum (sent frame):"<<endl;
		string result=crcmaker(serialstr2,to_string(poly));
		cout<<serialstr2;
		for(int i=0;i<result.size();i++){
			cout << "\033[1;36m"<<result[i]<<"\033[0m";
		}
		cout<<endl;
		serialstr2=serialstr2+result;
		cout<<"received frame:"<<endl;
		for(int i=0;i<serialstr2.size();i++){
			//cout<<serialstr2[i]<<endl;
			string bit=randomGen(serialstr2[i],p);
			recvstr2.insert(i,bit);
		}

		for(int i=0;i<recvstr2.size();i++){
			if(recvstr2[i] != serialstr2[i]){
				cout << "\033[1;31m"<<recvstr2[i]<<"\033[0m";				
			}
			else {
				cout<<recvstr2[i];
			}
		}
		cout<<"\nresult of CRC checksum matching:";
		string rem=crcmaker(recvstr2,to_string(poly));
		if(atoi(rem.c_str()) == 0){
			cout<<" no error detected"<<endl;
		}
		else {
			cout<<" error detected"<<endl;
		}
		cout<<"\ndata block after removing CRC checksum bits:"<<endl;
		recvstr2.erase(recvstr2.size()-result.size(),result.size());
		char temparr[stringbit2.size()][21];int r=0;
		for(int j=0;j<21;j++){
			for(int i=0;i<stringbit2.size();i++,r++){
				temparr[i][j]=recvstr2[r];
			}
		}
		for(int i=0;i<stringbit2.size();i++){
			string arr;
			for(int j=0;j<21;j++){
				arr+=temparr[i][j];
			}
			recvbit2.push_back(arr);
			arr.clear();
		}
		int count[recvbit2.size()]={0};
		for(int i=0;i<recvbit2.size();i++){
			//cout<<recvbit[i]<<endl;
			for(int j=0;j<21;j++){
				if(recvbit2[i].at(j) != stringbit2[i].at(j)){
					cout << "\033[1;31m"<<recvbit2[i].at(j)<<"\033[0m";
					count[i]++;
				}
				else {
					cout<<recvbit2[i].at(j);
				}
			}
			cout<<endl;
		}
		for(int i=0;i<recvbit2.size();i++){
			if(count[i]==1)
			{
				if((recvbit2[i].at(0)-'0'+recvbit2[i].at(2)-'0'+recvbit2[i].at(4)-'0'+recvbit2[i].at(6)-'0'+
recvbit2[i].at(8)-'0'+recvbit2[i].at(10)-'0'+recvbit2[i].at(12)-'0'+recvbit2[i].at(14)-'0')%2==0){
					r1="0";
				}
				else {
					r1="1";
				}
				if((recvbit2[i].at(1)-'0'+recvbit2[i].at(2)-'0'+recvbit2[i].at(5)-'0'+recvbit2[i].at(6)-'0'
+recvbit2[i].at(9)-'0'+recvbit2[i].at(10)-'0'+recvbit2[i].at(13)-'0'+recvbit2[i].at(14)-'0')%2==0){
					r2="0";
				}
				else {
					r2="1";
				}
				if((recvbit2[i].at(3)-'0'+recvbit2[i].at(4)-'0'+recvbit2[i].at(5)-'0'+recvbit2[i].at(6)-'0'
+recvbit2[i].at(11)-'0'+recvbit2[i].at(12)-'0'+recvbit2[i].at(13)-'0'+recvbit2[i].at(14)-'0')%2==0){
					r3="0";
				}
				else {
					r3="1";
				}
				if((recvbit2[i].at(7)-'0'+recvbit2[i].at(8)-'0'+recvbit2[i].at(9)-'0'+recvbit2[i].at(10)-'0'
+recvbit2[i].at(11)-'0'+recvbit2[i].at(12)-'0'+recvbit2[i].at(13)-'0'+recvbit2[i].at(14)-'0')%2==0){
					r4="0";
				}
				else {
					r4="1";
				}
			        if((recvbit2[i].at(15)-'0'+recvbit2[i].at(16)-'0'+recvbit2[i].at(17)-'0'+recvbit2[i].at(18)-'0'
+recvbit2[i].at(19)-'0')%2==0){
					r5="0";
			}
			else {
					r5="1";
			}
			string num=r1+r2+r3+r4+r5;
			char * ptr;
    			long parsed = strtol(num.c_str(), & ptr, 2);
			if(parsed<=recvbit2.size()){
			if(recvbit2[i].at(parsed-1)==0){
				recvbit2[i].replace(parsed-1,1,"1");
			}
			else {
				recvbit2[i].replace(parsed-1,1,"0");
			}
			}}
		}

		cout<<"\ndata block after removing check bits:"<<endl;
		for(int i=0;i<recvbit2.size();i++){
			recvbit2[i].erase(15,1);
			recvbit2[i].erase(7,1);
			recvbit2[i].erase(3,1);
			recvbit2[i].erase(1,1);
			recvbit2[i].erase(0,1);
		}
		string frame;
		for(int i=0;i<recvbit2.size();i++){
			cout<<recvbit2[i]<<endl;
			frame+=recvbit2[i];
		}
		cout<<"\noutput frame: ";

		cout<<bittostr(frame)<<endl;
	}
	if(m==3){
		stringbit3.clear();
		for(int i=0;i<bits.size();i=i+m){
			string result=bits[i].to_string();
			string result2=bits[i+1].to_string();
			string result3=bits[i+2].to_string();
			string cat=result+result2+result3;
			stringbit3.push_back(cat);
			//cout<<"result "<<cat<<endl;
			cout<<bits[i]<<bits[i+1]<<bits[i+2]<<endl;
		}
		cout<<"\ndata block after adding check bits:"<<endl;
		for(int i=0;i<stringbit3.size();i++){
			if((stringbit3[i].at(0)-'0'+stringbit3[i].at(2)-'0'+stringbit3[i].at(4)-'0'+stringbit3[i].at(6)-'0'+
stringbit3[i].at(8)-'0'+stringbit3[i].at(10)-'0'+stringbit3[i].at(12)-'0'+stringbit3[i].at(14)-'0'+stringbit3[i].at(16)-'0'+
stringbit3[i].at(18)-'0'+stringbit3[i].at(20)-'0'+stringbit3[i].at(22)-'0')%2==0){
				stringbit3[i].insert(0,"0");
			}
			else {
				stringbit3[i].insert(0,"1");
			}
			if((stringbit3[i].at(1)-'0'+stringbit3[i].at(2)-'0'+stringbit3[i].at(5)-'0'+stringbit3[i].at(6)-'0'
+stringbit3[i].at(9)-'0'+stringbit3[i].at(10)-'0'+stringbit3[i].at(13)-'0'+stringbit3[i].at(14)-'0'+stringbit3[i].at(17)-'0'
+stringbit3[i].at(18)-'0'+stringbit3[i].at(21)-'0'+stringbit3[i].at(22)-'0')%2==0){
				stringbit3[i].insert(1,"0");
			}
			else {
				stringbit3[i].insert(1,"1");
			}
			if((stringbit3[i].at(3)-'0'+stringbit3[i].at(4)-'0'+stringbit3[i].at(5)-'0'+stringbit3[i].at(6)-'0'
+stringbit3[i].at(11)-'0'+stringbit3[i].at(12)-'0'+stringbit3[i].at(13)-'0'+stringbit3[i].at(14)-'0'+stringbit3[i].at(19)-'0'
+stringbit3[i].at(20)-'0'+stringbit3[i].at(21)-'0'+stringbit3[i].at(22)-'0')%2==0){
				stringbit3[i].insert(3,"0");
			}
			else {
				stringbit3[i].insert(3,"1");
			}
			if((stringbit3[i].at(7)-'0'+stringbit3[i].at(8)-'0'+stringbit3[i].at(9)-'0'+stringbit3[i].at(10)-'0'
+stringbit3[i].at(11)-'0'+stringbit3[i].at(12)-'0'+stringbit3[i].at(13)-'0'+stringbit3[i].at(14)-'0'+stringbit3[i].at(23)-'0')%2==0){
				stringbit3[i].insert(7,"0");
			}
			else {
				stringbit3[i].insert(7,"1");
			}
			if((stringbit3[i].at(15)-'0'+stringbit3[i].at(16)-'0'+stringbit3[i].at(17)-'0'+stringbit3[i].at(18)-'0'
+stringbit3[i].at(19)-'0'+stringbit3[i].at(20)-'0'+stringbit3[i].at(21)-'0'+stringbit3[i].at(22)-'0'+stringbit3[i].at(23)-'0')%2==0){
				stringbit3[i].insert(15,"0");
			}
			else {
				stringbit3[i].insert(15,"1");
			}
			cout<<endl;
			for(int j=0;j<29;j++){
				if(j==0 || j==1 || j==3 || j==7 || j==15){
					cout << "\033[1;32m"<<stringbit3[i].at(j)<<"\033[0m";
				}
				else {
					cout<<stringbit3[i].at(j);
				}
			}
		}
		cout<<"\ndata bit after column wise serialization:"<<endl;
		for(int j=0;j<29;j++){
			for(int i=0;i<stringbit3.size();i++){
				//cout<<stringbit3[i].at(j);
				serialstr3+=stringbit3[i].at(j);
			}
		}
		cout<<serialstr3<<endl;
		cout<<"\ndata bit after appending CRC checksum (sent frame):"<<endl;
		string result=crcmaker(serialstr3,to_string(poly));
		cout<<serialstr3;
		for(int i=0;i<result.size();i++){
			cout << "\033[1;36m"<<result[i]<<"\033[0m";
		}
		cout<<endl;
		serialstr3=serialstr3+result;
		cout<<"received frame:"<<endl;
		for(int i=0;i<serialstr3.size();i++){
			//cout<<serialstr3[i]<<endl;
			string bit=randomGen(serialstr3[i],p);
			recvstr3.insert(i,bit);
		}

		for(int i=0;i<recvstr3.size();i++){
			if(recvstr3[i] != serialstr3[i]){
				cout << "\033[1;31m"<<recvstr3[i]<<"\033[0m";				
			}
			else {
				cout<<recvstr3[i];
			}
		}
		cout<<"\nresult of CRC checksum matching:";
		string rem=crcmaker(recvstr3,to_string(poly));
		if(atoi(rem.c_str()) == 0){
			cout<<" no error detected"<<endl;
		}
		else {
			cout<<" error detected"<<endl;
		}
		cout<<"\ndata block after removing CRC checksum bits:"<<endl;
		recvstr3.erase(recvstr3.size()-result.size(),result.size());
		char temparr[stringbit3.size()][29];int r=0;
		for(int j=0;j<29;j++){
			for(int i=0;i<stringbit3.size();i++,r++){
				temparr[i][j]=recvstr3[r];
			}
		}
		for(int i=0;i<stringbit3.size();i++){
			string arr;
			for(int j=0;j<29;j++){
				arr+=temparr[i][j];
			}
			recvbit3.push_back(arr);
			arr.clear();
		}
		int count[recvbit3.size()]={0};
		for(int i=0;i<recvbit3.size();i++){
			//cout<<recvbit[i]<<endl;
			for(int j=0;j<29;j++){
				if(recvbit3[i].at(j) != stringbit3[i].at(j)){
					cout << "\033[1;31m"<<recvbit3[i].at(j)<<"\033[0m";
					count[i]++;
				}
				else {
					cout<<recvbit3[i].at(j);
				}
			}
			cout<<endl;
		}
		for(int i=0;i<recvbit3.size();i++){
			if(count[i]==1)
			{
				if((recvbit3[i].at(0)-'0'+recvbit3[i].at(2)-'0'+recvbit3[i].at(4)-'0'+recvbit3[i].at(6)-'0'+
recvbit3[i].at(8)-'0'+recvbit3[i].at(10)-'0'+recvbit3[i].at(12)-'0'+recvbit3[i].at(14)-'0'+recvbit3[i].at(16)-'0'+
recvbit3[i].at(18)-'0'+recvbit3[i].at(20)-'0'+recvbit3[i].at(22)-'0')%2==0){
					r1="0";
				}
				else {
					r1="1";
				}
				if((recvbit3[i].at(1)-'0'+recvbit3[i].at(2)-'0'+recvbit3[i].at(5)-'0'+recvbit3[i].at(6)-'0'
+recvbit3[i].at(9)-'0'+recvbit3[i].at(10)-'0'+recvbit3[i].at(13)-'0'+recvbit3[i].at(14)-'0'+recvbit3[i].at(17)-'0'
+recvbit3[i].at(18)-'0'+recvbit3[i].at(21)-'0'+recvbit3[i].at(22)-'0')%2==0){
					r2="0";
				}
				else {
					r2="1";
				}
				if((recvbit3[i].at(3)-'0'+recvbit3[i].at(4)-'0'+recvbit3[i].at(5)-'0'+recvbit3[i].at(6)-'0'
+recvbit3[i].at(11)-'0'+recvbit3[i].at(12)-'0'+recvbit3[i].at(13)-'0'+recvbit3[i].at(14)-'0'+recvbit3[i].at(19)-'0'
+recvbit3[i].at(20)-'0'+recvbit3[i].at(21)-'0'+recvbit3[i].at(22)-'0')%2==0){
					r3="0";
				}
				else {
					r3="1";
				}
				if((recvbit3[i].at(7)-'0'+recvbit3[i].at(8)-'0'+recvbit3[i].at(9)-'0'+recvbit3[i].at(10)-'0'
+recvbit3[i].at(11)-'0'+recvbit3[i].at(12)-'0'+recvbit3[i].at(13)-'0'+recvbit3[i].at(14)-'0'+recvbit3[i].at(23)-'0')%2==0){
					r4="0";
				}
				else {
					r4="1";
				}
			        if((recvbit3[i].at(15)-'0'+recvbit3[i].at(16)-'0'+recvbit3[i].at(17)-'0'+recvbit3[i].at(18)-'0'
+recvbit3[i].at(19)-'0'+recvbit3[i].at(20)-'0'+recvbit3[i].at(21)-'0'+recvbit3[i].at(22)-'0'+recvbit3[i].at(23)-'0')%2==0){
					r5="0";
			}
			else {
					r5="1";
			}
			string num=r1+r2+r3+r4+r5;
			char * ptr;
    			long parsed = strtol(num.c_str(), & ptr, 2);
			if(parsed<=recvbit3.size()){
			if(recvbit3[i].at(parsed-1)==0){
				recvbit3[i].replace(parsed-1,1,"1");
			}
			else {
				recvbit3[i].replace(parsed-1,1,"0");
			}
			}}
		}
		cout<<"\ndata block after removing check bits:"<<endl;
		for(int i=0;i<recvbit3.size();i++){
			recvbit3[i].erase(15,1);
			recvbit3[i].erase(7,1);
			recvbit3[i].erase(3,1);
			recvbit3[i].erase(1,1);
			recvbit3[i].erase(0,1);
		}
		string frame;
		for(int i=0;i<recvbit3.size();i++){
			cout<<recvbit3[i]<<endl;
			frame+=recvbit3[i];
		}
		cout<<"\noutput frame: ";

		cout<<bittostr(frame)<<endl;
	}
	if(m==4){
		stringbit4.clear();
		for(int i=0;i<bits.size();i=i+m){
			string result=bits[i].to_string();
			string result2=bits[i+1].to_string();
			string result3=bits[i+2].to_string();
			string result4=bits[i+3].to_string();
			string cat=result+result2+result3+result4;
			stringbit4.push_back(cat);
			//cout<<"result "<<cat<<endl;
			cout<<bits[i]<<bits[i+1]<<bits[i+2]<<bits[i+3]<<endl;
		}
		cout<<"\ndata block after adding check bits:"<<endl;
		for(int i=0;i<stringbit4.size();i++){
			if((stringbit4[i].at(0)-'0'+stringbit4[i].at(2)-'0'+stringbit4[i].at(4)-'0'+stringbit4[i].at(6)-'0'+
stringbit4[i].at(8)-'0'+stringbit4[i].at(10)-'0'+stringbit4[i].at(12)-'0'+stringbit4[i].at(14)-'0'+stringbit4[i].at(16)-'0'+
stringbit4[i].at(18)-'0'+stringbit4[i].at(20)-'0'+stringbit4[i].at(22)-'0'+stringbit4[i].at(24)-'0'+stringbit4[i].at(26)-'0'+
stringbit4[i].at(28)-'0'+stringbit4[i].at(30)-'0')%2==0){
				stringbit4[i].insert(0,"0");
			}
			else {
				stringbit4[i].insert(0,"1");
			}
			if((stringbit4[i].at(1)-'0'+stringbit4[i].at(2)-'0'+stringbit4[i].at(5)-'0'+stringbit4[i].at(6)-'0'
+stringbit4[i].at(9)-'0'+stringbit4[i].at(10)-'0'+stringbit4[i].at(13)-'0'+stringbit4[i].at(14)-'0'+stringbit4[i].at(17)-'0'
+stringbit4[i].at(18)-'0'+stringbit4[i].at(21)-'0'+stringbit4[i].at(22)-'0'+stringbit4[i].at(25)-'0'+stringbit4[i].at(26)-'0'
+stringbit4[i].at(29)-'0'+stringbit4[i].at(30)-'0')%2==0){
				stringbit4[i].insert(1,"0");
			}
			else {
				stringbit4[i].insert(1,"1");
			}
			if((stringbit4[i].at(3)-'0'+stringbit4[i].at(4)-'0'+stringbit4[i].at(5)-'0'+stringbit4[i].at(6)-'0'
+stringbit4[i].at(11)-'0'+stringbit4[i].at(12)-'0'+stringbit4[i].at(13)-'0'+stringbit4[i].at(14)-'0'+stringbit4[i].at(19)-'0'
+stringbit4[i].at(20)-'0'+stringbit4[i].at(21)-'0'+stringbit4[i].at(22)-'0'+stringbit4[i].at(27)-'0'+stringbit4[i].at(28)-'0'
+stringbit4[i].at(29)-'0'+stringbit4[i].at(30)-'0')%2==0){
				stringbit4[i].insert(3,"0");
			}
			else {
				stringbit4[i].insert(3,"1");
			}
			if((stringbit4[i].at(7)-'0'+stringbit4[i].at(8)-'0'+stringbit4[i].at(9)-'0'+stringbit4[i].at(10)-'0'
+stringbit4[i].at(11)-'0'+stringbit4[i].at(12)-'0'+stringbit4[i].at(13)-'0'+stringbit4[i].at(14)-'0'+stringbit4[i].at(23)-'0'
+stringbit4[i].at(24)-'0'+stringbit4[i].at(25)-'0'+stringbit4[i].at(26)-'0'+stringbit4[i].at(27)-'0'+stringbit4[i].at(28)-'0'
+stringbit4[i].at(29)-'0'+stringbit4[i].at(30)-'0')%2==0){
				stringbit4[i].insert(7,"0");
			}
			else {
				stringbit4[i].insert(7,"1");
			}
			if((stringbit4[i].at(15)-'0'+stringbit4[i].at(16)-'0'+stringbit4[i].at(17)-'0'+stringbit4[i].at(18)-'0'
+stringbit4[i].at(19)-'0'+stringbit4[i].at(20)-'0'+stringbit4[i].at(21)-'0'+stringbit4[i].at(22)-'0'+stringbit4[i].at(23)-'0'
+stringbit4[i].at(24)-'0'+stringbit4[i].at(25)-'0'+stringbit4[i].at(26)-'0'+stringbit4[i].at(27)-'0'+stringbit4[i].at(28)-'0'
+stringbit4[i].at(29)-'0'+stringbit4[i].at(30)-'0')%2==0){
				stringbit4[i].insert(15,"0");
			}
			else {
				stringbit4[i].insert(15,"1");
			}
			if((stringbit4[i].at(31)-'0')%2==0){
				stringbit4[i].insert(31,"0");
			}
			else {
				stringbit4[i].insert(31,"1");
			}
			cout<<endl;
			for(int j=0;j<38;j++){
				if(j==0 || j==1 || j==3 || j==7 || j==15 || j==31){
					cout << "\033[1;32m"<<stringbit4[i].at(j)<<"\033[0m";
				}
				else {
					cout<<stringbit4[i].at(j);
				}
			}
		}
		cout<<"\ndata bit after column wise serialization:"<<endl;
		for(int j=0;j<38;j++){
			for(int i=0;i<stringbit4.size();i++){
				//cout<<stringbit4[i].at(j);
				serialstr4+=stringbit4[i].at(j);
			}
		}
		cout<<serialstr4<<endl;
		cout<<"\ndata bit after appending CRC checksum (sent frame):"<<endl;
		string result=crcmaker(serialstr4,to_string(poly));
		cout<<serialstr4;
		for(int i=0;i<result.size();i++){
			cout << "\033[1;36m"<<result[i]<<"\033[0m";
		}
		cout<<endl;
		serialstr4=serialstr4+result;
		cout<<"received frame:"<<endl;
		for(int i=0;i<serialstr4.size();i++){
			//cout<<serialstr4[i]<<endl;
			string bit=randomGen(serialstr4[i],p);
			recvstr4.insert(i,bit);
		}

		for(int i=0;i<recvstr4.size();i++){
			if(recvstr4[i] != serialstr4[i]){
				cout << "\033[1;31m"<<recvstr4[i]<<"\033[0m";				
			}
			else {
				cout<<recvstr4[i];
			}
		}
		cout<<"\nresult of CRC checksum matching:";
		string rem=crcmaker(recvstr4,to_string(poly));
		if(atoi(rem.c_str()) == 0){
			cout<<" no error detected"<<endl;
		}
		else {
			cout<<" error detected"<<endl;
		}
		cout<<"\ndata block after removing CRC checksum bits:"<<endl;
		recvstr4.erase(recvstr4.size()-result.size(),result.size());
		char temparr[stringbit4.size()][38];int r=0;
		for(int j=0;j<38;j++){
			for(int i=0;i<stringbit4.size();i++,r++){
				temparr[i][j]=recvstr4[r];
			}
		}
		for(int i=0;i<stringbit4.size();i++){
			string arr;
			for(int j=0;j<38;j++){
				arr+=temparr[i][j];
			}
			recvbit4.push_back(arr);
			arr.clear();
		}
		int count[recvbit4.size()]={0};
		for(int i=0;i<recvbit4.size();i++){
			//cout<<recvbit[i]<<endl;
			for(int j=0;j<38;j++){
				if(recvbit4[i].at(j) != stringbit4[i].at(j)){
					cout << "\033[1;31m"<<recvbit4[i].at(j)<<"\033[0m";
					count[i]++;
				}
				else {
					cout<<recvbit4[i].at(j);
				}
			}
			cout<<endl;
		}
		for(int i=0;i<recvbit4.size();i++){
			if(count[i]==1)
			{
				if((recvbit4[i].at(0)-'0'+recvbit4[i].at(2)-'0'+recvbit4[i].at(4)-'0'+recvbit4[i].at(6)-'0'+
recvbit4[i].at(8)-'0'+recvbit4[i].at(10)-'0'+recvbit4[i].at(12)-'0'+recvbit4[i].at(14)-'0'+recvbit4[i].at(16)-'0'+
recvbit4[i].at(18)-'0'+recvbit4[i].at(20)-'0'+recvbit4[i].at(22)-'0'+recvbit4[i].at(24)-'0'+recvbit4[i].at(26)-'0'+
recvbit4[i].at(28)-'0'+recvbit4[i].at(30)-'0')%2==0){
					r1="0";
				}
				else {
					r1="1";
				}
				if((recvbit4[i].at(1)-'0'+recvbit4[i].at(2)-'0'+recvbit4[i].at(5)-'0'+recvbit4[i].at(6)-'0'
+recvbit4[i].at(9)-'0'+recvbit4[i].at(10)-'0'+recvbit4[i].at(13)-'0'+recvbit4[i].at(14)-'0'+recvbit4[i].at(17)-'0'
+recvbit4[i].at(18)-'0'+recvbit4[i].at(21)-'0'+recvbit4[i].at(22)-'0'+recvbit4[i].at(25)-'0'+recvbit4[i].at(26)-'0'
+recvbit4[i].at(29)-'0'+recvbit4[i].at(30)-'0')%2==0){
					r2="0";
				}
				else {
					r2="1";
				}
				if((recvbit4[i].at(3)-'0'+recvbit4[i].at(4)-'0'+recvbit4[i].at(5)-'0'+recvbit4[i].at(6)-'0'
+recvbit4[i].at(11)-'0'+recvbit4[i].at(12)-'0'+recvbit4[i].at(13)-'0'+recvbit4[i].at(14)-'0'+recvbit4[i].at(19)-'0'
+recvbit4[i].at(20)-'0'+recvbit4[i].at(21)-'0'+recvbit4[i].at(22)-'0'+recvbit4[i].at(27)-'0'+recvbit4[i].at(28)-'0'
+recvbit4[i].at(29)-'0'+recvbit4[i].at(30)-'0')%2==0){
					r3="0";
				}
				else {
					r3="1";
				}
				if((recvbit4[i].at(7)-'0'+recvbit4[i].at(8)-'0'+recvbit4[i].at(9)-'0'+recvbit4[i].at(10)-'0'
+recvbit4[i].at(11)-'0'+recvbit4[i].at(12)-'0'+recvbit4[i].at(13)-'0'+recvbit4[i].at(14)-'0'+recvbit4[i].at(23)-'0'
+recvbit4[i].at(24)-'0'+recvbit4[i].at(25)-'0'+recvbit4[i].at(26)-'0'+recvbit4[i].at(27)-'0'+recvbit4[i].at(28)-'0'
+recvbit4[i].at(29)-'0'+recvbit4[i].at(30)-'0')%2==0){
					r4="0";
				}
				else {
					r4="1";
				}
			        if((recvbit4[i].at(15)-'0'+recvbit4[i].at(16)-'0'+recvbit4[i].at(17)-'0'+recvbit4[i].at(18)-'0'
+recvbit4[i].at(19)-'0'+recvbit4[i].at(20)-'0'+recvbit4[i].at(21)-'0'+recvbit4[i].at(22)-'0'+recvbit4[i].at(23)-'0'+recvbit4[i].at(24)-'0'
+recvbit4[i].at(25)-'0'+recvbit4[i].at(26)-'0'+recvbit4[i].at(27)-'0'+recvbit4[i].at(28)-'0'+recvbit4[i].at(29)-'0'+recvbit4[i].at(30)-'0')%2==0){
					r5="0";
			}
			else {
					r5="1";
			}
			if((recvbit4[i].at(31)-'0')%2==0){
				r6="0";
			}
			else {
				r6="1";
			}
			string num=r1+r2+r3+r4+r5+r6;
			char * ptr;
    			long parsed = strtol(num.c_str(), & ptr, 2);
			if(parsed<=recvbit4.size()){
			if(recvbit4[i].at(parsed-1)==0){
				recvbit4[i].replace(parsed-1,1,"1");
			}
			else {
				recvbit4[i].replace(parsed-1,1,"0");
			}
			}}
		}

		cout<<"\ndata block after removing check bits:"<<endl;
		for(int i=0;i<recvbit4.size();i++){
			recvbit4[i].erase(31,1);
			recvbit4[i].erase(15,1);
			recvbit4[i].erase(7,1);
			recvbit4[i].erase(3,1);
			recvbit4[i].erase(1,1);
			recvbit4[i].erase(0,1);
		}
		string frame;
		for(int i=0;i<recvbit4.size();i++){
			cout<<recvbit4[i]<<endl;
			frame+=recvbit4[i];
		}
		cout<<"\noutput frame: ";

		cout<<bittostr(frame)<<endl;
	}

	return 0;
}
