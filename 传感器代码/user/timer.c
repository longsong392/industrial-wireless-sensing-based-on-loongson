/*
 * timer.c
 *
 * created: 2024/5/17
 *  author: 
 */

/*
HW_TIMER_t *g_timer = (HW_TIMER_t *)LS1C102_TIMER_BASE;// 获取定时器寄存器
HW_INTC_t *intc = (HW_INTC_t *)LS1C102_INTC_BASE;      // 获取中断寄存器

int gpio20_state = 0;

void timer_irq(int vector, void *arg)
{
    // 测试定时器如下
    gpio20_state = ~gpio20_state;
    gpio_write(20, gpio20_state);
    // 测试定时器如上

    g_timer->cfg = g_timer->cfg;// 执行后清除中断标志位     ???666
}

void timer_interrupt(){
    // 定时器配置、中断配置如下
    g_timer->cfg = 0;                    // 定时器配置复位
    g_timer->cnt = 0;                    // 定时器计数值清空

    g_timer->cmp = 8000000;             // 首次进入定时器中断的计数次数为16M，如果时钟为8M，那么大约是2s，时间没问题。
    //g_timer->step = 32000000;            // 定时器中断步进，计数32M触发一次中断，大约是4s，时间没问题。
    // 一般都是周期触发模式，所以第一次触发中断需要计数到 cmp ，之后每次计数 step 就会触发一次中断。
    g_timer->step = 2000000;

    g_timer->cfg = 0x07;                 // 开启周期触发，中断使能，开始计数
    intc->en |= 0x01;                    // 开启定时器全局中断

    ls1c102_install_isr(LS1C102_IRQ_TIMER, &timer_irq, NULL);// 装载定时中断函数
    // 定时器中断编号 LS1C102_IRQ_TIMER = 11。把中断函数 timer_irq 的地址作为参数传入
    // 定时器配置、中断配置如上
}
*/

