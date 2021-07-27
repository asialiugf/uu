cd /home/rabbit/u/dat

ls * > /tmp/zz
cat /tmp/zz | while read LINE  
do
  sort $LINE > $LINE.sss
done

