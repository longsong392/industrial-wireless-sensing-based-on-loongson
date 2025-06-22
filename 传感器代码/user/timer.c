/*
 * timer.c
 *
 * created: 2024/5/17
 *  author: 
 */

/*
HW_TIMER_t *g_timer = (HW_TIMER_t *)LS1C102_TIMER_BASE;// ��ȡ��ʱ���Ĵ���
HW_INTC_t *intc = (HW_INTC_t *)LS1C102_INTC_BASE;      // ��ȡ�жϼĴ���

int gpio20_state = 0;

void timer_irq(int vector, void *arg)
{
    // ���Զ�ʱ������
    gpio20_state = ~gpio20_state;
    gpio_write(20, gpio20_state);
    // ���Զ�ʱ������

    g_timer->cfg = g_timer->cfg;// ִ�к�����жϱ�־λ     ???666
}

void timer_interrupt(){
    // ��ʱ�����á��ж���������
    g_timer->cfg = 0;                    // ��ʱ�����ø�λ
    g_timer->cnt = 0;                    // ��ʱ������ֵ���

    g_timer->cmp = 8000000;             // �״ν��붨ʱ���жϵļ�������Ϊ16M�����ʱ��Ϊ8M����ô��Լ��2s��ʱ��û���⡣
    //g_timer->step = 32000000;            // ��ʱ���жϲ���������32M����һ���жϣ���Լ��4s��ʱ��û���⡣
    // һ�㶼�����ڴ���ģʽ�����Ե�һ�δ����ж���Ҫ������ cmp ��֮��ÿ�μ��� step �ͻᴥ��һ���жϡ�
    g_timer->step = 2000000;

    g_timer->cfg = 0x07;                 // �������ڴ������ж�ʹ�ܣ���ʼ����
    intc->en |= 0x01;                    // ������ʱ��ȫ���ж�

    ls1c102_install_isr(LS1C102_IRQ_TIMER, &timer_irq, NULL);// װ�ض�ʱ�жϺ���
    // ��ʱ���жϱ�� LS1C102_IRQ_TIMER = 11�����жϺ��� timer_irq �ĵ�ַ��Ϊ��������
    // ��ʱ�����á��ж���������
}
*/

