#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cmdBuffer.h>
#include <time.h>

using std::cout;
using std::endl;

#define NUM_BUF_ITEMS           20
#define MAX_LOG_MSG             1024
#define NUM_TESTS               2
#define CMD_MESSAGE_HDR_LEN     5

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
 * TEST CASE 2: Variable Argument Size 
 * Create a small cmdBuffer
 * Add command with variable length argument
 * Read the command and verify the vArgs and vArgLen
 */
bool tc2_vArgTest(char * msgTestResult) 
{

    #define TC2_MAX_N_ARGS          32
    int tc_nArgs, tc_int, tc_cmdMsgLen, i;
    bool tc_result = true;
        
    // Determine number of vArgs
    srand(time(0));
    tc_nArgs = rand()%(TC2_MAX_N_ARGS - 1) + 1;

    // Randomly generate vArgs
    int tc_randArgs[tc_nArgs];
    for (i = 0; i < tc_nArgs; i++)
    {
        tc_randArgs[i] = rand()%256;
    }

    // Create command message
    char * tc_cmdMsg = new char[CMD_MESSAGE_HDR_LEN + 5 + TC2_MAX_N_ARGS * 2]();
    sprintf(tc_cmdMsg, "L01%02X", tc_nArgs);
    for(i = 0; i <= tc_nArgs + 1; i++)    
    {
        if (i < tc_nArgs)
        {
            sprintf(&tc_cmdMsg[CMD_MESSAGE_HDR_LEN + i * 2], "%02X", tc_randArgs[i]);
        }
        else if (i == tc_nArgs)
        {
            tc_cmdMsg[CMD_MESSAGE_HDR_LEN + i * 2] = ';';
        }
        else
        {
            tc_cmdMsg[CMD_MESSAGE_HDR_LEN + i * 2] = NULL;
        }
    }
    cout << "Generated command message: " << tc_cmdMsg << endl;

    // Add message to buffer
    cout << "Writing to buffer..." << endl;
    CmdBuffer* cmdBuf;
    cmdBuf = new CmdBuffer(NUM_BUF_ITEMS);
    tc_cmdMsgLen = strlen(tc_cmdMsg);
    tc_int = cmdBuf->writeCmdMsg(tc_cmdMsg, tc_cmdMsgLen);

    // Read message from buffer
    cout << "Reading from buffer..." << endl;
    cmdItem tc_cmdItem;
    tc_int = cmdBuf->readCmd(&tc_cmdItem);
    
    // Compare result
    cout << "  Validating result..." << endl;
    if (tc_nArgs != tc_cmdItem.vArgLen)
    {
        cout << "    Expected (" << tc_nArgs << ") arguments, Received (" << tc_cmdItem.vArgLen << ")" << endl;
        tc_result = false;
    } 
    for (i = 0; i < tc_nArgs; i++)
    {
        if(tc_randArgs[i] != tc_cmdItem.vArg[i])
        {
            cout << "    Expected (" << tc_randArgs[i] << "), Received (" << tc_cmdItem.vArg[i] << ")" << endl;
            tc_result = false;
        }
    }
    
    return tc_result;

} // TC2

/*
 * TEST CASE 2: Variable Argument Size 
 * Create a small cmdBuffer
 * Add command with variable length argument
 * Read the command and verify the vArgs and vArgLen
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


    testResults[1] = tc2_vArgTest(tc_resultMessage);   
    printTcResult(testResults[1], 2, tc_resultMessage);
/*    
    testResults[2] = tc3_addMultipleCommands(tc_resultMessage);   
    printTcResult(testResults[2], 3, tc_resultMessage);
*/

}