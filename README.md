# RM2019SummerCamp
All the codes for the robomaster 2019 high school summer camp
Our Process：

```mermaid
graph LR
    start[开始] --> input[输入A,B,C]
    input --> conditionA{A是否大于B}
    conditionA -- YES --> conditionC{A是否大于C}
    conditionA -- NO --> conditionB{B是否大于C}
    conditionC -- YES --> printA[输出A]
    conditionC -- NO --> printC[输出C]
    conditionB -- YES --> printB[输出B]
    conditionB -- NO --> printC[输出C]
    printA --> stop[结束]
    printC --> stop
    printB --> stop
```

 ***
Staffing table:

Embedded System|Algorithm|Mechanics
---|:--:|---:
Liao Qiayuan |Wang Guan|Chen Wenxiao
