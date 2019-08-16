<!-- $theme: gaia -->
<!-- page_number: true -->

技术答辩 / SUMMARY
====
###### Group 7
<!-- *template: gaia -->
<!-- *page_number: false -->

---

<!-- *template: invert -->
<!-- *page_number: false -->

<center>   Presented By Group 7 Members <br><br><br>

**郑雨盺** / Yuxin  &nbsp;&nbsp;**廖洽源** / Qiayuan

**张哲慜** / Hisen &nbsp;&nbsp; **王冠** / One

**陈文潇** / Minovsky &nbsp;&nbsp;  **青雨晴** / Joy

**朱彦臻** / Yanzhen


</center>


---

# 项目特性 / FEATURES
<br>

- 完全自动控制 / 100% Autonomous
- 完全开源 / 100% Open Source
- 敏捷开发管理 / Agile Development


---

# 机械 / Mechanics
<!-- *page_number: false -->
<!-- *template: gaia -->

---

## 我们要做什么
<br>

- 提供稳定可靠的运行平台
- 可以实现所有的比赛元素动作
- 快速连续的放置能量球
- 快速稳定的放置能量仓
- 外形美观

---

## 如何完成
<br>

- 完整的soildworks建模数据
- 使用Soildworks运动算例中的motion分析进行可行性评估
- 毫米级的做工
- 科学的布局（底盘电机位置、重心位移评估、装置干涉排除）

---

## 系统设计
<br>

底盘系统
    存球系统
        供球系统
            存杯系统
                放杯系统
                      升降结构

---

## 底盘结构1.0

按照嵌入式和算法队员的指标，1.0版本的底盘形状是一个近似400mmx400mm的正方形

目的：让四个麦克纳姆轮的轴心成一个正方形，有助于嵌入式和算法队员路径规划的调试，减小嵌入式和算法队员工作的难度。

舍弃原因:由于最大的伸展宽度为600mmx600mmx1000mm，考虑到放置机构需要伸出大约200mm才能实现功能，底盘又必须为正方形，所有底盘的尺寸严重受限，出现了上大下小的情况。不能保证高速移动中的稳定性，故舍弃。


---

## 底盘结构2.0（正在采用）


通过与嵌入式和算法队员的协商，最后商定的方案是一个400*600的矩形底盘，并使用4个3508电机直连麦克纳姆驱动。

实行原因：放弃了对正方形底盘的一味追求，反而获得了更稳定的结构和调试空间

---
![](/home/hisenzhang/Documents/底盘结构2.0.png)

---
## 存、供球机构1.0

由于考虑到可携带的球数量已知且中通不需要再额外收集或手动添加更多的球，所以采用了30颗球依次排列的方法。使用单槽拨轮比多槽拨轮更为可靠，减小卡弹风险。

舍弃原因：30颗能量球的重量远远超过我们的预期，且顺序排列占用的空间太大，超过了加工的最大尺寸。

---
为防止卡球，我们决定使用单层圆形拨盘的结构，并且只开出一个槽口，逐个依次供球，为了防止多槽时小球容易滑落和卡球的不稳定情况。
舍弃原因：
       单层拨盘边缘摩擦力较大，尽管是圆形拨盘，测试时依然多次出现卡球情况

---
![](/home/hisenzhang/Documents/存供求机构1.0.png)



---

## 存、供球机构2.0（正在采用）

改进：
锯短原本的存球机构
将要放入能量仓的10颗能量球和直接放置的20颗能量球分别放置，20颗能量球
分为10颗10颗分别放在机构的左右两边并固定高度，可直接放球，无需升降，
10颗需要放入能量仓的能量球放置于能量仓上方依靠重力通过拨盘直接滚入仓中
使用双层拨盘，并在拨盘间加入平推轴承和滚珠轴承

---
改进拨盘形状，放弃单纯的圆形，为后来装配的滚珠轴承留下空间
依旧使用单槽口，逐个依次送球
使用2006电机驱动
测试供球十分流畅，极少出现卡球情况
改造原本的存球机构
设计新的独立的存球机构
2颗2颗出球，为机器放球提供更多数量上的选择
（一个一个太慢了）

---
![](/home/hisenzhang/Documents/存供2.0-2.png)

---
![](/home/hisenzhang/Documents/存供2.0-3.png)


---

![](/home/hisenzhang/Documents/存供2.0-1.png)
![](/home/hisenzhang/Documents/存供2.0-4.png)

---
## 存杯机构1.0

为防止取杯时无法实现逐个依次取杯。我们设计了卡杯的结构（如图）


但由于没有找到合适的弹簧和皮筋，锁定卡片无法按照设想弹回，解除对能量仓的锁定。此机构开发失败。

---
![](/home/hisenzhang/Documents/存杯机构1.0.png)

---

## 存杯机构1.1

我们希望使用组委会提供的弹簧，所以对存杯机构做出了新的改进（如图）

由于此版本组装困难且复杂，时间有限，我们并没有将它制作完成

---

## 存杯机构2.0（正在使用）

使用扎带和502胶水
扎带卡住能量仓
502胶水固定扎带

存在问题：
        嵌入和算法调试难度极大
        
---


## 放杯机构1.0（正在使用）

使用3个自由度的机械爪实现对能量仓进行抓取、放球、放杯功能

2舵机、1丝杆驱动（随升降装置上下移动）

---

![](/home/hisenzhang/Documents/放杯1.0.png)


---

## 升降机构1.0（正在使用）

使用顺滑且稳定的导程较大的滚珠丝杆实现上层机构的升降

出现问题：
丝杆购买严重失败，丝杆质量过重，不仅使机器整体质量偏大，还使得嵌入和算法队员难以控制底盘。
丝杆的调试也十分困难。

---

![](/home/hisenzhang/Documents/升降1.0.png)

---


# 软件 / Software

<!-- *template: gaia -->
<!-- *page_number: false -->

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

## 双摄视觉 / Dual Camera Vision
<br>

双摄扩大视野, 捕获更多的视觉定位标签

Moving Average滤波并融合定位数据以提升精度

<br>

$$\sum\frac{(x, y, \theta)}{ D_{depth}}$$

---
## 自动决策 / Algorithm

- 应用增强后的A*算法
- 搜索回合内所有决策
- 使用估价函数评估决策侧优劣
- 每五步重新动态决策一次
- 决策转化为动作序列执行
- 裁判系统读取反馈判断放杯/放球/占领是否成功


---

## 算法增强 / A* Enhancement
<br>

`A*` + `Transposition Table` + `Bitboard`

<br>

优化前仅搜索6层, 预测能力差
优化后搜索==28==层, 非常接近最优解(30层)

可以在==0.5s==内执行完毕, 满足实时动态决策的需求, 大幅提高系统应变能力


---

## Locomotion
<!-- *template: gaia -->
<!-- *page_number: false -->

---
![](/home/hisenzhang/Documents/robomaster/LatexPPT/RM_PNG/RM-2.png)
<!-- *template: gaia -->

---
![](/home/hisenzhang/Documents/robomaster/LatexPPT/RM_PNG/RM-3.png)
<!-- *template: gaia -->

---
![](/home/hisenzhang/Documents/robomaster/LatexPPT/RM_PNG/RM-4.png)
<!-- *template: gaia -->

---
![](/home/hisenzhang/Documents/robomaster/LatexPPT/RM_PNG/RM-5.png)
<!-- *template: gaia -->

---
![](/home/hisenzhang/Documents/robomaster/LatexPPT/RM_PNG/RM-7.png)
<!-- *template: gaia -->

---
![](/home/hisenzhang/Documents/robomaster/LatexPPT/RM_PNG/RM-8.png)
<!-- *template: gaia -->

---
![](/home/hisenzhang/Documents/robomaster/LatexPPT/RM_PNG/RM-9.png)
<!-- *template: gaia -->

---
![](/home/hisenzhang/Documents/robomaster/LatexPPT/RM_PNG/RM-10.png)
<!-- *template: gaia -->

---
## 敏捷开发 / Agile Development
<br>
引入敏捷开发模型, 快速迭代以灵活应对需求变化.

<br><br>

<center>

**==15== 天研发
==3== Sprints
==2== 次每日例会**

</center>

---
## 开源社区 / Open Source

<br>
感谢互联网上的开源社区给我们许多学习的机会, 是时候向社区回馈我们的成果了.

我们的开源方案在营内广受欢迎:

- ==5号组==采用路径规划方案
- ==1号组==采用搜索策略
- etc.

本项目使用MIT开源许可.

---


# 管理 / Management
<!-- *page_number: false -->
<!-- *template: gaia -->

---
## 看板 Trello
Trello替代了传统项管中使用的白板, 实现管理数字化

**最大化效率**
	
- 任务以小时为精度拆解
- 分配到每个成员身上

---

<!-- *template: gaia -->

## Trello 演示

![](https://img.vim-cn.com/47/b4886b78123ef3ccaafd26de7185c2418a85f7.png)

---

![](https://img.vim-cn.com/2a/cddef5207683d1bc3954b6cdebd21ac6193740.jpg)

---
![](https://img.vim-cn.com/8b/748f85993d3a2292a8d0d69b9c753466f72138.png)
<!-- *template: gaia -->
---

# 个人成长 / Learning
<!-- *template: gaia -->
<!-- *page_number: false -->

---



**郑雨盺** / Yuxin  
<!-- *template: invert -->

---
**廖洽源** / Qiayuan
<!-- *template: invert -->

---

> 虽然自己捣鼓软件开发也有6年之久了, 但在此之前我从未接触过机器人开发; 现在我跳出了舒适区. 我与廖神和王冠大佬工作的时候接触了ROS开发, 因为比较熟悉Linux而意外的成为了运维. 除此之外, 之前留心观察揣摩的项目管理方式在这里也得到实战检验. 很幸运, 我在人生的第一个转折点遇到了这一群人.
> 
> -- **张哲慜** / Hisen 
<!-- *template: invert -->

---
**王冠** / One
<!-- *template: invert -->

---
**陈文潇** / Minovsky 
<!-- *template: invert -->

---
**青雨晴** / Joy
<!-- *template: invert -->

---
**朱彦臻** / Yanzhen
<!-- *template: invert -->

