# _*_ coding: utf-8 _*_
# import sympy
import numpy as np
import matplotlib.pyplot as plt

# 计算函数f的雅克比矩阵，是解析式
# f, a, b, x = sympy.symbols('f a b x')
# f=a*sympy.cos(b*x)+b*sympy.sin(a*x);

# funcs = sympy.Matrix([f])
# args = sympy.Matrix([a, b])
# Jsym = funcs.jacobian(args)
# print(Jsym)

# 拟合用数据
x_data = np.array([ 0.  , 0.05128205, 0.1025641,  0.15384615, 0.20512821, 0.25641026, \
    0.30769231, 0.35897436, 0.41025641, 0.46153846, 0.51282051, 0.56410256, \
    0.61538462, 0.66666667, 0.71794872, 0.76923077, 0.82051282, 0.87179487, \
    0.92307692, 0.97435897, 1.02564103, 1.07692308, 1.12820513, 1.17948718, \
    1.23076923, 1.28205128, 1.33333333, 1.38461538, 1.43589744, 1.48717949, \
    1.53846154, 1.58974359, 1.64102564, 1.69230769, 1.74358974, 1.79487179, \
    1.84615385,1.8974359,1.94871795,2.  ])
y_data = np.array([ 50.14205309,74.52945195,-125.60406092,50.73876215,-78.06990572,\
    69.07600961,72.59307969,-89.85886577,49.63661953,-116.99899809,\
    75.61742457,54.64519908,-47.2762507,53.40895121,-143.71858796,\
    72.98163061,26.37395524,-2.60377048,65.1767561,-148.47687307,\
    64.41096274,-10.09721052,35.12977766,71.16446338,-138.68616457,\
    55.83174324,-55.22601629,61.38557638,75.7080723,-109.84633432,\
    48.68186317,-95.74894683,71.69891297,66.86944822,-70.12822282,\
    52.36352661,-130.17602848,74.33260689,44.69661749,-26.65394757])
 
# 2. LM算法
# 初始猜测s
# best estimate
a_best = 0
b_best = 0
(AL,AR)=(50,100)
(BL,BR)=(50,100)
e_lm = np.array([])
pre_e_lm = np.array([10000000])
for ai in np.linspace(AL,AR,(AR-AL)*2+1):
    for bi in np.linspace(BL,BR,(BR-BL)*2+1):
        a0=ai; b0=bi;
        y_init = a0*np.cos(b0*x_data)+b0*np.sin(a0*x_data)
        # 数据个数
        Ndata=len(y_data);
        # 参数维数
        Nparams=2;
        # 迭代最大次数
        n_iters=50;
        # LM算法的阻尼系数初值
        lamda=0.01;
         
        # step1: 变量赋值
        updateJ=1;
        a_est=a0;
        b_est=b0;

        # error
        e = 100

        # step2: 迭代
        for it in xrange(0,n_iters):
            if updateJ==1:
                # 根据当前估计值，计算雅克比矩阵
                J=np.zeros([Ndata,Nparams])
                for i in xrange(0,len(x_data)):
                    J[i,:] = np.array([(b_est*x_data[i]) + b_est*x_data[i]*np.cos(a_est*x_data[i]), \
                        np.sin(a_est*x_data[i]) - a_est*x_data[i]*np.sin(b_est*x_data[i])])
                # 根据当前参数，得到函数值
                y_est = a_est*np.exp(-b_est*x_data);
                y_est = a_est*np.cos(b_est*x_data)+b_est*np.sin(a_est*x_data)
                # 计算误差
                d=y_data-y_est
                # 计算（拟）海塞矩阵
                H=np.dot(np.transpose(J),J)
                # 若是第一次迭代，计算误差
                if it==1:
                    e=np.dot(np.transpose(d),d)

            # 根据阻尼系数lamda混合得到H矩阵
            H_lm=H+(lamda*np.eye(Nparams,Nparams));
            # 计算步长dp，并根据步长计算新的可能的\参数估计值
            dp=np.linalg.inv(H_lm)*(np.dot(np.transpose(J), d));
            g = np.dot(np.transpose(J),d)
            a_lm=(a_est+dp[0])[0]
            b_lm=(b_est+dp[1])[0]
            # 计算新的可能估计值对应的y的残差e
            d_lm=y_data-np.array([a_lm*np.cos(b_lm*x_data)+b_lm*np.sin(a_lm*x_data)])
            e_lm=np.dot(d_lm,np.transpose(d_lm))
            # 根据误差，决定如何更新参数和阻尼系数
            # print('tmp error: %f' % (e_lm[0][0]))
            if e_lm<e:
                lamda=lamda/10;
                a_est=a_lm;
                b_est=b_lm;
                e=e_lm;
                updateJ=1;
            else:
                updateJ=0;
                lamda=lamda*10;

        if e_lm<pre_e_lm:
            pre_e_lm = e_lm
            a_best = a_est
            b_best = b_est
            print("error: %f, a_best: %f, b_best: %f" % (pre_e_lm[0][0],a_best,b_best))

#显示优化的结果
print ('Algorithm Run Done!\nThe best estimate of a,b is : ',a_best, b_best)
x_curve = np.linspace(-0.5, 2.5, 1000)
y_curve = a_best*np.cos(b_best*x_curve)+b_best*np.sin(a_best*x_curve)

plt.plot(x_curve, y_curve, 'r')
plt.plot(x_data, y_data, 'go')
plt.title('LM Algorithm')
plt.show()
