dnl $Id$

PHP_ARG_ENABLE(pod, whether to enable pod support,
 [  --enable-pod           Enable pod support])

if test "$PHP_POD" != "no"; then
  PHP_NEW_EXTENSION(pod, pod.c, $ext_shared)
fi
