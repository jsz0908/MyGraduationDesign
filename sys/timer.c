#include "timer.h"
#include <REGX52.H>

void Timer0Init(void) // 1ms中断 @ 11.0592MHz，12T模式
{

	TMOD &= 0xF0;    // 清除定时器0控制位
	TMOD |= 0x01;    // 设置定时器0为模式1（16位定时器）
	TL0 = 0x66;      // 设置定时初始值 -> TL0
	TH0 = 0xFC;      // 设置定时初始值 -> TH0 （FC66 = 64518，1ms）
	TF0 = 0;         // 清除TF0溢出标志
	TR0 = 1;         // 启动定时器0

	ET0 = 1;         // 允许定时器0中断
	EA = 1;          // 开启总中断
}
