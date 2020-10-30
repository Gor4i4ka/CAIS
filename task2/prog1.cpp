#include <fstream>
#include <iomanip>
#include <iostream>
#include <string.h>
#include "pin.H"


VOID Key_extract(INT64 len, char* key_p) {

        FILE* f = fopen("key.data", "w");
        fwrite(key_p, sizeof(char), len, f);
        fclose(f);
}


VOID Routine(RTN rtn, VOID *v)
{
        RTN_Open(rtn);
        string name = RTN_Name(rtn);
        if (name == "BF_set_key") {
                RTN_InsertCall(rtn, IPOINT_BEFORE, (AFUNPTR)Key_extract,
                        IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
                        IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
                        IARG_END);
        }
        RTN_Close(rtn);
}


/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    cerr << "This Pintool gets BF_key_edi data param" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */

int main(int argc, char * argv[])
{

    // Initialize symbol table code, needed for rtn instrumentation
    PIN_InitSymbols();

    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

    // Register Routine to be called to instrument rtn
    RTN_AddInstrumentFunction(Routine, 0);

    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}
