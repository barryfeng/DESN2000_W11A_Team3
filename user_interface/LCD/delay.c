//Crude millisecond delay function.

//This should be done with a timer, but isn't
//so that we don't give away any hints for
//lab 6!

void mdelay (unsigned int ms) {                     
  unsigned int i, j;
  unsigned int max = 0xFFF;
  
  for (i = 0; i < ms; i++) {
		//Delay approx 1ms
		for (j = 0; j < max; j++);
	}
}
