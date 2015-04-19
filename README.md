# avr led cube 2x2x2
AtMega8 Led Cube 2x2x2

Run on stock AtMega8 at 1mHz. Compiled and programmed using winavr and usbasp.

My old project to illuminate ledcube 2x2x2. 

Assumptions:
* leds are connected in two layers
* layer contain 4 leds
leds are connected to ports C2-5
layers are connected to ports B5-6

Program CsLedCube222.exe is made in c# to visualize illumination and generate sequence code. Just press circles to change intensity. To view generated code press "Poka¿ kod" button. You must manually copy it into code.
Files New 1.txt etc. contain some sequences.

Code contains comments in Polish language.

Working cube:
https://www.youtube.com/watch?v=_gGHOcaozg4


License
------------
Copyright 2015 V-master

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
