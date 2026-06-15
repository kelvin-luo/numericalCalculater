# Numerical Calculator

数值计算演示程序（MFC + CMake + OpenCV + Eigen）。



## 功能

| #   | 方法     | 说明                  |
| --- | ------ | ------------------- |
| 0   | YUV 转换 | 图像 BGR → YUV / I420 |
| 1   | 矩阵运算   | Eigen 乘、逆、特征值、解方程   |
| 2   | 概率     | 二项、正态、贝叶斯           |
| 3   | 统计     | 描述统计、线性回归           |
| 4   | 逻辑斯回归  | 梯度下降二分类             |
| 5   | k-NN   | k 近邻与决策区域           |
| 6   | ANN    | MLP 求解 XOR          |
| 7   | CNN    | 卷积 + ReLU + 池化      |



## 快速开始

```bat
code\scripts\build.bat
code\scripts\run.bat
```

详细说明见 [docs/build_and_run.md](docs/build_and_run.md)，算法原理见 [docs/algorithms.md](docs/algorithms.md)。

程序截图：

![程序截图](.\docs\assets\程序截图.PNG)



## 目录

- `src/numcalc/` — 算法库
- `src/app/` — MFC 界面
- `scripts/` — 构建脚本
- `docs/` — 文档

版本：**1.0.0**
