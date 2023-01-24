#include <bits/stdc++.h>
using namespace std;
#include <string>
int setsize[7];
void initializemybin();
int wherebin(unsigned long long int,int);
int checkexist(unsigned long long int);
set<unsigned long long int > binlink;
unsigned long long combine(unsigned int high, unsigned int low)
{
    return (((uint64_t) high) << 32) | ((uint64_t) low);
}

int  findindex(int y,string xval)
{


    FILE *fp = fopen("output.txt", "r");
    char *token;
    int i,res=-1;
    int cnt=0;
    if(fp != NULL)
    {
        char * pch;
        char line[10000];
        while(fgets(line, sizeof line, fp) != NULL)
        {
            cnt++;
            if(cnt==y)
            {
                pch = strtok (line,",");
                int x=0;
                while (pch != NULL)
                {
                    string val=pch;
                    if(val==xval)
                        res=x;
                    x++;
                    pch = strtok (NULL, ",");
                }
                cout<<endl;
                cout<<res<<endl;
            }
        }
        fclose(fp);
    }
    return res;
}


void takelength()
{
    FILE *fp = fopen("output.txt", "r");
    char *token;
    int i,res=-1;
    int cnt=0;
    if(fp != NULL)
    {
        char * pch;
        char line[10000];
        while(fgets(line, sizeof line, fp) != NULL)
        {
            if(cnt==0)
            {
                int x=0;
                pch = strtok (line," ");
                while (pch != NULL)
                {
                    string val=pch;
                    int inval=atoi(pch);
                    setsize[x]=inval;
                    x++;
                    pch = strtok (NULL, " ");
                }
            }
            break;

        }
        fclose(fp);
    }

}

int main()
{
    initializemybin();
    cout<<"Please Enter the Number You Want to Search : :"<<endl;
    int setindex[7];
    takelength();
    for(int i=0; i<7; i++)
    {
        cout<<i+1<<" th number : ";
        string x;
        getline (cin, x);
        setindex[i]=findindex(i+2,x);
        if(setindex[i]==-1)
        {
            cout<<"No element there, Try Again Please"<<endl; ;
            i--;
            continue;
        }
        else
        {
            cout<<"OK"<<endl;
        }
    }
    unsigned long  int x0;
    unsigned long  int x1;
    x0=(setindex[0]*setsize[2]*setsize[4]*setsize[6])+(setindex[2]*setsize[4]*setsize[6])+(setindex[4]*setsize[6])+setindex[6];
    x1=(setindex[1]*setsize[3]*setsize[5])+(setindex[3]*setsize[5])+setindex[5];
    cout<<x0<<" "<<x1<<endl;
    unsigned long long  int com=combine(x0,x1);

    int binnum=checkexist(com);
    cout<<"Key value is: "<<com<<endl;
    if(binnum!=-1)
    {
        int last=wherebin(com,binnum);
        if(last)
            cout<<"Yes exist"<<endl;
        else
            cout<<"Oh data doesn,t exist"<<endl;
    }
    else
        cout<<"Oh data doesn,t exist"<<endl;


}
int wherebin(unsigned long long int value,int binnum)
{


    int flag=0;
    stringstream ss;
    ss << 'v' <<binnum<<".bin";
    string name=ss.str();
    const char * c = name.c_str();
    cout<<c<<endl;
    FILE *f2 = fopen(c, "rb");
    unsigned long long   int xyz[1000];
    size_t r2 = fread(xyz, sizeof xyz[0],100, f2);
    fclose(f2);
    for(int i = 0; i <100; i++)
    {
        if(xyz[i]==value)
        {
            flag=1;
            cout<<"Index in bin "<<i<<endl;
            break;
        }
    }

    return flag;
}
int checkexist(unsigned long long int keyvalue)
{
    set<unsigned long long int> ::iterator it;
    int cnt=0;
    for(it=binlink.begin(); it!=binlink.end(); it++)
    {
        unsigned long long int value=*it;
        if(keyvalue<value)
            return cnt-1;
        cnt++;
    }

    return -1;
}
void initializemybin()
{


    FILE *fp = fopen("links.txt", "r");
    int i,res=-1;
    int cnt=0;
    if(fp != NULL)
    {
        char * pch;
        char line[10000];
        while(fgets(line, sizeof line, fp) != NULL)
        {
            if(cnt==0)
            {
                pch = strtok (line," ");
                int x=0;
                while (pch != NULL)
                {

                    char *pend;
                    unsigned long long int binvalue=strtoull(pch,&pend,10);
                    binlink.insert(binvalue);
                    pch = strtok (NULL, " ");
                }
            }
            break;
        }
        fclose(fp);
    }

}

