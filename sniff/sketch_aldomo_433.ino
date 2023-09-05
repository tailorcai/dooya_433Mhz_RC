int logfile[40];
int i = 0;
float lastTime = 0;
boolean capturing = false;
boolean checking = false;
boolean dataIncoming = false;
long tms[40];
int ti = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("start...");
  pinMode(2, INPUT);
  attachInterrupt(0, handleInterrupt, CHANGE);
}


void loop() {
}

void _log(char* name, long dt) {
  // tms[ti++] = dt;
  // Serial.print(name);
  // Serial.println( dt );
}

void handleInterrupt() {

  if (!capturing) {  //if we are not capturing
    if (!checking) {  //if we don't wait for the "start" signal
      if (digitalRead(2) == HIGH) {  //if change from LOW to (now) HIGH
        lastTime = micros();
        checking = true;
        _log("line34:", 0);
      }
    }

    else {    //if we check for the "start" signal
      if ((micros() - lastTime > 4000) && (digitalRead(2) == LOW)) {    //if HIGH was longer than 4000µs and we are LOW now
        checking = false;
        capturing = true;
        _log("line41", micros()-lastTime);
        lastTime = micros();
      }

      else {
        //that wasn't the "start" signal
        checking = false;
      }
    }
  }

  else {  //we are capturing
    if (!dataIncoming) {  //we haven't received data yet
      if ((micros() - lastTime > 1000) && digitalRead(2) == HIGH) {  //that was the long LOW part before transmission of data
        dataIncoming = true; //now we receive data  
        _log("line56", micros()-lastTime);

        lastTime = micros();
      }
    }

    else {  //now we receive data
      //if rising flank (now HIGH)
      if (digitalRead(2) == HIGH) {
        //store the time
        lastTime = micros();
      }  

      //if falling flank (now LOW) 
      else if (digitalRead(2) == LOW) {
        //=> check how long we were HIGH
        // _log(micros() - lastTime);
        if (micros() - lastTime > 500) {
          //long
          logfile[i] = 1;
        }

        else {
          //short
          logfile[i] = 0;
        }

        if (i < 39) {
          //as long as we haven't received all bits
          i++;
        }

        else {
          //now we are done
          noInterrupts();  //turn interrupts off
          Serial.println("Empfangene Daten:");
          //print as "quad-bit"
          for (i = 0; i <= 38; i = i + 2) {
            if ((logfile[i] == 0) && (logfile[i+1] == 0))
              Serial.print("0");

            else if ((logfile[i] == 0) && (logfile[i+1] == 1))
              Serial.print("F");

            else if ((logfile[i] == 1) && (logfile[i+1] == 0))
              Serial.print("Q");

            else if ((logfile[i] == 1) && (logfile[i+1] == 1))
              Serial.print("1");
          }
          Serial.println("\ntiming...");
          i = 0;
          dataIncoming = false;
          capturing = false;
          // for( int x=0;x<ti;x++) 
          //   Serial.println( tms[x]);
          // ti = 0;
          interrupts();  //turn interrupts on
          return;  //and begin again
        }
      }

    }
  }
}