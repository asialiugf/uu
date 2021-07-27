#!/bin/bash

echo -n "please enter 日期（格式:20180808）:"
read line
echo $line
mkdir /home/rabbit/u/tick/$line

mv /home/rabbit/u/tick/tick.json  /home/rabbit/u/dat/tick.$line.json
cd /home/rabbit/u/dat

sed -i 's/pre_open_interest/j/g'  *
sed -i 's/last_price/e/g'   *
sed -i 's/instrument_id/f/g'  *
sed -i 's/open_interest/g/g'  *
sed -i 's/pre_close/i/g'  *
sed -i 's/pre_settlement/k/g'  *
sed -i 's/ask_price1/m/g'  *
sed -i 's/ask_volume1/n/g'  *
sed -i 's/bid_price1/p/g'  *
sed -i 's/bid_volume1/q/g'  *
sed -i 's/lower_limit/t/g'  *
sed -i 's/upper_limit/u/g'  *
sed -i 's/amount/a/g'  *
sed -i 's/average/b/g'  *
sed -i 's/close/c/g'  *
sed -i 's/datetime/d/g'  *
sed -i 's/highest/h/g'  *
sed -i 's/lowest/l/g'  *
sed -i 's/open/o/g'  *
sed -i 's/settlement/s/g'  *
sed -i 's/volume/v/g'  *
sed -i 's/status/z/g'  *

sed -i 's/\t//g' *
sed -i ':a;N;$!ba;s/{\n/{/g' *
sed -i ':a;N;$!ba;s/,\n/,/g' *
sed -i ':a;N;$!ba;s/\n}/}/g' *

cd /home/rabbit/u

mv    /home/rabbit/u/dat             /home/rabbit/u/tick/$line/$line.tick
mkdir /home/rabbit/u/dat

echo -n " ---- 结束 ---- "
