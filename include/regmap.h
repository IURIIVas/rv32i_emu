#ifndef _REGMAP_H_
#define _REGMAP_H_

#define REGS_NUM                (32)

enum gpr_basic_names_e {
        X0 = 0,
        X1 = 1,
        X2 = 2,
        X3 = 3,
        X4 = 4,
        X5 = 5,
        X6 = 6,
        X7 = 7,
        X8 = 8,
        X9 = 9,
        X10 = 10,
        X11 = 11,
        X12 = 12,
        X13 = 13,
        X14 = 14,
        X15 = 15,
        X16 = 16,
        X17 = 17,
        X18 = 18,
        X19 = 19,
        X20 = 20,
        X21 = 21,
        X22 = 22,
        X23 = 23,
        X24 = 24,
        X25 = 25,
        X26 = 26,
        X27 = 27,
        X28 = 28,
        X29 = 29,
        X30 = 30,
        X31 = 31
};

enum gpr_abi_names_e {
        ZERO = 0,       /* hardwired zero */
        RA = 1,         /* return address */
        SP = 2,         /* stack pointer */
        GP = 3,         /* global pointer */
        TP = 4,         /* thread pointer */
        T0 = 5,         /* tmp reg 0 */
        T1 = 6,         /* tmp reg 1 */
        T2 = 7,         /* tmp reg 2 */
        S0 = 8,         /* saved reg 0 */
        FP = 8,         /* frame pointer */
        S1 = 9,         /* saved reg 1 */
        A0 = 10,        /* function argument 0 / return value 0 */
        A1 = 11,        /* function argument 1 / return value 1 */
        A2 = 12,        /* function argument 2 */
        A3 = 13,        /* fucntion argument 3 */
        A4 = 14,        /* fucntion argument 4 */
        A5 = 15,        /* fucntion argument 5 */
        A6 = 16,        /* fucntion argument 6 */
        A7 = 17,        /* fucntion argument 7 */
        S2 = 18,        /* saved reg 2 */
        S3 = 19,        /* saved reg 3 */
        S4 = 20,        /* saved reg 4 */
        S5 = 21,        /* saved reg 5 */
        S6 = 22,        /* saved reg 6 */
        S7 = 23,        /* saved reg 7 */
        S8 = 24,        /* saved reg 8 */
        S9 = 25,        /* saved reg 9 */
        S10 = 26,       /* saved reg 10 */
        S11 = 27,       /* saved reg 11 */
        T3 = 28,        /* tmp reg 3 */
        T4 = 29,        /* tmp reg 4 */
        T5 = 30,        /* tmp reg 5 */
        T6 = 31         /* tmp reg 6 */
};

#endif