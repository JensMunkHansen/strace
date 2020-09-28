#include <strace.hpp>

namespace sps {
const size_t STrace::kNeededMemory = 12288;
  
STrace::STrace() {
  if (memory_ == NULL) {
    memory_ = new char[kNeededMemory];
  }
}

STrace::~STrace() {
  if (memory_) {
    delete [] memory_;
  }
}

sps::STrace::straceErrorCodes STrace::Enable() {
  if (!m_bInitialized) {
    this->init();
  }
  // What about SIGTERM
  signal(SIGSEGV, handle_signal);
  signal(SIGABRT, handle_signal);

  // Unhook
  //signal(SIGSEGV, SIG_DFL);
  signal(SIGABRT, SIG_DFL);
}
}

void handle_signal(int signal) {
  switch (signal) {
#ifdef _WIN32
  case SIGTERM:
  case SIGABRT:
  case SIGBREAK:
#else
  case SIGHUP:
#endif
    got_sighup = true;
    break;
  case SIGINT:
    got_sigint = true;
    break;
  }
}
