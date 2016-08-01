#include<iostream>
#include<fstream>
#include<QList>

#define N 25  // number of webpages
#define D 0.85 // damping factor
#define NORM 100 // we score in the [0, 100] interval

using namespace std;

int links[N][N], L[N]; // adjacency matrix and number of links on each page
double Rank[N];

void init() // setting of initial values
{
	for(int i=0; i<N; i++)
	{
		L[i]=0;
		double n=N;
		Rank[i]=1/n;
		cout<< Rank[i] << endl;
		for(int j=0; j<N; j++)
		{
			links[i][j]=-1;
		}
	}
}

void readdata() // reading data from file and computing L(i) at once
{
	ifstream infile("data.csv");
	for(int i=0; i<N; i++)
	{
		for(int j=0; j<N; j++)
		{
			if(i!=j)
			{
				infile >> links[i][j];
				if(links[i][j]>0)
				{
					L[i]++;
				}
			}
		}
	}
}

double sum(int ind) // the sum needed in the Rank formula
{
	double  ret=0;
	for(int i=0; i<N; i++)
	{
		if(links[ind][i]>0)
		{
			ret+=Rank[i]/(double)L[i];
		}
	}
	return ret;
}

bool desc(const QPair<int, double> & p1, const QPair<int, double> & p2) // helper for sorting the result
{
	if(p1.second > p2.second) return true;
	else return false;
}

void makeorder()
{
	QList<QPair<int, double> > list;
	double max=0;
	for(int i=0; i<N; i++)
	{
		list.append(QPair<int, double>(i+1, Rank[i]));
		if(Rank[i]>max)
		{
			max=Rank[i];
		}
	}

	qSort(list.begin(), list.end(), desc);
	for(int i=0; i<list.length(); i++)
	{
		int score=list[i].second*NORM/max;
		cout << list[i].first << "\t" << list[i].second << "\t" << score << endl;
	}
}

int main()
{
	int niter=0; // number of iterations
	while(niter<1)
	{
		cout << "So how many iterations would you like?" << endl;
		cin >> niter;
	}

	int all=niter; // we count all the performed iteration to have a neat answer at the end

	init();
	readdata();

	while(niter--)
	{
		for(int i=0; i<N; i++)
		{
			Rank[i]=(1-D)/(double)N+D*sum(i); // the PageRank formula itself
		}
		if(niter==0)
		{
			for(int i=0; i<N; i++)
			{
				cout << i+1 << "\t" << Rank[i] << endl;
			}
			cout << "How many more iterations would you like?\n";
			int more;
			cin >> more;
			if(more >= 0)
			{
				niter+=more;
				all+=more;
			}
			else cout << "Very funny...\n";
		}
	}

	cout << "Final order of webpages after " << all << " iterations:\n";

	makeorder();

	return 0;
}
