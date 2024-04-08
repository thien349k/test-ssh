#include<iostream>
#include<vector>
#include<unordered_set>
#include<ctime>
#include<fstream>
#include<stdlib.h>
#include<iomanip>
#include<numeric>
#include<stdexcept>
using namespace std;
class Graph //Defining a Graph class
{
    public:
        string filename;
        vector<vector <int>> edges;
        vector<vector <bool>> adjmat;
        int nvertex,nedges,max_degree;
        Graph(string str);
        void printAdjMat();
};
Graph :: Graph(string str)//Graph Constructor
{
    filename=str;
    ifstream inData(str);
    if(inData.fail())
    {
        cout<<"\nFilename doesn't exist!!....\n";
        exit(1);
    }
    int x,y;
    char e;
    string s;
    inData >> s;
    while(s != "edge")
            inData >> s;
    inData>>nvertex>>nedges;
    cout<<"\nFile name:"<<filename<<endl;
    cout<<"\nVertex:"<<nvertex<<endl;
    cout<<"Edges:"<<nedges<<endl;
    adjmat.resize(nvertex,vector<bool>(nvertex,0));
    edges.resize(nedges,vector<int>(2));
    for (int i=0; i<nedges; i++)
    {
    	inData >> e >> x >> y;
        edges[i][0]=x;
        edges[i][1]=y;
        adjmat[x-1][y-1]=1;
        adjmat[y-1][x-1]=1;
    }
    int sum,max=0;
    for (int i = 0; i < nvertex; i++)
    {
        sum=accumulate(adjmat[i].begin(),adjmat[i].end(),0);
        if(max<sum)
            max=sum;
    }
    max_degree=max;
    cout<<"Max Degree: "<<max_degree<<endl;
    inData.close();
    if(max_degree+1==nvertex)
    {
        cout<<"\nChromatic Number: "<<nvertex;
        exit(1);
    }
}
void Graph :: printAdjMat()
{
    cout<<"Adjacent Matrix:"<<endl;
    int c=1;
    for (int i = 0; i < nvertex+1; i++)
    {
        for (int j = 0; j < nvertex+1; j++)
        {
            if(i==0)
			{
				if(i==0 && j==0)
					cout<<" \t";
				else
					cout<<j<<"\t";
			}
			else
			{
				if(j==0)
					cout<<i<<"\t";
				else
                    cout<<adjmat[i-1][j-1]<<"\t";
            }
        }
		cout<<endl;      
    }
}

class ACOGCP
{
    public:
        string filename;
        vector<vector <bool>> adjmat;
        vector<int> color;
        int max_deg1,vertex,ncolor,edges;
        bool result;
        ACOGCP(Graph const &g1);
        bool check();
        int countDistinct();
        void compute();
        void run();
        void printColor();
        void generateReport(string, int, int, int, int);
};
ACOGCP::ACOGCP(Graph const &g1)
{
    filename=g1.filename;
    max_deg1=g1.max_degree+1;
    vertex=g1.nvertex;
    edges=g1.nedges;
    adjmat.resize(vertex,vector<bool>(vertex));
    color.resize(vertex,1);
    for(int i=0; i<vertex; i++)
    {
        for (int j = 0; j < vertex; j++)
        {
            adjmat[i][j]=g1.adjmat[i][j];
        }
    }
}
void ACOGCP::compute()
{
    int value, j, k;
    bool result;
    for (j = 1; j <vertex; j++)
    {
        ncolor=countDistinct();
        result=check();
        if (result==1)
        {
            return;
        }
        for (k = 0; k < vertex; k++)
        {
            if (j!=k)
            {
                value=adjmat[j][k]*color[k];
                if (value==color[j])
                {
                    if (color[k]<=max_deg1)
                        color[k]=color[k]+1;
                    else
                        color[k]=color[k]%(+max_deg1)+1;
                    j=0;
                    k=0;
                }
            }
        }
    }
}
int ACOGCP::countDistinct()
{
    unordered_set<int> s;  
	int res = 0; 
	for (int i = 0; i < vertex; i++)
    { 
		if (s.find(color[i]) == s.end())
        { 
			s.insert(color[i]); 
			res++;
		} 
	}
	return res;
}
bool ACOGCP::check()
{
    for(int i=0;i<(vertex-1);i++)
    {
        for (int j = i+1; j<vertex; j++)
        {
            if((color[i]==color[j]) && (adjmat[i][j]))
            {
               return 0;
            }
        }
    }
    return 1;
}
void ACOGCP::printColor()
{
    for (int j = 0; j < vertex; j++)
    {
        cout<<color[j]<<"\t";
    }
}
void ACOGCP::generateReport(string filename, int vertex, int edges,int chromatic_number,int time_taken){
    fstream fstream_ob;
    fstream_ob.open("output.csv", ios::app);
    fstream_ob<<filename<<","<<vertex<<","<<edges<<","<<chromatic_number<<","<<time_taken<<endl;
    fstream_ob.close();
}
void ACOGCP::run()
{
    clock_t start, end;
    int a;
    cout<<"\n\nACOGCP:";
    start=clock();
    compute();
    result=check();
    end=clock();
    int chromatic_number=countDistinct();
    cout<<"\nChromatic Number: "<<chromatic_number<<endl;
    printColor();
    double time_taken = double(end - start)/double(CLOCKS_PER_SEC);
    cout << "\n\nExecution Time: " << fixed 
         << time_taken << setprecision(5); 
    cout << " sec " << endl;
    generateReport(filename,vertex,edges,chromatic_number,time_taken);
}
int main(int argc,char* argv[])
{
    try
    {
        if(argc==1)
            throw invalid_argument("No command line arguments specified. Filename not passed.");
        string file=argv[1];
        Graph g(file);
        //g.printAdjMat();
        ACOGCP aco(g);
        aco.run();
    }
    catch (const invalid_argument& e)
    {
        cout << "Error: " << e.what() << endl;
    }
}
