#ifndef USART2_H
#define USART2_H

//function declaration
void Usart2_Init(int baudrate);
void Usart2_DMA_Send(uint8_t *buf, uint16_t len);

#endif
