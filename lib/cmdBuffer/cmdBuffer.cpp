#include <cmdBuffer.h>
#include <string>

// Constructor
CmdBuffer::CmdBuffer(size_t size)
{
    rbCmdBuf = new ringbuffer<cmdItem>(size);
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
size_t CmdBuffer::readCmd(cmdItem* dest)
{
    if (rbCmdBuf->getOccupied() > 0)
    {
        return rbCmdBuf->read(dest, 1);
    }
    
    return 0; 
    
}

// Sanitize a command received for the buffer.  
// Command formats are:
// <module_name><instruction>[<nArgs><n*args>]<delimiter>, where 
// all fields excluding module_name and delimiter are 2-digit hex values
// Returns: 
cmdItem* CmdBuffer::parseCmd(cmdItem* cmd, char * data, size_t n)
{
    static int i, iArgs, intField;
    static char strField[3] = {};
    
    // First field is always the target module identifier
    switch (data[0])
    {
        case 'L':
            cmd->moduleTarget = 1;
            break;
        default:
            cmd->moduleTarget = 0;
            break;
    }
    
    // All remaining fields are 2-digit hex numbers
    strField[2] = 0;
    for (i = 1, iArgs = 0; i < n - 1; i += 2)
    {
        strncpy(strField, data + i, 2);
        intField = std::stoi(strField, nullptr, 16);
        if (i == 1)
        {
            cmd->instruction = intField;
        }
        // Number of optional arguments that follow
        else if (i == 3)
        {
            cmd->vArg = (int*) malloc(sizeof(int) * intField);
            cmd->vArgLen = intField;
        }
        // Remaining (optional) arguments
        else 
        {
            cmd->vArg[iArgs++] = intField;
        }
    }

    return cmd;

}

// Parse the command string and write commands to the buffer provided there is space
// Returns: Number of commands written to the buffer
size_t CmdBuffer::writeCmdMsg(char* data, size_t n)
{
    
    static int i, nCmds;
    static int thisCmdStart;
    static cmdItem thisCmd;
    
//TODO make sure there's room for the command(s)

    // Loop over the whole message
    for(i = 0, nCmds = 0, thisCmdStart = 0; i < n; i++)
    {
        if (data[i] == CmdBuffer::delim) 
        { 
            CmdBuffer::parseCmd(&thisCmd, &data[thisCmdStart], i - thisCmdStart + 1);
            rbCmdBuf->write(&thisCmd, 1);
            nCmds++; 
        }
    }

    return nCmds;
    
}