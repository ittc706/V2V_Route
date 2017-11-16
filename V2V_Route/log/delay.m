clear all;
close all;
clc;

delays = load('total_delay.txt','%n');
interval=0:1:200;
[number,center]=hist(delays,interval);
number=number./(sum(number));
bar(center,number);


