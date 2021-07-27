### git configure

```
$ git clone https://gitee.com/asialiugf/uquant.git
$ cd uquant
  111  git config --global user.name "asialiugf"
  113  git config --global user.passwd "xxxxxxxx"
  115  git config --global user.email "a@aa.com"
$ cd uquant 
$ ./a
```
```
rabbit@VM-0-12-ubuntu:~/uquant$ cat  .git/config  
[core]
        repositoryformatversion = 0
        filemode = true
        bare = false
        logallrefupdates = true
[remote "origin"]
        url = https://gitee.com/asialiugf/uquant.git
        fetch = +refs/heads/*:refs/remotes/origin/*
[branch "master"]
        remote = origin
        merge = refs/heads/master
[credential]
            helper = store
rabbit@VM-0-12-ubuntu:~/uquant$ 
```
### uws编译
```
 git clone https://github.com/uNetworking/uWebSockets.git
 cd uWebsockets
$ cd uqunat
$ cd uws
$ make
$ sudo make install

```
### 环境

```
$ cd ~
$ mkdir u
$ mkdir u/lib
$ mkdir u/exe
$ mkdir u/obj

```


### CTP的目录

  把CTP相关的文件移到下面两个目录里面去。
  /usr/local/include
  /usr/local/lib

### LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:/home/rabbit/u/lib:/usr/lib64

 cp ./uquant/lib/seetalib-1.0.0/lib/libsee_talib.so ~/u/lib

apt-get install g++
apt-get install gcc
apt-get install make
apt-get install libssl-dev
apt-get install cmake
apt-get install uuid-dev
apt-get install pkg-config

### nginx
```
# apt install nginx
(base) root@foobar:~# which nginx
/usr/sbin/nginx
(base) root@foobar:~# 
(base) root@foobar:~# 
(base) root@foobar:~# cd /
(base) root@foobar:/# find . -name nginx.conf
./etc/nginx/nginx.conf
(base) root@foobar:/# 
(base) root@foobar:/# 
(base) root@foobar:/# 
(base) root@foobar:/# ps -ef | grep nginx
root        2081       1  0 13:26 ?        00:00:00 nginx: master process /usr/sbin/nginx -g daemon on; master_process on;
www-data    2082    2081  0 13:26 ?        00:00:00 nginx: worker process
www-data    2083    2081  0 13:26 ?        00:00:00 nginx: worker process
www-data    2084    2081  0 13:26 ?        00:00:00 nginx: worker process
www-data    2085    2081  0 13:26 ?        00:00:00 nginx: worker process
root        2241    1587  0 13:26 pts/1    00:00:00 grep --color=auto nginx
(base) root@foobar:/# 
(base) root@foobar:/# service nginx stop
(base) root@foobar:/# 
```
