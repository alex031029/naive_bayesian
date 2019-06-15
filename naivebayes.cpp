#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<string>
#include<string.h>
#include<map>
#include<iostream>
#include<ios>
#include<cctype>
#include<algorithm>
#include<iomanip>
#include<stdexcept>
#include<string>
#include<fstream>
#define N 56
using namespace std;
vector< map<string,int> > part;     //to record the number of times of each word appearing in each group
//FILE *f;
double rate[20];
double fuzz_rate[20];
char filename[20][30]={"alt.atheism","comp.graphics","comp.os.ms-windows.misc","comp.sys.ibm.pc.hardware","comp.sys.mac.hardware","comp.windows.x","misc.forsale","rec.autos","rec.motorcycles","rec.sport.baseball","rec.sport.hockey","sci.crypt","sci.electronics","sci.med","sci.space","soc.religion.christian","talk.politics.guns","talk.politics.mideast","talk.politics.misc","talk.religion.misc"};
string useless[]={"is","are","am","i","you","he","it","she","that","this","these","those","the","a","an","not","do","does","one","two","day","good","we","they","them","my","your","his","her","him","mine","me","new",";",",","\"","'",".","<",">","com","www","@","(",")","and","or","day","to","of","was","were","as","so","then","-"};
int fuzz[20][20]={{0},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},
{1,2,3,4,5},{6,19,18,17},{7,8,9,10},{7,8,9,10},{7,8,9,10},
{7,8,9,10},{11,12,13,14},{11,12,13,14},{11,12,13,14},{11,12,13,14},
{15,19},{16,17,18,19},{6,16,17,18,19},{6,16,17,18,19},{6,16,17,18,19,15}};
void b2s(string &t)     //change the capital letters of a string into lowercase letters
{
     string temp;
     for(int i=0;i<t.size();i++)
     {
             if(t[i]>='A'&&t[i]<='Z')
                t[i]=t[i]-'A'+'a';
     }
     if(t[t.size()-1]>'z'||t[t.size()-1]<'a')
         t[t.size()-1]='\0';
 }
void init()
{
    for(int i=0;i<20;i++)
    {
        map<string,int> tempv;
        for(int j=0;j<100;j++)
        {
            map<string,int> check;
            ifstream is;
            char fname[80]="mini_newsgroups/";
            strcat(fname,filename[i]);
            char temp[20];
            temp[0]='/';
            int bias=0;
            
            if(i<10)
            {
               temp[1]=i+'0';
            }
            else
            {
                temp[1]=i/10+'0';
                temp[2]=i%10+'0';
                bias=1;
            }
            temp[2+bias]=' ';
            temp[3+bias]='(';
            if(j<10)
            {
                temp[4+bias]=j+'0';
                temp[5+bias]=')';
                temp[6+bias]='\0';
            }
            else
            {
                temp[4+bias]=j/10+'0';
                temp[5+bias]=j%10+'0';
                temp[6+bias]=')';
                temp[7+bias]='\0';
            }
            strcat(fname,temp);
            //cout<<fname;
            is.open(fname);    //open the file
            string t;
            //getline(is,t,' ');
            //cout<<i<<j<<fname<<endl;
            while(getline(is,t,' '))
            {
                //system("pause");
                b2s(t);
               // cout<<t;
               // system("pause");
                bool flag=0;
                for(int k=0;k<N;k++)
                {   
                    if(useless[k]==t)
                    {
                        flag=1;
                        break;
                    }
                }
                if(flag==0&&check[t]==0)
                {
                    tempv[t]++;        //count the frequent
                    check[t]=1;
                }
            }
            //system("pause");
        }
        part.push_back(tempv);
    }
}
int bayse()
{
    double p[20][20];
    for(int n=0;n<20;n++)
    {
         ifstream is;
         ofstream out;
         char fname[5];
         if(n<10)
         {
                    fname[0]=n+'0';
                    fname[1]='\0';
         }
         else
         {
                    fname[0]=n/10+'0';
                    fname[1]=n%10+'0';
                    fname[2]='\0';
         }
         
         is.open(fname);          //the test data
         strcat(fname,".out");
         out.open(fname);         //the result
         string t;
         int cnt=0;
         map<string,int> check;   //to mark whether this word has been put into the vector test 
         vector<string> test;     //record the frequent of each word
         while(getline(is,t,' '))
         {
             b2s(t);
             bool flag=0;
             for(int k=0;k<N;k++)
             {
                 if(useless[k]==t)
                 {
                     flag=1;
                     break;
                 }
    
             }
             if(flag==0&&check[t]==0)
             {
                test.push_back(t);
                check[t]=1;
                cnt++;
             }
         }
         //cout<<test[0]<<endl;
         for(int i=0;i<20;i++)
         {
                 double right=1.0;
                 //cout<<test.size();
                 for(int j=0;j<test.size();j++)
                 {
                        // cout<<(1+part[i][test[j]])/2002.0;
                         right*=(1+part[i][test[j]])/22.0;
                        // system("pause");
                         //cout<<right;
                         
                 }
                 p[n][i]=right;
                 out<<i<<"\t"<<filename[i]<<"\t"<<p[n][i]<<endl;               
         }
   }  
}
void accurate()   //to compute te accurate rate
{
    for(int i=0;i<20;i++)
    {
        for(int j=100;j<200;j++)
        {
            ifstream is;
            ofstream out;
            char fname[80]="20_newsgroups/";
            strcat(fname,filename[i]);
            char temp[20];
            temp[0]='/';
            int bias=0;
            if(i<10)
            {
               temp[1]=i+'0';
            }
            else
            {
                temp[1]=i/10+'0';
                temp[2]=i%10+'0';
                bias=1;
            }
            temp[2+bias]=' ';
            temp[3+bias]='(';
            temp[4+bias]=j/100+'0';
            temp[5+bias]=(j%100)/10+'0';
            temp[6+bias]=j%10+'0';
            temp[7+bias]=')';
            temp[8+bias]='\0';
            
            strcat(fname,temp);
            //cout<<fname;
            is.open(fname);    //open the file
            string t;
            //getline(is,t,' ');
            //cout<<i<<j<<fname<<endl;
            //system("pause");
            map<string,int> check;   //to mark whether this word has been put into the vector test 
            vector<string> test;     //record the frequent of each word
             while(getline(is,t,' '))
             {
                 b2s(t);
                // cout<<t;
                // system("pause");
                 bool flag=0;
                 for(int k=0;k<N;k++)
                 {
                     if(useless[k]==t)
                     {
                         flag=1;
                         break;
                     }
        
                 }
                 if(flag==0&&check[t]==0)
                 {
                    test.push_back(t);
                    check[t]=1;
                   // cnt++;
                 }
             }
             //cout<<test[0]<<endl;
             double p[20];
             for(int n=0;n<20;n++)
             {
                     double right=1.0;
                     //cout<<test.size();
                     for(int m=0;m<test.size();m++)
                     {
                            // cout<<(1+part[i][test[j]])/2002.0;
                             right*=(1+part[n][test[m]])/2.0;
                            // system("pause");
                             //cout<<right;
                             
                     }
                     p[n]=right;
                     cout<<n<<"\t"<<filename[i]<<"\t"<<p[n]<<endl;               
             }
           
           double max=-10.0;
           int flag=5;
           for(int n=0;n<20;n++)  //to find the maximum of 20 probability
           {
                   if(p[n]>=max)
                   {
                               max=p[n];
                               flag=n;
                   }
                   
           }
           //cout<<flag;
           if(flag==i)               //if the classification is a correct
           {
                      rate[i]++;
           }
           for(int t=0;t<6;t++)
           {
                   if(flag==fuzz[i][t])
                   {  
                        fuzz_rate[i]++;
                        break;
                   }
           }
        }
    }
}
int main(int argc, char *argv[])
{

    init();
    //bayse();
    accurate();
    FILE *f2;
    f2=fopen("bool_rate.out","w");
    double totalrate=0.0;
    double fuzz_totalrate=0.0;
    double fuzz_sx=0.0;
    double sx=0.0;
    for(int i=0;i<20;i++)
    {
            fuzz_sx+=fuzz_rate[i]*fuzz_rate[i]/10000.0;
            sx+=rate[i]*rate[i]/10000.0;
            totalrate+=rate[i];
            fuzz_totalrate+=fuzz_rate[i];
            fprintf(f2,"accurate rate of group %i is %lf, the fuzz accurate rate is %lf\n",i,rate[i]/100,fuzz_rate[i]/100);
    }
    fuzz_totalrate/=2000.0;
    totalrate/=2000;
    fprintf(f2,"accurate rate of all groups is %lf,the fuzz accurate is %lf\n",totalrate,fuzz_totalrate);
    sx-=totalrate*totalrate*20;
    fuzz_sx-=fuzz_totalrate*fuzz_totalrate*20;
    sx/=19.0;
    fuzz_sx/=19.0;
    fprintf(f2,"variance of accurate rate of each groups is %lf,the fuzz accurate is %lf\n",sx,fuzz_sx);

}
