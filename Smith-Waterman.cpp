
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;



int idx; //case index 
double eq=4; //match score
double mmatch=-1; //mismatch penalty
double gapP=-10; //gap penalty

double similarityScore(char a, char b);
double findMax(double array[], int length);


//checks if the two characters are similar
double similarityScore(char a, char b)
{
	double result;
	if(a==b)
	{
		result=eq;
	}
	else
	{
		result=mmatch;
	}
	return result;
}

//finds the max value in the array
double findMax(double array[], int length)
{
	double max = array[0];
	idx = 0;

	for(int i=1; i<length; i++)
	{
		if(array[i] > max)
		{
			max = array[i];
			idx=i;
		}
	}
	return max;
}

int main()
{
	//initialisation for the two string that will be compared
	string seqA; 
	string seqB; 
	
	
	//read the input from console
	cout << "Enter Sequence A" << endl;
	cin >> seqA;
	cout << "Enter Sequence B" << endl;
	cin >> seqB;
	cout << endl << " " << "You typed in " << endl << seqA << endl << seqB << endl;
	
	
	//initialize matrix
	int lengthSeqA = seqA.length();
	int lengthSeqB = seqB.length();
	
	double matrix[lengthSeqB+1][lengthSeqA+1];
	for(int i=0;i<=lengthSeqB;i++)
	{
		for(int j=0;j<=lengthSeqA;j++)
		{
			matrix[i][j]=0;
		}
	}

	//initialize the two trackback matrix to store from where the score came for i and j index
	double traceback[4];
	
	int I_i[lengthSeqB+1][lengthSeqA+1];
	for(int i=0;i<=lengthSeqB;i++)
	{
		for(int j=0;j<=lengthSeqA;j++)
		{
			I_i[i][j]=0;
		}
	}
	
	int I_j[lengthSeqB+1][lengthSeqA+1];
	for(int i=0;i<=lengthSeqB;i++)
	{
		for(int j=0;j<=lengthSeqA;j++)
		{
			I_j[i][j]=0;
		}
	}


	//start populating matrix
	for (int i=1;i<=lengthSeqB;i++)
	{
		for(int j=1;j<=lengthSeqA;j++)
        {
			traceback[0] = matrix[i-1][j-1]+similarityScore(seqA[j-1],seqB[i-1]); //case equal
			traceback[1] = matrix[i-1][j]+gapP; //case gap in seq B
			traceback[2] = matrix[i][j-1]+gapP; //case gap in seq A
			traceback[3] = 0;
			matrix[i][j] = findMax(traceback,4);
			switch(idx)
			{
				case 0:
					I_i[i][j] = i-1;
					I_j[i][j] = j-1;
					break;
				case 1:
					I_i[i][j] = i-1;
                    I_j[i][j] = j;
                    break;
				case 2:
					I_i[i][j] = i;
                    I_j[i][j] = j-1;
                    break;
				case 3:
					I_i[i][j] = i;
                	I_j[i][j] = j;
                    break;
			}
        }
	}

	//prints the Smith-Waterman matrix
	cout << "Smith-Waterman matrix" << endl; 
	for(int i=0;i<lengthSeqB+1;i++)
	{
		for(int j=0;j<lengthSeqA+1;j++)
		{
			cout << matrix[i][j] << "   ";
		}
		cout << endl;
	}
	
	

	//finds the max score in the matrix
	double matrix_max = 0;
	int i_max=0, j_max=0;
	for(int i=0;i<=lengthSeqB;i++)
	{
		for(int j=0;j<=lengthSeqA;j++)
		{
			if(matrix[i][j]>matrix_max)
			{
				matrix_max = matrix[i][j];
				i_max=i;
				j_max=j;
			}
		}
	}

	cout << "Max score in the matrix is " << matrix_max << endl;
	

	//traceback part to find the best matching sequence
	
	int current_i=i_max,current_j=j_max;
	int next_i=I_i[current_i][current_j];
	int next_j=I_j[current_i][current_j];
	int eqSeqIdx=0;
	char sim_a[lengthSeqA+lengthSeqB+2],sim_b[lengthSeqA+lengthSeqB+2];

	
	while(((current_i!=next_i) || (current_j!=next_j)) && (next_j!=0) && (next_i!=0))
	{

		if(next_i==current_i)  sim_b[eqSeqIdx] = '-';                  //if deletion in A
		else                   sim_b[eqSeqIdx] = seqB[current_i-1];   //if match/mismatch in A

		if(next_j==current_j)  sim_a[eqSeqIdx] = '-';                  //if deletion in B
		else                   sim_a[eqSeqIdx] = seqA[current_j-1];   //if match/mismatch in B
		
		//cout << current_i << endl << current_j << endl;
		current_i = next_i;
		current_j = next_j;
		next_i = I_i[current_i][current_j];
		next_j = I_j[current_i][current_j];
		eqSeqIdx++;
	}
	

	

	//prints the sequences
	cout<<endl<<" "<<endl;
	cout<<"Alignment:"<<endl<<endl;
	for(int i=0;i<lengthSeqA;i++){cout<<seqA[i];}; cout<<"  and"<<endl;
	for(int i=0;i<lengthSeqB;i++){cout<<seqB[i];}; cout<<endl<<endl;  
	for(int i=eqSeqIdx;i>=0;i--) cout<<sim_a[i]; 
	cout<<endl;
	for(int j=eqSeqIdx;j>=0;j--) cout<<sim_b[j];
	cout<<endl;
	
	return 0;
}