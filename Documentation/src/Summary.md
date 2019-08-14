<!-- $theme: gaia -->

技术答辩 / SUMMARY
====
###### Group 7
<!-- *template: gaia -->

---

<!-- *template: invert -->

<center>   Presented By Group 7 Members <br><br><br>

**郑雨盺** / Yuxin  &nbsp;&nbsp;**廖洽源** / Qiayuan

**张哲慜** / Hisen &nbsp;&nbsp; **王冠** / One

**陈文潇** / Minovsky &nbsp;&nbsp;  **青雨晴** / Joy

**朱彦臻** / Yanzhen


</center>


---

# 项目特性 / FEATURES

- 完全自动控制 / 100% Autonomous
- 敏捷开发管理 / Agile Development
- 

---

# 软件 / Software

<!-- *template: gaia -->

---

## 妙算直驱 / Manifold&reg; Driven
<br>

率先提出使用"妙算"作为主控制器的方案

得益于这种架构, 软件开发复杂度大幅降低.

---
## 软件架构 / System Architecture
| Node | Usage | Description |
|:-:|:-:|:-:|
|RCBigAlgo | 决策 | 给出总体决策|
|RCBigController|运动控制|接收目标点规划轨迹; 控制运动|
|RCBigVision|视觉定位|读取视觉数据与定位|
|RCBigCar|驱动|负责硬件驱动及底盘驱动; 裁判系统数据处理|


---

## 双摄视觉 / Dual Camara Vision
<br>

双摄扩大视野, 捕获更多的视觉定位标签

Moving Average滤波并融合定位数据以提升精度

<br>

$$\sum\frac{(x, y, \theta)}{ D_{depth}}$$

---
## 模型预测控制 / MPC
PID控制面对复杂系统显得力不从心
引入==MPC==解决这一问题.


由于调试时间有限, 虽然MPC方案已经实现, 但没有启用.

---
## 自动决策 / Algorithm

- 应用`转置表+位棋盘优化的A*算法`节省内存与算力
- 搜索回合内所有决策
- 使用估价函数评估决策侧优劣
- 每五步重新动态决策一次
- 从裁判系统读取环境反馈

---

## 算法优化 / A* Enhancement
<br>

`A*` + `Transposition Table` + `Bitboard`

<br>

优化前搜索层数仅为6, 预测能力差
优化后搜索层数为28, 非常接近最优解(30层)

可以在0.5s内执行完毕, 满足实时动态决策的需求, 大幅提高系统应变能力


---
## 敏捷开发 / Agile Development
<br>
引入敏捷开发模型, 快速迭代以灵活应对需求变化.

<br><br>

<center>

**==15== 天研发
==3== Sprints**

</center>

---
## 开源社区 / Open Source

<br>

我们希望团队的成果可以帮助到更多的人
使用MIT开源许可

---
## 看板管理 / Trello

<br>



---

<!-- *template: gaia -->

# Pics


----

## 



---


# 机械 / Mechanics
<!-- *template: gaia -->

---


# 管理 / Management

<!-- *template: gaia -->