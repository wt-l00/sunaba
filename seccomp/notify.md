# seccompでシステムコールの引数チェック

以下のような感じで，process_vm_readvを使うとあるpidのpointerからデータを引っ張ってくることができる．
例えば，seccomp notificationを使ってseccompの引数（data.args）をチェックして実行するかどうかみたいなことができる．

``` c
struct iovec local[1];
struct iovec remote[1];
char buf1[3];
ssize_t nread;
local[0].iov_base=buf1;
local[0].iov_len=3;
remote[0].iov_base=(char*)(pointer);
remote[0].iov_len=3;
nread = process_vm_readv(pid,local,1,remote,1,0);

printf("%ld bytes read\n",nread);
write(1, local[0].iov_base, nread);
```

***

## 参考文献

* https://man7.org/linux/man-pages/man2/process_vm_readv.2.html
* https://brauner.github.io/2020/07/23/seccomp-notify.html
