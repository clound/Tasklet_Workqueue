#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/random.h>
char buf[100] = {'\0'};
static char *change(int num){
	int k = 0,i;
	int tmp = num;
	while(num != 0){
		num = num/10;
		k++;
	}
	for(i = 0;i < k;i++){
		buf[k-i-1] = tmp%10 + '0' ;
		tmp = tmp/10;
	}
	printk("The buf sh is %s\n",buf);
}
static int __init
init_read(void){
	struct file* fp;
	mm_segment_t fs;
	loff_t pos;
	unsigned short rand_num;
	printk("hello kernel\n");
	fp = filp_open("/tmp/random",O_RDWR|O_CREAT,0);//打开文件
	if(IS_ERR(fp)){
		printk("open fail");
		return -1;
	}
	get_random_bytes(&rand_num,sizeof(unsigned short));//得到随机数
	change(rand_num);
	printk("The buf is %s\n",buf);
	fs = get_fs();
	set_fs(KERNEL_DS);//进入内核态
	pos = 0;
	vfs_write(fp,buf,sizeof(buf),&pos);//写入文件
	filp_close(fp,NULL);
	set_fs(fs);//返回用户态
	printk(KERN_ALERT "wirte file finished!\n");	
	return 0;
}

static void __exit
exit_read(void)
{
	printk("Goodbye kernel\n");
}
module_init(init_read);
module_exit(exit_read);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Valerie Henson <val@nmt.edu>");
MODULE_DESCRIPTION("\"Hello, world!\" minimal module");
MODULE_VERSION("printk");
