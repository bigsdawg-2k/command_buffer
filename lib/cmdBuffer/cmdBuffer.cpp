/*
    Member functions for CmdBuffer class.
*/

#include <ringbuffer.h>
#include <cmdBuffer.h>

// Constructor
CmdBuffer::CmdBuffer(size_t size)
{
    // Buffer that will be used to store commands received over BT
    rbSerBuf = new ringbuffer<uint8_t>(size);
    rbCmdLenBuf = new ringbuffer<int>(size);
}

int CmdBuffer::readCmd(uint8_t * dest)
{
    if (rbCmdLenBuf->getOccupied() > 0)
    {
        int size;
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
size_t CmdBuffer::readToEnd(uint8_t * dest)
{
    size_t n = rbSerBuf->getOccupied();
    if (n > 0) 
    {
        rbSerBuf->read(dest, n);
    }
    return n;
}

size_t CmdBuffer::write(uint8_t * data, size_t n)
{
    
    static int i = 0;
    static int prevCmdEnd = 0;
    static int scratch = 0;

    // Copy data to rbCmdBuf if there's room, otherwise return immediately
    if(rbSerBuf->getFree() >= n && rbCmdLenBuf->getFree() >= n)
    {
        rbSerBuf->write(data, n);
        // Check data for are any command delimiters
        for (; i < n; i++)
        {
            if (data[i] == ';')
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