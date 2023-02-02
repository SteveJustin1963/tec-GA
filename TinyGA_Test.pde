TinyGA_Test.pde - Alexander Hiam - 1/2012
Uses the TinyGA genetic algorithm library to guess numbers.
*/

#include <TinyGA.h>

// Create TinyGA instance, giving it a fitness function:
TinyGA ga = TinyGA(fitness);

uint8_t number_to_guess, char_num;

void setup() {
  Serial.begin(9600);
  // Must seed random number generator before initializing TinyGA:
  randomSeed(analogRead(A4));
  start();
}

void start() {
  uint8_t i;
  char buffer[10]; // Buffer for serial receiver
  // Initialize with 10 individuals:
  ga.init(10);
  char_num = 0;
  Serial.println("TinyGA TestnEnter a number between 0-255 and TinyGA will try to guess it");
  Serial.flush();
  while (!Serial.available()); // Wait for input
  i = 0;
  while(Serial.available()) {
    buffer[i++] = Serial.read();
    delay(10); // Give plenty of time for next character to arrive
  }
  buffer[i] = ''; // End array
  number_to_guess = atoi(buffer); // Convert to int
  Serial.print("Looking for number: ");
  Serial.println(number_to_guess, DEC);
}

uint8_t fitness(uint8_t individual) {
  /* The fitenss function; takes a guess and returns a score that is larger
     the closer to the correct number it is, and 255 if correct. */
  return 255 - abs(number_to_guess-individual);
}

void loop() {
  uint8_t result;
  if (char_num++ >= 60) {
    // Just keep track of where the status character is and move to a new line if >=60
    Serial.println();
    char_num = 1;
  }
  Serial.print(".");
  // Run for 100 generations:
  result = ga.run(100);
  if(result) {
    // Run returns 0 unless it got the correct answer, so we know we have it here
    Serial.println();
    Serial.print(result, DEC);
    Serial.println(" - Got it!");
    ga.print(); // Have TinyGA print its current population
    Serial.println();
    start(); // Back to the beginning
  }
}

\\\\\\\\\\\\\\\\\\\\\\\\\\\\

10 'TinyGA_Test.bas - Alexander Hiam - 1/2012
20 'Uses the TinyGA genetic algorithm library to guess numbers.

30 'Create TinyGA instance, giving it a fitness function:
40 DIM ga AS TinyGA
50 SET ga = NEW TinyGA(fitness)

60 DIM number_to_guess AS INTEGER
70 DIM char_num AS INTEGER

80 'Start function
90 SUB start()
100 DIM i AS INTEGER
110 DIM buffer(10) AS STRING
120 'Initialize with 10 individuals:
130 ga.init 10
140 char_num = 0
150 PRINT "TinyGA Test"
160 PRINT "Enter a number between 0-255 and TinyGA will try to guess it"
170 'Wait for input
180 WHILE INKEY$ = ""
190 WEND
200 i = 0
210 'Receive buffer from input
220 WHILE INKEY$ <> ""
230 buffer(i) = INKEY$
240 i = i + 1
250 WEND
260 'End array
270 buffer(i) = ""
280 'Convert to int
290 number_to_guess = VAL(buffer)
300 PRINT "Looking for number: "; number_to_guess
310 END SUB

320 'Fitness function
330 FUNCTION fitness(individual AS INTEGER) AS INTEGER
340 'The fitness function; takes a guess and returns a score that is larger
350 'the closer to the correct number it is, and 255 if correct.
360 fitness = 255 - ABS(number_to_guess - individual)
370 END FUNCTION

380 'Main loop
390 DO
400 'Just keep track of where the status character is
410 IF char_num >= 60 THEN PRINT: char_num = 0
420 PRINT ".";
430 char_num = char_num + 1
440 'Run for 100 generations:
450 result = ga.run(100)
460 'Check if result is found
470 IF result THEN
480 PRINT
490 PRINT result; " - Got it!"
500 ga.print 'Have TinyGA print its current population
510 PRINT
520 CALL start 'Back to the beginning
530 END IF
540 LOOP

\\\\\\\\\\\\
equivalent Forth 83 code for the original code:

\ TinyGA_Test.fth - Alexander Hiam - 1/2012
\ Uses the TinyGA genetic algorithm library to guess numbers.

: fitness ( individual -- score )
\ The fitness function; takes a guess and returns a score that is larger
\ the closer to the correct number it is, and 255 if correct.
number_to_guess abs 255 - ;

: start ( -- )
0 value char_num
cr .("TinyGA Test")
cr .("Enter a number between 0-255 and TinyGA will try to guess it")
cr
key ?dup if
10 buffer !
10 0 do i buffer i + c@ atoi buffer ! loop
buffer number_to_guess !
cr .("Looking for number: ") number_to_guess . cr
else
start
then ;

: loop ( -- )
char_num @ 60 >= if
cr char_num 0 !
then
."."
100 ga run dup if
cr . cr
number_to_guess . cr
." - Got it!" cr ga print cr
start
then ;

This code assumes that the TinyGA library has been included and is defined in Forth 83.
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


\\ another try
\\ there are no libraries assumed in this code.

\ TinyGA_Test.f83
\ Alexander Hiam - 1/2012
\ Uses the TinyGA genetic algorithm library to guess numbers.

\ Create TinyGA instance, giving it a fitness function:
variable ga
create ga tinyga fitness 

variable number_to_guess
variable char_num

\ Start function
: start
   10 0 do i buffer 10 allot loop 
   \ Initialize with 10 individuals:
   ga init 10
   char_num 0 !
   cr ." TinyGA Test"
   cr ." Enter a number between 0-255 and TinyGA will try to guess it"
   \ Wait for input
   begin key? while repeat
   i 0 !
   \ Receive buffer from input
   begin key? while
      buffer i key c@ i 1+ !
   repeat
   \ End array
   buffer i "" c!
   \ Convert to int
   buffer value number_to_guess !
   cr ." Looking for number: " number_to_guess .
;

\ Fitness function
: fitness ( individual -- score)
   \ The fitness function; takes a guess and returns a score that is larger
   \ the closer to the correct number it is, and 255 if correct.
   number_to_guess @ - abs 255 - ; 
;

\ Main loop
begin 
   \ Just keep track of where the status character is
   60 char_num @ >= if cr char_num 0 ! then
   \ "." char_num 1+ char_num !
   ." ."
   \ Run for 100 generations:
   ga run 100 result ? 
   \ Check if result is found
   result @ if 
      cr result @ . ." - Got it!" cr
      ga print \ Have TinyGA print its current population
      cr
      start \ Back to the beginning
   then
again

\\\\\\\\\\\\\\\\\\\\\
