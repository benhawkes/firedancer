#include "fd_util.h"
#include <sys/random.h>

void
fd_boot( int *    pargc,
         char *** pargv ) {
  /* At this point, we are immediately after the program start, there is
     only one thread of execution and fd has not yet been booted. */
  fd_log_private_boot  ( pargc, pargv );
  fd_shmem_private_boot( pargc, pargv );
  fd_tile_private_boot ( pargc, pargv ); /* The caller is now tile 0 */
}

void
fd_halt( void ) {
  /* At this point, we are immediately before normal program
     termination, and fd has already been booted. */
  fd_tile_private_halt ();
  fd_shmem_private_halt();
  fd_log_private_halt  ();
}

FD_TL ulong fd_canary_base;
FD_TL ulong fd_canary_locs[1024];
FD_TL size_t fd_ncanary;
FD_TL int fd_ncanary_warn;

void
fd_canary_init( void ) {
  ssize_t n;

  n = getrandom( &fd_canary_base, sizeof(fd_canary_base), 0 );

  if (n != sizeof(fd_canary_base))
    FD_LOG_ERR(( "Failed to create random canary for layout security." ));

  fd_ncanary_warn = 1;
}

#if FD_HAS_HOSTED

#include <sched.h>

void fd_yield( void ) { sched_yield(); }

#endif

