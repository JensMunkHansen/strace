#include "signal.h"
void seghandle_userspace() {
  // note: because we set up a proper stackframe,
  // unwinding is safe from here.
  // also, by the time this function runs, the
  // operating system is "done" with the signal.

  // choose language-appropriate throw instruction
  // raise new MemoryAccessError "Segmentation Fault";
  // throw new MemoryAccessException;
  // longjmp(erroneous_exit);
  // asm { int 3; }
  *(int*) NULL = 0;
}
enum X86Registers {
  GS, FS, ES, DS, EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX,
  TRAPNO, ERR, EIP, CS, EFL, UESP, SS
}
void seghandle(int sig, void* si, void* unused) {
  ucontext_t* uc = (ucontext_t*) unused;
  // No. I refuse to use triple-pointers.
  // Just pretend ref a = v; is V* ap = &v;
  // and then substitute a with (*ap).
  ref gregs = uc->uc_mcontext.gregs;
  ref eip = (void*) gregs[X86Registers.EIP],
  ref esp = (void**) gregs[X86Registers.ESP];

  // imitate the effects of "call seghandle_userspace"
  esp --; // decrement stackpointer.
          // remember: stack grows down!
  *esp = eip;

  // set up OS for call via return, like in the attack
  eip = (void*) &seghandle_userspace;
}
void setup_segfault_handler() {
  struct sigaction sa;
  sa.flags = SA_SIGINFO;
  sigemptyset (&sa.mask);
  sa.sigaction = &seghandle;
  if (sigaction(SIGSEGV, &sa, NULL) == -1) {
    fprintf(stderr, "failed to setup SIGSEGV handler\n");
    exit(1);
  }
}

#include "winbase.h"
  #include "windows.h"
  #include "excpt.h"
  void seghandle_userspace() {
    // see above re. what to do now
    *(int*) NULL = 0;
  }
  EXCEPTION_DISPOSITION seghandle
  (_EXCEPTION_RECORD* record, void* establisher_frame,
   _CONTEXT* context, void* dispatcher_context
  ) {
    // see above
    ref
      esp = (void**)context.Esp,
      eip = (void* )context.Eip;
    // imitate the effects of "call seghandle_userspace"
    // stack grows down under windows, too
    esp --;
    *esp = eip;
    // rewrite eip for return-to-lib, same as linux
    eip = (void*) &seghandle_userspace;
    // and back to user code.
    return ExceptionContinueExecution;
  }
  void setup_segfault_handler() {
    _EXCEPTION_REGISTRATION reg;
    // Note: this bit will require assembly.
    // Consult your compiler vendor documentation.
    reg.prev = FS[0];
    reg.handler = &seghandle;
    FS[0] = &reg;
    // note: do this on exit
    // fs[0] = ((_EXCEPTION_REGISTRATION) fs[0]).prev;
  }
