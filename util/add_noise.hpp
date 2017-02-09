#ifndef ADD_NOISE
#define ADD_NOISE


#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
const double pi = acos(-1);
double randomDouble()
{
    double den = 2147483647;
    return rand()/den;
}
double randNormal(double std)//the mean value is zero;
{
    double t1 = randomDouble();
    double t2 = randomDouble();
    return std*sqrt(-2.0*log(t1))*cos(2.0*pi*t2);
}
template <typename Type>
void add_noise(vector<Type>& vec,double down_ratio, double noise)
{
    int newnum = vec.size() * (1.0 - down_ratio);
    int getout = vec.size() - newnum;
    vector<int> help(vec.size(), 0);
    vector<bool> rem(vec.size(), 1);
    for (int j = 0; j < vec.size(); j++)
    {
        help[j] = j;
    }
    for (int j = 0; j < getout; j++)
    {
        int position = rand() % (vec.size() - j);
        rem[help[position]] = 0;
        swap(help[position], help[vec.size() - j - 1]);
    }
    vector<Type> now(newnum);
    int add = 0;
    for (int j = 0; j < vec.size(); j++)
    {
        if (rem[j])
            now[add++] = vec[j];
    }
    for(int i=0;i<now.size();i++)
    {
        now[i].add_noise(noise);
    }
    swap(vec, now);
}

#endif // !ADD_NOISE


