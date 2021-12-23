#include <unity.h>
#include <cmdBuffer.h>

#define NUM_BYTES_SERIAL_BT_BUFF 1024

CmdBuffer* cmdBuf = new CmdBuffer(NUM_BYTES_SERIAL_BT_BUFF);

void test_add_single_command_to_buffer() {
    
    uint8_t data[10] = "P0;";

    cmdBuf->write( (uint8_t*) &data, 3 );
    
    TEST_ASSERT_TRUE(cmdBuf->getNumCmds() == 1);
}

void setUp(void) { }
void tearDown(void) { }

int main( int argc, char **argv) {
    
    UNITY_BEGIN();
    
    RUN_TEST(test_add_single_command_to_buffer);

    UNITY_END();

}