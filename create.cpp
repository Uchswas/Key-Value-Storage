#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include <set>
#include <string>
using namespace std;

// Global variables for G2A matrix dimensions and dataset info
vector<int> setsize;
vector<int> setind;
void mylastsearch(void);
bool** arra;
int a=1,b=1;  // G2A matrix dimensions (a x b)
int workingset=0;

set<string> s[10];  // Unique values for each column
set<unsigned long long int> his;  // Set to store unique keys
void create(void);
ofstream out;
ofstream keyvalue;
ofstream linkvalue;

// Combines two 32-bit values into a 64-bit key (G2A key generation)
long long combine(unsigned int high, unsigned int low)
{
    return (((uint64_t) high) << 32) | ((uint64_t) low);
}

// Finds index of a value in a set
int findindex(set<string> mySet,string match)
{
    int index = 0;
    set<string>::iterator b;
    for( b= mySet.begin(); b != mySet.end(); b++)
    {
        string element = *b;
        if(element==match)
            break;
        index++;
    }
    if(b==mySet.end())
        return -1;
    return index;
}

int main()
{
    out.open("output.txt");
    keyvalue.open("keyvalues.txt");
    linkvalue.open("links.txt");
    int totalset=0;
    FILE *fp = fopen("dataset.csv", "r");
    char *token;
    int i;
    int cnt=0;
    // Read dataset and extract unique values for each column
    if(fp != NULL)
    {
        char * pch;

        char line[100];

        while(fgets(line, sizeof line, fp) != NULL && cnt<10000)
        {
            cnt++;
            if(cnt==1)
                continue;
            pch = strtok (line,",");
            int x=0;
            while (pch != NULL)
            {
                s[x].insert(pch);
                pch = strtok (NULL, ",");
                x++;
            }
        }
        fclose(fp);
    }
    else
    {
        perror("file not found");
    }
    // Find columns to use for G2A (exclude primary keys and constant columns)
    int mx=0;
    int keyind[10]= {0};
    for(int i=0; i<9; i++)
    {
        cout<<"Unique Value in "<<i<<" Column is "<<s[i].size()<<endl;
        if(s[i].size()>mx)
            mx=s[i].size();
    }
    cout<<"So we will work with the column = ";
    int evenoddcnt=1;
    // Calculate G2A dimensions: split columns into two groups (a and b)
    for(int i=0; i<9; i++)
    {

        if(s[i].size()!=mx && s[i].size()>1)
        {
            out<<s[i].size()<<" ";
            workingset++;
            keyind[i]=1;
            if(evenoddcnt%2==0)
                b*=s[i].size();
            else
                a*=s[i].size();
            cout<<i<<" ";
            evenoddcnt++;
            setind.push_back(i);
            setsize.push_back(s[i].size());
        }
    }
    out<<endl;
    cout<<endl<<"Others are Primary Key or Have Same Value Everywhere"<<endl;
    cout<<"And the size of matrix is : "<<a<<"x"<<b<<endl;

    for(int xx=0; xx<9; xx++)
    {
        if(keyind[xx]==1 && xx!=7)
        {
            for (set<string>::iterator i = s[xx].begin(); i != s[xx].end(); i++)
            {
                string element = *i;
                out<<element<<",";
            }
            out<<endl;
        }
    }
    cout<<s[4].size();
    out<<"AM,PM,"<<endl;
    create();
}

// Generates keys from dataset and stores them in bins
void create(void)
{
    FILE * pFile;
    pFile = fopen ("myfile.bin", "w");
    FILE *fp = fopen("dataset.csv", "r");
    char *token;
    int i;
    int cnt=0;
    if(fp != NULL)
    {
        char * pch;

        char line[100];
        vector<string> eval;
        int setindex[7];
        while(fgets(line, sizeof line, fp) != NULL && cnt<10000)
        {
            cnt++;
            if(cnt%1000==0)
                cout<<"O";
            if(cnt==1)
                continue;
            pch = strtok (line,",");
            int x=0;
            int realcount=0;
            // Extract indices for selected columns
            while (pch != NULL)
            {
                if(x==setind[realcount])
                {
                    eval.push_back(pch);

                    setindex[realcount]=findindex(s[x],pch);
                    realcount++;
                }
                x++;
                pch = strtok (NULL, ",");
            }

            // G2A transformation: convert nD indices to 2D (x0, x1)
            unsigned int x0;
            unsigned int x1;
            x0=(setindex[0]*setsize[2]*setsize[4]*setsize[6])+(setindex[2]*setsize[4]*setsize[6])+(setindex[4]*setsize[6])+setindex[6];
            x1=(setindex[1]*setsize[3]*setsize[5])+(setindex[3]*setsize[5])+setindex[5];
            // Generate 64-bit key from x0 and x1
            unsigned long long int lastres=combine(x0,x1);
            his.insert(lastres);
            eval.clear();
        }
        fclose(fp);
    }
    else
    {
        perror("Sorry!! File not found");
    }

    // Store keys in fixed-size bins (100 keys per bin)
    set<unsigned long long int>::iterator m;
    unsigned long long int *a=new unsigned long long int[cnt];
    int gun=0;
    for(m=his.begin(); m!=his.end(); m++)
    {
        unsigned long long int x=*m;
        a[gun++]=x;
    }
    cout<<"Total Number of Key Values : "<<gun<<endl;
    int newcnt=0;
    unsigned long long int b[101];
    b[0]=a[0];
    int filename=0;
    linkvalue<<a[0]<<" ";  // Store first key of each bin in links.txt
    int anotherindex=0;
    for(int i=1; i<gun; i++)
    {
        b[anotherindex]=a[newcnt++];
        if(i%100==0)  // Create new bin file every 100 keys
        {
            stringstream ss;
            ss << 'v' << filename<<".bin";
            string name=ss.str();
            const char * c = name.c_str();
            FILE *f1 = fopen(c, "wb");
            assert(f1);
            size_t r1 = fwrite(b, sizeof b[0], 100, f1);
            fclose(f1);
            filename++;
            anotherindex=0;
            linkvalue<<a[i]<<" ";  // Store link to bin
        }

        anotherindex++;
        keyvalue<<a[i]<<endl;
    }
}
