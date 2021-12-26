/*
    Member functions for CmdBuffer class.
*/

#include <cmdBuffer.h>

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

// TODO needs to update rbCmdLenBuf state as well, maybe replace with a flush instead of read as this isn't particularily useful.
size_t CmdBuffer::readToEnd(char * dest)
{
    size_t n = rbSerBuf->getOccupied();
    if (n > 0) 
    {
        rbSerBuf->read(dest, n);
    }
    return n;
}

size_t CmdBuffer::write(char * data, size_t n)
{
    
    static int i;
    static int prevCmdEnd;
    static int scratch;

    // Copy data to rbCmdBuf if there's room for:
    //   - data in rbSerBuf
    //   - command lengths in rbCmdLenBuf
    if( rbSerBuf->getFree() >= n && 
        rbCmdLenBuf->getFree() >= CmdBuffer::getNumDelims(data, n))
    {
        rbSerBuf->write(data, n);
        // Check data for are any command delimiters
        for (i = 0, prevCmdEnd = 0, scratch = 0; i < n; i++)
        {
            if (data[i] == CmdBuffer::delim)
            {
                scratch = i - prevCmdEnd;
                rbCmdLenBuf->write(&scratch, 1);
                prevCmdEnd = i;
            } 
        }
        return n;
    } 
    else
    {
        return 0;
    }
    
}