#include <stdio.h>

#include "bstr.h"

#include "comp.h"


void bitStringTest(void) {

    BitString bStr;
    BitString bStr1;
    BitString bStr2;
    BitString bStr3;
    BitString bStr4;

    int testValues[] = { 255, -255, -83, 83 };
    int i;
    for (i = 0; i < 4; i++) {

        BSTR_SetValueTwosComp(&bStr, testValues[i], 16);
        BSTR_Display(bStr, 1);
        printf("\n");
        printf("Value = %d\n", BSTR_GetValueTwosComp(bStr));
    }


    BSTR_SetValue(&bStr1, 4, 8);
    BSTR_SetValue(&bStr2, 10, 8);
    BSTR_Display(bStr1, 1);
    printf("\n");
    BSTR_Display(bStr2, 1);
    printf("\n");
    printf("Length = %d\n", BSTR_Length(bStr1));
    printf("Length = %d\n", BSTR_Length(bStr2));
    BSTR_Append(&bStr3, bStr1, bStr2);
    printf("Length = %d\n", BSTR_Length(bStr3));
    BSTR_Display(bStr3, 1);
    printf("\n");

    BSTR_Substring(&bStr4,bStr3,4,8);
    BSTR_Display(bStr4, 1);
    printf("\n");
}


void testNot() {/* NOT test */
    printf("\n\n\t\t####  Starting NOT test  ####\n");

    /* Initialize computer */
    Computer comp;
    COMP_Init(&comp);

    /* Create desired instructions */
    BitString notInstr;
    BSTR_SetBits(&notInstr, "1001100101111111"); // NOT Dest: R4, Source: R5

    /* Load desired instructions */
    COMP_LoadWord(&comp,0,notInstr);

    /* Display initialized computer and loaded instructions */
    printf("\t####  Displaying computers initial state  ####\n");
    COMP_Display(comp);

    /* Execute all loaded instructions */
    COMP_Execute(&comp);

    /* Display state of computer after execution */
    printf("\n\n\t####  Displaying computers state after execution ####\n");
    COMP_Display(comp);

    printf("\n\t\t####  Finished NOT test  ####\n\n");
    printf("Verify R4 <-- NOT(R5)\n");
    printf("R4 Value = %d\n", BSTR_GetValueTwosComp(comp.reg[4]));
    printf("Verify CC n,z,p = 100");
}

/**
 * Tests register mode of the ADD instruction using the following
 * formula for the given values a and b stored respectively in R0 and R1.
 *
 * c = a + b
 * R2 <- R0 + R1
 *
 * @param a 16 bit signed integer value to be added
 * @param b 16 bit signed integer value to be added
 */
void testAddRegisterMode(int a, int b) {
    printf("\n\n\t\t####  Starting ADD - Register Mode test  ####\n");

    /* Initialize computer */
    Computer comp;
    COMP_Init(&comp);

    /* Create desired instructions */
    BitString addInstr;                         // R2 <-- R0 + R1
    BSTR_SetBits(&addInstr, "0001010000000001"); // ADD Dest: R2, Src1: R0 Src2: R1

    /* Load desired instructions */
    COMP_LoadWord(&comp, 0, addInstr);

    /* Setup R0 and R1 with the given value of a */
    BSTR_SetValueTwosComp(&(comp.reg[0]), a, 16); // R0 <-- a
    BSTR_SetValueTwosComp(&(comp.reg[1]), b, 16); // R1 <-- b

    /* Display initialized computer and loaded instructions */
    printf("\t####  Displaying computers initial state  ####\n");
    COMP_Display(comp);

    /* Execute all loaded instructions */
    COMP_Execute(&comp);

    /* Display state of computer after execution */
    printf("\n\n\t####  Displaying computers state after execution ####\n");
    COMP_Display(comp);

    printf("\n\t\t####  Finished ADD - Register Mode test  ####\n\n");
    printf("Verify R2 <-- R0 + R1)\n");
    printf("Verify R2 <-- %d + %d)\n", a, b);
    printf("R2 Value = %d\n", BSTR_GetValueTwosComp(comp.reg[2]));
    printf("Verify CC n,z,p for the given stored result\n");
}

/***
 * Tests immediate mode of the ADD instruction using the following
 * formula for the given values a and b stored respectively in R0 and b
 * where b is a literal value to be added to a and stored in R1
 *
 * c = a + b
 * R1 <- R0 + b
 *
 * @param a 16 bit signed integer value to be added
 * @param b 5 bit signed integer value to be added
 */
void testAddImmediateMode(int a, int b) {
    printf("\n\n\t\t####  Starting ADD - Immediate Mode test  ####\n");

    /* Initialize computer */
    Computer comp;
    COMP_Init(&comp);

    /* Create desired instructions */
    BitString addInstrComplete; // R1 <-- R0 + b
    BitString addInstr;         // R1 <-- R0 + imm5
    BitString imm5BS;           // 5 bit 2's comp binary number
    BSTR_SetValueTwosComp(&imm5BS, b, 5);
    BSTR_SetBits(&addInstr, "00010010001"); // ADD Dest: R1, Src1: R0 Imm5: b
    BSTR_Append(&addInstrComplete, addInstr, imm5BS);

    /* Load desired instructions */
    COMP_LoadWord(&comp, 0, addInstrComplete);

    /* Setup R0 with the given value of a */
    BSTR_SetValueTwosComp(&(comp.reg[0]), a, 16); // R0 <-- a

    /* Display initialized computer and loaded instructions */
    printf("\t####  Displaying computers initial state  ####\n");
    COMP_Display(comp);

    /* Execute all loaded instructions */
    COMP_Execute(&comp);

    /* Display state of computer after execution */
    printf("\n\n\t####  Displaying computers state after execution ####\n");
    COMP_Display(comp);

    printf("\n\t\t####  Finished ADD - Immediate Mode test  ####\n\n");
    printf("Verify R1 <-- R0 + b)\n");
    printf("Verify R1 <-- %d + %d)\n", a, b);
    printf("R1 Value = %d\n", BSTR_GetValueTwosComp(comp.reg[1]));
    printf("Verify CC n,z,p for the given stored result\n");
}


/***
 * Tests the load instruction
 * given a destination register and a 9bit signed PCoffset
 *
 * @param dest_register 3 bit int in the range of 1-7 representing the Destination.
 * @param offset 9 bit int representing the memory address to fetch the data to be loaded.
 * @param initialPC 16 bit int representing the desired initial PC value.
 */
void testLdMode(int dest_register, int offset, int initialPC) {

    printf("\n\n\t\t####  Starting LD test  ####\n");

    /* Initialize computer */
    Computer comp;
    COMP_Init(&comp);

    BSTR_SetValue(&(comp.pc),initialPC,16);

    /* Load some interesting things into the all of the memory registers */
    int i;
    for (i = 0; i < 50; i++) {
        if (i % 2) {
            BSTR_SetValueTwosComp(&(comp.mem[i]), i, 16);
        } else {
            BSTR_SetValueTwosComp(&(comp.mem[i]), i * -1, 16);
        }
    }

    /* Create desired instruction set */
    BitString ldInstr;
    BSTR_SetBits(&ldInstr, "0010");         // LD Dest: R0
    /* Create dest_register bitstring for the desired storage address */
    BitString address;
    BSTR_SetValue(&address, dest_register, 3);
    /* Create the offset from offset */
    BitString pcOffset;                     // 9 bit 2's comp binary number
    BSTR_SetValueTwosComp(&pcOffset, offset, 9);
    /* Create the full instruction needed for LD operation */
    BitString ldInstrComplete;              // dest_register <-- M[PC + PCoffset9]
    BSTR_Append(&ldInstrComplete, ldInstr, address);
    BSTR_Append(&ldInstrComplete, ldInstrComplete, pcOffset);

    /* Load desired instruction */
    COMP_LoadWord(&comp, initialPC, ldInstrComplete);

    /* Display initialized computer and loaded instructions */
    printf("\t####  Displaying computers initial state  ####\n");
    COMP_Display(comp);

    /* Execute all loaded instructions */
    COMP_Execute(&comp);

    /* Display state of computer after execution */
    printf("\n\n\t####  Displaying computers state after execution ####\n");
    COMP_Display(comp);

    printf("\n\t\t####  Finished LD test  ####\n\n");
    printf("Verify R_a <-- PC + offset)\n");
    printf("Verify R_%d <-- ", dest_register); //%d + %d)\n", offset);
    printf("1 + %d\n", offset);
}


int main(int argc, const char * argv[]) {

    /* Tests the NOT instruction */
    //    testNot();

    /* Test the ADD instruction in Register mode with two positive numbers */
//    testAddRegisterMode(5, 5);

    /* Test the ADD instruction in Register mode with two negative numbers */
//    testAddRegisterMode(-5, -5);

    /* Test the ADD instruction in Register mode with a negative and positive number
     * result is negative  */
//    testAddRegisterMode(-5, 2);

    /* Test the ADD instruction in Register mode with a negative and positive number
     * result is positive  */
//    testAddRegisterMode(-5, 10);

    /* Test the ADD instruction in Register mode with a negative and positive number
     * result is zero  */
//    testAddRegisterMode(-5, 5);


    /* Test the ADD instruction in Immediate mode with two positive numbers */
//    testAddImmediateMode(5, 5);

    /* Test the ADD instruction in Immediate mode with two negative numbers */
//    testAddImmediateMode(-5, -5);

    /* Test the ADD instruction in Immediate mode with a negative and positive number
     * result is negative  */
//    testAddImmediateMode(-5, 2);

    /* Test the ADD instruction in Immediate mode with a negative and positive number
     * result is positive  */
//    testAddImmediateMode(-5, 10);

    /* Test the ADD instruction in Immediate mode with a negative and positive number
     * result is zero  */
//    testAddImmediateMode(-5, 5);



    /* Test LD instruction */

    int destRegister, pcOffset, initialPC;

    /* Test LD into register 0 with no offset and PC at 0 */
    // R1 <-- M[PC + 1 + PCOffset]
    destRegister = 0;   // Load into
    pcOffset = 0;       // This number of steps ahead
    initialPC = 0;      // Starting at PC + 1
    testLdMode(destRegister, pcOffset, initialPC);


    /* Test LD into register 0 with positive offset and PC at 0 */
    // R1 <-- M[PC + 1 + PCOffset]
    destRegister = 0;   // Load into
    pcOffset = 10;       // This number of steps ahead
    initialPC = 0;      // Starting at PC + 1
    testLdMode(destRegister, pcOffset, initialPC);


    /************************************** */
/** The next two variables - program and programSize - */
/** allow someone using the simulator (such as a grader) */
/** to decide what program will be simulated. */
/** The simulation must load and execute */
/** instructions found in the "program" array. */
/** For grading purposes, it must be possible for me to */
/**    - paste in a different set of binary strings to replace the existing ones */
/**    - adjust the programSize variable */
/**    - recompile your program without further changes */
/**    and see the simulator load and execute the new program. */
/**    Your grade will depend largely on how well that works. */
/************************************** */

//    char* program[] =
//    {
//     "0010000000000111",
//     "0010001000000111",
//     "0001010000000001",
//     "0000010000000011",
//     "1111000000100001",
//     "0001000000111111",
//     "0000111111111011",
//     "1111000000100101",
//     "0000000000111001",
//     "1111111111010000"
//	 };
//
//    int programSize = 10;

//	BitString addInstr;

    /*  You may find it useful to play around with the */
    /*  BitString routines.  Some code that does that is provided */
    /*  in the following commented out function. */

//     bitStringTest();

/*   This is the assembly program that was compiled into the binary
     program shown above.
    .ORIG x3000

            LD   R0  START
            LD   R1  END
     TOP    ADD  R2  R0  R1
            BRZ  DONE
            OUT
            ADD  R0 R0 -1
            BRNZP TOP
     DONE   HALT

     START .FILL x39
     END   .FILL x-30

     .END
*/


//    BitString demoStr, sub;
//
//	//BSTR_SetValue(&demoStr,15,8);
//	BSTR_SetBits(&demoStr,"0000000000001111");
//
//	BSTR_Display(demoStr,1);
//
//	BSTR_Substring(&sub,demoStr,2,4);
//	BSTR_Display(sub,1);
//
//	COMP_Init(&comp);
//	COMP_Display(comp);   /* displays computer's initial configuration
//
//	/* TO DO: load the instructions in the "program" array */
//
//	/* Next 3 lines are a test of NOT */
//	/* Once you are confident that single instructions work, you will */
//	/* want to replace this with code that loads all the instructions */
//	/* from the array shown above. */
//
//	/* following not instruction is   NOT Dest: R4, Source: R5 */
//    BitString addInstr;
//	BSTR_SetBits(&addInstr,"1001100101111111");
//	COMP_LoadWord(&comp,0,addInstr);
//    COMP_Display(comp);
//
//	/* execute the program */
//	/* During execution, the only output to the screen should be */
//	/* the result of executing OUT. */
//	COMP_Execute(&comp);
//
//	/* shows final configuration of computer */
//	COMP_Display(comp);

}





