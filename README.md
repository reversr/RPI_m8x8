# RPI_m8x8

This program shows how to control MAX7219 8x8 LED Matrix with the Raspberry Pi in C.

https://github.com/reversr/RPI_m8x8.git

Example program for bcm2835 library && Max7219 LED Matrix
Printing some geometric figures and alphanumeric chars

After installing bcm2835, you can build this with: gcc -o m8x8 m8x8.c -lbcm2835
execute via: sudo ./m8x8

If you run this without sudo/root rights you will have 'Segmentation fault'

Used materials:

http://pro-diod.ru/electronica/max7219-max7221-drajver-dlya-svetodiodnoj-indikacii.html
MAX7219 Init Explanation

http://www.14core.com/how-to-wire-8x8-matrix-led-with-max7219-on-arduino/
8×8 Matrix LED Row/Col & Bitwise explained

This is a modified version of m8x8.c from
https://s3-ap-northeast-1.amazonaws.com/sain-amzn/20/20-011-232/20-011-232.zip


IMPORTANT:
I AM NOT RESPONCIBLE FOR ANY DAMAGE TO YOUR RASPBERRY PI,
PC OR WHATEVER ELSE, USE THIS CODE AT YOUR OWN RISK.
