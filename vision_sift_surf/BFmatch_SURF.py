# -*- coding: utf-8 -*- 
# BFmatch_SIFT.py

# Created by 崔瑜翔 on 2019/12/1.
# Copyright © 2019年 崔瑜翔. All rights reserved.


# 任选两张同一场景的不同照片（要体现拍照角度，亮度，尺度变化），运行ＳＩＦＴ和ＳＵＲＦ算法，比较两个算法的优缺点


import numpy as np
import cv2
from matplotlib import pyplot as plt

# 读取图片
image1 = cv2.imread('/home/cyx/桌面/cv_homework/vision_sift_surf/INPUT/front.jpeg')
front_image = cv2.cvtColor(image1, cv2.COLOR_BGR2RGB)
front_gray = cv2.cvtColor(front_image, cv2.COLOR_RGB2GRAY)


side_image = cv2.imread('/home/cyx/桌面/cv_homework/vision_sift_surf/INPUT/side.jpeg')
side_image = cv2.cvtColor(side_image, cv2.COLOR_BGR2RGB)
side_gray = cv2.cvtColor(side_image, cv2.COLOR_RGB2GRAY)


# 显示图片
fig, plots = plt.subplots(1, 2, figsize=(20,10))

plots[0].set_title("FRONT Image")
plots[0].imshow(front_image)

plots[1].set_title("SIDE Image")
plots[1].imshow(side_image)
plt.show()
cv2.waitKey()

surf = cv2.xfeatures2d.SURF_create(800)

front_keypoints, front_descriptor = surf.detectAndCompute(front_gray, None)
side_keypoints, side_descriptor = surf.detectAndCompute(side_gray, None)

keypoints_without_size = np.copy(front_image)
keypoints_with_size = np.copy(front_image)

cv2.drawKeypoints(front_image, front_keypoints, keypoints_without_size, color = (0, 255, 0))

cv2.drawKeypoints(front_image, front_keypoints, keypoints_with_size, flags = cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

# 显示提取结果
fx, plots = plt.subplots(1, 2, figsize=(20,10))

plots[0].set_title("FRONT keypoints With Size")
plots[0].imshow(keypoints_with_size, cmap='gray')

plots[1].set_title("FRONT keypoints Without Size")
plots[1].imshow(keypoints_without_size, cmap='gray')

plt.show()
cv2.waitKey()

# 匹配
bf = cv2.BFMatcher(cv2.NORM_L1, crossCheck = False)
matches = bf.match(front_descriptor, side_descriptor)
# 按照距离排序，距离越小匹配效果好
matches = sorted(matches, key = lambda x : x.distance)

result = cv2.drawMatches(front_image, front_keypoints, side_gray, side_keypoints, matches[:50], side_gray, flags = 2)

# 显示最佳匹配结果
plt.rcParams['figure.figsize'] = [14.0, 14.0]
plt.title('Best Matching Points')
plt.imshow(result)
plt.show()


cv2.waitKey()

