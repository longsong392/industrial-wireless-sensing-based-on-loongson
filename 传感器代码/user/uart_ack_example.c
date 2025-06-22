/*
 * uart_ack_example.c
 * 串口应答机制示例
 * 节点端与终端端伪代码
 */

#include <stdio.h>
#include <string.h>

// 假设串口收发函数
void uart_send(const char *data);
int uart_receive(char *buf, int maxlen, int timeout_ms);

// 假设密文数据
const char *ciphertext = "A1B2C3D4E5F6";

// ================= 节点端 =================
void node_send_data_with_ack(const char *data)
{
    char ack_buf[16];
    int retry = 0;
    const int max_retry = 3;
    while (retry < max_retry)
    {
        uart_send(data); // 发送数据
        // 等待ACK，超时重发
        if (uart_receive(ack_buf, sizeof(ack_buf), 100) > 0 && strcmp(ack_buf, "ACK") == 0)
        {
            printf("节点端：收到终端ACK，数据发送成功\n");
            break;
        }
        else
        {
            printf("节点端：未收到ACK，重发\n");
            retry++;
        }
    }
    if (retry == max_retry)
    {
        printf("节点端：数据发送失败\n");
    }
}

// 节点端：等待"send"命令后发送密文
void node_wait_and_send_ciphertext()
{
    char cmd_buf[32];
    while (1)
    {
        // 等待终端发来"send"命令
        if (uart_receive(cmd_buf, sizeof(cmd_buf), 500) > 0)
        {
            if (strcmp(cmd_buf, "send") == 0)
            {
                printf("节点端：收到send命令，开始发送密文\n");
                node_send_data_with_ack(ciphertext);
            }
        }
    }
}

// ================= 终端端 =================
void terminal_receive_and_ack()
{
    char recv_buf[64];
    while (1)
    {
        if (uart_receive(recv_buf, sizeof(recv_buf), 500) > 0)
        {
            printf("终端端：收到数据：%s\n", recv_buf);
            // 处理数据...
            uart_send("ACK"); // 回复应答
        }
    }
}

// 终端端：主动发送"send"命令并接收密文
void terminal_send_cmd_and_receive()
{
    char recv_buf[64];
    // 发送"send"命令
    uart_send("send");
    // 等待节点端返回密文
    if (uart_receive(recv_buf, sizeof(recv_buf), 1000) > 0)
    {
        printf("终端端：收到密文：%s\n", recv_buf);
        // 回复ACK
        uart_send("ACK");
    }
}

/*
说明：
- uart_send/uart_receive 需根据你的硬件平台实现。
- 节点端每次发送数据后等待ACK，超时重发，最多3次。
- 终端端收到数据后立即回复ACK。
- 可根据实际需求扩展为带序号、带校验等。

用法说明：
- 节点端运行 node_wait_and_send_ciphertext()，等待终端命令。
- 终端端运行 terminal_send_cmd_and_receive()，主动请求密文。
- 发送和接收均采用应答机制，保证数据可靠。
*/ 