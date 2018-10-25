# _*_ coding: utf-8 _*_
import cv2
import numpy as np

SrcImage1 = cv2.imread('1.png')
SrcImage2 = cv2.imread('2.png')

ret1, corners1 = cv2.findChessboardCorners(SrcImage1, (9,6),None)# 找到棋盘格角点
ret2, corners2 = cv2.findChessboardCorners(SrcImage2, (9,6),None)

img1 = cv2.drawChessboardCorners(SrcImage1, (9,6), corners1,ret1)# 将角点画到图片中
img2 = cv2.drawChessboardCorners(SrcImage2, (9,6), corners2,ret2)

H, mask = cv2.findHomography(corners1, corners2, cv2.RANSAC,5.0)# 求解img1 到 img2 的单应矩阵, 简记为 corners2 = H*corners1

for srcpts in corners1:
	dstpts = np.dot(H,np.array([srcpts[0][0],srcpts[0][1], 1]))# 验证求解
	img2 = cv2.circle(img2,(int(dstpts[0]/dstpts[2]),int(dstpts[1]/dstpts[2])), 3, (0,0,255), -1)# 有一个归一化的过程


cv2.imshow('img1',img1)
cv2.imshow('img2',img2)
cv2.waitKey(0)
