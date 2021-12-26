#ifndef CMD_BUFFER_H
#define CMD_BUFFER_H

#include <stdint.h>
#include <ringbuffer.h>

class CmdBuffer {
    
    public:
    
        static const char delim = 59;   // ';' delimiter

        CmdBuffer(size_t size);
        size_t readToEnd(char * dest);
        int readCmd(char * dest);
        size_t write(char * data, size_t n);
        int getNumCmds(void) { return rbCmdLenBuf->getOccupied(); }
        size_t getOccupied(void) { return rbSerBuf->getOccupied(); }
        size_t getFree(void){ return rbSerBuf->getFree(); }
        
                    
    private:
        
        ringbuffer<char>* rbSerBuf;     // Holds serial stream of commands (incl. delimiters)
        ringbuffer<int>* rbCmdLenBuf;   // Holds length of each command in rbSerBuff
        int getNumDelims(char * data, int len);
        

}; // class cmdBuffer

#endif // CMD_BUFFER_H