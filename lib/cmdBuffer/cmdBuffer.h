#ifndef CMD_BUFFER_H
#define CMD_BUFFER_H

#include <stdint.h>
#include <ringbuffer.h>

struct cmdItem
{
    int moduleTarget;   // The command item's target module
    int instruction;    // The actual instruction for the target module
    int vArgLen;        // The number of the attached vArgs in the attached array that
                        // are associated with the given instruction
    int * vArg;         // The arguments that are associated with the given instruction
};

class CmdBuffer {
    
    public:
    
        static const char delim = 59;   // ';' delimiter

        CmdBuffer(size_t size);
        
        size_t getOccupied(void) { return rbCmdBuf->getOccupied(); }
        size_t getFree(void) { return rbCmdBuf->getFree(); }

        cmdItem* parseCmd(cmdItem* cmd, char* data, size_t n);
        size_t writeCmdMsg(char* data, size_t n);
        size_t readCmd(cmdItem* dest);
        
    private:
        
        // Holds the pre-parsed commands
        ringbuffer<cmdItem>* rbCmdBuf;   
        
        // Private utility members
        int getNumDelims(char* data, int len);

}; // class cmdBuffer

#endif // CMD_BUFFER_H
