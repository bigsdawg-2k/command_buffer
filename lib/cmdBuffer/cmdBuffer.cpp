/*
    Member functions for CmdBuffer class.
*/

#include <cmdBuffer.h>

//TODO: Needs updating to use cmdItem structuring
// Constructor
CmdBuffer::CmdBuffer(size_t size)
{
    // Buffer that will be used to store commands received over BT
    rbSerBuf = new ringbuffer<char>(size);
    rbCmdLenBuf = new ringbuffer<int>(size);
}

// Get the number of delimiters in a sequence of characters
int CmdBuffer::getNumDelims(char * data, int len)
{
    static int i, count;
    
    for(i = 0, count = 0; i < len; i++)
    {
        if (data[i] == CmdBuffer::delim) { count++; }
    }
    return count;
}

//TODO: Needs updating to use cmdItem structuring
// Get the next command and save it in dest.  Return the size of the command read.
int CmdBuffer::readCmd(char * dest)
{
    static int size;
    if (rbCmdLenBuf->getOccupied() > 0)
    {
        rbCmdLenBuf->read(&size, 1);
        rbSerBuf->read(dest, size);
        return size;
    }
    else
    {
        return 0; 
    }
}

// TODO to be replaced with writeCmdMsg()
size_t CmdBuffer::write(char * data, size_t n)
{
    
    static int i;
    static int thisCmdStart;
    
    // Copy data to rbCmdBuf if there's room for:
    //   - data in rbSerBuf
    //   - command lengths in rbCmdLenBuf
    if( rbSerBuf->getFree() >= n && 
        rbCmdLenBuf->getFree() >= CmdBuffer::getNumDelims(data, n))
    {
        rbSerBuf->write(data, n);
        // Check data for are any command delimiters
        for (i = 0, thisCmdStart = 0; i < n; i++)
        {
            if (data[i] == CmdBuffer::delim)
            {
                thisCmdStart = i - thisCmdStart + 1;  // temporarily used to hold size of command for buffer write
                rbCmdLenBuf->write(&thisCmdStart, 1);
                thisCmdStart = i + 1;
            } 
        }
        return n;
    } 
    else
    {
        return 0;
    }
    
}