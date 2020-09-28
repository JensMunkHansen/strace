namespace sps {
  
  class STrace {
  public:
  typedef enum {
    STRACE_ERR_OK                  = 0,
    STRACE_ERR_INVALID_STACK_DEPTH,    ///<
    STRACE_ERR_INVALID_FD,             ///<  Invalid file descriptor
    STRACE_ERR_COUNT
  } straceErrorCodes;
  static const size_t kNeededMemory;
    
  static STrace& Instance();
  sps::STrace::straceErrorCodes Enable();
  private:
    STrace();
    ~STrace();
    STrace(STrace const&) {} // = default;  // copy constructor is private
    STrace& operator=(STrace const&);//= default;  // assignment operator is private
    char* memory_;
  };
}
