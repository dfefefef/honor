#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include<limits.h>
#include <vector>
using namespace std;

class cross
{
public:
    int id;
    int road1;
    int road2;
    int road3;
    int road4;


    cross()
    {

    }
};

class road
{
public:
    int id;
    int length;
    int speed;
    int channel;
    int from;
    int to;
    int isDuplex;

    road(int i, int l, int s, int c, int f, int t, int d):id(i), length(l), speed(s), channel(c), from(f), to(t), isDuplex(d)
    {

    }

    road()
    {

    }
};

class car
{
public:
    int id;
    int from;
    int to;
    int speed;
    int planTime;

    car(){}

    car(int i, int f, int t, int s, int p):id(i), from(f), to(t), speed(s),  planTime(p)
    {
        ;
    }


};

class fileAnalzyer
{

public:
    string filename;
    string curline;
    int filelength;
    ifstream io;


    int numOfline()
    {
        int n = 0;
        ifstream i(filename.c_str());
        string tmp;

        if(i.fail())
        {
            cout<< filename<<" open fail"<<endl;
            return -1;
        }

        while(getline(i, tmp, '\n'))
            n++;

        i.close();

        return n-1;

    }


    fileAnalzyer(char* file):filename(file), filelength(0), io(file)
    {
        string first;
        getline(io, first);
        cout<<first<<" file readying"<<endl;
        filelength = numOfline();
    }


    //分词器
    void strTok(int *w)
    {
        int s = 0;  //每个子串的起始位置
        bool flag = false;
        int n = 0;

        for(int i = 0; i < curline.length(); i++)
        {
            if(curline[i] == ' ' || curline[i] == ')'||curline[i] == ',' || curline[i] == '(')//分割符号
            {
                if(flag)
                {
                    w[n++] = atoi(curline.substr(s+1, i-1).c_str());
                    flag = false;
                }
                s = i;
                continue;
            }
            flag = true;
        }
    }

    bool analyzeRoadline(road & r)
    {
        int words[7] = {};
        if (!getline(io, curline, '\n'))
        {
            cout<<"no more road"<<endl;
            return 0;
        }

        //分词器
        strTok(words);

        r.id = words[0];
        r.length = words[1];
        r.speed = words[2];
        r.channel = words[3];
        r.from = words[4];
        r.to = words[5];
        r.isDuplex = words[6];
        return 1;
    }

    bool analyzeCrossline(cross & c)
    {
        int words[5] = {};
        if (!getline(io, curline, '\n'))
        {
            cout<<"no more road"<<endl;
            return 0;
        }

        //分词器
        strTok(words);

        c.id = words[0];
        c.road1 = words[1];
        c.road2 = words[2];
        c.road3 = words[3];
        c.road4 = words[4];
        return 1;
    }

    bool analyzeCarline(car & c)
    {
        int words[5] = {};
        if (!getline(io, curline, '\n'))
        {
            cout<<"no more car"<<endl;
            return 0;
        }

        //分词器
        strTok(words);

        c.id = words[0];
        c.from = words[1];
        c.to = words[2];
        c.speed = words[3];
        c.planTime = words[4];
        return 1;
    }
};

class PathGenerator
{
public:
    int** lengths;
    int** speeds;
    int** times;
    vector<int>** path;
    int road_num;
    int cross_num;

    int cur_speed;  //当前速度

public:

    PathGenerator(int r, int c):road_num(r), cross_num(c)
    {
        lengths = new int*[c+1];
        speeds = new int*[c+1];
        times = new int*[c+1];
        path = new vector<int>*[c+1];

        for(int i = 1; i<=cross_num; i++)
        {
            lengths[i] = new int[c+1];
            speeds[i] = new int[c+1];
            times[i] = new int[c+1];
            path[i] = new vector<int>[c+1];
        }

        for(int i = 1; i<=cross_num; i++)
            for(int j = 1; j<=cross_num; j++)
            {
                lengths[i][j] = -1;
                speeds[i][j] = -1;
                times[i][j] = -1;

            }
    }


        //Floyd-Warshall算法核心语句
    void warShall()
    {
        for(int k = 1; k <= cross_num; k++)
            for(int i = 1 ; i <= cross_num; i++)
                for(int j = 1; j <= cross_num ; j++)
                {
                    int tmp = times[i][j] == -1 ? INT_MAX : times[i][j];
                    if((tmp > times[i][k] + times[k][j]) && times[i][k] != -1 && times[k][j] != -1)
                    {
                        path[i][j].assign(path[i][k].begin(), path[i][k].end());
                        path[i][j].push_back(k);
                        path[i][j].insert(path[i][j].end(), path[k][j].begin(), path[k][j].end());
                        times[i][j]=times[i][k]+times[k][j];
                    }
                }
    }


    void setSpeed(int speed)
    {
        cur_speed = speed;

        for(int i = 1; i<=cross_num; i++)
            for(int j = 1; j<=cross_num; j++)
            {
                if(speeds[i][j] > cur_speed && speeds[i][j] != -1)
                    speeds[i][j] = cur_speed;

                times[i][j] = speeds[i][j] == -1 ? -1 : lengths[i][j]/speeds[i][j];
            }

    warShall();
    }


    void assignRoad(int f, int t, int length, int speed)
    {
        lengths[f][t] = length;
        speeds[f][t] = speed;
    }

    void printMap()
    {
        for(int i = 1; i<=cross_num; i++)
        {
            for(int j = 1; j<=cross_num; j++)
                cout<<setw(4)<<times[i][j];
            cout<<endl;
        }
    }
};

int main()

{
    char filename[100] = "/home/zhenwang/cproject/horor/1-map-training-1/road.txt";
    char filename1[100] = "/home/zhenwang/cproject/horor/1-map-training-1/cross.txt";
    char filename2[100] = "/home/zhenwang/cproject/horor/1-map-training-1/car.txt";

    fileAnalzyer roadfile(filename);
    fileAnalzyer crossfile(filename1);
    fileAnalzyer carfile(filename2);


    PathGenerator pg(roadfile.filelength, crossfile.filelength);


    for(int i = 0; i<crossfile.filelength; i++)
    {
        cross C;
        crossfile.analyzeCrossline(C);
        cout<<C.id<<endl;
    }

    for(int i = 0; i<roadfile.filelength; i++)
    {
        road R;
        roadfile.analyzeRoadline(R);

        pg.assignRoad(R.from, R.to, R.length, R.speed);
        if(R.isDuplex)
            pg.assignRoad(R.to, R.from, R.length, R.speed);
    }

    for(int i = 0; i<2; i++)
    {
        car ca;
        carfile.analyzeCarline(ca);
        pg.setSpeed(ca.speed);

        for(int i = 0; i < pg.path[ca.from][ca.to].size(); ++i)
            cout<<pg.path[ca.from][ca.to][i]<<endl;
    }
    pg.printMap();

    return 0;

}


