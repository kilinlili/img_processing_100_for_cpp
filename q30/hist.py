import cv2
import numpy as np 
import matplotlib.pyplot as plt

# cv2.imreadでnumpy.ndarrayが得られる
# (height, width, channels)でBGR順．
# 何もしないとdtype = uint8で読み込み．

# numpy.ndarray.astypeを使って形式の変換
# 例:a = a.astype(np.float)/255.0で[0, 255] --> [0, 1.0]幅
# 例:a = a.astype(np.float)/128 - 1.0で[0, 255] --> [-1.0, 1.0)幅

img = cv2.imread("img/23q_ans.jpg").astype(np.float)

# http://labs.eecs.tottori-u.ac.jp/sd/Member/oyamada/OpenCV/html/py_tutorials/py_imgproc/py_histograms/py_histogram_begins/py_histogram_begins.html

plt.hist(img.ravel(), bins=255, rwidth=0.8, range=(0, 255)) 
plt.show()
