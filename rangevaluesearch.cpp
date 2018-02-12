
#include  <bits/stdc++.h>
using namespace std;
void initialize();
int  listelement[7]= {0};
int fixedvalue;
int totalper=0;
int mycnt=0;
int permutationnum=6;
vector<string> sets[8];
vector<int> temporary;
vector<int> permu[100000];
vector<int> binnas[7];
int currentvector;
void call(int);
int  currentlength=0;
int setsize[8];
void takelength();
int permutation();
vector<int> rangeindex;
ofstream out;
void initializepermutation();
void calculateranges(string,string);
void initializemybin();
int findindexinvector(int , string);
set <unsigned long long int> binlink;
void generatekeyvalue(int);
unsigned long long int combine(long  int,long  int);
int checkexist(unsigned long long int);
int wherebin(unsigned long long int,int);



int main()
{
    out.open("paul.txt");
    string from, to;
    cout<<"Enter the range you want to search: ";
    takelength();
    initialize();
    initializemybin();
    initializepermutation();
    cin>>from>>to;
    calculateranges(from,to);

    cout<<"Index of single key:"<<fixedvalue<<endl;
    for(int range=0; range<rangeindex.size(); range++)
    {
        fixedvalue=rangeindex[range];
        for(int i=0; i<totalper; i++)
        {
            currentvector=i;
            generatekeyvalue(i);
        }
    }

    for(int i=0; i<8; i++)
        cout<<setsize[i]<<" ";
    cout<<endl;
    cout<<mycnt;
}

void calculateranges(string from , string to)
{
    const char * ff = from.c_str();
    const char * tt = to.c_str();

    int f=atoi(ff);
    int t=atoi(tt);
    cout<<f<<" "<<t<<endl;
    for(int i=0; i<setsize[0]; i++)
    {
        const char * ss = sets[0][i].c_str();
        int now=atoi(ss);
        cout<<now<<endl;
        if(now<=t && now>=f)
        {
            cout<<"***** "<<i <<" ******"<<endl;
            rangeindex.push_back(i);
        }
    }



}

unsigned long long combine(long int high, long int low)
{
    return (((uint64_t) high) << 32) | ((uint64_t) low);
}

void generatekeyvalue(int xx)
{
    long int a;
    long  int b;
    a=(fixedvalue*setsize[2]*setsize[4]*setsize[6])+(permu[xx][1]*setsize[4]*setsize[6])+(permu[xx][3]*setsize[6])+permu[xx][5];
    b=(permu[xx][0]*setsize[3]*setsize[5])+(permu[xx][2]*setsize[5])+permu[xx][4];




    unsigned long long int com=combine(a,b);
    int binnum=checkexist(com);
    if(binnum!=-1)
    {
        int last=wherebin(com,binnum);

        if(last)
        {
            cout<<sets[0][fixedvalue]<<" ";
            for(int i=0; i<6; i++)
            {
                int m=permu[xx][i];
                cout<< sets[i+1][m]<<"  ";
            }
            cout<<endl;



        }

    }


}



int wherebin(unsigned long long int value,int binnum)
{
    mycnt++;

    int flag=0;
    stringstream ss;
    ss << 'v' <<binnum<<".bin";
    string name=ss.str();
    const char * c = name.c_str();
    FILE *f2 = fopen(c, "rb");
    unsigned long long   int xyz[1000];
    size_t r2 = fread(xyz, sizeof xyz[0],100, f2);
    fclose(f2);
    for(int i = 0; i <100; i++)
    {
        if(xyz[i]==value)
        {
            cout<<"Congratulation!! relation found for this key "<<endl;
            flag=1;
            cout<<endl<<endl<<endl;
            cout<<name<<endl;
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

int findindexinvector(int num,string value)
{
    int res=-1;
    for(int i=0; i<sets[num].size(); i++)
    {
        if(value==sets[num][i])
        {
            return i;
        }
    }
    return res;

}

void call(int id)
{
    if(id==permutationnum)
    {

        for(int i=0; i<temporary.size(); i++)
        {
            permu[totalper].push_back(temporary[i]);
        }
        totalper++;

        return;
    }
    for(int i=0; i<binnas[id].size(); i++)
    {
        temporary.push_back(binnas[id][i]);
        call(id+1);
        temporary.pop_back();
    }
}





void initializepermutation()
{
    for(int i=0; i<permutationnum; i++)
    {
        for(int j=0; j<setsize[i+1]; j++)
        {
            binnas[i].push_back(j);
        }
    }


    call(0);

}


void initialize()
{
    FILE *fp = fopen("output.txt", "r");
    char *token;
    int i,res=-1;
    int cnt=0;
    if(fp != NULL)
    {
        char * pch;
        char line[10000];
        while(fgets(line, sizeof line, fp) != NULL )
        {

            pch = strtok (line,",");
            int x=0;
            while (pch != NULL )
            {
                string val=pch;

                if(cnt!=0)
                {


                    sets[cnt-1].push_back(val);
                }




                pch = strtok (NULL, ",");
            }
            cnt++;



        }

    }

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
