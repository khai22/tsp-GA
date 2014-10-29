/*
   File: explode.h

   Description: Based on Zedwood's freely available explode function at
   http://www.zedwood.com/article/106/cpp-explode-function
*/

#ifndef _EXPLODE_H_
#define _EXPLODE_H_

#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
function: int_explode

Similar to explode(), but assumes all elements are integers and builds a vector
of ints.

*/
vector<int> int_explode( const string &delimiter, const string &str)
{
    vector<int> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng==0)
        return arr;//no change

    int i=0; 
    int k=0;
    while( i<strleng )
    {
        int j=0;
        while (i+j<strleng && j<delleng && str[i+j]==delimiter[j])
            j++;
        if (j==delleng)//found delimiter
        {
            arr.push_back(  atoi(str.substr(k, i-k).c_str()) );
            i+=delleng;
            k=i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(  atoi(str.substr(k, i-k).c_str()) );
    return arr;
}

/*
function: double_explode

Similar to explode(), but assumes all elements are doubles and builds a vector
of doubles.

*/
vector<double> double_explode( const string &delimiter, const string &str)
{
    vector<double> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng==0)
        return arr;//no change

    int i=0; 
    int k=0;
    while( i<strleng )
    {
        int j=0;
        while (i+j<strleng && j<delleng && str[i+j]==delimiter[j])
            j++;
        if (j==delleng)//found delimiter
        {
            arr.push_back(  strtod(str.substr(k, i-k).c_str(), NULL ) );
            i+=delleng;
            k=i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(  strtod(str.substr(k, i-k).c_str(), NULL) );
    return arr;
}
#endif
