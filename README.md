# Tasklet_Workqueue
#####介绍
>软中断和tasklet都是运行在中断上下文中，它们与任一进程无关，没有支持的进程完成重新调度。所以软中断和tasklet不能睡眠、
不能阻塞，它们的代码中不能含有导致睡眠的动作，如减少信号量、从用户空间拷贝数据或手工分配内存等。也正是由于它们运行在
中断上下文中，所以它们在同一个CPU上的执行是串行的，这样就不利于实时多媒体任务的优先处理。  
>那么，什么情况下使用工作队列，什么情况下使用tasklet。如果推后执行的任务需要睡眠，那么就选择工作队列。如果推后执行的
任务不需要睡眠，那么就选择tasklet。另外，如果需要用一个可以重新调度的实体来执行你的下半部处理，也应该使用工作队列。
它是唯一能在进程上下文运行的下半部实现的机制，也只有它才可以睡眠。这意味着在需要获得大量的内存时、在需要获取信号量
时，在需要执行阻塞式的I/O操作时，它都会非常有用。如果不需要用一个内核线程来推后执行工作，那么就考虑使用tasklet。

关键函数:  
1. tasklet
```
tasklet_init(&my_tasklet,tasklet_handler,0);//动态申请tasklet
req_ret=request_irq(irq,myirq_handler,IRQF_SHARED,devname,&irq);//中断申请(共享)	
free_irq(irq,&irq);//释放
tasklet_schedule(&my_tasklet);//调度tasklet  
```
2.workqueue 
```
queue = create_singlethread_workqueue("myqueue");//创建单一线程
queue_timer_register(&queue_timer,TIMER_OVER);//初始化工作队列并将工作队列预处理函数绑定
INIT_DELAYED_WORK(&work,work_handler);//初始化工作任务	
flush_workqueue(queue);//刷新工作队列
destroy_workqueue(queue);//释放创建的工作队列
static struct workqueue_struct *queue = NULL;
static struct delayed_work work;//工作队列
