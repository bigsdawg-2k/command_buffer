#ifndef CMD_BUFFER_H
#define CMD_BUFFER_H

#include <ringbuffer.h>

class CmdBuffer {
    
    public:
    
        CmdBuffer(size_t size);
        size_t readToEnd(uint8_t * dest);
        int readCmd(uint8_t * dest);
        size_t write(uint8_t * data, size_t n);
        int getNumCmds(void) { return rbCmdLenBuf->getOccupied(); }
        size_t getOccupied(void) { return rbSerBuf->getOccupied(); }
        size_t getFree(void){ return rbSerBuf->getFree(); }

                    
    private:
        ringbuffer<uint8_t>* rbSerBuf;  // Holds commands 
        ringbuffer<int>* rbCmdLenBuf;   // Holds length of commands currently in rbSerBuf

}; // class cmdBuffer

#endif // CMD_BUFFER_H