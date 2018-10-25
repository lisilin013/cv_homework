# _*_ coding: utf-8 _*_

import sys

#采用动态规划的思想
def GuessNumWithMinCost(n):
	min_cost = [ [0]*(n+1) for i in range(n+1) ]#方便从1开始访问下标

	# 1 ... j ... i ... n
	for i in xrange(2,n+1,1):
		for j in xrange(i-1,0,-1):# 先求[j,i]区间内的cost
			global_min = sys.maxsize # global_min使用一个较大的数初始化(eg: INT_MAX)
			for k in xrange(j+1,i,1):# 遍历区间[j,i], j+1<=k<=i-1,此处情况为i-j>=2
				local_max = k + max(min_cost[j][k-1], min_cost[k+1][i]) #local_max就是考虑局部最坏情况
				global_min = min(global_min, local_max) # global_min 就是考虑所有最坏情况后找出损失最小的情况
			if i-j>=2:
				min_cost[j][i] = global_min
			else: #i-j=1时
				min_cost[j][i] = j

	# print min_cost matrix
	# print("min cost matrix is: \n")
	# for i in xrange(2,n+1):
	# 	for j in xrange(1,i):
	# 		print min_cost[j][i],
	# 	print

	return min_cost[1][n]


if __name__ == '__main__':	
	n = int(raw_input("please input n: "))
	print("The Min Cost(%d) is: %d" % (n,GuessNumWithMinCost(n)))
