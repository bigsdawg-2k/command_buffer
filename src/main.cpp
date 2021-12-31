#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cmdBuffer.h>

using std::cout;
using std::endl;

#define NUM_BUF_ITEMS               64
#define MAX_LOG_MSG                 1024
#define NUM_TESTS                   2

char * tc_tmpMsg = new char[1024]();

/* 
 * Print a test result
 */
void printTcResult(bool result, int tcNum, char * msgResult)
{
    
    if(result == false)
    {
        cout << "TC" << tcNum << " failed" << endl;
    }
    else
    {
        cout << "TC" << tcNum << " passed" << endl;
    }

    cout << msgResult << endl << endl;
    
}

/*
 * TEST CASE 1: Basic Functionality
 * Create a cmdBuffer
 * Add a command
 * Check the buffer state
 * Get the command
 * Check the buffer state
 */
bool tc1_basicFunctionality(char * msgTestResult) 
{

    #define TC1_COMMAND                     "L02;"
    #define TC1_NUM_BYTES_SERIAL_BT_BUFF    NUM_BYTES_SERIAL_BT_BUFF

    // Main command buffer for testing
    CmdBuffer* cmdBuf;
    cmdBuf = new CmdBuffer(NUM_BUF_ITEMS);

    // TC variables
    bool tc_result = true;
    int tc_int, tc_CmdArrLen;
    char * tc_cmdArr = new char[64]();
    char * tc_charArr = new char[64]();
    cmdItem tc_cmdItem;

    strcpy(tc_cmdArr, TC1_COMMAND);
    tc_CmdArrLen = strlen(TC1_COMMAND);

    // Check that the newly created object has the correct initial state
    // The new buffer was created in the setUp()
    if(cmdBuf->getOccupied() != 0 ||
        cmdBuf->getFree() != NUM_BUF_ITEMS)
    {
        cout << "getOccupied() or getFree() incorrectly initialized";
        tc_result = false;
    }
    
    // Add a command to the buffer
    tc_int = cmdBuf->writeCmdMsg(tc_cmdArr, tc_CmdArrLen);
    if(tc_int != 1) 
    { 
        cout << "cmdBuf->writeCmdMsg() returned " << tc_int << ", expected " << 1 << endl; 
        tc_result = false;
    }
    
    // Check that there is an item in the buffer
    tc_int = cmdBuf->getOccupied();
    if (tc_int != 1)
    { 
        cout << "cmdBuf->getOccupied() returned " << tc_int << ", expected " << 1 << endl;
        tc_result = false;
    }
    tc_int = cmdBuf->getFree();
    if (tc_int != NUM_BUF_ITEMS - 1)
    {
        cout << "cmdBuf->getFree() returned " << tc_int << ", expected " << NUM_BUF_ITEMS - 1 << endl;
        tc_result = false;
    }
    
    // Read the command from the buffer
    tc_int = cmdBuf->readCmd(&tc_cmdItem);
    if (tc_int != 1 ||
        tc_cmdItem.instruction != 2 ||
        tc_cmdItem.moduleTarget != 1 ||
        tc_cmdItem.vArgLen != 0 ||
        cmdBuf->getOccupied() != 0 ||
        cmdBuf->getFree() != NUM_BUF_ITEMS)

    {
        cout << "cmdBuf->readCmd() returned " << tc_int << " commands, expected " << 1 << endl;
        cout << "cmdItem has moduleTarget " << tc_cmdItem.moduleTarget << ", expected " << 1 << endl;
        cout << "cmdItem has instruction " << tc_cmdItem.instruction << ", expected " << 2 << endl;
        cout << "cmdItem has vArgLen " << tc_cmdItem.vArgLen << ", expected " << 0 << endl;
        tc_result = false;
    }

    return tc_result;

} // TC1

/*
 * TEST CASE 2: Buffer overflow test
 * Create a small cmdBuffer
 * Add commands until it is full
 * Check the buffer state
 * Read out a command
 * Add commands until full
 * Read out all commands
 * Check the buffer state
 */
/*bool tc2_bufferFill(char * msgTestResult) 
{

    #define TC2_NUM_BYTES_SERIAL_BT_BUFF    20
    #define TC2_CMD_TEMPLATE                "P#;"
        
    strcpy(msgTestResult, "TC2: Buffer fill test...\n");

    // Main command buffer for testing
    CmdBuffer* cmdBuf;
    cmdBuf = new CmdBuffer(TC2_NUM_BYTES_SERIAL_BT_BUFF);

    // TC variables
    bool tc_result = true;
    int tc_int, tc_CmdArrLen, i;
    char * tc_cmd = new char[64]();
    char * tc_ref = new char[64]();

    // Fill the buffer   
    strcat(msgTestResult, "  Starting buffer fill through writes...\n");
    for(i = 0, tc_int = 1; tc_int > 0; i++)
    {
        sprintf(tc_cmd, "P%d;", i);
        tc_CmdArrLen = strlen(tc_cmd);
        tc_int = cmdBuf->write(tc_cmd, tc_CmdArrLen);
    }

    // Check the buffer state
    strcat(msgTestResult, "  Checking buffer state...\n");
    tc_int = TC2_NUM_BYTES_SERIAL_BT_BUFF - (strlen(TC2_CMD_TEMPLATE)*(TC2_NUM_BYTES_SERIAL_BT_BUFF/strlen(TC2_CMD_TEMPLATE)));
    if(cmdBuf->getFreeSerBuf() != tc_int) 
    {
        sprintf(tc_tmpMsg, "    Serial buffer free: %d, Needed %d\n", cmdBuf->getFreeSerBuf(), tc_int);
        strcat(msgTestResult, tc_tmpMsg);
        tc_result = false;
    }
    
    // Read out one command
    tc_int = cmdBuf->readCmd(tc_cmd);
    if (tc_int != sizeof("P0;") - 1)
    {
        sprintf(tc_tmpMsg, "    Read command of size (%d), Expected (%d)\n", tc_int, sizeof("P0;") - 1);
        strcat(msgTestResult, tc_tmpMsg);
        tc_result = false;
    }   

    if (strcmp(tc_cmd, "P0;") != 0)
    {
        sprintf(tc_tmpMsg, "    Read back: (%s), Expected (%s)\n", tc_cmd, "P0;");
        strcat(msgTestResult, tc_tmpMsg);
        tc_result = false;
    }   
    
    // Refill the buffer
    strcat(msgTestResult, "  Starting buffer refill through writes...\n");
    for(i = cmdBuf->getNumCmds() + 1, tc_int = 1; tc_int > 0; i++)
    {
        sprintf(tc_cmd, "P%d;", i);
        tc_CmdArrLen = strlen(tc_cmd);
        tc_int = cmdBuf->write(tc_cmd, tc_CmdArrLen);
    }

    // Check the buffer state
    strcat(msgTestResult, "  Checking buffer state after refill...\n");
    tc_int = TC2_NUM_BYTES_SERIAL_BT_BUFF - (strlen(TC2_CMD_TEMPLATE)*(TC2_NUM_BYTES_SERIAL_BT_BUFF/strlen(TC2_CMD_TEMPLATE)));
    if(cmdBuf->getFreeSerBuf() != tc_int) 
    {
        sprintf(tc_tmpMsg, "    Serial buffer free: %d, Needed %d\n", cmdBuf->getFreeSerBuf(), tc_int);
        strcat(msgTestResult, tc_tmpMsg);
        tc_result = false;
    }

    // Read out all of the buffer's commands
    strcat(msgTestResult, "  Starting buffer emptying through reads...\n");
    for(i = 1, tc_int = 1; tc_int > 0; i++)
    {
        sprintf(tc_ref, "P%d;", i);
        tc_int = cmdBuf->readCmd(tc_cmd);
        if( tc_int > 0 && strcmp(tc_cmd, tc_ref) != 0)
        {
            sprintf(tc_tmpMsg, "    Read back: (%s), Expected (%s)\n", tc_cmd, tc_ref);
            strcat(msgTestResult, tc_tmpMsg);
            tc_result = false;
        }
    }

    // Check the buffer state
    strcat(msgTestResult, "  Checking buffer state after emptying...\n");
    tc_int = cmdBuf->getNumCmds();
    tc_int = cmdBuf->getFreeSerBuf();
    tc_int = cmdBuf->getOccupied();
    if(cmdBuf->getNumCmds() != 0 ||
        cmdBuf->getFreeSerBuf() != TC2_NUM_BYTES_SERIAL_BT_BUFF || 
        cmdBuf->getOccupied() != 0)
    {
        strcat(msgTestResult, "    getNumCmds(), getFree(), or getOccupied() incorrectly initialized");
        tc_result = false;
    }

    return tc_result;

} // TC2
*/

/*
 * TEST CASE 3: Multiple command add 
 */
/*bool tc3_addMultipleCommands(char * msgTestResult) 
{
    #define TC3_NUM_BYTES_SERIAL_BT_BUFF    20
    #define TC3_CMD                "P0;P1;P2;"
        
    strcpy(msgTestResult, "TC3: Add multiple commands test (in one message)...\n");
    
    // Main command buffer for testing
    CmdBuffer* cmdBuf;
    cmdBuf = new CmdBuffer(TC3_NUM_BYTES_SERIAL_BT_BUFF);

    // TC variables
    bool tc_result = true;
    int tc_int, tc_CmdArrLen, i;
    char * tc_cmd = new char[64]();
    char * tc_ref = new char[64]();

    strcpy(tc_cmd, TC3_CMD);
    
    // Add the message with multiple commands
    strcat(msgTestResult, "  Starting message write...\n");
    tc_int = cmdBuf->write(tc_cmd, strlen(tc_cmd));
    if(tc_int != strlen(tc_cmd))
    {
        sprintf(tc_tmpMsg, "    Serial buffer received %d bytes, expected %d\n", tc_int, strlen(tc_cmd));
        strcat(msgTestResult, tc_tmpMsg);
        tc_result = false;
    }

    // Read out all of the commands
    strcat(msgTestResult, "  Starting buffer emptying through reads...\n");
    for(i = 0, tc_int = 1; i < 3; i++)
    {
        sprintf(tc_ref, "P%d;", i);
        tc_int = cmdBuf->readCmd(tc_cmd);
        tc_cmd[tc_int] = 0;
        if(tc_int <= 0)
        {
            strcat(msgTestResult, "    Expected more commands, but nothing was returned\n");
            tc_result = false;
        }
        else if(strcmp(tc_cmd, tc_ref) != 0)
        {
            sprintf(tc_tmpMsg, "    Read back: (%s), Expected (%s)\n", tc_cmd, tc_ref);
            strcat(msgTestResult, tc_tmpMsg);
            tc_result = false;
        }
    }

    return tc_result;
    
}
*/

// Stores test results
bool * testResults = new bool[NUM_TESTS]();
char * tc_resultMessage = new char[1024]();

int main(void) 
{
    
    testResults[0] = tc1_basicFunctionality(tc_resultMessage);   
    printTcResult(testResults[0], 1, tc_resultMessage);

/*
    testResults[1] = tc2_bufferFill(tc_resultMessage);   
    printTcResult(testResults[1], 2, tc_resultMessage);
    
    testResults[2] = tc3_addMultipleCommands(tc_resultMessage);   
    printTcResult(testResults[2], 3, tc_resultMessage);
*/

}