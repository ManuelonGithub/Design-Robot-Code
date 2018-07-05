/*
 * FSK.h
 *
 * Created: 03/07/2018 23:46:51
 *  Author: Manuel
 */ 


#ifndef FSK_H_
#define FSK_H_

#define FSK_OFF 0x00
#define FSK_ON ((1<<WGM12)|(0<<CS11)|(1<<CS10))

void init_FSK();
void char_c_FSK(int tx_count);


#endif /* FSK_H_ */