///////////////those two fns for counting pulses in photointerruptor ////////
bool flag;
void Choosetryy(){ if(digitalRead(2)) flag=LOW ; else flag=HIGH ;}

void tryy (){
 
if (digitalRead(2) && flag )      {photo++ ;flag = LOW ;}
if (!digitalRead(2)&& !flag)      {photo++ ;flag =HIGH ;}

}

