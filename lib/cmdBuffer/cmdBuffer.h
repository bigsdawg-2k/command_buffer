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
        
        size_t getOccupied(void) { return rbSerBuf->getOccupied(); }
        size_t getFree(void) { return rbSerBuf->getFree(); }

        size_t sanitizeMsg(char * data, size_t n);
        size_t writeCmdMsg(char * data, size_t n);
        size_t readCmd(cmdItem * dest);
        
        /*
         * Stuff to get rid of...
         */ 
        int getNumCmds(void) { return rbCmdLenBuf->getOccupied(); }
        int getFreeCmdLenBuf(void) { return rbCmdLenBuf->getFree(); }
        size_t getFreeSerBuf(void) { return rbSerBuf->getFree(); }
        int getNumCmds(void) { return rbCmdLenBuf->getOccupied(); }
        int getFreeCmdLenBuf(void) { return rbCmdLenBuf->getFree(); }
        int readCmd(char * dest);
        size_t write(char * data, size_t n);
        
        
    private:
        
        // Holds the pre-parsed commands
        ringbuffer<cmdItem>* rbCmdBuffer;   
        
        // Private utility members
        int getNumDelims(char * data, int len);

        /*
         * Stuff to get rid of...
         */ 
        ringbuffer<char>* rbSerBuf;     // Holds serial stream of commands (incl. delimiters)
        ringbuffer<int>* rbCmdLenBuf;   // Holds length of each command in rbSerBuff
        

}; // class cmdBuffer

#endif // CMD_BUFFER_H
