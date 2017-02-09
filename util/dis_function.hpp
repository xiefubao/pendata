#ifndef DIS_FUNCTION
#define DIS_FUNCTION



#include <fstream>
#include <cmath>
#include <climits>
#include <cstdlib>
#include <vector>

double coe = 0;
double INF = 1e10;
template <typename Type>
void get_nearest_md(vector<Type>& tra,vector<Type>& sub,int& beg,int& end)
{
    int n = tra.size();
    int m = sub.size();

    vector<vector <int> > beginA(m,vector<int>(n ,-1));
    vector<vector <int> > beginB(m,vector<int>(n,-1));
    vector<vector <double> > dpA(m,vector<double>(n,0));
    vector<vector <double> > dpB(m,vector<double>(n,0));

    vector<double> prefix(m+1,0);
    for(int i = 1;i < m ;i++)
    {
        prefix[i] = prefix[i - 1] + dis(sub[i],sub[i - 1]);
    }
    dpB[0][0] = dis(sub[0],tra[0])*coe;
    beginB[0][0] = 0;
    for(int i = 1;i < m;i++)
    {
        double be = dpA[i-1][0]+dis(sub[i],sub[i-1]);
        double af = dpB[i-1][0]+dis(sub[i],tra[0])*coe;
        if(be <= af)
        {
            dpA[i][0] = be;
            beginA[i][0] = beginA[i-1][0];
        }
        else
        {
            dpA[i][0] = af;
            beginA[i][0] = 0;
        }
        
        dpB[i][0] = prefix[i]+dis(sub[i],tra[0])*coe;
        beginB[i][0] = 0;
    }
    for(int i = 0;i < n;i++)
    {
        dpA[0][i] = 0;
        beginA[0][i] = -1;
        dpB[0][i] = dis(sub[0],tra[i])*coe;
        beginB[0][i] = i;
    }
    for(int i = 1;i < m;i++)
    for(int j = 1;j < n;j++)
    {
        double be = dpA[i - 1][j] + dis(sub[i - 1],sub[i]);
        double af = dpB[i - 1][j] + dis(sub[i],tra[j]) * coe;
        if(be<=af)
        {
            dpA[i][j] = be;
            beginA[i][j] = beginA[i - 1][j];
        }
        else
        {
            dpA[i][j] = af;
            beginA[i][j] = beginB[i - 1][j];
        }


        be = dpA[i][j - 1]+dis(sub[i],tra[j])*coe;
        af = dpB[i][j - 1]+dis(tra[j],tra[j - 1]);//coe2
        if(be<=af)
        {
            dpB[i][j] = be;
            beginB[i][j] = beginA[i][j - 1];
            if(beginA[i][j - 1] == -1)
            {
                beginB[i][j] = j;
            }
        }
        else
        {
            dpB[i][j] = af;
            beginB[i][j] = beginB[i][j - 1];
        }
    }
    double ans = prefix[m - 1];
    for(int i= 0;i < n;i++)
    {
        if(dpA[m - 1][i] < ans)
        {
            ans = dpA[m - 1][i];
            beg = beginA[m - 1][i];
            end = i;
        }
    }  
}

template <typename Type>
void get_nearest_dtw(vector<Type>& tra,vector<Type>& sub,int& beg,int& end)
{
    int n=tra.size();
    int m=sub.size();
    vector<vector <double> > dp(m ,vector<double>(n,0));
    vector<vector <int> > begin(m,vector<int>(n,-1));
    for(int i=0;i<n;i++)
    {
        dp[0][i]=dis(sub[0],tra[i]);
        begin[0][i]=i;
    }
    for(int i=1;i<m;i++)
    {
        dp[i][0]=dp[i-1][0]+dis(sub[i],tra[0]);
        begin[i][0]=0;
    }
    for(int i=1;i<m;i++)
    for(int j=1;j<n;j++)
    {
        if(dp[i-1][j-1]<=dp[i-1][j] && dp[i-1][j-1]<=dp[i][j-1])
        {
            dp[i][j]=dp[i-1][j-1];
            begin[i][j]=begin[i-1][j-1];
        }
        else if(dp[i-1][j]<=dp[i-1][j-1] && dp[i-1][j]<=dp[i][j-1])
        {
            dp[i][j]=dp[i-1][j];
            begin[i][j]=begin[i-1][j];
        }
        else
        {
            dp[i][j]=dp[i][j-1];
            begin[i][j]=begin[i][j-1];
        }
        dp[i][j] += dis(sub[i],tra[j]);
    }
    double ans = INF;
    for(int i=0;i<n;i++)
    {
        if(ans>dp[m-1][i])
        {
            ans=dp[m-1][i];
            beg=begin[m-1][i];
            end=i;
        }
    }
}

template <typename Type>
void get_nearest_ed(vector<Type>& tra,vector<Type>& sub,int& beg,int& end)
{
    double ans = INF;
    int n=tra.size();
    int m=sub.size();
    for(int i = 0;i<n-m+1;i++)
    {
        double disnow = 0;
        for(int j = 0;j<sub.size();j++)
        {
            disnow += dis(sub[j],tra[j + i]);
        }
        if(ans > disnow)  
        {
            ans = disnow;
            beg = i;
            end = i + m - 1;
        }
    }
}


template <typename Type>
double getdistance_dtw(vector<Type>& one, vector<Type>& two)
{
    int n = one.size();
    int m = two.size();
    vector<vector <double> > dp(n ,vector<double>(m,0));
    dp[0][0] = dis(one[0], two[0]);
    for (int i = 1; i < n; i++)
    {
        dp[i][0] = dp[i - 1][0] + dis(one[i], two[0]);
    }
    for (int i = 1; i < m; i++)
    {
        dp[0][i] = dp[0][i - 1] + dis(one[0], two[i]);
    }
    for (int i = 1; i < n; i++)
        for (int j = 1; j < m; j++)
        {
            dp[i][j] = dis(one[i], two[j]) + min(dp[i - 1][j], min(dp[i][j - 1], dp[i - 1][j - 1]));
        }
    return dp[n - 1][m - 1];
}

#endif