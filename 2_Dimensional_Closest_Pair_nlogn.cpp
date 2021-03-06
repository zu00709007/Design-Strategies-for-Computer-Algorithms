#include<stdio.h>
#include<limits.h>
#include<algorithm>

using namespace std;
int point_num, index = -1;
long long dis = LONG_LONG_MAX;

struct Point
{
    int x, y, index;
};

struct Ans
{
    int x, y;
};
struct Ans answer[100000];

inline bool sort_ans(struct Ans a, struct Ans b)
{
    if(a.x == b.x && a.y < b.y)
        return true;
    return a.x < b.x;
}

inline bool sort_x_coordinate(struct Point a, struct Point b)
{
    return a.x < b.x;
}

inline bool sort_y_coordinate(struct Point a, struct Point b)
{
    return a.y < b.y;
}

long long square(int a)
{
    return (long long)a * a;
}

void closest_pair(int point_num, struct Point point[])
{
    if(0 == point_num)
        return;

    int i, j, interval = 1, median_x, median = point_num >> 1;
    long long k;
    struct Point tmp[point_num+1], subleft[median+1], subright[point_num-median];

    //find the median of x coordinate
    for(i=0; i<=point_num; ++i)
        tmp[i] = point[i];
    nth_element(tmp, tmp + median, tmp + point_num + 1, sort_x_coordinate);
    median_x = tmp[median].x;

    //find the duplicated median
    for(i=median-1; i>=0; --i)
        if(tmp[i].x == tmp[median].x)
            ++interval;

    //divide the set by their x coordinate
    //use -1 to ensure that the answer be added once
    for(i=0, j=0, k=0; i<=point_num; ++i)
    {
        if(j <= median && point[i].x < median_x)
            subleft[j++] = point[i];
        else if(point[i].x == median_x && 0 < interval)
        {
            subleft[j++] = point[i];
            --interval;
        }
        else
        {
            subright[k++] = point[i];
            point[i].index *= -1;
        }
    }

    //conquer
    closest_pair(median, subleft);
    closest_pair(point_num - median - 1, subright);

    //merge the point in the distance
    interval = 0;
    for(i=0; i<=point_num; ++i)
        if(square(median_x - point[i].x) <= dis)
            tmp[interval++] = point[i];

    //find the most 5 point in the distance
    for(i=0; i<interval-1; ++i)
        for(j=1; j<=5 && i+j<interval; ++j)
        {
            k = square(tmp[i].x - tmp[i+j].x) + square(tmp[i].y - tmp[i+j].y);
            if(k <= dis)
                if(k == dis)
                {
                    if(tmp[i].index < 0 && tmp[i+j].index > 0)
                    {
                        ++index;
                        answer[index].x = -1 * tmp[i].index;
                        answer[index].y = tmp[i+j].index;
                        if(answer[index].x > answer[index].y)
                        {
                            answer[index].x ^= answer[index].y;
                            answer[index].y ^= answer[index].x;
                            answer[index].x ^= answer[index].y;
                        }
                    }
                    else if(tmp[i].index > 0 && tmp[i+j].index < 0)
                    {
                        ++index;
                        answer[index].x = tmp[i].index;
                        answer[index].y = -1 * tmp[i+j].index;
                        if(answer[index].x > answer[index].y)
                        {
                            answer[index].x ^= answer[index].y;
                            answer[index].y ^= answer[index].x;
                            answer[index].x ^= answer[index].y;
                        }
                    }
                }
                else
                {
                    dis = k;
                    index = 0;
                    answer[index].x = tmp[i].index > 0 ? tmp[i].index : -1 * tmp[i].index;
                    answer[index].y = tmp[i+j].index > 0 ? tmp[i+j].index : -1 * tmp[i+j].index;
                    if(answer[index].x > answer[index].y)
                    {
                        answer[index].x ^= answer[index].y;
                        answer[index].y ^= answer[index].x;
                        answer[index].x ^= answer[index].y;
                    }
                }
        }
}

int main()
{
    scanf("%d", &point_num);
    struct Point point[point_num];
    for(int i=0; i<point_num; ++i)
    {
        scanf("%d %d", &point[i].x, &point[i].y);
        point[i].index = i + 1;
    }
    //sort by y coordinate once
    sort(point, point + point_num, sort_y_coordinate);

    //start divide and conquer
    closest_pair(point_num - 1, point);
    printf("%lld %d\n", dis, index + 1);

    //sort the answer
    sort(answer, answer + index + 1, sort_ans);
    for(int i=0; i<=index; ++i)
        printf("%d %d\n", answer[i].x, answer[i].y);

    return 0;
}
