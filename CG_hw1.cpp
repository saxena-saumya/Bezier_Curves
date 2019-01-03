/* Author: Saumya Saxena
CS536: Computer Graphics - Assignment 1
Date: October 12, 2018
Description: Arbitrary-degree Bezier Curves
*/

/* Header Files */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <string.h>
#include <fstream>
#include <math.h>

using namespace std;

struct point
{
    double x,y,z;
};

/* Function to evaluate the binomial coefficient */
double binomialCoeff(double n, double k)
{
    double res = 1;

    if ( k > n - k )
        k = n - k;

    for (double i = 0; i < k; ++i)
    {
        res *= (n - i);
        res /= (i + 1);
    }

    return res;
}

/* Function implementing the bezier equation */
struct point bezierEquation(int n,double t, struct point points[100])
{
    struct point P;
    double x = 0,y = 0,z = 0;

    n=n-1;

    for(int i=0;i<=n;i++)
    {
        x = x + binomialCoeff(n,i) * pow((1-t),(n-i)) * pow(t,i) * points[i].x;
        y = y + binomialCoeff(n,i) * pow((1-t),(n-i)) * pow(t,i) * points[i].y;
        z = z + binomialCoeff(n,i) * pow((1-t),(n-i)) * pow(t,i) * points[i].z;
    }

    P.x = x;
    P.y = y;
    P.z = z;
    return P;
}

int main(int argc, char** argv)
{
    string filename;
    double du = 0.05,radius = 0.1;
    // loop to take arguments from command line else take up default values
    for (int i = 0; i < argc; ++i)
    {
        if( strcmp(argv[i],"-f") == 0)
        {      
           i++;
           filename = argv[i];
        }
        else if(strcmp(argv[i], "-u") == 0)
        {
           i++;
           du = atof(argv[i]);
           //cout<<argv[i]<<" ";
        }
        else if(strcmp(argv[i], "-r") == 0)
        {
           i++;
           radius = atof(argv[i]);
           //cout<<argv[i]<<" ";
        }
    }
  
    ifstream file;
    string line;
    file.open(filename.c_str());
    struct point points[100];

    int n = 0;
    // loop to read from the input file
    for(;;)
    {
        getline(file,line);
	 	if(file.eof()) {
            break;
        }

        struct point p;

        std::stringstream linestream(line);

        double value;
	
        linestream >> value;

        p.x = value;
	
        linestream >> value;

        p.y = value;

        linestream >> value;

        p.z = value;

        points[n] = p;

        n++;
    }
    
    cout<<"#Inventor V2.0 ascii \n\nSeparator {LightModel {model BASE_COLOR} Material {diffuseColor 1.0 1.0 1.0} \n";
    cout<<"Coordinate3 { 	point [\n";
    // loop to evaluate points 
    for(double t = 0.0 ;t <= 1.0;t += du)
    {
       struct point p = bezierEquation(n,t,points);
       cout<<p.x<<" "<<p.y<<" "<<p.z<<"\n";
    }
    cout<< points[n-1].x<<" "<<points[n-1].y<<" "<<points[n-1].z<<"\n";
    cout<< "] }\n";
    cout<< "IndexedLineSet {coordIndex [\n";
    int q = 0;
    for(double t = 0.0 ;t <= 1.0;t += du)
    { 
       cout<< q <<", ";
       q++;
    }
    cout<< q <<", ";
    cout<<"-1,\n";
    cout<<"] } }\n";
    for(int i=0;i<n;i++)
    {
        cout<<"Separator {LightModel {model PHONG}Material {	diffuseColor 1.0 1.0 1.0}\n";
        cout<<"Transform {translation\n";
        cout<<points[i].x<<" "<<points[i].y<<" "<<points[i].z;
        cout<<"}Sphere {	          radius " <<radius<<" }}";
    }
    return 0;
}
