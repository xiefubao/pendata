#include <iostream>
#include <vector>
using namespace std;

#include "util/dis_function.hpp"
#include "util/add_noise.hpp"


extern double coe;
// test1.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <cstring>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <ctime>
#include <iomanip>
#include <cmath>
#include <queue>
#include <sstream>

using namespace std;

struct point
{
    double x;
    double y;
    double f;
    point(){}
    point(double a,double b,double c)
    {
        x = a;
        y = b;
        f = c;
    }
    void add_noise(double noise)
    {
        x += randNormal(noise);
        y += randNormal(noise);
        f += randNormal(noise);
    }
};
double dis(const point& p1,const point& p2)
{
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y) + (p1.f - p2.f)*(p1.f - p2.f));
}

vector< vector< vector<point> > >series;
string names[40];
map<string,int> maps;
const int num_all = 2858;
int center[100];


extern double INF;

extern double coe;

double down_ratio = 0.2;

struct sentence{
 vector<point> sen;
 string label;
 int begin;
 int end;
};
vector<sentence> sentences;

string to_string(int value)
{
    char help[10];
    sprintf(help, "%d", value);
    return string(help);
}
int get_intersection(int l1,int r1,int l2,int r2)
{
    if(r2<l1||r1<l2)
      return 0;
    return min(r1,r2)-max(l1,l2)+1;
}
bool equel(double a,double b)
{
    return fabs(a - b) < 0.000000001;
}
void readFile()
{
    series.resize(20);
    ifstream read;
    read.open("label.txt");
    int t;
    int co = 0;
    vector<int> vec_label(num_all);
    for (int i = 0 ; i < num_all; ++i)
    {
        read >> vec_label[i];
        --vec_label[i];
        char c;
        read >> c;
    }
    
    string now;
    ifstream in("tra.txt");
    if (!in.is_open())
    {
        cout << "not open" << endl;
        exit(1);
    }
    string line_string;
    for(int i = 0;i < num_all; ++i){
        getline(in,line_string);
        stringstream line(line_string);
        double datax = 0,datay = 0,dataf = 0;
        vector<point> now;
        while(line >> datax >> datay >> dataf){
            if(!equel(datax ,0) || !equel(datay ,0) || !equel(dataf ,0))
            {
            now.push_back(point(datax,datay,dataf));
            }
        }
        cout << now.size() << " ";
        series[vec_label[i]].push_back(now);
    }
}

void getcenter()
{
    for(int  i = 0;i < series.size();i++)
    {
        double min_dist = INF;
        for(int j = 0;j < series[i].size();j++)
        {
            double sum_dist = 0;
            for(int k = 0;k < series[i].size();k++)
            {
                sum_dist += getdistance_dtw(series[i][j],series[i][k]);
            }
            if(sum_dist < min_dist)
            {
                min_dist = sum_dist;
                center[i] = j;
                //center[i] = rand()%series[i].size();
            }
        }
    }
    cout << "getcentered" << endl;
}
void get_sentences()
{
    sentences.resize(series.size());
    for(int i = 0 ;i < series.size();i++)
    {
        int pre = rand()%15 + 1;
        int suf = rand()%15 + 1;
        vector<point>& sen = sentences[i].sen;
        double mean_dist = -1;
        for(int j = 0;j < pre ;j++)
        {
            int cla = rand()%(series.size() - 1);
            if(cla >= i)
                cla++;
            int nu = rand()%series[cla].size();
            sen.insert(sen.end(),series[cla][nu].begin(),series[cla][nu].end());
        }
        sentences[i].begin = sen.size();
        sen.insert(sen.end(),series[i][center[i]].begin(),series[i][center[i]].end());
        sentences[i].end = sen.size() - 1;
        for(int j = 0;j < suf ;j++)
        {
            int cla = rand()%(series.size() - 1);
            if(cla >= i)
                cla++;
            int nu = rand()%series[cla].size();
            sen.insert(sen.end(),series[cla][nu].begin(),series[cla][nu].end());
        }
    }
    cout << "success" << endl;
}
void compute_accuracy(double& recall,double& accuracy,void (*get_distance)(vector<point>&,vector<point>&,int&,int&))
{
    double mean_begin = 0;
    double mean_end = 0;    
    int cou = 0;
    recall = 0;
    accuracy = 0;
    for(int i = 0;i < series.size();i++)
    {
        double re = 0;
        double ac = 0;
        for (int j = 0;j < series[i].size(); j++)
        {
            int beg = 0 ,end = 0;
            add_noise(series[i][j],down_ratio,0);
            get_distance(sentences[i].sen, series[i][j], beg, end);
            
            int beg_real = sentences[i].begin;
            int end_real = sentences[i].end;
            int inter = get_intersection(beg_real,end_real,beg,end);
            re += inter*1.0/(end_real - beg_real + 1);
            ac += inter*1.0/(end - beg + 1);             
        }
        re /= double(series[i].size());
        ac /= double(series[i].size());
        recall += re;
        accuracy += ac;
        double me = 2 * re * ac / (re + ac);
    }
    recall /= double(series.size());
    accuracy /= double(series.size());
}
int main()//*******************************************************************************************************
{
    readFile();
    cout << "read all " << endl;
    getcenter();
    cout << "getcenter" << endl;
    get_sentences();
    cout << "getsentences" << endl;



    double recall;
    double accuracy;
    compute_accuracy(recall,accuracy,get_nearest_dtw);
    cout<< "-----------------------------------------------------" << endl;
    cout << "md recall:   " << recall <<endl;
    cout << "md accuracy: " << accuracy <<endl;
    cout << "md mean:     " << 2 * recall * accuracy / (recall + accuracy) << endl << endl;

    compute_accuracy(recall,accuracy,get_nearest_ed);
    cout << "dtw recall:   " << recall <<endl;
    cout << "dtw accuracy: " << accuracy <<endl;
    cout << "dtw mean:     " << 2 * recall * accuracy / (recall + accuracy) << endl << endl;

    compute_accuracy(recall,accuracy,get_nearest_ed);
    cout << "ed recall:   " << recall <<endl;
    cout << "ed accuracy: " << accuracy <<endl;
    cout << "ed mean:     " << 2 * recall * accuracy / (recall + accuracy) << endl;
    return 0;
}