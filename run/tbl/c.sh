cd /home/rabbit/u/dat

if [ ! -f "*000000" ]; then
 rm *000000
fi
ls * > /tmp/zz
wc /tmp/zz
cp /tmp/zz /home/rabbit/u/exe/cmd


cd /home/rabbit/u
