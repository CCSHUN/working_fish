/*
  leecode题目 合并
*/
#include <vector>
#include <functional>
#include <iostream>
#include <algorithm>

using namespace std;

vector<vector<int>> merge(vector<vector<int>>& intervals) {
        vector<vector<int>> res;
        std::sort(intervals.begin(), intervals.end());
        int i = 0;
        while (i < intervals.size()) {
            if ((i + 1) < intervals.size()) {
                if (intervals[i][1] > intervals[i+1][0]) { //需要合并
                    //合并,取相邻两个区间最大值
                    int max = std::max(intervals[i][1], intervals[i+1][1]);
                    int min = intervals[i][0];
                    //res.emplace_back(min, max);
                    res.push_back(vector<int>(min,max));
                    //交换
                    std::swap(min, intervals[i+1][0]);
                    std::swap(max, intervals[i+1][1]);
                } 
            } else {
                res.emplace_back(intervals[i]);
            }
            i++;
        }
        return res;
    }

int main() {
  vector<vector<int>> vc = {{1,3},{2,6},{8,10},{15,18}};
  merge(vc);

  vector<int> test(1,2);
  return 0;
}