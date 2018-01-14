
作业：
结合timer_list，实现led灯的闪烁；
要求：
1. 用ioctl命令控制led灯的亮灭
2. 用gpio库来控制LED
3. 应能实现每个灯按不同频率闪烁(为每个led配置一个timer_list)
4. 4个定时器可以考虑共用同一个定时器处理函数(通过参数data来区分不同的定时器)

