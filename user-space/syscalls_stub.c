#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>


int _isatty(int fd) { (void)fd; return 1; } // si todo es tty
int _fstat(int fd, struct stat *st) {
    st->st_mode = 0x2000; // S_IFCHR
    st->st_blksize=1024;
    return 0;
}


extern char end;
void* _sbrk(int incr) {
     // from linker
    static char *heap_end;
    char *prev;
    if (!heap_end) heap_end = &end;
    prev = heap_end;
    heap_end += incr;
    return prev;
}

int _fork(void) {
    errno = ENOSYS;
    return -1;
}

int _execve(const char *path, char *const argv[], char *const envp[]) {
    errno = ENOSYS;
    return -1;
}

int _waitpid(int pid, int *wstatus, int options) {
    errno = ENOSYS;
    return -1;
}