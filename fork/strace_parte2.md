### **Desafio**
#### Parte 2

Se implemento el comando `strace` capas de soportar al menos un comando de argumento.
Para poner en evidencia la impementacion le recomendmaos ejecutar los siguentes comandos:
- ls
- pwd
- echo hi
- strace

A continuacion se adjuntara las comparaciones de haber ejecutado `strace ls` y `./strace ls`

** Ejecución del comando strace, version simplificada **

```
lucaswaisten@lucaswaisten-Sword-15-A11UD:~/Documents/sisOp/labs/fork$ ./strace ls
Nombre syscall: brk ,Return Value: 12
Nombre syscall: adjtimex ,Return Value: 158
Nombre syscall: access ,Return Value: 21
Nombre syscall: mkdirat ,Return Value: 257
Nombre syscall: fstat ,Return Value: 5
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: close ,Return Value: 3
Nombre syscall: mkdirat ,Return Value: 257
Nombre syscall: read ,Return Value: 0
Nombre syscall: fstat ,Return Value: 5
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mprotect ,Return Value: 10
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: close ,Return Value: 3
Nombre syscall: mkdirat ,Return Value: 257
Nombre syscall: read ,Return Value: 0
Nombre syscall: pread64 ,Return Value: 17
Nombre syscall: pread64 ,Return Value: 17
Nombre syscall: pread64 ,Return Value: 17
Nombre syscall: fstat ,Return Value: 5
Nombre syscall: pread64 ,Return Value: 17
Nombre syscall: pread64 ,Return Value: 17
Nombre syscall: pread64 ,Return Value: 17
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: close ,Return Value: 3
Nombre syscall: mkdirat ,Return Value: 257
Nombre syscall: read ,Return Value: 0
Nombre syscall: fstat ,Return Value: 5
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: close ,Return Value: 3
Nombre syscall: mkdirat ,Return Value: 257
Nombre syscall: read ,Return Value: 0
Nombre syscall: fstat ,Return Value: 5
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: close ,Return Value: 3
Nombre syscall: mkdirat ,Return Value: 257
Nombre syscall: read ,Return Value: 0
Nombre syscall: pread64 ,Return Value: 17
Nombre syscall: fstat ,Return Value: 5
Nombre syscall: pread64 ,Return Value: 17
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: close ,Return Value: 3
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: adjtimex ,Return Value: 158
Nombre syscall: mprotect ,Return Value: 10
Nombre syscall: mprotect ,Return Value: 10
Nombre syscall: mprotect ,Return Value: 10
Nombre syscall: mprotect ,Return Value: 10
Nombre syscall: mprotect ,Return Value: 10
Nombre syscall: mprotect ,Return Value: 10
Nombre syscall: mprotect ,Return Value: 10
Nombre syscall: munmap ,Return Value: 11
Nombre syscall: restart_syscall ,Return Value: 218
Nombre syscall: get_robust_list ,Return Value: 273
Nombre syscall: rt_sigaction ,Return Value: 13
Nombre syscall: rt_sigaction ,Return Value: 13
Nombre syscall: rt_sigprocmask ,Return Value: 14
Nombre syscall: name_to_handle_at ,Return Value: 302
Nombre syscall: fstatfs ,Return Value: 137
Nombre syscall: fstatfs ,Return Value: 137
Nombre syscall: brk ,Return Value: 12
Nombre syscall: brk ,Return Value: 12
Nombre syscall: mkdirat ,Return Value: 257
Nombre syscall: fstat ,Return Value: 5
Nombre syscall: read ,Return Value: 0
Nombre syscall: read ,Return Value: 0
Nombre syscall: close ,Return Value: 3
Nombre syscall: access ,Return Value: 21
Nombre syscall: mkdirat ,Return Value: 257
Nombre syscall: fstat ,Return Value: 5
Nombre syscall: mmap ,Return Value: 9
Nombre syscall: close ,Return Value: 3
Nombre syscall: ioctl ,Return Value: 16
Nombre syscall: ioctl ,Return Value: 16
Nombre syscall: mkdirat ,Return Value: 257
Nombre syscall: fstat ,Return Value: 5
Nombre syscall: set_tid_address ,Return Value: 217
Nombre syscall: set_tid_address ,Return Value: 217
Nombre syscall: close ,Return Value: 3
Nombre syscall: fstat ,Return Value: 5
Nombre syscall: write ,Return Value: 1
desafioStrace.md  find.c  LICENSE   pingpong    pingpong.o  primes.c  README.md  strace.c  syscallent.h  xargs.c
Nombre syscall: write ,Return Value: 1
find              find.o  Makefile  pingpong.c  primes      primes.o  strace     strace.o  xargs         xargs.o
Nombre syscall: close ,Return Value: 3
Nombre syscall: close ,Return Value: 3
Nombre syscall: epoll_wait ,Return Value: 231
+++ exited with 0 +++
```
** Ejecución del comando original strace **

```
lucaswaisten@lucaswaisten-Sword-15-A11UD:~/Documents/sisOp/labs/fork$ strace ls
execve("/usr/bin/ls", ["ls"], 0x7ffd275bdf80 /* 72 vars */) = 0
brk(NULL)                               = 0x564b3a9be000
arch_prctl(0x3001 /* ARCH_??? */, 0x7fff672c5ee0) = -1 EINVAL (Invalid argument)
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=89893, ...}) = 0
mmap(NULL, 89893, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7fc1d955d000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libselinux.so.1", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0@p\0\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0644, st_size=163200, ...}) = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fc1d955b000
mmap(NULL, 174600, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fc1d9530000
mprotect(0x7fc1d9536000, 135168, PROT_NONE) = 0
mmap(0x7fc1d9536000, 102400, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x6000) = 0x7fc1d9536000
mmap(0x7fc1d954f000, 28672, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1f000) = 0x7fc1d954f000
mmap(0x7fc1d9557000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x26000) = 0x7fc1d9557000
mmap(0x7fc1d9559000, 6664, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7fc1d9559000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\300A\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
pread64(3, "\4\0\0\0\20\0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0", 32, 848) = 32
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\30x\346\264ur\f|Q\226\236i\253-'o"..., 68, 880) = 68
fstat(3, {st_mode=S_IFREG|0755, st_size=2029592, ...}) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
pread64(3, "\4\0\0\0\20\0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0", 32, 848) = 32
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\30x\346\264ur\f|Q\226\236i\253-'o"..., 68, 880) = 68
mmap(NULL, 2037344, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fc1d933e000
mmap(0x7fc1d9360000, 1540096, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x22000) = 0x7fc1d9360000
mmap(0x7fc1d94d8000, 319488, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x19a000) = 0x7fc1d94d8000
mmap(0x7fc1d9526000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1e7000) = 0x7fc1d9526000
mmap(0x7fc1d952c000, 13920, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7fc1d952c000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libpcre2-8.so.0", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\340\"\0\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0644, st_size=584392, ...}) = 0
mmap(NULL, 586536, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fc1d92ae000
mmap(0x7fc1d92b0000, 409600, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x2000) = 0x7fc1d92b0000
mmap(0x7fc1d9314000, 163840, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x66000) = 0x7fc1d9314000
mmap(0x7fc1d933c000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x8d000) = 0x7fc1d933c000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libdl.so.2", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0 \22\0\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0644, st_size=18848, ...}) = 0
mmap(NULL, 20752, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fc1d92a8000
mmap(0x7fc1d92a9000, 8192, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1000) = 0x7fc1d92a9000
mmap(0x7fc1d92ab000, 4096, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x3000) = 0x7fc1d92ab000
mmap(0x7fc1d92ac000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x3000) = 0x7fc1d92ac000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libpthread.so.0", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\220q\0\0\0\0\0\0"..., 832) = 832
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0{E6\364\34\332\245\210\204\10\350-\0106\343="..., 68, 824) = 68
fstat(3, {st_mode=S_IFREG|0755, st_size=157224, ...}) = 0
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0{E6\364\34\332\245\210\204\10\350-\0106\343="..., 68, 824) = 68
mmap(NULL, 140408, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fc1d9285000
mmap(0x7fc1d928b000, 69632, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x6000) = 0x7fc1d928b000
mmap(0x7fc1d929c000, 24576, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x17000) = 0x7fc1d929c000
mmap(0x7fc1d92a2000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1c000) = 0x7fc1d92a2000
mmap(0x7fc1d92a4000, 13432, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7fc1d92a4000
close(3)                                = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fc1d9283000
arch_prctl(ARCH_SET_FS, 0x7fc1d9284400) = 0
mprotect(0x7fc1d9526000, 16384, PROT_READ) = 0
mprotect(0x7fc1d92a2000, 4096, PROT_READ) = 0
mprotect(0x7fc1d92ac000, 4096, PROT_READ) = 0
mprotect(0x7fc1d933c000, 4096, PROT_READ) = 0
mprotect(0x7fc1d9557000, 4096, PROT_READ) = 0
mprotect(0x564b3a43a000, 4096, PROT_READ) = 0
mprotect(0x7fc1d95a0000, 4096, PROT_READ) = 0
munmap(0x7fc1d955d000, 89893)           = 0
set_tid_address(0x7fc1d92846d0)         = 52577
set_robust_list(0x7fc1d92846e0, 24)     = 0
rt_sigaction(SIGRTMIN, {sa_handler=0x7fc1d928bbf0, sa_mask=[], sa_flags=SA_RESTORER|SA_SIGINFO, sa_restorer=0x7fc1d9299420}, NULL, 8) = 0
rt_sigaction(SIGRT_1, {sa_handler=0x7fc1d928bc90, sa_mask=[], sa_flags=SA_RESTORER|SA_RESTART|SA_SIGINFO, sa_restorer=0x7fc1d9299420}, NULL, 8) = 0
rt_sigprocmask(SIG_UNBLOCK, [RTMIN RT_1], NULL, 8) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
statfs("/sys/fs/selinux", 0x7fff672c5e30) = -1 ENOENT (No such file or directory)
statfs("/selinux", 0x7fff672c5e30)      = -1 ENOENT (No such file or directory)
brk(NULL)                               = 0x564b3a9be000
brk(0x564b3a9df000)                     = 0x564b3a9df000
openat(AT_FDCWD, "/proc/filesystems", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0444, st_size=0, ...}) = 0
read(3, "nodev\tsysfs\nnodev\ttmpfs\nnodev\tbd"..., 1024) = 467
read(3, "", 1024)                       = 0
close(3)                                = 0
access("/etc/selinux/config", F_OK)     = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/usr/lib/locale/locale-archive", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=8290544, ...}) = 0
mmap(NULL, 8290544, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7fc1d8a9a000
close(3)                                = 0
ioctl(1, TCGETS, {B38400 opost isig icanon echo ...}) = 0
ioctl(1, TIOCGWINSZ, {ws_row=12, ws_col=129, ws_xpixel=0, ws_ypixel=0}) = 0
openat(AT_FDCWD, ".", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 3
fstat(3, {st_mode=S_IFDIR|0775, st_size=4096, ...}) = 0
getdents64(3, /* 26 entries */, 32768)  = 824
getdents64(3, /* 0 entries */, 32768)   = 0
close(3)                                = 0
fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0x1), ...}) = 0
write(1, "desafioStrace.md  find.c  LICENS"..., 110desafioStrace.md  find.c  LICENSE   pingpong pingpong.o  primes.c  README.md  strace.c  syscallent.h  xargs.c
) = 110
write(1, "find\t\t  find.o  Makefile  pingpo"..., 91find                  find.o  Makefile  pingpong.c  primes      primes.o  strace      strace.o  xargs         xargs.o
) = 91
close(1)                                = 0
close(2)                                = 0
exit_group(0)                           = ?
+++ exited with 0 +++
```

Comentarios sobre las ejecuciones:
1. Como primer medida podemos observar que los valores de retornos son distintos, durante la implementacion segui los pasos del manual, el cual brinda el siguente struct que posee la informacion sobre la syscall:
```
            struct ptrace_syscall_info {
                      __u8 op;        /* Type of system call stop */
                      __u32 arch;     /* AUDIT_ARCH_* value; see seccomp(2) */
                      __u64 instruction_pointer; /* CPU instruction pointer */
                      __u64 stack_pointer;    /* CPU stack pointer */
                      union {
                          struct {    /* op == PTRACE_SYSCALL_INFO_ENTRY */
                              __u64 nr;       /* System call number */
                              __u64 args[6];  /* System call arguments */
                          } entry;
                          struct {    /* op == PTRACE_SYSCALL_INFO_EXIT */
                              __s64 rval;     /* System call return value */
                              __u8 is_error;  /* System call error flag;
                                                 Boolean: does rval contain
                                                 an error value (-ERRCODE) or
                                                 a nonerror return value? */
                          } exit;
                          struct {    /* op == PTRACE_SYSCALL_INFO_SECCOMP */
                              __u64 nr;       /* System call number */
                              __u64 args[6];  /* System call arguments */
                              __u32 ret_data; /* SECCOMP_RET_DATA portion
                                                 of SECCOMP_RET_TRACE
                                                 return value */
                          } seccomp;
                      };
                  };
``` 
El valor de retorno estara dado por el campo `exit.rval`. Sin embargo podemos notar que los valores son distintos. 
Este error se puede deber a que cuando se hace el llamado de `ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);` la mascara no sea la adecuada.

2. Hay algunos nombre de las syscall que son distintos y/o no aparecen por pantalla. En `syscallent.h` podemos observar las definiciones de las syscall; utilice [esta tabla](https://gitlab.com/strace/strace/-/blob/master/src/linux/x86_64/syscallent.h) como referencia para la definiciones. Que los nombre sean distintos puede deberse a un mal ordenamiento o que el ordenamiento sea distinto al que maneja Linux.
Por ejemplo la syscall `openat` no se muestra por pantalla en la implementacion hecha, pero sin embargo se encuentra definida en `syscallent.h`.
Otro supuesto puede deberse a la arquitectura, si la misma es x32 o x86_64 el ordenamiento de las syscall varia. (Este supuesto lo pude observar en [strace](https://gitlab.com/strace/strace/-/tree/master/) que si bien no es oficial de la catedra, me sirve como referencia para documentarlo, en caso de que sea erroneo descartar la observacion).