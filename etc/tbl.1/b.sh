cd /home/rabbit/u/dat

if [ ! -f "*000000" ]; then
 rm *000000
fi

ls * > /tmp/zz
wc /tmp/zz
sed -i ':a;N;$!ba;s/\n/,/g' /tmp/zz


cd /home/rabbit/u
cat /tmp/zz | while read LINE  
do
    echo "{\"aid\":\"subscribe_quote\",\"ins_list\":\"$LINE\"}"  > /home/rabbit/u/exe/cmd
done
