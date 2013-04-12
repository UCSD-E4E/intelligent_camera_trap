static unsigned int timer = 0;

void setTimerIn100ms(unsigned int timerSetting){
    timer = timerSetting;
}
void decrementTimerBy100ms(void){

    if (timer > 0) {
        timer--;
    }
}
unsigned int getTimerIn100ms(void){

    return timer;

}
unsigned char timerExpired(){

    return timer == 0;


}
