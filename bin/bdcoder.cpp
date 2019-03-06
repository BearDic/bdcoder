#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<windows.h> 
using namespace std;

string ffmpeg,conf;
const string common=" -threads 6 -vcodec libx264 -acodec aac -keyint_min 30 -preset slow -profile:v high10 -f mp4 ";

void exec(string s)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN);
	string tmp="\""+s+"\"";
	cout<<"Exec: "<<tmp<<endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE+FOREGROUND_RED);
	cout<<"-----BEGIN FFMPEG-----"<<endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
	system(tmp.c_str());
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE+FOREGROUND_RED);
	cout<<"-----END FFMPEG-----"<<endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
} 

string output(string s,string sig="bdcoder")
{
	int pos=s.rfind(".");
	return s.replace(pos,s.size()-pos,"_"+sig+".mp4\"");
}

void command(string input)
{
	string ans;
	input="\""+input+"\"";
	if (conf=="-crf") ans=ffmpeg+" -i "+input+common+" -crf 24 -g 120 -b:a 128ki "+output(input);
	else if (conf=="-long") ans=ffmpeg+" -i "+input+common+" -pass 1 -g 500 -b:v 3600k -an -y NUL & "+ffmpeg+" -i "+input+common+" -pass 2 -g 500 -b:v 3600k -b:a 128ki "+output(input);
	else if (conf=="-default") ans=ffmpeg+" -i "+input+common+" -pass 1 -g 120 -b:v 5000k -an -y NUL & "+ffmpeg+" -i "+input+common+" -pass 2 -g 120 -b:v 5000k -b:a 128ki "+output(input);
	else if (conf=="-cut")
	{
		string b,e;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),6);
		cout<<"Start point (HH:MM:SS or a digit in seconds, b for beginning): "<<endl<<">"; 
		cin>>b;
		if (b[0]!='b') b=" -ss "+b;
		else b.clear();
		cout<<"End point (e for ending): "<<endl<<">";
		cin>>e;
		if (e[0]!='e') e=" -to "+e;
		else e.clear();
		ans=ffmpeg+b+e+" -accurate_seek -i "+input+" -c copy -avoid_negative_ts 1 "+output(input,"cut");
	}
	else 
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED);
		cout<<"Invalid option \""+conf+"\", please check your command!"<<endl<<endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
		system("pause");
		exit(0);
	}
	exec(ans);
}

int main(int argc,char **argv)
{
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),3);
	cout<<"BDCoder - version 190302, by Dict Xiong."<<endl<<endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
	
	ffmpeg="\""+string(argv[0]).replace(string(argv[0]).rfind("\\"),string(argv[0]).size()-string(argv[0]).rfind("\\"),"\\ffmpeg.exe\"");
	conf="-default";
	
	int countConf=0;
	for (int i=1;i!=argc;i++) 
	{
		if (argv[i][0]=='-')
		{
			conf=argv[i];
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN+FOREGROUND_RED);
			cout<<"Option \""+string(argv[i])+"\" received."<<endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
			countConf++;
			continue;
		}
		
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),3);
		cout<<endl<<"Processing file "<<i-countConf<<": "<<argv[i]<<endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
		
		command(argv[i]);
	}
	cout<<endl;
	system("pause");
	return 0;
}
