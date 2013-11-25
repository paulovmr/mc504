#include <linux/unistd.h>
#include <linux/linkage.h>

asmlinkage long sys_mycall(void) {
  return(4096);
}
