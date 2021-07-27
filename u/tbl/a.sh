#!/bin/bash


cd /home/rabbit/u/dat
ls * > /tmp/zz
cat /tmp/zz | while read LINE  
do
  sort $LINE > $LINE.ss
done
rm *.tick
cd /home/rabbit/u

echo -n "please enter 日期（格式:20180808）:"
read line
echo $line
read pp
mkdir /home/rabbit/u/tick/$line
mv    /home/rabbit/u/dat             /home/rabbit/u/tick/$line
cd    /home/rabbit/u/tick
      tar cvzf tick.$line.tar.gz ./$line
cd    /home/rabbit/u
mkdir /home/rabbit/u/dat

echo -n " ---- 结束 ---- "
